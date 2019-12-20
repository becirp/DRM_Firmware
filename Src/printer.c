#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bios.h"
#include "control.h"
#include "comm.h"
#include "lang.h" 
#include "adc.h"

extern void HAL_Delay();
extern void USART1_UART_Init_printer_comm(void);
extern void MX_USART1_UART_Init(void);
extern unsigned int check_for_stop_button();

#define PRINT_NUMERICAL_RESULTS 'N'
#define PRINT_GRAPHICAL_RESULTS 'G'
#define PRINT_RMO_RESULTS       'R'
#define PRINT_EMPTY_LINES       'E'

#define VELIKI_PRINTER 1

extern unsigned char PrintBuffer[52];
extern void ClearPrintBuffer(void);

void read_dummy(void)
{
int i=0;
for(i=0; i<5; i++)
{
    //USART_Receive();
    HAL_Delay(1);
}
}

void set_printer_comm(void)
{
		USART1_UART_Init_printer_comm();
}

extern void putchar_printer(unsigned char data);

void PrinterParamters(unsigned char font, unsigned char bold, unsigned char italic, unsigned char underline, unsigned char reverse){
unsigned char i;
ClearPrintBuffer();
//delay_ms(1000);
for(i=0; i<10; i++)
{
    //putchar_printer(0x00);
    //delay_ms(250);
    putchar_printer(font);
    HAL_Delay(5);
}
//j=bold;
//j=italic;
//j=underline;
//j=reverse;
//putchar_printer(bold);
//putchar (italic);
//putchar_printer(underline);
//putchar_printer(reverse);
read_dummy();
}

#if 1
unsigned char PrintNewLine(unsigned int starting_position)
{
unsigned int i;
unsigned char retVal=1;
unsigned int Repeat=20;

read_dummy();

//chek if key stop pressed:
if(((check_for_stop_button())!=MAIN_OK)){goto stp;}  

#if 0
prijem_OK=0;
while((prijem_OK==0)&(Repeat>0))
{
    putchar_printer(0x18);
    retVal=USART_Receive();
    if(prijem_OK==0){delay_ms(5); for(i=0; i<10; i++){putchar_printer(0x00); delay_us(100);} delay_ms(5);}
    if(retVal==0x00){prijem_OK=0; delay_ms(5);}   
    Repeat--; 
}
#endif

retVal=0x00;
//bilo 200:
HAL_Delay(50);

//if(Repeat==0)   {return OPEN_HEAD; }

if(retVal==0x00){retVal=PRINTER_OK;}

//if(retVal!=PRINTER_OK)
//Tri kriticne greske: 1, 3 i 4
//Bit Status 0 1
//0 Near end Logic level is low Logic level is high
//1 Paper Present Absent
//2 Temperature Not too hot Head too hot to print
//3 Head Closed Open
//4 Cutter No error Error
//5 Rx error No error Rx error
//6 Buffer Not full. Full (less than 16 bytes)
//7 Always 1.                     
//if((retVal&0b00001010)!=0x00){ return retVal;}

HAL_Delay(10);

for(i=0; i<starting_position; i++)
{
    putchar_printer(' ');  
    HAL_Delay(1);                     
}                     
//Veliki printer:
#if (VELIKI_PRINTER)  
for(i=0; i<(51-starting_position); i++)
{
    if(PrintBuffer[i]>=32)
    {
        putchar_printer(PrintBuffer[i]);  
        HAL_Delay(1);                              
    }       
}
#else
for(i=0; i<(36-starting_position); i++)
{
    if(PrintBuffer[i]>=32)
    {
        putchar_printer(PrintBuffer[i]);              
        HAL_Delay(1);              
    }       
}
#endif                       
putchar_printer(0x0A); //Line Feed                           
ClearPrintBuffer(); 
HAL_Delay(100);                                                            
                                                            
return PRINTER_OK;  

stp:
return KEY_STOP; 
}
#endif

