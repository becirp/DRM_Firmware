#include <stdio.h>
#include <bios.h>

#include "adc.h"
#include "comm.h"
#include "control.h"

//CALIB_STRUCT EECALIB;

//OVO DODATI PREKO .h fileova:
//extern unsigned int check_if_digit(unsigned char first, unsigned char last);
//extern unsigned int string_to_int(unsigned char first, unsigned char last);
//extern char InputBuffer[MAX_COMM_BUFF_LENGHT];
//extern void OK_Response(void);
//extern char OutputBuffer[MAX_COMM_BUFF_LENGHT];
///////////////////////////////////////////////

//Analogni kanali:
//
//Citanje offseta: mURAXY
//Citanje gaina: mVRAXY
//Gdje je X – 1, 2, 3, 4, 5 ili 6 za kanal, a Y – 4 (0.5 V), 3 (2.5 V), 2 (60 V) ili 1 (300 V)
//
//Upis offseta: mUWAXYOOOOOS
//Upis gaina: mVWAXYGGGGG
//Gdje je OOOOO – pet cifara za offset, S – znak, GGGGG – pet cifara za gain
//
//Strujni kanali:
//
//Citanje offseta: mURCX1
//Citanje gaina: mVRCX1
//Kod starih uredjaja je bilo X – 1 (Close), 2 (Open1), 3 (Open2), 4 (Open3)
//Kod novih smo dogovorili da idu prvo tri opena, pa tri close-a, znaci: X – 1 (Open1), 2 (Open2), 3 (Open3), 4 (Close1), 5 (Close2), 6 (Close3)
//
//Upis offseta: mUWCX1OOOOOS
//Upis gaina: mVWCX1GGGGG
//Gdje je OOOOO – pet cifara za offset, S – znak, GGGGG – pet cifara za gain
//
//Takodje se koriste naredbe:
//
//mUUA – Citanje iz EEPROM-a
//mUPA – Upis u EEPROM
    

#define ANALOG_CH_TYPE 'A'
#define CURRENT_CH_TYPE 'C'
#define LINEAR_TRANSD_CH_TYPE 'L'

#define GAIN_COEF_TYPE 'V'
#define OFFSET_COEF_TYPE 'U'
 
#define CALIB_GAIN_MIN         9000
#define CALIB_GAIN_MAX        11000
#define CALIB_OFFSET_MAX       2000
#define CALIB_OFFSET_MIN          0
#define CALIB_DEFAULT_GAIN   (unsigned int) 10000
#define CALIB_DEFAULT_OFFSET (unsigned int)     0
#define CALIB_DEFAULT_SIGN   (unsigned char) '+'
#define CALIB_SIGN_PLUS      (unsigned char) '+'
#define CALIB_SIGN_MINUS     (unsigned cahr) '-'

unsigned int Store_Calib_Coef_to_EEPROM_Comm(void)
{
    //eeprom_write_block(&CALIB, &EECALIB, sizeof(CALIB_STRUCT));   
	  //TODO: store to SD CARD!
    return MAIN_OK;
}

void check_calibration_coeficients(void)
{
char i, j;

    for(i=0; i<NUMBER_OF_ANALOG_CHS; i++)
    {        
        for(j=0; j<NUMBER_OF_ANALOG_CH_RANGES; j++)
        {
            if(CALIB.analogch_gain[i][j]>CALIB_GAIN_MAX)
            {
                CALIB.analogch_gain[i][j]=CALIB_DEFAULT_GAIN;               
            }                                          
            if(CALIB.analogch_offset[i][j]>CALIB_OFFSET_MAX)
            {
                CALIB.analogch_offset[i][j]=CALIB_DEFAULT_OFFSET;
            }
        }                
    }
    
    for(i=0; i<NUMBER_OF_CURRENT_CHS; i++)
    {
        if(CALIB.currentch_gain[i]>CALIB_GAIN_MAX)
        {
            CALIB.currentch_gain[i]=CALIB_DEFAULT_GAIN;
        }                                          
        if(CALIB.currentch_offset[i]>CALIB_OFFSET_MAX)
        {
            CALIB.currentch_offset[i]=CALIB_DEFAULT_OFFSET;
        }                
    }
    
    for(i=0; i<NUMBER_OF_LT_CHS; i++)
    {
        if(CALIB.linear_tr_ch_gain[i]>CALIB_GAIN_MAX)
        {
            CALIB.linear_tr_ch_gain[i]=CALIB_DEFAULT_GAIN;
        }                                          
        if(CALIB.linear_tr_ch_offset[i]>CALIB_OFFSET_MAX)
        {
            CALIB.linear_tr_ch_offset[i]=CALIB_DEFAULT_OFFSET;
        }                
    }    
    
}

