#include "main.h"
#include <bios.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "calib.h"
#include "adc.h"
#include "init.h"
#include "comm.h"
#include "control.h"
#include "dwt_delay.h"

ADC_STRUCT     ADC_Results;
ADC8BIT_STRUCT ADC_8bit_results;

unsigned char ADCdataL[8];
unsigned char ADCdataH[8];

//SRM ADC settings
unsigned int SRM_ADC_Delay = 1;
unsigned int Test_Phase = 0;
float SRM1_C_Gain = 1.0;
float SRM1_V_Gain = 1.0;
float SRM1_C_Offset = 0.0;
float SRM1_V_Offset = 0.0;
float SRM2_C_Gain = 1.0;
float SRM2_V_Gain = 1.0;
float SRM2_C_Offset = 0.0;
float SRM2_V_Offset = 0.0;
unsigned int SRM1_G_I, SRM2_G_I;
unsigned int SRM1_G_V, SRM2_G_V;
//Ovi koeficijenti u RMO200H firmveru idu u eeprom. Odrediti gdje ce ici na DSRM kontroli.
unsigned long SRM1_C_CH0_Gain[4];
unsigned long SRM2_C_CH0_Gain[4];
unsigned int SRM1_C_CH0_Sign[4];
unsigned int SRM2_C_CH0_Sign[4];
unsigned long SRM1_C_CH0_Offset[4];
unsigned long SRM2_C_CH0_Offset[4];
unsigned long SRM1_C_CH1_Gain[4];
unsigned long SRM2_C_CH1_Gain[4];
unsigned int SRM1_C_CH1_Sign[4];
unsigned int SRM2_C_CH1_Sign[4];
unsigned long SRM1_C_CH1_Offset[4];
unsigned long SRM2_C_CH1_Offset[4];
unsigned long SRM1_V_CH0_Gain[4];
unsigned long SRM2_V_CH0_Gain[4];
unsigned int SRM1_V_CH0_Sign[4];
unsigned int SRM2_V_CH0_Sign[4];
unsigned long SRM1_V_CH0_Offset[4];
unsigned long SRM2_V_CH0_Offset[4];
unsigned long SRM1_V_CH1_Gain[4];
unsigned long SRM2_V_CH1_Gain[4];
unsigned int SRM1_V_CH1_Sign[4];
unsigned int SRM2_V_CH1_Sign[4];
unsigned long SRM1_V_CH1_Offset[4];
unsigned long SRM2_V_CH1_Offset[4];
unsigned long Curr_Gain;
unsigned char Curr_Sign;
unsigned long Curr_Offset;

//SRM variables
volatile unsigned int cptr[4];
unsigned int C_buffer[4][322];
unsigned int C_buffer_RET[2];
unsigned int Test_Count;
unsigned int DACV_Reg[5];
unsigned int Test_Current; //inace u eepromu
unsigned int RemoteTest_h;
unsigned int RemoteTest;
volatile unsigned int AD[4];
unsigned int TestDuration = 226;


//TODO: ponovo definisati:
void read_analog_adc_for_trigger(unsigned char ex_trig_channel){}
	
//mjereno: 0.05 us
#define DELAY_IN_us {__nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop();}

void ADC_Read_MainContacts(unsigned int ADC_CH)
{

uint16_t adc_array[16];
uint16_t adc_array2[16];

unsigned int i;
unsigned int mc_channel;
unsigned int ac_channel;
unsigned int pom, pom2;

//SET_MC_ADC_Ports();

for(i=0; i<16; i++){adc_array[i]=0;}
for(i=0; i<16; i++){adc_array2[i]=0;}
for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++){ADC_Results.MC[i]=0;}
for(i=0; i<NUMBER_OF_ANALOG_CHS+2; i++){ADC_Results.ANCH[i]=0;} //2 DRM channels!


//PINS ON PORT B ARE INPUTS (EXCEPT I2C AND BOOT1):
GPIO_InitTypeDef GPIO_InitStruct;
GPIO_InitStruct.Pin = (uint16_t)0xFF3B;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//PINS ON PORT C (FIRST BYTE) ARE INPUTS:
GPIO_InitStruct.Pin = GPIO_PIN_FIRST_BYTE;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init(USB_DATA_PORT, &GPIO_InitStruct);

SET_MC_ADC_ENABLE;
CLR_SC_ADC_CLCK;

DELAY_IN_us;

CLR_SC_ADC_nCS;

DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
CLR_SC_ADC_CLCK;
DELAY_IN_us;   
SET_SC_ADC_CLCK;
DELAY_IN_us;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
adc_array[0]=GPIOB->IDR;
adc_array2[0]=GPIOC->IDR;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
adc_array[1]=GPIOB->IDR;
adc_array2[1]=GPIOC->IDR;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
adc_array[2]=GPIOB->IDR;
adc_array2[2]=GPIOC->IDR;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
adc_array[3]=GPIOB->IDR;
adc_array2[3]=GPIOC->IDR;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
adc_array[4]=GPIOB->IDR;
adc_array2[4]=GPIOC->IDR;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
adc_array[5]=GPIOB->IDR;
adc_array2[5]=GPIOC->IDR;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
adc_array[6]=GPIOB->IDR;
adc_array2[6]=GPIOC->IDR;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
adc_array[7]=GPIOB->IDR;
adc_array2[7]=GPIOC->IDR;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
adc_array[8]=GPIOB->IDR;
adc_array2[8]=GPIOC->IDR;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
adc_array[9]=GPIOB->IDR;
adc_array2[9]=GPIOC->IDR;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
adc_array[10]=GPIOB->IDR;
adc_array2[10]=GPIOC->IDR;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
adc_array[11]=GPIOB->IDR;
adc_array2[11]=GPIOC->IDR;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
adc_array[12]=GPIOB->IDR;
adc_array2[12]=GPIOC->IDR;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
adc_array[13]=GPIOB->IDR;
adc_array2[13]=GPIOC->IDR;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
adc_array[14]=GPIOB->IDR;
adc_array2[14]=GPIOC->IDR;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
adc_array[15]=GPIOB->IDR;
adc_array2[15]=GPIOC->IDR;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_nCS;
DELAY_IN_us;

CLR_MC_ADC_ENABLE;


//Main Contacts CH0-12:
pom2=0x01;
for (mc_channel=0; mc_channel<12; mc_channel++)
{
    pom=0x8000;        
    for (i=0; i<16; i++){ if ((adc_array[i]&pom2)==pom2){ADC_Results.MC[mc_channel]|=pom;} pom>>=1; }    
    pom2<<=1; 
		//if(mc_channel==2){pom2<<=1;}
		//if(mc_channel==5){pom2<<=2;}
		if(mc_channel==1){pom2<<=1;}
		if(mc_channel==4){pom2<<=2;}
}

//Analog channels CH0-6(-8 DRM):
pom2=0x01;
for (ac_channel=0; ac_channel<8; ac_channel++)
{
    pom=0x8000;        
    for (i=0; i<16; i++){ if ((adc_array2[i]&pom2)==pom2){ADC_Results.ANCH[ac_channel]|=pom;} pom>>=1; }    
    pom2<<=1; 
}

}

unsigned char ADCresult1H;
unsigned char ADCresult2H;
unsigned char ADCresult1L;
unsigned char ADCresult2L;

void DigTrans_Counter_Reset(void)
{
CLR_DT_nOE;
DELAY_IN_us; 
CLR_DT_RES; 
DELAY_IN_us; 
SET_DT_RES;
SET_DT_nOE;
DELAY_IN_us; 
}

void read_digital_trans_chs(void)
{
char i;
//PINS ON PORT B ARE INPUTS (EXCEPT I2C AND BOOT1):
GPIO_InitTypeDef GPIO_InitStruct;
GPIO_InitStruct.Pin = (uint16_t)0xFF3B;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//PINS ON PORT C (FIRST BYTE) ARE INPUTS:
GPIO_InitStruct.Pin = GPIO_PIN_FIRST_BYTE;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init(USB_DATA_PORT, &GPIO_InitStruct);
	
CLR_DT_nOE;
	
for(i=0; i<3; i++)
{
DELAY_IN_us;
DELAY_IN_us;
CLR_DT_SEL1;  //SEL1=0 , MSB Bajt
SET_DT_SEL2;  //SEL2=1
DELAY_IN_us; 
switch(i)
{
    case 0: CLR_DT1_nOE; break; //#OE=0; 
    case 1: CLR_DT2_nOE; break; //#OE=1;
    case 2: CLR_DT3_nOE; break; //#OE=2;
}               
//Sacekaj Stabilan izlaz                        
DELAY_IN_us; 
ADCresult1H=GPIOC->IDR;
SET_DT_SEL1; //SEL1=1  , 3RD bajt
SET_DT_SEL2; //SEL2=1
DELAY_IN_us; 
ADCresult1L=GPIOC->IDR;               
CLR_DT_SEL1; //SEL1=0  , 2ND bajt
CLR_DT_SEL2; //SEL2=0                
DELAY_IN_us; 
ADCresult2H=GPIOC->IDR;
SET_DT_SEL1; //SEL1=1  , LSB bajt
CLR_DT_SEL2; //SEL2=0      
//Sacekaj Stabilan izlaz                               
DELAY_IN_us; 
ADCresult2L=GPIOC->IDR;
SET_DT1_nOE;
SET_DT2_nOE;
SET_DT3_nOE;                   
DELAY_IN_us; 
   
#if 0
ADCresult1H=rotl(ADCresult1H);
ADCresult1L=rotl(ADCresult1L);
ADCresult2H=rotl(ADCresult2H);
ADCresult2L=rotl(ADCresult2L);
#endif

ADC_Results.DTH[i]= (unsigned int)ADCresult1H<<8;
ADC_Results.DTH[i]+=(unsigned int)ADCresult1L;
ADC_Results.DTL[i]= (unsigned int)ADCresult2H<<8;
ADC_Results.DTL[i]+=(unsigned int)ADCresult2L;
}
SET_DT_nOE; 

}

unsigned char ReadAuxillaryChannels(void)
{
unsigned int retVal=0x00;
unsigned char retVal2=0x00;
//PINS ON PORT B ARE INPUTS (EXCEPT I2C AND BOOT1):
GPIO_InitTypeDef GPIO_InitStruct;
GPIO_InitStruct.Pin = (uint16_t)0xFF3B;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//PINS ON PORT C (FIRST BYTE) ARE INPUTS:
GPIO_InitStruct.Pin = GPIO_PIN_FIRST_BYTE;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init(USB_DATA_PORT, &GPIO_InitStruct);
	
CLR_AUX_nOE;
DELAY_IN_us; 
retVal=GPIOB->IDR;
DELAY_IN_us; 
SET_AUX_nOE; 
DELAY_IN_us; 

retVal2=retVal&0x03;
retVal>>=1;
retVal2|=retVal&0x1C;
retVal>>=2;
retVal2|=retVal&0x60;

ADC_Results.AUX=retVal2;
	
return retVal2;
}

