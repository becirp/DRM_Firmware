#include "main.h"
#include "bios.h"
#include "fatfs.h"
#include "stdio.h"
#include "string.h"

#include "comm.h"
#include "lang.h"
#include "bios.h"

/* Private variables ---------------------------------------------------------*/

SD_HandleTypeDef hsd;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
UART_HandleTypeDef huart1;
SRAM_HandleTypeDef hsram1;
I2C_HandleTypeDef hi2c1;

extern uint8_t PORT607; 
extern uint8_t PORT608;
extern uint8_t PORT609;
extern uint8_t PORT610;

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
	
void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 4800;
  huart1.Init.WordLength = UART_WORDLENGTH_9B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_EVEN;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void USART1_UART_Init_printer_comm(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_9B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}


/**
  * @brief SDIO Initialization Function
  * @param None
  * @retval None
  */
void MX_SDIO_SD_Init(void)
{

  /* USER CODE BEGIN SDIO_Init 0 */

  /* USER CODE END SDIO_Init 0 */

  /* USER CODE BEGIN SDIO_Init 1 */

  /* USER CODE END SDIO_Init 1 */
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 3; //bio na 3
  /* USER CODE BEGIN SDIO_Init 2 */

  /* USER CODE END SDIO_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */

//NAPOMENA:  TIM2 is connected to APB1 (72 Mhz ili 84 MHz - vidi kako generise cube!)

void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */
	
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
	
	//Hocemo da dobijemo frekvenciju fs = 20kHz ili Ts = 0.05ms za semplovanje. 
	//Vremenski period za preskaler biramo uzimajuci u obzir da je on 16bit.
	//Uzmemo manji period za preskaler nego za Ts, npr Tp = 0.01ms.
	//Odrediti za koji je clock vezan timer: Tc = APB1 Clock
	//Prescaler = Tc(Hz) * Tp(s) - 1 = 84MHz * 0.01ms - 1 = 840 - 1
	//Period za brojac uzimamo, koliko zelimo puta da izbroji do prescaler vrijednosti da bi dobili zeljeni period. Za timer3 period je 16bit vrijednost.
	//PeriodCounter = (Ts / Tp) - 1 = (0.05 / 0.01) - 1 = 10 - 1 
  htim2.Init.Prescaler = 840-1; 	
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 5-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
}

void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */
	
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
	//Hocemo da dobijemo frekvenciju fs = 1kHz ili Ts = 1ms za semplovanje. 
	//Vremenski period za preskaler biramo uzimajuci u obzir da je on 16bit.
	//Uzmemo manji period za preskaler nego za Ts, npr Tp = 0.1ms.
	//Odrediti za koji je clock vezan timer: Tc = APB1 Clock
	//Prescaler = Tc(Hz) * Tp(s) - 1 = 84MHz * 0.1ms - 1 = 8400 - 1
	//Period za brojac uzimamo, koliko zelimo puta da izbroji do prescaler vrijednosti da bi dobili zeljeni period. Za timer3 period je 16bit vrijednost.
	//PeriodCounter = (Ts / Tp) - 1 = (1 / 0.1) - 1 = 10 - 1 
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 8400-1;	
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 10-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 3 */

  /* USER CODE END TIM2_Init 3 */
}