void PrintEmptyLine(){
int i, j;
j=0;
#if (VELIKI_PRINTER) 
for(i=0; i<51;i++){
        if(j==0){PrintBuffer[i]='_';}
        else{PrintBuffer[i]=' ';}
        j++;
        if(j==2){j=0;}
}
#else
for(i=0; i<36;i++){
        if(j==0){PrintBuffer[i]='_';}
        else{PrintBuffer[i]=' ';}
        j++;
        if(j==2){j=0;}
}
#endif
PrintNewLine(0);
PrintNewLine(0);                     
}

unsigned char PRINT_Header(unsigned char *DevVer)
{
unsigned int i;
unsigned char PrinterStatus; 

//for(i=0; i<10; i++){putchar_printer(0x00); delay_us(100);}

read_dummy();

PrinterParamters(NORMAL, BOLD_OFF, ITALIC_OFF, UNDERLINE_OFF, REVERSE_OFF);

//LINE FEED:
putchar_printer(0x0A); //Line Feed;

//PrinterParamters(NORMAL, BOLD_OFF, ITALIC_OFF, UNDERLINE_OFF, REVERSE_OFF);  

//if((PrinterStatus=PrintNewLine(0))!=PRINTER_OK){return PrinterStatus;}

//LINE FEED:
//putchar_printer(0x0A); //Line Feed 

//read_dummy();

//if((PrinterStatus=PrintNewLine(0))!=PRINTER_OK){return PrinterStatus;}

//For some reason thermal printer here requires 2x same command:
//for(i=0; i<2; i++){PrinterParamters(NORMAL, BOLD_OFF, ITALIC_OFF, UNDERLINE_OFF, REVERSE_OFF);}

PrinterParamters(NORMAL, BOLD_OFF, ITALIC_OFF, UNDERLINE_OFF, REVERSE_OFF);

//TEST RAPORT:
strcpy(PrintBuffer, activetext.TestRaport);

#if (VELIKI_PRINTER) 
if((PrinterStatus=PrintNewLine(20))!=PRINTER_OK){return PrinterStatus;} 
#else
if((PrinterStatus=PrintNewLine(12))!=PRINTER_OK){return PrinterStatus;} 
#endif

//PrinterParamters(NORMAL, BOLD_OFF, ITALIC_OFF, UNDERLINE_OFF, REVERSE_OFF);

//Circuit Breaker Analyzer & Timer
strcpy(PrintBuffer, activetext.CircuitBreakerAnalyzerAndTimer);

#if (VELIKI_PRINTER)
if((PrinterStatus=PrintNewLine(10))!=PRINTER_OK){return PrinterStatus;}  
#else
if((PrinterStatus=PrintNewLine(2))!=PRINTER_OK){return PrinterStatus;}
#endif

//PrinterParamters(NORMAL, BOLD_OFF, ITALIC_OFF, UNDERLINE_OFF, REVERSE_OFF);

//CAT XXXX
sprintf(PrintBuffer, "CAT %c%c%c%c\n", DevVer[0], DevVer[1], DevVer[2], DevVer[3]);

#if (VELIKI_PRINTER)
if((PrinterStatus=PrintNewLine(23))!=PRINTER_OK){} 
#else
if((PrinterStatus=PrintNewLine(15))!=PRINTER_OK){} 
#endif

//CHANGE FONT:
//PrinterParamters(NORMAL, BOLD_OFF, ITALIC_OFF, UNDERLINE_OFF, REVERSE_OFF);

//if((PrinterStatus=PrintNewLine(0))!=PRINTER_OK){}

//LINE FEED:
putchar_printer(0x0A); //Line Feed;

//CHANGE FONT:
PrinterParamters(LOW, BOLD_OFF, ITALIC_OFF, UNDERLINE_OFF, REVERSE_OFF);

//DATE & TIME:
//SVE SE OKO OVOGA VRTI!? Zbog toga ne provjeravam u ovome dijelu priner status; 
sprintf(PrintBuffer, "%s:%02d-%02d-%02d %s:%02d:%02d:%02d", activetext.Date, TIMEDATE.dd, TIMEDATE.mm, TIMEDATE.yy, activetext.Time, TIMEDATE.hh, TIMEDATE.min, TIMEDATE.sec); 

#if (VELIKI_PRINTER) 
if((PrinterStatus=PrintNewLine(12))!=PRINTER_OK){}
#else
if((PrinterStatus=PrintNewLine(4))!=PRINTER_OK){}  
#endif

//NEW LINE x2 
//for(i=0; i<2; i++){if((PrinterStatus=PrintNewLine(0))!=PRINTER_OK){}}

//LINE FEED:
putchar_printer(0x0A); //Line Feed;  

//CHANGE FONT:
PrinterParamters(NORMAL, BOLD_OFF, ITALIC_OFF, UNDERLINE_OFF, REVERSE_OFF);

//*****************************
//****** BREAKER DATA:  *******
//*****************************
strcpy(PrintBuffer, activetext.BreakerData);     
      
if((PrinterStatus=PrintNewLine(0))!=PRINTER_OK){return PrinterStatus;}

//CHANGE FONT:
PrinterParamters(LOW, BOLD_OFF, ITALIC_OFF, UNDERLINE_OFF, REVERSE_OFF);

//STATION:
sprintf(PrintBuffer, "%s: %s", activetext.Station,  BDATA.StationName);
for(i=0; i<2; i++){if((PrinterStatus=PrintNewLine(0))!=PRINTER_OK){return PrinterStatus;}}  

//BREAKER ID:
sprintf(PrintBuffer, "%s: %s", activetext.BreakerID,  BDATA.BreakerID);
for(i=0; i<2; i++){if((PrinterStatus=PrintNewLine(0))!=PRINTER_OK){return PrinterStatus;}}

//COMPARTMENT:
sprintf(PrintBuffer, "%s: %s", activetext.Compartment,  BDATA.Compartment);
for(i=0; i<2; i++){if((PrinterStatus=PrintNewLine(0))!=PRINTER_OK){return PrinterStatus;}}

//SERIAL NUMBER:
sprintf(PrintBuffer, "%s: %s", activetext.SerialNumber,  BDATA.SerialNumer);
for(i=0; i<2; i++){if((PrinterStatus=PrintNewLine(0))!=PRINTER_OK){return PrinterStatus;}}

//MANUFACTURER:
sprintf(PrintBuffer, "%s: %s", activetext.Manufacturer,  BDATA.Manufacturer);
for(i=0; i<2; i++){if((PrinterStatus=PrintNewLine(0))!=PRINTER_OK){return PrinterStatus;}}

//BREAKER TYPE:
sprintf(PrintBuffer, "%s: %s", activetext.BreakerType,  BDATA.BreakerType);
for(i=0; i<3; i++){if((PrinterStatus=PrintNewLine(0))!=PRINTER_OK){return PrinterStatus;}}

//CHANGE FONT:
PrinterParamters(NORMAL, BOLD_OFF, ITALIC_OFF, UNDERLINE_OFF, REVERSE_OFF);

//**************************
//****** TEST DATA:  *******
//**************************
strcpy(PrintBuffer, activetext.TestData);           
if((PrinterStatus=PrintNewLine(0))!=PRINTER_OK){return PrinterStatus;}

//CHANGE FONT:
PrinterParamters(LOW, BOLD_OFF, ITALIC_OFF, UNDERLINE_OFF, REVERSE_OFF);

//TYPE OF  TEST:
sprintf(PrintBuffer, "%s: %s", activetext.TypeOfTest,  TDATA.TypeOfTest);
for(i=0; i<2; i++){if((PrinterStatus=PrintNewLine(0))!=PRINTER_OK){return PrinterStatus;}}

//COMPANY NAME:
sprintf(PrintBuffer, "%s: %s", activetext.CompanyName,  TDATA.CompanyName);
for(i=0; i<2; i++){if((PrinterStatus=PrintNewLine(0))!=PRINTER_OK){return PrinterStatus;}}

//OPERATER:
sprintf(PrintBuffer, "%s: %s", activetext.Operater,  TDATA.Operater);
for(i=0; i<3; i++){if((PrinterStatus=PrintNewLine(0))!=PRINTER_OK){return PrinterStatus;}}

//CHANGE FONT:
PrinterParamters(NORMAL, BOLD_OFF, ITALIC_OFF, UNDERLINE_OFF, REVERSE_OFF);

//COMMENTS:
strcpy(PrintBuffer, activetext.Comments);  
for(i=0; i<2; i++){if((PrinterStatus=PrintNewLine(0))!=PRINTER_OK){return PrinterStatus;}}

//CHAGNE FONT:
PrinterParamters(LOW, BOLD_OFF, ITALIC_OFF, UNDERLINE_OFF, REVERSE_OFF);
for(i=0; i<2; i++){if((PrinterStatus=PrintNewLine(0))!=PRINTER_OK){return PrinterStatus;}}

for (i=0; i<5; i++){PrintEmptyLine();}

for(i=0; i<3; i++){if((PrinterStatus=PrintNewLine(0))!=PRINTER_OK){return PrinterStatus;}}
  
return PRINTER_OK;
}