void read_multiple_ch_adcs(void)
{
char i;
signed int rezultat=0;

//PINS ON PORT C (FIRST BYTE) ARE INPUTS:
GPIO_InitTypeDef GPIO_InitStruct={0};
GPIO_InitStruct.Pin = GPIO_PIN_FIRST_BYTE;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	 
GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

DELAY_IN_us;

//Current channels:
SET_CM_nADC_CS;
SET_CM_ADC_RD_SCLK;
SET_MC_ADC_CONVST_AB; 
SET_MC_ADC_RESET;
DELAY_IN_us;
CLR_MC_ADC_RESET;
DELAY_IN_us;
CLR_MC_ADC_CONVST_AB;
DELAY_IN_us;
SET_MC_ADC_CONVST_AB;
HAL_Delay(1);
CLR_CM_nADC_CS;
HAL_Delay(1);
CLR_CM_ADC_RD_SCLK;
HAL_Delay(1);
ADCdataL[0]=GPIOC->IDR;
SET_CM_ADC_RD_SCLK ;
DELAY_IN_us;
CLR_CM_ADC_RD_SCLK ;
DELAY_IN_us;
ADCdataH[0]=GPIOC->IDR;
DELAY_IN_us;
for(i=1; i<NUMBER_OF_CURRENT_CHS; i++)
{
	SET_CM_ADC_RD_SCLK ;
	DELAY_IN_us;
	CLR_CM_ADC_RD_SCLK ;
	DELAY_IN_us;
	ADCdataL[i]=GPIOC->IDR;
	SET_CM_ADC_RD_SCLK ;
	DELAY_IN_us;
	CLR_CM_ADC_RD_SCLK ;
	DELAY_IN_us;	
	ADCdataH[i]=GPIOC->IDR;
	DELAY_IN_us;
}
SET_CM_ADC_RD_SCLK;
SET_CM_nADC_CS;

for(i=0; i<NUMBER_OF_CURRENT_CHS; i++)
{
    ADC_Results.CURRENT[i]=(((signed int)(ADCdataH[i]))<<8);
    ADC_Results.CURRENT[i]+=(ADCdataL[i]);
}

//Linear transduccer channels:
SET_LT_nADC_CS;
SET_LT_ADC_RD_SCLK;
SET_MC_ADC_CONVST_AB; 
SET_MC_ADC_RESET;
DELAY_IN_us;
CLR_MC_ADC_RESET;
DELAY_IN_us;
CLR_MC_ADC_CONVST_AB;
DELAY_IN_us;
SET_MC_ADC_CONVST_AB;
HAL_Delay(1);
CLR_LT_nADC_CS;
DELAY_IN_us;
CLR_LT_ADC_RD_SCLK ;
HAL_Delay(1);
//LINEARNI TRANSUCER IMA SAMO 4 kanala:
ADCdataL[0]=GPIOC->IDR;
SET_LT_ADC_RD_SCLK;
DELAY_IN_us;
CLR_LT_ADC_RD_SCLK ;
DELAY_IN_us;
ADCdataH[0]=GPIOC->IDR;
DELAY_IN_us;
for(i=1; i<4; i++)
{
	SET_LT_ADC_RD_SCLK ;
	DELAY_IN_us;
	CLR_LT_ADC_RD_SCLK ;
	DELAY_IN_us;
	ADCdataL[i]=GPIOC->IDR;
	SET_LT_ADC_RD_SCLK ;
	DELAY_IN_us;
	CLR_LT_ADC_RD_SCLK ;
	DELAY_IN_us;	
	ADCdataH[i]=GPIOC->IDR;
	DELAY_IN_us;
}
SET_LT_ADC_RD_SCLK;
SET_LT_nADC_CS;

//Novi ADC, CH4 je ustvari CH3:
//for(i=0; i<NUMBER_OF_LT_CHS; i++)
//{           
    //ADC_Results.LT[i]=(((signed int)(ADCdataH[i]))<<8);
    //ADC_Results.LT[i]+=(ADCdataL[i]);    
//}

ADC_Results.LT[0]=(((signed int)(ADCdataH[0]))<<8);
ADC_Results.LT[0]+=(ADCdataL[0]);

ADC_Results.LT[1]=(((signed int)(ADCdataH[1]))<<8);
ADC_Results.LT[1]+=(ADCdataL[1]);

#if LINEAR_TRANDUCER_ADC_6_CHS
ADC_Results.LT[2]=(((signed int)(ADCdataH[3]))<<8);
ADC_Results.LT[2]+=(ADCdataL[3]);
#else
ADC_Results.LT[2]=(((signed int)(ADCdataH[2]))<<8);
ADC_Results.LT[2]+=(ADCdataL[2]);
#endif

GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

signed long convert_to_slong_DT_res(unsigned char channel)
{
return (((signed long)(ADC_Results.DTH[channel])<<16)|ADC_Results.DTL[channel]);
} 

#if 0
void read_all_adc_channels_test(uint32_t sram_address)
{
unsigned int i;
uint16_t data;
uint32_t address_aux;
	
sram_address=sram_address*100;
address_aux=sram_address;
sram_address+=(uint32_t)0x60000000;
	
//address_aux=sram_address;	
	
#if 0
for(i=0; i<16; i++)
{		
	data=0xFFFF;
	HAL_SRAM_Write_16b(&hsram1, &address_aux, &data, 1); 					
	address_aux=sram_address+1;
}
#endif

#if 1
if(address_aux>567800) //5678*100
{
	for(i=0; i<16; i++)
	{		
		data=0xFFFF;
		HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data, 1); //ovako ide upis!
		sram_address+=2;
	}
}
else
{
	for(i=0; i<16; i++)
	{
		data=0x0000;
		HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data, 1); //ovako ide upis!
		sram_address+=2;

	}
}	
#endif
}
#endif

#if 1

#pragma O3

GPIO_InitTypeDef GPIO_InitStruct1={0};
GPIO_InitTypeDef GPIO_InitStruct2={0};

#define INPUT 0
#define OUTPUT 1

void SET_GPIO_Structures(void)
{
GPIO_InitTypeDef GPIO_InitStruct3={0};

GPIO_InitStruct1.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3;
GPIO_InitStruct1.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct1.Pull = GPIO_NOPULL;
GPIO_InitStruct1.Speed = GPIO_SPEED_FREQ_HIGH;

GPIO_InitStruct2.Pin = (uint16_t)0xFF3B;
GPIO_InitStruct2.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct2.Pull = GPIO_NOPULL;
GPIO_InitStruct2.Speed = GPIO_SPEED_FREQ_HIGH;	
	
GPIO_InitStruct3.Pin = GPIO_PIN_FIRST_BYTE;
GPIO_InitStruct3.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct3.Pull = GPIO_NOPULL;
HAL_GPIO_Init(GPIOC, &GPIO_InitStruct3);

}

void SET_GPIOB_Structure(unsigned char input_output)
{
	if(input_output==INPUT)
	{
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct2);
	}	
	else
	{
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct1);
	}
}

void read_all_adc_channels_test(uint32_t sram_address)
{
uint16_t data;
uint8_t data2;
unsigned int i;
	
//100 selektovano, mozda treba vise ili manje...
//sram_address*=226; //=113*2
sram_address*=100;
sram_address+=(uint32_t)0x60000000;
	
//INITIATE READ FOR CURRENT AND ANALOG TRNSDUCER CHANNELS:
	
//SET_GPIOB_Structure(OUTPUT);

SET_LT_nADC_CS;
SET_CM_nADC_CS;
SET_LT_ADC_RD_SCLK;
SET_CM_ADC_RD_SCLK;
SET_MC_ADC_CONVST_AB; 
SET_MC_ADC_RESET;
DELAY_IN_us;
CLR_MC_ADC_RESET;
DELAY_IN_us; DELAY_IN_us;
CLR_MC_ADC_CONVST_AB;
DELAY_IN_us; DELAY_IN_us;
SET_MC_ADC_CONVST_AB;
DELAY_IN_us;

//SET_GPIOB_Structure(INPUT);

//READ MAIN CONTACTS AND ANALOG CHANNELS:		
	
#if 0
//PINS ON PORT B ARE INPUTS (EXCEPT I2C AND BOOT1):
GPIO_InitTypeDef GPIO_InitStruct;
GPIO_InitStruct.Pin = (uint16_t)0xFF3B;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
//PINS ON PORT C (FIRST BYTE) ARE INPUTS:
GPIO_InitStruct.Pin = GPIO_PIN_FIRST_BYTE;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	
HAL_GPIO_Init(USB_DATA_PORT, &GPIO_InitStruct);
#endif

//SVI PORTOVI ULAZNI; OK

SET_MC_ADC_ENABLE;
CLR_SC_ADC_CLCK;

DELAY_IN_us;

CLR_SC_ADC_nCS;

DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
CLR_SC_ADC_CLCK;
DELAY_IN_us;   
SET_SC_ADC_CLCK;
DELAY_IN_us;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
CLR_SC_ADC_CLCK;
DELAY_IN_us;
SET_SC_ADC_CLCK;
DELAY_IN_us;
data=GPIOB->IDR;
data2=GPIOC->IDR;
//////////////////////////////////////////////////////
//if(sram_address>45*367){data=0x20;}else{data=0x00;}
///////////////////////////////////////////////////////
CLR_SC_ADC_CLCK;
HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data, 1); //sram_address+0
SET_SC_ADC_CLCK;
sram_address+=2;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+2
data=GPIOB->IDR;
data2=GPIOC->IDR;
sram_address+=1;
//////////////////////////////////////////////////////
//if(sram_address>45*367){data=0x20;}else{data=0x00;}
///////////////////////////////////////////////////////
CLR_SC_ADC_CLCK;
HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data, 1); //sram_address+3
SET_SC_ADC_CLCK;
sram_address+=2;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+5
data=GPIOB->IDR;
data2=GPIOC->IDR;
sram_address+=1;
//////////////////////////////////////////////////////
//if(sram_address>45*367){data=0x20;}else{data=0x00;}
///////////////////////////////////////////////////////
CLR_SC_ADC_CLCK;
HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data, 1); //sram_address+6
SET_SC_ADC_CLCK;
sram_address+=2;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+8
data=GPIOB->IDR;
data2=GPIOC->IDR;
sram_address+=1;
//////////////////////////////////////////////////////
//if(sram_address>45*367){data=0x20;}else{data=0x00;}
///////////////////////////////////////////////////////
CLR_SC_ADC_CLCK;
HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data, 1); //sram_address+9
SET_SC_ADC_CLCK;
sram_address+=2;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+11
data=GPIOB->IDR;
data2=GPIOC->IDR;
sram_address+=1;
//////////////////////////////////////////////////////
//if(sram_address>45*367){data=0x20;}else{data=0x00;}
///////////////////////////////////////////////////////
CLR_SC_ADC_CLCK;
HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data, 1); //sram_address+12
SET_SC_ADC_CLCK;
sram_address+=2;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+14
data=GPIOB->IDR;
data2=GPIOC->IDR;
sram_address+=1;
//////////////////////////////////////////////////////
//if(sram_address>45*367){data=0x20;}else{data=0x00;}
///////////////////////////////////////////////////////
CLR_SC_ADC_CLCK;
HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data, 1); //sram_address+15
SET_SC_ADC_CLCK;
sram_address+=2;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+17
data=GPIOB->IDR;
data2=GPIOC->IDR;
sram_address+=1;
//////////////////////////////////////////////////////
//if(sram_address>45*367){data=0x20;}else{data=0x00;}
///////////////////////////////////////////////////////
CLR_SC_ADC_CLCK;
HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data, 1); //sram_address+18
SET_SC_ADC_CLCK;
sram_address+=2;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+20
data=GPIOB->IDR;
data2=GPIOC->IDR;
sram_address+=1;
//////////////////////////////////////////////////////
//if(sram_address>45*367){data=0x20;}else{data=0x00;}
///////////////////////////////////////////////////////
CLR_SC_ADC_CLCK;
HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data, 1); //sram_address+21
SET_SC_ADC_CLCK;
sram_address+=2;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+23
data=GPIOB->IDR;
data2=GPIOC->IDR;
sram_address+=1;
//////////////////////////////////////////////////////
//if(sram_address>45*367){data=0x20;}else{data=0x00;}
///////////////////////////////////////////////////////
CLR_SC_ADC_CLCK;
HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data, 1); //sram_address+24
SET_SC_ADC_CLCK;
sram_address+=2;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+26
data=GPIOB->IDR;
data2=GPIOC->IDR;
sram_address+=1;
//////////////////////////////////////////////////////
//if(sram_address>45*367){data=0x20;}else{data=0x00;}
///////////////////////////////////////////////////////
CLR_SC_ADC_CLCK;
HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data, 1); //sram_address+27
SET_SC_ADC_CLCK;
sram_address+=2;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+29
data=GPIOB->IDR;
data2=GPIOC->IDR;
sram_address+=1;
//////////////////////////////////////////////////////
//if(sram_address>45*367){data=0x20;}else{data=0x00;}
///////////////////////////////////////////////////////
CLR_SC_ADC_CLCK;
HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data, 1); //sram_address+30
SET_SC_ADC_CLCK;
sram_address+=2;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+32
data=GPIOB->IDR;
data2=GPIOC->IDR;
sram_address+=1;
//////////////////////////////////////////////////////
if(sram_address>45*367){data=0x20;}else{data=0x00;}
///////////////////////////////////////////////////////
CLR_SC_ADC_CLCK;
HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data, 1); //sram_address+33
SET_SC_ADC_CLCK;
sram_address+=2;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+35
data=GPIOB->IDR;
data2=GPIOC->IDR;
sram_address+=1;
//////////////////////////////////////////////////////
//if(sram_address>45*367){data=0x20;}else{data=0x00;}
///////////////////////////////////////////////////////
CLR_SC_ADC_CLCK;
HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data, 1); //sram_address+36
SET_SC_ADC_CLCK;
sram_address+=2;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+38
data=GPIOB->IDR;
data2=GPIOC->IDR;
sram_address+=1;
//////////////////////////////////////////////////////
//if(sram_address>45*367){data=0x20;}else{data=0x00;}
///////////////////////////////////////////////////////
CLR_SC_ADC_CLCK;
HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data, 1); //sram_address+39
SET_SC_ADC_CLCK;
sram_address+=2;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+41
data=GPIOB->IDR;
data2=GPIOC->IDR;
sram_address+=1;
//////////////////////////////////////////////////////
//if(sram_address>45*367){data=0x20;}else{data=0x00;}
///////////////////////////////////////////////////////
CLR_SC_ADC_CLCK;
HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data, 1); //sram_address+43
SET_SC_ADC_CLCK;
sram_address+=2;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+45
data=GPIOB->IDR;
data2=GPIOC->IDR;
sram_address+=1;
//////////////////////////////////////////////////////
//if(sram_address>45*367){data=0x20;}else{data=0x00;}
///////////////////////////////////////////////////////
CLR_SC_ADC_CLCK;
HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data, 1); //sram_address+46
SET_SC_ADC_nCS;
sram_address+=2;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+48

