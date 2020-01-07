#include "main.h"
#include <bios.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "calib.h"
#include "adc.h"
#include "init.h"
#include "comm.h"

ADC_STRUCT     ADC_Results;
ADC8BIT_STRUCT ADC_8bit_results;

unsigned char ADCdataL[8];
unsigned char ADCdataH[8];

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
	uint8_t current_data[16] = 0;
	uint8_t voltage_data[16] = 0;
	uint16_t bit_operator = 0x8000;
	//DRM Channels buffers
	ADC_Results.ANCH[0]=0; 
	ADC_Results.ANCH[1]=0;
	
	DRM1_ADC_CLK_RESET;
	DELAY_IN_us;
	DRM1_ADC_CS_RESET;
	for(i=0; i<6; i++)
	{
		DELAY_IN_us;
		DRM1_ADC_CLK_SET;
		DELAY_IN_us;
		DRM1_ADC_CLK_RESET;
	}
		for(i=0; i<16; i++)
	{
		DELAY_IN_us;
		DRM1_ADC_CLK_SET;
		DELAY_IN_us;
		current_data[i] = DRM1_ADC_CURR1;
		voltage_data[i] = DRM1_ADC_VOUT1;
		DRM1_ADC_CLK_RESET;
	}
	DELAY_IN_us;
	DRM1_ADC_CS_SET;
	
	for(i=0; i<16; i++)
	{
		if(current_data[i] == 1) ADC_Results.ANCH[0] |= bit_operator;
		if(voltage_data[i] == 1) ADC_Results.ANCH[1] |= bit_operator;
		bit_operator = bit_operator >> 1;
	}	
}

/* READ All DRM Channels */

void DRM1_ADC_Read_All(void)
{
	unsigned int i;
	uint8_t current_data1[16] = 0;
	uint8_t voltage_data1[16] = 0;
	uint8_t current_data2[16] = 0;
	uint8_t voltage_data2[16] = 0;
	uint8_t current_data3[16] = 0;
	uint8_t voltage_data3[16] = 0;
	uint16_t bit_operator = 0x8000;
	//DRM Channels buffers
	ADC_Results.ANCH[0]=0; 
	ADC_Results.ANCH[1]=0;
	ADC_Results.ANCH[2]=0; 
	ADC_Results.ANCH[3]=0;
	ADC_Results.ANCH[4]=0; 
	ADC_Results.ANCH[5]=0;
	
	DRM1_ADC_CLK_RESET;
	DELAY_IN_us;
	DRM1_ADC_CS_RESET;
	for(i=0; i<6; i++)
	{
		DELAY_IN_us;
		DRM1_ADC_CLK_SET;
		DELAY_IN_us;
		DRM1_ADC_CLK_RESET;
	}
		for(i=0; i<16; i++)
	{
		DELAY_IN_us;
		DRM1_ADC_CLK_SET;
		DELAY_IN_us;
		current_data1[i] = DRM1_ADC_CURR1;
		voltage_data1[i] = DRM1_ADC_VOUT1;
		current_data2[i] = DRM1_ADC_CURR2;
		voltage_data2[i] = DRM1_ADC_VOUT2;
		current_data3[i] = DRM1_ADC_CURR3;
		voltage_data3[i] = DRM1_ADC_VOUT3;
		DRM1_ADC_CLK_RESET;
	}
	DELAY_IN_us;
	DRM1_ADC_CS_SET;
	
	for(i=0; i<16; i++)
	{
		if(current_data1[i] == 1) ADC_Results.ANCH[0] |= bit_operator;
		if(voltage_data1[i] == 1) ADC_Results.ANCH[1] |= bit_operator;
		if(current_data2[i] == 1) ADC_Results.ANCH[2] |= bit_operator;
		if(voltage_data2[i] == 1) ADC_Results.ANCH[3] |= bit_operator;
		if(current_data3[i] == 1) ADC_Results.ANCH[4] |= bit_operator;
		if(voltage_data3[i] == 1) ADC_Results.ANCH[5] |= bit_operator;
		bit_operator = bit_operator >> 1;
	}	
}



