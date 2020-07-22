#include "main.h"
#include "bios.h"
#include "fatfs.h"
#include "stdio.h"
#include "string.h"

#include "adc.h"
#include "bios.h"
#include "comm.h"
#include "init.h"
#include "control.h"
#include "dwt_delay.h"

//MCU Battery Charger Control
uint8_t MCU_bat_chg_ctrl = 0x02;

unsigned char language = 0; //English is default language

uint8_t PORT607; 
uint8_t PORT608;
uint8_t PORT609;
uint8_t PORT610;

uint8_t BSG_Relay_Control=0x00; //Treba staviti pocetnu vrijednost i napraviti INit funkciju!

TESTPAR_STRUCT TESTPAR;
COMM_STRUCT  COMM;
BDATA_STRUCT BDATA;
TDATA_STRUCT TDATA;
TIMEDATE_STRUCT TIMEDATE;
STICK_STRUCT STICK;
CALIB_STRUCT CALIB;

//Results:
MC_RESULTS_STRUCT      MainC_Results;
CURR_RESULTS_STRUCT    Current_Results;
AUX_RESULTS_STRUCT     AUX_Results;
ANALOG_RESULTS_STRUCT  AnalogCH_Results;
LINEAR_RESULTS_STRUCT  LTransd_Results;
DIGITAL_RESULTS_STRUCT DTransd_Results;
RMO_RESULTS_STRUCT     RMO_Results;

//SRM ADC Data struct
SRM_ADC_DATA_ARRAY SRM_ADC_Data_Array;
unsigned int SRM_ADC_Data[8];

#if 1
void delay_us(unsigned int us)
{
	for(unsigned int i=0; i<200*us; i++)
	{
		__nop();
	}
}
#endif

#if 0
uint32_t DWT_Get(void)
{
  return DWT->CYCCNT;
}

void delay_us(uint32_t us) // microseconds
{
  uint32_t dp = us * (SystemCoreClock/1000000);
  uint32_t d,n,sp=DWT_Get();
  do
  {
	  //ALTrst;
	  n=DWT_Get();
	  if(n>=sp)
	  {
		  d=n-sp;
		  if(d>=dp)
		  {
			  return;
		  }
		  
	  }
	  else
	  {
		  d=sp-n;
		  d=~d;
		  d++;
		  if(d>=dp)
		  {
			  return;
		  }
	  }
  }while(1);
}
#endif

void BSGRelayControl(unsigned char BSG_RELAY, unsigned char On_Off)
{
    if(On_Off==ON)
    {   
        BSG_Relay_Control&=~(BSG_RELAY);            
    }
    else
    {
        BSG_Relay_Control|=BSG_RELAY;    
    }                 
HAL_I2C_Master_Transmit(&hi2c1, BSG_CONTROL_PINS, (uint8_t*)(&BSG_Relay_Control), 1, 1000);
}

void BSGBuckControl(unsigned char On_Off)
{
    if(On_Off==ON)
    {   
        BSG_Relay_Control|=BSG_3V3_BUCK;                    
    }
    else
    {
        BSG_Relay_Control&=~(BSG_3V3_BUCK);    
    } 
HAL_I2C_Master_Transmit(&hi2c1,BSG_CONTROL_PINS,(uint8_t*)&BSG_Relay_Control,1, 1000);
}  

void BSGChargeDischargeRELControl(unsigned int CHARGE_DISCHARGE_REL, unsigned char On_Off)
{
    if(On_Off==ON)
    {                     
        BSG_Relay_Control|=CHARGE_DISCHARGE_REL;                    
    }
    else
    {
        BSG_Relay_Control&=~(CHARGE_DISCHARGE_REL);    
    } 
HAL_I2C_Master_Transmit(&hi2c1,BSG_CONTROL_PINS,(uint8_t*)&BSG_Relay_Control,1, 1000);
}

