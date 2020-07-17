#include "main.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <stdbool.h>

#include "bios.h"
#include "comm.h"
#include "lang.h" 
#include "adc.h"
#include "calib.h"
#include "init.h"
#include "sd_card.h"
#include "dwt_delay.h"

volatile unsigned char timer1_interrupt;
volatile unsigned char timer2_interrupt;
volatile unsigned char control_after_first_contact_touched;

volatile unsigned char timer1_DRM_ON;
volatile unsigned char timer2_SRM_ON;

//DRM I2C control
uint8_t DRM1_Bat_Chg_Info1 = 0x10;
uint8_t DRM1_Bat_Chg_Info2 = 0x42;
	
unsigned int check_if_digit(unsigned char first, unsigned char last)
{
//This function checks if all the received characters in InputBuffer[first] to InputBuffer[last]
//string are digits (0-9)
unsigned char i, check;
unsigned int retVal;
check=0;
for (i=first; i<(last+1); i++){
	
        if(isdigit(InputBuffer[i])==0)
				{
					check=1;
        }
}
//OK: all received characters are digits:
if (check==0)
{ 
		retVal=MAIN_OK;
}     
else 
{
//Errorr: received string contains non digit characters:
		retVal=MAIN_NOK;
}  
return retVal;
}

unsigned int string_to_int(unsigned char first, unsigned char last)
{
//This function converts part of the InputBuffer string (InputBuffer[first] to InputBuffer[last])
//into the integer value
unsigned int i;
unsigned int retVal=0;
unsigned char newstring[10];
for(i=first;i<(last+1);i++)
{
	newstring[i-first]=InputBuffer[i];
}      
newstring[(last-first+1)]=0x00;
retVal=atoi(newstring);
return retVal;
}

unsigned long string_to_long(unsigned char first, unsigned char last)
{
//This function converts part of the InputBuffer string (InputBuffer[first] to InputBuffer[last])
//into the long value
unsigned int i;
unsigned long retVal=0;
unsigned char newstring[10];
for(i=first;i<(last+1);i++){newstring[i-first]=InputBuffer[i];}      
newstring[(last-first+1)]=0x00;
retVal=atol(newstring);
return retVal;
}

void OK_Response(void)
{
//Returns OK response to received command
    sprintf(OutputBuffer, "OK");
}

unsigned int SetCommPort_COMM(void)
{
#if 1
unsigned char pom; 
if(COMM.port!=InputBuffer[2])
{
    COMM.change_port=YES;
    if(COMM.port==COMM_PORT_A){pom=COMM_PORT_B;}
                          else{pom=COMM_PORT_A;}
}                       
else
{
    pom=COMM.port;                                        
    
}
sprintf(OutputBuffer, "Pt%c", pom);
#endif
return MAIN_OK;
}

unsigned int SetRemoteControl_COMM(unsigned char enable_disable)
{
    if(enable_disable==REMOTE_ENABLE)
    {
        COMM.remote_enable=REMOTE_ENABLE;    
    }                              
    else
    {
        putcharB(0x1B); 
        putcharB('O'); 
        putcharB('K');
        putcharB(0x0D);
        putcharB(0x0A);
        COMM.remote_enable=REMOTE_DISABLE;
        COMM.port=COMM_PORT_A;                            
    } 
    return MAIN_OK;
}

unsigned int DT_Reset_Counter_COMM(void)
{
unsigned int retVal=MAIN_OK;
DigTrans_Counter_Reset();
OK_Response();
return retVal;
}

unsigned int BSGTest_COMM(void)
{
unsigned int retVal=MAIN_NOK;
    if((InputBuffer[4]=='C')||(InputBuffer[4]=='R'))
    {
            if(InputBuffer[5]=='1')
            {                  
                if(InputBuffer[4]=='C')
                {
                    BSG_CHARGE_REL_ON;                         
                }
                if(InputBuffer[4]=='R')
                {
                    BSG_DISCHARGE_REL_ON;
                }                
                retVal=MAIN_OK;    
            } 
            else if(InputBuffer[5]=='0')
            {   
                if(InputBuffer[4]=='C')
                {
                    BSG_CHARGE_REL_OFF;
                }
                if(InputBuffer[4]=='R')
                {
                    BSG_DISCHARGE_REL_OFF;
                }                
                retVal=MAIN_OK;
            }                                            
    }                                 
OK_Response();
return retVal;
}

unsigned int Set_Test_Mode_COMM(unsigned char bsg_mode)
{
unsigned int retVal=MAIN_OK;
    if(bsg_mode==NO)
    //OSG:
    {   
        TURN_OFF_BSG;        
    }              
    //1 BPPF:
    else if(bsg_mode=='1')
    {   
        #if 1     
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
        HAL_Delay(100);
        BSG_CHARGE_REL_OFF;                                         
        HAL_Delay(50);
        BSG_BUCK_ON;
        HAL_Delay(1500);
        //sluzi za provjeru BSG konekcija: 
        BSG_DISCHARGE_REL_ON;       
        HAL_Delay(200);
        #endif                                            
         
        #if 0
        BSG_SEQUENCE_ONE_BPPF;
        
        //sluzi za provjeru BSG konekcija: 
    	BSG_DISCHARGE_REL_ON;       
        HAL_Delay(200);
        #endif                                                        
    }
    else if(bsg_mode=='2')
    //2 BPPF:
    {      
    
        #if 1
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
        HAL_Delay(100); 
        //release BSG capaciotrs. device prepared for BSG measurement:
        BSG_CHARGE_REL_OFF;                                         
        HAL_Delay(100);                                                          
        BSG_BUCK_ON;
        HAL_Delay(1500);  
        
        //sluzi za provjeru BSG konekcija:
        //iz nekoga razloga kod nekih aparata nece da ukljuci ovaj relej, pa je potrebno poslati vise puta komadnu:s
    	BSG_DISCHARGE_REL_ON;
        HAL_Delay(200);       
        #endif         
    
        #if 0
        BSG_SEQUENCE_TWO_BPPF;           
        
        //sluzi za provjeru BSG konekcija:
        //iz nekoga razloga kod nekih aparata nece da ukljuci ovaj relej, pa je potrebno poslati vise puta komadnu:s
    	BSG_DISCHARGE_REL_ON;
        HAL_Delay(200);
        #endif                           
    }                  
    
OK_Response();
return MAIN_OK;
}

unsigned int AUX_DryWetControl_Command(unsigned char dry_wet)
{
char i;
unsigned char *p;
p=&TESTPAR.aux1_dry_wet;
for(i=0; i<NUMBER_OF_AUX_CHS; i++)
{
    if(dry_wet=='D')
    {
        *p=AUX_DRY;
    }
    else
    {
        *p=AUX_WET;
    }   
    p++;    
} 

SET_AUX_CHANNELS;        
   
OK_Response();
return MAIN_OK;                                        
}

uint16_t abs_16_bit(int16_t x)
{
    int y = (x >> 15);
    return ((x ^ y) - y);
}


void read_all_adcs(void)
{
unsigned int i, j;
float average[NUMBER_OF_ANALOG_CHS];
float prescaler;
unsigned char selected_range;
#if FAKE_ADC_RESULTS
Fake_ADC_Results();   
#else
ADC_Read_MainContacts(0);

//Analog channels - averaging:
for(j=0; j<NUMBER_OF_ANALOG_CHS; j++)
{
        average[j]=0;
}

for(i=0;i<50;i++)
{
    ADC_Read_MainContacts(0); //Sada su to dvije iste funkcije!
    for(j=0; j<NUMBER_OF_ANALOG_CHS; j++)
    {
        average[j]+=ADC_Results.ANCH[j];
    }
}

for(j=0; j<NUMBER_OF_ANALOG_CHS; j++)
{
        ADC_Results.ANCH[j]=average[j]/50.0;
}

#if (!USE_60V_ANALOG_CH_RANGE)
{
    for(j=0; j<NUMBER_OF_ANALOG_CHS; j++)
    {       
      switch(j)
      {
        case 0: selected_range=TESTPAR.analog1_gain; break;
        case 1: selected_range=TESTPAR.analog2_gain; break;
        case 2: selected_range=TESTPAR.analog3_gain; break;
        case 3: selected_range=TESTPAR.analog4_gain; break;
        case 4: selected_range=TESTPAR.analog5_gain; break;
        case 5: selected_range=TESTPAR.analog6_gain; break;      
      }      
      if(selected_range==ANALOG_CH_RANGE_HL)
      {               
        prescaler=(float)ADC_Results.ANCH[j];        
        prescaler*=ACH_RANGE_HL_SCAL_FACTOR;
        prescaler/=ACH_RANGE_HH_SCAL_FACTOR;                          
        ADC_Results.ANCH[j]=prescaler;        
      } 
    }    
}
#endif

////////////////////////////////
//Current channels - averaging:
for(j=0; j<NUMBER_OF_CURRENT_CHS; j++)
{
        average[j]=0;
}
for(i=0;i<50;i++)
{
    //read_multiple_ch_adcs();
    for(j=0; j<NUMBER_OF_CURRENT_CHS; j++)
    {
        average[j]+=ADC_Results.CURRENT[j];
    }
}
for(j=0; j<NUMBER_OF_CURRENT_CHS; j++)
{
        ADC_Results.CURRENT[j]=average[j]/50.0;
}
////////////////////////////////

read_multiple_ch_adcs();
ReadAuxillaryChannels();
read_digital_trans_chs(); 

//apply_calibration_to_analog_chs();  

#if 0
sprintf(OutputBuffer, "VRIJEDNOST: %05u", ADC_Results.ANCH[0]);
SendOutputBuffer('B');
#endif

#endif    
}

unsigned int Read_ADC_Res_COMM(void)
{
unsigned int retVal=MAIN_OK;
unsigned int i;
int32_t current_aux;
	
read_all_adcs();
      
retVal=MAIN_OK;
sprintf(OutputBuffer, "ADC Results:");
SendOutputBuffer(COMM.port); 
                           
for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++){sprintf(OutputBuffer, "ADM%02u:%05u",   i, ADC_Results.MC[i]);                           SendOutputBuffer(COMM.port); HAL_Delay(1); OutputBuffer[0]=0;}
for(i=0; i<NUMBER_OF_CURRENT_CHS; i++)  {sprintf(OutputBuffer, "ADC%02u:%05u",   i, (unsigned int)abs_16_bit(ADC_Results.CURRENT[i]));   SendOutputBuffer(COMM.port); HAL_Delay(1); OutputBuffer[0]=0;}
for(i=0; i<NUMBER_OF_ANALOG_CHS; i++)   {sprintf(OutputBuffer, "ADA%01u%c:%05u", i, '1' , ADC_Results.ANCH[i]);                   SendOutputBuffer(COMM.port); HAL_Delay(1); OutputBuffer[0]=0;}
for(i=0; i<NUMBER_OF_LT_CHS; i++)       {sprintf(OutputBuffer, "ADL%02u:%05d",   i, ADC_Results.LT[i]);                           SendOutputBuffer(COMM.port); HAL_Delay(1); OutputBuffer[0]=0;}                                                
for(i=0; i<NUMBER_OF_DT_CHS; i++)       {sprintf(OutputBuffer, "ADD%02u:H%05uL%05u", i, ADC_Results.DTH[i],  ADC_Results.DTL[i]); SendOutputBuffer(COMM.port); HAL_Delay(1); OutputBuffer[0]=0;}
i=0;                                     sprintf(OutputBuffer, "ADX%02u:%05u",   i, ADC_Results.AUX);

OK_Response();

return retVal;
}

unsigned int Read_One_ADC_CH_COMM(void)
{
unsigned int ADC_Value1, ADC_Value2, ADC_channel;
int16_t ADC_ValInt;
unsigned int i;
unsigned char *ADC_ACH_Gain;
unsigned int retVal=MAIN_NOK;
unsigned int check;
int32_t current_aux;
uint32_t current_aux2;
#if 1
signed long DTres;
#endif
	
ADC_Value1=0;

check=check_if_digit(4,5);                
if(check==MAIN_OK)
{

ADC_channel=string_to_int(4,5);
read_all_adcs();

#if 0
//Provjera proracuna DT:
DTres=convert_to_slong_DT_res(ADC_channel);
sprintf(OutputBuffer, "RES=%f", (float)DTres);
SendOutputBuffer('A');
#endif

switch(InputBuffer[3])
{
case 'M': if((ADC_channel>=0)&&(ADC_channel<NUMBER_OF_MAIN_CONTACTS)){ADC_Value1=ADC_Results.MC[ADC_channel]; retVal=MAIN_OK;}  break; //Main Contacts [0]-[11]
case 'C': if((ADC_channel>=0)&&(ADC_channel<NUMBER_OF_CURRENT_CHS )) {ADC_ValInt=ADC_Results.CURRENT[ADC_channel]; retVal=MAIN_OK;} break; //Currents      [0]-[6]
case 'A': if((ADC_channel>=0)&&(ADC_channel<8)) {ADC_Value1=ADC_Results.ANCH[ADC_channel]; retVal=MAIN_OK;} break; //Analog ch.    [0]-[6] + 2 DRM CH
case 'L': if((ADC_channel>=0)&&(ADC_channel<NUMBER_OF_LT_CHS))       {ADC_ValInt=ADC_Results.LT[ADC_channel]; retVal=MAIN_OK;} break; //Linear trans  [0]-[3]
case 'D': if((ADC_channel>=0)&&(ADC_channel<NUMBER_OF_DT_CHS))       {ADC_Value1=ADC_Results.DTH[ADC_channel]; ADC_Value2=ADC_Results.DTL[ADC_channel]; retVal=MAIN_OK;} break;//Digital Transducer
case 'X': if (ADC_channel==0)                                        {ADC_Value1=ADC_Results.AUX; retVal=MAIN_OK;} break; //Aux Channels             
}         

    
if(retVal==MAIN_OK)
{         

if(InputBuffer[3]=='A')
{
            
    if((ADC_channel>=0)&&(ADC_channel<NUMBER_OF_ANALOG_CHS))
    {                     
        //SET gain for analog channels:
        ADC_ACH_Gain=&TESTPAR.analog1_gain;
        for(i=0; i<ADC_channel; i++)
        {
            ADC_ACH_Gain++;
        }
    }
    else
    {           
         //DRM channels:
         *ADC_ACH_Gain=ACH_RANGE_LH;        
    }
} 

                  
//if      (InputBuffer[3]=='D'){sprintf(OutputBuffer, "AD%cH%05uL%05u", InputBuffer[3], ADC_Value1, ADC_Value2);} 
if      (InputBuffer[3]=='D'){DTres=convert_to_slong_DT_res(ADC_channel);sprintf(OutputBuffer, "AD%+010.0f", (float)DTres);}
else if (InputBuffer[3]=='A'){sprintf(OutputBuffer, "AD%c%c%05u", InputBuffer[3], *ADC_ACH_Gain, ADC_Value1);}  
else if (InputBuffer[3]=='C'){sprintf(OutputBuffer, "AD%c%c%05u", InputBuffer[3], '0', (unsigned int)abs_16_bit(ADC_ValInt));}  //Ubacena 0 zbog calib menija, da bude isto kao i kod analognih kanala!
else if (InputBuffer[3]=='L'){sprintf(OutputBuffer, "AD%c%c%05d", InputBuffer[3], '0', ADC_ValInt);}  //Ubacena 0 zbog calib menija, da bude isto kao i kod analognih kanala!                      
else                         {sprintf(OutputBuffer, "AD%c%05u", InputBuffer[3], ADC_Value1);}
}
}
return retVal; 
}

unsigned int CheckBreakerStatus_COMM(unsigned char break_units_per_phase)
{ 
//Provjera stanja prekidaèa - Open, Close ili NotRegular
unsigned int retVal=MAIN_OK;
unsigned char Bstatus;

Bstatus='N'; 

read_all_adcs();

if(break_units_per_phase==ONE_BREAK_UNIT_PER_PHASE)
{
    if((ADC_Results.MC[0]<MAIN_THRESHOLD_LOW)&&(ADC_Results.MC[4]<MAIN_THRESHOLD_LOW)&&(ADC_Results.MC[8]<MAIN_THRESHOLD_LOW))
    {
        Bstatus='O';
    }   
    else if((ADC_Results.MC[0]>MAIN_THRESHOLD_HIGH)&&(ADC_Results.MC[4]>MAIN_THRESHOLD_HIGH)&&(ADC_Results.MC[8]>MAIN_THRESHOLD_HIGH))
    {
        Bstatus='C';                    
    }             
    else
    {
        Bstatus='N';
    }
}               
else if(break_units_per_phase==TWO_BREAK_UNITS_PER_PHASE)
{
     if((ADC_Results.MC[0]<MAIN_THRESHOLD_LOW)&&(ADC_Results.MC[1]<MAIN_THRESHOLD_LOW)&&
        (ADC_Results.MC[4]<MAIN_THRESHOLD_LOW)&&(ADC_Results.MC[5]<MAIN_THRESHOLD_LOW)&&
        (ADC_Results.MC[8]<MAIN_THRESHOLD_LOW)&&(ADC_Results.MC[9]<MAIN_THRESHOLD_LOW))
     {
        Bstatus='O';
     }   
     else if((ADC_Results.MC[0]>MAIN_THRESHOLD_HIGH)&&(ADC_Results.MC[1]>MAIN_THRESHOLD_HIGH)&&
             (ADC_Results.MC[4]>MAIN_THRESHOLD_HIGH)&&(ADC_Results.MC[5]>MAIN_THRESHOLD_HIGH)&&
             (ADC_Results.MC[8]>MAIN_THRESHOLD_HIGH)&&(ADC_Results.MC[9]>MAIN_THRESHOLD_HIGH))
     {
        Bstatus='C';                    
     }             
     else
     {
        Bstatus='N';
     }
}     
else if(break_units_per_phase==FOUR_BREAK_UNITS_PER_PHASE)
{
     if((ADC_Results.MC[0]<MAIN_THRESHOLD_LOW)&&(ADC_Results.MC[1]<MAIN_THRESHOLD_LOW)&
        (ADC_Results.MC[2]<MAIN_THRESHOLD_LOW)&&(ADC_Results.MC[3]<MAIN_THRESHOLD_LOW)&
        (ADC_Results.MC[4]<MAIN_THRESHOLD_LOW)&&(ADC_Results.MC[5]<MAIN_THRESHOLD_LOW)&
        (ADC_Results.MC[6]<MAIN_THRESHOLD_LOW)&&(ADC_Results.MC[7]<MAIN_THRESHOLD_LOW)&
        (ADC_Results.MC[8]<MAIN_THRESHOLD_LOW)&&(ADC_Results.MC[9]<MAIN_THRESHOLD_LOW)&
        (ADC_Results.MC[10]<MAIN_THRESHOLD_LOW)&&(ADC_Results.MC[11]<MAIN_THRESHOLD_LOW))
     {
        Bstatus='O';
     }   
     else if((ADC_Results.MC[0]>MAIN_THRESHOLD_HIGH)&&(ADC_Results.MC[1]>MAIN_THRESHOLD_HIGH)&
        (ADC_Results.MC[2]>MAIN_THRESHOLD_HIGH)&&(ADC_Results.MC[3]>MAIN_THRESHOLD_HIGH)&
        (ADC_Results.MC[4]>MAIN_THRESHOLD_HIGH)&&(ADC_Results.MC[5]>MAIN_THRESHOLD_HIGH)&
        (ADC_Results.MC[6]>MAIN_THRESHOLD_HIGH)&&(ADC_Results.MC[7]>MAIN_THRESHOLD_HIGH)&
        (ADC_Results.MC[8]>MAIN_THRESHOLD_HIGH)&&(ADC_Results.MC[9]>MAIN_THRESHOLD_HIGH)&
        (ADC_Results.MC[10]>MAIN_THRESHOLD_HIGH)&&(ADC_Results.MC[11]>MAIN_THRESHOLD_HIGH))
     {
        Bstatus='C';
     }            
     else
     {
        Bstatus='N';
     }
}  

switch (Bstatus)
{
    case 'C': SET_LED_CLOSE; CLR_LED_OPEN; break;
    case 'O': SET_LED_OPEN;  CLR_LED_CLOSE; break;
    default:  SET_LED_OPEN;  SET_LED_CLOSE; break; 
}
     
sprintf(OutputBuffer, "B%c", Bstatus);                     
return retVal;                      
}

unsigned int SetMeasParameters_COMM(void)                         
{
unsigned int retVal=MAIN_NOK;
unsigned int provjera;
unsigned char comm_number=InputBuffer[2];

    if(comm_number=='1'){               
        //if ((InputBuffer[3]>=CAT03)&(InputBuffer[3]<=CAT126))
        { //Device Code
        //if ((InputBuffer[4]>=MEASURING_RES_50us)&(InputBuffer[4]<=MEASURING_RES_10ms))
        {        //Sampling Resolution
        if ((InputBuffer[5]>=CLOSE_TEST)&&(InputBuffer[5]<=FIRST_TRIP_TEST)){                   //Operating Sequence  
        if ((InputBuffer[6]>=COIL_PULSE_ALL_PHASES)&&(InputBuffer[6]<=COIL_PULSE_PHASE3)){      //Open Coil Phase
        if ((InputBuffer[7]>=COIL_PULSE_ALL_PHASES)&&(InputBuffer[7]<=COIL_PULSE_PHASE3)){      //Close Coil Phase
        if ((InputBuffer[8]==NO)||(InputBuffer[8]==YES)){                                      //DRM Test!
        /**if ((InputBuffer[9]==NO)||(InputBuffer[9]==YES))**/ {                               //DRM Test CURRENT! Provjeri vrijednosti                
        if ((InputBuffer[10]==LANG_ENGLISH)||(InputBuffer[10]==LANG_OTHER)){    //LANGUAGE: 0-ENGLISH, 1-OTHER
        if ((InputBuffer[11]>=ACH_RANGE_LL)&&(InputBuffer[11]<=ACH_RANGE_HH)){   //TESTPAR.analog1_gain  
        if ((InputBuffer[12]>=ACH_RANGE_LL)&&(InputBuffer[12]<=ACH_RANGE_HH)){   //TESTPAR.analog2_gain
        if ((InputBuffer[13]>=ACH_RANGE_LL)&&(InputBuffer[13]<=ACH_RANGE_HH)){   //TESTPAR.analog3_gain             
        if ((InputBuffer[14]>=ACH_RANGE_LL)&&(InputBuffer[14]<=ACH_RANGE_HH)){   //TESTPAR.analog4_gain
        if ((InputBuffer[15]>=ACH_RANGE_LL)&&(InputBuffer[15]<=ACH_RANGE_HH)){   //TESTPAR.analog5_gain
        if ((InputBuffer[16]>=ACH_RANGE_LL)&&(InputBuffer[16]<=ACH_RANGE_HH)){   //TESTPAR.analog6_gain  
        if ((InputBuffer[17]==AUX_DRY)||(InputBuffer[17]==AUX_WET)){            //TESTPAR.aux1_dry_wet
        if ((InputBuffer[18]==AUX_DRY)||(InputBuffer[18]==AUX_WET)){            //TESTPAR.aux2_dry_wet
        if ((InputBuffer[19]==AUX_DRY)||(InputBuffer[19]==AUX_WET)){            //TESTPAR.aux3_dry_wet   
        if ((InputBuffer[20]==AUX_DRY)||(InputBuffer[20]==AUX_WET)){            //TESTPAR.aux4_dry_wet
        if ((InputBuffer[21]==AUX_DRY)||(InputBuffer[21]==AUX_WET)){            //TESTPAR.aux5_dry_wet        
        if ((InputBuffer[22]==AUX_DRY)||(InputBuffer[22]==AUX_WET)){            //ATESTPAR.aux6_dry_wet
            
        //All received parameters are in the predefined range. proceed...                    
        
            TESTPAR.device=InputBuffer[3];
            TESTPAR.sampl_resol=InputBuffer[4];                
            TESTPAR.op_sequence=InputBuffer[5];
            TESTPAR.o_coil_phase=InputBuffer[6];
            TESTPAR.c_coil_phase=InputBuffer[7];               
            TESTPAR.drm_test=InputBuffer[8];
            TESTPAR.drm_current=InputBuffer[9];  
            language=(InputBuffer[10]-48);              
            TESTPAR.analog1_gain=InputBuffer[11]; 
            TESTPAR.analog2_gain=InputBuffer[12]; 
            TESTPAR.analog3_gain=InputBuffer[13]; 
            TESTPAR.analog4_gain=InputBuffer[14];                
            TESTPAR.analog5_gain=InputBuffer[15];
            TESTPAR.analog6_gain=InputBuffer[16];  
            TESTPAR.aux1_dry_wet=InputBuffer[17];
            TESTPAR.aux2_dry_wet=InputBuffer[18];
            TESTPAR.aux3_dry_wet=InputBuffer[19];
            TESTPAR.aux4_dry_wet=InputBuffer[20];
            TESTPAR.aux5_dry_wet=InputBuffer[21];
            TESTPAR.aux6_dry_wet=InputBuffer[22];  
                       
            activetext = printer_texts[language]; 
            
            sprintf(OutputBuffer, "s1%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 
            TESTPAR.device, TESTPAR.sampl_resol, TESTPAR.op_sequence,
            TESTPAR.o_coil_phase, TESTPAR.c_coil_phase, 
            TESTPAR.drm_test, TESTPAR.drm_current, (language+48),
            TESTPAR.analog1_gain, TESTPAR.analog2_gain, TESTPAR.analog3_gain, TESTPAR.analog4_gain, TESTPAR.analog5_gain, TESTPAR.analog6_gain,
            TESTPAR.aux1_dry_wet, TESTPAR.aux2_dry_wet, TESTPAR.aux3_dry_wet, TESTPAR.aux4_dry_wet, TESTPAR.aux5_dry_wet, TESTPAR.aux6_dry_wet);
                       
            retVal=MAIN_OK; 
                        
        }}}}}}}}}}}}}}}}}}}}        

    }                    
    
    else if(comm_number=='2')
    //Not used anymore
    {
    
    }  
    
    else if(comm_number=='3'){
    
        if ((InputBuffer[3]>='1')&(InputBuffer[3]<='3')){ //Time_Base
        if ((InputBuffer[4]=='1')||(InputBuffer[4]=='2')){ //Frequency
        if ((InputBuffer[5]>='1')&(InputBuffer[5]<='3')){ //Travel_Unit
        if ((InputBuffer[6]=='A')||(InputBuffer[6]=='B')){ //CATX4Tr                      
        if ((InputBuffer[7]>='0')&(InputBuffer[7]<='2')){ //ext_trigg_wtm
        if ((InputBuffer[8]=='1')||(InputBuffer[8]=='2')||(InputBuffer[8]=='4')){ //BreakUnitsPerPhase Device //Timur: bilo  ||(InputBuffer[8]=='3')
        if ((InputBuffer[9]>='0')&(InputBuffer[9]<='J')){ //External Trigger Source           
        if ((InputBuffer[10]=='>')||(InputBuffer[10]=='<')){ //External Trigger HL  
        if ((InputBuffer[11]=='0')||(InputBuffer[11]=='1')){ //bs_grounded 1-YES, 0-NO
        if ((InputBuffer[12]=='0')||(InputBuffer[12]=='1')){ //oper_mech 0-TRANSDUCER_THREE_POLE, 1-TRANSDUCER_SIGLE_POLE
        if ((InputBuffer[13]>='0')&(InputBuffer[13]<='3')){ //TESTPAR.transd_ch
        provjera=check_if_digit(14,23);                     
        if(provjera==MAIN_OK){ 
        //Sav prijem OK:
        TESTPAR.time_base=InputBuffer[3];
        TESTPAR.mains_freq=InputBuffer[4];
        TESTPAR.travel_unit=InputBuffer[5];
        TESTPAR.transd_type=InputBuffer[6];
        TESTPAR.ext_trigg_wtm=InputBuffer[7];
        TESTPAR.break_units=InputBuffer[8];    
        TESTPAR.ext_trigg_src=InputBuffer[9]; 
        TESTPAR.ext_trigg_edg=InputBuffer[10]; 
        TESTPAR.bs_grounded=InputBuffer[11];
        TESTPAR.oper_mech=InputBuffer[12];
        TESTPAR.transd_ch=InputBuffer[13];
        TESTPAR.test_duration=string_to_int(14,18);
        TESTPAR.ext_trigg_lev=string_to_int(19,23);
        
        sprintf(OutputBuffer, "s3%c%c%c%c%c%c%c%c%c%c%c%05u%05u", TESTPAR.time_base, TESTPAR.mains_freq, TESTPAR.travel_unit, TESTPAR.transd_type, 
                TESTPAR.ext_trigg_wtm, TESTPAR.break_units, TESTPAR.ext_trigg_src, TESTPAR.ext_trigg_edg, TESTPAR.bs_grounded, 
                TESTPAR.oper_mech, TESTPAR.transd_ch, TESTPAR.test_duration, TESTPAR.ext_trigg_lev);     
                
        retVal=MAIN_OK;
               
        }}}}}}}}}}}}  
        
    }  
    else if (comm_number=='4'){   
       
        provjera=check_if_digit(3,32);
    
        if(provjera==MAIN_OK)
        {
                
        TESTPAR.analog1tf=string_to_int(3,7);
        TESTPAR.analog2tf=string_to_int(8,12);
        TESTPAR.analog3tf=string_to_int(13,17);
        TESTPAR.analog4tf=string_to_int(18,22);
        TESTPAR.analog5tf=string_to_int(23,27);    
        TESTPAR.analog6tf=string_to_int(28,32);       
         
        OK_Response();                
        retVal=MAIN_OK;                  
        
        }
    }
return retVal;
}

