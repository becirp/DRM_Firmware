// Standard Input/Output functions
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "bios.h"
#include "comm.h"
#include "usb_flash_drive.h"
#include "adc.h"
#include "lang.h"

#define USB_STICK_DEBUG 0

//USB Stick

void delay_ms(unsigned int ms)
{
	HAL_Delay(ms);
}

unsigned char outbv[64];

void reset_flash_drive_MCU(void)
{
    
    SET_USB_FLASH_RST;
    HAL_Delay(25);     
    CLR_USB_FLASH_RST;
    HAL_Delay(25); 
    SET_USB_FLASH_RST; 
    HAL_Delay(500);   
}

void slave_select_usb_flash_drive(void)
{    
    CLR_USB_FLASH_nCS;
    #if 1
    HAL_Delay(1);
    #endif
    //bilo kasnjenje od 2ms    
}

void slave_deselect_usb_flash_drive(void)
{

    SET_USB_FLASH_nCS;
    //bilo kasnjenje od 2ms
    #if 1    
    HAL_Delay(1);         
    #endif
}

unsigned char ReadSPIs(unsigned char s,unsigned char b[])
{
    unsigned char t,i;                      
    
    slave_select_usb_flash_drive();
    spie_master_tx_rx('R');          
    t=spie_master_tx_rx(0);      
    if(t>s)
    {     
        //printf("\r\nOVF=%d\r\n",(int)t);
        t=s;
    }                  

    spie_master_tx_rx(t);
    for(i=0;i<t;i++)
    {
        b[i]=spie_master_tx_rx(0);
    }
    slave_deselect_usb_flash_drive(); 
    return(i);
}

unsigned char WriteSPIs(unsigned char s,unsigned char b[])
{
    volatile unsigned char i,j;
    j=s;          
    //SlaveSelectBezKasnjenja(STICK_SLAVE);
    //delay_ms(200);  
//    SPIstick('W');        
//    SPIstick(s);
    slave_select_usb_flash_drive();
    spie_master_tx_rx('W');        
    spie_master_tx_rx(s);        
    for(i=0;i<j;i++)
    {
        spie_master_tx_rx(b[i]);    
    }
        
    //SlaveSelectBezKasnjenja(NO_SLAVE);
    slave_deselect_usb_flash_drive(); 
    return(i);
}

void clrvn(void)
{
    unsigned char t1; 
    do
    {
    t1=ReadSPIs(64,&outbv[0]);
    }while(t1!=0);
}

unsigned char SendVNC1L(unsigned char c[],unsigned char s)
{
    unsigned char z;
    z=WriteSPIs(s, &c[0]);
    //c[s]=0; -Pravi problem kod upisa...  
    if(z!=s)
    {
        return(1);
    }
    else
    {
        return(0);
    }    
}

#if 1

unsigned char WriteSPIs_fast(unsigned char s,unsigned char b[])
{
    volatile unsigned char i;                
    
    #if 1             
    spie_master_tx_rx('W');                
    spie_master_tx_rx(s);          
    
    // Slave select =0 
    //PORTE.OUTCLR=PIN4;
    //delay_us(100);     
           
    for(i=0;i<s;i++)              
    {                  
        spie_master_tx_fast(b[i], 0);  
        //cini mi se da nema fule kada se salje samo jedan karakter: spie_master_tx_fast('A', 0);          
    }
        
    // Slave select =1
    //delay_us(100); 
    //PORTE.OUTSET=PIN4;
    #endif
 
    return(i);
}

#if 1
unsigned char SendVNC1L_fast(unsigned char array_to_send[],unsigned char number_of_bytes_to_send)
{
unsigned int leftover;
unsigned char z;    

    #if 0           
    leftover=number_of_bytes_to_send-60;
    delay_us(250);
    WriteSPIs_fast(10, &array_to_send[0]);
    delay_us(250);
    WriteSPIs_fast(10, &array_to_send[10]);
    delay_us(250);
    WriteSPIs_fast(10, &array_to_send[20]);
    delay_us(250);  
    WriteSPIs_fast(10, &array_to_send[30]);
    delay_us(250);  
    WriteSPIs_fast(10, &array_to_send[40]);
    delay_us(250);
    WriteSPIs_fast(10, &array_to_send[50]);
    delay_us(250);
    WriteSPIs_fast(leftover, &array_to_send[60]);  //dodaj +1 na leftover pa probaj tako! 
    delay_us(250);  
    return 0;                             
    #endif   
    #if 0
    WriteSPIs_fast(30, &array_to_send[0]);
    delay_us(300);
    WriteSPIs_fast(30, &array_to_send[30]);
    delay_us(300);
    WriteSPIs_fast((number_of_bytes_to_send-60), &array_to_send[60]);
    delay_us(300);        
    return 0;
    #endif  
    #if 1
    WriteSPIs_fast(40, &array_to_send[0]);
    delay_us(250);                
    WriteSPIs_fast((number_of_bytes_to_send-40), &array_to_send[40]);
    return 0;
    #endif    
    #if 0
    //slave_select_usb_flash_drive();
    z=WriteSPIs_fast(number_of_bytes_to_send, &array_to_send[0]);  
    //slave_deselect_usb_flash_drive(); 
    //c[s]=0; -Pravi problem kod upisa...  
    if(z!=number_of_bytes_to_send)
    {
        return(1);
    }
    else
    {
        return(0);
    }    
    #endif
}
#endif

