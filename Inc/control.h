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
extern unsigned int check_if_digit(unsigned char, unsigned char);
extern unsigned int string_to_int(unsigned char, unsigned char);

//RESULTS:
extern unsigned char PuniPrintBuffer        (unsigned char parameter1, unsigned char parameter2, unsigned char parameter3, unsigned char MenOrPrn, unsigned char PreinsertionR);
extern unsigned char PuniPrintBufferCurrent (unsigned char parameter1, unsigned char parameter2, unsigned char MenOrPrn);
extern unsigned char PuniPrintBufferAnalogCH(unsigned char parameter1, unsigned char parameter2, unsigned char MenOrPrn);
extern unsigned char PuniPrintBufferMotion  (unsigned char parameter1, unsigned char parameter2, unsigned char MenOrPrn);

//DRM Functions:
extern unsigned int Toggle_Pins(void);
extern unsigned int Write_DRM_CH1(void);
extern unsigned int Read_DRM_ADC_Results(void);
extern unsigned int Read_All_DRM_ADC_Results(void);
extern unsigned int Battery_Charger_Control(void);