void BSGConfigON(void)
{
BSG_Relay_Control|=OSG_BSG_REL_CTRL;      
HAL_I2C_Master_Transmit(&hi2c1,BSG_CONTROL_PINS,(uint8_t*)&BSG_Relay_Control,1, 1000);
}

void OSGConfigON(void)
{
BSG_Relay_Control&=~(OSG_BSG_REL_CTRL);      
HAL_I2C_Master_Transmit(&hi2c1,BSG_CONTROL_PINS,(uint8_t*)&BSG_Relay_Control,1, 1000);
}

void BSGSOneBPPF_ON_Sequence(void)
{
                    //Turn BGS ON:
                    OSG_OFF_BSG_ON; 
                    HAL_Delay(300);  
                    BSG_REL12_OFF;
                    BSG_REL3_ON;                    
                    HAL_Delay(150);  
                    ////////////////                    
                    BSG_DISCHARGE_REL_ON; //discharge capacitor
                    HAL_Delay(500);
                    //BSG capacitors discharged, start BSG sequence:
                    BSG_DISCHARGE_REL_OFF; //release capacitor
                    ////////////////     
                    BSG_CHARGE_REL_ON;               
                    HAL_Delay(50);
                    BSG_BUCK_ON;
                    HAL_Delay(1500);
                    //release BSG capaciotrs. device prepared for BSG measurement:
                    BSG_CHARGE_REL_OFF;                                         
                    HAL_Delay(100);                                            
}

void BSGSTwoBPPF_ON_Sequence(void)
{
                    //Turn BGS ON:
                    OSG_OFF_BSG_ON;  
                    HAL_Delay(300);  
                    BSG_REL12_ON;
                    BSG_REL3_OFF;                    
                    HAL_Delay(150);
                    ////////////////                    
                    BSG_DISCHARGE_REL_ON; //discharge capacitor
                    HAL_Delay(500);
                    //BSG capacitors discharged, start BSG sequence:
                    BSG_DISCHARGE_REL_OFF; //release capacitor
                    ////////////////     
                    BSG_CHARGE_REL_ON;                                         
                    HAL_Delay(50);                                                   
                    BSG_BUCK_ON;
                    HAL_Delay(1500);
                    //release BSG capaciotrs. device prepared for BSG measurement:
                    BSG_CHARGE_REL_OFF;                                         
                    HAL_Delay(100); 
}

#if 1

void BSG_OFF_Sequence(void)
{

                    //Turn off 3V3 BUCK:
                    BSG_BUCK_OFF;    
                    HAL_Delay(1000);
                    //Turn off BOTH BSG Relays: 
                    BSG_REL12_OFF;
                    BSG_REL3_OFF;          
                    ///////////////////////
                    //ubacio 03.01 (da isprazni kondenzator):
                    BSG_CHARGE_REL_ON;
                    ///////////////////////                    
                    HAL_Delay(300);                                           
                    BSG_DISCHARGE_REL_OFF; 
                    BSG_CHARGE_REL_OFF;                              
                    //Go BACK to OSG:
                    BSG_OFF_OSG_ON;                                                        
                    HAL_Delay(300);
}

void MainContactsInitSequence(void)
{           
    BSG_OFF_OSG_ON;               
    BSG_BUCK_OFF;     
    BSG_REL12_OFF;
    BSG_REL3_OFF;       
}

void GPIO_Expanders_InitSequence(void)
{

}