#endif

unsigned char VNrs(unsigned char s)
{
    unsigned char a=0,b=s,c;
    do
    {
        delay_ms(10);
        //delay_ms(100);
        c=ReadSPIs(b,&outbv[a]);
        b-=c;
        a+=c;        
    }while(a<s);

    outbv[a]=0;
    return(a);
}
unsigned char InitVNC1L(void)
{
unsigned char pom;
unsigned char niz[]={'S','C','S', 0x0d};

    clrvn();
    SendVNC1L(niz,4);
    pom=VNrs(2);    
    
    if(pom<2)
    {         
        return(1);
    }
    else if((outbv[0]!='>')&&(outbv[0]!=0x0d))
    {                             
        return(1);    
    }    
    return(0);
}

unsigned char CKD(void)
{    
unsigned char pom;
unsigned char niz[]={0x16,0x0d};

    clrvn();
    
    SendVNC1L(niz,2);
        
    pom=VNrs(2);                 
    
    if(pom<2)
    {
        return(1);
    }
    else if((outbv[0]=='N'))
    {
        return(1);    
    }    
    return(0);    
}
unsigned char FindFile(void)
{
    unsigned char pom;
    int t;
    clrvn(); 
    t=sprintf(&outbv[0],"%c %s%c", 0x01, STICK.filen, 0x0D);
    SendVNC1L(&outbv[0],(unsigned char)t);
    pom=VNrs(3);
    if(pom<3)
    {            
        return(0);
    }
    else if((outbv[1]=='C')&&(outbv[2]=='F')) //ili [1] i [2] - bilo kod vlade, nije radilo.. mozda radi sa [0] i [1]
    {   
        return(0);      
    }    
    return(1);
        
}

unsigned char DeleteFile(void)
{
    unsigned char pom;
    int t;
    clrvn();
    t=sprintf(&outbv[0],"%c %s%c", 0x07,STICK.filen, 0x0D);
    SendVNC1L(&outbv[0],(unsigned char)t);
    pom=VNrs(2);
    if(pom<2)
    {
        return(1);
    }
    else if((outbv[0]!='>'))
             {
             return(1);    
         }    
    return(0);    
}


void setdt(void)
{
    unsigned long dt=0;
    unsigned long t;
    //readdt();
    t=(unsigned long)TIMEDATE.yy+20;
    t=t<<25;
    dt|=t;
    
    t=(unsigned long)TIMEDATE.mm;
    t=t<<21;
    dt|=t;
    
    t=(unsigned long)TIMEDATE.dd;
    t=t<<16;
    dt|=t;
    
    t=(unsigned long)TIMEDATE.hh;
    t=t<<11;
    dt|=t;

    t=(unsigned long)TIMEDATE.min;
    t=t<<5;
    dt|=t;
    
    t=(unsigned long)TIMEDATE.sec;
    dt|=t;    
         
    #if 1
    STICK.dt3=(unsigned char)((dt>>24)&0x0ff);
    STICK.dt2=(unsigned char)((dt>>16)&0x0ff);
    STICK.dt1=(unsigned char)((dt>>8)&0x0ff);
    STICK.dt0=(unsigned char)((dt)&0x0ff);
    #endif                                
             
}

unsigned char OpenFileW(void)
{
    unsigned char pom;
    unsigned char size_of_file_name;
    int t;
    clrvn();
    setdt();
          
    size_of_file_name=sizeof(STICK.filen);
    
    #if USB_STICK_DEBUG   
    
    sprintf(&OutputBuffer[0], "DATUM:%c%c%c%c%c\r\n", STICK.filen, STICK.dt3, STICK.dt2, STICK.dt1, STICK.dt0, 0x0D);
    COMM.remote_enable='E';
    SendOutputBuffer('B');
    
    sprintf(&OutputBuffer[0], "%s %c%c%c%c%c\r\n", STICK.filen, STICK.dt3, STICK.dt2, STICK.dt1, STICK.dt0, 0x0D);
    COMM.remote_enable='E';
    SendOutputBuffer('B');
    #endif                              
                                 
    t=sprintf(&outbv[0],"%c %s %c%c%c%c%c", 0x09, STICK.filen, STICK.dt3, STICK.dt2, STICK.dt1, STICK.dt0, 0x0D);
    
    #if USB_STICK_DEBUG
    sprintf(&OutputBuffer[0], "%s", outbv);
    COMM.remote_enable='E';
    SendOutputBuffer('B');
    #endif
    
    outbv[t]=0;

    //t=sprintf(&outbv[0],"\X009 %s\X00d",STICK.filen);
    //outbv[t]=0;
                                      
    SendVNC1L(&outbv[0],(unsigned char)t);
    delay_ms(500);    
    pom=VNrs(2);
    if(pom<2)
    {
        return(1);
    }
    else if((outbv[0]!='>'))
    {
             return(1);    
    }
    return(0);    
}