void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
#if 0
extern void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
	
	//PORTA
	/*Configure GPIO pin Output Level */	
	HAL_GPIO_WritePin(GPIOA, (GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_8), GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, (GPIO_PIN_5 | GPIO_PIN_7 | GPIO_PIN_12 | GPIO_PIN_15), GPIO_PIN_RESET);	
	/*Configure GPIO pins : PA1 PA2 PA3 PA5 PA7 PA8 PA12 PA15 OUTPUT*/
	GPIO_InitStruct.Pin =  GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3  | GPIO_PIN_5 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_12 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/*Configure GPIO pins : PA0 PA4 PA6 INPUT*/
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_4 | GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//PORTB
	/*Configure GPIO pin Output Level */	
	HAL_GPIO_WritePin(GPIOB, (GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_14), GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, (GPIO_PIN_0 | GPIO_PIN_1), GPIO_PIN_RESET);
	/*Configure GPIO pins : PB0 PB1 PB4 PB5 PB9 PB10 PB12 PB14 OUTPUT*/
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	/*Configure GPIO pins : PB3 PB8 PB11 PB13 PB15 INPUT*/
	GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//PORTC
	/*Configure GPIO pin Output Level */	
	
	/*Configure GPIO pins : PC0-PC6 OUTPUT */	
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	/*Configure GPIO pins : PC7 INPUT */
//	GPIO_InitStruct.Pin = GPIO_PIN_7;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//PORTD
	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, (GPIO_PIN_3 | GPIO_PIN_6), GPIO_PIN_RESET);
	/*Configure GPIO pins : PD3 PD6 OUTPUT */	
	GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//PORTE:
	/*Configure GPIO pins : PE1 OUTPUT*/