void SetAnalogCHRange(unsigned char Channel, unsigned char Range)
{
unsigned char GainGPIO_Expander;
unsigned char Channel_Gain;
unsigned char Channel_Relay;


#if (!USE_60V_ANALOG_CH_RANGE)
unsigned char range_changed=0;
//if 60V range is selected, use 300V range:
if(Range==ANALOG_CH_RANGE_HL){Range=ANALOG_CH_RANGE_HH; range_changed=1;}
#endif    

    switch(Channel)
    {
        case ANALOGCH1:                      
            GainGPIO_Expander=ANALOG_CH12_CONTROL;
            Channel_Gain=ANALOG_CH1_GAIN_CTRL; 
            Channel_Relay=ANALOG_CH1_RELAY_CTRL;                    
            TESTPAR.analog1_gain=Range;            
            #if (!USE_60V_ANALOG_CH_RANGE)
            if(range_changed){TESTPAR.analog1_gain=ANALOG_CH_RANGE_HL;} 
            #endif      
        break;
        case ANALOGCH2:
            GainGPIO_Expander=ANALOG_CH12_CONTROL;
            Channel_Gain=ANALOG_CH2_GAIN_CTRL; 
            Channel_Relay=ANALOG_CH2_RELAY_CTRL;
            TESTPAR.analog2_gain=Range;  
            #if (!USE_60V_ANALOG_CH_RANGE)
            if(range_changed){TESTPAR.analog2_gain=ANALOG_CH_RANGE_HL;} 
            #endif                          
        break;
        case ANALOGCH3:             
            GainGPIO_Expander=ANALOG_CH34_CONTROL;
            Channel_Gain=ANALOG_CH3_GAIN_CTRL; 
            Channel_Relay=ANALOG_CH3_RELAY_CTRL;
            TESTPAR.analog3_gain=Range;
            #if (!USE_60V_ANALOG_CH_RANGE)
            if(range_changed){TESTPAR.analog3_gain=ANALOG_CH_RANGE_HL;} 
            #endif                         
        break;
        case ANALOGCH4:
            GainGPIO_Expander=ANALOG_CH34_CONTROL;
            Channel_Gain=ANALOG_CH4_GAIN_CTRL; 
            Channel_Relay=ANALOG_CH4_RELAY_CTRL;         
            TESTPAR.analog4_gain=Range;
            #if (!USE_60V_ANALOG_CH_RANGE)
            if(range_changed){TESTPAR.analog4_gain=ANALOG_CH_RANGE_HL;} 
            #endif                 
        break;
        case ANALOGCH5:             
            GainGPIO_Expander=ANALOG_CH56_CONTROL;
            Channel_Gain=ANALOG_CH5_GAIN_CTRL; 
            Channel_Relay=ANALOG_CH5_RELAY_CTRL;         
            TESTPAR.analog5_gain=Range;
            #if (!USE_60V_ANALOG_CH_RANGE)
            if(range_changed){TESTPAR.analog5_gain=ANALOG_CH_RANGE_HL;} 
            #endif                         
        break;
        case ANALOGCH6:
            GainGPIO_Expander=ANALOG_CH56_CONTROL;
            Channel_Gain=ANALOG_CH6_GAIN_CTRL; 
            Channel_Relay=ANALOG_CH6_RELAY_CTRL;                       
            TESTPAR.analog6_gain=Range;
            #if (!USE_60V_ANALOG_CH_RANGE)
            if(range_changed){TESTPAR.analog6_gain=ANALOG_CH_RANGE_HL;} 
            #endif                 
        break;                                    
    } 
                 
    //First change gain:       
    switch(GainGPIO_Expander)
    {
        case ANALOG_CH12_CONTROL:  
            if((Range==ANALOG_CH_RANGE_LL)||(Range==ANALOG_CH_RANGE_HL))
            {                          
								PORT607&=~Channel_Gain;							
                //PORT607|=Channel_Gain; 
            }
            else
            {     
								PORT607|=Channel_Gain; 
                //PORT607&=~Channel_Gain;
            }                                                                                         
            HAL_I2C_Master_Transmit(&hi2c1, ANALOG_CH12_CONTROL, (uint8_t*)&PORT607,1, 1000);
        break;                                                                                    
        case ANALOG_CH34_CONTROL:
            if((Range==ANALOG_CH_RANGE_LL)||(Range==ANALOG_CH_RANGE_HL))
            {
								PORT608&=~Channel_Gain;  
								//PORT608|=Channel_Gain;
            }
            else
            {
								PORT608|=Channel_Gain;  
								//PORT608&=~Channel_Gain;
            }                                                                             
            HAL_I2C_Master_Transmit(&hi2c1, ANALOG_CH34_CONTROL, (uint8_t*)&PORT608,1, 1000);
        break;    
        case ANALOG_CH56_CONTROL:
            if((Range==ANALOG_CH_RANGE_LL)||(Range==ANALOG_CH_RANGE_HL))
            {
								PORT609&=~Channel_Gain;  
								//PORT609|=Channel_Gain;
            }
            else
            {
								PORT609|=Channel_Gain;  
								//PORT609&=~Channel_Gain;
            }                                                                             
            HAL_I2C_Master_Transmit(&hi2c1, ANALOG_CH56_CONTROL, (uint8_t*)&PORT609,1, 1000);
        break;        
    }     
    
//Set Gain Relay:
if((Range==ANALOG_CH_RANGE_LL)||(Range==ANALOG_CH_RANGE_LH))
{
       PORT609&=~Channel_Relay; 
}
else
{           
       PORT609|=Channel_Relay;
}   
HAL_I2C_Master_Transmit(&hi2c1, ANALOG_CH1TO6_REL_CONTROL,(uint8_t*)&PORT609,1, 1000);
}