unsigned int EnterTFUnits_COMM(void)
{
unsigned char *pointer;
unsigned int i;

switch (InputBuffer[2]){
    case '1':
        pointer=&TESTPAR.analog1tf_unit[0];    
    break;
    case '2':
        pointer=&TESTPAR.analog2tf_unit[0];    
    break; 
    case '3':
        pointer=&TESTPAR.analog3tf_unit[0];    
    break;                                                    
    case '4':
        pointer=&TESTPAR.analog4tf_unit[0];    
    break;
    case '5':
        pointer=&TESTPAR.analog5tf_unit[0];    
    break;
    case '6':
        pointer=&TESTPAR.analog6tf_unit[0];    
    break;
}

for(i=0; i<3; i++)
{
    *pointer=InputBuffer[(i+3)];
    pointer++;
}
OK_Response();
return MAIN_OK;
}

unsigned int SetPulseLenghts_COMM(void)
{
unsigned int retVal=MAIN_NOK; 
unsigned int provjera; 
unsigned int pom;     
                provjera=check_if_digit(3,7);
                if(provjera==MAIN_OK){ //Sav prijem OK!  
                pom=string_to_int(3,7);   
                if((pom>0)&(pom<MAX_SAMPLES)){                
                        switch(InputBuffer[2]){
                        case '1':
                        TESTPAR.c_pulse_lenght=pom;                        
                        break;
                        case '2':
                        TESTPAR.o_pulse_lenght=pom;                        
                        break;
                        case '3':
                        TESTPAR.trip_free_delay=pom;                        
                        break;                         
                        case '4':
                        TESTPAR.reclosing_delay=pom;
                        break;
                        case '5':
                        TESTPAR.oco_delay1=pom;
                        break;
                        case '6':
                        TESTPAR.oco_delay2=pom;                        
                        break;                         
                        }
                retVal=MAIN_OK;
                }
                } 
                if (retVal==MAIN_OK){                                                          
                sprintf(OutputBuffer, "%c%05u", InputBuffer[2], pom);                           
                }                                                
return retVal;
}

unsigned int SetLTransdParameters_COMM(void)
{
unsigned int retVal=MAIN_NOK; 
unsigned int provjera; 
unsigned int pom;     
//NAPOMENA: skroz izaciti Ltransd. a ubaciti je u TESTPAR.lengh
        provjera=check_if_digit(3,22);  
        if(provjera==MAIN_OK)
        { 
        pom=InputBuffer[2]-48;
        #if 0
        if(InputBuffer[2]=='0')     {pom=0;}
        else if(InputBuffer[2]=='1'){pom=1;}
        else                        {pom=2;} 
        #endif        
        TESTPAR.tr_lenght_mm[pom]=string_to_int(3,7);
        TESTPAR.tr_lenght_inch[pom]=string_to_int(8,12);
        TESTPAR.tr_gain[pom]=string_to_int(13,17);
        TESTPAR.tr_offset[pom]=string_to_int(18,22);
        OK_Response();            
        retVal=MAIN_OK;       
        }                                 
return retVal;
}

unsigned int EnterTimeDate_COMM(void)
{
unsigned int retVal=MAIN_NOK;

unsigned int provjera;
unsigned int pom;
        provjera=check_if_digit(3,14);
        if(provjera==MAIN_OK){
                pom=string_to_int(3,4);                   
                TIMEDATE.yy=pom;
                pom=string_to_int(5,6);                   
                TIMEDATE.mm=pom;
                pom=string_to_int(7,8);                   
                TIMEDATE.dd=pom;
                pom=string_to_int(9,10);                   
                TIMEDATE.hh=pom;
                pom=string_to_int(11,12);                   
                TIMEDATE.min=pom;       
                pom=string_to_int(13,14);
                TIMEDATE.sec=pom;
                OK_Response();                                                                                
                retVal=MAIN_OK;
        }
return retVal;
}

unsigned int EnterBreakerTestDataFileName_COMM(void)
{
unsigned int retVal=MAIN_NOK;
unsigned char PomString[20];
unsigned char *pPomString; 
unsigned char *pBTDATA;  
//unsigned char StrLenght;
int i;
if(InputBuffer[3]=='D')
{
        if(((InputBuffer[4]>='0')&&(InputBuffer[4]<='9'))||(InputBuffer[4]=='A')||(InputBuffer[4]=='B'))
        {
        for(i=0; i<20; i++)
        {
            PomString[i]=InputBuffer[i+5];  
        }                             
        //StrLenght=strlen(PomString);
        pPomString=PomString;        
        switch (InputBuffer[4])
        {   
        case '0':pBTDATA=STICK.filen;         break;
        case '1':pBTDATA=BDATA.BreakerID;     break;     
        case '2':pBTDATA=BDATA.StationName;   break;
        case '3':pBTDATA=BDATA.Compartment;   break;                    
        case '4':pBTDATA=BDATA.Manufacturer;  break;
        case '5':pBTDATA=BDATA.SerialNumer;   break;
        case '6':pBTDATA=BDATA.BreakerType;   break;
        case '7':pBTDATA=TDATA.TypeOfTest;    break;        
        case '8':pBTDATA=TDATA.CompanyName;   break;
        case '9':pBTDATA=TDATA.Operater;      break; 
        case 'A':pBTDATA=STICK.ModelInfo;     break;  
        case 'B':pBTDATA=STICK.dirname;       break;              
        }                         
        strcpy(pBTDATA, pPomString);  
        //strncpy(pBTDATA, pPomString, StrLenght);
        OK_Response();
        retVal=MAIN_OK;
        }
}
return retVal;
}

unsigned int SendMeasurementParameters_Comm(void)
{
unsigned char test_duration;
	
    if(TESTPAR.test_duration!=20000)
    {
        test_duration='1';
    }   
    else
    {
        test_duration='2';
    }        
		
sprintf(OutputBuffer, "T:%c%c%c%c%c", TESTPAR.op_sequence, TESTPAR.break_units, test_duration, TESTPAR.transd_ch, TESTPAR.oper_mech);
return MAIN_OK;
}   

unsigned int Get_Test_Settings_Comm(void)
{  

send_test_data_to_pc();

return MAIN_OK;
} 


unsigned int Set_RMO_Results_Comm(void)
{
unsigned int i, k;
unsigned int retVal=MAIN_NOK;
if((InputBuffer[3]>='0')&&(InputBuffer[3]<='7'))
{
    if(InputBuffer[3]=='0')     {TESTPAR.op_sequence='R';}    
    else if(InputBuffer[3]=='1'){k=0;}
    else if(InputBuffer[3]=='2'){k=20;}
    else if(InputBuffer[3]=='3'){k=40;}
    else if(InputBuffer[3]=='4'){k=60;}     
    else if(InputBuffer[3]=='5'){k=80;}
    else if(InputBuffer[3]=='6'){k=100;}
    else if(InputBuffer[3]=='7'){k=120;}
    if(InputBuffer[3]>'0')
    {                                      
        for(i=0; i<20; i++)
        {             
            RMO_Results.static_res[(i+k)]=InputBuffer[(i+4)];
        }
    }                                                                                                                  
    OK_Response();
    retVal=MAIN_OK;
}
return retVal;
}                    

unsigned int Get_RMO_Results_Comm(void)
{
unsigned int i, k;
unsigned int retVal=MAIN_NOK;
if((InputBuffer[4]>='0')&&(InputBuffer[4]<='4'))
{
    if     (InputBuffer[4]=='0'){k=0;}
    else if(InputBuffer[4]=='1'){k=20;}
    else if(InputBuffer[4]=='2'){k=40;}
    else if(InputBuffer[4]=='3'){k=60;}  

    if(InputBuffer[4]=='4')
    {      
    //Send RMO results to DV Win:
    //Napomena: Prije je komanda glasila SMNxxxY, sada je samo SMNY... izmjeniti u DV Winu!
        for(i=0; i<48; i++){putcharB(RMO_Results.static_res[(i+80)]);}   
        putcharB(0);                         
        OutputBuffer[0]=0; 
        GetDataFromMemory=YES;
    }
    else
    {
        OutputBuffer[0]='D';                            
        for(i=0; i<20; i++){OutputBuffer[(i+1)]=RMO_Results.static_res[(k+i)];}
        OutputBuffer[21]=0;                        
    }                                   
retVal=MAIN_OK;
}                                                        
return retVal;
}

unsigned int Internal_SD_Mem_Comm(void)
{
unsigned int retVal=MAIN_NOK;
unsigned int check;
unsigned char channel;
unsigned int mem_loc;
unsigned int command;
//commands:
//'S' - store data to mem_loc
//'R' - read data from mem_loc (reads data from SD card and saves results to the internal SRAM)
//'P' - read data from mem_loc and send it to PC
//'E' - erase data on defined mem_loc
//'A' - erase all data in SD memory  
//'L' - send list of all avalable results

//commands:

//mSDCSXXX - store data to mem_loc XXX (000 to 999) - returns OK!

//Read numerical results from the device:
//mSDCRXXX - load numerical results from SD memory card to internal SRAM
//Response: OK
//After this command DV Win can use mVMD command to read test settings. 
//Important : TESTPAR.test_duration will define number of samples that DV Win receives with next command

//Read graphical results from the device:
//mSDCGXXX - read data from SD card and send it directly to PC
//Device returns "ready_waiting for 'A'" string
//Device waits character 'A' to be received on Port B
//after receiving char 'A' device starts sending data (amount of data will be defined by TESTPAR.test_duration parameter)

//Get info about result on one mem location
//mSDCR000
//Returns:
//XXX:Y\n (XXX address)

//List memory locations:
//mSDCL000 - lists all memory locations with recorded test
//Results are sent in form:
//XXX:Y\n (XXX address)
//sprintf(OutputBuffer, "%03d:%c\n", address, mem_loc_res[i]);
//Y (recorded test):
//#define CLOSE_TEST            (unsigned char) 'A'   
//#define OPEN_TEST             (unsigned char) 'B'   
//#define TRIPFREE_DELAY_TEST   (unsigned char) 'C'  
//#define TRIPFREE_FCT_TEST     (unsigned char) 'D'   //TRIP FREE, First Contact Touch
//#define O_03s_C_TEST          (unsigned char) 'E'   
//#define O_03s_CO_TEST         (unsigned char) 'F'    
//#define CdelayO_TEST          (unsigned char) 'G'    
//#define OdelayC_TEST          (unsigned char) 'H'    
//#define OdelayCO_DELAY_TEST   (unsigned char) 'I'    
//#define OdelayCO_FCT_TEST     (unsigned char) 'J'    //OdelayC, First Contact Touch
//#define O_03s_CO_FCT_TEST     (unsigned char) 'K'    //O03sCO, First Contact Touch
//#define FIRST_TRIP_TEST		(unsigned char) 'L'	
//#define DRM_TEST              (unsigned char) 'M'
//#define NO_TEST               (unsigned char) 'N'
//RMO TEST; STATIC RESISTANCE RESULT:
//#define STATIC_RESISTANCE     (unsigned char) 'R'

//Erase result from mem location:
//mSDCEXXX - deletes result from memory location XXX
//Response: OK

//Erase all memory: 
//mSDCA000 - deletes result from memory location XXX
//Response: OK                
   
command=InputBuffer[4];

check=check_if_digit(5,7);
if(check==MAIN_OK)
{
    mem_loc=string_to_int(5,7);  
    
    if((mem_loc>=0)&&(mem_loc<=MAX_NUMBER_MEM_LOC))
    {                                   
        switch(command)
        {
            case 'S':
            #if DEBUG   
            TESTPAR.test_duration=10;   
            TESTPAR.op_sequence=CdelayO_TEST;
            TESTPAR.sampl_resol=MEASURING_RES_01ms; 
            TESTPAR.break_units=ONE_BREAK_UNIT_PER_PHASE;
            COMM.remote_enable=REMOTE_ENABLE;            
                         
            sprintf(OutputBuffer, "Snima podatke na mem loc: %05u\n", mem_loc);
            SendOutputBuffer('B');
            sprintf(OutputBuffer, "Tip testa: %c\n", TESTPAR.op_sequence);
            SendOutputBuffer('B');            
            sprintf(OutputBuffer, "Duzina testa: %05u\n", TESTPAR.test_duration);
            SendOutputBuffer('B');
            sprintf(OutputBuffer, "Prekidnih mjesta: %c\n", TESTPAR.break_units);
            SendOutputBuffer('B');           
            #endif
                
						#if 0
						SET_LED_OPEN;
						SET_LED_CLOSE;
						#endif                                                         
						//todo: poslati gresku ukoliko nije uspio da snimi na SD karticu!
						if((retVal=store_data_to_sd_card(mem_loc))!=MAIN_OK)           {}
						#if 0
						CLR_LED_OPEN;
						CLR_LED_CLOSE;
						#endif 
						OK_Response();    
							
            break;						
					
            case 'G':                           
                if((retVal=read_data_from_sd_card(mem_loc, GRAPHICS))!=MAIN_OK){return MAIN_OK;}
                //if((retVal=read_data_from_sd_card(mem_loc, NUMERICS))!=MAIN_OK){return MAIN_OK;}  
                #if DEBUG
                    sprintf(OutputBuffer, "AUX1=%05u",AUX_Results.AUX1Time);
                    SendOutputBuffer(COMM.port);
                    sprintf(OutputBuffer, "AUX2=%05u",AUX_Results.AUX2Time);
                    SendOutputBuffer(COMM.port);
                    sprintf(OutputBuffer, "AUX3=%05u",AUX_Results.AUX3Time);
                    SendOutputBuffer(COMM.port);
                    sprintf(OutputBuffer, "AUX4=%05u",AUX_Results.AUX4Time);
                    SendOutputBuffer(COMM.port);
                    sprintf(OutputBuffer, "AUX5=%05u",AUX_Results.AUX5Time);
                    SendOutputBuffer(COMM.port);
                    sprintf(OutputBuffer, "AUX6=%05u",AUX_Results.AUX6Time);
                    SendOutputBuffer(COMM.port);                 
                #endif     
            OK_Response();                
            break;      
								
            case 'N':                             
                  
            #if 0
            TESTPAR.test_duration=20;   
            TESTPAR.op_sequence=CLOSE_TEST;
            TESTPAR.sampl_resol=MEASURING_RES_1ms;
            #endif
                          
                if((retVal=read_data_from_sd_card(mem_loc, NUMERICS))!=MAIN_OK){return MAIN_OK;}  
                OK_Response();                
            break;									
								
            case 'E':                        
                if((retVal=delete_result_from_the_list(mem_loc))!=MAIN_OK)     {return MAIN_OK;}
                OK_Response();                                        
            break;
								
            case 'A':                                                       
                if((retVal=erase_all_mem_locations())!=MAIN_OK)                {return MAIN_OK;}
                OK_Response();            
            break;    
								
            case 'L':                                                       
                if((retVal=list_all_mem_locations())!=MAIN_OK)                 {return MAIN_OK;}
                OK_Response();
            break;    
								
            case 'R':
                if((retVal=mem_location_info(mem_loc))!=MAIN_OK)               {return MAIN_OK;}     
            break;

        }
                     
    }    
}        
}


unsigned int Set_Active_Language_Comm(void)
{
unsigned int retVal=MAIN_NOK;
unsigned char active_lang;
active_lang=InputBuffer[4]-48;
if((active_lang==0)||(active_lang==1))
{
    language=active_lang;
    activetext = printer_texts[language];      
    OK_Response();
    retVal=MAIN_OK;
}
return retVal;
}


////////////////// DISPLAY RESULTS //////////////////////

unsigned int  MaxValuePom=0;
unsigned int  MinValuePom=30000;

unsigned char PrintBuffer[52];

void ClearPrintBuffer(void){
int i;
for(i=0; i<51; i++){PrintBuffer[i]=' ';}
PrintBuffer[0]=0;
}

#if (REZULTATI_KERIM)

void RezZaKerima(void){
unsigned int i;

MainC_Results.PreinsertionResistance[0][0]=20000;
MainC_Results.PreinsertionResistance[0][1]=20100;
MainC_Results.PreinsertionResistance[1][0]=20200;
MainC_Results.PreinsertionResistance[1][1]=20300;
MainC_Results.PreinsertionResistance[2][0]=20400;
MainC_Results.PreinsertionResistance[2][1]=20500;
MainC_Results.PreinsertionResistance[3][0]=20600;
MainC_Results.PreinsertionResistance[3][1]=20700;
MainC_Results.PreinsertionResistance[4][0]=20800;
MainC_Results.PreinsertionResistance[4][1]=20900;
MainC_Results.PreinsertionResistance[5][0]=21000;
MainC_Results.PreinsertionResistance[5][1]=21100;

AUX_Results.AUX1Time=704;
AUX_Results.AUX2Time=699;
AUX_Results.AUX3Time=710;
AUX_Results.AUX4Time=712;
AUX_Results.AUX5Time=698;
AUX_Results.AUX6Time=695;

MainC_Results.ClosingTime[0][0]=605;
MainC_Results.ClosingTime[0][1]=597;
MainC_Results.ClosingTime[1][0]=622;
MainC_Results.ClosingTime[1][1]=611;
MainC_Results.ClosingTime[2][0]=615;
MainC_Results.ClosingTime[2][1]=614;
MainC_Results.ClosingTime[3][0]=615;
MainC_Results.ClosingTime[3][1]=620;
MainC_Results.ClosingTime[4][0]=601;
MainC_Results.ClosingTime[4][1]=603;
MainC_Results.ClosingTime[5][0]=608;
MainC_Results.ClosingTime[5][1]=604;

MainC_Results.PreinsertionTime[0][0]=505;
MainC_Results.PreinsertionTime[0][1]=500;
MainC_Results.PreinsertionTime[1][0]=490;
MainC_Results.PreinsertionTime[1][1]=456;
MainC_Results.PreinsertionTime[2][0]=411;
MainC_Results.PreinsertionTime[2][1]=560;
MainC_Results.PreinsertionTime[3][0]=530;
MainC_Results.PreinsertionTime[3][1]=380;
MainC_Results.PreinsertionTime[4][0]=482;
MainC_Results.PreinsertionTime[4][1]=563;
MainC_Results.PreinsertionTime[5][0]=556;
MainC_Results.PreinsertionTime[5][1]=300;

MainC_Results.FirstOpeningTime[0][0]=484;
MainC_Results.FirstOpeningTime[0][1]=498;
MainC_Results.FirstOpeningTime[1][0]=481;
MainC_Results.FirstOpeningTime[1][1]=491;
MainC_Results.FirstOpeningTime[2][0]=471;
MainC_Results.FirstOpeningTime[2][1]=469;
MainC_Results.FirstOpeningTime[3][0]=476;
MainC_Results.FirstOpeningTime[3][1]=480;
MainC_Results.FirstOpeningTime[4][0]=499;
MainC_Results.FirstOpeningTime[4][1]=498;
MainC_Results.FirstOpeningTime[5][0]=495;
MainC_Results.FirstOpeningTime[5][1]=499;


MainC_Results.SecondOpeningTime[0][0]=931;
MainC_Results.SecondOpeningTime[0][1]=929;
MainC_Results.SecondOpeningTime[1][0]=930;
MainC_Results.SecondOpeningTime[1][1]=928;
MainC_Results.SecondOpeningTime[2][0]=915;
MainC_Results.SecondOpeningTime[2][1]=913;
MainC_Results.SecondOpeningTime[3][0]=916;
MainC_Results.SecondOpeningTime[3][1]=911;
MainC_Results.SecondOpeningTime[4][0]=948;
MainC_Results.SecondOpeningTime[4][1]=946;
MainC_Results.SecondOpeningTime[5][0]=948;
MainC_Results.SecondOpeningTime[5][1]=944;

if(TESTPAR.op_sequence==OPEN_TEST){
AUX_Results.AUX1Time=504;
AUX_Results.AUX2Time=599;
AUX_Results.AUX3Time=510;
AUX_Results.AUX4Time=512;
AUX_Results.AUX5Time=598;
AUX_Results.AUX6Time=595;
}

switch(TESTPAR.op_sequence){
            case TRIPFREE_DELAY_TEST:
            case TRIPFREE_FCT_TEST:
            case CdelayO_TEST: 
                MainC_Results.FirstOpeningTime[0][0]+=1000;
                MainC_Results.FirstOpeningTime[0][1]+=1000;
                MainC_Results.FirstOpeningTime[1][0]+=1000;
                MainC_Results.FirstOpeningTime[1][1]+=1000;
                MainC_Results.FirstOpeningTime[2][0]+=1000;
                MainC_Results.FirstOpeningTime[2][1]+=1000;
                MainC_Results.FirstOpeningTime[3][0]+=1000;
                MainC_Results.FirstOpeningTime[3][1]+=1000;
                MainC_Results.FirstOpeningTime[4][0]+=1000;
                MainC_Results.FirstOpeningTime[4][1]+=1000;
                MainC_Results.FirstOpeningTime[5][0]+=1000;
                MainC_Results.FirstOpeningTime[5][1]+=1000;
            break; 
}
Current_Results.MaxCurrentClosing[0]=54321;
Current_Results.MaxCurrentClosingTime[0]=12345;

Current_Results.MaxCurrentClosing[1]=54322;
Current_Results.MaxCurrentClosingTime[1]=12344;

Current_Results.MaxCurrentClosing[2]=54333;
Current_Results.MaxCurrentClosingTime[2]=12355;

Current_Results.MaxCurrentBreaking[0]=12222;
Current_Results.MaxCurrentBreakingTime[0]=1222;

Current_Results.MaxCurrentBreaking[1]=13333;
Current_Results.MaxCurrentBreakingTime[1]=1333;

Current_Results.MaxCurrentBreaking[2]=14444;
Current_Results.MaxCurrentBreakingTime[2]=1444;

AnalogCH_Results.MaxVoltageCH[0]=66666;
AnalogCH_Results.MaxVoltageCHTime[0]=666;
AnalogCH_Results.MinVoltageCH[0]=55555;
AnalogCH_Results.MinVoltageCHTime[0]=555;

AnalogCH_Results.MaxVoltageCH[1]=77777;
AnalogCH_Results.MaxVoltageCHTime[1]=777;
AnalogCH_Results.MinVoltageCH[1]=33333;
AnalogCH_Results.MinVoltageCHTime[1]=333;

AnalogCH_Results.MaxVoltageCH[2]=12345;
AnalogCH_Results.MaxVoltageCHTime[2]=12346;
AnalogCH_Results.MinVoltageCH[2]=12347;
AnalogCH_Results.MinVoltageCHTime[2]=12348;

AnalogCH_Results.MaxVoltageCH[3]=3211;
AnalogCH_Results.MaxVoltageCHTime[3]=3212;
AnalogCH_Results.MinVoltageCH[3]=3213;
AnalogCH_Results.MinVoltageCHTime[3]=3214;

AnalogCH_Results.MaxVoltageCH[4]=3215;
AnalogCH_Results.MaxVoltageCHTime[4]=3216;
AnalogCH_Results.MinVoltageCH[4]=3217;
AnalogCH_Results.MinVoltageCHTime[4]=3218;

AnalogCH_Results.MaxVoltageCH[5]=3219;
AnalogCH_Results.MaxVoltageCHTime[5]=3220;
AnalogCH_Results.MinVoltageCH[5]=3221;
AnalogCH_Results.MinVoltageCHTime[5]=3222;

for(i=0; i<3 ;i++){
	DTransd_Results.TotalTravel[i]=(signed long)(1000+(signed long)i);
	LTransd_Results.TotalTravel[i]=(1005+i);
	DTransd_Results.OverTravel[i]=(signed long)(1010+(signed long)i);
	LTransd_Results.OverTravel[i]=(1020+i);
	DTransd_Results.OverTravelTime[i]=(1025+i);
	LTransd_Results.OverTravelTime[i]=(1030+i);
	DTransd_Results.Rebound[i]=(signed long)(1035+(signed long)i);
	LTransd_Results.Rebound[i]=(1040+i);
	DTransd_Results.ReboundTime[i]=(1045+i);
	LTransd_Results.ReboundTime[i]=(1050+i); 
}

DTransd_Results.ContactWipe[0][0]=1000;
DTransd_Results.ContactWipe[0][1]=2000;
DTransd_Results.ContactWipe[1][0]=3000;
DTransd_Results.ContactWipe[1][1]=4000;
DTransd_Results.ContactWipe[2][0]=5000;
DTransd_Results.ContactWipe[2][1]=6000;
DTransd_Results.ContactWipe[3][0]=7000;
DTransd_Results.ContactWipe[3][1]=8000;
DTransd_Results.ContactWipe[4][0]=9000;
DTransd_Results.ContactWipe[4][1]=10000;
DTransd_Results.ContactWipe[5][0]=11000;
DTransd_Results.ContactWipe[5][1]=12000;

LTransd_Results.ContactWipe[0][0]=15000;
LTransd_Results.ContactWipe[0][1]=16000;
LTransd_Results.ContactWipe[1][0]=17000;
LTransd_Results.ContactWipe[1][1]=18000;
LTransd_Results.ContactWipe[2][0]=19000;
LTransd_Results.ContactWipe[2][1]=20000;
LTransd_Results.ContactWipe[3][0]=21000;
LTransd_Results.ContactWipe[3][1]=22000;
LTransd_Results.ContactWipe[4][0]=23000;
LTransd_Results.ContactWipe[4][1]=24000;
LTransd_Results.ContactWipe[5][0]=25000;
LTransd_Results.ContactWipe[5][1]=26000;

}
#endif

unsigned int calc_max_time(int num,...)
{
   va_list valist;
   unsigned int max;
   unsigned int pom;
   int i;

   /* initialize valist for num number of arguments */
   va_start(valist, num);

   max=va_arg(valist, unsigned int);

   /* access all the arguments assigned to valist */
   for (i = 1; i < num; i++) {
        pom=va_arg(valist, unsigned int);
        if(pom>max)
        {
            max=pom;
        }
   }
   
   /* clean memory reserved for valist */
   va_end(valist);

   return max;
}

unsigned int calc_min_time(int num,...)
{
   va_list valist;
   unsigned int min;
   unsigned int pom;
   int i;

   /* initialize valist for num number of arguments */
   va_start(valist, num);

   min=va_arg(valist, unsigned int);

   /* access all the arguments assigned to valist */
   for (i = 1; i < num; i++) {
        pom=va_arg(valist, unsigned int);
        if(pom<min)
        {
            min=pom;
        }
   }

   /* clean memory reserved for valist */
   va_end(valist);

   return min;
}


void TimePrikaz(unsigned char Index, unsigned int Result){
float ResultPrikaz;
if(Result>20000){
Result=0;
}         
    switch (TESTPAR.time_base){
    case TIME_BASE_ms:    
        ResultPrikaz=(Result/10.0);
        if(TESTPAR.sampl_resol==MEASURING_RES_1ms){ResultPrikaz*=10.0;}
        else if(TESTPAR.sampl_resol==MEASURING_RES_10ms){ResultPrikaz*=100.0;}
        sprintf(&PrintBuffer[Index],"%-6.1f ms", ResultPrikaz);
    break;    
    case TIME_BASE_s:
        ResultPrikaz=(Result/10000.0);
        if(TESTPAR.sampl_resol==MEASURING_RES_1ms){ResultPrikaz*=10.0;}
        else if(TESTPAR.sampl_resol==MEASURING_RES_10ms){ResultPrikaz*=100.0;}
        sprintf(&PrintBuffer[Index],"%-6.4f s", ResultPrikaz);
    break;
    case TIME_BASE_Cy:
        switch(TESTPAR.mains_freq){
        case FREQ_50Hz:
            ResultPrikaz=(Result/200.0);
            if(TESTPAR.sampl_resol==MEASURING_RES_1ms){ResultPrikaz*=10.0;}
            else if(TESTPAR.sampl_resol==MEASURING_RES_10ms){ResultPrikaz*=100.0;}
            sprintf(&PrintBuffer[Index],"%-6.2f Cy50", ResultPrikaz);
        break;
        case FREQ_60Hz:
            ResultPrikaz=(Result/166.6666);
            if(TESTPAR.sampl_resol==MEASURING_RES_1ms){ResultPrikaz*=10.0;}
            else if(TESTPAR.sampl_resol==MEASURING_RES_10ms){ResultPrikaz*=100.0;}
            sprintf(&PrintBuffer[Index],"%-6.2f Cy60", ResultPrikaz);        
        break;
        }
    break;
    }  
//if(Printer==1){PrintNewLine(0);}    
}