unsigned char CloseFile(void)
{
    unsigned char pom;
    int t;
    clrvn();
    t=sprintf(&outbv[0],"%c %s%c", 0x0A, STICK.filen, 0x0D);
    SendVNC1L(&outbv[0],(unsigned char)t);
    delay_ms(200);
    pom=VNrs(2);
    if(pom<2)
    {
        return(1);
    }
    else if((outbv[0]!='>'))
    {
        return(1);    
    }    
    return(0);    
}
         
unsigned char SetDir(void)
{
    unsigned char pom;
    int t;
    delay_ms(500);
    clrvn();
    t=sprintf(&outbv[0],"%c ..%c%c ..%c", 0x02, 0x0D, 0x02, 0x0D);
    SendVNC1L(&outbv[0],(unsigned char)t);
    VNrs(4);
    delay_ms(200);
    clrvn();
    setdt();             
    t=sprintf(&outbv[0],"%c DVDATA %c%c%c%c%c", 0x06,STICK.dt3,STICK.dt2,STICK.dt1,STICK.dt0, 0x0D);
    
    #if USB_STICK_DEBUG
    sprintf(&OutputBuffer[0], "%s", outbv);
    COMM.remote_enable='E';
    SendOutputBuffer('B');
    #endif
    
    SendVNC1L(&outbv[0],(unsigned char)t);        
    
    pom=VNrs(2);
    if(pom<2)
    {
        //return(1);
    }
    
    
    delay_ms(300);
    clrvn();    
    
    t=sprintf(&outbv[0],"%c DVDATA%c", 0x02, 0x0D);
    SendVNC1L(&outbv[0],(unsigned char)t);    
    pom=VNrs(2);
    if(pom<2)
    {
        return(1);
    } 
    ///////////////////////////////////////////////////////
    //Ubaceno kreiranje novog direktorija...               
    ////////////////////////////////////////////////////////
    delay_ms(300); 
    clrvn();
    setdt();                                        
    //Napravi novi direktorij
    t=sprintf(&outbv[0],"%c %s %c%c%c%c%c", 0x06,STICK.dirname,STICK.dt3,STICK.dt2,STICK.dt1,STICK.dt0, 0x0D); 
    
    #if USB_STICK_DEBUG
    sprintf(&OutputBuffer[0], "%s", outbv);
    COMM.remote_enable='E';
    SendOutputBuffer('B');
    #endif
    
    SendVNC1L(&outbv[0],(unsigned char)t);
    
    pom=VNrs(2);
    if(pom<2)
    {
        //return(1);
    }
    
    delay_ms(300);
    clrvn();    
                             
    //Udji u novi direktorij...
    t=sprintf(&outbv[0],"%c %s%c", 0x02, STICK.dirname , 0x0D);
    SendVNC1L(&outbv[0],(unsigned char)t);    
    pom=VNrs(2);
    if(pom<2)
    {
        return(1);
    }   
    /////////////////////////////////////////////////////
    return(0);
        
}


#if 0

unsigned char SetDir(void)
{
    unsigned char pom;
    int t;
    delay_ms(500);
    clrvn();
    t=sprintf(&outbv[0],"%c ..%c%c ..%c", 0x02, 0x0D, 0x02, 0x0D);
    SendVNC1L(&outbv[0],(unsigned char)t);
    VNrs(4);
    delay_ms(200);
    clrvn();
    setdt();             
    t=sprintf(&outbv[0],"%c DVDATA %c%c%c%c%c", 0x06,STICK.dt3,STICK.dt2,STICK.dt1,STICK.dt0, 0x0D);
    SendVNC1L(&outbv[0],(unsigned char)t);
    
    pom=VNrs(2);
    if(pom<2)
    {
        //return(1);
    }
    
    
    delay_ms(300);
    clrvn();    
    
    t=sprintf(&outbv[0],"%c DVDATA%c", 0x02, 0x0D);
    SendVNC1L(&outbv[0],(unsigned char)t);    
    pom=VNrs(2);
    if(pom<2)
    {
        return(1);
    } 
    ///////////////////////////////////////////////////////
    //Ubaceno kreiranje novog direktorija...               
    ////////////////////////////////////////////////////////
    delay_ms(300); 
    clrvn();
    setdt();                                        
    //Napravi novi direktorij 
//    sprintf(STICK.dirname, "NOVI");
    t=sprintf(&outbv[0],"%c %s %c%c%c%c%c", 0x06,STICK.dirname,STICK.dt3,STICK.dt2,STICK.dt1,STICK.dt0, 0x0D);
    SendVNC1L(&outbv[0],(unsigned char)t);
    
    pom=VNrs(2);
    if(pom<2)
    {
        //return(1);
    }
    
    delay_ms(300);
    clrvn();    
                             
    //Udji u novi direktorij...
    t=sprintf(&outbv[0],"%c %s%c", 0x02, STICK.dirname , 0x0D);
    SendVNC1L(&outbv[0],(unsigned char)t);    
    pom=VNrs(2);
    if(pom<2)
    {
        return(1);
    }   
    /////////////////////////////////////////////////////
    return(0);
        
}
#endif

