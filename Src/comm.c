// Standard Input/Output functions
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "fatfs.h"
#include "stdio.h"
#include "string.h"

#include "bios.h"
#include "adc.h"
#include "init.h"
//#include "comm.h"

unsigned char GetDataFromMemory;
unsigned char GMD;

char OutputBuffer[MAX_COMM_BUFF_LENGHT];
char InputBuffer[MAX_COMM_BUFF_LENGHT];

void putcharB(unsigned char byte)
{
		/*Configure GPIO pins : PC0-PC7 as outputs */		
	//Prebaceno u init.c
		GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_FIRST_BYTE;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(USB_DATA_PORT, &GPIO_InitStruct);
		
	  //todo: try with this one instead of HAL_GPIO_INIT:
		//GPIOC->OTYPER = GPIO_PIN_FIRST_BYTE;
		
		WAIT_FTDI_BCBUS1;
			
		//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
		GPIOC->ODR = byte;
			
		SET_FTDI_BC2_nRD;
					
		//add delay:
		__nop();
		__nop();
	  __nop();			
		
				__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	
		
		SET_FTDI_BC3_WR;			

		//add delay:
		__nop();			
	  __nop();			
		__nop();		

		__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	
			
	  CLR_FTDI_BC3_WR;

		//add delay:
		__nop();			
	  __nop();			
		__nop();	

		__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	
}

void my_putchar(unsigned char ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (unsigned char *)&ch, 1, 0xFFFF);   
}

unsigned char getcharB(void)
{		
	  unsigned char byte; 
	
	  /*Configure GPIO pins : PC0-PC7 as inputs */
		GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_FIRST_BYTE;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(USB_DATA_PORT, &GPIO_InitStruct);
	
	  //todo: try with this one instead of HAL_GPIO_INIT:
		//GPIOC->OTYPER = GPIO_PIN_FIRST_BYTE;
			
	  //Wait for FTDI BCBUS0 (RXF#) 
		WAIT_FTDI_BCBUS0;
			
		//Reset FTDI BCBUS3 (WR)
   	CLR_FTDI_BC3_WR;
			
		//add delay:
		__nop();			
	  __nop();			
		__nop();

		__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	
			
		//Reset FTDI BCBUS2 (RD#)
		CLR_FTDI_BC2_nRD;
		
				__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	

		byte=USB_DATA_PORT->IDR;	

		__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	
			
		SET_FTDI_BC2_nRD;
			
	  //add delay:
		__nop();			
	  __nop();			
		__nop();		

		__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	
				__nop();
		__nop();
	  __nop();	
		
		return byte;
}

void SendOutputBuffer(unsigned char port){
unsigned char BuffLen;
unsigned char i;
unsigned char data;
				
        if(port==COM_PORT_A)
				{	
				  //start char:					
					data=0x1B;										
					HAL_UART_Transmit(&huart1, &data, 1, 0xFFFF); 										
					//comm specific char:					
					data='m';					
					HAL_UART_Transmit(&huart1, &data, 1, 0xFFFF); 					
					
					BuffLen = strlen(OutputBuffer);
					for (i = 0; i < BuffLen; i++ )
					{                						
						data=OutputBuffer[i];						
						HAL_UART_Transmit(&huart1, &data, 1, 0xFFFF); 					
					}             
					//termination char:
					data=0x00;					
					HAL_UART_Transmit(&huart1, &data, 1, 0xFFFF); 										
        }
        else
				{   
					if(COMM.remote_enable==REMOTE_ENABLE){
					putcharB(0x1B);
					BuffLen = strlen(OutputBuffer);
					for (i = 0; i < BuffLen; i++ )
					{
							putcharB(OutputBuffer[i]);
					} 
					putcharB(0x0D);
					putcharB(0x0A);      
					}
        }
}

unsigned char my_getchar(void)
{
	unsigned char retVal;
	HAL_UART_Receive(&huart1, &retVal, 1, HAL_MAX_DELAY);	
	return retVal;
}
/********* UART i FTDI KOMUNIKACIJA ***********/
void ClearInputBuff(void){ unsigned char i;  for (i=0; i<MAX_COMM_BUFF_LENGHT; i++){InputBuffer[i]='0';} }