void DisplayText(unsigned char parameter1, unsigned char parameter2, unsigned char* dodajrez, unsigned char* karakter1, unsigned char* karakter2){

unsigned int parameter;

//struct PrinterTexts activetext;
//activetext=printer_texts[language];

parameter1-=48;
parameter2-=48;
parameter=(unsigned int)parameter1*10+parameter2;

switch(parameter)
{
    case 0:  sprintf(&PrintBuffer[0], "A1 = ");    *dodajrez=1; *karakter1=0; *karakter2=0; break;
    case 1:  sprintf(&PrintBuffer[0], "A2 = ");    *dodajrez=1; *karakter1=0; *karakter2=1; break;
    case 2:  sprintf(&PrintBuffer[0], "A3 = ");    *dodajrez=1; *karakter1=1; *karakter2=0; break;
    case 3:  sprintf(&PrintBuffer[0], "A4 = ");    *dodajrez=1; *karakter1=1; *karakter2=1; break;
    case 4:  sprintf(&PrintBuffer[0], "B1 = ");    *dodajrez=1; *karakter1=2; *karakter2=0; break;
    case 5:  sprintf(&PrintBuffer[0], "B2 = ");    *dodajrez=1; *karakter1=2; *karakter2=1; break;
    case 6:  sprintf(&PrintBuffer[0], "B3 = ");    *dodajrez=1; *karakter1=3; *karakter2=0; break;
    case 7:  sprintf(&PrintBuffer[0], "B4 = ");    *dodajrez=1; *karakter1=3; *karakter2=1; break;
    case 8:  sprintf(&PrintBuffer[0], "C1 = ");    *dodajrez=1; *karakter1=4; *karakter2=0; break;
    case 9:  sprintf(&PrintBuffer[0], "C2 = ");    *dodajrez=1; *karakter1=4; *karakter2=1; break;
    case 10: sprintf(&PrintBuffer[0], "C3 = ");    *dodajrez=1; *karakter1=5; *karakter2=0; break;
    case 11: sprintf(&PrintBuffer[0], "C4 = ");    *dodajrez=1; *karakter1=5; *karakter2=1; break;
    case 12: sprintf(&PrintBuffer[0], "A = ");     *dodajrez=2; break; 
    case 13: sprintf(&PrintBuffer[0], "B = ");     *dodajrez=3; break; 
    case 14: sprintf(&PrintBuffer[0], "C = ");     *dodajrez=4; break;              
    case 15: sprintf(&PrintBuffer[0], "A Syn = "); *dodajrez=5; break; 
    case 16: sprintf(&PrintBuffer[0], "B Syn = "); *dodajrez=6; break; 
    case 17: sprintf(&PrintBuffer[0], "C Syn = "); *dodajrez=7; break; 
    case 18: sprintf(&PrintBuffer[0], "    ");     *dodajrez=0; break; //Empty!   
    case 19: sprintf(&PrintBuffer[0], "%s",         activetext.OpeningTime);        break;
    case 20: sprintf(&PrintBuffer[0], "%s",         activetext.ClosingTime);        break;
    case 21: sprintf(&PrintBuffer[0], "%s",         activetext.PhaseOpeningTime);   break;
    case 22: sprintf(&PrintBuffer[0], "%s",         activetext.PhaseClosingTime);   break;
    case 23: sprintf(&PrintBuffer[0], "%s",         activetext.PhaseSync);          break;
    case 24: sprintf(&PrintBuffer[0], "%s",         activetext.BreakerOpeningTime); break;
    case 25: sprintf(&PrintBuffer[0], "%s",         activetext.BreakerClosingTime); break;
    case 26: sprintf(&PrintBuffer[0], "%s",         activetext.AUX_OpeningTime);    break;
    case 27: sprintf(&PrintBuffer[0], "%s",         activetext.AUX_ClosingTime);    break;
    case 28: sprintf(&PrintBuffer[0], "%s",         activetext.O_Sync);             break;
    case 29: sprintf(&PrintBuffer[0], "%s",         activetext.C_Sync);             break; 
    case 30: sprintf(&PrintBuffer[0], "      ");    *dodajrez=8;                    break;
    case 31: sprintf(&PrintBuffer[0], "      ");    *dodajrez=9;                    break;
    case 32: sprintf(&PrintBuffer[0], "1a = ");     *dodajrez=10;                   break;
    case 33: sprintf(&PrintBuffer[0], "1b = ");     *dodajrez=11;                   break;
    case 34: sprintf(&PrintBuffer[0], "2a = ");     *dodajrez=12;                   break;
    case 35: sprintf(&PrintBuffer[0], "2b = ");     *dodajrez=13;                   break;
    case 36: sprintf(&PrintBuffer[0], "3a = ");     *dodajrez=14;                   break;
    case 37: sprintf(&PrintBuffer[0], "3b = ");     *dodajrez=15;                   break;
    case 38: sprintf(&PrintBuffer[0], "%s",         activetext.CloseOpenTime);      break;
    case 39: sprintf(&PrintBuffer[0], "%s",         activetext.Phase_CO_Time);      break;
    case 40: sprintf(&PrintBuffer[0], "%s",         activetext.Breaker_CO_Time);    break;
    case 41: sprintf(&PrintBuffer[0], "%s",         activetext.ReclosingTime);      break;
    case 42: sprintf(&PrintBuffer[0], "%s",         activetext.PhaseReclosingTime); break;
    case 43: sprintf(&PrintBuffer[0], "%s",         activetext.BreakerReclosingTime);   break;
    case 44: sprintf(&PrintBuffer[0], "%s",         activetext.OpenCloseTime);      break;
    case 45: sprintf(&PrintBuffer[0], "%s",         activetext.Phase_OC_Time);      break;
    case 46: sprintf(&PrintBuffer[0], "%s",         activetext.Breaker_OC_Time);    break;
    case 47: sprintf(&PrintBuffer[0], "%s",         activetext.O1_Sync);            break;
    case 48: sprintf(&PrintBuffer[0], "%s",         activetext.O2_Sync);            break; 
    case 50: sprintf(&PrintBuffer[0], "%s",         activetext.PreinsertionRTime);  break;
    case 51: sprintf(&PrintBuffer[0], "%s",         activetext.ResistorOnTime);     break;
    case 52: sprintf(&PrintBuffer[0], "%s",         activetext.ResistanceValue);    break;
    case 53: sprintf(&PrintBuffer[0], "%s",         activetext.BreakerRSync);       break;
    case 54: sprintf(&PrintBuffer[0], "%s",         activetext.Phase_R_ON_Time);    break;                
}

}

void CalculateMaxMinTimePerPhase(unsigned char CalculateForPreinsertion, unsigned char MaxMin, unsigned char CalculateParameter, unsigned int* Time1, unsigned int* Time2, unsigned int* Time3)
{
//MaxMin: '1'=Max, '0'=Min
//CalculateParameter: '1': ClosingTime, '2': FirstOpeningTime, '3': SecondOpeningTime
unsigned int ContactTime[3][4]= {
{0, 0, 0, 0} ,   /*  initializers for row indexed by 0 */
{0, 0, 0, 0} ,   /*  initializers for row indexed by 1 */
{0, 0, 0, 0}   /*  initializers for row indexed by 2 */
};
int i=0;
int j=0;
int m=0;
int n=0;

    //****************************************************************************
    //CALCULATING MAXIMUM OR MINIMUM CLOSING/OPEN1&2/PREINSERTION TIME PER PHASE:   
    //****************************************************************************

    for(i=0; i<3; i++)
    {
        for(j=0; j<4; j++)
        {                                                                              
            if(CalculateForPreinsertion)
            {                           
            //Contact Time=PreinsertionTime
                ContactTime[i][j]=MainC_Results.PreinsertionTime[m][n];
            }             
            else
            {
            //Contact Time=ClosingTime                
                switch(CalculateParameter)
                { 
                    case '1': ContactTime[i][j]=MainC_Results.ClosingTime[m][n]; break;
                    case '2': ContactTime[i][j]=MainC_Results.FirstOpeningTime[m][n]; break;
                    case '3': ContactTime[i][j]=MainC_Results.SecondOpeningTime[m][n]; break;
                }   
                         
            }  
            //Take maximum values in case of '1' and '2' Break Points per Phase!          
            switch(TESTPAR.break_units){
                case '1':      
                    if(MaxMin=='1')
                    {
                        if(j>0){ContactTime[i][j]=MaxValuePom;}
                    }                                          
                    else
                    {
                        if(j>0){ContactTime[i][j]=MinValuePom;}
                    }
                break;                                     
                case '2':             
                    if(MaxMin=='1')
                    {
                        if(j>1){ContactTime[i][j]=MaxValuePom;}
                    }
                    else
                    {
                        if(j>1){ContactTime[i][j]=MinValuePom;}
                    }                
                break;
                default:
                    //Do nothing (4 points per phase!)
                break;   
            }          
            //Indexing:
            n++; if(n==2){n=0;} if((j==1)||(j==3)){m++;}
        }
    }   
    
    if(MaxMin=='1')
    {
    *Time1=calc_max_time(4, ContactTime[0][0], ContactTime[0][1],
                            ContactTime[0][2], ContactTime[0][3]);                                                                   
                                
    *Time2=calc_max_time(4, ContactTime[1][0], ContactTime[1][1],
                            ContactTime[1][2], ContactTime[1][3]);                            
    
    *Time3=calc_max_time(4, ContactTime[2][0], ContactTime[2][1],
                            ContactTime[2][2], ContactTime[2][3]);
    }   
    else
    {
    *Time1=calc_min_time(4, ContactTime[0][0], ContactTime[0][1],
                            ContactTime[0][2], ContactTime[0][3]);
                                                                   
                                
    *Time2=calc_min_time(4, ContactTime[1][0], ContactTime[1][1],
                            ContactTime[1][2], ContactTime[1][3]);
                            
    
    *Time3=calc_min_time(4, ContactTime[2][0], ContactTime[2][1],
                            ContactTime[2][2], ContactTime[2][3]);    
    }      

}      

#define CO_TEST_CLOSING_TIME    (unsigned char) '0'
#define CO_TEST_CO_TIME         (unsigned char) '1'
#define OC_TEST_OPENING_TIME    (unsigned char) '0'
#define OC_TEST_RECLOSING_TIME  (unsigned char) '1'
#define OC_TEST_OC_TIME         (unsigned char) '2'
#define OCO_TEST_OPENING_TIME   (unsigned char) '0'
#define OCO_TEST_RECLOSING_TIME (unsigned char) '1'
#define OCO_TEST_OC_TIME        (unsigned char) '2'
#define OCO_TEST_CO_TIME        (unsigned char) '3'


unsigned int AddSinglePhaseResult(unsigned char PreinsertionR, unsigned char parameter3, unsigned char karakter1, unsigned char karakter2,  float *Rx)
{
unsigned int vrijednost=0;
float vrijednost_otpornika=0;
*Rx=0;

        switch(TESTPAR.op_sequence){
        case CLOSE_TEST:
         
        //RETURN CLOSING TIME:
        vrijednost=MainC_Results.ClosingTime[karakter1][karakter2];       
        
        if(PreinsertionR=='1')
        {                   
            //RETURN PREINSERTION TIME:
            vrijednost=MainC_Results.PreinsertionTime[karakter1][karakter2]; 
        }
                                 
        if(PreinsertionR=='2')
        {
        //RETURN CLOSING TIME - PREINSERTION R TIME:                                         
                if(MainC_Results.PreinsertionTime[karakter1][karakter2]!=0){
                if(MainC_Results.ClosingTime[karakter1][karakter2]>MainC_Results.PreinsertionTime[karakter1][karakter2])
                    {
                        vrijednost=MainC_Results.ClosingTime[karakter1][karakter2]-MainC_Results.PreinsertionTime[karakter1][karakter2];                    
                    }                                                                                                                   
                    else{
                        vrijednost=0;
                    }                                                       
                }   
                else
                {
                    vrijednost=0;
                }
        }                                                                                   
        else if(PreinsertionR=='3')
        {
        //RETURN PREINSERTION R VALUE:
                vrijednost=MainC_Results.PreinsertionResistance[karakter1][karakter2]; 
                if(vrijednost!=0){
                        vrijednost_otpornika=(float)vrijednost*1.0;
                        *Rx=(65535.0/vrijednost_otpornika);
                        *Rx*=360.0;
                        if(*Rx>380){
                            *Rx-=380.0; 
                        }   
                        else{
                            *Rx=0;
                        }
                }                            
        }                                 
        break;   
        case OPEN_TEST:
            vrijednost=MainC_Results.FirstOpeningTime[karakter1][karakter2];
        break;    
        case TRIPFREE_DELAY_TEST:
        case TRIPFREE_FCT_TEST:
        case CdelayO_TEST:                                               
                             if(parameter3=='0'){               
                                //Closing Time:
                                vrijednost=MainC_Results.ClosingTime[karakter1][karakter2];
                             }
                             else{  
                                //Close-Open Time:        
                                if(MainC_Results.FirstOpeningTime[karakter1][karakter2]>MainC_Results.ClosingTime[karakter1][karakter2])
                                {
                                    vrijednost=(MainC_Results.FirstOpeningTime[karakter1][karakter2]-MainC_Results.ClosingTime[karakter1][karakter2]);
                                }   
                                else
                                {
                                    vrijednost=0;
                                }
                             }
        break; 
        case O_03s_C_TEST:
        case OdelayC_TEST:
                            if  (parameter3=='0'){
                                //Opening Time:               
                                vrijednost=MainC_Results.FirstOpeningTime[karakter1][karakter2];
                            }    
                            else if(parameter3=='1'){
                                //Reclosing Time: 
                                vrijednost=MainC_Results.ClosingTime[karakter1][karakter2];                             
                            }        
                            else{             
                                //Open-Close Time:
                                if(MainC_Results.ClosingTime[karakter1][karakter2]>MainC_Results.FirstOpeningTime[karakter1][karakter2])
                                {
                                    vrijednost=(MainC_Results.ClosingTime[karakter1][karakter2]-MainC_Results.FirstOpeningTime[karakter1][karakter2]);
                                }
                                else
                                {
                                    vrijednost=0;
                                }
                            }
        break; 
        case O_03s_CO_TEST:
        case OdelayCO_DELAY_TEST:
        case OdelayCO_FCT_TEST:
        case O_03s_CO_FCT_TEST:
                            if(parameter3=='0')
                            {   
                                //OPENING TIME:            
                                vrijednost=MainC_Results.FirstOpeningTime[karakter1][karakter2];
                            }       
                            else if(parameter3=='1')                            
                            {                       
                                //RECLOSING TIME:
                                vrijednost=MainC_Results.ClosingTime[karakter1][karakter2];                            
                            }   
                            else if(parameter3=='2')
                            {
                                //OPEN-CLOSE TIME                        
                                vrijednost=(MainC_Results.ClosingTime[karakter1][karakter2]-MainC_Results.FirstOpeningTime[karakter1][karakter2]);                    
                            }
                            else if(parameter3=='3')
                            {  
                                //CLOSE-OPEN TIME:
                                vrijednost=(MainC_Results.SecondOpeningTime[karakter1][karakter2]-MainC_Results.ClosingTime[karakter1][karakter2]);                    
                            }                            
        break;                    
        }                 
return vrijednost;
}

unsigned char PuniPrintBuffer(unsigned char parameter1, unsigned char parameter2, unsigned char parameter3, unsigned char MenOrPrn, unsigned char PreinsertionR){
unsigned int duzina;
unsigned int vrijednost;
unsigned int vrijednost2;
unsigned char dodajrez=0;
unsigned char karakter1, karakter2;

unsigned int PhaseACloseMaxTime;
unsigned int PhaseBCloseMaxTime;
unsigned int PhaseCCloseMaxTime;
unsigned int PhaseACloseMinTime;
unsigned int PhaseBCloseMinTime;
unsigned int PhaseCCloseMinTime;
unsigned int PhaseAOpenMinTime;
unsigned int PhaseBOpenMinTime;
unsigned int PhaseCOpenMinTime;
unsigned int PhaseAOpenMaxTime;
unsigned int PhaseBOpenMaxTime;
unsigned int PhaseCOpenMaxTime;
unsigned int PhaseAOpen2MinTime;
unsigned int PhaseBOpen2MinTime;
unsigned int PhaseCOpen2MinTime;
unsigned int PhaseAOpen2MaxTime;
unsigned int PhaseBOpen2MaxTime;
unsigned int PhaseCOpen2MaxTime;

unsigned int PhaseOpenTime=0;
unsigned int PhaseOpen2Time=0;
unsigned int PhaseCloseTime=0;

unsigned int PhaseOpenMaxTime;
unsigned int PhaseOpenMinTime;
unsigned int PhaseOpen2MaxTime;
unsigned int PhaseOpen2MinTime;
unsigned int PhaseCloseMaxTime;
unsigned int PhaseCloseMinTime;

unsigned int PhaseMaxResTime;
unsigned int PhaseMinResTime;

unsigned int PhaseAResistorMaxTime;
unsigned int PhaseBResistorMaxTime;
unsigned int PhaseCResistorMaxTime;

unsigned int PhaseAResistorMinTime;
unsigned int PhaseBResistorMinTime;
unsigned int PhaseCResistorMinTime;

unsigned int PhaseCloseResTime;

#define CALCULATE_MAX (unsigned char) '1'
#define CALCULATE_MIN (unsigned char) '0'
#define CALCULATE_PREINSERTION (unsigned char) 1
#define DONT_CALCULATE_PREINS  (unsigned char) 0

#define CALCULATE_CLOSING       (unsigned char)  '1'
#define CALCULATE_FIRST_OPENING (unsigned char)  '2'
#define CALCULATE_SECOND_OPENING (unsigned char) '3'

#define TEXT_WITHOUT_RESULT (unsigned char) 0
#define SINGLE_PHASE_RESULT (unsigned char) 1
#define PHASE_A_RESULT      (unsigned char) 2
#define PHASE_B_RESULT      (unsigned char) 3
#define PHASE_C_RESULT      (unsigned char) 4
#define PHASE_A_SYNC        (unsigned char) 5
#define PHASE_B_SYNC        (unsigned char) 6
#define PHASE_C_SYNC        (unsigned char) 7
#define BREAKER_RESULT      (unsigned char) 8
#define BREAKER_SYNC        (unsigned char) 9

float Rx;

unsigned char retValPom=0;

//Samo za testiranje:
//TESTPAR.op_sequence=OdelayCO_DELAY_TEST;
//TESTPAR.time_base=TIME_BASE_ms;
//TESTPAR.sampl_resol=MEASURING_RES_01ms;   
//TESTPAR.break_units='4';

#if (REZULTATI_KERIM)
activetext = printer_texts[0];
RezZaKerima();
#endif

DisplayText(parameter1, parameter2, &dodajrez, &karakter1, &karakter2);

if(dodajrez!=TEXT_WITHOUT_RESULT){

if(dodajrez==SINGLE_PHASE_RESULT)
{
    //ODNOSI SE NA REZULTATE KOJI SE ISPISUJU SAMO ZA POJEDINAÈNU FAZU!
    vrijednost=AddSinglePhaseResult(PreinsertionR, parameter3, karakter1, karakter2, &Rx);
    // *********************************************    
    // ********* parameter3 definitions: ***********
    // ********************************************* 
    //
    //CLOSE TEST:
    //OPEN TEST:
    //No influence
    //
    //CO TESTS:
    //parameter3='0': Closing Time
    //parameter3='1': Close-Open Time
    //
    //OC TEST:
    //parameter3='0': Opening Time
    //parameter3='1': Reclosing Time
    //parameter3='2': Open-Close Time
    //
    //OC TESTO:
    //parameter3='0': Opening Time
    //parameter3='1': Reclosing Time
    //parameter3='2': Open-Close Time
    //parameter3='3': Close-Open Time
}
    
////////////////////////////////////////////////////////////////////////////
/////////////////// CALCULATE MIN&MAX TIMES ////////////////////////////////        
////////////////////////////////////////////////////////////////////////////         
//********************************************************
//CALCULATING MAXIMUM CLOSING/PREINSERTION TIME PER PHASE:   
//********************************************************        
CalculateMaxMinTimePerPhase(DONT_CALCULATE_PREINS, CALCULATE_MAX, CALCULATE_CLOSING, &PhaseACloseMaxTime, &PhaseBCloseMaxTime, &PhaseCCloseMaxTime);

//********************************************************
//CALCULATING MINIMUM CLOSING/PREINSERTION TIME PER PHASE:   
//********************************************************      
    
CalculateMaxMinTimePerPhase(DONT_CALCULATE_PREINS, CALCULATE_MIN, CALCULATE_CLOSING, &PhaseACloseMinTime, &PhaseBCloseMinTime, &PhaseCCloseMinTime);    
  
//********************************************************
//CALCULATING MAXIMUM OPEN1 TIME PER PHASE:   
//********************************************************                             
CalculateMaxMinTimePerPhase(DONT_CALCULATE_PREINS, CALCULATE_MAX, CALCULATE_FIRST_OPENING, &PhaseAOpenMaxTime, &PhaseBOpenMaxTime, &PhaseCOpenMaxTime);         

//********************************************************
//CALCULATING MINIMUM OPEN1 TIME PER PHASE:   
//********************************************************                             
CalculateMaxMinTimePerPhase(DONT_CALCULATE_PREINS, CALCULATE_MIN, CALCULATE_FIRST_OPENING, &PhaseAOpenMinTime, &PhaseBOpenMinTime, &PhaseCOpenMinTime);                                                    
    
//********************************************************
//CALCULATING MAXIMUM OPEN2 TIME PER PHASE:   
//********************************************************                             
CalculateMaxMinTimePerPhase(DONT_CALCULATE_PREINS, CALCULATE_MAX, CALCULATE_SECOND_OPENING, &PhaseAOpen2MaxTime, &PhaseBOpen2MaxTime, &PhaseCOpen2MaxTime);            

//********************************************************
//CALCULATING MINIMUM OPEN2 TIME PER PHASE:   
//********************************************************                             
CalculateMaxMinTimePerPhase(DONT_CALCULATE_PREINS, CALCULATE_MIN, CALCULATE_SECOND_OPENING, &PhaseAOpen2MinTime, &PhaseBOpen2MinTime, &PhaseCOpen2MinTime);

//********************************************************
//CALCULATING MAXIMUM RESISTOR TIME PER PHASE:   
//********************************************************                             
CalculateMaxMinTimePerPhase(CALCULATE_PREINSERTION, CALCULATE_MAX, CALCULATE_CLOSING, &PhaseAResistorMaxTime, &PhaseBResistorMaxTime, &PhaseCResistorMaxTime);      

//********************************************************
//CALCULATING MINIMUM RESISTOR TIME PER PHASE:   
//********************************************************                             
CalculateMaxMinTimePerPhase(CALCULATE_PREINSERTION, CALCULATE_MIN, CALCULATE_CLOSING, &PhaseAResistorMinTime, &PhaseBResistorMinTime, &PhaseCResistorMinTime);
                                    
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////        
//////////////////////////////////////////////////////////////////////////// 

//////////////////////////////////////////////////////////////////////////// 
//PHASE TIME FOR A, B & C PHASE:           
//////////////////////////////////////////////////////////////////////////// 
if((dodajrez==PHASE_A_RESULT)||(dodajrez==PHASE_B_RESULT)||(dodajrez==PHASE_C_RESULT))
{
    switch(dodajrez)
    {
        case PHASE_A_RESULT: 
            PhaseCloseTime=PhaseACloseMaxTime; PhaseOpenTime=PhaseAOpenMinTime; PhaseOpen2Time=PhaseAOpen2MinTime;
            if(PreinsertionR=='1'){
                PhaseCloseResTime=MainC_Results.PreinsertionTime[0][0];
            }
            if(PreinsertionR=='2'){                                 
                if(PhaseACloseMaxTime>PhaseAResistorMaxTime)
                {                                 
                    PhaseCloseResTime=PhaseACloseMaxTime-PhaseAResistorMaxTime;
                    if(PhaseAResistorMaxTime==0)
                    {
                        PhaseCloseResTime=0;
                    }
                }           
            } 
        break; 
        case PHASE_B_RESULT: 
            PhaseCloseTime=PhaseBCloseMaxTime; PhaseOpenTime=PhaseBOpenMinTime; PhaseOpen2Time=PhaseBOpen2MinTime;
            if(PreinsertionR=='1'){
                PhaseCloseResTime=MainC_Results.PreinsertionTime[2][0];
            }
            if(PreinsertionR=='2'){                                 
                if(PhaseBCloseMaxTime>PhaseBResistorMaxTime)
                {
                    PhaseCloseResTime=PhaseBCloseMaxTime-PhaseBResistorMaxTime;   
                    if(PhaseBResistorMaxTime==0)
                    {
                        PhaseCloseResTime=0;
                    }
                }           
            }  
        break;
        case PHASE_C_RESULT: 
            PhaseCloseTime=PhaseCCloseMaxTime; PhaseOpenTime=PhaseCOpenMinTime; PhaseOpen2Time=PhaseCOpen2MinTime;
            if(PreinsertionR=='1'){
                PhaseCloseResTime=MainC_Results.PreinsertionTime[4][0];
            }
            if(PreinsertionR=='2'){                                 
                if(PhaseCCloseMaxTime>PhaseCResistorMaxTime)
                {
                    PhaseCloseResTime=PhaseCCloseMaxTime-PhaseCResistorMaxTime; 
                    if(PhaseCResistorMaxTime==0)
                    {
                        PhaseCloseResTime=0;
                    }
                }           
            }   
        break;             
    }
        
     switch(TESTPAR.op_sequence){
        case CLOSE_TEST: 
                vrijednost=PhaseCloseTime;                                
                if((PreinsertionR=='1')||(PreinsertionR=='2'))
                {
                    vrijednost=PhaseCloseResTime;
                }
        break;
        case OPEN_TEST:  
                vrijednost=PhaseOpenTime;   
        break; 
        case TRIPFREE_DELAY_TEST:
        case TRIPFREE_FCT_TEST:
        case CdelayO_TEST:
            switch(parameter3)
            {
                case CO_TEST_CLOSING_TIME:   vrijednost=PhaseCloseTime;                 break; 
                case CO_TEST_CO_TIME:        vrijednost=(PhaseOpenTime-PhaseCloseTime); break; 
            }                  
        break; 
        case O_03s_C_TEST:
        case OdelayC_TEST:    
        case O_03s_CO_TEST:
        case OdelayCO_DELAY_TEST:
        case OdelayCO_FCT_TEST:
        case O_03s_CO_FCT_TEST:    
            switch(parameter3)
            {
                case OCO_TEST_OPENING_TIME:   vrijednost=PhaseOpenTime; break; 
                case OCO_TEST_RECLOSING_TIME: vrijednost=PhaseCloseTime; break; 
                case OCO_TEST_OC_TIME:        vrijednost=(PhaseCloseTime-PhaseOpenTime); break;
                case OCO_TEST_CO_TIME:        vrijednost=(PhaseOpen2Time-PhaseCloseTime); break; 
            }                                            
        break;                                            
    }                                                                                                                              
}     
//////////////////////////////////////////////////////////////////////////// 

////////////////////////////////////////////////////////////////////////////                                                                                
//SYNC TIME FOR A, B & C PHASE:           
//////////////////////////////////////////////////////////////////////////// 
if((dodajrez==PHASE_A_SYNC)||(dodajrez==PHASE_B_SYNC)||(dodajrez==PHASE_C_SYNC))                      
{
    switch(dodajrez)
    {
        case PHASE_A_SYNC:
        {
           PhaseOpenMaxTime=PhaseAOpenMaxTime;
           PhaseOpenMinTime=PhaseAOpenMinTime;
           PhaseOpen2MaxTime=PhaseAOpen2MaxTime;
           PhaseOpen2MinTime=PhaseAOpen2MinTime;
           PhaseCloseMaxTime=PhaseACloseMaxTime;
           PhaseCloseMinTime=PhaseACloseMinTime;
           PhaseMaxResTime=PhaseAResistorMaxTime;
           PhaseMinResTime=PhaseAResistorMinTime;    
        } 
        break;
        case PHASE_B_SYNC:
        {           
           PhaseOpenMaxTime=PhaseBOpenMaxTime;
           PhaseOpenMinTime=PhaseBOpenMinTime;
           PhaseOpen2MaxTime=PhaseBOpen2MaxTime;
           PhaseOpen2MinTime=PhaseBOpen2MinTime;
           PhaseCloseMaxTime=PhaseBCloseMaxTime;
           PhaseCloseMinTime=PhaseBCloseMinTime;  
           PhaseMaxResTime=PhaseBResistorMaxTime;
           PhaseMinResTime=PhaseBResistorMinTime;  
        }
        break;
        case PHASE_C_SYNC:
        {           
           PhaseOpenMaxTime=PhaseCOpenMaxTime;
           PhaseOpenMinTime=PhaseCOpenMinTime;
           PhaseOpen2MaxTime=PhaseCOpen2MaxTime;
           PhaseOpen2MinTime=PhaseCOpen2MinTime;           
           PhaseCloseMaxTime=PhaseCCloseMaxTime;
           PhaseCloseMinTime=PhaseCCloseMinTime;
           PhaseMaxResTime=PhaseCResistorMaxTime;
           PhaseMinResTime=PhaseCResistorMinTime;                
        }
        break;
    } 
    switch(TESTPAR.op_sequence){
            case CLOSE_TEST:     
                vrijednost=(PhaseCloseMaxTime-PhaseCloseMinTime); 
                if(PreinsertionR=='2')
                {   
                    if(PhaseMaxResTime>PhaseMinResTime)
                    {
                        vrijednost=(PhaseMaxResTime-PhaseMinResTime);
                    }
                    else
                    {
                        vrijednost=0;
                    }
                }                                                                                                                    
            break;
            case OPEN_TEST: 
                            vrijednost=(PhaseOpenMaxTime-PhaseOpenMinTime);    
            break;            
            case TRIPFREE_DELAY_TEST:
            case TRIPFREE_FCT_TEST:
            case CdelayO_TEST:                     
                            vrijednost=(PhaseCloseMaxTime-PhaseCloseMinTime); //samo se gleda za C
            break;       
            case O_03s_C_TEST:
            case OdelayC_TEST:    
                        if(parameter3=='0')
                        { //Phase Open sinh.
                            vrijednost=(PhaseOpenMaxTime-PhaseOpenMinTime);
                        }
                        else
                        { //Phase Close sinh.
                            vrijednost=(PhaseCloseMaxTime-PhaseCloseMinTime); 
                        }                       
            break;            
    }         
}
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////                                                                                
//TIMINGS FOR CIRCUIT BREAKER:           
////////////////////////////////////////////////////////////////////////////
if(dodajrez==BREAKER_RESULT)
{
           switch(TESTPAR.op_sequence){
            case CLOSE_TEST:   
            if(PreinsertionR=='2')
            {
                vrijednost2=calc_min_time(3, PhaseAResistorMaxTime, PhaseBResistorMaxTime, PhaseCResistorMaxTime);                                
                                                                                                      
                vrijednost=calc_max_time(3, PhaseACloseMaxTime, PhaseBCloseMaxTime, PhaseCCloseMaxTime);
                                
                 if(vrijednost>vrijednost2)
                {              
                    if(vrijednost2==0)
                    {
                        vrijednost=0;
                    }                
                    else
                    {
                    vrijednost-=vrijednost2;
                    }
                }                           
                else
                {
                    vrijednost=0;
                }           
            }
            else
            {
            vrijednost=calc_max_time(3, PhaseACloseMaxTime, PhaseBCloseMaxTime, PhaseCCloseMaxTime);
            }            
            break;
            case OPEN_TEST: 
             vrijednost=calc_max_time(3, PhaseAOpenMinTime, PhaseBOpenMinTime, PhaseCOpenMinTime);       
            break;            
            case TRIPFREE_DELAY_TEST:
            case TRIPFREE_FCT_TEST:
            case CdelayO_TEST:                                                                                  
                 if(parameter3==CO_TEST_CLOSING_TIME)
                 {   
                 //Breaker C Time:               
                 vrijednost=calc_max_time(3, PhaseACloseMaxTime, PhaseBCloseMaxTime, PhaseCCloseMaxTime);  
                 }     
                 else
                 {
                 //Breaker CO Time:
                 vrijednost=calc_max_time(3, PhaseAOpenMinTime, PhaseBOpenMinTime, PhaseCOpenMinTime);
                 
                 vrijednost-=calc_min_time(3, PhaseACloseMaxTime, PhaseBCloseMaxTime, PhaseCCloseMaxTime); 
                           
                 }
            break;   
            case O_03s_C_TEST:
            case OdelayC_TEST:
                 if(parameter3==OC_TEST_OPENING_TIME)
                 {   
                 //Breaker Opening time 
                 vrijednost=calc_max_time(3, PhaseAOpenMinTime, PhaseBOpenMinTime, PhaseCOpenMinTime);                 
                 }
                 else if(parameter3==OC_TEST_RECLOSING_TIME)
                 {  
                 //Breaker Reclosing time
                 vrijednost=calc_max_time(3, PhaseACloseMaxTime, PhaseBCloseMaxTime, PhaseCCloseMaxTime); 
                 }                       
                 else
                 { 
                 //Breaker O-C time
                 vrijednost=calc_min_time(3, PhaseACloseMaxTime, PhaseBCloseMaxTime, PhaseCCloseMaxTime);

                 vrijednost-=calc_max_time(3, PhaseAOpenMinTime, PhaseBOpenMinTime, PhaseCOpenMinTime);                           
                 }       
                 
            break;  
            case O_03s_CO_TEST:
            case OdelayCO_DELAY_TEST:
            case OdelayCO_FCT_TEST:
            case O_03s_CO_FCT_TEST:               
                 if(parameter3==OCO_TEST_OPENING_TIME)
                 {   
                 //Breaker Opening time 
                 vrijednost=calc_max_time(3, PhaseAOpenMinTime, PhaseBOpenMinTime, PhaseCOpenMinTime);
                 }  
                 else if(parameter3==OCO_TEST_RECLOSING_TIME)
                 {     
                 //Breaker Reclosing time
                 vrijednost=calc_max_time(3, PhaseACloseMaxTime, PhaseBCloseMaxTime, PhaseCCloseMaxTime);
                 }
                 else if(parameter3==OCO_TEST_OC_TIME)
                 {     
                 //Breaker O-C Time
                 vrijednost=calc_min_time(3, PhaseACloseMaxTime, PhaseBCloseMaxTime, PhaseCCloseMaxTime);
                 
                 vrijednost-=calc_max_time(3, PhaseAOpenMaxTime, PhaseBOpenMaxTime, PhaseCOpenMaxTime);
                 } 
                 else if(parameter3==OCO_TEST_CO_TIME)
                 { //Breaker C-O Time
                 vrijednost=calc_max_time(3, PhaseAOpen2MinTime, PhaseBOpen2MinTime, PhaseCOpen2MinTime);
                    
                vrijednost-=calc_min_time(3, PhaseACloseMaxTime, PhaseBCloseMaxTime, PhaseCCloseMaxTime);                       
                 }                                                            
            break;              
            }
}  
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////                                                                                
//SYNC TIMING FOR CIRCUIT BREAKER:           
////////////////////////////////////////////////////////////////////////////
if(dodajrez==BREAKER_SYNC)
{
            switch(TESTPAR.op_sequence){
            case CLOSE_TEST:        
            if(PreinsertionR=='2')
            {
                vrijednost2=calc_min_time(3, PhaseAResistorMaxTime, PhaseBResistorMaxTime, PhaseCResistorMaxTime);
                                                                                                      
                vrijednost=calc_max_time(3, PhaseAResistorMaxTime, PhaseBResistorMaxTime, PhaseCResistorMaxTime);
                
                 if(vrijednost>vrijednost2)
                {
                    vrijednost-=vrijednost2;
                }                           
                else
                {
                    vrijednost=0;
                }                    
            }
            else
            {
            vrijednost=calc_max_time(3, PhaseACloseMaxTime, PhaseBCloseMaxTime, PhaseCCloseMaxTime);
            
            vrijednost-=calc_min_time(3, PhaseACloseMaxTime, PhaseBCloseMaxTime, PhaseCCloseMaxTime);
            }            
            break;
            case OPEN_TEST:
            vrijednost=calc_max_time(3, PhaseAOpenMinTime, PhaseBOpenMinTime, PhaseCOpenMinTime);
            
            vrijednost-=calc_min_time(3, PhaseAOpenMinTime, PhaseBOpenMinTime, PhaseCOpenMinTime);                       
            break;            
            case TRIPFREE_DELAY_TEST:
            case TRIPFREE_FCT_TEST:
            case CdelayO_TEST:   
            case O_03s_C_TEST:
            case OdelayC_TEST:   
            case O_03s_CO_TEST:
            case OdelayCO_DELAY_TEST:
            case OdelayCO_FCT_TEST:
            case O_03s_CO_FCT_TEST:                   
            if(parameter3=='0')
            {   
            //C Sinhronizacija:
            vrijednost=calc_max_time(3, PhaseACloseMaxTime, PhaseBCloseMaxTime, PhaseCCloseMaxTime); 
            
            vrijednost-=calc_min_time(3, PhaseACloseMaxTime, PhaseBCloseMaxTime, PhaseCCloseMaxTime);                            
            }
            else if(parameter3=='1')
            { 
            //O Sinhronizacija, O1 Sinhronizacija
            vrijednost=calc_max_time(3, PhaseAOpenMinTime, PhaseBOpenMinTime, PhaseCOpenMinTime); 
                           
            vrijednost-=calc_min_time(3, PhaseAOpenMinTime, PhaseBOpenMinTime, PhaseCOpenMinTime);  
            }
            else
            {             
            //O2 Sinhronizacija:
            vrijednost=calc_max_time(3, PhaseAOpen2MinTime, PhaseBOpen2MinTime, PhaseCOpen2MinTime);
            
            vrijednost-=calc_min_time(3, PhaseAOpen2MinTime, PhaseBOpen2MinTime, PhaseCOpen2MinTime);                                     
            }   
            break;        
            } 
}
//////////////////////////////////////////////////////////////////////////////                  
  
////////////////////////////////////////////////////////////////////////////                                                                                
//AUX CONTACTS TIMING::           
////////////////////////////////////////////////////////////////////////////
if((dodajrez>=10)&&(dodajrez<=15)){
    switch (dodajrez)
    {
        case 10: vrijednost=AUX_Results.AUX1Time; break;
        case 11: vrijednost=AUX_Results.AUX2Time; break;
        case 12: vrijednost=AUX_Results.AUX3Time; break;
        case 13: vrijednost=AUX_Results.AUX4Time; break;
        case 14: vrijednost=AUX_Results.AUX5Time; break;
        case 15: vrijednost=AUX_Results.AUX6Time; break;
    }
}
  
////////////////////////////////////////////////////////////////////////////                                                                                
//PRINTOUT OR DISPLAY RESULTS:           
////////////////////////////////////////////////////////////////////////////
               
duzina=strlen(PrintBuffer);
if(PreinsertionR!='3'){    
    //Prikaz vremenskog rezultata:
    TimePrikaz(duzina,  vrijednost);
}
else{
    //Prikaz vrijednosti otpornika (preinsertion Resistance)    
    sprintf(&PrintBuffer[duzina],"%-5.1f Ohm", Rx);
}             
}

//Show Results on Display or Print it on Thermal Printer:  
if(MenOrPrn==0)
{
    sprintf(OutputBuffer, "D%-20s", PrintBuffer);
}
else
{      
    sprintf(OutputBuffer, "-20s", PrintBuffer);
    retValPom=PrintNewLine(0);    
}            
    
return retValPom;    
}