CLR_MC_ADC_ENABLE;

//////////////////////////////////////////////////////////////////
//////////////////AUX CHANNELS: //////////////////////////////////
//////////////////////////////////////////////////////////////////
DELAY_IN_us; 
	
CLR_AUX_nOE;
DELAY_IN_us; 
data2=GPIOB->IDR;
sram_address+=1;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+49
DELAY_IN_us; 
SET_AUX_nOE; 
DELAY_IN_us; 
//////////////////////////////////////////////////////////////////
////////////////DIGITAL TRANSDUCER: //////////////////////////////
//////////////////////////////////////////////////////////////////
DELAY_IN_us;

CLR_DT_nOE;
	
for(i=0; i<3; i++)
{
	DELAY_IN_us;
	DELAY_IN_us;
	CLR_DT_SEL1;  //SEL1=0 , MSB Bajt
	SET_DT_SEL2;  //SEL2=1
	DELAY_IN_us; 
	switch(i)
	{
			case 0: CLR_DT1_nOE; break; //#OE=0; 
			case 1: CLR_DT2_nOE; break; //#OE=1;
			case 2: CLR_DT3_nOE; break; //#OE=2;
	}               
	//Sacekaj Stabilan izlaz                        
	DELAY_IN_us; 
	data2=GPIOC->IDR;
	sram_address+=1;
	HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+50+54+58
	SET_DT_SEL1; //SEL1=1  , 3RD bajt
	SET_DT_SEL2; //SEL2=1
	DELAY_IN_us; 
	data2=GPIOC->IDR;  
	sram_address+=1;
	HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+51+55+59
	CLR_DT_SEL1; //SEL1=0  , 2ND bajt
	CLR_DT_SEL2; //SEL2=0                
	DELAY_IN_us; 
	data2=GPIOC->IDR;
	sram_address+=1;
	HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+52+56+60
	SET_DT_SEL1; //SEL1=1  , LSB bajt
	CLR_DT_SEL2; //SEL2=0      
	//Sacekaj Stabilan izlaz                               
	DELAY_IN_us; 
	data2=GPIOC->IDR;
	sram_address+=1;
	HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+53+57+61
	SET_DT1_nOE;
	SET_DT2_nOE;
	SET_DT3_nOE;                   
	DELAY_IN_us; 
}
SET_DT_nOE;

// CURRENT CHANNELS:

#if 1

DELAY_IN_us;

SET_GPIOB_Structure(OUTPUT);

DELAY_IN_us;

#if 0
//Current channels:
SET_CM_nADC_CS;
SET_CM_ADC_RD_SCLK;
SET_MC_ADC_CONVST_AB; 
SET_MC_ADC_RESET;
DELAY_IN_us;
CLR_MC_ADC_RESET;
DELAY_IN_us;
CLR_MC_ADC_CONVST_AB;
DELAY_IN_us;
SET_MC_ADC_CONVST_AB;
#endif

CLR_CM_nADC_CS;
DELAY_IN_us; 
CLR_CM_ADC_RD_SCLK;
DELAY_IN_us; 

data2=GPIOC->IDR;
sram_address+=1;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+62
SET_CM_ADC_RD_SCLK ;
DELAY_IN_us;
DELAY_IN_us;
CLR_CM_ADC_RD_SCLK ;
DELAY_IN_us;
DELAY_IN_us;
data2=GPIOC->IDR;
sram_address+=1;
HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+63
DELAY_IN_us;
for(i=1; i<NUMBER_OF_CURRENT_CHS; i++)
{
	SET_CM_ADC_RD_SCLK;
	DELAY_IN_us;
	DELAY_IN_us;
	CLR_CM_ADC_RD_SCLK;
	DELAY_IN_us;
	DELAY_IN_us;
	data2=GPIOC->IDR;	
	SET_CM_ADC_RD_SCLK;
	sram_address+=1;
	HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+64+66+68+70+72
	CLR_CM_ADC_RD_SCLK ;
	DELAY_IN_us;
	DELAY_IN_us;	
	data2=GPIOC->IDR;
	sram_address+=1;
	HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+65+67+69+71+73
	DELAY_IN_us;
	DELAY_IN_us;
}
SET_CM_ADC_RD_SCLK;
SET_CM_nADC_CS;
#endif

#if 1

// LINEAR TRANSDUCER CHANNELS:

SET_LT_nADC_CS;
SET_LT_ADC_RD_SCLK;
SET_MC_ADC_CONVST_AB; 
SET_MC_ADC_RESET;
DELAY_IN_us;
CLR_MC_ADC_RESET;
DELAY_IN_us;
CLR_MC_ADC_CONVST_AB;
DELAY_IN_us;
SET_MC_ADC_CONVST_AB;
DELAY_IN_us;
CLR_LT_nADC_CS;
DELAY_IN_us;
CLR_LT_ADC_RD_SCLK ;
DELAY_IN_us;
//LINEARNI TRANSUCER IMA SAMO 4 kanala:
data2=GPIOC->IDR;
sram_address++; 
HAL_SRAM_Write_8b(&hsram1, &sram_address,  &data2, 1); //sram_address+74
SET_LT_ADC_RD_SCLK;
DELAY_IN_us;
CLR_LT_ADC_RD_SCLK ;
DELAY_IN_us;
data2=GPIOC->IDR;
sram_address++; 
HAL_SRAM_Write_8b(&hsram1, &sram_address,  &data2, 1); //sram_address+75
DELAY_IN_us;
for(i=1; i<3; i++)
{
	SET_LT_ADC_RD_SCLK ;
	DELAY_IN_us;
	CLR_LT_ADC_RD_SCLK ;
	DELAY_IN_us;
	data2=GPIOC->IDR;
	sram_address++;
	HAL_SRAM_Write_8b(&hsram1, &sram_address,  &data2, 1); //sram_address+76+78
	SET_LT_ADC_RD_SCLK ;
	DELAY_IN_us;
	CLR_LT_ADC_RD_SCLK ;
	DELAY_IN_us;	
	data2=GPIOC->IDR;
	sram_address++;
	HAL_SRAM_Write_8b(&hsram1, &sram_address,  &data2, 1); //sram_address+77+79
	DELAY_IN_us;
}
SET_LT_ADC_RD_SCLK;
SET_LT_nADC_CS;

#endif

//OVAJ ZADNJI MOZDA I NE TREBA, ALI ZA SVAKI SLUCAJ:
SET_GPIOB_Structure(INPUT);

}

#endif

#pragma pop

void read_adc_res_sram_test(uint32_t sram_address)
{
uint16_t adc_array[16];
uint8_t adc_array2[16];	
uint16_t data;
uint8_t data2;
uint32_t sram_address_aux;
unsigned int i;
unsigned int mc_channel;
unsigned int ac_channel;
unsigned int pom, pom2;
	
//100 selektovano, mozda treba vise ili manje...
//sram_address*=226; //113*2
sram_address*=100;
sram_address+=(uint32_t)0x60000000;
	
for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++) {ADC_Results.MC[i]=0;}
for(i=0; i<(NUMBER_OF_ANALOG_CHS+2); i++){ADC_Results.ANCH[i]=0;}