unsigned int Read_Calib_Coef_from_EEPROM_Comm(void)
{

    //eeprom_read_block(&CALIB, &EECALIB, sizeof(CALIB_STRUCT));
    //TODO: read from SD CARD!
    check_calibration_coeficients();
                
    return MAIN_OK;
}       

//extern void putcharB(unsigned char);

unsigned char get_range(unsigned char channel_range)
{
unsigned char retVal;
    switch(channel_range)
    {
        case '4': retVal=ANALOG_CH_RANGE_LL; break;
        case '3': retVal=ANALOG_CH_RANGE_LH; break;
        case '2': retVal=ANALOG_CH_RANGE_HL; break;
        case '1': retVal=ANALOG_CH_RANGE_HH; break;
    }     
      
    retVal-='0';
    
return retVal;
}

unsigned int Write_Calib_Coef_Comm(unsigned char coef_type, unsigned char channel_type, unsigned char channel_number, unsigned int channel_range)
{
unsigned int retVal=MAIN_NOK;
unsigned int aux_gain;
unsigned int aux_offset;
unsigned int aux_sign;
unsigned int check;  

if(coef_type==GAIN_COEF_TYPE)
{
    if(channel_type==ANALOG_CH_TYPE)
    {       
        if(channel_number>='0'){channel_number-='0';}       
        channel_range=get_range(channel_range);
        if(((channel_number>0)&&(channel_number<=NUMBER_OF_ANALOG_CHS))&&(((channel_number>=0)&&(channel_range<NUMBER_OF_ANALOG_CH_RANGES))))
        {                          
            check=check_if_digit(6, 10);            
            if(check==MAIN_OK)
            {                             
                aux_gain=string_to_int(6, 10);                 
                if((aux_gain>=CALIB_GAIN_MIN)&&(aux_gain<=CALIB_GAIN_MAX))
                {                                                                    
                    CALIB.analogch_gain[channel_number-1][channel_range]=aux_gain;                       
                    //store_calib_coef_to_eeprom(); 
                    OK_Response();
                    retVal=MAIN_OK;
                } 
            }                
        }            
    }   
    if(channel_type==CURRENT_CH_TYPE)
    {      
        if(channel_number>='0'){channel_number-='0';}          
        if(((channel_number>0)&&(channel_number<=NUMBER_OF_CURRENT_CHS))&&((channel_range=='1')))
        {                  
            check=check_if_digit(6, 10);
            if(check==MAIN_OK)
            {         
                aux_gain=string_to_int(6, 10); 
                if((aux_gain>=CALIB_GAIN_MIN)&&(aux_gain<=CALIB_GAIN_MAX))
                {                                                                    
                    CALIB.currentch_gain[channel_number-1]=aux_gain;                       
                    //store_calib_coef_to_eeprom(); 
                    OK_Response();
                    retVal=MAIN_OK;
                } 
            }                
        }            
    }            
    if(channel_type==LINEAR_TRANSD_CH_TYPE)
    {      
        if(channel_number>='0'){channel_number-='0';}          
        if(((channel_number>0)&&(channel_number<=NUMBER_OF_LT_CHS))&&((channel_range=='1')))
        {                  
            check=check_if_digit(6, 10);
            if(check==MAIN_OK)
            {         
                aux_gain=string_to_int(6, 10); 
                if((aux_gain>=CALIB_GAIN_MIN)&&(aux_gain<=CALIB_GAIN_MAX))
                {                                                                    
                    CALIB.linear_tr_ch_gain[channel_number-1]=aux_gain;                       
                    //store_calib_coef_to_eeprom(); 
                    OK_Response();
                    retVal=MAIN_OK;
                } 
            }                
        }            
    }        
}

if(coef_type==OFFSET_COEF_TYPE)
{
    if(channel_type==ANALOG_CH_TYPE)
    {         
        if(channel_number>='0'){channel_number-='0';}        
        channel_range=get_range(channel_range);
        if(((channel_number>0)&&(channel_number<=NUMBER_OF_ANALOG_CHS))&&(((channel_range>=0)&&(channel_range<NUMBER_OF_ANALOG_CH_RANGES))))
        {                  
            check=check_if_digit(6, 10);
            
            if(check==MAIN_OK)
            {                              
                aux_offset=string_to_int(6, 10);       
                aux_sign=InputBuffer[11];
                            
                if((aux_offset>=CALIB_OFFSET_MIN)&&(aux_offset<=CALIB_OFFSET_MAX))
                {
                    if((aux_sign=='+')||(aux_sign=='-'))
                    {                                    
                        CALIB.analogch_offset[channel_number-1][channel_range]=aux_offset;
                        CALIB.analogch_sign[channel_number-1][channel_range]=aux_sign;
                        OK_Response();   
                        //store_calib_coef_to_eeprom();
                        retVal=MAIN_OK; 
                    }
                }                    
            }                
        }            
    }        
    if(channel_type==CURRENT_CH_TYPE)
    {            
        if(channel_number>='0'){channel_number-='0';}        
        if(((channel_number>0)&&(channel_number<=NUMBER_OF_CURRENT_CHS))&&((channel_range=='1')))
        {                  
            check=check_if_digit(6, 10);
            
            if(check==MAIN_OK)
            {                              
                aux_offset=string_to_int(6, 10);       
                aux_sign=InputBuffer[11];
                            
                if((aux_offset>=CALIB_OFFSET_MIN)&&(aux_offset<=CALIB_OFFSET_MAX))
                {
                    if((aux_sign=='+')||(aux_sign=='-'))
                    {                                    
                        CALIB.currentch_offset[channel_number-1]=aux_offset;
                        CALIB.currentch_sign[channel_number-1]=aux_sign;
                        OK_Response();   
                        //store_calib_coef_to_eeprom(); 
                        retVal=MAIN_OK;                             
                    }
                }                                 
            }                
        }            
    }    
    if(channel_type==LINEAR_TRANSD_CH_TYPE)
    {            
        if(channel_number>='0'){channel_number-='0';}        
        if(((channel_number>0)&&(channel_number<=NUMBER_OF_LT_CHS))&&((channel_range=='1')))
        {                  
            check=check_if_digit(6, 10);
            
            if(check==MAIN_OK)
            {                              
                aux_offset=string_to_int(6, 10);       
                aux_sign=InputBuffer[11];
                            
                if((aux_offset>=CALIB_OFFSET_MIN)&&(aux_offset<=CALIB_OFFSET_MAX))
                {
                    if((aux_sign=='+')||(aux_sign=='-'))
                    {                                    
                        CALIB.linear_tr_ch_offset[channel_number-1]=aux_offset;
                        CALIB.linear_tr_ch_sign[channel_number-1]=aux_sign;
                        OK_Response();   
                        //store_calib_coef_to_eeprom(); 
                        retVal=MAIN_OK;                             
                    }
                }                                 
            }                
        }            
    }        
}
 
return retVal;
}           