unsigned char OpenVfile(unsigned char tt)
{
unsigned char resp;
    resp=CKD();
    if(resp==1){return(NO_DISK);}
    SetDir();
    delay_ms(100);
    if(tt==1){
    resp=FindFile();
    if(resp==1){return(FILE_EXIST);}
    else{return (0);}        
    }  
    else{       
    DeleteFile();        
    delay_ms(100);
    {         
        resp=OpenFileW();
        if(resp==1){return(FILE_ERROR);}
    }
    delay_ms(200);
    clrvn();
    STICK.s=1;  
    }
    return(0);        
}

                                     
void CloseVfile(void)
{
    CloseFile();
    STICK.s=0;
}

volatile unsigned char  greska;

unsigned char InitStick(unsigned char tt)
{
    unsigned char r;  
    unsigned char resp;
        
    //Komentarisati...    
    #if 0               
    sprintf(STICK.filen, "FileABC.CFD");
    TIMEDATE.yy=6;
    TIMEDATE.mm=5;
    TIMEDATE.dd=4;
    TIMEDATE.hh=1;
    TIMEDATE.min=2;
    TIMEDATE.sec=3;
    #endif
        
    resp=InitVNC1L();  
      
    if(resp==1)
    {               
        greska=STICK_ERROR;                   
        return(1);    
    }
    else
    {   
        r=OpenVfile(tt);

        if(r!=0)
        {                     
            if(r==FILE_EXIST)
            {             
                return(r);        
            }            
            greska=STICK_ERROR;
            return(1);        
        }
    }                  
    return(0);
}

unsigned char sector[512];

#if 0
void reset_this_mcu(void)
{
    //Reset MCU:
    #asm("cli")//__disable_interrupt();
    CCP = CCP_IOREG_gc;//(unsigned int)CCP_IOREG_gc;
    RST.CTRL = RST_SWRST_bm;//(unsigned int)RST_SWRST_bm;
}
#endif 

void clear_sector(void)
{
    int i;
    for(i=0; i<512; i++){sector[i]=0x00;}
}

//extern functions: 
extern void SendProgressBar(void);
//extern unsigned int check_for_stop_button();

#define PC_DEBUG 0