for(i=0; i<16; i++){adc_array[i]=0;}
for(i=0; i<16; i++){adc_array2[i]=0;}


////////////////////// START: READ MAIN CONTACSTS AND ANALOG CHANNELS /////////////////////////////////////
for(i=0;i<16;i++)
{
	HAL_SRAM_Read_16b(&hsram1, (uint32_t *)sram_address, &adc_array[i], 1); //ovako mora ici citanje!	
	sram_address+=2;
	HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sram_address, &adc_array2[i], 1); //sram_address=sram_address+31 //zavrsio MC
	sram_address+=1;	//sram_address=(sram_address+64) izasao iz petlje
}

//Main Contacts CH0-12:
pom2=0x01;
for (mc_channel=0; mc_channel<NUMBER_OF_MAIN_CONTACTS; mc_channel++)
{
    pom=0x8000;        
    for (i=0; i<16; i++){ if ((adc_array[i]&pom2)==pom2){ADC_Results.MC[mc_channel]|=pom;} pom>>=1; }    
    pom2<<=1; 
		if(mc_channel==1){pom2<<=1;}
		if(mc_channel==4){pom2<<=2;}
}

//Analog channels CH0-6(-8 DRM):
pom2=0x01;
for (ac_channel=0; ac_channel<(NUMBER_OF_ANALOG_CHS+2); ac_channel++)
{
    pom=0x8000;        
    for (i=0; i<16; i++){ if ((adc_array2[i]&pom2)==pom2){ADC_Results.ANCH[ac_channel]|=pom;} pom>>=1; }    
    pom2<<=1; 
}	
///////////////////////// END: READ MAIN CONTACSTS AND ANALOG CHANNELS /////////////////////////////////////

/////////////////////////// START: READ AUX CHANNELS ///////////////////////////////////////////////////////
//sram address = 64
HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address=sram_address+32;
ADC_Results.AUX=(unsigned char)data2;
/////////////////////////// END: READ AUX CHANNELS /////////////////////////////////////////////////////////

/////////////////////////////// START: READ DIGITAL TRANSDUCERS ////////////////////////////////////////////
for(i=0; i<NUMBER_OF_DT_CHS; i++){ADC_Results.DTH[i]=0;	 ADC_Results.DTL[i]=0;}

for(i=0; i<3; i++)
{
sram_address+=1;
HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sram_address, &data2, 1);  //sram_address+66+74+82
ADCresult1H=data2;
sram_address+=1;
ADC_Results.DTH[i]= (unsigned int)ADCresult1H<<8;
HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sram_address, &data2, 1);  //sram_address+68+76+84
ADCresult1L=data2;
sram_address+=1;
ADC_Results.DTH[i]+=(unsigned int)ADCresult1L;
HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sram_address, &data2, 1);  //sram_address+70+78+86
ADCresult2H=data2;
sram_address+=1;	
ADC_Results.DTL[i]= (unsigned int)ADCresult2H<<8;
HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sram_address, &data2, 1);  //sram_address+72+80+88
ADCresult2L=data2;
ADC_Results.DTL[i]+=(unsigned int)ADCresult2L;
}
/////////////////////////////// END: READ DIGITAL TRANSDUCERS //////////////////////////////////////////////

/////////////////////////////// START: READ CURRENT CHANNELS ///////////////////////////////////////////////
for(i=0; i<NUMBER_OF_CURRENT_CHS; i++){ADC_Results.CURRENT[i]=0;}

for(i=0; i<NUMBER_OF_CURRENT_CHS; i++)
{
	sram_address+=1; 
	HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+90+94+98+102+106+110
	ADCdataL[i]=data2;
	sram_address+=1; 
	HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+92+96+100+104+108+112
	ADCdataH[i]=data2;	
}

//sram_address=43 zavrsio current

for(i=0; i<NUMBER_OF_CURRENT_CHS; i++)
{
    ADC_Results.CURRENT[i]=(((signed int)(ADCdataH[i]))<<8);
    ADC_Results.CURRENT[i]+=(ADCdataL[i]);
}
/////////////////////////////// END: READ CURRENT CHANNELS ///////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
//////////// LINEAR TRANSDUCERS START ///////////////////////////
/////////////////////////////////////////////////////////////////
#if 1
for(i=0; i<NUMBER_OF_LT_CHS; i++) {ADC_Results.LT[i]=0;	}
	
for(i=0; i<NUMBER_OF_LT_CHS; i++)
{
	HAL_SRAM_Read_8b(&hsram1, &sram_address,  &data2, 1); 
	ADCdataL[i]=data2;
	sram_address++; //sram_address=44+46+48
	HAL_SRAM_Read_8b(&hsram1, &sram_address,  &data2, 1); 
	ADCdataH[i]=data2;
	sram_address++;	//sram_address=45+47+49
}

ADC_Results.LT[0]=(((signed int)(ADCdataH[0]))<<8);
ADC_Results.LT[0]+=(ADCdataL[0]);

ADC_Results.LT[1]=(((signed int)(ADCdataH[1]))<<8);
ADC_Results.LT[1]+=(ADCdataL[1]);

ADC_Results.LT[2]=(((signed int)(ADCdataH[2]))<<8);
ADC_Results.LT[2]+=(ADCdataL[2]);
//////////// LINEAR TRANSDUCERS END /////////////////////////////
#endif

}

void read_adc_raw_results(uint32_t sram_address)
{
uint16_t adc_array[16];
uint8_t adc_array2[16];	
uint16_t data;
uint8_t data2;
uint32_t sram_address_aux;
unsigned int i;
unsigned int mc_channel;
unsigned int ac_channel;
unsigned int pom, pom2;
	
//100 selektovano, mozda treba vise ili manje...
//sram_address*=226; //113*2
sram_address*=100;
sram_address+=(uint32_t)0x60000000;
	
//for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++) {ADC_8bit_results.MC_ARRAY[i]=0;}
//for(i=0; i<(NUMBER_OF_ANALOG_CHS+2); i++){ADC_8bit_results.ANCH_ARRAY[i]=0;}

//ADC_8bit_results={0};
//set all ADC_8bit_results struct elements to 0:
memset(&ADC_8bit_results, 0, sizeof ADC_8bit_results);

//ovo se moze izbaciti:
for(i=0; i<16; i++){adc_array[i]=0;}
for(i=0; i<16; i++){adc_array2[i]=0;}


////////////////////// START: READ MAIN CONTACSTS AND ANALOG CHANNELS /////////////////////////////////////
for(i=0;i<16;i++)
{
	HAL_SRAM_Read_16b(&hsram1, (uint32_t *)sram_address, &adc_array[i], 1); //ovako mora ici citanje!	
	sram_address+=2;
	HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sram_address, &adc_array2[i], 1); //sram_address=sram_address+31 //zavrsio MC
	sram_address+=1;	//sram_address=(sram_address+64) izasao iz petlje
	
	ADC_8bit_results.MC_ARRAY[i]=adc_array[i];
	ADC_8bit_results.ANCH_ARRAY[i]=adc_array2[i];
	
}

/////////////////////////// START: READ AUX CHANNELS ///////////////////////////////////////////////////////
//sram address = 64
HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address=sram_address+32;
ADC_8bit_results.AUX=data2;
/////////////////////////// END: READ AUX CHANNELS /////////////////////////////////////////////////////////

/////////////////////////////// START: READ DIGITAL TRANSDUCERS ////////////////////////////////////////////
for(i=0; i<NUMBER_OF_DT_CHS; i++){ADC_Results.DTH[i]=0;	 ADC_Results.DTL[i]=0;}

for(i=0; i<3; i++)
{
sram_address+=1;
HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sram_address, &data2, 1);  //sram_address+66+74+82
ADC_8bit_results.DTH_H[i]=data2;
sram_address+=1;
HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sram_address, &data2, 1);  //sram_address+68+76+84
ADC_8bit_results.DTH_L[i]=data2;
sram_address+=1;
HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sram_address, &data2, 1);  //sram_address+70+78+86
ADC_8bit_results.DTL_H[i]=data2;
sram_address+=1;	
HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sram_address, &data2, 1);  //sram_address+72+80+88
ADC_8bit_results.DTL_L[i]=data2;
}
/////////////////////////////// END: READ DIGITAL TRANSDUCERS //////////////////////////////////////////////

/////////////////////////////// START: READ CURRENT CHANNELS ///////////////////////////////////////////////
for(i=0; i<NUMBER_OF_CURRENT_CHS; i++){ADC_Results.CURRENT[i]=0;}

for(i=0; i<NUMBER_OF_CURRENT_CHS; i++)
{
	sram_address+=1; 
	HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+90+94+98+102+106+110
	ADC_8bit_results.CURRENT_H[i]=data2;
	sram_address+=1; 
	HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sram_address, &data2, 1); //sram_address+92+96+100+104+108+112
	ADC_8bit_results.CURRENT_L[i]=data2;	
}
/////////////////////////////// END: READ CURRENT CHANNELS ///////////////////////////////////////////////

/////////////////////////////// START: READ ANALOG TRANSDUCERS ////////////////////////////////////////////
for(i=0; i<NUMBER_OF_LT_CHS; i++) {ADC_Results.LT[i]=0;	}

for(i=0; i<NUMBER_OF_LT_CHS; i++)
{
	HAL_SRAM_Read_8b(&hsram1, &sram_address,  &data2, 1); 
	ADC_8bit_results.LT_H[i]=data2;
	sram_address++; //sram_address=44+46+48
	HAL_SRAM_Read_8b(&hsram1, &sram_address,  &data2, 1); 
	ADC_8bit_results.LT_L[i]=data2;
	sram_address++;	//sram_address=45+47+49
}
/////////////////////////////// END: READ ANALOG TRANSDUCERS ////////////////////////////////////////////

}