unsigned char PuniPrintBufferCurrent(unsigned char parameter1, unsigned char parameter2, unsigned char MenOrPrn){
float MaxCurrent;
unsigned int Current;
unsigned int TimeRes; 
unsigned int duzina;
unsigned char retValPom=0;
unsigned char calib_ch;

//struct PrinterTexts activetext;
//activetext=printer_texts[language];

#if (REZULTATI_KERIM)
RezZaKerima();
#endif

switch(parameter1){
    //OVDJE TREBA IZMJENITI, od [1] do [3] idu closing currents, od [4] do [6] idu breaking currents
    case '1':  Current=Current_Results.MaxCurrentClosing[0];  TimeRes=Current_Results.MaxCurrentClosingTime[0];  calib_ch=3; break;
    case '2':  Current=Current_Results.MaxCurrentClosing[1];  TimeRes=Current_Results.MaxCurrentClosingTime[1];  calib_ch=4; break;
    case '3':  Current=Current_Results.MaxCurrentClosing[2];  TimeRes=Current_Results.MaxCurrentClosingTime[2];  calib_ch=5; break;
    case '4':  Current=Current_Results.MaxCurrentBreaking[0]; TimeRes=Current_Results.MaxCurrentBreakingTime[0]; calib_ch=0; break;
    case '5':  Current=Current_Results.MaxCurrentBreaking[1]; TimeRes=Current_Results.MaxCurrentBreakingTime[1]; calib_ch=1; break;
    case '6':  Current=Current_Results.MaxCurrentBreaking[2]; TimeRes=Current_Results.MaxCurrentBreakingTime[2]; calib_ch=2; break;
}

////////////////////////////////////////////////////////////////
//ubaceno 20.12.2018. kalibracija max vrijednosti struje:
//ovo su prema sasi koeficijenti:
//0: OPEN1  //1: OPEN2 //2: OPEN3 //3: CLOSE1 //4: CLOSE2 //5: CLOSE3 - pogledaj kako je gore definisano u switch
Current=calculate_gained_value(CALIB.currentch_gain[calib_ch], (float)Current, 0);
Current=apply_offset_correction((unsigned int) Current, (unsigned int)CALIB.currentch_offset[calib_ch], CALIB.currentch_sign[calib_ch]);
////////////////////////////////////////////////////////////////

MaxCurrent=(float)(Current/CURRENT_CH_SCAL_FACTOR);

if(MaxCurrent<0.1){MaxCurrent=0.0; TimeRes=0;}


switch(parameter2)
{
    case '1':         sprintf(&PrintBuffer[0], "%s",         activetext.MaximumCurrent);    break;
    case '2': 
        switch(parameter1)
        {
            case '1': 
            case '2':
            case '3': 
                  sprintf(&PrintBuffer[0], "%s %c",         activetext.ClosingCoil, (unsigned char)(parameter1));    break;
            break;
            case '4':
            case '5':
            case '6':
                      sprintf(&PrintBuffer[0], "%s %c",      activetext.TripCoil, (unsigned char)(parameter1-3));    break;                    
            break;
        }
    break;    
    case '3':
                      sprintf(&PrintBuffer[0],"Imax =  %-6.3f A", MaxCurrent);
                      
    break;         
    case '4':
                      sprintf(&PrintBuffer[0], "tmax = ");
                      duzina=strlen(PrintBuffer);
                      TimePrikaz(duzina,  TimeRes);           
    break;   
}

//Show Results on Display or Print it on Thermal Printer:  
if(MenOrPrn==0)
{
    sprintf(OutputBuffer, "D%-20s", PrintBuffer);
}
else
{      
    sprintf(OutputBuffer, "-20s", PrintBuffer);
    retValPom=PrintNewLine(0);    
}      

return retValPom;
}

unsigned char PuniPrintBufferAnalogCH(unsigned char parameter1, unsigned char parameter2, unsigned char MenOrPrn){
float MaxVoltage, MinVoltage;
unsigned char CHRange;
unsigned int CHTransferFunction;
unsigned char pomchar[6];
unsigned char jedinica[3];
unsigned int i;
unsigned char retValPom=0;
unsigned char pomchar2;

float koeficijent;
float koef2;

//struct PrinterTexts activetext;
//activetext=printer_texts[language];

#if (REZULTATI_KERIM)
RezZaKerima();
#endif  

MaxVoltage=(float)AnalogCH_Results.MaxVoltageCH[(parameter1-49)];
MinVoltage=(float)AnalogCH_Results.MinVoltageCH[(parameter1-49)];

  switch(parameter1)
  {
            case '1':           
            CHRange=TESTPAR.analog1_gain; 
            CHTransferFunction=TESTPAR.analog1tf;
            if(CHTransferFunction!=0){for(i=0; i<3; i++){jedinica[i]=TESTPAR.analog1tf_unit[i];}}     
            break;
            case '2':
            CHRange=TESTPAR.analog2_gain; 
            CHTransferFunction=TESTPAR.analog2tf; 
            if(CHTransferFunction!=0){for(i=0; i<3; i++){jedinica[i]=TESTPAR.analog2tf_unit[i];}}     
            break;
            case '3':                                        
            CHRange=TESTPAR.analog3_gain; 
            CHTransferFunction=TESTPAR.analog3tf;
            if(CHTransferFunction!=0){for(i=0; i<3; i++){jedinica[i]=TESTPAR.analog3tf_unit[i];}}      
            break;
            case '4':                                        
            CHRange=TESTPAR.analog4_gain; 
            CHTransferFunction=TESTPAR.analog4tf;
            if(CHTransferFunction!=0){for(i=0; i<3; i++){jedinica[i]=TESTPAR.analog4tf_unit[i];}}      
            break;
            case '5':                                        
            CHRange=TESTPAR.analog5_gain; 
            CHTransferFunction=TESTPAR.analog5tf;
            if(CHTransferFunction!=0){for(i=0; i<3; i++){jedinica[i]=TESTPAR.analog5tf_unit[i];}}      
            break;                 
            case '6':                                        
            CHRange=TESTPAR.analog6_gain; 
            CHTransferFunction=TESTPAR.analog6tf;
            if(CHTransferFunction!=0){for(i=0; i<3; i++){jedinica[i]=TESTPAR.analog6tf_unit[i];}}      
            break;                                            
  }    
                                                 
if(CHTransferFunction==0)
{               
    jedinica[0]='V';
    jedinica[1]=' ';
    jedinica[2]=' ';   
}

switch(CHRange){
        case ACH_RANGE_LL:
             sprintf(&pomchar[0], "0.5 V");     
             koeficijent=ACH_RANGE_LL_SCAL_FACTOR;
        break;
        case ACH_RANGE_LH:
            sprintf(&pomchar[0], "2.5 V");
            koeficijent=ACH_RANGE_LH_SCAL_FACTOR;  
        break;
        case ACH_RANGE_HL: 
            sprintf(&pomchar[0], "60 V");  
             koeficijent=ACH_RANGE_HL_SCAL_FACTOR; 
        break;
        case ACH_RANGE_HH: 
            sprintf(&pomchar[0], "300 V");     
            koeficijent=ACH_RANGE_HH_SCAL_FACTOR;
        break;                
}         
  
MaxVoltage=calculate_gained_value(CALIB.analogch_gain[parameter1-'1'][(CHRange-'0')], (float)MaxVoltage, 0);
MaxVoltage=apply_offset_correction((unsigned int) MaxVoltage, (unsigned int)CALIB.analogch_offset[parameter1-'1'][(CHRange-'0')], CALIB.analogch_sign[i][(CHRange-'0')]);
MinVoltage=calculate_gained_value(CALIB.analogch_gain[parameter1-'1'][(CHRange-'0')], (float)MinVoltage, 0); 
MinVoltage=apply_offset_correction((unsigned int)MinVoltage, (unsigned int)CALIB.analogch_offset[parameter1-'1'][(CHRange-'0')], CALIB.analogch_sign[i][(CHRange-'0')]);
  
MaxVoltage-=32768.0;
MinVoltage-=32768.0; 

MaxVoltage/=koeficijent;
MinVoltage/=koeficijent;  

if(CHTransferFunction!=0)
{
    koef2=(1000.0/CHTransferFunction);
    MaxVoltage*=koef2;
    MinVoltage*=koef2;    
}    

if(CHTransferFunction!=0){pomchar2='I';}  else{pomchar2='U';}       

switch(parameter2){
    case '1':   
        if(CHTransferFunction==0){ 
               if(language==0){    
               sprintf(&PrintBuffer[0], "Analog CH %c", parameter1);
               }
               else{
               sprintf(&PrintBuffer[0], "CH Analogique %c", parameter1);               
               }                       
        }
        else{                                        
               if(language==0){
               sprintf(&PrintBuffer[0], "Analog CH %c  %-4u", parameter1, CHTransferFunction);
               }
               else{
               sprintf(&PrintBuffer[0], "CH Analogique %c %-4u", parameter1, CHTransferFunction);
               }
        }                                                                  
    break;
    case '2':                                                                       
        if(CHTransferFunction==0){       
            if(language==0){                                 
            sprintf(&PrintBuffer[0], "Range %s", pomchar);
            }
            else{
            sprintf(&PrintBuffer[0], "Gamme %s", pomchar);            
            }
        }           
        else{        
            if(language==0){
            sprintf(&PrintBuffer[0], "Range %-5s  mV/A%c%c", pomchar, jedinica[1], jedinica[2]);
            }
            else{                                                                                           
            sprintf(&PrintBuffer[0], "Gamme %-8s  mV/A", pomchar);
            }
        }         
    break;
    case '3':
        if(CHTransferFunction==0){  
           sprintf(&PrintBuffer[0], "%cmax = %-3.2f %c%c%c", pomchar2, MaxVoltage, jedinica[0], jedinica[1], jedinica[2]);
        }
        else{
           sprintf(&PrintBuffer[0], "%cmax = %-3.2f A%c%c", pomchar2, MaxVoltage, jedinica[1], jedinica[2]);
        }
    break;   
    case '4': 
        if(CHTransferFunction==0){          
           sprintf(&PrintBuffer[0], "%cmin = %-3.2f %c%c%c",  pomchar2, MinVoltage, jedinica[0], jedinica[1], jedinica[2]);                 
        }
        else{
           sprintf(&PrintBuffer[0], "%cmin = %-3.2f A%c%c",  pomchar2, MinVoltage, jedinica[1], jedinica[2]);                 
        }
    break;           
}         

//Show Results on Display or Print it on Thermal Printer:  
if(MenOrPrn==0)
{
    sprintf(OutputBuffer, "D%-20s", PrintBuffer);
}
else{      
    sprintf(OutputBuffer, "-20s", PrintBuffer);
    retValPom=PrintNewLine(0);    
}      

return retValPom;
}

void LinearTransducerRes_Prikaz(unsigned char kanal, unsigned char rezultat){
float TotalTravel;
float OverTravel;
float Rebound;
float ContactWipe=0;
float pom;
float pom2;
float pom3=(-1.0);
float GranicaSuma;
unsigned char GranicaSumaLinear;
unsigned char karakter[4];
unsigned char kanalCW1;
unsigned char kanalCW2;	
unsigned char kanalpom;

kanalpom=kanal;

switch(kanal)
{
    case 'A':
    kanal=0; 
        switch (rezultat){
            case '1': kanalCW1=0; kanalCW2=0; break;
            case '2': kanalCW1=0; kanalCW2=1; break;
            case '3': kanalCW1=1; kanalCW2=0; break;
            case '4': kanalCW1=1; kanalCW2=1; break;  
        }
    break;                                  
    case 'B':
    kanal=1;  
       switch (rezultat){
            case '1': kanalCW1=2; kanalCW2=0; break;
            case '2': kanalCW1=2; kanalCW2=1; break;
            case '3': kanalCW1=3; kanalCW2=0; break;
            case '4': kanalCW1=3; kanalCW2=1; break;  
        }  
    break;
    case 'C':
    kanal=2;                    
       switch (rezultat){
            case '1': kanalCW1=4; kanalCW2=0; break;
            case '2': kanalCW1=4; kanalCW2=1; break;
            case '3': kanalCW1=5; kanalCW2=0; break;
            case '4': kanalCW1=5; kanalCW2=1; break;  
        }
    break;    
}

GranicaSumaLinear=0;
if(TESTPAR.travel_unit==TRAVEL_UNIT_mm){GranicaSuma=1.3;}
else                                   {GranicaSuma=0.06;}

TotalTravel=(float)(LTransd_Results.TotalTravel[kanal]);
OverTravel =(float)(LTransd_Results.OverTravel[kanal]);
Rebound	   =(float)(LTransd_Results.Rebound[kanal]);

if((kanalpom=='A')||(kanalpom=='B')||(kanalpom=='C'))
{
    ContactWipe=(float)(LTransd_Results.ContactWipe[kanalCW1][kanalCW2]);
}

if(TotalTravel<0)	 {TotalTravel*=pom3;}
if(OverTravel<0) 	 {OverTravel*=pom3;}
if(Rebound<0)    	 {Rebound*=pom3;}
if(ContactWipe<0)    {ContactWipe*=pom3;}

if(TESTPAR.travel_unit==TRAVEL_UNIT_mm)
{
    //Metric:
    pom=(TESTPAR.tr_lenght_mm[kanal]/10.0);
	karakter[0]='m';
	karakter[1]='m';
	karakter[2]=' ';
	karakter[3]=' ';
}

else 
{ 
    //Imperial:
    pom=(TESTPAR.tr_lenght_inch[kanal]/1000.0);
	karakter[0]='i';
	karakter[1]='n';
	karakter[2]='c';
	karakter[3]='h';
}

pom2=(TESTPAR.tr_gain[kanal]*1.0);
if(pom2!=0)
{
    pom3=(pom/pom2);
}   
else{
    pom3=0;
}

TotalTravel*=pom3;
OverTravel *=pom3;
Rebound    *=pom3;
ContactWipe*=pom3;

if(TotalTravel>GranicaSuma)
{
    if(LTransd_Results.OverTravelTime==0){OverTravel=0;}
    if((OverTravel==0)||(Rebound>(0.7*TotalTravel))){Rebound=0;}
}
else
{TotalTravel=0; OverTravel=0; Rebound=0; ContactWipe=0; GranicaSumaLinear=1;}

//if(OverTravel>TotalTravel){OverTravel-=TotalTravel;}
//else{OverTravel=0;}
//if(Rebound<OverTravel){Rebound=OverTravel-Rebound;}
//else{Rebound=0;}

if((kanalpom=='A')||(kanalpom=='B')||(kanalpom=='C')){ 
        if(TESTPAR.break_units=='1'){
            sprintf(&PrintBuffer[0],"%c =%4.2f %c%c%c%c", kanalpom, ContactWipe, karakter[0], karakter[1], karakter[2], karakter[3]);
        }
        else{sprintf(&PrintBuffer[0],"%c%c =%4.2f %c%c%c%c", kanalpom, rezultat, ContactWipe, karakter[0], karakter[1], karakter[2], karakter[3]);}
}
else{
switch(rezultat){
    case '2':    
        sprintf(&PrintBuffer[0],"%s:%6.3f %c%c%c%c", activetext.txtStroke, TotalTravel, karakter[0], karakter[1], karakter[2], karakter[3]);
    break;                                                                                                                   
    case '3':
        sprintf(&PrintBuffer[0],"%s:%6.3f %c%c%c%c", activetext.txtOvertravel, OverTravel, karakter[0], karakter[1], karakter[2], karakter[3]);    
    break;
    case '4':
        sprintf(&PrintBuffer[0],"%s:%6.3f %c%c%c%c", activetext.txtRebound, Rebound, karakter[0], karakter[1], karakter[2], karakter[3]);    
    break;    
}
}
}	


#define DigTransducerKoef1 (float) 0.036 
//Ovaj koeficijent se odredjuje na osnovu broja impulsa po obrtaju za digitalni transducer. Za 2500impulsa on je DigTransducerKoef=360stepeni/(2500*4)=0,144/4=0.036
//gdje je 4 koeficijent jer se broji svaka ivica impulsa...		

void  DigitalTransRes_Prikaz(unsigned char kanal, unsigned char rezultat){ 
float TotalTravel;
float OverTravel;			
float Rebound;	
float ContactWipe=0.0;
float pom3=(-1.0);
float koeficijent=0.349055556; //koeficijent=20mm*pi/180
unsigned char karakter[4];
unsigned char kanalCW1;
unsigned char kanalCW2;	
unsigned char kanalpom;
//struct PrinterTexts activetext;
//activetext=printer_texts[language];

kanalpom=kanal;

switch(kanal){
    case 'A':
    kanal=0; 
        switch (rezultat){
            case '1': kanalCW1=0; kanalCW2=0; break;
            case '2': kanalCW1=0; kanalCW2=1; break;
            case '3': kanalCW1=1; kanalCW2=0; break;
            case '4': kanalCW1=1; kanalCW2=1; break;  
        }
    break;                                  
    case 'B':
    kanal=1;  
       switch (rezultat){
            case '1': kanalCW1=2; kanalCW2=0; break;
            case '2': kanalCW1=2; kanalCW2=1; break;
            case '3': kanalCW1=3; kanalCW2=0; break;
            case '4': kanalCW1=3; kanalCW2=1; break;  
        }  
    break;
    case 'C':
    kanal=2;                    
       switch (rezultat){
            case '1': kanalCW1=4; kanalCW2=0; break;
            case '2': kanalCW1=4; kanalCW2=1; break;
            case '3': kanalCW1=5; kanalCW2=0; break;
            case '4': kanalCW1=5; kanalCW2=1; break;  
        }
    break;    
}

						  
TotalTravel=((float)DTransd_Results.TotalTravel[kanal])*(float)DigTransducerKoef1;
//TotalTravel*=(float)DigTransducerKoef1;
OverTravel=((float)DTransd_Results.OverTravel[kanal])*(float)DigTransducerKoef1;
//OverTravel*=DigTransducerKoef1;
Rebound=((float)DTransd_Results.Rebound[kanal])*(float)DigTransducerKoef1;;
//Rebound*=DigTransducerKoef1;

if((kanalpom=='A')||(kanalpom=='B')||(kanalpom=='C'))
{
    ContactWipe=(signed int)(DTransd_Results.ContactWipe[kanalCW1][kanalCW2]);
    ContactWipe*=DigTransducerKoef1;
}

//if(OverTravel!=0){OverTravel=(OverTravel-TotalTravel);}
//if(Rebound!=0)   {Rebound=(Rebound-TotalTravel);}
		 						 
if(TESTPAR.travel_unit==TRAVEL_UNIT_mm){	
	TotalTravel*=koeficijent;
	OverTravel*=koeficijent;
	Rebound*=koeficijent;  
    ContactWipe*=koeficijent;
	karakter[0]='m';
	karakter[1]='m';
	karakter[2]=' ';
	karakter[3]=' ';
}
else if(TESTPAR.travel_unit==TRAVEL_UNIT_inch){
	koeficijent*=0.0393700787 ; //inch
	TotalTravel*=koeficijent;
	OverTravel*=koeficijent;
	Rebound*=koeficijent;     
    ContactWipe*=koeficijent;
	karakter[0]='i';
	karakter[1]='n';
	karakter[2]='c';
	karakter[3]='h';
}
else{
	karakter[0]='D';
	karakter[1]='e';
	karakter[2]='g';
	karakter[3]=' ';
}
if(TotalTravel<0){TotalTravel*=pom3;}  
if(OverTravel<0){OverTravel*=pom3;}
if(Rebound<0){Rebound*=pom3;}
if(ContactWipe<0){ContactWipe*=pom3;}

//if(DTransd_Results.OverTravelTime[kanal]==0){OverTravel=0;}
//if(OverTravel==0)                            {Rebound=0;}

//if(OverTravel>TotalTravel){OverTravel-=TotalTravel;}
//else{OverTravel=0;}
//if(Rebound<OverTravel){Rebound=OverTravel-Rebound;}
//else{Rebound=0;}


if((kanalpom=='A')||(kanalpom=='B')||(kanalpom=='C')){   
        if(TESTPAR.break_units=='1'){
            sprintf(&PrintBuffer[0],"%c =%4.2f %c%c%c%c", kanalpom, ContactWipe, karakter[0], karakter[1], karakter[2], karakter[3]);
        }
        else{sprintf(&PrintBuffer[0],"%c%c =%4.2f %c%c%c%c", kanalpom, rezultat, ContactWipe, karakter[0], karakter[1], karakter[2], karakter[3]);}
}
else{
switch(rezultat){
    case '2':    
        sprintf(&PrintBuffer[0],"%2s:%4.2f %c%c%c%c", activetext.txtStroke, TotalTravel, karakter[0], karakter[1], karakter[2], karakter[3]);
    break;                                                                                                                   
    case '3':
        sprintf(&PrintBuffer[0],"%2s:%4.2f %c%c%c%c", activetext.txtOvertravel, OverTravel, karakter[0], karakter[1], karakter[2], karakter[3]);    
    break;
    case '4':
        sprintf(&PrintBuffer[0],"%2s:%4.2f %c%c%c%c", activetext.txtRebound, Rebound, karakter[0], karakter[1], karakter[2], karakter[3]);    
    break;   
}
}
}