unsigned int Read_Calib_Coef_Comm(unsigned char coef_type, unsigned char channel_type, unsigned char channel_number, unsigned int channel_range)
{
unsigned int retVal=MAIN_NOK;
unsigned int aux_gain;
unsigned int aux_offset;
unsigned int aux_sign;

if(coef_type==GAIN_COEF_TYPE)
{
    if((channel_type==ANALOG_CH_TYPE)||(channel_type==CURRENT_CH_TYPE)||(channel_type==LINEAR_TRANSD_CH_TYPE))
    {                
        if(channel_number>='0'){channel_number-='0';}        
                if((channel_type==ANALOG_CH_TYPE)&&((channel_number>0)&&(channel_number<=NUMBER_OF_ANALOG_CHS)))
                {                                    
                channel_range=get_range(channel_range);                    
                    if((channel_range>=0)&&(channel_range<NUMBER_OF_ANALOG_CH_RANGES))
                    {
                    aux_gain=CALIB.analogch_gain[channel_number-1][channel_range];
                    retVal=MAIN_OK;
                    }
                }
                if((channel_type==CURRENT_CH_TYPE)&&((channel_number>0)&&(channel_number<=NUMBER_OF_CURRENT_CHS)))
                {
                   aux_gain=CALIB.currentch_gain[channel_number-1];    
                   retVal=MAIN_OK;
                }                    
                if((channel_type==LINEAR_TRANSD_CH_TYPE)&&((channel_number>0)&&(channel_number<=NUMBER_OF_LT_CHS)))
                {
                   aux_gain=CALIB.linear_tr_ch_gain[channel_number-1];    
                   retVal=MAIN_OK;
                }  
        if(retVal==MAIN_OK)
        {                
            sprintf(OutputBuffer, "Koef[%c][%c][%c]:%05u, Znak:+", InputBuffer[3], InputBuffer[4], InputBuffer[5], aux_gain);                                                                                                          
        }                
    }               
}
else if(coef_type==OFFSET_COEF_TYPE)
{
    if((channel_type==ANALOG_CH_TYPE)||(channel_type==CURRENT_CH_TYPE)||(channel_type==LINEAR_TRANSD_CH_TYPE))
    {             
    if(channel_number>='0'){channel_number-='0';}                                                                        
                if((channel_type==ANALOG_CH_TYPE)&&((channel_number>0)&&(channel_number<=NUMBER_OF_ANALOG_CHS)))
                {                                         
                channel_range=get_range(channel_range);
                    if((channel_range>=0)&&(channel_range<NUMBER_OF_ANALOG_CH_RANGES))
                    {   
                    aux_offset=CALIB.analogch_offset[channel_number-1][channel_range];   
                    aux_sign=CALIB.analogch_sign[channel_number-1][channel_range];    
                    retVal=MAIN_OK;
                    }
                }
                if((channel_type==CURRENT_CH_TYPE)&&((channel_number>0)&&(channel_number<=NUMBER_OF_CURRENT_CHS)))
                {
                   aux_offset=CALIB.currentch_offset[channel_number-1];
                   aux_sign=CALIB.currentch_sign[channel_number-1];
                   retVal=MAIN_OK;
                }
                if((channel_type==LINEAR_TRANSD_CH_TYPE)&&((channel_number>0)&&(channel_number<=NUMBER_OF_LT_CHS)))
                {
                   aux_offset=CALIB.linear_tr_ch_offset[channel_number-1];
                   aux_sign=CALIB.linear_tr_ch_sign[channel_number-1];
                   retVal=MAIN_OK;
                }                                            
        if(retVal==MAIN_OK)
        {                        
                sprintf(OutputBuffer, "Koef[%c][%c][%c]:%05u, Znak:%c", InputBuffer[3], InputBuffer[4], InputBuffer[5], aux_offset, aux_sign);
        }                                                                                                                              
    }               
}

return retVal;
}