unsigned char LWriteToFile(unsigned int ss)
{
unsigned int s, i, j, k;
unsigned char sector_position;
unsigned char channel;
unsigned int address;
unsigned int current_rectified[NUMBER_OF_CURRENT_CHS];
unsigned int percent_completed[9];

percent_completed[0]=0.1*TESTPAR.test_duration;
percent_completed[1]=0.2*TESTPAR.test_duration;
percent_completed[2]=0.3*TESTPAR.test_duration;
percent_completed[3]=0.4*TESTPAR.test_duration;
percent_completed[4]=0.5*TESTPAR.test_duration;
percent_completed[5]=0.6*TESTPAR.test_duration;
percent_completed[6]=0.7*TESTPAR.test_duration;
percent_completed[7]=0.8*TESTPAR.test_duration;
percent_completed[8]=0.9*TESTPAR.test_duration;

k=0;

//s=(ss/8);
//s1=(unsigned char)(ss%200);
delay_ms(100);
clrvn();


//7000*67 + 16  : 0x072818 
//10000*67 + 16 : 0x0A3940
//20000*67 + 16 : 0x147270
//16 predstavlja 0x0A 0x0D SAMPLES:  te 0x0A 0x0D DTSO na kraju 
if(ss==7000)      {sprintf(&outbv[0],"%c %c%c%c%c%c",0x08,0x00,0x07,0x28,0x18,0x0d);}
else if(ss==10000){sprintf(&outbv[0],"%c %c%c%c%c%c",0x08,0x00,0x0A,0x39,0x40,0x0d);}    
else if(ss==20000){sprintf(&outbv[0],"%c %c%c%c%c%c",0x08,0x00,0x14,0x72,0x70,0x0d);}

SendVNC1L(&outbv[0],7);  

//send data to USB flash drive!:                                    
for(address=0; address<TESTPAR.test_duration; address++)
{                                                                  

delay_us(100);
read_adc_res_sram_test(address);

clear_sector();

sector_position=0;

if(address==0)
{
    sector[sector_position++]=0x0A;
    sector[sector_position++]=0x0D;
    sector[sector_position++]='S';
    sector[sector_position++]='A';
    sector[sector_position++]='M';
    sector[sector_position++]='P';
    sector[sector_position++]='L';
    sector[sector_position++]='E';
    sector[sector_position++]='S';
    sector[sector_position++]=':';   
}
            
#if 1    
//send data from main contacts:  
for(channel=0; channel<NUMBER_OF_MAIN_CONTACTS; channel++)
{                                     
    sector[sector_position++]=((unsigned char)(ADC_Results.MC[channel]>>8));
    sector[sector_position++]=((unsigned char)(ADC_Results.MC[channel]));
} 
#else
//send data from main contacts:  
for(channel=0; channel<NUMBER_OF_MAIN_CONTACTS; channel++)
{                                     
    sector[sector_position++]=('A');
    sector[sector_position++]=('B');
} 
#endif
                   
//send data from current channels:
for(channel=0; channel<NUMBER_OF_CURRENT_CHS; channel++)
{                               
    //calculate rectified current for each channel
    current_rectified[channel]=abs(ADC_Results.CURRENT[channel]);   
    
    sector[sector_position++]=(unsigned char)(current_rectified[channel]>>8);    
    sector[sector_position++]=(unsigned char)(current_rectified[channel]);    
}               
//send data from analog channels:
for(channel=0; channel<NUMBER_OF_ANALOG_CHS; channel++)
{
    sector[sector_position++]=(unsigned char)(ADC_Results.ANCH[channel]>>8);    
    sector[sector_position++]=(unsigned char)(ADC_Results.ANCH[channel]);            
}   
                        
//send AUX channels:
   // sector[sector_position++]=(unsigned char)(ADC_Results.AUX); //|0b11000000); //only 6 bits are used (AUX1 to AUX6)               
//    ADC_Results.AUX&=0b00111111; //only 6 bits are used (AUX1 to AUX6);
		ADC_Results.AUX&=0x3F; //only 6 bits are used (AUX1 to AUX6);

    #if 0     
    ADC_Results.AUX=0b00111111;                                                                                                        
    if(address>=1000)
    {
        ADC_Results.AUX|=0b00000001;    
    }               
    if(address>=2000)
    {
        ADC_Results.AUX|=0b00000010;
    }
    if(address>=3000)
    {
        ADC_Results.AUX|=0b00000100;
    }           
    if(address>=4000)
    {
        ADC_Results.AUX|=0b00001000;
    }           
    if(address>=5000)
    {
        ADC_Results.AUX|=0b00010000;
    }           
    if(address>=6000)
    {
        ADC_Results.AUX|=0b00100000;
    }  
    #endif  
    sector[sector_position++]=(unsigned char)(ADC_Results.AUX); //vjestacki napravljena nula da vidim sta ce biti u tome slucaju!  
            
//send data from linear transducers:
for(channel=0; channel<NUMBER_OF_LT_CHS; channel++)
{            
   sector[sector_position++]=(unsigned char)(ADC_Results.LT[channel]>>8);    
   sector[sector_position++]=(unsigned char)(ADC_Results.LT[channel]);                
}
//send data from digital transducers:
for(channel=0; channel<NUMBER_OF_DT_CHS; channel++)
{
    if(TESTPAR.transd_type==TRANSDUCER_ANALOG)
    {                             
    //digital transudcer not recorded, send some dummy bytes:
    sector[sector_position++]='D';    
    sector[sector_position++]='D';           
    sector[sector_position++]='D';          
    sector[sector_position++]='D';    
    }
    else
    {
    sector[sector_position++]=(unsigned char)(ADC_Results.DTH[channel]>>8);    
    sector[sector_position++]=(unsigned char)(ADC_Results.DTH[channel]);           
    sector[sector_position++]=(unsigned char)(ADC_Results.DTL[channel]>>8);          
    sector[sector_position++]=(unsigned char)(ADC_Results.DTL[channel]);
    }            
}

//add data transfer over status:
if(address==TESTPAR.test_duration-1)
{
    sector[sector_position++]=0x0A;
    sector[sector_position++]=0x0D;
    sector[sector_position++]='D';
    sector[sector_position++]='T';
    sector[sector_position++]='S';
    sector[sector_position++]='O';     
}

sector[sector_position]='X';
  
if(SendVNC1L_fast(&sector[0], sector_position)!=0){}//return 1;

for(i=0; i<9; i++)
{
    if(address==percent_completed[i])
    {
    
    #if PC_DEBUG
    putcharB('A');
    #endif
            
    SendProgressBar();  
    delay_ms(100);      
    }
}

//if(((check_for_stop_button())!=MAIN_OK)){reset_this_mcu();}    
 
}

#if PC_DEBUG
putcharB('B');
#endif    

//jedan extra progress bar za svaki slucaj:
SendProgressBar();             

slave_deselect_usb_flash_drive();

#if PC_DEBUG
putcharB('C');
#endif         

#if 1
if(VNrs(2)<2)
{
  #if PC_DEBUG
  putcharB('X');
  #endif
  return(2);
}

else if((outbv[0]!='>'))
{ 
  #if PC_DEBUG
  putcharB('Y');
  #endif        
  return(1);         
} 
#endif

return(0); 
}


unsigned char LWriteToFile3(unsigned int ss)
{
unsigned int k, z;
unsigned int pom_ss;

k=0; z=0;
delay_ms(50);
clrvn();

pom_ss=ss;
sprintf(&outbv[0],"%c %c%c%c%c%c",0x08,0x00,0x00,(unsigned char)(pom_ss/256),(unsigned char)(pom_ss%256),0x0d);
                                        
SendVNC1L(&outbv[0],7);                

//Ovdje treba setovati sector[o]=...
clear_sector();      
sprintf(&sector[0], "F:06.00.");
SendVNC1L(&sector[0], 8);
          
//SlaveSelect(NO_SLAVE);
slave_deselect_usb_flash_drive();

if(VNrs(2)<2)
{
    return(2);
}
else if((outbv[0]!='>'))
{         
    return(1); 
}
  
return(0); 
}