unsigned char PuniPrintBufferMotion(unsigned char parameter1, unsigned char parameter2, unsigned char MenOrPrn){
unsigned char kanal;
unsigned char retValPom=0;

//struct PrinterTexts activetext;
//activetext=printer_texts[language];

switch(parameter1)
{
    case 'A':
    case 'B':
    case 'C':
        kanal=parameter1;
    break;
    default:
        kanal=(parameter1-50);
    break;
}


#if (REZULTATI_KERIM)
RezZaKerima();
#endif

switch(parameter1){
    case '1':   
        sprintf(&PrintBuffer[0], "%s TR%c", activetext.MotionResults, parameter2);  
    break;     
    case '2':    
        switch(TESTPAR.transd_type)
        {
            case 'A':
            //Digitalni:
                DigitalTransRes_Prikaz(kanal, parameter2); //Stroke  
            break;
            default:    
            //Linearni:
                LinearTransducerRes_Prikaz(kanal, parameter2); //Stroke
            break;
        }              
    break;                                      
    case '3':  
        switch(TESTPAR.transd_type)
        {
            case 'A':
            //Digitalni:
                DigitalTransRes_Prikaz(kanal, parameter2); //OverTravel  
            break;
            default:    
            //Linearni:
                LinearTransducerRes_Prikaz(kanal, parameter2); //OverTravel
            break;
        }                 
    break;                         
    case '4':
        if(TESTPAR.transd_type=='A')
        {
        //Digitalni:
            DigitalTransRes_Prikaz(kanal, parameter2); //Rebound         
        }
        else
        { 
        //Linearni:
            LinearTransducerRes_Prikaz(kanal, parameter2); //Rebound
        }        
    break;      
    case '5': 
            sprintf(&PrintBuffer[0],"%-20s", activetext.ContactWipe);
    break;             
    case '6': 
            sprintf(&PrintBuffer[0],"%-20s", activetext.MotionResultsAre);
    break;
    case '7':  
            sprintf(&PrintBuffer[0],"%-20s", activetext.AvailableOnlyFor);
    break;                                   
    case '8':
            sprintf(&PrintBuffer[0],"%-20s", activetext.OpenAndCloseTest);
    break;
    case 'A': 
    case 'B':
    case 'C':
         if(TESTPAR.transd_type=='A'){
        //Digitalni:
            DigitalTransRes_Prikaz(kanal, parameter2); //CW   kanal='A', 'B' ili 'C'; parameter2='1', '2', '3' ili '4';      
        }
        else{ 
        //Linearni:
            LinearTransducerRes_Prikaz(kanal, parameter2); //CW  kanal='A', 'B' ili 'C'; parameter2='1', '2', '3' ili '4';
        }        
    break;            
        
}  
if(MenOrPrn==0){
    sprintf(OutputBuffer, "D%-20s", PrintBuffer);
    }
else{      
    sprintf(OutputBuffer, "-20s", PrintBuffer);
    retValPom=PrintNewLine(0);    
}   
return retValPom;       
}

void set_osg_or_bsg(void)
{
//set osg:
if(TESTPAR.bs_grounded==NO)
{
        //TURN_OFF_BSG;   
        BSG_OFF_OSG_ON;                
}
//set bsg:
else
{
    //Set BSG measuring mode:
    BSG_CHARGE_REL_OFF;  
    //Set BSG and prepare the device for measurement:
    if(TESTPAR.break_units==ONE_BREAK_UNIT_PER_PHASE)
    {
        BSG_SEQUENCE_ONE_BPPF;        
    }
    else
    {
        BSG_SEQUENCE_TWO_BPPF;
    } 
}
}

void drive_close_coil(unsigned char SetReset)
{
if(TESTPAR.c_coil_phase==COIL_PULSE_ALL_PHASES)
{         
    if(SetReset)
    {
        SET_CLOSING1_IGBT;
        SET_CLOSING2_IGBT;
        SET_CLOSING3_IGBT;
    }                      
    else
    {
        CLR_CLOSING1_IGBT;
        CLR_CLOSING2_IGBT;
        CLR_CLOSING3_IGBT;
    }                                             
}                             
else if(TESTPAR.c_coil_phase==COIL_PULSE_PHASE1)
{
    if(SetReset)
    {
        SET_CLOSING1_IGBT;    
    }                      
    else
    {
        CLR_CLOSING1_IGBT;
    }                        
}                                 
else if(TESTPAR.c_coil_phase==COIL_PULSE_PHASE2)
{
    if(SetReset)
    {
        SET_CLOSING2_IGBT;    
    }                      
    else
    {
        CLR_CLOSING2_IGBT;
    }                        
}
else if(TESTPAR.c_coil_phase==COIL_PULSE_PHASE3)
{
    if(SetReset)
    {
        SET_CLOSING3_IGBT;    
    }                      
    else
    {
        CLR_CLOSING3_IGBT;
    }                        
}
}

void drive_open_coil(unsigned char SetReset)
{

if(TESTPAR.o_coil_phase==COIL_PULSE_ALL_PHASES)
{         
    if(SetReset)
    {
        SET_BREAKING1_IGBT;
        SET_BREAKING2_IGBT;
        SET_BREAKING3_IGBT;
    }                      
    else
    {
        CLR_BREAKING1_IGBT;
        CLR_BREAKING2_IGBT;
        CLR_BREAKING3_IGBT;
    }                                             
}                             
else if(TESTPAR.o_coil_phase==COIL_PULSE_PHASE1)
{
    if(SetReset)
    {
        SET_BREAKING1_IGBT;    
    }                      
    else
    {
        CLR_BREAKING1_IGBT;
    }                        
}                                 
else if(TESTPAR.o_coil_phase==COIL_PULSE_PHASE2)
{
    if(SetReset)
    {
        SET_BREAKING2_IGBT;    
    }                      
    else
    {
        CLR_BREAKING2_IGBT;
    }                        
}
else if(TESTPAR.o_coil_phase==COIL_PULSE_PHASE3)
{
    if(SetReset)
    {
        SET_BREAKING3_IGBT;    
    }                      
    else
    {
        CLR_BREAKING3_IGBT;
    }                        
}
}

void coil_control(unsigned int address)
{
if(TESTPAR.ext_trigg_src==EXT_TRIG_OFF)
{
    switch(TESTPAR.op_sequence)     
    { 
    case CLOSE_TEST:           if(address==(TESTPAR.c_pulse_lenght-1)){drive_close_coil(0);}  break;                      
    case OPEN_TEST:            if(address==(TESTPAR.o_pulse_lenght-1)){drive_open_coil (0);}  break;
    /**
    case DRM_TEST:
                              if(address==1000){drive_open_coil(0, 1);}
                              if(address==2000){PORTC.1=0;}
                              if(address==(ControlPulse.OpenPulse+1000)){drive_open_coil (0);}
    break;
    **/                                                                  
    case OdelayC_TEST:               
    case O_03s_C_TEST:       
                               if(address==TESTPAR.o_pulse_lenght)    {drive_open_coil (0);}
                               if(address==TESTPAR.reclosing_delay)   {drive_close_coil(1);}
    break;                           
    case CdelayO_TEST:          
    case TRIPFREE_DELAY_TEST:     
                               if(address==TESTPAR.c_pulse_lenght)    {drive_close_coil(0);}
                               if(address==TESTPAR.trip_free_delay)   {drive_open_coil (1);}                                                    
    break;                  
                    
    case TRIPFREE_FCT_TEST:
    //Iskoristiti TRIG_IN signal za FCT
                               if(address==TESTPAR.c_pulse_lenght)    {drive_close_coil(0);}                               
                               if(address>50)//Ostavi 5ms da ne daje nalog za ovo
                               { 
                                        //DODAJ OVAJ DIO ZA KONTROLU:
                                        //if(TRIG_IN==0){drive_open_coil (1);}
                                        //Dodati i MC_ARRAY2 uslov        
                                        if(control_after_first_contact_touched)
                                        {                                        
                                            //todo: ovdje treba napraviti da gleda prvi kontakt (0b11001100) je za CAT6x:
                                            if(TESTPAR.break_units=='1')
                                            {
                                                if(ADC_8bit_results.MC_ARRAY[0]!=0xCC)//0b11001100)
                                                {
                                                drive_open_coil(1); 
                                                control_after_first_contact_touched=0;
                                                }
                                            }
                                            else if(TESTPAR.break_units=='2')
                                            {
                                                if(ADC_8bit_results.MC_ARRAY[0]!=0xCC)//0b11001100)
                                                {
                                                drive_open_coil(1); 
                                                control_after_first_contact_touched=0;
                                                }
                                            }   
                                            else
                                            {
                                                if(ADC_8bit_results.MC_ARRAY[0]!=0x00)//0b00000000)
                                                {
                                                drive_open_coil(1); 
                                                control_after_first_contact_touched=0;
                                                }
                                            }
                                        }   
                               }                    
    break;
                    
    case O_03s_CO_TEST:
    case OdelayCO_DELAY_TEST:  
                               if(address==TESTPAR.o_pulse_lenght)    {drive_open_coil (0);}
                               if(address==TESTPAR.oco_delay1)        {drive_close_coil(1);}  
                               if(address==(TESTPAR.c_pulse_lenght+TESTPAR.oco_delay1))     {{drive_close_coil(0);}}   
                               if(address==(TESTPAR.oco_delay1+TESTPAR.oco_delay2))         {{drive_open_coil (1);}}                                    
                    
    break;                                     
                                                                                                                            
    case OdelayCO_FCT_TEST:   
    case O_03s_CO_FCT_TEST: 
    //Iskoristiti TRIG_IN signal za FCT
                               if(address==TESTPAR.o_pulse_lenght)    {drive_open_coil (0);}
                               if(address==TESTPAR.oco_delay1)        {drive_close_coil(1);}  
                               if(address==(TESTPAR.c_pulse_lenght+TESTPAR.oco_delay1))     {{drive_close_coil(0);}}   
                               if(address>TESTPAR.oco_delay1)
                               {                         
                                        //if(TRIG_IN==0){drive_open_coil (1);} 
                                        //Dodati i MC_ARRAY2 uslov                  
                                        if(control_after_first_contact_touched)
                                        {         
                                            #if 0
                                            if(ADC_8bit_results.MC_ARRAY[0]!=0)
                                            {
                                                drive_open_coil(1); 
                                                control_after_first_contact_touched=0;
                                            }    
                                            #endif
                                            //todo: ovdje treba napraviti da gleda prvi kontakt (0b11001100) je za CAT6x:
                                            if(TESTPAR.break_units=='1')
                                            {
                                                if(ADC_8bit_results.MC_ARRAY[0]!=0xCC)//0b11001100)
                                                {
                                                drive_open_coil(1); 
                                                control_after_first_contact_touched=0;
                                                }
                                            }
                                            else if(TESTPAR.break_units=='2')
                                            {
                                                if(ADC_8bit_results.MC_ARRAY[0]!=0xCC)//0b11001100)
                                                {
                                                drive_open_coil(1); 
                                                control_after_first_contact_touched=0;
                                                }
                                            }   
                                            else
                                            {
                                                if(ADC_8bit_results.MC_ARRAY[0]!=0x00)//0b00000000)
                                                {
                                                drive_open_coil(1); 
                                                control_after_first_contact_touched=0;
                                                }
                                            }                                            
                                        }                                 
                               }      
    break;                       
    }
}
}

void PosaljiMSodgovor(unsigned char odgovor){ my_putchar(0x1B); my_putchar('m'); my_putchar('M'); my_putchar('S');  my_putchar(odgovor); my_putchar(0x00);}
extern void HAL_Delay();

#define CURRCLOSING1   (unsigned char) 6
#define CURRCLOSING2   (unsigned char) 7
#define CURRCLOSING3   (unsigned char) 8
#define CURRBREAKING1  (unsigned char) 9
#define CURRBREAKING2  (unsigned char) 10
#define CURRBREAKING3  (unsigned char) 11
#define LINEARTRANSD1  (unsigned char) 12
#define LINEARTRANSD2  (unsigned char) 13
#define LINEARTRANSD3  (unsigned char) 14
#define DIGITALTRANSD1 (unsigned char) 15
#define DIGITALTRANSD2 (unsigned char) 16
#define DIGITALTRANSD3 (unsigned char) 17

#define MIN            (unsigned char) 'A'
#define MAX            (unsigned char) 'B' 
	
unsigned int check_for_stop_button()
{
	//TODO: DEFINISATI STOP BUTTON PROCEDURU!!!
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)==0)
	{
		   return(STOP_BUTTON);
	}	
return MAIN_OK;
}

unsigned int compare_analog_value(unsigned char ch, unsigned char MinMax, unsigned int value){
unsigned int retVal;
unsigned int pomVal;   
    switch (ch)
    {
        case ANALOGCH1:     pomVal=ADC_Results.ANCH[0];         break;
        case ANALOGCH2:     pomVal=ADC_Results.ANCH[1];         break;
        case ANALOGCH3:     pomVal=ADC_Results.ANCH[2];         break;
        case ANALOGCH4:     pomVal=ADC_Results.ANCH[3];         break;
        case ANALOGCH5:     pomVal=ADC_Results.ANCH[4];         break;
        case ANALOGCH6:     pomVal=ADC_Results.ANCH[5];         break;
        case CURRCLOSING1:  pomVal=abs(ADC_Results.CURRENT[3]); break;
        case CURRCLOSING2:  pomVal=abs(ADC_Results.CURRENT[4]); break;
        case CURRCLOSING3:  pomVal=abs(ADC_Results.CURRENT[5]); break;
        case CURRBREAKING1: pomVal=abs(ADC_Results.CURRENT[0]); break;
        case CURRBREAKING2: pomVal=abs(ADC_Results.CURRENT[1]); break;
        case CURRBREAKING3: pomVal=abs(ADC_Results.CURRENT[2]); break;             
    }            
    if(MinMax==MAX)
    {    
        if(pomVal>value){retVal=pomVal;} 
        else            {retVal=value; }
    }
    else
    {
        if(pomVal<value){retVal=pomVal;}
        else            {retVal=value; }    
    }            
return (retVal);    
} 

void ProcessAnalogCH(unsigned char ch, unsigned char Min_Max, unsigned int *Voltage, unsigned int *VoltageTime, unsigned int adresa){
unsigned int pom;
unsigned int Voltagepom;

Voltagepom=*Voltage;

pom=compare_analog_value(ch, Min_Max, Voltagepom);
if(pom!=Voltagepom)
{            
        *Voltage=pom;
        *VoltageTime=adresa;
}            
}

void search_max_current_per_ch(unsigned int address)
{    
                     
char i, pom;

//disregard first and last 3 samples in min max calculation:
if(address<3){return;}
if(address>TESTPAR.test_duration-3){return;}

    for(i=0; i<NUMBER_OF_CURRENT_CHS; i++)
    {                                         
        switch(i)
        {
            case 0:pom=CURRCLOSING1;  break;
            case 1:pom=CURRCLOSING2;  break;
            case 2:pom=CURRCLOSING3;  break;
            case 3:pom=CURRBREAKING1; break;
            case 4:pom=CURRBREAKING2; break;
            case 5:pom=CURRBREAKING3; break;
        }   
        if(i<3) {ProcessAnalogCH(pom, MAX, &Current_Results.MaxCurrentClosing[i]      , &Current_Results.MaxCurrentClosingTime[i],      address);}
        else    {ProcessAnalogCH(pom, MAX, &Current_Results.MaxCurrentBreaking[(i-3)] , &Current_Results.MaxCurrentBreakingTime[(i-3)], address);}
    }
}  

void search_min_max_voltage_per_ch(unsigned int address)
{                         
char i, pom;

pom=ANALOGCH1;

    for(i=0; i<NUMBER_OF_ANALOG_CHS; i++)
    {                                         
        ProcessAnalogCH(pom, MAX, &AnalogCH_Results.MaxVoltageCH[i], &AnalogCH_Results.MaxVoltageCHTime[i], address);
        ProcessAnalogCH(pom, MIN, &AnalogCH_Results.MinVoltageCH[i], &AnalogCH_Results.MinVoltageCHTime[i], address);
        pom++;        
    }
}  


void check_mc_test_results(unsigned char *search_mc_res)
{
unsigned int MC_first_values[NUMBER_OF_MAIN_CONTACTS];
unsigned int MC_last_values[NUMBER_OF_MAIN_CONTACTS];
int i;

read_adc_res_sram_test(1); //bilo read_adc_res_sram(0); 
for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++){MC_first_values[i]=ADC_Results.MC[i];} 
                           
read_adc_res_sram_test(TESTPAR.test_duration-2); //Treba minus 1. provjeri! //bilo read_adc_res_sram(TESTPAR.test_duration-2);
for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++){MC_last_values[i]=ADC_Results.MC[i];}

for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
{
    switch(TESTPAR.op_sequence)
    {
        case CLOSE_TEST:
            //For close test, breaker goes from open to close position:
            if((MC_first_values[i]<MAIN_THRESHOLD_LOW)&&(MC_last_values[i]>MAIN_THRESHOLD_HIGH)){*search_mc_res=1;}
            else                                                                                {*search_mc_res=0;}       
        break;
        case OPEN_TEST: 
        case FIRST_TRIP_TEST:
            //For open test, breaker goes from close to open position:
            //For OCO test, breaker goes from close position to open position:
            if((MC_first_values[i]>MAIN_THRESHOLD_HIGH)&&(MC_last_values[i]<MAIN_THRESHOLD_LOW)) {*search_mc_res=1;}
            else                                                                                 {*search_mc_res=0;}       
        break;       
        case O_03s_C_TEST:  
        case OdelayC_TEST:        
            //For OC test, breaker goes from close to close position:
            if((MC_first_values[i]>MAIN_THRESHOLD_HIGH)&&(MC_last_values[i]>MAIN_THRESHOLD_HIGH)){*search_mc_res=1;}
            else                                                                                 {*search_mc_res=0;}       
        break;                                                              
        case TRIPFREE_DELAY_TEST:
        case TRIPFREE_FCT_TEST: 
        case CdelayO_TEST:  
            //For CO test, breaker goes from open to open position:
            if((MC_first_values[i]<MAIN_THRESHOLD_LOW)&&(MC_last_values[i]<MAIN_THRESHOLD_LOW))  {*search_mc_res=1;}
            else                                                                                 {*search_mc_res=0;}          
        break;  
        case O_03s_CO_TEST:
        case OdelayCO_DELAY_TEST:
        case OdelayCO_FCT_TEST:
        case O_03s_CO_FCT_TEST:
            //For OCO test, breaker goes from close to open position:
            if((MC_first_values[i]>MAIN_THRESHOLD_HIGH)&&(MC_last_values[i]<MAIN_THRESHOLD_LOW))  {*search_mc_res=1;}
            else                                                                                  {*search_mc_res=0;}        
        break;               
    }         
    *search_mc_res++;
}
}

void init_results_vaules(void)
{
int i;    

    for(i=0; i<NUMBER_OF_ANALOG_CHS; i++)
    { 
        AnalogCH_Results.MaxVoltageCH[i]=0;
        AnalogCH_Results.MinVoltageCH[i]=0xFFFF;
        AnalogCH_Results.MaxVoltageCHTime[i]=0;
        AnalogCH_Results.MinVoltageCHTime[i]=0; 
    }   
    
    for(i=0; i<NUMBER_OF_CURRENT_CHS/2; i++)
    {
         Current_Results.MaxCurrentClosing[i]=0;
         Current_Results.MaxCurrentBreaking[i]=0;
         Current_Results.MaxCurrentClosingTime[i]=0;
         Current_Results.MaxCurrentBreakingTime[i]=0;        
    }      
    
    for(i=0; i<NUMBER_OF_LT_CHS; i++)
    {                                    
        LTransd_Results.TotalTravel[i]=0;
        LTransd_Results.OverTravel[i]=0;
        LTransd_Results.OverTravelTime[i]=0;
        LTransd_Results.Rebound[i]=0;
        LTransd_Results.ReboundTime[i]=0; 
        LTransd_Results.Max[i]=-32767; //min signed int value
        LTransd_Results.Min[i]=+32767;   
    }
    
    for(i=0; i<NUMBER_OF_DT_CHS; i++)
    {
        DTransd_Results.OverTravel[i]=0;
        DTransd_Results.OverTravelTime[i]=0;
        DTransd_Results.Rebound[i]=0;
        DTransd_Results.ReboundTime[i]=0;  
        DTransd_Results.Max[i]=-2147483647; //min signed long value
        DTransd_Results.Min[i]=+2147483647; //max signed long value  
    }
    
    for(i=0; i<6; i++)
    {           
        LTransd_Results.ContactWipe[i][0]=0;
        LTransd_Results.ContactWipe[i][1]=0;    
    }                                 

}

void calculate_total_travel(signed int* fin_val_lin, unsigned char* direction_lin, signed long* fin_val_dig, unsigned char* direction_dig)
{
unsigned int i;
float lin_transd_aux;
signed int lin_transd_aux1;
signed int lin_transd_aux2;
float dig_transd_aux;
signed long dig_transd_aux1;
signed long dig_transd_aux2;  

    //Calculate total travel for linear transducers:
    for(i=0; i<NUMBER_OF_LT_CHS; i++)
    {                          
        //get last recorded value:    
        read_adc_res_sram_test(TESTPAR.test_duration-1);
        lin_transd_aux1=ADC_Results.LT[i];                      
        *fin_val_lin=lin_transd_aux1; 
        //get first recorded value:                                           
        read_adc_res_sram_test(1);        
        lin_transd_aux2=ADC_Results.LT[i];                
        lin_transd_aux=(float)lin_transd_aux1-(float)lin_transd_aux2;
        if(lin_transd_aux>0)
        {              
            //Final value > initial value 
            *direction_lin=1;
        }                
        else 
        {   
            //Final value < initial value 
            *direction_lin=0;
            lin_transd_aux*=-1.0;
        }      
        LTransd_Results.TotalTravel[i]=(unsigned int)lin_transd_aux;        
        #if 0                        
        COMM.remote_enable=REMOTE_ENABLE;    
        sprintf(OutputBuffer, "FirstValue[i]=%05d", lin_transd_aux1);
        SendOutputBuffer('B');
        sprintf(OutputBuffer, "LastValue[i]=%05d", lin_transd_aux2);
        SendOutputBuffer('B');         
        sprintf(OutputBuffer, "TotalTravel_float[i]=%f", lin_transd_aux);
        SendOutputBuffer('B');                             
        sprintf(OutputBuffer, "TotalTravel[i]=%05u", LTransd_Results.TotalTravel[i]);
        SendOutputBuffer('B');
        #endif
        *fin_val_lin++;      
        *direction_lin++;
    }                
    //Calculate total travel for digital transducers:
    for(i=0; i<NUMBER_OF_DT_CHS; i++)
    {
        //get last recorded value:    
        read_adc_res_sram_test(TESTPAR.test_duration-1); 
                        
        #if 0
        //Kompletan broj je 150000:  
        ADC_Results.DTH[i]=0b10;
        ADC_Results.DTL[i]=0b0100100111110000;
        #endif  

        dig_transd_aux1=convert_to_slong_DT_res(i);                           
        *fin_val_dig=dig_transd_aux1; 
        //get first recorded value:                                           
        read_adc_res_sram_test(0); 
              
        #if 0                              
        //Kompletan broj je 500000: 
        ADC_Results.DTH[i]=0b0111; //458752
        ADC_Results.DTL[i]=0b1010000100100000; //41248
        #endif
        
        dig_transd_aux2=convert_to_slong_DT_res(i); 
                
        dig_transd_aux=(float)dig_transd_aux1-(float)dig_transd_aux2;           
        if(dig_transd_aux>0)
        {              
            //Final value > initial value 
            *direction_dig=1;
        }                
        else 
        {   
            //Final value < initial value 
            *direction_dig=0;
        }                              
        //Paziti, kod longa mora ici labs!
        dig_transd_aux=fabs(dig_transd_aux);          
        DTransd_Results.TotalTravel[i]=(signed long)dig_transd_aux;
        #if 0                                      
        COMM.remote_enable=REMOTE_ENABLE;  
        SendOutputBuffer('B');                  
        sprintf(OutputBuffer, "FirstValue[i]=%f", (float)dig_transd_aux2);
        SendOutputBuffer('B');
        sprintf(OutputBuffer, "LastValue[i]=%f", (float)dig_transd_aux1);
        SendOutputBuffer('B');         
        sprintf(OutputBuffer, "TotalTravel_float[i]=%f", (float)dig_transd_aux);
        SendOutputBuffer('B');                             
        sprintf(OutputBuffer, "TotalTravel[i]=%f", (float)DTransd_Results.TotalTravel[i]);
        SendOutputBuffer('B');             
        #endif
        *fin_val_dig++;      
        *direction_dig++;
    }    
}

void calculate_overtavel(signed int fin_val_lin[], unsigned char direction_lin[], signed long fin_val_dig[], unsigned char direction_dig[], unsigned int address)
{
int i;
unsigned int res;
float res_aux;
signed long dig_transd_aux;
signed long dig_res;
    //Calculate overtavel for linear tranducers:
    for(i=0; i<NUMBER_OF_LT_CHS; i++)
    {                       
        if(direction_lin[i]==1)
        {
            if(ADC_Results.LT[i]>fin_val_lin[i])
            {                                                           
                //res=abs(fin_val_lin[i]-ADC_Results.LT[i]); 
                res_aux=(float)ADC_Results.LT[i]-(float)fin_val_lin[i];
                res_aux=fabs(res_aux);
                res=(unsigned int)res_aux;
                if(res>LTransd_Results.OverTravel[i])
                {                
                    LTransd_Results.OverTravel[i]=res;
                    LTransd_Results.OverTravelTime[i]=address;
                }            
            }       
        }  
        else
        {
            if(ADC_Results.LT[i]<fin_val_lin[i])
            {                                                           
                //res=abs(fin_val_lin[i]-ADC_Results.LT[i]);
                res_aux=(float)fin_val_lin[i]-(float)ADC_Results.LT[i];
                res_aux=fabs(res_aux);
                res=(unsigned int)res_aux;
                if(res>LTransd_Results.OverTravel[i])
                {                
                    LTransd_Results.OverTravel[i]=res;
                    LTransd_Results.OverTravelTime[i]=address;
                }            
            }              
        }              
    } 
    //Calculate overtavel for digital tranducers:
    for(i=0; i<NUMBER_OF_DT_CHS; i++)
    {             
        dig_transd_aux=convert_to_slong_DT_res(i);  
        
        if(dig_transd_aux<DTransd_Results.Min[i])
        {
            DTransd_Results.Min[i]=dig_transd_aux;   
        }
        if(dig_transd_aux>DTransd_Results.Max[i])
        {
            DTransd_Results.Max[i]=dig_transd_aux;   
        }
                 
        if(direction_dig[i]==1)
        {
            if(dig_transd_aux>fin_val_dig[i])
            {                                                           
                //dig_res=labs(fin_val_dig[i]-dig_transd_aux);
                res_aux=(float)dig_transd_aux-(float)fin_val_dig[i];
                res_aux=fabs(res_aux); 
                dig_res=(signed long)res_aux;                
                if(dig_res>DTransd_Results.OverTravel[i])
                {                
                    DTransd_Results.OverTravel[i]=dig_res;
                    DTransd_Results.OverTravelTime[i]=address;
                }            
            }       
        }  
        else
        {
            if(dig_transd_aux<fin_val_dig[i])
            {                                                           
                //dig_res=labs(fin_val_dig[i]-dig_transd_aux);
                res_aux=(float)fin_val_dig[i]-(float)dig_transd_aux;
                res_aux=fabs(res_aux); 
                dig_res=(signed long)res_aux; 
                if(dig_res>DTransd_Results.OverTravel[i])
                {                
                    DTransd_Results.OverTravel[i]=dig_res;
                    DTransd_Results.OverTravelTime[i]=address;
                }            
            }              
        }           
    }         
}

void calculate_rebound(signed int fin_val_lin[],  unsigned char direction_lin[], signed long fin_val_dig[],  unsigned char direction_dig[], unsigned int address)
{
int i;
unsigned int res; 
float res_aux;
signed long dig_transd_aux;
signed long dig_res;
    //Calculate rebound for linear transducers:
    for(i=0; i<NUMBER_OF_LT_CHS; i++)
    {            
        if(address>LTransd_Results.OverTravelTime[i])
        {
            if(direction_lin[i]==1)
            {
                if(ADC_Results.LT[i]<fin_val_lin[i])
                {                                                           
                    //res=abs(fin_val_lin[i]-ADC_Results.LT[i]);
                    res_aux=(float)fin_val_lin[i]-(float)ADC_Results.LT[i];
                    res_aux=fabs(res_aux);
                    res=(unsigned int)res_aux;
                    if(res>LTransd_Results.Rebound[i])
                    {                
                        LTransd_Results.Rebound[i]=res;
                        LTransd_Results.ReboundTime[i]=address;
                    }            
                }       
            }    
            else
            {            
                if(ADC_Results.LT[i]>fin_val_lin[i])
                {                                                               
                    //res=abs(fin_val_lin[i]-ADC_Results.LT[i]); 
                    res_aux=(float)ADC_Results.LT[i]-(float)fin_val_lin[i];
                    res_aux=fabs(res_aux);
                    res=(unsigned int)res_aux;                    
                    if(res>LTransd_Results.Rebound[i])
                    {                
                        LTransd_Results.Rebound[i]=res;
                        LTransd_Results.ReboundTime[i]=address;
                    }            
                }  
            }
        }
    } 
    //Calculate rebound for digital transducers
    for(i=0; i<NUMBER_OF_DT_CHS; i++)
    {                                 
    dig_transd_aux=convert_to_slong_DT_res(i);           
    
        if(address>DTransd_Results.OverTravelTime[i])
        {
            if(direction_dig[i]==1)
            {
                if(dig_transd_aux<fin_val_dig[i])
                {                                                           
                    //dig_res=labs(fin_val_dig[i]-dig_transd_aux);
                    res_aux=(float)fin_val_dig[i]-(float)dig_transd_aux;
                    res_aux=fabs(res_aux); 
                    dig_res=(signed long)res_aux; 
                    if(dig_res>DTransd_Results.Rebound[i])
                    {                
                        DTransd_Results.Rebound[i]=dig_res;
                        DTransd_Results.ReboundTime[i]=address;
                    }            
                }       
            }    
            else
            {            
                if(dig_transd_aux>fin_val_dig[i])
                {                                                               
                    //dig_res=labs(fin_val_dig[i]-dig_transd_aux);
                    res_aux=(float)fin_val_dig[i]-(float)dig_transd_aux;
                    res_aux=fabs(res_aux); 
                    dig_res=(signed long)res_aux; 
                    if(dig_res>DTransd_Results.Rebound[i])
                    {                
                        DTransd_Results.Rebound[i]=dig_res;
                        DTransd_Results.ReboundTime[i]=address;
                    }            
                }  
            }
        }
    }
}  