unsigned char GetInputBuffer(unsigned char port)
{
char byte, byte2, pointer, retVal;
pointer=1;         
byte=0;
byte2=0;
ClearInputBuff();
tryAgain:
do      
    {            
                               
        if(port==COM_PORT_A){byte=my_getchar();} //{byte=getchar_usartc0();}
        else                {byte=getcharB();}
       
    }      
    while(byte!=0x1B);             
		
if(port==COM_PORT_A){byte2=my_getchar();}
else                {byte2=getcharB();}

if (byte2!='m')
{
    goto tryAgain;     
}

InputBuffer[0]='m';      
do       
        {
          if(port==COM_PORT_A) {InputBuffer[pointer] = my_getchar();}                      
          else          			 {InputBuffer[pointer] = getcharB();}                                
          pointer++;
        }          
        while ( (pointer<MAX_COMM_BUFF_LENGHT) & (InputBuffer[pointer-1]!=0x00) );
				
if (InputBuffer[pointer-1]==0x00){
retVal=MAIN_OK;
}        
else {
retVal=MAIN_NOK;
}         

if (pointer>=MAX_COMM_BUFF_LENGHT){
retVal=MAIN_NOK;
}  

return retVal;
}

void wait_char_from_pc(unsigned char memory)
{
unsigned char status;

if(memory==0)
{
    sprintf(OutputBuffer, "GMD%c%c", InputBuffer[2], InputBuffer[3]);    
}           
else
{
    sprintf(OutputBuffer, "ready... waiting for 'A'");                
}    
SendOutputBuffer(COMM.port);
                    
//wait for char 'A' on port B to start data trasnfer:    
do{status=getcharB();}
while(status!='A');   
                                                                 
//send acknowledgement byte
putcharB(0x1B);
}

void send_test_data_to_pc(void)
{
unsigned int i;
unsigned int pomint;
unsigned char pomchar;
//unsigned int delay_time_ms=50;

sprintf(OutputBuffer, "T%05u", TESTPAR.test_duration); 
SendOutputBuffer(COMM.port);

//delay_ms(delay_time_ms);

sprintf(OutputBuffer, "P1%c%c%c%c%c%c%c%c%c%c%c",
                       TESTPAR.sampl_resol,
                       TESTPAR.op_sequence,
                       TESTPAR.time_base,
                       TESTPAR.mains_freq,
                       TESTPAR.travel_unit, 
                       TESTPAR.analog1_gain,
                       TESTPAR.analog2_gain,
                       TESTPAR.analog3_gain,
                       TESTPAR.analog4_gain,
                       TESTPAR.analog5_gain,
                       TESTPAR.analog6_gain
);                                                 
SendOutputBuffer(COMM.port);  

//delay_ms(delay_time_ms);

sprintf(OutputBuffer,"P2%c%c%c%c%c%c%c%c%c", 
                      TESTPAR.ext_trigg_src,                       
                      TESTPAR.ext_trigg_edg,                        
                      TESTPAR.break_units,                          
                      TESTPAR.transd_type,    
                      TESTPAR.transd_ch,
                      TESTPAR.o_coil_phase,
                      TESTPAR.ext_trigg_wtm,
                      TESTPAR.oper_mech,
                      TESTPAR.bs_grounded                         
);
SendOutputBuffer(COMM.port);

//delay_ms(delay_time_ms);  

sprintf(OutputBuffer,"P3%c%c%c%c%c%c",
                      TESTPAR.aux1_dry_wet,
                      TESTPAR.aux2_dry_wet,
                      TESTPAR.aux3_dry_wet,
                      TESTPAR.aux4_dry_wet,
                      TESTPAR.aux5_dry_wet,
                      TESTPAR.aux6_dry_wet                         
); 
                    
SendOutputBuffer(COMM.port);  

//delay_ms(delay_time_ms);  

sprintf(OutputBuffer, "P4%c%c%c%c%c%c", 
                    TESTPAR.analog1tf_unit[0],
                    TESTPAR.analog1tf_unit[1],
                    TESTPAR.analog1tf_unit[2],
                    TESTPAR.analog2tf_unit[0],
                    TESTPAR.analog2tf_unit[1],
                    TESTPAR.analog2tf_unit[2]
);

//delay_ms(delay_time_ms);  
                       
SendOutputBuffer(COMM.port);

sprintf(OutputBuffer, "P5%c%c%c%c%c%c", 
                    TESTPAR.analog3tf_unit[0],
                    TESTPAR.analog3tf_unit[1],
                    TESTPAR.analog3tf_unit[2],
                    TESTPAR.analog4tf_unit[0],
                    TESTPAR.analog4tf_unit[1],
                    TESTPAR.analog4tf_unit[2]
);

//delay_ms(delay_time_ms);  
                       
SendOutputBuffer(COMM.port);

sprintf(OutputBuffer, "P6%c%c%c%c%c%c", 
                    TESTPAR.analog5tf_unit[0],
                    TESTPAR.analog5tf_unit[1],
                    TESTPAR.analog5tf_unit[2],
                    TESTPAR.analog6tf_unit[0],
                    TESTPAR.analog6tf_unit[1],
                    TESTPAR.analog6tf_unit[2]
);

//delay_ms(delay_time_ms);  
                       
SendOutputBuffer(COMM.port);

for (pomchar='0'; pomchar<='9'; pomchar++){
switch (pomchar)
{
//KERIM JE TRAZIO DA SE OVAKO NAPRAVI:
    case '0': pomint=TESTPAR.analog5tf;     break;
    case '1': pomint=TESTPAR.analog1tf;     break;
    case '2': pomint=TESTPAR.analog2tf;     break;
    case '3': pomint=TESTPAR.analog3tf;     break;
    case '4': pomint=TESTPAR.analog4tf;     break;
    case '5': pomint=TESTPAR.transd1_tf;    break;
    case '6': pomint=TESTPAR.transd2_tf;    break;
    case '7': pomint=TESTPAR.transd3_tf;    break;
    case '8': pomint=TESTPAR.ext_trigg_lev; break;
    case '9': pomint=TESTPAR.analog6tf;     break;        
}
sprintf(OutputBuffer, "Q%c%05u", pomchar, pomint);                                
SendOutputBuffer(COMM.port);
//delay_ms(delay_time_ms);
}

sprintf(OutputBuffer, "RT%01u%05u", 1, TESTPAR.c_pulse_lenght);
SendOutputBuffer(COMM.port);
//delay_ms(delay_time_ms);
sprintf(OutputBuffer, "RT%01u%05u", 2, TESTPAR.o_pulse_lenght);
SendOutputBuffer(COMM.port); 
//delay_ms(delay_time_ms);                         
sprintf(OutputBuffer, "RT%01u%05u", 3, TESTPAR.trip_free_delay);
SendOutputBuffer(COMM.port);
//delay_ms(delay_time_ms);
sprintf(OutputBuffer, "RT%01u%05u", 4, TESTPAR.reclosing_delay);
SendOutputBuffer(COMM.port);
//delay_ms(delay_time_ms);
sprintf(OutputBuffer, "RT%01u%05u", 5, TESTPAR.oco_delay1);
SendOutputBuffer(COMM.port);
//delay_ms(delay_time_ms);
sprintf(OutputBuffer, "RT%01u%05u", 6, TESTPAR.oco_delay2);
SendOutputBuffer(COMM.port);
//delay_ms(delay_time_ms);

for(i=0; i<NUMBER_OF_LT_CHS; i++)
{
sprintf(OutputBuffer, "RK%01u%01u%05u", i, 1, TESTPAR.tr_lenght_mm[i]);
SendOutputBuffer(COMM.port);
//delay_ms(delay_time_ms);
sprintf(OutputBuffer, "RK%01u%01u%05u", i, 2, TESTPAR.tr_lenght_inch[i]);
SendOutputBuffer(COMM.port); 
//delay_ms(delay_time_ms);
sprintf(OutputBuffer, "RK%01u%01u%05u", i, 3, TESTPAR.tr_gain[i]);
SendOutputBuffer(COMM.port);
//delay_ms(delay_time_ms);
sprintf(OutputBuffer, "RK%01u%01u%05u", i, 4, TESTPAR.tr_offset[i]);
SendOutputBuffer(COMM.port);
//delay_ms(delay_time_ms);
}
}