void CC_RelayControl(unsigned char CC_RELAY, unsigned char On_Off)
{
    if(On_Off==ON)
    {
        PORT608|=CC_RELAY;
    }
    else
    {
        PORT608&=~CC_RELAY;
    } 
HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U608,(uint8_t*)&PORT608,1, 1000);
}

unsigned char enable_printer=OFF;

void AUX_dry_wet_control(void)
{
unsigned char i;
unsigned char control;
#if 0
unsigned char pom;
unsigned char *p;
p=&TESTPAR.aux1_dry_wet;
pom=0x01;
control=0x00;
for(i=0; i<NUMBER_OF_AUX_CHS; i++)
{
    if(*p==AUX_DRY)
    {
        control|=pom;    
    }               
    *p++;
    pom=pom<<1;
}
#endif
	
				#if 0
        control=0x00;         
                      
        //SD Card Power control (SD POWER ON= 1)!
        //Printer control (COMMUNICATE WITH CONTROL BOARD = 1)!
        if(enable_printer==OFF)
        {     
            control|=0xC0; //0b11000000;
        }
        else
        {
            control|=0x80; //0b10000000;
        }     
        #endif
    
        if(TESTPAR.aux1_dry_wet==AUX_WET)
        {
            PORT610|=0x01; //0b00000001;
        }       
				else
				{
					  PORT610&=0xFE ; //0b11111110;
				}
        if(TESTPAR.aux2_dry_wet==AUX_WET)
        {
            PORT610|=0x02; //0b00000010;
        }     
				else
				{
					  PORT610&=0xFD; //0b11111101;
				}
        if(TESTPAR.aux3_dry_wet==AUX_WET)
        {
            PORT610|=0x04; //0b00000100;
        }            
				else
				{
					  PORT610&=0xFB; //0b11111011;
				}
        if(TESTPAR.aux4_dry_wet==AUX_WET)
        {
            PORT610|=0x08; //0b00001000;
        }
				else
				{
					  PORT610&=0xF7; //0b11110111;
				}
        if(TESTPAR.aux5_dry_wet==AUX_WET)
        {
            PORT610|=0x10; //0b00010000;
        }
				else
				{
					  PORT610&=0xEF; //0b11101111;
				}
        if(TESTPAR.aux6_dry_wet==AUX_WET)
        {
            PORT610|=0x20; //0b00100000;
        }       
				else
				{
						PORT610&=0xDF; //0b11011111;
				}	
                                          
HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U610,(uint8_t*)&PORT610,1, 1000);
    
}