void set_contact_wipe_results_to_zero()
{
    if(TESTPAR.break_units==ONE_BREAK_UNIT_PER_PHASE)
    {
        //DTransd_Results.ContactWipe[0][0]=0;
        DTransd_Results.ContactWipe[0][1]=0;
        DTransd_Results.ContactWipe[1][0]=0;
        DTransd_Results.ContactWipe[1][1]=0;
        //DTransd_Results.ContactWipe[2][0]=0;
        DTransd_Results.ContactWipe[2][1]=0;
        DTransd_Results.ContactWipe[3][0]=0;
        DTransd_Results.ContactWipe[3][1]=0;
        //DTransd_Results.ContactWipe[4][0]=0;
        DTransd_Results.ContactWipe[4][1]=0;
        DTransd_Results.ContactWipe[5][0]=0;
        DTransd_Results.ContactWipe[5][1]=0; 
        
        //LTransd_Results.ContactWipe[0][0]=0;
        LTransd_Results.ContactWipe[0][1]=0;
        LTransd_Results.ContactWipe[1][0]=0;
        LTransd_Results.ContactWipe[1][1]=0;
        //LTransd_Results.ContactWipe[2][0]=0;
        LTransd_Results.ContactWipe[2][1]=0;
        LTransd_Results.ContactWipe[3][0]=0;
        LTransd_Results.ContactWipe[3][1]=0;
        //LTransd_Results.ContactWipe[4][0]=0;
        LTransd_Results.ContactWipe[4][1]=0;
        LTransd_Results.ContactWipe[5][0]=0;
        LTransd_Results.ContactWipe[5][1]=0;                                                 
    }
    if(TESTPAR.break_units==TWO_BREAK_UNITS_PER_PHASE)
    {
        //DTransd_Results.ContactWipe[0][0]=0;
        //DTransd_Results.ContactWipe[0][1]=0;
        DTransd_Results.ContactWipe[1][0]=0;
        DTransd_Results.ContactWipe[1][1]=0;
        //DTransd_Results.ContactWipe[2][0]=0;
        //DTransd_Results.ContactWipe[2][1]=0;
        DTransd_Results.ContactWipe[3][0]=0;
        DTransd_Results.ContactWipe[3][1]=0;
        //DTransd_Results.ContactWipe[4][0]=0;
        //DTransd_Results.ContactWipe[4][1]=0;
        DTransd_Results.ContactWipe[5][0]=0;
        DTransd_Results.ContactWipe[5][1]=0; 
        
        //LTransd_Results.ContactWipe[0][0]=0;
        //LTransd_Results.ContactWipe[0][1]=0;
        LTransd_Results.ContactWipe[1][0]=0;
        LTransd_Results.ContactWipe[1][1]=0;
        //LTransd_Results.ContactWipe[2][0]=0;
        //LTransd_Results.ContactWipe[2][1]=0;
        LTransd_Results.ContactWipe[3][0]=0;
        LTransd_Results.ContactWipe[3][1]=0;
        //LTransd_Results.ContactWipe[4][0]=0;
        //LTransd_Results.ContactWipe[4][1]=0;
        LTransd_Results.ContactWipe[5][0]=0;
        LTransd_Results.ContactWipe[5][1]=0;            
    }           
}    

unsigned int process_data_for_closing_test(void)
{
    unsigned int retVal=MAIN_OK;
    unsigned int address; 
    unsigned int i, j;
    unsigned int closing_time[NUMBER_OF_MAIN_CONTACTS];
    unsigned int preinsert_time[NUMBER_OF_MAIN_CONTACTS];
    unsigned int aux_time[NUMBER_OF_AUX_CHS];
    unsigned int preinsert_res[NUMBER_OF_MAIN_CONTACTS];    
    unsigned char search_closing_time[NUMBER_OF_MAIN_CONTACTS];
    unsigned char search_preinsert_time[NUMBER_OF_MAIN_CONTACTS]; 
    unsigned char search_aux_time[NUMBER_OF_AUX_CHS];
    unsigned char aux_int_state[NUMBER_OF_AUX_CHS];   
    
    signed int LT_final_value[NUMBER_OF_LT_CHS];
    unsigned char LT_direction[NUMBER_OF_LT_CHS];   
    signed long DT_final_value[NUMBER_OF_DT_CHS];
    unsigned char DT_direction[NUMBER_OF_DT_CHS]; 
    
    //contact wipe temp variables:  
    unsigned int lt_cw_first_value;
    unsigned int lt_cw_last_value;
    signed long  dt_cw_first_value;
    signed long  dt_cw_last_value;
    
    float aux_variable;                     
                  
    unsigned char aux_ch1_byte;   
    
    init_results_vaules();      
                   
    for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
    {
        closing_time[i]=0;
        preinsert_time[i]=0;
        preinsert_res[i]=0;
        search_closing_time[i]=1;
        search_preinsert_time[i]=1;                 
    }                     
    
    //Set initial aux states:
    read_adc_res_sram_test(1);    
        
    #if 0
    //Testiram aux kanal: 
    ADC_Results.AUX=0b10000100;    
    #endif                       
      
    aux_ch1_byte=0x01;//0b00000001; 
    
    for(i=0; i<NUMBER_OF_AUX_CHS; i++)
    {              
         aux_int_state[i]=((aux_ch1_byte<<i)&((unsigned char)ADC_Results.AUX));
         search_aux_time[i]=1; 
         aux_time[i]=0;
    }                                                   
                                                 
    //Check if test was successiful for each phase:
    
    check_mc_test_results(search_closing_time);  
    
    for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
    {
        if(search_closing_time[i]==0)
        {
            search_preinsert_time[i]=0;
        }
    }   
    
    //Caluclate total travel for linear and digital transducer:
    calculate_total_travel(&LT_final_value[0], &LT_direction[0], &DT_final_value[0], &DT_direction[0]); 
      
    #if 0 
    sprintf(OutputBuffer, "DTFV: %f", (float)DT_final_value[0]);
    SendOutputBuffer('A');
    sprintf(OutputBuffer, "DTOT: %f", (float)DTransd_Results.TotalTravel[0]);
    SendOutputBuffer('A');         
    sprintf(OutputBuffer, "DTDIR: %u", DT_direction[0]);
    SendOutputBuffer('A');
    #endif
                                                     
    //Read values from SRAM and get results:
    
    for(address=1; address<(TESTPAR.test_duration); address++)
    {
      
    read_adc_res_sram_test(address);                      

        for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
        {                
            if(search_closing_time[i]==1)
            {
                if(ADC_Results.MC[i]>MAIN_THRESHOLD_HIGH)
                {
                    search_closing_time[i]=0;
                    search_preinsert_time[i]=0;
                    closing_time[i]=address;                                        
            }     
                                     
            if(search_preinsert_time[i]==1)
            {
                if(ADC_Results.MC[i]>MAIN_THRESHOLD_LOW)
                {
                    preinsert_time[i]=address;
                    search_preinsert_time[i]=0;
                }       
            }                                     
            }                       
        }                                  
        
        #if 0
        //Testiram aux kanal:
        //Cini se da i ovo radi kako treba!
        if(address==1111)
        { 
					ADC_Results.AUX=0x04;//0b00000100;
        }    
        else if(address==2222)
        {
            ADC_Results.AUX=0x00;//0b00000000;
        }    
        else
        {
            ADC_Results.AUX=0x84;//0b10000100;
        }
        #endif
        
        aux_ch1_byte=0x01;//0b00000001; 
        
        for(i=0; i<NUMBER_OF_AUX_CHS; i++)
        {         
             if(search_aux_time[i]==1)
             {              
                if(((aux_ch1_byte<<i)&((unsigned char)ADC_Results.AUX))!=aux_int_state[i])                
                {                    
                    #if 0
                    sprintf(OutputBuffer, "NASAO:kanal:%02u_address:%05u", i, address);
                    COMM.remote_enable=REMOTE_ENABLE;                    
                    SendOutputBuffer('B');
                    COMM.remote_enable=REMOTE_DISABLE;
                    #endif                     
                    search_aux_time[i]=0;
                    aux_time[i]=address;
                }  
             }
        }                    
        
        //Cini mi se da dobro racuna:                                                               
        search_max_current_per_ch(address);                                                                                  
        search_min_max_voltage_per_ch(address); 
         
        #if 0 
        //Testiram proracun overtavela i rebounda:
        //Overtravel za DT = 30000   
        //Rebound za DT=20000        
        if(address==20)
        {
            ///DT=15000;
            ADC_Results.DTH[0]=150000/0xFFFF;
            ADC_Results.DTL[0]=150000&0xFFFF;                                            
        }        
        else if (address==30)
        {                                  
            ADC_Results.DTH[0]=100000/0xFFFF;
            ADC_Results.DTL[0]=100000&0xFFFF;
        }
        else
        {            
            ///DT=12000;
            ADC_Results.DTH[0]=120000/0xFFFF;
            ADC_Results.DTL[0]=120000&0xFFFF;    
            DT_final_value[0]=120000; 
            DT_direction[0]=1;          
            
        }    
        #endif 
        
        #if 0
        //Testiram proracun overtavela i rebounda:
        //Overtravel za Linearni transducer = 6000
        //Rebound za Linearni transducer = 7000  
        if(address==10)
        {
            ADC_Results.LT[0]=18000;                         
        }        
        else if (address==50)
        {                                  
            ADC_Results.LT[0]=5000;
        }
        else
        {            
    
            ADC_Results.LT[0]=12000;
            LT_final_value[0]=12000; 
            LT_direction[0]=1;                          
        }    
        #endif                           
        
        //Caluclate linear transducer overtavel:
        calculate_overtavel(LT_final_value, LT_direction, DT_final_value, DT_direction, address);            
    
    //Chek if stop button is pressed:    
    if(((retVal=check_for_stop_button())!=MAIN_OK)){return retVal;}    
                                          
    }                                       
    
    //Caluclate rebounds:
    for(address=1; address<(TESTPAR.test_duration); address++)
    {                                                                   
        //Read SRAM values:
        read_adc_res_sram_test(address); 
        
        #if 0 
        //Testiram proracun overtavela i rebounda:
        //Overtravel za DT = 30000   
        //Rebound za DT=20000        
        if(address==20)
        {
            ///DT=15000;
            ADC_Results.DTH[0]=150000/0xFFFF;
            ADC_Results.DTL[0]=150000&0xFFFF;                                            
        }        
        else if (address==30)
        {                                  
            ADC_Results.DTH[0]=100000/0xFFFF;
            ADC_Results.DTL[0]=100000&0xFFFF;
        }
        else
        {            
            ///DT=12000;
            ADC_Results.DTH[0]=120000/0xFFFF;
            ADC_Results.DTL[0]=120000&0xFFFF;    
            DT_final_value[0]=120000; 
            DT_direction[0]=1;          
            
        }    
        #endif 
        
        #if 0 
        //Testiram proracun overtavela i rebounda:
        //Overtravel za Linearni transducer = 6000
        //Rebound za Linearni transducer = 7000  
        if(address==10)
        {
            ADC_Results.LT[0]=18000;                         
        }        
        else if (address==50)
        {                                  
            ADC_Results.LT[0]=5000;
        }
        else
        {            
    
            ADC_Results.LT[0]=12000;
            LT_final_value[0]=12000; 
            LT_direction[0]=1;                          
        }    
        #endif                 
        
        //Caluclate linear transducer rebound:
        calculate_rebound(LT_final_value, LT_direction, DT_final_value, DT_direction, address);       
    
    //Chek if stop button is pressed:    
    if(((retVal=check_for_stop_button())!=MAIN_OK)){return retVal;}     
    }               
    
    //Calculate preinsertion resistance:
    for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
    {
        if(preinsert_time[i]!=0)
        {
            address=preinsert_time[i]/2;
            address+=closing_time[i]/2;
            read_adc_res_sram_test(address);
            preinsert_res[i]=ADC_Results.MC[i];    
        }
    }
    
    //Store obtained results to global variables:
    j=0;
    for(i=0; i<6; i++)
    {
        MainC_Results.ClosingTime[i][0]=closing_time[j];         
        if((closing_time[j]<(preinsert_time[j]+50))||(preinsert_res[j]<2272)||(preinsert_res[j]>60493)){preinsert_time[j]=0; preinsert_res[j]=0;} //Ogranicenje tako da je Resistor ON time < 5ms onda na 0; Ako je R<10E ili R>10k na 0           
        MainC_Results.PreinsertionTime[i][0]=preinsert_time[j];
        MainC_Results.PreinsertionResistance[i][0]=preinsert_res[j];
        j++;
        MainC_Results.ClosingTime[i][1]=closing_time[j];
        if((closing_time[j]<(preinsert_time[j]+50))||(preinsert_res[j]<2272)||(preinsert_res[j]>60493)){preinsert_time[j]=0; preinsert_res[j]=0;} //Ogranicenje tako da je Resistor ON time < 5ms onda na 0; Ako je R<10E ili R>10k na 0           
        MainC_Results.PreinsertionTime[i][1]=preinsert_time[j];
        MainC_Results.PreinsertionResistance[i][1]=preinsert_res[j];
        j++;
    } 
    
    //auxiliary results:
    AUX_Results.AUX1Time=aux_time[0];
    AUX_Results.AUX2Time=aux_time[1];
    AUX_Results.AUX3Time=aux_time[2];
    AUX_Results.AUX4Time=aux_time[3];
    AUX_Results.AUX5Time=aux_time[4];
    AUX_Results.AUX6Time=aux_time[5];
      
    //contact wipes, analog motion transducer:
	for(i=0; i<6; i++)
	{
		for(j=0; j<2; j++)
		{				
            if((i==0)||(i==1))
            {
                read_adc_res_sram_test(TESTPAR.test_duration-1);
			    lt_cw_first_value=ADC_Results.LT[0];
			    read_adc_res_sram_test(MainC_Results.ClosingTime[i][j]);
                lt_cw_last_value=ADC_Results.LT[0];                
            }
            else if((i==2)||(i==3))
            {                                 
                read_adc_res_sram_test(TESTPAR.test_duration-1);
			    lt_cw_first_value=ADC_Results.LT[1];
                read_adc_res_sram_test(MainC_Results.ClosingTime[i][j]);
    			lt_cw_last_value=ADC_Results.LT[1];            
            }
            else
            {                               
                read_adc_res_sram_test(TESTPAR.test_duration-1);
			    lt_cw_first_value=ADC_Results.LT[2];
                read_adc_res_sram_test(MainC_Results.ClosingTime[i][j]);
    			lt_cw_last_value=ADC_Results.LT[2];            
            }	
            		                       
            if(lt_cw_last_value>lt_cw_first_value){LTransd_Results.ContactWipe[i][j]=lt_cw_last_value-lt_cw_first_value;}
			else			                      {LTransd_Results.ContactWipe[i][j]=lt_cw_first_value-lt_cw_last_value;}
		}               
	}
    

    //contact wipes, digital motion transducer:
	for(i=0; i<6; i++)
	{
		for(j=0; j<2; j++)
		{				
            if((i==0)||(i==1))
            {
                read_adc_res_sram_test(TESTPAR.test_duration-1);
			    dt_cw_first_value=convert_to_slong_DT_res(0);
			    read_adc_res_sram_test(MainC_Results.ClosingTime[i][j]);
                dt_cw_last_value=convert_to_slong_DT_res(0);               
            }
            else if((i==2)||(i==3))
            {                                 
                read_adc_res_sram_test(TESTPAR.test_duration-1);
			    dt_cw_first_value=convert_to_slong_DT_res(1);
                read_adc_res_sram_test(MainC_Results.ClosingTime[i][j]);
    			dt_cw_last_value=convert_to_slong_DT_res(1);           
            }
            else
            {                               
                read_adc_res_sram_test(TESTPAR.test_duration-1);
			    dt_cw_first_value=convert_to_slong_DT_res(2);
                read_adc_res_sram_test(MainC_Results.ClosingTime[i][j]);
    			dt_cw_last_value=convert_to_slong_DT_res(2);         
            }	
            
            aux_variable=(float)dt_cw_last_value-(float)dt_cw_first_value;
            aux_variable=fabs(aux_variable); 
            DTransd_Results.ContactWipe[i][j]=(signed long)aux_variable; 
            		                       
            //if(dt_cw_last_value>dt_cw_first_value){DTransd_Results.ContactWipe[i][j]=dt_cw_last_value-dt_cw_first_value;}
			//else			                      {DTransd_Results.ContactWipe[i][j]=dt_cw_first_value-dt_cw_last_value;}
		}
	}  
    
    //set contact wipes to zero in case that the closing time was not found:
    for(i=0; i<6; i++)    
    {
        for(j=0; j<2; j++)        
        {
            if(MainC_Results.ClosingTime[i][j]==0){LTransd_Results.ContactWipe[i][j]=0; DTransd_Results.ContactWipe[i][j]=0;}
        }
    }   
    
    set_contact_wipe_results_to_zero();
    
return retVal;
}

char process_data_for_opening_test(void)
{
    unsigned int retVal=MAIN_OK;
    unsigned int address;
    unsigned int i, j;
    unsigned int opening_time[NUMBER_OF_MAIN_CONTACTS];
    unsigned int aux_time[NUMBER_OF_AUX_CHS];
    unsigned char search_opening_time[NUMBER_OF_MAIN_CONTACTS];
    unsigned char search_aux_time[NUMBER_OF_AUX_CHS];
    unsigned char aux_int_state[NUMBER_OF_AUX_CHS];   
    
    signed int LT_final_value[NUMBER_OF_LT_CHS];
    unsigned char LT_direction[NUMBER_OF_LT_CHS];   
    signed long DT_final_value[NUMBER_OF_DT_CHS];
    unsigned char DT_direction[NUMBER_OF_DT_CHS];             
    
    //contact wipe temp variables:
    unsigned int lt_cw_first_value;
    unsigned int lt_cw_last_value;
    signed long dt_cw_first_value;
    signed long dt_cw_last_value;
    
    float aux_variable;         
     
    unsigned char aux_ch1_byte;   
    
    init_results_vaules();      
                   
    for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
    {
        opening_time[i]=0;
        search_opening_time[i]=1;
    }             
    
    //Set initial aux states:
    read_adc_res_sram_test(1);                           
      
    aux_ch1_byte=0x01; //0b00000001; 
    
    for(i=0; i<NUMBER_OF_AUX_CHS; i++)
    {              
         aux_int_state[i]=((aux_ch1_byte<<i)&((unsigned char)ADC_Results.AUX));
         search_aux_time[i]=1;
         aux_time[i]=0;
    }                                                   
                                                 
    //Check if test was successiful for each phase:
    
    check_mc_test_results(search_opening_time);        
    
    //Caluclate total travel for linear and digital transducer:
    calculate_total_travel(&LT_final_value[0], &LT_direction[0], &DT_final_value[0], &DT_direction[0]); 
                                                     
    //Read values from SRAM and get results:
                            
    //find opening times:
    for(address=(TESTPAR.test_duration-1); address>0; address--)
    {
    read_adc_res_sram_test(address);                      

        for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
        {                
            if(search_opening_time[i]==1)
            {
                if(ADC_Results.MC[i]>MAIN_THRESHOLD_HIGH)
                {
                    search_opening_time[i]=0;
                    opening_time[i]=address;                                        
                }                                                                              
            }                       
        } 
        //Chek if stop button is pressed:    
        if(((retVal=check_for_stop_button())!=MAIN_OK)){return retVal;}                                                                                          
    }                                                         
                         
    //find aux times and transducer results:
    for(address=1; address<TESTPAR.test_duration; address++)
    {        
        //Read SRAM values:PB    
        read_adc_res_sram_test(address);                      
                                
        aux_ch1_byte=0x01; //0b00000001;        
        
        for(i=0; i<NUMBER_OF_AUX_CHS; i++)
        {         
             if(search_aux_time[i]==1)
             {                                                          
                if(((aux_ch1_byte<<i)&((unsigned char)ADC_Results.AUX))!=aux_int_state[i])                
                {                         
                    #if 0
                    sprintf(OutputBuffer, "NASAO:kanal:%02u_address:%05u", i, address);
                    COMM.remote_enable=REMOTE_ENABLE;                    
                    SendOutputBuffer('B');
                    COMM.remote_enable=REMOTE_DISABLE;
                    #endif
                    search_aux_time[i]=0;
                    aux_time[i]=address;
                }  
             }
        }                    
        
        //Cini mi se da dobro racuna:                                                       
        search_max_current_per_ch(address);                                                                                  
        search_min_max_voltage_per_ch(address); 
                                              
        //Caluclate linear transducer overtavel:
        calculate_overtavel(LT_final_value, LT_direction, DT_final_value, DT_direction, address);
        
        //Chek if stop button is pressed:    
        if(((retVal=check_for_stop_button())!=MAIN_OK)){return retVal;}                                       
    }
    
    //Caluclate rebounds:
    for(address=1; address<(TESTPAR.test_duration); address++)
    {                                            
        //Read SRAM values:
        read_adc_res_sram_test(address);                               
        
        //Caluclate linear transducer rebound:
        calculate_rebound(LT_final_value, LT_direction, DT_final_value, DT_direction, address);     
        
        //Chek if stop button is pressed:    
        if(((retVal=check_for_stop_button())!=MAIN_OK)){return retVal;}    
    }               
            
    //Store obtained results into the global variables:
    j=0;
    for(i=0; i<6; i++)
    {
        MainC_Results.FirstOpeningTime[i][0]=opening_time[j]; 
        j++;
        MainC_Results.FirstOpeningTime[i][1]=opening_time[j];
        j++;
    }  
    
    //auxiliary results:    
    AUX_Results.AUX1Time=aux_time[0];
    AUX_Results.AUX2Time=aux_time[1];
    AUX_Results.AUX3Time=aux_time[2];
    AUX_Results.AUX4Time=aux_time[3];
    AUX_Results.AUX5Time=aux_time[4];
    AUX_Results.AUX6Time=aux_time[5];
    
    //contact wipes, analog motion transducer:
	for(i=0; i<6; i++)
	{
		for(j=0; j<2; j++)
		{				
            if((i==0)||(i==1))
            {
                read_adc_res_sram_test(1);
			    lt_cw_first_value=ADC_Results.LT[0];
			    read_adc_res_sram_test(MainC_Results.FirstOpeningTime[i][j]);
                lt_cw_last_value=ADC_Results.LT[0];                
            }
            else if((i==2)||(i==3))
            {                                 
                read_adc_res_sram_test(1);
			    lt_cw_first_value=ADC_Results.LT[1];
                read_adc_res_sram_test(MainC_Results.FirstOpeningTime[i][j]);
    			lt_cw_last_value=ADC_Results.LT[1];            
            }
            else
            {                               
                read_adc_res_sram_test(1);
			    lt_cw_first_value=ADC_Results.LT[2];
                read_adc_res_sram_test(MainC_Results.FirstOpeningTime[i][j]);
    			lt_cw_last_value=ADC_Results.LT[2];            
            }	
            		                       
            if(lt_cw_last_value>lt_cw_first_value){LTransd_Results.ContactWipe[i][j]=lt_cw_last_value-lt_cw_first_value;}
			else			                      {LTransd_Results.ContactWipe[i][j]=lt_cw_first_value-lt_cw_last_value;}
		}
	}
    
    //contact wipes, digital motion transducer:
	for(i=0; i<6; i++)
	{
		for(j=0; j<2; j++)
		{				
            if((i==0)||(i==1))
            {
                read_adc_res_sram_test(1);
			    dt_cw_first_value=convert_to_slong_DT_res(0);
			    read_adc_res_sram_test(MainC_Results.FirstOpeningTime[i][j]);
                dt_cw_last_value=convert_to_slong_DT_res(0);               
            }
            else if((i==2)||(i==3))
            {                                 
                read_adc_res_sram_test(1);
			    dt_cw_first_value=convert_to_slong_DT_res(1);
                read_adc_res_sram_test(MainC_Results.FirstOpeningTime[i][j]);
    			dt_cw_last_value=convert_to_slong_DT_res(1);           
            }
            else
            {                               
                read_adc_res_sram_test(1);
			    dt_cw_first_value=convert_to_slong_DT_res(2);
                read_adc_res_sram_test(MainC_Results.FirstOpeningTime[i][j]);
    			dt_cw_last_value=convert_to_slong_DT_res(2);         
            }	
            
            
            aux_variable=(float)dt_cw_last_value-(float)dt_cw_first_value;
            aux_variable=fabs(aux_variable); 
            DTransd_Results.ContactWipe[i][j]=(signed long)aux_variable; 		                       
            //if(dt_cw_last_value>dt_cw_first_value){DTransd_Results.ContactWipe[i][j]=dt_cw_last_value-dt_cw_first_value;}
			//else			                      {DTransd_Results.ContactWipe[i][j]=dt_cw_first_value-dt_cw_last_value;}
		}
	}             
        
    //set contact wipes to zero in case that the first opening time was not found:
    for(i=0; i<6; i++)    
    {
        for(j=0; j<2; j++)        
        {
            if(MainC_Results.FirstOpeningTime[i][j]==0){LTransd_Results.ContactWipe[i][j]=0; DTransd_Results.ContactWipe[i][j]=0;}
        }
    }
    
    set_contact_wipe_results_to_zero();          
    
return retVal;           
}

unsigned int process_data_for_CO_test(void)
{
    unsigned int retVal=MAIN_OK;
    unsigned int address;
    unsigned int i, j;
    unsigned int closing_time[NUMBER_OF_MAIN_CONTACTS];
    unsigned char search_closing_time[NUMBER_OF_MAIN_CONTACTS];
    unsigned int opening_time[NUMBER_OF_MAIN_CONTACTS];    
    unsigned char search_opening_time[NUMBER_OF_MAIN_CONTACTS];        
        
    init_results_vaules();      
                   
    for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
    {
        closing_time[i]=0;
        search_closing_time[i]=1;
        opening_time[i]=0;
        search_opening_time[i]=1;        
    }                
                                     
    //Check if test was successiful for each phase:    
    check_mc_test_results(search_closing_time);   
    check_mc_test_results(search_opening_time);    
    //////////////////////////////////////////////   
                                                         
    //Read values from SRAM and get results:
    
    for(address=0; address<(TESTPAR.test_duration); address++)
    {
    read_adc_res_sram_test(address);                      

        for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
        {                
            if(search_closing_time[i]==1)
            {
                if(ADC_Results.MC[i]>MAIN_THRESHOLD_HIGH)
                {
                    search_closing_time[i]=0;
                    closing_time[i]=address;                                        
            }                                                                              
            }                       
        }                                                             
        
        //Cini mi se da dobro racuna:                                                       
        search_max_current_per_ch(address);                                                                                  
        search_min_max_voltage_per_ch(address);
        
        //Chek if stop button is pressed:    
        if(((retVal=check_for_stop_button())!=MAIN_OK)){return retVal;}                                                          
    }          
    
    //find opening times:
    for(address=(TESTPAR.test_duration-1); address>0; address--)
    {
    read_adc_res_sram_test(address);                      

        for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
        {                
            if(search_opening_time[i]==1)
            {
                if(ADC_Results.MC[i]>MAIN_THRESHOLD_HIGH)
                {
                    search_opening_time[i]=0;
                    opening_time[i]=address;                                        
                }                                                                              
            }                       
        } 
        //Chek if stop button is pressed:    
        if(((retVal=check_for_stop_button())!=MAIN_OK)){return retVal;}                                                                                                  
    }                                   
        
    //Store obtained results to global variables:
    j=0;
    for(i=0; i<NUMBER_OF_MAIN_CONTACTS/2; i++)
    {
        MainC_Results.ClosingTime[i][0]=closing_time[j];
        MainC_Results.FirstOpeningTime[i][0]=opening_time[j];  
        j++;
        MainC_Results.ClosingTime[i][1]=closing_time[j];  
        MainC_Results.FirstOpeningTime[i][1]=opening_time[j];  
        j++;
    }
return retVal;                  
}