#define MAX_INTEGER_VALUE 0xFFFF
#define MAX_SIGNED_INT_VALUE  32767
//#define MIN_SIGNED_INT_VALUE -32768 

unsigned int apply_offset_correction(unsigned int input_value, unsigned int offset, unsigned char offset_sign){
unsigned int retVal;
unsigned int offset2;

offset2=(MAX_INTEGER_VALUE-offset);
retVal=input_value;

#if 0
    sprintf(OutputBuffer, "OFFSET:%05u", offset);
    SendOutputBuffer('B');                                
    sprintf(OutputBuffer, "SIGN:%c", offset_sign);
    SendOutputBuffer('B');
#endif           

    if(offset_sign=='+')
    {        
        if(input_value<offset2)
        {
            retVal+=offset;
        }
        else{
            retVal=MAX_INTEGER_VALUE;
        }
    }
    else{
        if (input_value>offset)
        {
            retVal-=offset;            
        }                  
        else{
            retVal=0;
        }
    }        
return retVal;
}

signed int apply_offset_correction_current(signed int input_value, unsigned int offset, unsigned char offset_sign){
signed int retVal;
float aux_variable; 

aux_variable=(float)input_value;
    
if(offset_sign=='+')
{
    aux_variable+=(float)offset;
}
else
{
    aux_variable-=(float)offset;
}

#if 0
sprintf(OutputBuffer, "AUX_VAR=%f", aux_variable);
SendOutputBuffer('B');
sprintf(OutputBuffer, "OFFSET=%f", (float)offset);
SendOutputBuffer('B');
#endif

#if 1
if(aux_variable>MAX_SIGNED_INT_VALUE)
{
    aux_variable=MAX_SIGNED_INT_VALUE;
}
if(aux_variable<-MAX_SIGNED_INT_VALUE)
{
    aux_variable=-MAX_SIGNED_INT_VALUE;
}
#endif
        
retVal=(signed int)aux_variable;

return retVal;
}