extern unsigned int abs_16_bit(signed int x);

void send_measuring_samples_to_pc(void)
{
unsigned char channel;
uint16_t current_rectified[NUMBER_OF_CURRENT_CHS];

#if 0
    ADC_Results.ANCH[0]=ADC_Results.ANCH[6]/2;
    ADC_Results.ANCH[0]+=32767;
#endif
         
        //send data from main contacts:  
        for(channel=0; channel<NUMBER_OF_MAIN_CONTACTS; channel++)
        {                                 
            putcharB((unsigned char)(ADC_Results.MC[channel]>>8));
            putcharB((unsigned char)(ADC_Results.MC[channel]));
        }                    
        //send data from current channels:
        for(channel=0; channel<NUMBER_OF_CURRENT_CHS; channel++)
        {                               
						current_rectified[channel]=abs_16_bit(ADC_Results.CURRENT[channel]);
										
            putcharB((unsigned char)(current_rectified[channel]>>8));
            putcharB((unsigned char)(current_rectified[channel]));  
        }               
        //send data from analog channels:
        for(channel=0; channel<NUMBER_OF_ANALOG_CHS; channel++)
        {
            putcharB((unsigned char)(ADC_Results.ANCH[channel]>>8));
            putcharB((unsigned char)(ADC_Results.ANCH[channel]));  
        }      
        //send AUX channels:
        putcharB(ADC_Results.AUX);
        //send data from linear transducers:
        for(channel=0; channel<NUMBER_OF_LT_CHS; channel++)
        {
            putcharB((unsigned char)(ADC_Results.LT[channel]>>8));
            putcharB((unsigned char)(ADC_Results.LT[channel]));  
        }
        //send data from digital transducers:
        for(channel=0; channel<NUMBER_OF_DT_CHS; channel++)
        {
            putcharB((unsigned char)(ADC_Results.DTH[channel]>>8));
            putcharB((unsigned char)(ADC_Results.DTH[channel]));
            putcharB((unsigned char)(ADC_Results.DTL[channel]>>8));
            putcharB((unsigned char)(ADC_Results.DTL[channel]));  
        } 
}   