void get_real_adc_values(void)
{
unsigned int i;
unsigned int mc_channel;
unsigned int ac_channel;
unsigned int pom, pom2;
	
memset(&ADC_Results, 0, sizeof ADC_Results);
	
//Main Contacts CH0-12:
pom2=0x01;
for (mc_channel=0; mc_channel<NUMBER_OF_MAIN_CONTACTS; mc_channel++)
{
    pom=0x8000;        
    for (i=0; i<16; i++){ if ((ADC_8bit_results.MC_ARRAY[i]&pom2)==pom2){ADC_Results.MC[mc_channel]|=pom;} pom>>=1; }    
    pom2<<=1; 
		if(mc_channel==1){pom2<<=1;}
		if(mc_channel==4){pom2<<=2;}
}

//Analog channels CH0-6(-8 DRM):
pom2=0x01;
for (ac_channel=0; ac_channel<(NUMBER_OF_ANALOG_CHS+2); ac_channel++)
{
    pom=0x8000;        
    for (i=0; i<16; i++){ if ((ADC_8bit_results.ANCH_ARRAY[i]&pom2)==pom2){ADC_Results.ANCH[ac_channel]|=pom;} pom>>=1; }    
    pom2<<=1; 
}	

ADC_Results.AUX=(unsigned char)ADC_8bit_results.AUX;

for(i=0; i<NUMBER_OF_DT_CHS; i++){ADC_Results.DTH[i]=0;	 ADC_Results.DTL[i]=0;}

for(i=0; i<3; i++)
{
	ADC_Results.DTH[i]= (unsigned int)ADC_8bit_results.DTH_H[i]<<8;
	ADC_Results.DTH[i]+=(unsigned int)ADC_8bit_results.DTH_L[i];
	ADC_Results.DTL[i]= (unsigned int)ADC_8bit_results.DTL_H[i]<<8;
	ADC_Results.DTL[i]+=(unsigned int)ADC_8bit_results.DTL_L[i];
}

for(i=0; i<NUMBER_OF_CURRENT_CHS; i++){ADC_Results.CURRENT[i]=0;}

for(i=0; i<NUMBER_OF_CURRENT_CHS; i++)
{
    ADC_Results.CURRENT[i]=(((signed int)(ADC_8bit_results.CURRENT_H[i]))<<8);
    ADC_Results.CURRENT[i]+=(ADC_8bit_results.CURRENT_H[i]);
}

for(i=0; i<NUMBER_OF_LT_CHS; i++) {ADC_Results.LT[i]=0;	}

ADC_Results.LT[0]=(((signed int)(ADC_8bit_results.LT_H[0]))<<8);
ADC_Results.LT[0]+=(ADC_8bit_results.LT_L[0]);

ADC_Results.LT[1]=(((signed int)(ADC_8bit_results.LT_H[1]))<<8);
ADC_Results.LT[1]+=(ADC_8bit_results.LT_L[1]);

ADC_Results.LT[2]=(((signed int)(ADC_8bit_results.LT_H[2]))<<8);
ADC_Results.LT[2]+=(ADC_8bit_results.LT_L[2]);

}


/* READ DRM Channels */
void DRM1_ADC1_Read(void)
{
	unsigned int i;
	uint8_t current_data[16];
	uint8_t voltage_data[16];
	uint16_t bit_operator = 0x8000;
	//DRM Channels buffers
	ADC_Results.ANCH[0]=0; 
	ADC_Results.ANCH[1]=0;
//	ADC_Results.ANCH[4]=0; 
//	ADC_Results.ANCH[5]=0;
	
	DRM_ADC_CLK_LOW;
	DELAY_IN_us;
	DRM_ADC_CS_RESET;
	for(i=0; i<6; i++)
	{
		DELAY_IN_us;
		DRM_ADC_CLK_HIGH;
		DELAY_IN_us;
		DRM_ADC_CLK_LOW;
	}
	for(i=0; i<16; i++)
	{
		DELAY_IN_us;
		DRM_ADC_CLK_HIGH;
		DELAY_IN_us;
		current_data[i] = DRM_ADC_CURR1;
		voltage_data[i] = DRM_ADC_VOUT1;
		DRM_ADC_CLK_LOW;
	}
	DELAY_IN_us;
	DRM_ADC_CS_SET;
	
	for(i=0; i<16; i++)
	{
		if(current_data[i] == 1) ADC_Results.ANCH[0] |= bit_operator;
		if(voltage_data[i] == 1) ADC_Results.ANCH[1] |= bit_operator;
//		if(current_data[i] == 1) ADC_Results.ANCH[4] |= bit_operator;
//		if(voltage_data[i] == 1) ADC_Results.ANCH[5] |= bit_operator;
		bit_operator = bit_operator >> 1;
	}	
}

/* READ All DRM Channels */
//Sada su samo dva kanala umjesto tri. Ako bude kakvih gresaka oko spremanja i citanja rezultata
void DRM_ADC_Read_All(void)
{
	unsigned int i;
	uint8_t current_data1[16];
	uint8_t voltage_data1[16];
	uint8_t current_data2[16];
	uint8_t voltage_data2[16];
	uint16_t bit_operator = 0x8000;
	//DRM Channels buffers
	ADC_Results.ANCH[0]=0; 
	ADC_Results.ANCH[1]=0;
	ADC_Results.ANCH[2]=0; 
	ADC_Results.ANCH[3]=0;
	
	DRM_ADC_CLK_LOW;
	DWT_Delay_ns(50);
	DRM_ADC_CS_RESET;
	for(i=0; i<6; i++)
	{
		DWT_Delay_ns(50);
		DRM_ADC_CLK_HIGH;
		DWT_Delay_ns(50);
		DRM_ADC_CLK_LOW;
	}
	for(i=0; i<16; i++)
	{
		DWT_Delay_ns(50);
		DRM_ADC_CLK_HIGH;
		DWT_Delay_ns(50);
		current_data1[i] = DRM_ADC_CURR1;
		voltage_data1[i] = DRM_ADC_VOUT1;
		current_data2[i] = DRM_ADC_CURR2;
		voltage_data2[i] = DRM_ADC_VOUT2;
		DRM_ADC_CLK_LOW;
	}
	DWT_Delay_ns(50);
	DRM_ADC_CS_SET;
	
	for(i=0; i<16; i++)
	{
		if(current_data1[i] == 1) ADC_Results.ANCH[0] |= bit_operator;
		if(voltage_data1[i] == 1) ADC_Results.ANCH[1] |= bit_operator;
		if(current_data2[i] == 1) ADC_Results.ANCH[2] |= bit_operator;
		if(voltage_data2[i] == 1) ADC_Results.ANCH[3] |= bit_operator;
		bit_operator = bit_operator >> 1;
	}	
}

void VBAT_ADC_Read(void)
{
	unsigned int i;
	uint8_t vbat_data1[16];
	uint8_t vbat_data2[16];
	uint16_t bit_operator = 0x8000;
	ADC_Results.ANCH[4]=0;
	ADC_Results.ANCH[5]=0;
	
	VBAT_ADC_CLK_LOW;
	DELAY_IN_us;
	VBAT_ADC_CS_RESET;
	for(i=0; i<6; i++)
	{
		DELAY_IN_us;
		VBAT_ADC_CLK_HIGH;
		DELAY_IN_us;
		VBAT_ADC_CLK_LOW;
	}
	for(i=0; i<16; i++)
	{
		DELAY_IN_us;
		VBAT_ADC_CLK_HIGH;
		DELAY_IN_us;
		vbat_data1[i] = VBAT_ADC_DATA1;
		vbat_data2[i] = VBAT_ADC_DATA2;
		VBAT_ADC_CLK_LOW;
	}
	DELAY_IN_us;
	VBAT_ADC_CS_SET;
	for(i=0; i<16; i++)
	{
		if(vbat_data1[i] == 1) ADC_Results.ANCH[4] |= bit_operator;
		if(vbat_data2[i] == 1) ADC_Results.ANCH[5] |= bit_operator;
		bit_operator = bit_operator >> 1;
	}	
}

void SRM_ADC_Select(unsigned int channel, unsigned int select)
{
	if(select == SELECT_CHIP)
	{
		switch(channel)
		{
			case SRM1_ADC1: SRM1_CS1_RESET; break;
			case SRM1_ADC2: SRM1_CS2_RESET; break;
			case SRM2_ADC1: SRM2_CS1_RESET; break;
			case SRM2_ADC2: SRM2_CS2_RESET; break;
		}
	}
	else if(select == UNSELECT_CHIP)
	{
		switch(channel)
		{
		case SRM1_ADC1: SRM1_CS1_SET; break;
		case SRM1_ADC2: SRM1_CS2_SET; break;
		case SRM2_ADC1: SRM2_CS1_SET; break;
		case SRM2_ADC2: SRM2_CS2_SET; break;
		}
	}
}

#if 0
//Write byte to AD7732 test function
void Write_ADC_Byte(unsigned int channel, unsigned int commandWord)
{
	unsigned int i;

	SRM_ADC_Select(channel, SELECT_CHIP);
	delay_us(SRM_ADC_Delay);
	for(i=0; i<8; i++)
	{
		if(commandWord & 0x80)
		{
			SRM_DIN_HIGH;
		}
		else
		{
			SRM_DIN_LOW;
		}
		commandWord = commandWord << 1;
		delay_us(SRM_ADC_Delay);
		SRM_CLK_LOW;
		delay_us(SRM_ADC_Delay);
		SRM_CLK_HIGH;
	}
	delay_us(SRM_ADC_Delay);
	//SRM_DIN_HIGH;
	SRM_ADC_Select(channel, UNSELECT_CHIP);
}
#endif

void SRM_Write_ADC_Byte(unsigned int channel, unsigned int commandWord)
{
	unsigned int i;

	SRM_ADC_Select(channel, SELECT_CHIP);
	//delay_us(SRM_ADC_Delay);
	DWT_Delay_us(SRM_ADC_Delay);
	for(i=0; i<8; i++)
	{
		if(commandWord & 0x80)
		{
			SRM_DIN_HIGH;
		}
		else
		{
			SRM_DIN_LOW;
		}
		commandWord = commandWord << 1;
		DWT_Delay_us(SRM_ADC_Delay);
		SRM_CLK_LOW;
		DWT_Delay_us(SRM_ADC_Delay);
		SRM_CLK_HIGH;
	}
	DWT_Delay_us(SRM_ADC_Delay);
	SRM_DIN_HIGH;
	SRM_ADC_Select(channel, UNSELECT_CHIP);
}

void SRM_Write_ADC_24Bits(unsigned int channel, unsigned long data)
{
	unsigned int i;
	SRM_ADC_Select(channel, SELECT_CHIP);
	delay_us(SRM_ADC_Delay);
	for(i=0; i<24; i++)
	{
		if(data & 0x800000)
		{
			SRM_DIN_HIGH;
		}
		else
		{
			SRM_DIN_LOW;
		}
		data = data << 1;
		delay_us(SRM_ADC_Delay);
		SRM_CLK_LOW;
		delay_us(SRM_ADC_Delay);
		SRM_CLK_HIGH;
	}
	delay_us(SRM_ADC_Delay);
	//SRM_DIN_HIGH;
	SRM_ADC_Select(channel, UNSELECT_CHIP);
}

unsigned char SRM_Read_ADC_Byte(unsigned int channel)
{
	unsigned int i;
	unsigned char data = 0x00;
	SRM_ADC_Select(channel, SELECT_CHIP);
	DWT_Delay_us(SRM_ADC_Delay);
	for(i=0; i<8; i++)
	{
		data = data << 1;
		DWT_Delay_us(SRM_ADC_Delay);
		SRM_CLK_LOW;
		DWT_Delay_us(SRM_ADC_Delay);
		SRM_CLK_HIGH;
		if((channel == SRM1_ADC1) || (channel == SRM1_ADC2))
		{
			data |= SRM1_DOUT;
		}
		if((channel == SRM2_ADC1) || (channel == SRM2_ADC2))
		{
			data |= SRM2_DOUT;
		}
	}
	DWT_Delay_us(SRM_ADC_Delay);
	SRM_ADC_Select(channel, UNSELECT_CHIP);
	return data;
}