void UcitajNumerickeRezultate(unsigned char Rezultati, unsigned char TestData){
unsigned int i,j,k;
unsigned char *p_range;
clear_sector();
j=0;  

if(Rezultati==1)
{
    
    //selected sampling resolution:
    sector[j++]=TESTPAR.sampl_resol;        //0
    //selected operating sequence: 
    sector[j++]=TESTPAR.op_sequence;        //1
    //selcted time base:
    sector[j++]=TESTPAR.time_base;          //2
    //selected mains freq (50/60Hz)
    sector[j++]=TESTPAR.mains_freq;         //3
    //selected travle unit:
    sector[j++]=TESTPAR.travel_unit;        //4
    //selected gains for analog channels:
    sector[j++]=TESTPAR.analog1_gain;       //5
    sector[j++]=TESTPAR.analog2_gain;       //6
    sector[j++]=TESTPAR.analog3_gain;       //7
    sector[j++]=TESTPAR.analog4_gain;       //8
    sector[j++]=TESTPAR.analog5_gain;       //9
    sector[j++]=TESTPAR.analog6_gain;       //10
    //opening pulse phase:
    sector[j++]=TESTPAR.o_coil_phase;       //11
    //closing pulse phase:       
    sector[j++]=TESTPAR.c_coil_phase;       //12
    //test duration:
    sector[j++]=TESTPAR.test_duration>>8;   //13
    sector[j++]=TESTPAR.test_duration;      //14
    //closing pulse lenght:
    sector[j++]=TESTPAR.c_pulse_lenght>>8;  //15
    sector[j++]=TESTPAR.c_pulse_lenght;     //16
    //opening pulse lenght:
    sector[j++]=TESTPAR.o_pulse_lenght>>8;  //17
    sector[j++]=TESTPAR.o_pulse_lenght;     //18
    //trip free delay:
    sector[j++]=TESTPAR.trip_free_delay>>8; //19
    sector[j++]=TESTPAR.trip_free_delay;    //20
    //reclosing delay:
    sector[j++]=TESTPAR.reclosing_delay>>8; //21
    sector[j++]=TESTPAR.reclosing_delay;    //22
    //oco delay1:
    sector[j++]=TESTPAR.oco_delay1>>8;      //23
    sector[j++]=TESTPAR.oco_delay1;         //24
    //oco delay2:
    sector[j++]=TESTPAR.oco_delay2>>8;      //25
    sector[j++]=TESTPAR.oco_delay2;         //26
    //transfer functions for analog channels:
    sector[j++]=TESTPAR.analog1tf>>8;       //27
    sector[j++]=TESTPAR.analog1tf;          //28
    sector[j++]=TESTPAR.analog2tf>>8;       //29
    sector[j++]=TESTPAR.analog2tf;          //30
    sector[j++]=TESTPAR.analog3tf>>8;       //31
    sector[j++]=TESTPAR.analog3tf;          //32          
    sector[j++]=TESTPAR.analog4tf>>8;       //33
    sector[j++]=TESTPAR.analog4tf;          //34  
    sector[j++]=TESTPAR.analog5tf>>8;       //35
    sector[j++]=TESTPAR.analog5tf;          //36
    sector[j++]=TESTPAR.analog6tf>>8;       //37
    sector[j++]=TESTPAR.analog6tf;          //38
    //selected external trigger source:  
    sector[j++]=TESTPAR.ext_trigg_src;      //39
    //selected external trigger edge 
    //(rising or falling)
    sector[j++]=TESTPAR.ext_trigg_edg;      //40
    //selected external trigger level:
    sector[j++]=TESTPAR.ext_trigg_lev>>8;   //41    
    sector[j++]=TESTPAR.ext_trigg_lev;      //42
    //break units per phase: 
    sector[j++]=TESTPAR.break_units;        //43
    //transducer type (rotary or linear):  
    sector[j++]=TESTPAR.transd_type;        //44
    //selected transducer channel: 
    sector[j++]=TESTPAR.transd_ch;          //45
    //transfer functions for transducers:
    sector[j++]=TESTPAR.transd1_tf>>8;      //46
    sector[j++]=TESTPAR.transd1_tf;         //47
    sector[j++]=TESTPAR.transd2_tf>>8;      //48
    sector[j++]=TESTPAR.transd2_tf;         //49
    sector[j++]=TESTPAR.transd3_tf>>8;      //50
    sector[j++]=TESTPAR.transd3_tf;         //51
    //selected external trigger waiting time:      
    sector[j++]=TESTPAR.ext_trigg_wtm;      //52
    //auxiliary channels, dry or wet mode:
    sector[j++]=TESTPAR.aux1_dry_wet;       //53
    sector[j++]=TESTPAR.aux2_dry_wet;       //54
    sector[j++]=TESTPAR.aux3_dry_wet;       //55
    sector[j++]=TESTPAR.aux4_dry_wet;       //56
    sector[j++]=TESTPAR.aux5_dry_wet;       //57
    sector[j++]=TESTPAR.aux6_dry_wet;       //58
    //both sides grounded:
    sector[j++]=TESTPAR.bs_grounded;        //59
    //cb operating mechanism:
    sector[j++]=TESTPAR.oper_mech;          //60 
    //analog channels transfer function units:
    sector[j++]=TESTPAR.analog1tf_unit[0];  //61
    sector[j++]=TESTPAR.analog1tf_unit[1];  //62
    sector[j++]=TESTPAR.analog1tf_unit[2];  //63
    sector[j++]=TESTPAR.analog2tf_unit[0];  //64
    sector[j++]=TESTPAR.analog2tf_unit[1];  //65
    sector[j++]=TESTPAR.analog2tf_unit[2];  //66
    sector[j++]=TESTPAR.analog3tf_unit[0];  //67
    sector[j++]=TESTPAR.analog3tf_unit[1];  //68
    sector[j++]=TESTPAR.analog3tf_unit[2];  //69
    sector[j++]=TESTPAR.analog4tf_unit[0];  //70
    sector[j++]=TESTPAR.analog4tf_unit[1];  //71
    sector[j++]=TESTPAR.analog4tf_unit[2];  //72
    sector[j++]=TESTPAR.analog5tf_unit[0];  //73
    sector[j++]=TESTPAR.analog5tf_unit[1];  //74
    sector[j++]=TESTPAR.analog5tf_unit[2];  //75
    sector[j++]=TESTPAR.analog6tf_unit[0];  //76
    sector[j++]=TESTPAR.analog6tf_unit[1];  //77
    sector[j++]=TESTPAR.analog6tf_unit[2];  //78
    //selcted transducer lenghts (mm):  
    sector[j++]=TESTPAR.tr_lenght_mm[0]>>8; //79
    sector[j++]=TESTPAR.tr_lenght_mm[0];    //80
    sector[j++]=TESTPAR.tr_lenght_mm[1]>>8; //81
    sector[j++]=TESTPAR.tr_lenght_mm[1];    //82
    sector[j++]=TESTPAR.tr_lenght_mm[2]>>8; //83
    sector[j++]=TESTPAR.tr_lenght_mm[2];    //84
    //selected transducer lenghts (inch):                             
    sector[j++]=TESTPAR.tr_lenght_inch[0]>>8; //85
    sector[j++]=TESTPAR.tr_lenght_inch[0];    //86
    sector[j++]=TESTPAR.tr_lenght_inch[1]>>8; //87
    sector[j++]=TESTPAR.tr_lenght_inch[1];    //88
    sector[j++]=TESTPAR.tr_lenght_inch[2]>>8; //89
    sector[j++]=TESTPAR.tr_lenght_inch[2];    //90
    //linear transducer gain (don't use for now):
    sector[j++]=TESTPAR.tr_gain[0]>>8;        //91
    sector[j++]=TESTPAR.tr_gain[0];           //92
    sector[j++]=TESTPAR.tr_gain[1]>>8;        //93
    sector[j++]=TESTPAR.tr_gain[1];           //94
    sector[j++]=TESTPAR.tr_gain[2]>>8;        //95
    sector[j++]=TESTPAR.tr_gain[2];           //96
    //linear transducer offset (don't use for now):              
    sector[j++]=TESTPAR.tr_offset[0]>>8;      //97
    sector[j++]=TESTPAR.tr_offset[0];         //98
    sector[j++]=TESTPAR.tr_offset[1]>>8;      //99
    sector[j++]=TESTPAR.tr_offset[1];         //100
    sector[j++]=TESTPAR.tr_offset[2]>>8;      //101
    sector[j++]=TESTPAR.tr_offset[2];         //102                       
    //dalje smo upisivali rezultate:
    
    //calibration coeficients:
    sector[j++]='C';                          //103
    sector[j++]='A';                          //104
    sector[j++]='L';                          //105
    sector[j++]='I';                          //106
    sector[j++]='B';                          //107                       
    //analog channels:    
    p_range=&TESTPAR.analog1_gain;                                   
    //od 108 do (138= 108 + (5*6))
    for(i=0; i< NUMBER_OF_ANALOG_CHS; i++)
    {                  
            //Active analog CH gain:                          
            sector[j++]=CALIB.analogch_gain[i][(*p_range-'0')]>>8;   
            sector[j++]=CALIB.analogch_gain[i][(*p_range-'0')];      
            //Active analog CH offset:
            sector[j++]=CALIB.analogch_offset[i][(*p_range-'0')]>>8; 
            sector[j++]=CALIB.analogch_offset[i][(*p_range-'0')];    
            //Active analog CH sign:
            sector[j++]=CALIB.analogch_sign[i][(*p_range-'0')];        
    }  
    //current channels:    
    //Ovim redosljedom idu koeficijenti:
    //i=0: OPEN1
    //i=1: OPEN2
    //i=2: OPEN3
    //i=3: CLOSE1
    //i=4: CLOSE2
    //i=5: CLOSE3 
    //od 139 do 169 (=139+(5*6))
    for(i=0; i<NUMBER_OF_CURRENT_CHS; i++)
    {
            sector[j++]=CALIB.currentch_gain[i]>>8;
            sector[j++]=CALIB.currentch_gain[i];
            sector[j++]=CALIB.currentch_offset[i]>>8;
            sector[j++]=CALIB.currentch_offset[i];
            sector[j++]=CALIB.currentch_sign[i];            
    }
    //od 170 do 200 (=170+(5*6))  
    //linear transducers:
    for(i=0; i<NUMBER_OF_LT_CHS; i++)
    {
            sector[j++]=CALIB.linear_tr_ch_gain[i]>>8;
            sector[j++]=CALIB.linear_tr_ch_gain[i];
            sector[j++]=CALIB.linear_tr_ch_offset[i]>>8;
            sector[j++]=CALIB.linear_tr_ch_offset[i];
            sector[j++]=CALIB.linear_tr_ch_sign[i];            
    }  
                               
}


if(TestData==1)
{

for(i=0; i<20; i++)   {sector[i]=BDATA.BreakerID[i];}              
for(i=20; i<40; i++)  {sector[i]=BDATA.BreakerType[(i-20)];}
for(i=40; i<60; i++)  {sector[i]=BDATA.Compartment[(i-40)];}
for(i=60; i<80; i++)  {sector[i]=BDATA.Manufacturer[(i-60)];}
for(i=80; i<100; i++) {sector[i]=BDATA.SerialNumer[(i-80)];}
for(i=100; i<120; i++){sector[i]=BDATA.StationName[(i-100)];}

for(i=120; i<140; i++){sector[i]=TDATA.CompanyName[(i-120)];}
for(i=140; i<160; i++){sector[i]=TDATA.Operater[(i-140)];}
for(i=160; i<180; i++){sector[i]=TDATA.TypeOfTest[(i-160)];}

sector[180]=TIMEDATE.yy;
sector[181]=TIMEDATE.mm;
sector[182]=TIMEDATE.dd;
sector[183]=TIMEDATE.hh;
sector[184]=TIMEDATE.min;
sector[185]=TIMEDATE.sec;

//Ovdje jos ubaciti podatke o verziji FW i uredjaju...
for(i=190; i<210; i++){sector[i]=STICK.ModelInfo[i-190];}       
                
}           
}

