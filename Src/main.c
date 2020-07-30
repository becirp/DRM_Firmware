/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "stdio.h"
#include "string.h"
#include "dwt_delay.h"

#include "bios.h"
#include "comm.h"
#include "remote.h"
#include "init.h"
#include "calib.h"
#include "control.h"
#include "adc.h"

unsigned char varijabla=0;
unsigned char nova_varijabla=0; 
extern volatile unsigned char timer1_interrupt;
extern volatile unsigned char timer2_interrupt;
unsigned int Test_Counter;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
	//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
	//varijabla=1; 
	
	if(timer1_DRM_ON)
	{
		timer1_interrupt=1;
		varijabla=1;
	}
	if(timer2_SRM_ON)
	{
		timer2_interrupt=1;
		//SRM_Get_Samples();
		//Test_Counter++;
	}
		
	//__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);	
  /* NOTE : This function Should not be modified, when the callback is needed,
            the __HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
}

#if ST_LINK_DEBUG
extern unsigned int MeasurementStart_Comm();
extern unsigned int InitStick_Comm(unsigned char tt);
#endif

int main(void)
{
	
	unsigned char status=0;     
	
  /* USER CODE BEGIN 1 */
    uint16_t myVariable;
	  unsigned char myVar2;
	  FATFS myFATFS;
		FIL myFILE;
	  UINT testByte;
	  unsigned int i;
		static int timerValue=0;
//	  uint8_t PORT607; 
//    uint8_t data;