unsigned int SRM_Read_ADC_16Bits(unsigned int channel)
{
	unsigned int i;
	unsigned int data = 0x00;
	SRM_ADC_Select(channel, SELECT_CHIP);
	DWT_Delay_us(SRM_ADC_Delay);
	for(i=0; i<16; i++)
	{
		data = data << 1;
		DWT_Delay_us(SRM_ADC_Delay);
		SRM_CLK_LOW;
		DWT_Delay_us(SRM_ADC_Delay);
		SRM_CLK_HIGH;
		//if(SRM1_DOUT == GPIO_PIN_SET) data |= 0x01;
		if((channel == SRM1_ADC1) || (channel == SRM1_ADC2))
		{
			data |= SRM1_DOUT;
		}
		if((channel == SRM2_ADC1) || (channel == SRM2_ADC2))
		{
			data |= SRM2_DOUT;
		}
	}
	DWT_Delay_us(SRM_ADC_Delay);
	SRM_ADC_Select(channel, UNSELECT_CHIP);
	return data;
}

//Access clamp and calibrate registers on SRM ADC circuit.
void SRM_ADC_ClampAndCalibrate(unsigned int adc_channel)
{
	switch(adc_channel)
	{
		case SRM1_ADC1_CH1:
			SRM_Write_ADC_Byte(SRM1_ADC1, 0x38);
			SRM_Write_ADC_Byte(SRM1_ADC1, 0x01);
			SRM_Write_ADC_Byte(SRM1_ADC1, 0x18);
		break;
		case SRM1_ADC1_CH2:
			SRM_Write_ADC_Byte(SRM1_ADC1, 0x3A);
			SRM_Write_ADC_Byte(SRM1_ADC1, 0x01);
			SRM_Write_ADC_Byte(SRM1_ADC1, 0x1A);
		break;
		case SRM1_ADC2_CH1:
			SRM_Write_ADC_Byte(SRM1_ADC2, 0x38);
			SRM_Write_ADC_Byte(SRM1_ADC2, 0x01);
			SRM_Write_ADC_Byte(SRM1_ADC2, 0x18);
		break;
		case SRM1_ADC2_CH2:
			SRM_Write_ADC_Byte(SRM1_ADC2, 0x3A);
			SRM_Write_ADC_Byte(SRM1_ADC2, 0x01);
			SRM_Write_ADC_Byte(SRM1_ADC2, 0x1A);
		break;
		case SRM2_ADC1_CH1:
			SRM_Write_ADC_Byte(SRM2_ADC1, 0x38);
			SRM_Write_ADC_Byte(SRM2_ADC1, 0x01);
			SRM_Write_ADC_Byte(SRM2_ADC1, 0x18);
		break;
		case SRM2_ADC1_CH2:
			SRM_Write_ADC_Byte(SRM2_ADC1, 0x3A);
			SRM_Write_ADC_Byte(SRM2_ADC1, 0x01);
			SRM_Write_ADC_Byte(SRM2_ADC1, 0x1A);
		break;
		case SRM2_ADC2_CH1:
			SRM_Write_ADC_Byte(SRM2_ADC2, 0x38);
			SRM_Write_ADC_Byte(SRM2_ADC2, 0x01);
			SRM_Write_ADC_Byte(SRM2_ADC2, 0x18);
		break;
		case SRM2_ADC2_CH2:
			SRM_Write_ADC_Byte(SRM2_ADC2, 0x3A);
			SRM_Write_ADC_Byte(SRM2_ADC2, 0x01);
			SRM_Write_ADC_Byte(SRM2_ADC2, 0x1A);
		break;
	}
}

void SRM_ADC_Set_Gain(unsigned int srm_adc_channel, unsigned int gain)
{
	SRM_ADC_ClampAndCalibrate(srm_adc_channel);
	switch(gain)
	{
		case 1:
			SRM_Write_ADC_24Bits(srm_adc_channel,0x200000); //pojacanje 1
			switch(srm_adc_channel)
			{
				//SRM Channel 1
				case SRM1_ADC1_CH1:
					SRM1_G_I = 2;
					SRM1_C_Gain = (float)SRM1_C_CH0_Gain[0]/1000000;
					SRM1_C_Offset = (float)SRM1_C_CH0_Offset[0]/1000000;
					if(SRM1_C_CH0_Sign[0] == '-') SRM1_C_Offset *= (-1.0);
				break;
				case SRM1_ADC1_CH2:
					SRM1_G_I = 32;
					SRM1_C_Gain = (float)SRM1_C_CH1_Gain[0]/1000000;
					SRM1_C_Offset = (float)SRM1_C_CH1_Offset[0]/1000000;
					if(SRM1_C_CH1_Sign[0] == '-') SRM1_C_Offset *= (-1.0);
				break;
				case SRM1_ADC2_CH1:
					SRM1_G_V = 1;
					SRM1_V_Gain = (float)SRM1_V_CH0_Gain[0]/1000000;
					SRM1_V_Offset = (float)SRM1_V_CH0_Gain[0]/1000000;
					if(SRM1_V_CH0_Sign[0] == '-') SRM1_V_Offset *= (-1.0);
				break;
				case SRM1_ADC2_CH2:
					SRM1_G_V = 16;
					SRM1_V_Gain = (float)SRM1_V_CH1_Gain[0]/1000000;
					SRM1_V_Offset = (float)SRM1_V_CH1_Gain[0]/1000000;
					if(SRM1_V_CH1_Sign[0] == '-') SRM1_V_Offset *= (-1.0);
				break;
				//SRM Channel 2
				case SRM2_ADC1_CH1:
					SRM2_G_I = 2;
					SRM2_C_Gain = (float)SRM2_C_CH0_Gain[0]/1000000;
					SRM2_C_Offset = (float)SRM2_C_CH0_Offset[0]/1000000;
					if(SRM2_C_CH0_Sign[0] == '-') SRM2_C_Offset *= (-1.0);
				break;
				case SRM2_ADC1_CH2:
					SRM2_G_I = 32;
					SRM2_C_Gain = (float)SRM2_C_CH1_Gain[0]/1000000;
					SRM2_C_Offset = (float)SRM2_C_CH1_Offset[0]/1000000;
					if(SRM2_C_CH1_Sign[0] == '-') SRM2_C_Offset *= (-1.0);
				break;
				case SRM2_ADC2_CH1:
					SRM2_G_V = 1;
					SRM2_V_Gain = (float)SRM2_V_CH0_Gain[0]/1000000;
					SRM2_V_Offset = (float)SRM2_V_CH0_Gain[0]/1000000;
					if(SRM2_V_CH0_Sign[0] == '-') SRM2_V_Offset *= (-1.0);
				break;
				case SRM2_ADC2_CH2:
					SRM2_G_V = 16;
					SRM2_V_Gain = (float)SRM2_V_CH1_Gain[0]/1000000;
					SRM2_V_Offset = (float)SRM2_V_CH1_Gain[0]/1000000;
					if(SRM2_V_CH1_Sign[0] == '-') SRM2_V_Offset *= (-1.0);
				break;
			}
		break;
		case 2:
			SRM_Write_ADC_24Bits(srm_adc_channel,0x400000); //pojacanje 2
			switch(srm_adc_channel)
			{
				//SRM Channel 1
				case SRM1_ADC1_CH1:
					SRM1_G_I = 4;
					SRM1_C_Gain = (float)SRM1_C_CH0_Gain[1]/1000000;
					SRM1_C_Offset = (float)SRM1_C_CH0_Offset[1]/1000000;
					if(SRM1_C_CH0_Sign[1] == '-') SRM1_C_Offset *= (-1.0);
				break;
				case SRM1_ADC1_CH2:
					SRM1_G_I = 64;
					SRM1_C_Gain = (float)SRM1_C_CH1_Gain[1]/1000000;
					SRM1_C_Offset = (float)SRM1_C_CH1_Offset[1]/1000000;
					if(SRM1_C_CH1_Sign[1] == '-') SRM1_C_Offset *= (-1.0);
				break;
				case SRM1_ADC2_CH1:
					SRM1_G_V = 2;
					SRM1_V_Gain = (float)SRM1_V_CH0_Gain[1]/1000000;
					SRM1_V_Offset = (float)SRM1_V_CH0_Gain[1]/1000000;
					if(SRM1_V_CH0_Sign[1] == '-') SRM1_V_Offset *= (-1.0);
				break;
				case SRM1_ADC2_CH2:
					SRM1_G_V = 32;
					SRM1_V_Gain = (float)SRM1_V_CH1_Gain[1]/1000000;
					SRM1_V_Offset = (float)SRM1_V_CH1_Gain[1]/1000000;
					if(SRM1_V_CH1_Sign[1] == '-') SRM1_V_Offset *= (-1.0);
				break;
				//SRM Channel 2
				case SRM2_ADC1_CH1:
					SRM2_G_I = 4;
					SRM2_C_Gain = (float)SRM2_C_CH0_Gain[1]/1000000;
					SRM2_C_Offset = (float)SRM2_C_CH0_Offset[1]/1000000;
					if(SRM2_C_CH0_Sign[1] == '-') SRM2_C_Offset *= (-1.0);
				break;
				case SRM2_ADC1_CH2:
					SRM2_G_I = 64;
					SRM2_C_Gain = (float)SRM2_C_CH1_Gain[1]/1000000;
					SRM2_C_Offset = (float)SRM2_C_CH1_Offset[1]/1000000;
					if(SRM2_C_CH1_Sign[1] == '-') SRM2_C_Offset *= (-1.0);
				break;
				case SRM2_ADC2_CH1:
					SRM2_G_V = 2;
					SRM2_V_Gain = (float)SRM2_V_CH0_Gain[1]/1000000;
					SRM2_V_Offset = (float)SRM2_V_CH0_Gain[1]/1000000;
					if(SRM2_V_CH0_Sign[1] == '-') SRM2_V_Offset *= (-1.0);
				break;
				case SRM2_ADC2_CH2:
					SRM2_G_V = 32;
					SRM2_V_Gain = (float)SRM2_V_CH1_Gain[1]/1000000;
					SRM2_V_Offset = (float)SRM2_V_CH1_Gain[1]/1000000;
					if(SRM2_V_CH1_Sign[1] == '-') SRM2_V_Offset *= (-1.0);
				break;
			}
		break;
		case 4:
			SRM_Write_ADC_24Bits(srm_adc_channel,0x800000); //pojacanje 4
			switch(srm_adc_channel)
			{
				//SRM Channel 1
				case SRM1_ADC1_CH1:
					SRM1_G_I = 8;
					SRM1_C_Gain = (float)SRM1_C_CH0_Gain[2]/1000000;
					SRM1_C_Offset = (float)SRM1_C_CH0_Offset[2]/1000000;
					if(SRM1_C_CH0_Sign[2] == '-') SRM1_C_Offset *= (-1.0);
				break;
				case SRM1_ADC1_CH2:
					SRM1_G_I = 128;
					SRM1_C_Gain = (float)SRM1_C_CH1_Gain[2]/1000000;
					SRM1_C_Offset = (float)SRM1_C_CH1_Offset[2]/1000000;
					if(SRM1_C_CH1_Sign[2] == '-') SRM1_C_Offset *= (-1.0);
				break;
				case SRM1_ADC2_CH1:
					SRM1_G_V = 4;
					SRM1_V_Gain = (float)SRM1_V_CH0_Gain[2]/1000000;
					SRM1_V_Offset = (float)SRM1_V_CH0_Gain[2]/1000000;
					if(SRM1_V_CH0_Sign[2] == '-') SRM1_V_Offset *= (-1.0);
				break;
				case SRM1_ADC2_CH2:
					SRM1_G_V = 64;
					SRM1_V_Gain = (float)SRM1_V_CH1_Gain[2]/1000000;
					SRM1_V_Offset = (float)SRM1_V_CH1_Gain[2]/1000000;
					if(SRM1_V_CH1_Sign[2] == '-') SRM1_V_Offset *= (-1.0);
				break;
				//SRM Channel 2
				case SRM2_ADC1_CH1:
					SRM2_G_I = 8;
					SRM2_C_Gain = (float)SRM2_C_CH0_Gain[2]/1000000;
					SRM2_C_Offset = (float)SRM2_C_CH0_Offset[2]/1000000;
					if(SRM2_C_CH0_Sign[2] == '-') SRM2_C_Offset *= (-1.0);
				break;
				case SRM2_ADC1_CH2:
					SRM2_G_I = 128;
					SRM2_C_Gain = (float)SRM2_C_CH1_Gain[2]/1000000;
					SRM2_C_Offset = (float)SRM2_C_CH1_Offset[2]/1000000;
					if(SRM2_C_CH1_Sign[2] == '-') SRM2_C_Offset *= (-1.0);
				break;
				case SRM2_ADC2_CH1:
					SRM2_G_V = 4;
					SRM2_V_Gain = (float)SRM2_V_CH0_Gain[2]/1000000;
					SRM2_V_Offset = (float)SRM2_V_CH0_Gain[2]/1000000;
					if(SRM2_V_CH0_Sign[2] == '-') SRM2_V_Offset *= (-1.0);
				break;
				case SRM2_ADC2_CH2:
					SRM2_G_V = 64;
					SRM2_V_Gain = (float)SRM2_V_CH1_Gain[2]/1000000;
					SRM2_V_Offset = (float)SRM2_V_CH1_Gain[2]/1000000;
					if(SRM2_V_CH1_Sign[2] == '-') SRM2_V_Offset *= (-1.0);
				break;
			}
		break;
		case 8:
			SRM_Write_ADC_24Bits(srm_adc_channel,0xFFFFFF); //pojacanje 8
			switch(srm_adc_channel)
			{
				//SRM Channel 1
				case SRM1_ADC1_CH1:
					SRM1_G_I = 16;
					SRM1_C_Gain = (float)SRM1_C_CH0_Gain[3]/1000000;
					SRM1_C_Offset = (float)SRM1_C_CH0_Offset[3]/1000000;
					if(SRM1_C_CH0_Sign[3] == '-') SRM1_C_Offset *= (-1.0);
				break;
				case SRM1_ADC1_CH2:
					SRM1_G_I = 255;
					SRM1_C_Gain = (float)SRM1_C_CH1_Gain[3]/1000000;
					SRM1_C_Offset = (float)SRM1_C_CH1_Offset[3]/1000000;
					if(SRM1_C_CH1_Sign[3] == '-') SRM1_C_Offset *= (-1.0);
				break;
				case SRM1_ADC2_CH1:
					SRM1_G_V = 8;
					SRM1_V_Gain = (float)SRM1_V_CH0_Gain[3]/1000000;
					SRM1_V_Offset = (float)SRM1_V_CH0_Gain[3]/1000000;
					if(SRM1_V_CH0_Sign[3] == '-') SRM1_V_Offset *= (-1.0);
				break;
				case SRM1_ADC2_CH2:
					SRM1_G_V = 128;
					SRM1_V_Gain = (float)SRM1_V_CH1_Gain[3]/1000000;
					SRM1_V_Offset = (float)SRM1_V_CH1_Gain[3]/1000000;
					if(SRM1_V_CH1_Sign[3] == '-') SRM1_V_Offset *= (-1.0);
				break;
				//SRM Channel 2
				case SRM2_ADC1_CH1:
					SRM2_G_I = 16;
					SRM2_C_Gain = (float)SRM2_C_CH0_Gain[3]/1000000;
					SRM2_C_Offset = (float)SRM2_C_CH0_Offset[3]/1000000;
					if(SRM2_C_CH0_Sign[3] == '-') SRM2_C_Offset *= (-1.0);
				break;
				case SRM2_ADC1_CH2:
					SRM2_G_I = 255;
					SRM2_C_Gain = (float)SRM2_C_CH1_Gain[3]/1000000;
					SRM2_C_Offset = (float)SRM2_C_CH1_Offset[3]/1000000;
					if(SRM2_C_CH1_Sign[3] == '-') SRM2_C_Offset *= (-1.0);
				break;
				case SRM2_ADC2_CH1:
					SRM2_G_V = 8;
					SRM2_V_Gain = (float)SRM2_V_CH0_Gain[3]/1000000;
					SRM2_V_Offset = (float)SRM2_V_CH0_Gain[3]/1000000;
					if(SRM2_V_CH0_Sign[3] == '-') SRM2_V_Offset *= (-1.0);
				break;
				case SRM2_ADC2_CH2:
					SRM2_G_V = 128;
					SRM2_V_Gain = (float)SRM2_V_CH1_Gain[3]/1000000;
					SRM2_V_Offset = (float)SRM2_V_CH1_Gain[3]/1000000;
					if(SRM2_V_CH1_Sign[3] == '-') SRM2_V_Offset *= (-1.0);
				break;
			}
		break;
	}
	
	switch(srm_adc_channel)
	{
		case SRM1_ADC1_CH1:
		case SRM1_ADC2_CH1:
		case SRM2_ADC1_CH1:	
		case SRM2_ADC2_CH1:	
			SRM_Write_ADC_Byte(srm_adc_channel, 0x38);
		break;
		case SRM1_ADC1_CH2:
		case SRM1_ADC2_CH2:
		case SRM2_ADC1_CH2:
		case SRM2_ADC2_CH2:
			SRM_Write_ADC_Byte(srm_adc_channel, 0x3A);
		break;
	}
	SRM_Write_ADC_Byte(srm_adc_channel, 0x21);
}