unsigned int process_data_for_OC_test(void)
{
    unsigned int retVal=MAIN_OK;
    unsigned int address;
    unsigned int i, j;
    unsigned int closing_time[NUMBER_OF_MAIN_CONTACTS];
    unsigned char search_closing_time[NUMBER_OF_MAIN_CONTACTS];
    unsigned int opening_time[NUMBER_OF_MAIN_CONTACTS];    
    unsigned char search_opening_time[NUMBER_OF_MAIN_CONTACTS];   
    //aux variables:                   
    unsigned int search_first_low_value[NUMBER_OF_MAIN_CONTACTS];
    unsigned int first_low_value_address[NUMBER_OF_MAIN_CONTACTS];
    unsigned int search_last_low_value[NUMBER_OF_MAIN_CONTACTS];
    unsigned int last_low_value_address[NUMBER_OF_MAIN_CONTACTS];
    unsigned int mid_low_value_address[NUMBER_OF_MAIN_CONTACTS];         
    
    unsigned char aux_ch1_byte;   
    
    init_results_vaules();      
                   
    for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
    {
        closing_time[i]=0;
        search_closing_time[i]=1;
        opening_time[i]=0;
        search_opening_time[i]=1;  
        
        search_first_low_value[i]=1;  
        search_last_low_value[i]=1;   
        first_low_value_address[i]=0; 
        last_low_value_address[i]=0;
        mid_low_value_address[i]=0;
    }                
                                     
    //Check if test was successiful for each phase:    
    check_mc_test_results(search_closing_time);   
    check_mc_test_results(search_opening_time);    
    //////////////////////////////////////////////   
                                                         
    //Read values from SRAM and get results:                  
    {
        for(address=0; address<(TESTPAR.test_duration); address++)
        {
            read_adc_res_sram_test(address);                                  
            for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
            {                                                     
                if((search_closing_time[i])&&(search_opening_time[i]))
                {
                if(ADC_Results.MC[i]<MAIN_THRESHOLD_LOW)
                {                  
                    if(search_first_low_value[i])
                    {
                        first_low_value_address[i]=address;
                        search_first_low_value[i]=0;
                    }
                }
                }
            }                      
        }
        for(address=(TESTPAR.test_duration-1); address>0; address--)
        {
            read_adc_res_sram_test(address);
            for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
            {                                       
                if((search_closing_time[i])&&(search_opening_time[i]))
                {
                if(ADC_Results.MC[i]<MAIN_THRESHOLD_LOW)
                {                  
                    if(search_last_low_value[i])
                    {
                        last_low_value_address[i]=address;
                        search_last_low_value[i]=0;
                    }
                }
                }
            }                      
        } 
        for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
        {   
            if((search_first_low_value[i]==0)&&(search_last_low_value[i]==0))
            {
                if(last_low_value_address[i]>first_low_value_address[i])
                {
                    mid_low_value_address[i]=last_low_value_address[i]/2;
                    mid_low_value_address[i]+=first_low_value_address[i]/2;
                }        
            }
        }
    }   
                
    //find opening times for each channel:
    for(address=(TESTPAR.test_duration-1); address>0; address--)
    {
    read_adc_res_sram_test(address);                      

        for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
        {                
            if(search_opening_time[i]==1)
            {      
                if(address<mid_low_value_address[i])
                {
                    if(ADC_Results.MC[i]>MAIN_THRESHOLD_HIGH)
                    {
                        search_opening_time[i]=0;
                        opening_time[i]=address;                                        
                    }
                }                                                                              
            }                       
        } 
        //Chek if stop button is pressed:    
        if(((retVal=check_for_stop_button())!=MAIN_OK)){return retVal;}                                                                                              
    }   
    
    for(address=0; address<(TESTPAR.test_duration); address++)
    {
    read_adc_res_sram_test(address);                      

        for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
        {                
            if(search_closing_time[i]==1)
            {       
                if(address>mid_low_value_address[i])
                {                                
                    if(ADC_Results.MC[i]>MAIN_THRESHOLD_HIGH)
                    {
                        search_closing_time[i]=0;
                        closing_time[i]=address;                                        
                    }
                }                                                                              
            }                       
        }                                                             
        
        //Cini mi se da dobro racuna:                                                       
        search_max_current_per_ch(address);                                                                                  
        search_min_max_voltage_per_ch(address);
        
        //Chek if stop button is pressed:    
        if(((retVal=check_for_stop_button())!=MAIN_OK)){return retVal;}                                                            
    }   
                
    //Store obtained results to global variables:
    j=0;
    for(i=0; i<6; i++)
    {
        MainC_Results.ClosingTime[i][0]=closing_time[j]; 
        MainC_Results.FirstOpeningTime[i][0]=opening_time[j];  
        j++;
        MainC_Results.ClosingTime[i][1]=closing_time[j];
        MainC_Results.FirstOpeningTime[i][1]=opening_time[j];
        j++;
    }  
    
return retVal;                
}

unsigned int process_data_for_OCO_test(void)
{
    unsigned int retVal=MAIN_OK;
    unsigned int address;                                              
    unsigned int i, j;  
    //Second O test variables (second opening time is searched as in O test):
    unsigned int second_opening_time[NUMBER_OF_MAIN_CONTACTS];    
    unsigned char search_second_opening_time[NUMBER_OF_MAIN_CONTACTS];    
    //OC test variables:
    unsigned int closing_time[NUMBER_OF_MAIN_CONTACTS];
    unsigned char search_closing_time[NUMBER_OF_MAIN_CONTACTS];
    unsigned int opening_time[NUMBER_OF_MAIN_CONTACTS];    
    unsigned char search_opening_time[NUMBER_OF_MAIN_CONTACTS];   
    //aux variables:                   
    unsigned int search_first_low_value[NUMBER_OF_MAIN_CONTACTS];
    unsigned int first_low_value_address[NUMBER_OF_MAIN_CONTACTS];
    unsigned int search_last_low_value[NUMBER_OF_MAIN_CONTACTS];
    unsigned int last_low_value_address[NUMBER_OF_MAIN_CONTACTS];
    unsigned int mid_low_value_address[NUMBER_OF_MAIN_CONTACTS];   
    
    unsigned int distance_from_last_opening_time=10;      
        
    init_results_vaules();      
                   
    for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
    {        
        closing_time[i]=0;
        search_closing_time[i]=1;
        opening_time[i]=0;
        search_opening_time[i]=1;
        second_opening_time[i]=0;
        search_second_opening_time[i]=1;  
        
        search_first_low_value[i]=1;  
        search_last_low_value[i]=1;   
        first_low_value_address[i]=0; 
        last_low_value_address[i]=0;
        mid_low_value_address[i]=0;
    }                
                                     
    //Check if test was successiful for each phase:    
    check_mc_test_results(search_closing_time);   
    check_mc_test_results(search_opening_time);
    check_mc_test_results(search_second_opening_time);    
    //////////////////////////////////////////////
    
    //find second opening times:
    for(address=(TESTPAR.test_duration-1); address>0; address--)
    {
    read_adc_res_sram_test(address);                      

        for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
        {                
            if(search_second_opening_time[i]==1)
            {
                if(ADC_Results.MC[i]>MAIN_THRESHOLD_HIGH)
                {
                    search_second_opening_time[i]=0;
                    second_opening_time[i]=address;                                        
                }                                                                              
            }                       
        } 
        //Chek if stop button is pressed:    
        if(((retVal=check_for_stop_button())!=MAIN_OK)){return retVal;}                                                                                                       
    }   
    
    //find first opening times and closing times:                                                         
    //Read values from SRAM and get results:                  
    {
        for(address=0; address<(TESTPAR.test_duration); address++)
        {
            read_adc_res_sram_test(address);                                  
            for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
            {                                                     
                if((search_closing_time[i])&&(search_opening_time[i])&&(address<second_opening_time[i]-distance_from_last_opening_time))
                {
                if(ADC_Results.MC[i]<MAIN_THRESHOLD_LOW)
                {                  
                    if(search_first_low_value[i])
                    {
                        first_low_value_address[i]=address;
                        search_first_low_value[i]=0;
                    }
                }
                }
            }  
            //Chek if stop button is pressed:    
            if(((retVal=check_for_stop_button())!=MAIN_OK)){return retVal;}                      
        }
        for(address=(TESTPAR.test_duration-1); address>0; address--)
        {
            read_adc_res_sram_test(address);
            for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
            {                                       
                if((search_closing_time[i])&&(search_opening_time[i])&&(address<second_opening_time[i]-distance_from_last_opening_time))
                {
                if(ADC_Results.MC[i]<MAIN_THRESHOLD_LOW)
                {                  
                    if(search_last_low_value[i])
                    {
                        last_low_value_address[i]=address;
                        search_last_low_value[i]=0;
                    }
                }
                }
            }            
            //Chek if stop button is pressed:    
            if(((retVal=check_for_stop_button())!=MAIN_OK)){return retVal;}                       
        } 
        for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
        {   
            if((search_first_low_value[i]==0)&&(search_last_low_value[i]==0))
            {
                if(last_low_value_address[i]>first_low_value_address[i])
                {
                    mid_low_value_address[i]=last_low_value_address[i]/2;
                    mid_low_value_address[i]+=first_low_value_address[i]/2;
                }        
            }
        }
    }   
                
    //find opening times for each channel:
    for(address=(TESTPAR.test_duration-1); address>0; address--)
    {
    read_adc_res_sram_test(address);                      

        for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
        {                
            if(search_opening_time[i]==1)
            {      
                if((address<mid_low_value_address[i])&&(address<second_opening_time[i]-distance_from_last_opening_time))
                {
                    if(ADC_Results.MC[i]>MAIN_THRESHOLD_HIGH)
                    {
                        search_opening_time[i]=0;
                        opening_time[i]=address;                                        
                    }
                }                                                                              
            }                       
        } 
        //Chek if stop button is pressed:    
        if(((retVal=check_for_stop_button())!=MAIN_OK)){return retVal;}                                                                                             
    }   
    
    for(address=0; address<(TESTPAR.test_duration); address++)
    {
    read_adc_res_sram_test(address);                      

        for(i=0; i<NUMBER_OF_MAIN_CONTACTS; i++)
        {                
            if(search_closing_time[i]==1)
            {       
                if((address>mid_low_value_address[i])&&(address<second_opening_time[i]-distance_from_last_opening_time))
                {                                
                    if(ADC_Results.MC[i]>MAIN_THRESHOLD_HIGH)
                    {
                        search_closing_time[i]=0;
                        closing_time[i]=address;                                        
                    }
                }                                                                              
            }                       
        }                                                             
        
        //Cini mi se da dobro racuna:                                                       
        search_max_current_per_ch(address);                                                                                  
        search_min_max_voltage_per_ch(address);
        
        //Chek if stop button is pressed:    
        if(((retVal=check_for_stop_button())!=MAIN_OK)){return retVal;}                                                                                                   
    }   
                
    //Store obtained results to global variables:
    j=0;
    for(i=0; i<6; i++)
    {
        MainC_Results.ClosingTime[i][0]=closing_time[j]; 
        MainC_Results.FirstOpeningTime[i][0]=opening_time[j]; 
        MainC_Results.SecondOpeningTime[i][0]=second_opening_time[j]; 
        j++;
        MainC_Results.ClosingTime[i][1]=closing_time[j];
        MainC_Results.FirstOpeningTime[i][1]=opening_time[j];
        MainC_Results.SecondOpeningTime[i][1]=second_opening_time[j];
        j++;
    }  
    
return retVal;                
}

unsigned int MeasurementStart_Comm(void)
{
unsigned int retVal=MAIN_NOK;
unsigned int DRMThreshold;
unsigned int DRM_open_coil_time;
unsigned int i, j;
uint32_t address;

#if ST_LINK_DEBUG	
TESTPAR.test_duration=7000;
TESTPAR.sampl_resol=MEASURING_RES_01ms;
TESTPAR.bs_grounded=NO;
TESTPAR.drm_test=NO;
TESTPAR.ext_trigg_src=EXT_TRIG_OFF;
#endif

float pomvar=0;

DRM_open_coil_time=TESTPAR.o_pulse_lenght*2-1;
		  
//operating:                                                           
PosaljiMSodgovor('1');

//Set AUX/DRY wet mode:
//TODO : PAZI, moze biti problem ako se prebaci na printer:
SET_AUX_CHANNELS; //PROBATI SA I BEZ OVE FUNKCIJE!

//Set range for all analog channels:
//TODO: I2C
SetAnalogCHRange(ANALOGCH1, TESTPAR.analog1_gain);
SetAnalogCHRange(ANALOGCH2, TESTPAR.analog2_gain);
SetAnalogCHRange(ANALOGCH3, TESTPAR.analog3_gain);
SetAnalogCHRange(ANALOGCH4, TESTPAR.analog4_gain);
SetAnalogCHRange(ANALOGCH5, TESTPAR.analog5_gain);
SetAnalogCHRange(ANALOGCH6, TESTPAR.analog6_gain);

//Set OSG/BSG mode:
set_osg_or_bsg();

sprintf(OutputBuffer, "OK");    
SendOutputBuffer('A');

//Reset digital transducer counter:
DigTrans_Counter_Reset();

if(TESTPAR.drm_test==NO)
{
  calculate_offset_for_CC_channels();                            
}

//ovo je bilo prije popravke mjerenja vremena na kalibraciji:

if(TESTPAR.drm_test==YES)
{
    HAL_Delay(500);
	  //TODO:
    //set_DRM_test();   
    //tcc1_init_50us();    
}

if(TESTPAR.ext_trigg_src==EXT_TRIG_OFF)
{			
       sprintf(OutputBuffer, "OP"); 
       SendOutputBuffer('B');
}
else
{
      sprintf(OutputBuffer, "WT"); 
      SendOutputBuffer('B'); 
      
	    //TODO:
      //retVal=wait_for_external_trigger(); 
      
      if(retVal!=MAIN_OK)
      {
        return (retVal);                                      
      }      
			
      if(TESTPAR.sampl_resol!=MEASURING_RES_01ms)
      {
        //Ovo izmjeniti: Treba dodati da salje u toku testa kod 1 i 10ms rezolucija.
        //Kod 0.1 ms rezolucije uzima previse vremena. Zato se prakticno stavlja na kraju zavrsenog testa!                                        
        sprintf(OutputBuffer, "MS4");
        SendOutputBuffer('A');
      }                  
} 

switch(TESTPAR.op_sequence)
{
    case OPEN_TEST: 
    case OdelayC_TEST:
    case O_03s_C_TEST: 
    case O_03s_CO_TEST:
    case O_03s_CO_FCT_TEST:
    case OdelayCO_DELAY_TEST: 
    case OdelayCO_FCT_TEST:  
        drive_open_coil(1);                                                                                                               
    break;
    case CLOSE_TEST:
    case TRIPFREE_DELAY_TEST:  
    case TRIPFREE_FCT_TEST:
    case CdelayO_TEST:     
        drive_close_coil(1);                                              
    break;  
}


if(TESTPAR.drm_test==NO)
{
    //#asm ("cli");
	  //TODO:setovati timer:
    //tcc1_init_100us();
    //#asm ("sei");
	
		//treba clear-at flag prije pozivanje START funkcije:
		__HAL_TIM_CLEAR_FLAG(&htim2, TIM_IT_UPDATE);	
		//inace ce otici odmah u intterupt prije po pozivanju START_IT funkcije (dole)
		HAL_TIM_Base_Start_IT(&htim2);
	
    timer1_interrupt=1;
}

j=0;

control_after_first_contact_touched=1;

SET_GPIO_Structures();
SET_GPIOB_Structure(0);

#if 1
for(address=0; address<TESTPAR.test_duration; address++)
{   
		//TODO:
		#if 0
    if(TESTPAR.drm_test==YES)
    {
            while(timer1_interrupt==0)
						{
															
						} 
            #asm ("nop");
            timer1_interrupt=0;												
                          
            //read_all_adc_channels_DRM_test(address);
            read_all_adc_channels_test(address);            
                        
            //if(address!=0)
            //{                                                                                        
            //    coil_control(address);
            //}
            
            if(address==DRM_open_coil_time){drive_open_coil(0);}
                    
            retVal=check_for_stop_button();
            #if (ATMEL_STUDIO_DEBUG)
            retVal=MAIN_OK;            
            #endif
            if(retVal!=MAIN_OK)
            {
                address=TESTPAR.test_duration;                
            }
    }
    else
		#endif
    {
    if(TESTPAR.sampl_resol==MEASURING_RES_01ms)
		//measuring resolution = 0.1 ms
    {
						#if 1
            while(timer1_interrupt==0)
						{
							HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0); //treba pozvati bilo koju HAL funkciju da bi izasao iz ove petlje!
						}
						#endif
						//__HAL_TIM_CLEAR_FLAG(&htim2, TIM_IT_UPDATE);	
            timer1_interrupt=0;
						//char nova_varijabla=HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
						//HAL_Delay(1);
                          
            read_all_adc_channels_test(address);   
			
            if(address>5)
            {                                                                                        
                coil_control(address);
            } 
						//TODO: implement this one:
            //retVal=check_for_stop_button();
						//privremeno:
						retVal=MAIN_OK;
            
            if(retVal!=MAIN_OK)
            {
                address=TESTPAR.test_duration;                
            }
    }
		#if 0
    else if(TESTPAR.sampl_resol==MEASURING_RES_1ms)
		//measuring resolution = 1 ms
    {
        for(i=1; i<11; i++)
        {   
            while(timer1_interrupt==0)
						{
							HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0); //treba pozvati bilo koju HAL funkciju da bi izasao iz ove petlje!
						}
            timer1_interrupt=0;  
                      
            //read adc channels:
            if(i==1)
            {
                read_all_adc_channels_test(address);                            
            }                         
            else
            {
                //delay_us(100);
            } 
            //coil control:                                 
            if(address<2000)
            {                
                coil_control(address+j+i-1);                            
            }
            
            retVal=check_for_stop_button();
            if(retVal!=MAIN_OK)
            {
                address=TESTPAR.test_duration;                
            }  
        }
        j+=9;
    }                       
    else
		//measuring resolution = 10 ms
    {
        for(i=1; i<101; i++)
        {            
            while(timer1_interrupt==0)
						{
							HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0); //treba pozvati bilo koju HAL funkciju da bi izasao iz ove petlje!
						}
            timer1_interrupt=0;   
                        
            //read adc channels:
            if(i==1)
            {
                read_all_adc_channels_test(address);
            }                         
            else
            {
                //delay_us(100);
            }
            //coil control: 
            if(address<200)
            {
                coil_control(address*i);            
            }
            
            retVal=check_for_stop_button();
            if(retVal!=MAIN_OK)
            {
                address=TESTPAR.test_duration;                
            }  
        }
    }
		#endif
    }
}
#endif

if(TESTPAR.drm_test==NO)
{
    if(TESTPAR.ext_trigg_src!=EXT_TRIG_OFF)
    {
        if(TESTPAR.sampl_resol==MEASURING_RES_01ms)
        {
            sprintf(OutputBuffer, "MS4");
            SendOutputBuffer('A');
            //Vjestacki napravljeno kasnjenje da se ima osjecaj da je poceo test.
            HAL_Delay(1200);
        }
    }
}

#if 1
// Disable and reset the timer/counter:
//tc1_disable(&TCC1);
	HAL_TIM_Base_Stop_IT(&htim2);
#endif

//Turn off all coils:
CLR_BREAKING1_IGBT;
CLR_BREAKING2_IGBT;
CLR_BREAKING3_IGBT;
CLR_CLOSING1_IGBT;
CLR_CLOSING2_IGBT;
CLR_CLOSING3_IGBT;

if(TESTPAR.bs_grounded==YES)
{
    TURN_OFF_BSG;
}

if(retVal==STOP_BUTTON)
{
    //return(stop_button_procedure());
}

 

//Send processing data status to DV Win:
sprintf(OutputBuffer, "PD"); 
SendOutputBuffer('B');  
//Send PD status to control board:
PosaljiMSodgovor('2');

if(COMM.remote_enable==REMOTE_ENABLE)
{
    HAL_Delay(500);
}
else
{		
		
	  #if 1
    switch(TESTPAR.op_sequence)
    {
        case CLOSE_TEST:
            retVal=process_data_for_closing_test();       
        break;
        case OPEN_TEST: 
        case FIRST_TRIP_TEST:
            retVal=process_data_for_opening_test();               
        break;            
        case TRIPFREE_DELAY_TEST:
        case TRIPFREE_FCT_TEST: 
        case CdelayO_TEST:  
            retVal=process_data_for_CO_test();        
        break; 
        case O_03s_C_TEST:  
        case OdelayC_TEST:
            retVal=process_data_for_OC_test();
        break;    
        case O_03s_CO_TEST:
        case OdelayCO_DELAY_TEST:
        case OdelayCO_FCT_TEST:
        case O_03s_CO_FCT_TEST:
            retVal=process_data_for_OCO_test();
        break;    
    }
    if(retVal==STOP_BUTTON)
    {
        //return(stop_button_procedure());
    }
		#endif
}    

HAL_Delay(1000);                   

#if 1
if(TESTPAR.drm_test==YES){sprintf(OutputBuffer, "RM"); SendOutputBuffer(COM_PORT_B); sprintf(OutputBuffer, ""); COMM.port=COM_PORT_A; TESTPAR.drm_test=NO;}
else                     {sprintf(OutputBuffer, "RM"); SendOutputBuffer(COM_PORT_B); sprintf(OutputBuffer, "MS3");}
#endif                                                     
   
                           
//OK, GO TO RESULT MENU:                                                                                                                                                    
                                                                                                                                            
retVal=MAIN_OK;    
                                                                                                                                                                                                                                                
return retVal;
}

unsigned int Get_Measurement_Data_Comm(void)
{
unsigned int retVal=MAIN_NOK;
unsigned int check, address;
unsigned char channel;
unsigned int number_of_samples;
unsigned int step; 
                
unsigned int MAX_NUMBER_OF_SAMPLES=20000;
unsigned int MAX_SAMPLE_STEP=9999;   
   
check=check_if_digit(4,12);

if(check==MAIN_OK)
{                                                                                     
    number_of_samples=string_to_int(4,8);      
    step=string_to_int(9,12);  
    
    if((number_of_samples>0)&&(number_of_samples<=MAX_NUMBER_OF_SAMPLES)&&(step>0)&&(step<=MAX_SAMPLE_STEP))
    {                
    
    //wait 'A' from PC before sending data:
    wait_char_from_pc(0);                                                                                                                                                                                                                                              
      
		//ovo izbrisi. napravljano privremeno!
		//step=2;
		//number_of_samples*=2;
		//
			
    //send data to PC:                                    
    for(address=0; address<number_of_samples; address+=step)
    {                                                                  
        read_adc_res_sram_test(address);
        
        send_measuring_samples_to_pc();                                                                                
    }
    
    send_data_transfer_over_status();             
                        
    retVal=MAIN_OK;                                  
    }                               
}                  
return retVal;
}

// Relay&IGBT Control Command
unsigned int Relay_IGBT_Control(void)
{
unsigned int retVal=MAIN_NOK;
switch (InputBuffer[2])
{
    case 'A':
    //Analog Channels: Gain Control Commands
    //RA X Y Z
    //X -  1-Analog1, 2-Analog2, 3-Analog3, 4-Analog4, 5-Analog5, 6-Analog6
    //Y  - S - Set
    //Z  - 0-LL, 1-LH, 2-HL, 3-HH (represents relay/gain cobination for 4 measurement ranges on all analog channels!
        //if((InputBuffer[3]=='1')||(InputBuffer[3]=='2'))
        if((InputBuffer[3]==ANALOGCH1)||(InputBuffer[3]==ANALOGCH2)||(InputBuffer[3]==ANALOGCH3)
         ||(InputBuffer[3]==ANALOGCH4)||(InputBuffer[3]==ANALOGCH5)||(InputBuffer[3]==ANALOGCH6))
        {                                    
            if(InputBuffer[4]=='S')
            {
                if((InputBuffer[5]==ANALOG_CH_RANGE_LL)||(InputBuffer[5]==ANALOG_CH_RANGE_LH)
                ||(InputBuffer[5]==ANALOG_CH_RANGE_HL)||(InputBuffer[5]==ANALOG_CH_RANGE_HH))
                {
                    SetAnalogCHRange(InputBuffer[3],InputBuffer[5]);
                    retVal=MAIN_OK;   
                    OK_Response();
                }
            }                 
        }                
    break; 
    case 'C':
    //CoilControl: IGBT/Relay Control Commands
    //RC XX Y Z 
    //XX = C1-Closing1, C2-Closing2, C3-Closing3, B1-Breaking1, B2-Breaking2, B3-Breaking3;
    //Y  = R - Relay , I - IGBT
    //Z  = 0 ili 1 (reset/set)      
    if((InputBuffer[3]=='C')||(InputBuffer[3]=='B')){ 
    if((InputBuffer[4]=='1')||(InputBuffer[4]=='2')||(InputBuffer[4]=='3')){
    if((InputBuffer[5]=='R')||(InputBuffer[5]=='I')){
    if((InputBuffer[6]=='0')||(InputBuffer[6]=='1')){  
    retVal=MAIN_OK;
    OK_Response();   
        if(InputBuffer[3]=='B')
        {
            if((InputBuffer[4]=='1')&&(InputBuffer[5]=='R'))
            {
                if((InputBuffer[6]=='1')){SET_BREAKING1_RELAY;}
                else                     {CLR_BREAKING1_RELAY;}                
            }
            if((InputBuffer[4]=='2')&&(InputBuffer[5]=='R'))
            {
                if((InputBuffer[6]=='1')){SET_BREAKING2_RELAY;}
                else                     {CLR_BREAKING2_RELAY;}                
            }
            if((InputBuffer[4]=='3')&&(InputBuffer[5]=='R'))
            {
                if((InputBuffer[6]=='1')){SET_BREAKING3_RELAY;}
                else                     {CLR_BREAKING3_RELAY;}                
            }
            if((InputBuffer[4]=='1')&&(InputBuffer[5]=='I'))
            {
                if((InputBuffer[6]=='1')){SET_BREAKING1_IGBT;}
                else                     {CLR_BREAKING1_IGBT;}                
            }
            if((InputBuffer[4]=='2')&&(InputBuffer[5]=='I'))
            {
                if((InputBuffer[6]=='1')){SET_BREAKING2_IGBT;}
                else                     {CLR_BREAKING2_IGBT;}                
            }
            if((InputBuffer[4]=='3')&&(InputBuffer[5]=='I'))
            {
                if((InputBuffer[6]=='1')){SET_BREAKING3_IGBT;}
                else                     {CLR_BREAKING3_IGBT;}                
            }            
        }
        if(InputBuffer[3]=='C')
        {                      
              if((InputBuffer[4]=='1')&&(InputBuffer[5]=='R'))
            {
                if((InputBuffer[6]=='1')){SET_CLOSING1_RELAY;}
                else                     {CLR_CLOSING1_RELAY;}           
            }
            if((InputBuffer[4]=='2')&&(InputBuffer[5]=='R'))
            {
                if((InputBuffer[6]=='1')){SET_CLOSING2_RELAY;}
                else                     {CLR_CLOSING2_RELAY;}                
            }
            if((InputBuffer[4]=='3')&&(InputBuffer[5]=='R'))
            {
                if((InputBuffer[6]=='1')){SET_CLOSING3_RELAY;}
                else                     {CLR_CLOSING3_RELAY;}                
            }
            if((InputBuffer[4]=='1')&&(InputBuffer[5]=='I'))
            {
                if((InputBuffer[6]=='1')){SET_CLOSING1_IGBT;}
                else                     {CLR_CLOSING1_IGBT;}                
            }
            if((InputBuffer[4]=='2')&&(InputBuffer[5]=='I'))
            {
                if((InputBuffer[6]=='1')){SET_CLOSING2_IGBT;}
                else                     {CLR_CLOSING2_IGBT;}                
            }
            if((InputBuffer[4]=='3')&&(InputBuffer[5]=='I'))
            {
                if((InputBuffer[6]=='1')){SET_CLOSING3_IGBT;}
                else                     {CLR_CLOSING3_IGBT;}                
            }                
        }            
    }
    }
    }
    }
    break;    
}
return retVal;
}

unsigned int MemoryTest_Comm(void)
{
unsigned int retVal=MAIN_NOK;
uint32_t sram_address;
uint32_t sram_address_aux;
uint16_t sram_data;
uint16_t sram_data2;
unsigned char byte;
unsigned char Memory_Read_Error=0;

	for(sram_address=0; sram_address<0x800000; sram_address+=2)  
	//for(sram_address=0; sram_address<0x400000; sram_address+=2)  //ovo radi i sa 0x400000!? 
	//for(sram_address=0; sram_address<0x200000; sram_address+=1) //probati
	{
			sram_data=(uint16_t) sram_address;
			sram_data+=3;
			sram_address_aux=(uint32_t)0x60000000+sram_address;
			HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address_aux, &sram_data, 1); //ovako ide upis!
	}
	
	for(sram_address=0; sram_address<0x800000; sram_address+=2)  
	//for(sram_address=0; sram_address<0x400000; sram_address+=2) //ovo radi i sa 0x400000!?
	//for(sram_address=0; sram_address<0x200000; sram_address+=1) //ovo ne radi?
  {
		
			sram_data2=0;
			sram_data=(uint16_t) sram_address;
			sram_address_aux=(uint32_t)0x60000000+sram_address;
		
			HAL_SRAM_Read_16b(&hsram1, (uint32_t *)sram_address_aux, &sram_data2, 1); //ovako mora ici citanje!
				
			sram_data+=3; 
		
            if(sram_data!=sram_data2)
            {
                Memory_Read_Error=1;
								sprintf(OutputBuffer, "address=%lu", (unsigned long)sram_address);
								SendOutputBuffer('B');
								sprintf(OutputBuffer, "sram_data1=%d", sram_data);
								SendOutputBuffer('B');
								sprintf(OutputBuffer, "sram_data2=%d", sram_data2);
								SendOutputBuffer('B');
								break;								    
            }    
    }   
	
    if(Memory_Read_Error==1)
    {                 
        retVal=ERROR_SRAM;
    }
    else                    
    {    
        retVal=MAIN_OK;        			
    }        

return retVal;
}

//NAPOMENA: IZGLEDA DA IMAM PROBLEM SA NEPARNIM BROJEVIMA!!

//TEST COMMANDS:
unsigned int MemoryWriteByte_Comm(void)
{
unsigned int retVal=MAIN_NOK;
unsigned int check;
uint32_t address;
uint32_t received_address;
uint16_t data;
		      
check=check_if_digit(4,15);
if(check==MAIN_OK)
{            
    address=(uint32_t)string_to_long(4,10);
		received_address=(unsigned long)address;	
		address+=(uint32_t)0x60000000; //mora biti veca od 0x60000000 gdje je lociran sram
		data=(uint16_t)string_to_int(11,15);
		sprintf(OutputBuffer, "A:%07lu_D:%05u", (unsigned long)received_address, (unsigned int)data);
		SendOutputBuffer('B');
	  
		HAL_SRAM_Write_16b(&hsram1, (uint32_t *)address, &data, 1); //ovako ide upis!	
		
    OK_Response();                                                                                                                                    
    retVal=MAIN_OK;                              
}
                                                                                                                                                                                                                                                
return retVal;
}

unsigned int MemoryReadByte_Comm(void)
{
unsigned int retVal=MAIN_NOK;
unsigned int check;
uint32_t address;
uint32_t received_address;
uint16_t data;
	
data=0;
		      
check=check_if_digit(4,10);
if(check==MAIN_OK)
{        
    address=string_to_long(4,10);  
		received_address=(unsigned long)address;
	  address+=0x60000000; //mora biti veca od 0x60000000 gdje je lociran sram		
	
		//HAL_SRAM_Read_16b(&hsram1, &address2, &data, 1); 				
		HAL_SRAM_Read_16b(&hsram1, (uint32_t *)address, &data, 1); //ovako mora ici citanje!
    
    sprintf(OutputBuffer, "A:%07lu_D:%05u", (unsigned long)received_address, data);                                                                                                                                                
    retVal=MAIN_OK;
}    
                                                                                                                                                                                                                                                
return retVal;
}

//DRM Functions

//Toggle pins test function
unsigned int Toggle_Pins(void)
{
		unsigned int retVal=MAIN_OK;						
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
	  sprintf(OutputBuffer,"Toggling DRM pins.");
		return retVal;
}

