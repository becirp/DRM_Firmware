extern unsigned int SetCommPort_COMM(void);
extern unsigned int SetRemoteControl_COMM(unsigned char);
extern unsigned int DT_Reset_Counter_COMM(void);
extern unsigned int AUX_DryWetControl_Command(unsigned char dry_wet);
extern unsigned int BSGTest_COMM(void);
extern unsigned int Set_Test_Mode_COMM(unsigned char bsg_mode);
extern unsigned int Read_ADC_Res_COMM(void);
extern unsigned int Read_One_ADC_CH_COMM(void);
extern unsigned int SetMeasParameters_COMM(void);
extern unsigned int CheckBreakerStatus_COMM(unsigned char);
extern unsigned int EnterTFUnits_COMM(void);
extern unsigned int SetPulseLenghts_COMM(void);
extern unsigned int SetLTransdParameters_COMM(void);
extern unsigned int EnterTimeDate_COMM(void);
extern unsigned int MeasurementStart_Comm(void);
extern unsigned int SendMeasurementParameters_Comm(void);
extern unsigned int EnterBreakerTestDataFileName_COMM(void);
extern unsigned int Get_Test_Settings_Comm(void);
extern unsigned int Set_RMO_Results_Comm(void);
extern unsigned int Get_RMO_Results_Comm(void);
extern unsigned int Set_Active_Language_Comm(void);
extern unsigned int Get_Measurement_Data_Comm(void);
extern unsigned int Relay_IGBT_Control(void);
extern unsigned int MemoryTest_Comm(void);
extern unsigned int MemoryWriteByte_Comm(void);
extern unsigned int MemoryReadByte_Comm(void);
extern unsigned int Internal_SD_Mem_Comm(void);

extern void OK_Response(void);
extern unsigned int check_if_digit(unsigned char first, unsigned char last);
extern unsigned int string_to_int(unsigned char first, unsigned char last);

//RESULTS:
extern unsigned char PuniPrintBuffer        (unsigned char parameter1, unsigned char parameter2, unsigned char parameter3, unsigned char MenOrPrn, unsigned char PreinsertionR);
extern unsigned char PuniPrintBufferCurrent (unsigned char parameter1, unsigned char parameter2, unsigned char MenOrPrn);
extern unsigned char PuniPrintBufferAnalogCH(unsigned char parameter1, unsigned char parameter2, unsigned char MenOrPrn);
extern unsigned char PuniPrintBufferMotion  (unsigned char parameter1, unsigned char parameter2, unsigned char MenOrPrn);

//DRM Functions:
extern unsigned int foo_function(void);
extern unsigned int Toggle_Pins(void);
extern unsigned int Write_DRM_Channel(void);
extern unsigned int DRM_Get_Results();
//extern unsigned int Battery_Battery_Charger_Control(void);
extern unsigned int DRM_Start_Test(void);
extern unsigned int DRM_Current_Control(void);
extern unsigned int DRM_DAC_Test(void);

//SRM Functions
extern unsigned int SRM_Start_Test(void);
extern unsigned int SRM_ADC_GetData(void);
extern void SRM_AllChannels_Conversion(void);
extern void SRM_Multiple_Conversions(void);
extern void SRM_Write_Data_ToArray(int i);
	
//BAT Functions
extern unsigned int Get_BAT_Voltage(void);
extern unsigned int Battery_Charger_Control(void);

//PWR Control
extern unsigned int Channel_Power_Control(void);

//Timer variables
extern unsigned char timer1_DRM_ON;
extern unsigned char timer2_SRM_ON;

//SRM variables
extern unsigned char timer1_DRM_ON;
extern unsigned char timer2_SRM_ON;