//	GPIO_InitStruct.Pin = GPIO_PIN_1;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pins : PE2 PE6 INPUT*/
	GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	//PORTF:
	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, (GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_10 | GPIO_PIN_11), GPIO_PIN_RESET);
	/*Configure GPIO pins : PF6 PF7 PF8 PF10 PF11 OUTPUT*/
	GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_10 | GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	/*Configure GPIO pins : PF9 INPUT*/
	GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	//PORTG
	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOG, (GPIO_PIN_9 | GPIO_PIN_12 | GPIO_PIN_13), GPIO_PIN_RESET);
	/*Configure GPIO pins : PG8 PG9 PG12 PG13 OUTPUT*/
  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_12 | GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);	
  /*Configure GPIO pins : PG6 PG7 PG10 PG11 PG14 PG15 INPUT*/
  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_14  | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	//PORTH
	/*Configure GPIO pins : PH1 OUTPUT*/
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	#if 0
  //PORTA0  = AN_CH_ENABLE/AN_MC_ENABLE, INIT STATE: OUTPUT, 0 OK
	//PORTA1  = #MCU1_ADC1_#ADC_CS       , INIT STATE: OUTPUT, 1 OK (linear transducer ch)
	//PORTA2  = AN_CH_#CS                , INIT STATE: OUTPUT, 1 OK
	//PORTA3  = AN_CH_CLCK               , INIT STATE: OUTPUT, 0 OK
	//PORTA5  = FLASH DRIVE SCK          , INIT STATE: OUTPUT, 0 OK
	//PORTA7  = FLASH DRIVE MOSI         , INIT STATE: OUTPUT, 0 OK
	//PORTA8  = #MCU1_ADC1_CONVST_AB     , INIT STATE: OUTPUT, 1 OK
	//PORTA12 = #MCU1_ADC2_#ADC_CS       , INIT STATE: OUTPUT, 1 OK (current ch)
	//PORTA15 = #MCU1_ADC1_ADC_RESET     , INIT STATE: OUTPUT, 0 OK
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	CLR_MC_ADC_ENABLE;
	CLR_AC_ADC_ENABLE;
	SET_SC_ADC_nCS;
	CLR_SC_ADC_CLCK;
	
	SET_CM_nADC_CS;
	SET_LT_nADC_CS;
	
	CLR_MC_ADC_RESET;
	HAL_Delay(1);
	SET_MC_ADC_RESET;	
	HAL_Delay(1);
	CLR_MC_ADC_RESET;	
	#endif
	
	
	#if 0
	while(1)
	{
		HAL_Delay(2000);
		SET_CM_nADC_CS;
		SET_LT_nADC_CS;
		HAL_Delay(2000);
		CLR_CM_nADC_CS;
		CLR_LT_nADC_CS;
	};
	#endif
	
	SET_MC_ADC_CONVST_AB;
	CLR_MC_ADC_RESET;
	
	#if 0
	//PORTE:
  //PORTE2 = DT_#OE, INIT STATE : OUTPUT, 1 OK
	//PORTE6 = AUX_OE, INIT STATE : OUTPUT, 1 OK
	GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	SET_DT_nOE;
	SET_AUX_nOE;
	#endif
	
	#if 0
	while(1)
	{
		HAL_Delay(2000);
		SET_CM_nADC_CS;
		SET_LT_nADC_CS;
		SET_DT_nOE;
		SET_AUX_nOE;
		HAL_Delay(2000);
		CLR_DT_nOE;
		CLR_AUX_nOE;
		CLR_CM_nADC_CS;
		CLR_LT_nADC_CS;		
	};
	#endif
	
	#if 0
	//PORTG:
  //PORTG10 = DT_SEL1, INIT STATE : OUTPUT, 1 OK
	//PORTG11 = DT_SEL2, INIT STATE : OUTPUT, 1 OK
	//PORTG12 = DT_SEL3, INIT STATE : OUTPUT, 1 OK
	//PORTG13 = DT_OE#1, INIT STATE : OUTPUT, 1 OK
	//PORTG14 = DT_OE#2, INIT STATE : OUTPUT, 1 OK
	//PORTG15 = DT_OE#3, INIT STATE : OUTPUT, 1 OK
	GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	SET_DT1_nOE;
	SET_DT2_nOE;
	SET_DT3_nOE;  
	#endif 
	
	#if 0
	//PORTF:
  //PORTF6  = MCU1_C3_IGBT, INIT STATE : OUTPUT, 0 OK
	//PORTF7  = MCU1_C2_IGBT, INIT STATE : OUTPUT, 0 OK
	//PORTF8  = MCU1_C1_IGBT, INIT STATE : OUTPUT, 0 OK
	//PORTF9  = MCU1_B3_IGBT, INIT STATE : OUTPUT, 0 OK
	//PORTF10 = MCU1_B2_IGBT, INIT STATE : OUTPUT, 0 OK
	//PORTF11 = MCU1_B1_IGBT, INIT STATE : OUTPUT, 0 OK
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	CLR_BREAKING1_IGBT;
	CLR_BREAKING2_IGBT;
	CLR_BREAKING3_IGBT;
	CLR_CLOSING1_IGBT;
	CLR_CLOSING2_IGBT;
	CLR_CLOSING3_IGBT;
	#endif
	
	//PORTD: USB FLASH SETTINGS
  //PORTD3  = FLASH_#CS, INIT STATE : OUTPUT, 1 OK
	//PORTD6  = FLASH_RST, INIT STATE : OUTPUT, 0 OK
	GPIO_InitStruct.Pin	= 0; 
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	SET_USB_FLASH_nCS;
	SET_USB_FLASH_RST;
		
	//PORTA 4 and PORTA6 -- FLASH DRIVE INPUTS
	GPIO_InitStruct.Pin	= 0; 
	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
}
#endif

//Ubacena alternativna inicijalizacija, jer je bilo problema na nekim portovima
#if 1
extern void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_10 
                          |GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_8, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_4 
                          |GPIO_PIN_5|GPIO_PIN_9, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9|GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pins : PE2 PE6 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PF6 PF7 PF8 PF10 
                           PF11 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_10 
                          |GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : PF9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PC0 PC1 PC2 PC3 
                           PC4 PC5 PC6 PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA3 PA5 PA7 
                           PA8 PA12 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_7 
                          |GPIO_PIN_8|GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB10 PB12 
                           PB14 PB4 PB5 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_10|GPIO_PIN_12 
                          |GPIO_PIN_14|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB11 PB13 PB15 PB3 
                           PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_15|GPIO_PIN_3 
                          |GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PG6 PG7 PG10 PG11 
                           PG14 PG15 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PG8 PG9 PG12 PG13 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PD3 PD6 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}
