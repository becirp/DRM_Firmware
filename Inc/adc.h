#include <stdint.h>

#ifndef ADC_8BIT_STRUCTURE
#define ADC_8BIT_STRUCTURE
typedef struct _ADC_8bit_
{
volatile uint8_t CURRENT_H[NUMBER_OF_CURRENT_CHS]; 
volatile uint8_t CURRENT_L[NUMBER_OF_CURRENT_CHS];  
volatile uint8_t LT_H[NUMBER_OF_LT_CHS]; 
volatile uint8_t LT_L[NUMBER_OF_LT_CHS];
volatile uint8_t DTH_H[NUMBER_OF_DT_CHS];
volatile uint8_t DTH_L[NUMBER_OF_DT_CHS];
volatile uint8_t DTL_H[NUMBER_OF_DT_CHS];
volatile uint8_t DTL_L[NUMBER_OF_DT_CHS];
volatile uint8_t  ANCH_ARRAY[16];
volatile uint16_t MC_ARRAY[16];
//volatile unsigned char MC_ARRAY2[16];
volatile uint8_t AUX;
volatile uint8_t dummy;
}ADC8BIT_STRUCT;
#endif

#ifndef ADC_STRUCTURE
#define ADC_STRUCTURE

typedef struct _ADC_
{
volatile uint16_t MC[NUMBER_OF_MAIN_CONTACTS];
volatile int16_t CURRENT[NUMBER_OF_CURRENT_CHS];
volatile uint16_t ANCH[8]; //NUMBER_OF_ANALOG_CHS+2 (+2 DRM Channels)
volatile int16_t LT[NUMBER_OF_LT_CHS];
volatile uint16_t DTH[NUMBER_OF_DT_CHS];
volatile uint16_t DTL[NUMBER_OF_DT_CHS];
volatile unsigned char AUX;
volatile unsigned char dummy;
}ADC_STRUCT;
#endif

extern ADC_STRUCT ADC_Results;
extern ADC8BIT_STRUCT ADC_8bit_results;

//TODO: definisati ponovo:
extern void read_analog_adc_for_trigger(unsigned char ex_trig_channel);

//extern signed int MultipleCH_ADC_Read(unsigned char ADC_Channel);
extern void ADC_Read_MainContacts(unsigned int ADC_CH);
extern void read_multiple_ch_adcs(void);
extern void read_digital_trans_chs(void);
extern void DigTrans_Counter_Reset(void);
extern unsigned char ReadAuxillaryChannels(void);
extern void read_all_adc_channels_test(uint32_t address);
extern void read_adc_res_sram_test(uint32_t address);

extern void SET_GPIO_Structures(void);
extern void SET_GPIOB_Structure(unsigned char input_output);


#if 0
extern void ADC_Read_AnalogChannels(void);

//extern void Fake_ADC_Results(void);
extern void read_all_adc_channels(void);

extern void read_all_adc_channels_test(unsigned int);
//extern void read_all_adc_channels_DRM_test(unsigned int sram_data_address);
extern void read_adc_res_sram_test(unsigned int);

//Vibration measurement functions:
//extern void read_and_send_analog_ch_values(void);

//external trigger functions:
extern void read_analog_adc_for_trigger(unsigned char ex_trig_channel);
extern void read_current_adc_for_trigger(void);
#endif

extern signed long convert_to_slong_DT_res(unsigned char channel);

//DRM Functions

extern void DRM1_ADC1_Read(void);
extern void DRM1_ADC_Read_All(void);

//VBAT Monitor function
extern void VBAT_ADC_Read(void);

//SRM functions
extern void SRM_Write_ADC_Byte(unsigned int, unsigned char);
extern void SRM_Write_ADC_24Bits(unsigned int, unsigned long);
extern unsigned char SRM_Read_ADC_Byte(unsigned int);
extern unsigned char SRM_Read_ADC_16Bits(unsigned int);