unsigned char PrintResults(unsigned int PrintingLoc, unsigned char *DevVer){
int i;
unsigned char PrinterStatus;
unsigned int BrojRezultata;
unsigned int UkupanBrojRez;
unsigned char parameter1, parameter2, parameter3, parameter4;
unsigned char PrintajPraznuLiniju;
unsigned char PrintajRezultat;  

//PRINT HEADER:
if(((PrinterStatus=PRINT_Header(DevVer))!=PRINTER_OK)){return PrinterStatus;}
}

unsigned int PrintResults_Comm(unsigned char results_to_print)
{  
unsigned int retVal=MAIN_NOK;
unsigned int i;
unsigned int check;
unsigned char DevVer[4];
unsigned int  printing_location;

switch(results_to_print)
{
    case PRINT_NUMERICAL_RESULTS:			
    case PRINT_RMO_RESULTS:
        check=check_if_digit(5, 7);
        if(check==MAIN_OK)
        {                    
            //GET PRINTING LOCATION:             
            printing_location=string_to_int(5, 7);
            //GET DEVICE INFO:
            DevVer[0]=InputBuffer[8];
            DevVer[1]=InputBuffer[9];
            DevVer[2]=InputBuffer[10];
            DevVer[3]=InputBuffer[11];
            //SET PRINTER COMMUNICATION:  
            set_printer_comm();
            //START PRINTING NUMERICAL RESULTS:                                                                                                                                                                                                                                                                         
            check=PrintResults(printing_location, DevVer);            
        }  
        else                                        
        {                                     
            return retVal;
        }
    break;             
    case PRINT_EMPTY_LINES: 
            //SET PRINTER COMMUNICATION:  
            set_printer_comm();
            for(i=0; i<7; i++){check=PrintNewLine(0);}                                             
    break;
    case PRINT_GRAPHICAL_RESULTS: 
		#if 0
            //SET PRINTER COMMUNICATION:  
            check=check_if_digit(5, 7);  
            //GET PRINTING LOCATION:             
            printing_location=string_to_int(5, 7);
            if(check==MAIN_OK)
            {
            set_printer_comm();
            if((InputBuffer[4]>=CAT03)&&(InputBuffer[4]<=CAT126))
            {         if(InputBuffer[4]>=CAT60)
                      {
                        check=Print_Graphics(CAT6X, InputBuffer[4], printing_location);
                      }
                      else
                      {
                        check=Print_Graphics(CAT3X, InputBuffer[4], printing_location);
                      }                                                                                                                                                     
            }  
            }
		#endif
    break;					
}   

//ENABLE PRINTER:
enable_printer_communication();
//LINE FEED
putchar_printer(0x0A); 
                                                                               
if((check==PRINTER_OK)||(check==KEY_STOP)){retVal=MAIN_OK;}
else if(check==0xA3) {retVal=1;}
else if(check==0xA9) {retVal=2;}              
else                 {retVal=3;}
             
//SlaveSelect(NO_SLAVE);
disable_printer_communication();

//PORTD.7=1; //Switch commmunication to Control Board!             
MX_USART1_UART_Init();

return retVal;
}