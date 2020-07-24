extern void MX_USART1_UART_Init(void);
extern void USART1_UART_Init_printer_comm(void);
extern void MX_SDIO_SD_Init(void);
extern void MX_TIM2_Init_DRM(void);
extern void MX_TIM2_Init_SRM(void);
extern void MX_TIM3_Init(void);
extern void MX_GPIO_Init(void);
extern void MX_FSMC_Init(void);
extern void MX_I2C1_Init(void);
extern void init_variables(void);

extern void init_coil_control(void);


extern void SystemClock_Config(void);

extern SD_HandleTypeDef hsd;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart1;
extern SRAM_HandleTypeDef hsram1;
extern I2C_HandleTypeDef hi2c1;

//DRM functions
extern void init_bat_control(void);

//SRM functions
extern void InitADC(void);