//Write DAC for selected Channel
unsigned int Write_DRM_Channel(void)
{
		unsigned int retVal = MAIN_NOK;
		unsigned int dac_output;
		unsigned int ramp = 0;
		unsigned int delay_ms = 100;
	  unsigned int i = 0;
		unsigned int offset1 = 470;
		unsigned int offset2 = 700;
		unsigned int offset3 = 391;
		unsigned int dac_limit = 30000;
	  unsigned int channel;
		uint16_t drm1_current1, drm1_voltage1;
		uint16_t drm1_current2, drm1_voltage2;
		uint16_t drm1_current3, drm1_voltage3;
		double conversion_factor = 0.00335628458; //conversion_factor = (Vadc(max)/ADC_RANGE)*(Aop/Rsh) = 10/65535*(1/(1+50/0.556))*2000
		if(InputBuffer[3]=='1') channel = CHANNEL1;
		if(InputBuffer[3]=='2') channel = CHANNEL2;
		dac_output = string_to_int(4, 8);			
		DRM_Channel_Enable(channel);
		Ramp(channel, dac_output, RAMP_UP);
		HAL_Delay(10);
		DRM_Channel_Enable(channel);
		HAL_Delay(delay_ms);
		DRM1_ADC_Read_All();
		drm1_current1 = ADC_Results.ANCH[0] - offset1;
		drm1_voltage1 = ADC_Results.ANCH[1];
		drm1_current2 = ADC_Results.ANCH[2] - offset2;
		drm1_voltage2 = ADC_Results.ANCH[3];
		drm1_current3 = ADC_Results.ANCH[4] - offset3;
		drm1_voltage3 = ADC_Results.ANCH[5];
		Ramp(channel, dac_output, RAMP_DOWN);
		DRM_Channel_Disable(channel);
		DRM_DAC_Write(0, channel);
		if(InputBuffer[3]=='1') sprintf(OutputBuffer,"Write %.2fV to DAC1:\r\nADC current: (%u), %.2fA\r\nADC voltage: (%u)V, %.2f", dac_output*5.0/65535, drm1_current1, 
			drm1_current1*conversion_factor, drm1_voltage1, drm1_voltage1*10.0/65535);
		if(InputBuffer[3]=='2') sprintf(OutputBuffer,"Write %.2fV to DAC2:\r\nADC current: (%u), %.2fA\r\nADC voltage: (%u)V, %.2f", dac_output*5.0/65535, drm1_current2, 
			drm1_current2*conversion_factor, drm1_voltage2, drm1_voltage2*10.0/65535);
		if(InputBuffer[3]=='3') sprintf(OutputBuffer,"Write %.2fV to DAC3:\r\nADC current: (%u), %.2fA\r\nADC voltage: (%u)V, %.2f", dac_output*5.0/65535, drm1_current3, 
			drm1_current3*conversion_factor, drm1_voltage3, drm1_voltage3*10.0/65535);
		retVal = MAIN_OK;
		return retVal;
}

unsigned int DRM_Get_Results(void)
{
		unsigned int retVal = MAIN_OK;
		uint16_t sample_size = DRM_SAMPLE_SIZE;
		uint16_t data_voltage1, data_voltage2, data_voltage3;
		uint16_t data_current1, data_current2, data_current3;
		uint16_t voltage_array1[sample_size], voltage_array2[sample_size], voltage_array3[sample_size]; 
	  uint16_t current_array1[sample_size], current_array2[sample_size], current_array3[sample_size];
		uint16_t i, j;
		uint32_t sram_address = SRAM_BASE_ADDRESS;	
		
		#if 1
		float voltage1, voltage2;
		DRM1_ADC_Read_All();
		data_current1 = ADC_Results.ANCH[0];
		data_voltage1 = ADC_Results.ANCH[1];
		data_current2 = ADC_Results.ANCH[2];
		data_voltage2 = ADC_Results.ANCH[3];	
		
		voltage1 = data_voltage1 * 10.1 / 65535;
		voltage2 = data_voltage2 * 10.14 / 65535;
	
		sprintf(OutputBuffer, "I1: %u, V1: %.2f, I2: %u, V2: %.2f;", data_current1, voltage1, data_current2, voltage2);
		#endif
	
		//Onesposobljeno u toku proradjivanja mjernih kanala
		#if 0
		//Timer setup and start
		__HAL_TIM_CLEAR_FLAG(&htim2, TIM_IT_UPDATE);	//clear flag odmah kako ne bi usao u interrupt
		HAL_TIM_Base_Start_IT(&htim2); //pokreni tajmer
    timer1_interrupt=1;
		
		//Read ADC and write to RAM
		for(i=0; i<sample_size; i++)
		{
			while(timer1_interrupt==0);
			timer1_interrupt=0;
			//DRM1_ADC1_Read();
			DRM1_ADC_Read_All();
			data_current1 = ADC_Results.ANCH[0];
			data_voltage1 = ADC_Results.ANCH[1];
			data_current2 = ADC_Results.ANCH[2];
			data_voltage2 = ADC_Results.ANCH[3];
			HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data_current1, 1);					
			sram_address+=2;
			HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data_voltage1, 1);
			sram_address+=2;
			HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data_current2, 1);					
			sram_address+=2;
			HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data_voltage2, 1);
			sram_address+=2;	
		}		
		
		//RAM read and send to GUI
		sram_address = SRAM_BASE_ADDRESS;
		for(i=0; i<sample_size; i++)
		{
			HAL_SRAM_Read_16b(&hsram1, (uint32_t *)sram_address, &data_current1, 1);
			sram_address+=2;
			HAL_SRAM_Read_16b(&hsram1, (uint32_t *)sram_address, &data_voltage1, 1);
			sram_address+=2;
			HAL_SRAM_Read_16b(&hsram1, (uint32_t *)sram_address, &data_current2, 1);
			sram_address+=2;
			HAL_SRAM_Read_16b(&hsram1, (uint32_t *)sram_address, &data_voltage2, 1);
			sram_address+=2;
			if(InputBuffer[3]=='C')
				sprintf(OutputBuffer, "%u,%u,%u,%u;", data_current1, data_voltage1, data_current2, data_voltage2);
			if(InputBuffer[3]=='1')
				sprintf(OutputBuffer, "%u,%u;", data_current1, data_voltage1);
			if(InputBuffer[3]=='2')
				sprintf(OutputBuffer, "%u,%u;", data_current2, data_voltage2);
			SendOutputBuffer(COMM.port);
			if(i==2000 | i==4000 | i==6000 | i==8000)
			{
				while(getcharB() != 'A') HAL_Delay(10);	
			}
		}		
		//End of transfer
		sprintf(OutputBuffer,"OK");
		#endif
				
		return retVal;
}

unsigned int DRM_Get_Results3(void)
{
		unsigned int retVal = MAIN_OK;
		uint16_t sample_size = DRM_SAMPLE_SIZE;
		uint16_t data_voltage3;
		uint16_t data_current3;
		uint16_t voltage_array3[sample_size]; 
	  uint16_t current_array3[sample_size];
		uint16_t i, j;
		uint32_t sram_address = SRAM_BASE_ADDRESS;	
	
		//Timer setup and start
		__HAL_TIM_CLEAR_FLAG(&htim2, TIM_IT_UPDATE);	//clear flag odmah kako ne bi usao u interrupt
		HAL_TIM_Base_Start_IT(&htim2); //pokreni tajmer
    timer1_interrupt=1;
		
		//Read ADC and write to RAM
		for(i=0; i<sample_size; i++)
		{
			while(timer1_interrupt==0);
			timer1_interrupt=0;
			//DRM1_ADC1_Read();
			DRM1_ADC_Read_All();
			data_current3 = ADC_Results.ANCH[4];
			data_voltage3 = ADC_Results.ANCH[5];
			HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data_current3, 1);					
			sram_address+=2;
			HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data_voltage3, 1);
			sram_address+=2;
		}		
		
		//RAM read and send to GUI
		sram_address = SRAM_BASE_ADDRESS;
		for(i=0; i<sample_size; i++)
		{
			HAL_SRAM_Read_16b(&hsram1, (uint32_t *)sram_address, &data_current3, 1);
			sram_address+=2;
			HAL_SRAM_Read_16b(&hsram1, (uint32_t *)sram_address, &data_voltage3, 1);
			sram_address+=2;
			sprintf(OutputBuffer, "%u,%u;", data_current3, data_voltage3);
			SendOutputBuffer(COMM.port);
			if(i==2000 | i==4000 | i==6000 | i==8000)
			{
				while(getcharB() != 'A') HAL_Delay(10);	
			}
		}
				
		//End of transfer
		sprintf(OutputBuffer,"OK");
		return retVal;
}


//Battery charger control (I2C) Turn on/off +5V from battery
//unsigned int Battery_Battery_Charger_Control(void)
//{
//		unsigned int retVal = MAIN_OK;
//	//Command: BAT11 - ukljuci CHG1, BAT10 - iskljuci CHG1, itd.
//		if(InputBuffer[5] == '1'){
//			if(InputBuffer[4] == '1')
//			{
//				DRM1_Bat_Chg_Info1 &= ~(DRM1_ADPT_RLY_CH1);
//				HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U2, (uint8_t*)&DRM1_Bat_Chg_Info1, 1, 1000);
//				sprintf(OutputBuffer, "Enable PWR Relay CHG1");
//			}
//			if(InputBuffer[4] == '2')
//			{
//				DRM1_Bat_Chg_Info1 &= ~(DRM1_ADPT_RLY_CH2);	
//				HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U2, (uint8_t*)&DRM1_Bat_Chg_Info1, 1, 1000);				
//				sprintf(OutputBuffer, "Enable PWR Relay CHG2");
//			}
//			if(InputBuffer[4] == '3')
//			{			
//				DRM1_Bat_Chg_Info2 &= ~(DRM1_ADPT_RLY_CH3);	
//				HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U4, (uint8_t*)&DRM1_Bat_Chg_Info2, 1, 1000);	
//				sprintf(OutputBuffer, "Enable PWR Relay CHG3");
//			}
//		}
//		else if(InputBuffer[5] == '0')
//		{
//			if(InputBuffer[4] == '1')
//			{
//				DRM1_Bat_Chg_Info1 |= DRM1_ADPT_RLY_CH1;
//				HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U2, (uint8_t*)&DRM1_Bat_Chg_Info1, 1, 1000);
//				sprintf(OutputBuffer, "Disable PWR Relay CHG1");
//			}
//			if(InputBuffer[4] == '2')
//			{
//				DRM1_Bat_Chg_Info1 |= DRM1_ADPT_RLY_CH2;
//				HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U2, (uint8_t*)&DRM1_Bat_Chg_Info1, 1, 1000);
//				sprintf(OutputBuffer, "Disable PWR Relay CHG2");
//			}
//			if(InputBuffer[4] == '3')
//			{
//				DRM1_Bat_Chg_Info2 |= DRM1_ADPT_RLY_CH3;	
//				HAL_I2C_Master_Transmit(&hi2c1, GPIO_EXPANSION_U4, (uint8_t*)&DRM1_Bat_Chg_Info2, 1, 1000);	
//				sprintf(OutputBuffer, "Disable PWR Relay CHG3");
//			}
//		}
//		return retVal;
//}

unsigned int DRM_Start_Test(void)
{
	//1000ms, 20000 uzoraka, 20kHz, struja maksimalna DAC:65000. 
	//Za sada fiksna vrijednost struje za DAC=28000, oko 100A.
	//Postaviti da se moze odabrati vrijednost struje. Ubaciti regulator uspostavljanja struje. 
	//Za slozene operacije potrebno ubaciti odabir i kontrolu u petlji.
		unsigned int retVal = MAIN_OK;
		uint16_t sample_size = DRM_SAMPLE_SIZE;
		unsigned int dac_output = 0;
		uint16_t data_voltage1, data_voltage2;
		uint16_t data_current1, data_current2;
		uint16_t voltage_array1[sample_size], voltage_array2[sample_size]; 
	  uint16_t current_array1[sample_size], current_array2[sample_size];
		uint16_t i;
		uint32_t sram_address = SRAM_BASE_ADDRESS;
		
		dac_output = string_to_int(5, 9);
	
	//2. Ukljucivanje izlaznih tranzistora.	
		DRM_Channel_Enable(CHANNEL1);
		DRM_Channel_Enable(CHANNEL2);
	
	//3. Ispisati na DAC vrijednost struje. Sacekati 10ms da se izregulise. Ovdje ubaciti rampu
//		DRM_DAC_Write(dac_output, CHANNEL1);
//		DRM_DAC_Write(dac_output, CHANNEL2);
//		DRM_DAC_Write(dac_output, CHANNEL3);
		Ramp(CHANNEL1, dac_output, RAMP_UP);
		Ramp(CHANNEL2, dac_output, RAMP_UP);
		HAL_Delay(10);

	//4. Ukljuciti spulu (open ili close). Omoguciti odabir spule prije testa. Ubaciti funkcije za ovo i provjeriti pomocu napojne pustanje struje preko coila.
	//TODO: Ukljuciti spulu
		if(InputBuffer[4]=='1')
		{
			Coil_Control(COIL_CLOSE, SET);
		}
		if(InputBuffer[4]=='2')
		{
			Coil_Control(COIL_OPEN, SET);
		}
	
	/* Timer setup and start */
		MX_TIM2_Init();
		timer1_DRM_ON = 1;
		__HAL_TIM_CLEAR_FLAG(&htim2, TIM_IT_UPDATE);	//clear flag odmah kako ne bi usao u interrupt
		HAL_TIM_Base_Start_IT(&htim2); //pokreni tajmer
		timer1_interrupt=1;
		
	//5. Poceti semplovanje svih kanala.	
		for(i=0; i<sample_size; i++)
		{
			while(timer1_interrupt==0);
			timer1_interrupt=0;
			DRM1_ADC_Read_All();
			data_current1 = ADC_Results.ANCH[0];
			data_voltage1 = ADC_Results.ANCH[1];
			data_current2 = ADC_Results.ANCH[2];
			data_voltage2 = ADC_Results.ANCH[3];
			HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data_current1, 1);					
			sram_address+=2;
			HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data_voltage1, 1);
			sram_address+=2;
			HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data_current2, 1);					
			sram_address+=2;
			HAL_SRAM_Write_16b(&hsram1, (uint32_t *)sram_address, &data_voltage2, 1);
			sram_address+=2;	
			
			//6. Vremena operacija podesiti. Ovaj put staviti isto trajanje. 2000iteracija = 100ms
			if(i==2000)
			{
				DRM_DAC_Write(0, CHANNEL1);
				DRM_DAC_Write(0, CHANNEL2);
				DRM_Channel_Disable(CHANNEL1);
				DRM_Channel_Disable(CHANNEL2);
			}
		}
		//7. Radi sigurnosti jos jednom iskljucujemo kanale na kraju testa.
		DRM_DAC_Write(0, CHANNEL1);
		DRM_DAC_Write(0, CHANNEL2);	
		DRM_Channel_Disable(CHANNEL1);
		DRM_Channel_Disable(CHANNEL2);
		Coil_Control(COIL_OPEN, RESET);
		Coil_Control(COIL_CLOSE, RESET);
		
		//7.1 Iskljuci timer
		__HAL_TIM_CLEAR_FLAG(&htim2, TIM_IT_UPDATE);
		HAL_TIM_Base_Stop_IT(&htim2);
		timer1_DRM_ON = 0;
		//8. Slanje rezultata
		
		/* RAM read and send to GUI */
		sram_address = SRAM_BASE_ADDRESS;
		for(i=0; i<sample_size; i++)
		{
			HAL_SRAM_Read_16b(&hsram1, (uint32_t *)sram_address, &data_current1, 1);
			sram_address+=2;
			HAL_SRAM_Read_16b(&hsram1, (uint32_t *)sram_address, &data_voltage1, 1);
			sram_address+=2;
			HAL_SRAM_Read_16b(&hsram1, (uint32_t *)sram_address, &data_current2, 1);
			sram_address+=2;
			HAL_SRAM_Read_16b(&hsram1, (uint32_t *)sram_address, &data_voltage2, 1);
			sram_address+=2;
			sprintf(OutputBuffer, "%u,%u,%u,%u;", data_current1, data_voltage1, data_current2, data_voltage2);
			SendOutputBuffer(COMM.port);
			if(i % 2000 == 0)
			{
				while(getcharB() != 'A') HAL_Delay(10);	
			}
		}
		//End of transfer
		sprintf(OutputBuffer,"OK");
		return retVal;
}

unsigned int foo_function(void)
{
	unsigned int retVal = MAIN_OK;
	
	CURRENT_CH2_ENABLE;
	HAL_Delay(5000);
	CURRENT_CH2_DISABLE;
	sprintf(OutputBuffer, "Toggle pin PA12");
	
	return retVal;
}


unsigned int Get_BAT_Voltage(void)
{
	unsigned int retVal = MAIN_OK;
	float batVoltage1 = 0.0;
	float batVoltage2 = 0.0;
	
	VBAT_ADC_Read();	
	batVoltage1 = ADC_Results.ANCH[4] * 10.1 / 65535;
	batVoltage2 = ADC_Results.ANCH[5] * 10.14 / 65535;	
	
	sprintf(OutputBuffer, "%.2f, %.2f", batVoltage1, batVoltage2);
	
	return retVal;
}

//Funkcija za SRM test: sastoji se od jedne glavne funkcije GetSmp, koja ovisno od faze testa radi uzorkovanje. GetSmp se izvrsava na timer overflow. 
unsigned int SRM_Start_Test(void)
{
	unsigned int retVal = MAIN_OK;
	
	//1.Uzlazna rampa
	//2.Cekanje smirivanja izlaza
	//3.Mjerenje i smjestanje u buffer
	//4.Detektovanje opsega
	//5.Pustanje testa
	//6.Silazna rampa
	//Pseudokod:	
//	RAMP_UP;		//trajanje 5ms
//	HAL_Delay(5); //cekanje smirivanja izlaza
//	DETECT_RANGE; //smjestanje u buffer i detektovanje opsega
//	START_TEST; //pocetak testa i pustanje struja, mjerenje
//	RAMP_DOWN;
	
	//Kod Timura se poziva unutar timer interrupt handlera.
	//Kod DRM funkcije, interrupt podesava vrijeme semplova.
	
	timer2_SRM_ON = 1;
	__HAL_TIM_CLEAR_FLAG(&htim3, TIM_IT_UPDATE);	//clear flag odmah kako ne bi usao u interrupt
	HAL_TIM_Base_Start_IT(&htim3); //pokreni tajmer
	//SRM funkcija na kraju izvrsavanja iskljucuje ovaj interrupt
	
	while(1)
	{
		if(timer2_SRM_ON == 0) break;
	}
	
	__HAL_TIM_CLEAR_FLAG(&htim3, TIM_IT_UPDATE);
	HAL_TIM_Base_Stop_IT(&htim3); //zaustavi tajmer
	
	return retVal;
}

unsigned int Channel_Power_Control(void)
{
	unsigned int retVal = MAIN_OK;
	
	if(InputBuffer[4]=='1')
	{
		if(InputBuffer[5]=='1') 
		{
			Pwr_Control(CHANNEL1, ON);
			sprintf(OutputBuffer, "Channel 1 Power ON");
		}
		else if(InputBuffer[5]=='0') 
		{
			Pwr_Control(CHANNEL1, OFF);
		  sprintf(OutputBuffer, "Channel 1 Power OFF");
		}
	}
	if(InputBuffer[4]=='2')
	{
		if(InputBuffer[5]=='1')
		{
			Pwr_Control(CHANNEL2, ON);
			sprintf(OutputBuffer, "Channel 2 Power ON");
		}			
		else if(InputBuffer[5]=='0')
		{
			Pwr_Control(CHANNEL2, OFF);
			sprintf(OutputBuffer, "Channel 2 Power OFF");
		}			
	}
	if(InputBuffer[4]=='3')
	{
		if(InputBuffer[5]=='1')
		{
			Pwr_Control(CHANNEL1, ON);
			Pwr_Control(CHANNEL2, ON);
			sprintf(OutputBuffer, "Channel 1 and 2 Power ON");
		}			
		else if(InputBuffer[5]=='0')
		{
			Pwr_Control(CHANNEL1, OFF);
			Pwr_Control(CHANNEL2, OFF);
			sprintf(OutputBuffer, "Channel 1 and 2 Power OFF");
		}	
	}
	
	return retVal;
}

unsigned int Battery_Charger_Control(void)
{
	unsigned int retVal = MAIN_OK;
	
	if(InputBuffer[4]=='1')
	{
		if(InputBuffer[5]=='1') 
		{
			CHG1_ENABLE;
			sprintf(OutputBuffer, "Charger 1 ON");
		}
		else if(InputBuffer[5]=='0') 
		{
			CHG1_DISABLE;
		  sprintf(OutputBuffer, "Charger 1 OFF");
		}
	}
		else if(InputBuffer[4]=='2')
	{
		if(InputBuffer[5]=='1')
		{
			CHG2_ENABLE;
			sprintf(OutputBuffer, "Charger 2 ON");
		}			
		else if(InputBuffer[5]=='0')
		{
			CHG2_DISABLE;
			sprintf(OutputBuffer, "Charger 2 OFF");
		}			
	}
	return retVal;
}

unsigned int DRM_Current_Control(void)
{
	unsigned int retVal = MAIN_OK;
	
	if(InputBuffer[4]=='1')
	{
		if(InputBuffer[5]=='1')
		{
			CURRENT_CH1_ENABLE;
		}
		if(InputBuffer[5]=='0')
		{
			CURRENT_CH1_DISABLE;
		}
	}
		if(InputBuffer[4]=='2')
	{
		if(InputBuffer[5]=='1')
		{
			CURRENT_CH2_ENABLE;
		}
		if(InputBuffer[5]=='0')
		{
			CURRENT_CH2_DISABLE;
		}
	}
	return retVal;
}

unsigned int DRM_DAC_Test(void)
{
		unsigned int retVal = MAIN_OK;
		unsigned int dacValue = 0;
		
		sprintf(OutputBuffer, "DRM DAC test");
			
		if(check_if_digit(5, 9) == MAIN_OK)
		{
			dacValue = string_to_int(5, 9);
		}	
		if(InputBuffer[4]=='1')
		{	
			DRM_DAC_Write(dacValue, CHANNEL1);
			CURRENT_CH1_ENABLE;
		}
		if(InputBuffer[4]=='2')
		{
			DRM_DAC_Write(dacValue, CHANNEL2);
			CURRENT_CH2_ENABLE;
		}
		HAL_Delay(1000);
		DRM_DAC_Write(0, CHANNEL1);
		DRM_DAC_Write(0, CHANNEL2);
		CURRENT_CH1_DISABLE;
		CURRENT_CH2_DISABLE;
		return retVal;
}

void Read_ADC_Chip_Revision(unsigned int channel)
{
	unsigned int registerData;	
	SRM_Write_ADC_Byte(channel, 0x42);     
	registerData = SRM_Read_ADC_Byte(SRM1_ADC1);
	sprintf(OutputBuffer, "Chip %u Revision: 0x%x", channel, registerData);
}

void Read_ADC_IOPort_Config(unsigned int channel)
{
	unsigned int registerData;	
	SRM_Write_ADC_Byte(channel, 0x41);
	registerData = SRM_Read_ADC_Byte(SRM1_ADC1);
	sprintf(OutputBuffer, "Chip %u IO Port register: 0x%x", channel, registerData);
}

void Wait_ADC_RDY(unsigned int channel)
{
	unsigned int timeout = 1000;
	switch (channel)
	{
		case SRM1_ADC1:
			while(SRM1_RDY1 != 0)
			{
				if(--timeout == 0) break;
				DWT_Delay_us(1);
			};
		break;
		case SRM1_ADC2:
			while(SRM1_RDY2 != 0)
			{
				if(--timeout == 0) break;
				DWT_Delay_us(1);
			};
		break; 
		case SRM2_ADC1:
			while(SRM2_RDY1 != 0)
			{
				if(--timeout == 0) break;
				DWT_Delay_us(1);
			};
		break; 
		case SRM2_ADC2:
			while(SRM2_RDY2 != 0)
			{
				if(--timeout == 0) break;
				DWT_Delay_us(1);
			};
		break;
	}
}

//Setup mode za AD konverziju na jednom kolu AD7732, na oba njegova kanala. Trazi SYNC u funkciji koja je poziva.
void SRM_Single_Conversion(unsigned int channel)
{
	unsigned int data1 = 0, data2 = 0;
	SRM_Write_ADC_Byte(channel, 0x38);
	SRM_Write_ADC_Byte(channel, 0x40);
	Wait_ADC_RDY(channel);
	SRM_Write_ADC_Byte(channel, 0x48);	
	data1 = SRM_Read_ADC_16Bits(channel);
	SRM_Write_ADC_Byte(channel, 0x3A);
	SRM_Write_ADC_Byte(channel, 0x40);
	Wait_ADC_RDY(channel);
	SRM_Write_ADC_Byte(channel, 0x4A);
	data2 = SRM_Read_ADC_16Bits(channel);
	SRM_ADC_Data[2*channel] = data1;
	SRM_ADC_Data[2*channel + 1] = data2;
}


//Setup sync
void Setup_Sync_Mode(unsigned int set)
{
	if(set == SET)
	{
		for(int i = 1; i < 5; i++)
		{
			SRM_Write_ADC_Byte(i,0x01);
			SRM_Write_ADC_Byte(i,0x31);
		}
	}
	if(set == RESET)
	{
		for(int i = 1; i < 5; i++)
		{
			SRM_Write_ADC_Byte(i,0x01);
			SRM_Write_ADC_Byte(i,0x30);
		}
	}
}

//Multiple single conversions all channels
void SRM_Multiple_Conversions(void)
{
	SRM_Single_Conversion(SRM1_ADC1);
	SRM_Single_Conversion(SRM1_ADC2);
	SRM_Single_Conversion(SRM2_ADC1);
	SRM_Single_Conversion(SRM2_ADC2);
}

void SRM_Write_Data_ToArray(int i)
{
	SRM_ADC_Data_Array.data1[i] = SRM_ADC_Data[0];
	SRM_ADC_Data_Array.data2[i] = SRM_ADC_Data[1];
	SRM_ADC_Data_Array.data3[i] = SRM_ADC_Data[2];
	SRM_ADC_Data_Array.data4[i] = SRM_ADC_Data[3];
	SRM_ADC_Data_Array.data5[i] = SRM_ADC_Data[4];
	SRM_ADC_Data_Array.data6[i] = SRM_ADC_Data[5];
	SRM_ADC_Data_Array.data7[i] = SRM_ADC_Data[6];
	SRM_ADC_Data_Array.data8[i] = SRM_ADC_Data[7];
}

void SRM_Send_Data(void)
{	
	for(int i = 0; i < SRM_DATA_ARRAY_SIZE; i++)
	{
		sprintf(OutputBuffer, "%05u,%05u,%05u,%05u,%05u,%05u,%05u,%05u;", SRM_ADC_Data_Array.data1[i], SRM_ADC_Data_Array.data2[i], SRM_ADC_Data_Array.data3[i], 
							SRM_ADC_Data_Array.data4[i], SRM_ADC_Data_Array.data5[i], SRM_ADC_Data_Array.data6[i], SRM_ADC_Data_Array.data7[i], SRM_ADC_Data_Array.data8[i]);
		SendOutputBuffer(COMM.port);
	}
}

void SRM_AllChannels_Conversion(void)
{
	InitADC();
	//Turn on sync mode
	//Setup_Sync_Mode(SET);
	//SRM_SYNC_LOW;
	//Timer 2 init for SRM: 1ms timer
	MX_TIM2_Init_SRM();
	/* Timer setup and start */
	timer2_SRM_ON = 1;
	__HAL_TIM_CLEAR_FLAG(&htim2, TIM_IT_UPDATE);	//clear flag odmah kako ne bi usao u interrupt
	HAL_TIM_Base_Start_IT(&htim2); //pokreni tajmer
	timer2_interrupt=1;
	for(int i = 0; i < SRM_DATA_ARRAY_SIZE; i++)
	{
		while(timer2_interrupt==0);
		timer2_interrupt=0;
		SRM_Multiple_Conversions();
		//SRM_SYNC_HIGH;
		//SRM_Multiple_Reads();
		SRM_Write_Data_ToArray(i);
		//SRM_SYNC_LOW;
	}
	__HAL_TIM_CLEAR_FLAG(&htim2, TIM_IT_UPDATE);
	HAL_TIM_Base_Stop_IT(&htim2);
	timer2_SRM_ON = 0;
	Setup_Sync_Mode(RESET);
	SRM_Send_Data();
}

unsigned int SRM_ADC_GetData(void)
{
	unsigned int retVal = MAIN_OK;
	unsigned int d = 0;
	unsigned int data1 = 0, data2 = 0, data = 0, registerContent;
	unsigned int channel;
	unsigned int i;
	
	#if 0
	channel = SRM1_ADC1;
	Read_ADC_Chip_Revision(channel);
	#endif
	
	#if 0
	SRM_Write_ADC_Byte(SRM1_ADC1, 0x70);
	d = SRM_Read_ADC_Byte(SRM1_ADC1);    
	sprintf(OutputBuffer, "SRM1 ADC1 CH0 Conversion Time Register: 0x%X", d);
	#endif
	
	#if 0
	channel = SRM1_ADC1;
	Read_ADC_IOPort_Config(channel);
	#endif
	
	#if 0
	SRM_Write_ADC_Byte(SRM1_ADC1, 0x44);
	d = SRM_Read_ADC_Byte(SRM1_ADC2);    
	sprintf(OutputBuffer, "SRM1 ADC1 ADC Status Register: 0x%X", d);
	#endif
		
	//Single conversion AD7732 test
	if(InputBuffer[4] == 'S')
	{

	}		
	
	//Continuous conversion AD7732
	if(InputBuffer[4] == 'C')
	{	
		SRM_AllChannels_Conversion();	
	}
	return retVal;
}

