float calculate_gained_value(unsigned int gain, float value, char signed_int_value)
{

float retVal;

retVal=(float)gain;
retVal/=10000.0;
retVal*=value;  
     
    if(signed_int_value)
    {
        if(retVal>MAX_SIGNED_INT_VALUE) {retVal=MAX_SIGNED_INT_VALUE;}  
        if(retVal<-MAX_SIGNED_INT_VALUE){retVal=-MAX_SIGNED_INT_VALUE;}
    }         
    else
    {                               
        if(retVal>MAX_INTEGER_VALUE){retVal=MAX_INTEGER_VALUE;}         
    }
    
return retVal;        
} 

void apply_calibration_to_analog_chs(void)
{   
unsigned int i;
float aux_variable;
unsigned char *p_range;
unsigned int *p_analog1tf;
       
//#define ACH_RANGE_LL 		(unsigned char) '0'
//#define ACH_RANGE_LH 		(unsigned char) '1'
//#define ACH_RANGE_HL	    (unsigned char) '2'
//#define ACH_RANGE_HH 		(unsigned char) '3'

#if 0
TESTPAR.analog1_gain=ACH_RANGE_HH;
TESTPAR.analog2_gain=ACH_RANGE_LH;
TESTPAR.analog3_gain=ACH_RANGE_HL;
TESTPAR.analog4_gain=ACH_RANGE_HH;
TESTPAR.analog5_gain=ACH_RANGE_LL;
TESTPAR.analog6_gain=ACH_RANGE_LH;

ADC_Results.ANCH[0]=1000;
ADC_Results.ANCH[1]=2000;
ADC_Results.ANCH[2]=3000;
ADC_Results.ANCH[3]=4000;
ADC_Results.ANCH[4]=6000;
ADC_Results.ANCH[5]=60000;

CALIB.analogch_gain[0][(TESTPAR.analog1_gain-'0')]=10000;
CALIB.analogch_gain[1][(TESTPAR.analog2_gain-'0')]=11000;
CALIB.analogch_gain[2][(TESTPAR.analog3_gain-'0')]=13000;
CALIB.analogch_gain[3][(TESTPAR.analog4_gain-'0')]=14000;
CALIB.analogch_gain[4][(TESTPAR.analog5_gain-'0')]=15000;
CALIB.analogch_gain[5][(TESTPAR.analog6_gain-'0')]=20000;

CALIB.analogch_offset[0][(TESTPAR.analog1_gain-'0')]=2000;
CALIB.analogch_offset[1][(TESTPAR.analog2_gain-'0')]=200;
CALIB.analogch_offset[2][(TESTPAR.analog3_gain-'0')]=300;
CALIB.analogch_offset[3][(TESTPAR.analog4_gain-'0')]=400;
CALIB.analogch_offset[4][(TESTPAR.analog5_gain-'0')]=500;
CALIB.analogch_offset[5][(TESTPAR.analog6_gain-'0')]=600;

CALIB.analogch_sign[0][(TESTPAR.analog1_gain-'0')]='-';
CALIB.analogch_sign[1][(TESTPAR.analog2_gain-'0')]='-';
CALIB.analogch_sign[2][(TESTPAR.analog3_gain-'0')]='+';
CALIB.analogch_sign[3][(TESTPAR.analog4_gain-'0')]='-';
CALIB.analogch_sign[4][(TESTPAR.analog5_gain-'0')]='+';
CALIB.analogch_sign[5][(TESTPAR.analog6_gain-'0')]='-';
#endif

p_range=&TESTPAR.analog1_gain;
p_analog1tf=&TESTPAR.analog1tf;

for(i=0; i<NUMBER_OF_ANALOG_CHS; i++)
{   


#if 0           
    sprintf(OutputBuffer, "OPSEG[%2u]=%c", i, *p_range);
    SendOutputBuffer('B');
    
    sprintf(OutputBuffer, "VRIJEDNOST[%2u]=%05u", i, ADC_Results.ANCH[i]);
    SendOutputBuffer('B');
#endif                 

#if 0
    sprintf(OutputBuffer, "GAIN[%2u]: %f", i, (float)aux_variable);
    SendOutputBuffer('B');                                
#endif
             
    aux_variable=calculate_gained_value(CALIB.analogch_gain[i][(*p_range-'0')], (float)ADC_Results.ANCH[i], 0);
                                                                                                                 
    #if 0
    //zamjenjeno sa gornjom funkcijom
    aux_variable=(float)CALIB.analogch_gain[i][(*p_range-'0')];
    aux_variable/=10000.0;    
    aux_variable*=ADC_Results.ANCH[i];        
    if(aux_variable>MAX_INTEGER_VALUE){aux_variable=MAX_INTEGER_VALUE;}
    #endif    
        
    ADC_Results.ANCH[i]=(unsigned int)aux_variable;
    
#if 0
    sprintf(OutputBuffer, "KALIBRISANO[%2u]: %f", i, (float)ADC_Results.ANCH[i]);
    SendOutputBuffer('B');                                
#endif    
    
    //if(*p_analog1tf==0)
    {   //case when current clamps are not used                                
        ADC_Results.ANCH[i]=apply_offset_correction(ADC_Results.ANCH[i], CALIB.analogch_offset[i][(*p_range-'0')], CALIB.analogch_sign[i][(*p_range-'0')]);
    }
    //else
    {
       //case when current clamps are used (auto zero correction)       
        //ADC_Results.ANCH[i]=apply_offset_correction(ADC_Results.ANCH[0], CALIB.analogch_CC_offset[0], CALIB.analogch_CC_sign[0])    
    }

#if 0
    sprintf(OutputBuffer, "KALIBRISANO2[%2u]: %f", i, (float)ADC_Results.ANCH[i]);
    SendOutputBuffer('B');                                
#endif    

    p_range++;
    p_analog1tf++;
} 


#if 1

#if 0
ADC_Results.CURRENT[0]=100;
ADC_Results.CURRENT[1]=200;
ADC_Results.CURRENT[2]=300;
ADC_Results.CURRENT[3]=400;
ADC_Results.CURRENT[4]=500;
ADC_Results.CURRENT[5]=600;
#endif

#if 0
//povesti racuna:
//ovo su rezultati sa AD konvertora:
case CURRCLOSING1:  pomVal=abs(ADC_Results.CURRENT[3]); break;
case CURRCLOSING2:  pomVal=abs(ADC_Results.CURRENT[4]); break;
case CURRCLOSING3:  pomVal=abs(ADC_Results.CURRENT[5]); break;
case CURRBREAKING1: pomVal=abs(ADC_Results.CURRENT[0]); break;
case CURRBREAKING2: pomVal=abs(ADC_Results.CURRENT[1]); break;
case CURRBREAKING3: pomVal=abs(ADC_Results.CURRENT[2]); break;
        
//ovo su prema sasi koeficijenti:
//0: OPEN1
//1: OPEN2
//2: OPEN3
//3: CLOSE1
//4: CLOSE2
//5: CLOSE3
#endif

for(i=0; i<NUMBER_OF_CURRENT_CHS; i++)
{   

#if 0                  
    sprintf(OutputBuffer, "VRIJEDNOST[%2u]=%f", i, (float)ADC_Results.CURRENT[i]);
    SendOutputBuffer('B');               
    sprintf(OutputBuffer, "GAIN[%2u]=%05u", i, CALIB.currentch_gain[i]);
    SendOutputBuffer('B');    
#endif                 

#if 0
    sprintf(OutputBuffer, "ODABRANI GAIN[%2u]: %f", i, (float)aux_variable);
    SendOutputBuffer('B');                                
#endif
            
    aux_variable=calculate_gained_value(CALIB.currentch_gain[i], (float)ADC_Results.CURRENT[i], 1);                                                    

    #if 0 
    //zamjenjeno sve sa gornjom funkcijom!
    aux_variable=(float)CALIB.currentch_gain[i];
    aux_variable/=10000.0;    
    aux_variable*=ADC_Results.CURRENT[i];        
    if(aux_variable>MAX_SIGNED_INT_VALUE) {aux_variable=MAX_SIGNED_INT_VALUE;}  
    if(aux_variable<-MAX_SIGNED_INT_VALUE){aux_variable=-MAX_SIGNED_INT_VALUE;}
    #endif
          
    ADC_Results.CURRENT[i]=(signed int)aux_variable; 
    
#if 0
    sprintf(OutputBuffer, "KALIBRISANO[%2u]: %f", i, (float)ADC_Results.CURRENT[i]);
    SendOutputBuffer('B');                     
#endif    
                                   
        ADC_Results.CURRENT[i]=apply_offset_correction_current(ADC_Results.CURRENT[i], CALIB.currentch_offset[i], CALIB.currentch_sign[i]);

#if 0
    sprintf(OutputBuffer, "KALIBRISANO2[%2u]: %f", i, (float)ADC_Results.CURRENT[i]);
    SendOutputBuffer('B');                                
#endif    
} 

#endif

for(i=0; i<NUMBER_OF_LT_CHS; i++)
{                    
                 
    aux_variable=calculate_gained_value(CALIB.linear_tr_ch_gain[i], (float)ADC_Results.LT[i], 1);                                                    
    
    #if 0        
    //Zamjenjeno sve godnjom funkcijom!   
    aux_variable=(float)CALIB.linear_tr_ch_gain[i];
    aux_variable/=10000.0;                            
    aux_variable*=ADC_Results.LT[i];        
    if(aux_variable>MAX_SIGNED_INT_VALUE) {aux_variable=MAX_SIGNED_INT_VALUE;}  
    if(aux_variable<-MAX_SIGNED_INT_VALUE){aux_variable=-MAX_SIGNED_INT_VALUE;}
    #endif
          
    ADC_Results.LT[i]=(signed int)aux_variable;      
    
    //treaba napisati ovako:
    ADC_Results.LT[i]=(signed int)calculate_gained_value(CALIB.linear_tr_ch_gain[i], (float)ADC_Results.LT[i], 1);
                                   
    ADC_Results.LT[i]=apply_offset_correction_current(ADC_Results.LT[i], CALIB.linear_tr_ch_offset[i], CALIB.linear_tr_ch_sign[i]);
    
} 


   
}

