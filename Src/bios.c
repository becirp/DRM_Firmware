#include "main.h"
#include "bios.h"
#include "fatfs.h"
#include "stdio.h"
#include "string.h"

#include "bios.h"
#include "comm.h"
#include "init.h"
#include "dwt_delay.h"

//MCU Battery Charger Control
uint8_t MCU_bat_chg_ctrl = 0x02;

unsigned char language=0; //English is default language

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
	uint32_t spi_delay = 10;
	
	data |= 0x01;
	full_data = (unsigned long)0x180000;
	full_data |= ((unsigned long)data);	
	
	DRM1_CLK_HIGH;
	delay_us(spi_delay);
	if(channel == CHANNEL1 | channel == CHANNEL2) DRM1_SYNC1_LOW;
	//if(channel == CHANNEL2) DRM1_SYNC2_LOW; TODO: pin za SYNC2 visi. Zicom je kratko spojen na SYNC2. Istovremeno se zadaje DAC1 i DAC2 i na istu vrijednost.
	if(channel == CHANNEL3) DRM1_SYNC3_LOW;
	delay_us(spi_delay);
	
	for(i=0; i<24; i++)
	{
		if(full_data & bit_operator)
		{
			DRM1_DIN_HIGH;
		}
		else
		{
			DRM1_DIN_LOW;
		}
		delay_us(spi_delay);
    DRM1_CLK_LOW;
    delay_us(spi_delay);
		full_data = (full_data << 1); 
    DRM1_CLK_HIGH;		
    delay_us(spi_delay);
	}
	delay_us(spi_delay);
	if(channel == CHANNEL1 | channel == CHANNEL2) DRM1_SYNC1_HIGH;
	//if(channel == CHANNEL2) DRM1_SYNC2_HIGH; TODO: pin za SYNC2 visi
	if(channel == CHANNEL3) DRM1_SYNC3_HIGH;
}

void DRM_Channel_Enable(unsigned int channel_number)
{
	//TODO: dodati case za ostale kanale
		switch(channel_number)
		{
			case 1: CHANNEL1_ENABLE; break;
			case 2: CHANNEL2_ENABLE; break;
			case 3: CHANNEL3_ENABLE; break;
			default: sprintf(OutputBuffer,"Error: operator not correct!");
		}
}

void DRM_Channel_Disable(unsigned int channel_number)
{
			//TODO: dodati case za ostale kanale
		switch(channel_number)
		{
			case 1: CHANNEL1_DISABLE; break;
			case 2: CHANNEL2_DISABLE; break;
			case 3: CHANNEL3_DISABLE; break;
			default: sprintf(OutputBuffer,"Error: operator not correct!");
		}
}

void MCU_Battery_Charger_Control(unsigned char port_control, unsigned int on_off)
{
		if(on_off == 1)
		{
				MCU_bat_chg_ctrl |= port_control;
		}
		else
		{
				MCU_bat_chg_ctrl &= ~(port_control);
		}
		HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U3, (uint8_t*)&MCU_bat_chg_ctrl, 1, 1000);
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

void DRM_Battery_Charger_Control(unsigned char port_control, unsigned int on_off)
{
	//TODO: function body
}

void DRM_Pwr_Cfg_Ch1(unsigned int setOnOff)
{

}