void I2C_CLOSE_LED_control(unsigned char LED_CONTROLER, unsigned char On_Off)
{
		if(On_Off==OFF) //ovdje je invertovana logika
    {   
        BSG_Relay_Control&=~(LED_CONTROLER);            
    }
    else
    {
        BSG_Relay_Control|=LED_CONTROLER;    
    }                 
HAL_I2C_Master_Transmit(&hi2c1, BSG_CONTROL_PINS, (uint8_t*)(&BSG_Relay_Control), 1, 1000);
}

void I2C_OPEN_LED_control(unsigned char LED_CONTROLER, unsigned char On_Off)
{
    if(On_Off==OFF)//ovdje je invertovana logika
    {   
        PORT610&=~(LED_CONTROLER);            
    }
    else
    {
        PORT610|=LED_CONTROLER;    
    }                 
HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U610, (uint8_t*)(&PORT610), 1, 1000);
}

void enable_printer_communication(void)
{
    //enable_printer=ON;
    //AUX_dry_wet_control();
	
     PORT610&=0xBF; //0b10111111; 

HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U610, (uint8_t*)(&PORT610), 1, 1000);
	
}

void disable_printer_communication(void)
{
    //enable_printer=OFF;
    //AUX_dry_wet_control();
	
	  PORT610|=0x40; //0b01000000;

HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U610, (uint8_t*)(&PORT610), 1, 1000);

}

#endif

void DRM_DAC_Write(unsigned int data, unsigned int channel)
{
	unsigned char i;
	unsigned long full_data = 0;
	unsigned long bit_operator = 0x800000;
	
	data |= 0x01;
	full_data = (unsigned long)0x180000;
	full_data |= ((unsigned long)data);	
	
	DRM_CLK_HIGH;
	DWT_Delay_ns(50);
	if(channel == CHANNEL1) DRM_SYNC1_LOW;
	if(channel == CHANNEL2) DRM_SYNC2_LOW;
	DWT_Delay_ns(50);
	
	for(i=0; i<24; i++)
	{
		if(full_data & bit_operator)
		{
			DRM_DIN_HIGH;
		}
		else
		{
			DRM_DIN_LOW;
		}
		DWT_Delay_ns(50);
    DRM_CLK_LOW;
    DWT_Delay_ns(50);
		full_data = (full_data << 1); 
    DRM_CLK_HIGH;		
    DWT_Delay_ns(50);
	}
	DWT_Delay_ns(50);
	if(channel == CHANNEL1) DRM_SYNC1_HIGH;
	if(channel == CHANNEL2) DRM_SYNC2_HIGH;
}

void DRM_Channel_Enable(unsigned int channel_number)
{
	//TODO: dodati case za ostale kanale
		switch(channel_number)
		{
			case 1: CURRENT_CH1_ENABLE; break;
			case 2: CURRENT_CH2_ENABLE; break;
			default: sprintf(OutputBuffer,"Error: operator not correct!");
		}
}

void DRM_Channel_Disable(unsigned int channel_number)
{
			//TODO: dodati case za ostale kanale
		switch(channel_number)
		{
			case 1: CURRENT_CH1_DISABLE; break;
			case 2: CURRENT_CH2_DISABLE; break;
			default: sprintf(OutputBuffer,"Error: operator not correct!");
		}
}

void Coil_Control(unsigned char coil_select, unsigned int on_off)
{
		if(coil_select == COIL_CLOSE)
		{		
			if(on_off == SET)
			{
				MCU_bat_chg_ctrl |= COIL_IGBT1_CTRL;
				HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U3, (uint8_t*)&MCU_bat_chg_ctrl, 1, 1000);
			}				
			else if(on_off == RESET)
			{
				MCU_bat_chg_ctrl &= ~(COIL_IGBT1_CTRL);
				HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U3, (uint8_t*)&MCU_bat_chg_ctrl, 1, 1000); 
			}				
		}
		else if(coil_select == COIL_OPEN)
		{
			if(on_off == SET)
			{
				MCU_bat_chg_ctrl |= COIL_IGBT2_CTRL;
				HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U3, (uint8_t*)&MCU_bat_chg_ctrl, 1, 1000);
			}				
			else if(on_off == RESET)
			{
				MCU_bat_chg_ctrl &= ~(COIL_IGBT2_CTRL);
				HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U3, (uint8_t*)&MCU_bat_chg_ctrl, 1, 1000); 
			}	
		}
		
}