unsigned int Reset_Calibration_COMM(void)
{
unsigned int i, j;

    for(i=0; i<NUMBER_OF_ANALOG_CHS; i++)
    {                          
        for(j=0; j<NUMBER_OF_ANALOG_CH_RANGES; j++)
        {
            CALIB.analogch_gain[i][j]=CALIB_DEFAULT_GAIN;
            CALIB.analogch_offset[i][j]=CALIB_DEFAULT_OFFSET;
            CALIB.analogch_sign[i][j]=CALIB_DEFAULT_SIGN;
        }
    }                                     
    
    for(i=0; i<NUMBER_OF_CURRENT_CHS; i++)
    {
            CALIB.currentch_gain[i]=CALIB_DEFAULT_GAIN;
            CALIB.currentch_offset[i]=CALIB_DEFAULT_OFFSET;
            CALIB.currentch_sign[i]=CALIB_DEFAULT_SIGN;
    }       
    for(i=0; i<NUMBER_OF_LT_CHS; i++)
    {
            CALIB.linear_tr_ch_gain[i]=CALIB_DEFAULT_GAIN;
            CALIB.linear_tr_ch_offset[i]=CALIB_DEFAULT_OFFSET;
            CALIB.linear_tr_ch_sign[i]=CALIB_DEFAULT_SIGN;    
    }
    
return (Store_Calib_Coef_to_EEPROM_Comm());

}