unsigned char LWriteToFile2(unsigned int ss)
{
unsigned int k, z;//, i;
unsigned int pom_ss;

k=0; z=0;
delay_ms(100);
clrvn();

pom_ss=ss*4+50;

sprintf(&outbv[0],"%c %c%c%c%c%c",0x08,0x00,0x00,(unsigned char)(pom_ss/256),(unsigned char)(pom_ss%256),0x0d);
                                        
SendVNC1L(&outbv[0],7);                
 
UcitajNumerickeRezultate(0,1);
SendVNC1L(&sector[0],   200);
SendVNC1L(&sector[200], 200);
UcitajNumerickeRezultate(1,0);
SendVNC1L(&sector[0],   225);        
SendVNC1L(&sector[225], 225);
          
//SlaveSelect(NO_SLAVE);
slave_deselect_usb_flash_drive();

 if(VNrs(2)<2)
 {
  return(2);
 }
 else if((outbv[0]!='>'))
   {         
       return(1); 
   } 
 return(0); 
}

unsigned int TestWrite(void)
{
//unsigned char Pom;
                //OVA OK:                  
                LWriteToFile3(8); //8 znaci da se 8 karaktera upisuje u file        
                SendProgressBar();             
                LWriteToFile2(200); //200 znaci da se 200*4+50 karaktera upisuje u file!                                                

                LWriteToFile(TESTPAR.test_duration);
                return 0;                
                #if 0
                if(LWriteToFile(TESTPAR.test_duration)!=0){return 1;}
                else{return 0;}
                #endif                
}