//Ramp function
void Ramp(unsigned int channel, unsigned int dac_output, unsigned int up_down)
{
	unsigned int i = 0;
	unsigned int ramp = 0;
	unsigned int dac_limit = 30000;
	if(up_down == RAMP_UP)
	{
		ramp = 0;
		DRM_DAC_Write(ramp, channel);
		for(i = 0; i < 5; i++)
		{
			ramp += (dac_output / 5);
			if(ramp > dac_limit)
			{	
				DRM_DAC_Write(0, channel);
				DRM_Channel_Disable(channel);
				break;
			}
			DRM_DAC_Write(ramp, channel);
			HAL_Delay(1);
		}
	}
	else if(up_down == RAMP_DOWN)
	{
		ramp = dac_output;
		DRM_DAC_Write(ramp, channel);
		for(i = 0; i < 5; i++)
		{
			ramp -= (dac_output / 5);
			if(ramp > dac_limit || ramp < 0)
			{	
				DRM_DAC_Write(0, channel);
				DRM_Channel_Disable(channel);
				break;
			}
			DRM_DAC_Write(ramp, channel);
			HAL_Delay(1);
		}
	}
}

//Balancer control function
void Battery_Balancer_Control(unsigned int channel, unsigned int on_off)
{	
		if(channel == CHANNEL1)
		{
			if(on_off == ON) BAL1_ENABLE;
			else if(on_off == OFF) BAL1_DISABLE;
		}
		if(channel == CHANNEL2)
		{
			if(on_off == ON) BAL2_ENABLE;
			else if(on_off == OFF) BAL2_DISABLE;
		}	
}

//Pwr on/off control
void Pwr_Control(unsigned int channel, unsigned int on_off)
{
	if(channel == CHANNEL1)
	{
		if(on_off == ON) 
		{
			PWR1_ENABLE;
			//sprintf(OutputBuffer, "PWR1_ENABLE");
		}
		if(on_off == OFF)
		{
			PWR1_DISABLE;
			//sprintf(OutputBuffer, "PWR1_DISABLE");
		}			
	}
	if(channel == CHANNEL2)
	{
		if(on_off == ON) PWR2_ENABLE;
		if(on_off == OFF) PWR2_DISABLE;
	}
}

//Charger control function
void Battery_Battery_Charger_Control(unsigned int channel, unsigned int on_off)
{
	if(channel == CHANNEL1)
	{
		if(on_off == ON) CHG1_ENABLE;
		if(on_off == OFF) CHG1_DISABLE;
	}
	if(channel == CHANNEL2)
	{
		if(on_off == ON) CHG2_ENABLE;
		if(on_off == OFF) CHG2_DISABLE;
	}
}

unsigned int Battery_Charging_Status(unsigned int channel)
{
	unsigned int batChargeStatus;
	if(channel == CHANNEL1)
	{
		if(CHG1_INFO == GPIO_PIN_SET) batChargeStatus = BAT_NOT_CHARGING;
		else if(CHG1_INFO == GPIO_PIN_RESET) batChargeStatus = BAT_CHARGING;
	}
	if(channel == CHANNEL2)
	{
		if(CHG2_INFO == GPIO_PIN_SET) batChargeStatus = BAT_NOT_CHARGING;
		else if(CHG2_INFO == GPIO_PIN_RESET) batChargeStatus = BAT_CHARGING;
	}
	return batChargeStatus;
}




