//Test_Current inicijaliziran na nulu, promijeniti u toku testiranja. Kasnije napraviti da se moze odabrati.
void SRM_Get_Samples(void)
{
	Test_Phase = START_PHASE;
	switch(Test_Phase)
	{
		case START_PHASE:
			cptr[0]=0;
      cptr[1]=0;
      cptr[2]=0;
      cptr[3]=0;
			C_buffer_RET[0]=0; 
			C_buffer_RET[1]=0;
			C_buffer[0][0]=32768; C_buffer[1][0]=32768; C_buffer[2][0]=32768; C_buffer[3][0]=32768;
      C_buffer[0][1]=32768; C_buffer[1][1]=32768; C_buffer[2][1]=32768; C_buffer[3][1]=32768;
			if(RemoteTest_h == 0)
			{
				SRM_ADC_Set_Gain(SRM1_ADC1_CH1, 1);
				SRM_ADC_Set_Gain(SRM1_ADC1_CH2, 1);
				SRM_ADC_Set_Gain(SRM1_ADC2_CH1, 1);
				SRM_ADC_Set_Gain(SRM1_ADC2_CH2, 1);
				SRM_ADC_Set_Gain(SRM2_ADC1_CH1, 1);
				SRM_ADC_Set_Gain(SRM2_ADC1_CH2, 1);
				SRM_ADC_Set_Gain(SRM2_ADC2_CH1, 1);
				SRM_ADC_Set_Gain(SRM2_ADC2_CH2, 1);
				SRM1_G_I = 32;
				SRM1_G_V = 16;
				SRM2_G_I = 32;
				SRM2_G_V = 16;
			}
			Test_Count = 0;
			SRM_SYNC_HIGH;
			DRM_DAC_Write(0, CHANNEL1);
			DRM_DAC_Write(0, CHANNEL2);
			Test_Phase = RAMP_UP_PHASE;
			break;
		case RAMP_UP_PHASE:
			//ramp function for SRM: postavljeno da zadaje istu vrijednost na oba kanala
			if(Test_Count == 2)
			{
				DRM_DAC_Write(DACV_Reg[1], CHANNEL1);
				DRM_DAC_Write(DACV_Reg[1], CHANNEL2);
			}
			if(Test_Count == 4)
			{
				DRM_DAC_Write(DACV_Reg[2], CHANNEL1);
				DRM_DAC_Write(DACV_Reg[2], CHANNEL2);
			}	
			if(Test_Count == 6)
			{
				DRM_DAC_Write(DACV_Reg[3], CHANNEL1);
				DRM_DAC_Write(DACV_Reg[3], CHANNEL2);
			}	
			if(Test_Count == 8)
			{
				DRM_DAC_Write(DACV_Reg[4], CHANNEL1);
				DRM_DAC_Write(DACV_Reg[4], CHANNEL2);
			}	
			if(Test_Current < 5)
			{
				if(Test_Count >= 48)
				{
					if(RemoteTest == 0)
					{
						Test_Phase = DETECT_RANGE1_PHASE; 
						Test_Count = 0;
					}
					else
					{
						cptr[0]=0;
						cptr[1]=0;
						cptr[2]=0;
						cptr[3]=0;
						Test_Count = 0;
						Test_Phase = PAUSE_PHASE;
					}
				}
			}
			else if(Test_Count >= 18)
			{
				if(RemoteTest == 0)
				{
					Test_Phase = DETECT_RANGE1_PHASE;
				}
				else
				{
						cptr[0]=0;
						cptr[1]=0;
						cptr[2]=0;
						cptr[3]=0;
						Test_Phase = PAUSE_PHASE;
				}
				Test_Count = 0;
			}
			break;
		case DETECT_RANGE1_PHASE:
			if(Test_Count % 2 == 0) //If test counter is even read ADC1 IC, if odd read ADC2 IC. Ubaciti i za SRM 2 citanje.
			{
				if(Check_ADC_Ready(SRM1_ADC1) == 1)
				{
					SRM_Write_ADC_Byte(SRM1_ADC1, 0x44); //Check ADC status register
					if((SRM_Read_ADC_Byte(SRM1_ADC1) & 0x01) == 0x01) //If status RDY0 == 1
					{
						SRM_Write_ADC_Byte(SRM1_ADC1, 0x48); //Setup read from Channel Data register
						AD[0] = SRM_Read_ADC_16Bits(SRM1_ADC1);
						if(AD[0]>C_buffer[0][0])C_buffer[0][0]=AD[0];
						if(AD[0]<C_buffer[0][1])C_buffer[0][1]=AD[0];
						C_buffer[0][cptr[0]]=AD[0];
						if(cptr[0] >= 214) cptr[0] = 200;
						else cptr[0]++; 
					}
					SRM_Write_ADC_Byte(SRM1_ADC1, 0x44);
					if((SRM_Read_ADC_Byte(SRM1_ADC1) & 0x04) == 0x04)
					{
						SRM_Write_ADC_Byte(SRM1_ADC1, 0x4A);
						AD[2] = SRM_Read_ADC_16Bits(SRM1_ADC1);
						if(AD[2]>C_buffer[2][0])C_buffer[2][0]=AD[2];
						if(AD[2]<C_buffer[2][1])C_buffer[2][1]=AD[2];
						C_buffer[2][cptr[2]]=AD[2];
						if(cptr[2] >= 214) cptr[2] = 200;
						else cptr[2]++; 
					}
				}				
			}
			else	
			{
				if(Check_ADC_Ready(SRM1_ADC2) == 1)
				{
					SRM_Write_ADC_Byte(SRM1_ADC2, 0x44);
					if((SRM_Read_ADC_Byte(SRM1_ADC2) & 0x01) == 0x01) //If status RDY0 == 1
					{
						SRM_Write_ADC_Byte(SRM1_ADC2, 0x48); //Setup read from Channel Data register
						AD[1] = SRM_Read_ADC_16Bits(SRM1_ADC2);
						if(AD[1]>C_buffer[1][0])C_buffer[1][0]=AD[1];
						if(AD[1]<C_buffer[1][1])C_buffer[1][1]=AD[1];
						C_buffer[1][cptr[1]]=AD[1];
						if(cptr[1] >= 214) cptr[1] = 200;
						else cptr[1]++; 
					}
					SRM_Write_ADC_Byte(SRM1_ADC2, 0x44);
					if((SRM_Read_ADC_Byte(SRM1_ADC2) & 0x04) == 0x04)
					{
						SRM_Write_ADC_Byte(SRM1_ADC2, 0x4A);
						AD[3] = SRM_Read_ADC_16Bits(SRM1_ADC2);
						if(AD[3]>C_buffer[3][0])C_buffer[3][0]=AD[3];
						if(AD[3]<C_buffer[3][1])C_buffer[3][1]=AD[3];
						C_buffer[3][cptr[3]]=AD[3];
						if(cptr[3] >= 214) cptr[3] = 200;
						else cptr[3]++; 
					}
				}
			}
			if(Test_Count >= 30)
			{
				Test_Phase = DT_PAUSE_PHASE;
				Test_Count = 0;
				if((C_buffer[0][0]-32768)<(32768-(C_buffer[0][1]))){C_buffer[0][0]=C_buffer[0][1];}
				if((C_buffer[1][0]-32768)<(32768-(C_buffer[1][1]))){C_buffer[1][0]=C_buffer[1][1];}
				if((C_buffer[2][0]-32768)<(32768-(C_buffer[2][1]))){C_buffer[2][0]=C_buffer[2][1];}
				if((C_buffer[3][0]-32768)<(32768-(C_buffer[3][1]))){C_buffer[3][0]=C_buffer[3][1];}
      }
			break;
		case DETECT_RANGE2_PHASE:
			if(Test_Count == 1)
			{
				if((C_buffer[2][0] > 48000)||(C_buffer[2][0] < 17536)){
					if((C_buffer[0][0] < 34500)&&(C_buffer[0][0] > 31036))  
					{
						SRM_ADC_Set_Gain(SRM1_ADC1_CH1, 8);
						C_buffer_RET[0]=C_buffer[0][0];
					}
					else                                                    
					{
						SRM_ADC_Set_Gain(SRM1_ADC1_CH1, 4);
						C_buffer_RET[0]=C_buffer[0][0];
					}
				}
				else
				{
					if((C_buffer[2][0] < 34500)&&(C_buffer[2][0] > 31036))  
					{
						SRM_ADC_Set_Gain(SRM1_ADC1_CH2, 8);
						C_buffer_RET[0]=C_buffer[2][0];
					}
					else if((C_buffer[2][0] < 36500)&&(C_buffer[2][0] > 29036)) 
					{
						SRM_ADC_Set_Gain(SRM1_ADC1_CH2, 4);
						C_buffer_RET[0]=C_buffer[2][0];
					}
					else if((C_buffer[2][0] < 39500)&&(C_buffer[2][0] > 26036))  
					{
						SRM_ADC_Set_Gain(SRM1_ADC1_CH2, 2);
						C_buffer_RET[0]=C_buffer[2][0];
					}
				}
			}
			if(Test_Count == 2)
			{
				if((C_buffer[3][0] > 48000)||(C_buffer[3][0] < 17536))
				{
					if((C_buffer[1][0] < 34500)&&(C_buffer[1][0] > 31036))  
					{
						SRM_ADC_Set_Gain(SRM1_ADC2_CH1, 8);
						C_buffer_RET[1]=C_buffer[1][0];
					}
					else if((C_buffer[1][0] < 36500)&&(C_buffer[1][0] > 29036))  
					{
						SRM_ADC_Set_Gain(SRM1_ADC2_CH1, 4);
						C_buffer_RET[1]=C_buffer[1][0];
					}
					else if((C_buffer[1][0] < 39500)&&(C_buffer[1][0] > 26036)) 
					{
						SRM_ADC_Set_Gain(SRM1_ADC2_CH1, 2);
						C_buffer_RET[1]=C_buffer[1][0];
					}
					else
					{
						SRM_ADC_Set_Gain(SRM1_ADC2_CH1, 1);
						C_buffer_RET[1]=C_buffer[1][0];
					}
				}
				else
				{
					if((C_buffer[3][0] < 34500)&&(C_buffer[3][0] > 31036))  
					{
						SRM_ADC_Set_Gain(SRM1_ADC2_CH2, 8);
						C_buffer_RET[1]=C_buffer[3][0];
					}
					else if((C_buffer[3][0] < 36500)&&(C_buffer[3][0] > 29036))  
					{
						SRM_ADC_Set_Gain(SRM1_ADC2_CH2, 4);
						C_buffer_RET[1]=C_buffer[3][0];
					}
					else if((C_buffer[3][0] < 39500)&&(C_buffer[3][0] > 26036))  
					{
						SRM_ADC_Set_Gain(SRM1_ADC2_CH2, 2);
						C_buffer_RET[1]=C_buffer[3][0];
					}
				}
			}
			if(Test_Count >= 3)
			{
				cptr[0]=0;
				cptr[1]=0;
				cptr[2]=0;
				cptr[3]=0;
				Test_Count = 0;
				//if(DeadTank){ Test_Phase = DT_PAUSE_PH; }
				Test_Phase = GET_SAMPLES_PHASE;
			}			
			break;
		case PAUSE_PHASE:
			if(Test_Count >= 34)
			{
				cptr[0]=0;
				cptr[1]=0;
				cptr[2]=0;
				cptr[3]=0;
				Test_Count = 0;
				Test_Phase = GET_SAMPLES_PHASE;
			}
			break;
		case DT_PAUSE_PHASE:
//			if(DeadTank==2){
//					Test_Count = 0;
//					Test_Phase = DETECT_RANGE2_PH;
//			}
		break;
		case GET_SAMPLES_PHASE:
			if(Test_Count%2 == 0){  				
				if(Check_ADC_Ready(SRM1_ADC1))
				{
					SRM_Write_ADC_Byte(SRM1_ADC1, 0x44); 
					if((SRM_Read_ADC_Byte(SRM1_ADC1)&0x01)==0x01)
					{
							SRM_Write_ADC_Byte(SRM1_ADC1,0x48);
							AD[0]=SRM_Read_ADC_16Bits(SRM1_ADC1);
							C_buffer[0][cptr[0]]=AD[0];
							cptr[0]++;
							if(cptr[0]==320) cptr[0]=0;
					}
					SRM_Write_ADC_Byte(SRM1_ADC1, 0x44);
					if((SRM_Read_ADC_Byte(SRM1_ADC1)&0x04)==0x04)
					{
							SRM_Write_ADC_Byte(SRM1_ADC1,0x4A);
							AD[2]=SRM_Read_ADC_16Bits(SRM1_ADC1);
							C_buffer[2][cptr[2]]=AD[2];
							cptr[2]++;
							if(cptr[2]==320) cptr[2]=0;
					}
				}
			}
		  else{    
				if(Check_ADC_Ready(SRM1_ADC2))
				{
					SRM_Write_ADC_Byte(SRM1_ADC2, 0x44);
					if((SRM_Read_ADC_Byte(SRM1_ADC2)&0x01)==0x01)
					{
							SRM_Write_ADC_Byte(SRM1_ADC2,0x48);
							AD[1]=SRM_Read_ADC_16Bits(SRM1_ADC2);
							C_buffer[1][cptr[1]]=AD[1];
							cptr[1]++;
							if(cptr[1]==320) cptr[1]=0;
					}
					SRM_Write_ADC_Byte(SRM1_ADC2, 0x44);
					if((SRM_Read_ADC_Byte(SRM1_ADC2)&0x04)==0x04)
					{
							SRM_Write_ADC_Byte(SRM1_ADC2,0x4A);
							AD[3]=SRM_Read_ADC_16Bits(SRM1_ADC2);
							C_buffer[3][cptr[3]]=AD[3];
							cptr[3]++;
							if(cptr[3]==320) cptr[3]=0;
					}
				}
      }
			if(Test_Count >= TestDuration)
			{
				//if(DeadTank==2) Test_Phase = DT_PAUSE2_PH;
				//else{Test_Phase = RAMP2_PH;}
				Test_Phase = RAMP_DOWN_PHASE;
				Test_Count = 0;
			}
			break;
		case RAMP_DOWN_PHASE:
			if(Test_Count > 1) DRM_DAC_Write(DACV_Reg[3], CHANNEL1);
			else if(Test_Count > 3) DRM_DAC_Write(DACV_Reg[2], CHANNEL1);
			else if(Test_Count > 5) DRM_DAC_Write(DACV_Reg[1], CHANNEL1);
			else if(Test_Count > 7) DRM_DAC_Write(DACV_Reg[0], CHANNEL1);						
			else if(Test_Count > 9)
			{/*TestSTOP;*/ 
//				SyncSTOP;
//				Meas_flag = Test_Count; 
//				SPI_24_Write(0); 
//				OutSTOP; 
//				UV_Enable; 
				Test_Phase = END_PHASE;
			}
			break;
		case END_PHASE:
			timer2_SRM_ON = 0;
			//Dodati ovdje dodatna osiguranja da je tranzistor zatvoren.
			break;
	}	
}

unsigned int Check_ADC_Ready(unsigned int channel)
{
	unsigned int ADC_ready = 0;
	switch(channel)
	{
		case SRM1_ADC1:		
			if(SRM1_RDY1 == RESET) ADC_ready = 1;
		break;
		case SRM1_ADC2:	
			if(SRM1_RDY2 == RESET) ADC_ready = 1;
		break;
		case SRM2_ADC1:
			if(SRM2_RDY1 == RESET) ADC_ready = 1;
		break;
		case SRM2_ADC2:	
			if(SRM2_RDY2 == RESET) ADC_ready = 1;
		break;
	}
	return ADC_ready;
}






