//extern unsigned char trigger_from_current_clamps;

unsigned int Get_Active_Calibration_Coef_COMM(void)
{
unsigned char i;
unsigned char *p_range;
unsigned int *p_analog_ch_transfer_function;
unsigned int offset_value;
unsigned char sign_value;

p_range=&TESTPAR.analog1_gain;
p_analog_ch_transfer_function=&TESTPAR.analog1tf;

//Send active calibration coeficients for analog chs:
for(i=0; i<NUMBER_OF_ANALOG_CHS; i++)
{  
	    sprintf(OutputBuffer, "ACH%c_GAIN=%05u", (i+'1'), CALIB.analogch_gain[i][(*p_range-'0')]);
	    SendOutputBuffer(COMM.port);  
        if(*p_analog_ch_transfer_function!=0)
        {
            offset_value=CALIB.analogch_CC_offset[i];
            //if(offset_value>3000){offset_value=0;}
            sign_value=CALIB.analogch_CC_sign[i];        
        }
        else
        {
            offset_value=CALIB.analogch_offset[i][(*p_range-'0')];      
            sign_value=CALIB.analogch_sign[i][(*p_range-'0')];
        }
        #if 0
        //Only for first trip test:
        //First 3 analog CH's are for DC current
        if((i<3)&(trigger_from_current_clamps))
        {                          
               offset_value=CALIB.analogch_CC_offset[i];
               if(offset_value>3000){offset_value=0;}
               sign_value=CALIB.analogch_CC_sign[i];           
        }     
        else
        {         
                
                offset_value=CALIB.analogch_offset[i][(*p_range-'0')];      
                sign_value=CALIB.analogch_sign[i][(*p_range-'0')];
        }    
        #endif
        
	    sprintf(OutputBuffer, "ACH%c_OFFSET=%05u", (i+'1'), offset_value);
	    SendOutputBuffer(COMM.port);	
	    sprintf(OutputBuffer, "ACH%c_SIGN=%c", (i+'1'), sign_value);
	    SendOutputBuffer(COMM.port);
	    p_range++;
        p_analog_ch_transfer_function++;            
}
//Send active calibration coeficients for current chs:
for(i=0; i<NUMBER_OF_CURRENT_CHS; i++)
{   
//Ovim redosljedom idu koeficijenti:
//0: OPEN1
//1: OPEN2
//2: OPEN3
//3: CLOSE1
//4: CLOSE2
//5: CLOSE3 
    if(i<3)
    {
        sprintf(OutputBuffer, "CURR_OPEN%c_GAIN=%05u", (i+'1'), CALIB.currentch_gain[i]);
	    SendOutputBuffer(COMM.port);
        sprintf(OutputBuffer, "CURR_OPEN%c_OFFSET=%05u", (i+'1'), CALIB.currentch_offset[i]);
	    SendOutputBuffer(COMM.port);
        sprintf(OutputBuffer, "CURR_OPEN%c_SIGN=%c", (i+'1'), CALIB.currentch_sign[i]);
    	SendOutputBuffer(COMM.port);        
    }
    else
    {
        sprintf(OutputBuffer, "CURR_CLOSE%c_GAIN=%05u", (i+'1'), CALIB.currentch_gain[i]);
	    SendOutputBuffer(COMM.port);
        sprintf(OutputBuffer, "CURR_CLOSE%c_OFFSET=%05u", (i+'1'), CALIB.currentch_offset[i]);
	    SendOutputBuffer(COMM.port);
        sprintf(OutputBuffer, "CURR_CLOSE%c_SIGN=%c", (i+'1'), CALIB.currentch_sign[i]);
	    SendOutputBuffer(COMM.port);    
    }        
}

for(i=0; i<NUMBER_OF_LT_CHS; i++)
{    
        sprintf(OutputBuffer, "ANALOG_TR%c_GAIN=%05u",  (i+'1'), CALIB.linear_tr_ch_gain[i]);
	    SendOutputBuffer(COMM.port);
        sprintf(OutputBuffer, "ANALOG_TR%c_OFFSET=%05u", (i+'1'), CALIB.linear_tr_ch_offset[i]);
	    SendOutputBuffer(COMM.port);
        sprintf(OutputBuffer, "ANALOG_TR%c_SIGN=%c",     (i+'1'), CALIB.linear_tr_ch_sign[i]);
    	SendOutputBuffer(COMM.port);   
}
OK_Response();
return MAIN_OK;
}

//Get offset values for analog channels with acive current clamps:
void calculate_offset_for_CC_channels(void)
{   
unsigned char channel_number;
unsigned int offset_level;

 for(channel_number=0; channel_number<NUMBER_OF_ANALOG_CHS; channel_number++)
    {           	        
            CALIB.analogch_CC_offset[channel_number]=0;    
            read_analog_adc_for_trigger(EXT_TRIG_ANALOG1+channel_number);
            offset_level=ADC_Results.ANCH[channel_number];
            if(offset_level>(MAX_ADC_VALUE/2))
            {
                CALIB.analogch_CC_offset[channel_number]=offset_level-(MAX_ADC_VALUE/2);
                CALIB.analogch_CC_sign[channel_number]='-';                            
            }                              
            else
            {
                CALIB.analogch_CC_offset[channel_number]=(MAX_ADC_VALUE/2)-offset_level;
                CALIB.analogch_CC_sign[channel_number]='+';
            }
    }  
}