//    data = 0x00;     // 0x0C in your example

		myVariable='A';
	
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
	
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
	SystemCoreClockUpdate();
  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SDIO_SD_Init();
  MX_USART1_UART_Init();
  MX_FSMC_Init();
  MX_FATFS_Init();
  MX_TIM2_Init_DRM();
	//MX_TIM3_Init();
	DWT_Delay_Init();
	//MX_I2C1_Init();
	//init_variables();
	//init_coil_control();
	//init_bat_control();
	//MAIN_CONTACTS_INIT_SEQ;
	
	//Upaljen kanal na pocetku
	PWR1_ENABLE;
	PWR2_ENABLE;
	
	CURRENT_CH2_DISABLE;
	CURRENT_CH1_DISABLE;
	
	InitADC();
	
	//Ugasiti punjac na pocetku -- CHG ne radi dok se ne ukljuci TPS kolo, koje pravi +5V koje se referencira za ove upravljacke signale. Ovo treba prepraviti u hardveru.
	CHG1_DISABLE;
	CHG2_DISABLE;
	
	//DAC Start state
	DRM_SYNC1_HIGH;
	DRM_SYNC2_HIGH;
	DRM_CLK_LOW;
	DRM_DIN_LOW;
	
	//Vbat init
	VBAT_ADC_CLK_HIGH;
	
	//SRM init
	SRM1_CS1_SET;
	SRM1_CS2_SET;
	SRM2_CS1_SET;
	SRM2_CS2_SET;
	SRM_SYNC_LOW;
	
	//PRIVREMENO SE UPISUJU JEDINICNI KOEFICIJENTI:
	Reset_Calibration_COMM();
	
	#if ST_LINK_DEBUG
	COMM.port='B';
	COMM.remote_enable='E';
	sprintf(OutputBuffer, "Debbugging session starts");
	SendOutputBuffer('B');
	#endif
		
	#if ST_LINK_DEBUG
	CLR_SC_ADC_CLCK;
	SET_SC_ADC_CLCK;
	CLR_SC_ADC_CLCK;
	SET_SC_ADC_CLCK;
	CLR_SC_ADC_CLCK;
	SET_SC_ADC_CLCK;
	CLR_SC_ADC_CLCK;
	SET_SC_ADC_CLCK;
	CLR_SC_ADC_CLCK;
	SET_SC_ADC_CLCK;
	CLR_SC_ADC_CLCK;
	SET_SC_ADC_CLCK;
	
	//InitStick_Comm(1);
	
	#endif
		
	/* Remote ENABLE PORTB */
		COMM.remote_enable=REMOTE_ENABLE;
		COMM.port = COMM_PORT_B;
		#if ST_LINK_DEBUG
			MeasurementStart_Comm();
		#endif
	
	//Watchdog setup
	
	
	/* WHILE LOOP BEGINS HERE*/
  while (1)
  {		
		//Slanje komandi sa PC na PORTB
		#if 1
			COMM.remote_enable=REMOTE_ENABLE;
			COMM.port = COMM_PORT_B;
			#if ST_LINK_DEBUG
				MeasurementStart_Comm();
			#endif
			status=GetInputBuffer(COMM.port);
			if (status==MAIN_OK)
      {             
      status=RemoteCommandSwitch();
                if (status==MAIN_NOK)
                {                                         
                     sprintf(OutputBuffer,"ErRCS"); //Error: Remote Comand Switch               
                }      
                else if (status==ERROR_SRAM)
                {
                    sprintf(OutputBuffer, "ErMEM"); //Error: SRAM Memory!               
                }
                else if(status==ERROR_SDC)
                {
                    //vec upisana greska, samo je posalji nazad!
                }       
                else if (status!=MAIN_OK){                
                //sprintf(OutputBuffer, "ErS%02u", ErrorSLAVE_nbr); //Error: Slave Device XX
                }                  
      } 
      else 
      {
                sprintf(OutputBuffer,"ErGIB"); //Error: Get Input Buffer
      }       
			SendOutputBuffer(COMM.port);
			GetDataFromMemory=NO;
		#endif
	
	
	#if 0
	//treba clear-at flag prije pozivanje START funkcije:
	__HAL_TIM_CLEAR_FLAG(&htim2, TIM_IT_UPDATE);	
	//inace ce otici odmah u intterupt prije po pozivanju START_IT funkcije (dole)
		HAL_TIM_Base_Start_IT(&htim2);
		
		for(;;){
			
			#if 0
			if(__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_UPDATE) != RESET)
			{				
				__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
			}
			#endif
			
			while(timer1_interrupt==0)
			{
				nova_varijabla=HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
			}
			
			timer1_interrupt=0;	

			__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
						
			nova_varijabla=HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
		}
	#endif
	
	#if 0
	for(i=0; i<10; i++)
	{
	SET_LED_OPEN;
	HAL_Delay(1000);
	CLR_LED_OPEN;
	HAL_Delay(1000);
	}
	#endif
		
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	HAL_Delay(100);	
	
	//HAL_TIM_Base_Start_IT(&htim2);

	#if 0
	if(f_mount(&myFATFS, "M0:", 1)==FR_OK)
	{
		myVar2='A';
	}
	else
	{
		myVar2='B';
	}
	
	if(myVar2=='A')
	{
		char myPath[]="WRITE123.TXT\0";
		f_open(&myFILE, myPath, FA_WRITE|FA_CREATE_ALWAYS);
		char myData[]="Test data for SD card... New text added\0";
		f_write(&myFILE, myData, sizeof(myData), &testByte);
		f_close(&myFILE);
	}
	#endif
	
	
		
		
		//HAL_UART_Receive(&huart1, &myVar2, 1, HAL_MAX_DELAY);
		//sprintf(OutputBuffer, "PRIMIO PODATAK\r\n");
		//SendOutputBuffer('A');			
		
		
		///myVar2='X';
		//HAL_UART_Transmit(&huart1, &myVar2, 1, 20000);
		//HAL_Delay(1000);	
		//twi_master_trans(&twic_master, GPIO_EXPANSION_U608,(unsigned char *) &PORT608,1,0,0);

		//HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U608, PORT607, 2, 100);
		
		#if 0
		PORT607=0xFF;
		while(1)
		{	
		
		HAL_Delay(3000);		
		if(data==0xFF)
			{
				data=0x00;				
			}
			else
			{
				data=0xFF;				
			}
						
		HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U607, (uint8_t*)(&data), 1, 1000);
		//HAL_I2C_Master_Transmit(&hi2c1, 0x40, &PORT607, 1, 1000);
		//data='A';										
		//HAL_UART_Transmit(&huart1, &data, 1, 0xFFFF); 
		putcharB(data);		
		}
		#endif
		
		
		
		
		#if 0
		while(1)
		{
//			COMM.remote_enable=REMOTE_ENABLE;
//			sprintf(OutputBuffer, "TEST KOMUNIKACIJE");
//			SendOutputBuffer('B');
//			DWT_Delay(1000000);
//			putcharB(0x00);
//		  DWT_Delay(1000000);
//			putcharB(0xFF);
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
			DWT_Delay(5);
		}
		#endif
		

		
		
		#if 0
      GMD=NO;
      if(COMM.change_port==YES)
      {
            COMM.change_port=NO;
            if(COMM.port==COM_PORT_A){COMM.port=COM_PORT_B;}
            else                     {COMM.port=COM_PORT_A;}  
      }                                                       

			#if ST_LINK_DEBUG
				MeasurementStart_Comm();
			#endif
			
      status=GetInputBuffer(COMM.port);
			//status=GetInputBuffer('B');
        
      if (status==MAIN_OK)
      {             
      status=RemoteCommandSwitch();
                if (status==MAIN_NOK)
                {                                         
                     sprintf(OutputBuffer,"ErRCS"); //Error: Remote Comand Switch               
                }      
                else if (status==ERROR_SRAM)
                {
                    sprintf(OutputBuffer, "ErMEM"); //Error: SRAM Memory!               
                }
                else if(status==ERROR_SDC)
                {
                    //vec upisana greska, samo je posalji nazad!
                }       
                else if (status!=MAIN_OK){                
                //sprintf(OutputBuffer, "ErS%02u", ErrorSLAVE_nbr); //Error: Slave Device XX
                }                  
      } 
      else 
      {
                sprintf(OutputBuffer,"ErGIB"); //Error: Get Input Buffer
      }          
      if(GetDataFromMemory==NO){SendOutputBuffer(COMM.port);}
      GetDataFromMemory=NO;      
      #endif
		
		#if 0
		unsigned int address=1000;
		//unsigned char data2[]="Komunikacija putem porta A";
		unsigned char podatak;
		char buffer [60];
		int n;
		uint16_t data='X';
		//Rade obje funkcije!
		//putcharB(myVariable);		
		
		sprintf(buffer, "Komunikacija putem porta A...");		

		myVariable=getcharB();
		
		//putcharB('X');
		HAL_Delay(100);
		
		if(myVariable=='A')
		{
			sprintf(OutputBuffer, "Inicijaliziram SD CARD");
			SendOutputBuffer('B');
			//char SDPath[4];   /* SD logical drive path */

			if(f_mount(&myFATFS, SDPath, 1)==FR_OK)
			{
			sprintf(OutputBuffer, "Uspio!");
			SendOutputBuffer('B');
			
			sprintf(OutputBuffer, "Upisujem file...");
			SendOutputBuffer('B');
				
			char myPath[]="WRITE123.TXT\0";
		  f_open(&myFILE, myPath, FA_WRITE|FA_CREATE_ALWAYS);
		   char myData[]="Test data for SD card... New text added\0";
		  f_write(&myFILE, myData, sizeof(myData), &testByte);
		  f_close(&myFILE);
				
			sprintf(OutputBuffer, "Zavrsio!");
			SendOutputBuffer('B');
				
			}
			else
			{
			sprintf(OutputBuffer, "Greska...");
			SendOutputBuffer('B');
			}
		}
		else if(myVariable=='B')
		{
			sprintf(OutputBuffer, "Komunikacija radi!");
			SendOutputBuffer('B');
		}
		else if(myVariable=='C')
		{
			sprintf(OutputBuffer, "Komunikacija radi! Cekam karakter na B");
			SendOutputBuffer('B');
	
	//		HAL_UART_Receive(&huart1, &podatak, 1, 10000 );
			
			sprintf(OutputBuffer, "Dobio sam podatak: %c", podatak);
			SendOutputBuffer('B');
			
		}
		else if(myVariable=='D')
		{
				data='A';
				for(i=0; i<100; i++)
				{
					address++;								
					data++;					
					HAL_SRAM_Write_16b(&hsram1, &address, &data, 1);
				}
				
			address=1000;								
			data=0;
			myVariable=0;		
    		
			for(i=0; i<100; i++)
			{
			address++;
			HAL_SRAM_Read_16b(&hsram1, &address,  &myVariable, 1);
			putcharB(myVariable);
			}
		}
		else if(myVariable=='E')
		{
				sprintf(OutputBuffer, "Startam timer");
				SendOutputBuffer('B');
				#if 0
				varijabla=0;
				HAL_TIM_Base_Stop_IT(&htim2);
				HAL_TIM_Base_DeInit(&htim2);				
				__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);	
				HAL_TIM_Base_Init(&htim2);	
				__HAL_TIM_CLEAR_FLAG(&htim2, TIM_IT_UPDATE);					 							
				HAL_TIM_Base_Start_IT(&htim2);
				#endif
			
			   //treba clear-at flag prije pozivanje START funkcije:
			  __HAL_TIM_CLEAR_FLAG(&htim2, TIM_IT_UPDATE);	
				//inace ce otici odmah u intterupt prije po pozivanju START_IT funkcije (dole)
				HAL_TIM_Base_Start_IT(&htim2);
			
				while(varijabla==0)
				{
						//izgleda da mu watchdog proradi ovdje, ako ostane samo u while petlji!
							//putcharB('Y');
							//HAL_Delay(100);
					    //for(i=0;i<100;i++)
							//{
					    //nova_varijabla=i;
							//}	
							//izgleda da moras bilo kakvu HAL funkciju pozvati u ovome slucaju inace ce ostati u petlji!?					
							//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);							
							nova_varijabla=HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
				}
				varijabla=0;
				sprintf(OutputBuffer, "Zavrsio sam 1");
				SendOutputBuffer('B');
				
				varijabla=0;
				while(varijabla==0)
				{
						putcharB('X');
						HAL_Delay(100);
				}				
				sprintf(OutputBuffer, "Zavrsio sam 2");
				SendOutputBuffer('B');				
				
				varijabla=0;
				while(varijabla==0)
				{
						putcharB('Y');
						HAL_Delay(100);
				}				
				sprintf(OutputBuffer, "Zavrsio sam 3");
				SendOutputBuffer('B');

				HAL_TIM_Base_Stop_IT(&htim2);			
		}
		else
		{
			sprintf(OutputBuffer, "Komunikacija radi! Saljem podatake na UART");
			SendOutputBuffer('B');
			for(i=0; i<10; i++)
			{
//				HAL_UART_Transmit(&huart1, (unsigned char *)buffer, strlen(buffer), 2000);	
				
				HAL_Delay(1000);
			}
			sprintf(OutputBuffer, "Zavrsio!");
			SendOutputBuffer('B');			
		}
		
		
		#if 0
		myVariable++;
		HAL_Delay(3000);
		
		if(myVar2=='A')
		{
			putcharB('A');
		}
		else
		{
			putcharB('B');
		}
		#endif
		
		#if 0
		data='A';
		for(i=0; i<100; i++)
		{
			address++;								
			data++;
			HAL_SRAM_Write_16b(&hsram1, &address, &data, 1);
		}
				
		address=1000;								
		data=0;
		myVariable=0;		
    		
		for(i=0; i<100; i++)
		{
			address++;
			HAL_SRAM_Read_16b(&hsram1, &address,  &myVariable, 1);
			putcharB(myVariable);
		}
		#endif
		
		#endif
       
  }
	/* WHILE LOOP ENDS HERE*/
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