#endif
/* FSMC initialization function */
void MX_FSMC_Init(void)
{
  FSMC_NORSRAM_TimingTypeDef Timing;

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  hsram1.Init.PageSize = FSMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 15;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 20;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
	
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
#if 0
//clock 72 MHz:
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /**Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}
#endif
/**
  * @brief System Clock Configuration
  * @retval None
  */

//Clock 168 Mhz (pazi, koristi interni):
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /**Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

void init_variables(void)
{
#if (VIBRO_BOX)
COMM.port=COM_PORT_B;
#else
COMM.port=COM_PORT_A;
#endif 
COMM.remote_enable=REMOTE_DISABLE;
COMM.change_port=NO;
GetDataFromMemory=NO;
GMD=NO;
	
//multiplechannel adc's range = 5V
//multiplechannel adc's OSC PINS = 0/0/0 (oversampling off) 
PORT607=0xC6;
//all relays off:
PORT608=0x00;
PORT609=0x00;
//aux board dry contacts selection:
PORT610=0x00;

//HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U607, (uint8_t*)(&data), 1, 1000);

TESTPAR.analog1_gain=ANALOG_CH_RANGE_HH;
TESTPAR.analog2_gain=ANALOG_CH_RANGE_HH;
TESTPAR.analog3_gain=ANALOG_CH_RANGE_HH;
TESTPAR.analog4_gain=ANALOG_CH_RANGE_HH;
TESTPAR.analog5_gain=ANALOG_CH_RANGE_HH;
TESTPAR.analog6_gain=ANALOG_CH_RANGE_HH;
	
activetext = printer_texts[language]; 
}

void init_coil_control(void)
{
//	uint8_t MCU_bat_chg_ctrl = 0x02;
//	HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U3, (uint8_t*)&MCU_bat_chg_ctrl, 1, 1000);
//uint8_t data=0xC6;
//	uint8_t data=0xFF;
//HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U607, (uint8_t*)(&PORT607), 1, 1000);
//HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U608, (uint8_t*)(&PORT608), 1, 1000);
//HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U609, (uint8_t*)(&PORT609), 1, 1000);
//HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U610, (uint8_t*)(&PORT610), 1, 1000);

//CLR_BREAKING1_RELAY;
//CLR_BREAKING2_RELAY;
//CLR_BREAKING3_RELAY;
//CLR_CLOSING1_RELAY;
//CLR_CLOSING2_RELAY;
//CLR_CLOSING3_RELAY;
//CLR_BREAKING1_IGBT;
//CLR_BREAKING2_IGBT;
//CLR_BREAKING3_IGBT;
//CLR_CLOSING1_IGBT;
//CLR_CLOSING2_IGBT;
//CLR_CLOSING3_IGBT;
}

void init_bat_control(void)
{
//	uint8_t DRM1_Bat_Chg_Info1 = 0x10;
//	uint8_t DRM1_Bat_Chg_Info2 = 0x42;
//	DRM1_Bat_Chg_Info1 |= (DRM1_ADPT_RLY_CH1 | DRM1_ADPT_RLY_CH2);
//	DRM1_Bat_Chg_Info2 |= DRM1_ADPT_RLY_CH3;
//	HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U2, (uint8_t*)&DRM1_Bat_Chg_Info1, 1, 1000);
//	HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U4, (uint8_t*)&DRM1_Bat_Chg_Info2, 1, 1000);
}

void InitADC(void)
{
    unsigned char i,d;
    //me=0;
    for(i=1;i<3;i++)
    {    
        SRM_Write_ADC_Byte(i,0xff);       //Software Reset
        SRM_Write_ADC_Byte(i,0xff);
        SRM_Write_ADC_Byte(i,0xff);
        SRM_Write_ADC_Byte(i,0xff);
				
				delay_us(1000);
				
        //IO Port register
        SRM_Write_ADC_Byte(i,0x01);      //Write the next comm to reg. 0x01 - I/O Port
        SRM_Write_ADC_Byte(i,0x31);      //P0/P1 = input, SYNC = 1
        
        //Channel Conversion Time register
				//CH1 0x30
				//CHOP = 1, continuous conversion with two channels enabled. 
				//Conversion Time (µs) = (FW * 128 + 249) / MCLK Frequency (MHz), the FW range is 2 to 127.
				// FW = (Conversion Time(us) * MCLK (MHz) - 249) / 128
        SRM_Write_ADC_Byte(i,0x30);      //ADD 0x30 - Channel Conversion Time
        SRM_Write_ADC_Byte(i,0x9D);		   //Chop = 1, FW = 29, MCLK = 4MHz -> Conv. Time = 1 ms 
				//CH2 0x32
        SRM_Write_ADC_Byte(i,0x32);    	 //Write the next comm to reg. 0x32 - Channel Converion Time
        SRM_Write_ADC_Byte(i,0x9D);			 //Chop = 1, FW = 29, MCLK = 4MHz -> Conv. Time = 1 ms
			
        //Channel Setup register
				if(i==1)
				{
					SRM_Write_ADC_Byte(i,0x28);    //Write the next comm to reg. 0x28 - Channel Setup
					SRM_Write_ADC_Byte(i,0x08);    //stat OPT = 0, ENABLE = 1 (Continous conversion mode), Range: -10 to +10V
				}
				if(i==2)
				{
					SRM_Write_ADC_Byte(i,0x28);    //Write the next comm to reg. 0x28 - Channel Setup
					SRM_Write_ADC_Byte(i,0x08);    //stat OPT = 0, ENABLE = 1 (Continous conversion mode), Range: -10 to +10V
				}
				if (i==1)
				{
					SRM_Write_ADC_Byte(i,0x2A);     //Write the next comm to reg. 0x28 - Channel Setup
					SRM_Write_ADC_Byte(i,0x08);     //stat OPT = 0, ENABLE = 1 (Continous conversion mode), Range: -10 to +10V
				}
				if (i==2)
				{
					SRM_Write_ADC_Byte(i,0x2A);     //Write the next comm to reg. 0x28 - Channel Setup
					SRM_Write_ADC_Byte(i,0x08);     //stat OPT = 0, ENABLE = 1 (Continous conversion mode), Range: -10 to +10V
				}

        #if ADC24bit
				// Mode reg  (CH00)
        SRM_Write_ADC_Byte(i,0x38);      //Write the next comm to reg. 0x38 - Mod Reg
        SRM_Write_ADC_Byte(i,0x23);      //Mode: Continous Conversion Mode, CLAMP = 1, 24-bit resolution
				#else
				SRM_Write_ADC_Byte(i,0x38);      //Write the next comm to reg. 0x38 - Mod Reg
        SRM_Write_ADC_Byte(i,0x21);      //Mode: Continous Conversion Mode, CLAMP = 1, 16-bit resolution
				#endif
				#if ADC24bit
        // Mode reg (CH01)
        SRM_Write_ADC_Byte(i,0x3A);      //Write the next comm to reg. 0x38 - Mod Reg
        SRM_Write_ADC_Byte(i,0x23);      //Mode: Continous Conversion Mode, CLAMP = 1, 24-bit resolution
				#else
				// Mode reg (CH01)
        SRM_Write_ADC_Byte(i,0x3A);      //Write the next comm to reg. 0x38 - Mod Reg
        SRM_Write_ADC_Byte(i,0x21);      //Mode: Continous Conversion Mode, CLAMP = 1, 16-bit resolution
				#endif
    }
		
		//SRM_Write_ADC_Byte(1,0x1A);//SRM_Write_ADC_Byte(adc,0x18);
    //WriteADC24(1,0x00200000);
		//Write_ADC_GAIN(1, 0, 0);
		//Write_ADC_GAIN(1, 0, 1);

		delay_us(1000);
}