unsigned int InitStick_Comm(unsigned char tt){
unsigned char i,r;
greska=0;

#if 1
//added - reset usb flash drive MCU before writing...
//reset_flash_drive_MCU();
#endif


slave_select_usb_flash_drive();
for(i=0; i<100; i++){spie_master_tx_rx('0'); delay_us(100);} //Ubaceno radi sinhronizacije STICK plocice.
slave_deselect_usb_flash_drive();

r=InitStick(tt);

if(tt==1){

    if(r!=0)
    {
                     
        if(greska==STICK_ERROR)
        {              
        slave_select_usb_flash_drive();
        spie_master_tx_rx('S');    //Provjeriti jel ovo OK!
        slave_deselect_usb_flash_drive();
        return (STICK_ERROR);
        }                             
                   
        return (FILE_EXIST);
    }                                     
    else
    {     
        
    return (MAIN_OK);
    }
}
else
{
    if(r!=0)
    {
        slave_select_usb_flash_drive();
        spie_master_tx_rx('S');        //Provjeriti jel ovo OK!
        slave_deselect_usb_flash_drive();
        return (STICK_ERROR);
    }  
    else
    {                                          
        if(TestWrite()){return (STICK_ERROR);}       
        #if PC_DEBUG
        putcharB('C');                        
        slave_select_usb_flash_drive();
        for(i=0; i<100; i++){spie_master_tx_rx('0'); delay_us(100);} //Ubaceno radi sinhronizacije STICK plocice.
        slave_deselect_usb_flash_drive();
        #endif             
        CloseVfile(); 
        #if PC_DEBUG
        putcharB('D');        
        #endif                                                          
        return (MAIN_OK);
    }
}
}