void send_measuring_samples_to_pc_DRM_test(unsigned char sensor, unsigned char measuring_channel)
{

switch(sensor)
{
    //Linear transducer:
    case 'L':
            putcharB((unsigned char)(ADC_Results.LT[measuring_channel]>>8));
            putcharB((unsigned char)(ADC_Results.LT[measuring_channel]));          
    break;
    //Digital transducer:
    case 'D':
        //send data from digital transducers:
            putcharB((unsigned char)(ADC_Results.DTH[measuring_channel]>>8));
            putcharB((unsigned char)(ADC_Results.DTH[measuring_channel]));
            putcharB((unsigned char)(ADC_Results.DTL[measuring_channel]>>8));
            putcharB((unsigned char)(ADC_Results.DTL[measuring_channel]));          
    break; 
    //DRM Voltage and Current channels
    case 'R':                                                
            putcharB((unsigned char)(ADC_Results.ANCH[measuring_channel]>>8));
            putcharB((unsigned char)(ADC_Results.ANCH[measuring_channel]));
    break;       
}       
}   

void send_data_transfer_over_status(void)
{
    //Send data transfer over status:    
    putcharB('D');
    putcharB('T');
    putcharB('S');
    putcharB('O');
    putcharB(0x0D);
    putcharB(0x0A);
    GetDataFromMemory=1; 
}


void tst_sprintf (void) {
  char buf [100];
  int n;
  int a,b;
  float pi;

  a = 123;
  b = 456;
  pi = 3.14159;

  n = sprintf (buf, "%f\n", 1.1);
  n += sprintf (buf+n, "%d\n", a);
  n += sprintf (buf+n, "%d %s %g", b, "---", pi);

  puts (buf);
}

void putchar_printer(unsigned char data)
{
	HAL_UART_Transmit(&huart1, &data, 1, 0xFFFF); 
}

void S_D8(void)
{
	for (unsigned int i=0; i<250; i++)
	{
				__nop();
	}
}

void S_D82(void)
{
	for (unsigned int i=0; i<100; i++)
	{
				__nop();
	}
}

unsigned char spie_master_tx_rx(unsigned char shOut)
{
  unsigned char shIn = 0;
  unsigned char i;
	
	//slave select USB flash drive:
	CLR_USB_FLASH_nCS;
	
	//wait for USB flash drive ready signal:	 
	while(HAL_GPIO_ReadPin(USB_FLASH_READY_PORT, USB_FLASH_READY_PIN)==1){}
	
  for(i = 0; i < 8; i++) {
    if (shOut > 127)  SET_USB_FLASH_MOSI; 
    else              CLR_USB_FLASH_MOSI; 
    S_D8();S_D8();
    if (HAL_GPIO_ReadPin(USB_FLASH_MISO_PORT, USB_FLASH_MISO_PIN)==1){shIn += 1;}    
    S_D8();S_D8();
    SET_USB_FLASH_SCK;
    S_D8();S_D8();    
    CLR_USB_FLASH_SCK;
    S_D8();S_D8();
    if (i != 7)	shIn <<= 1;
    shOut <<= 1;
  }
	
	S_D8(); S_D8();
	
	//slave select USB flash drive:
	SET_USB_FLASH_nCS;
	
	#if 0
	putcharB(shIn);
	#endif
	
  return shIn;
}

unsigned char spie_master_tx_fast(unsigned char shOut, unsigned char delay)
{
  unsigned char shIn = 0;
  unsigned char i;
	
	//slave select USB flash drive:
	CLR_USB_FLASH_nCS;
	
	//wait for USB flash drive ready signal:	 
	while(HAL_GPIO_ReadPin(USB_FLASH_READY_PORT, USB_FLASH_READY_PIN)==1){}
	
 for(i = 0; i < 8; i++) {
    if (shOut > 127)  SET_USB_FLASH_MOSI; 
    else              CLR_USB_FLASH_MOSI; 
    S_D82();S_D82();
    if (HAL_GPIO_ReadPin(USB_FLASH_MISO_PORT, USB_FLASH_MISO_PIN)==1){shIn += 1;}    
    S_D82();S_D82();
    SET_USB_FLASH_SCK;
    S_D82();S_D82();    
    CLR_USB_FLASH_SCK;
    S_D82();S_D82();
    if (i != 7)	shIn <<= 1;
    shOut <<= 1;
  }
		
	//slave select USB flash drive:
	SET_USB_FLASH_nCS;
	
  return shIn;
}