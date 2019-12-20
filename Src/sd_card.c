#include "main.h"
#include "fatfs.h"

#include <stdio.h>
#include <comm.h>
#include "bios.h"
#include "adc.h"

#define FILE_TYPE_GRAPHIC_RESULTS         (unsigned char) 10
#define FILE_TYPE_NUMERIC_RESULTS         (unsigned char) 20 
#define FILE_TYPE_MEM_ORGANISATION        (unsigned char) 30
#define FILE_TYPE_LIST_MEMORY             (unsigned char) 40

/* will hold the information for logical drive 0: */
FATFS fat;
/* number of bytes written/read to the file */
unsigned int nbytes;
/* will hold the file information */
FIL file;
/* will hold file attributes, time stamp information */
//FILINFO finfo;
/* file path */
char path[20];

void SendProgressBar(void)
{
    sprintf(OutputBuffer, "PB");
    SendOutputBuffer(COMM.port); 
    HAL_Delay(10);
}

unsigned int size_of_mem_loc_res;

void define_path(unsigned char file_type, unsigned int mem_loc)
{
switch(file_type)
{
    case FILE_TYPE_GRAPHIC_RESULTS:
        //sprintf(path, "0:/gr_res%05u.txt", mem_loc);
        sprintf(path, "0:/g%05u.txt", mem_loc);
    break;
    case FILE_TYPE_NUMERIC_RESULTS:
         //sprintf(path, "0:/num_res%05u.txt", mem_loc);
         sprintf(path, "0:/n%05u.txt", mem_loc);
    break;
    case FILE_TYPE_MEM_ORGANISATION:
        sprintf(path, "0:/memorg.txt");
    break;  
    default:
        #if DEBUG
        sprintf(OutputBuffer, "ERROR!!!");
        SendOutputBuffer('B');        
        #endif    
    break;
}
}


unsigned int init_sd_card(void)
{
//init SD card:
//#asm("cli")
//sd_card_tcc0_init();
//#asm("sei")

//Time and date, not important:
//prtc_get_time=NULL;
//prtc_get_date=NULL;

/* globally enable interrupts */
///#asm("sei")

/* check if SD card inserted (without using the /CD signal) */
//if (!sdcard_present()) {return(error(FR_NOT_READY));}


#if DEBUG
sprintf(OutputBuffer, "SD Card detected...\r\n");
SendOutputBuffer('B');
#endif   

/* mount logical drive 0: */
//if ((res=f_mount(0,&fat))!=FR_OK){return(error(res));}
if(f_mount(&fat, SDPath, 1)==!FR_OK){return(ERROR_SDC);}

#if DEBUG
sprintf(OutputBuffer, "Drive O: mounted...\r\n");
SendOutputBuffer('B');
#endif   
return MAIN_OK;
}

unsigned int close_file_and_unmount()
{
/* close the file */
if ((f_close(&file))!=FR_OK){return(ERROR_SDC);}

#if DEBUG
sprintf(OutputBuffer, "File closed\r\n");
SendOutputBuffer('B');
#endif 

/* diable timer t0 used for SD card: */
//sd_card_tc0_disable(&TCC0);
f_mount(0, "", 0);                     /* Unmount the default drive */

return MAIN_OK;
}

extern void read_adc_raw_results(uint32_t sram_address);

unsigned int write_to_file(unsigned char file_type, char mem_loc_res[])
{
unsigned int i;
//char mem_loc_res2[MAX_NUMBER_MEM_LOC];

#if 1
unsigned int progress_20_percent;
unsigned int progress_40_percent;
unsigned int progress_60_percent;
unsigned int progress_80_percent;

progress_20_percent=TESTPAR.test_duration/10;
progress_20_percent*=2;

progress_40_percent=TESTPAR.test_duration/10;
progress_40_percent*=4;

progress_60_percent=TESTPAR.test_duration/10;
progress_60_percent*=6;

progress_80_percent=TESTPAR.test_duration/10;
progress_80_percent*=8;
#endif

#if DEBUG
    sprintf(OutputBuffer, "Creating file...\r\n");
    SendOutputBuffer('B');
#endif     
    
/* delete an eventual old file with the same name */
f_unlink(path);

#if DEBUG
    sprintf(OutputBuffer, "Old file deleted...\r\n");
    SendOutputBuffer('B');
#endif    

/* create a new file in the root of drive 0:/ and set write access mode */
if ((f_open(&file,path,FA_CREATE_ALWAYS | FA_WRITE))!=FR_OK){return(0);}    
    
#if DEBUG
    //sprintf(OutputBuffer, "File opened...\r\n");
    //SendOutputBuffer(COMM.port);
#endif     

    switch(file_type)
    {
    case FILE_TYPE_GRAPHIC_RESULTS:
        /* write data to file */
        for(i=0; i<TESTPAR.test_duration; i++)
        {                          
            //read data from SRAM memory:  
						//TODO: read_adc_raw_results
            read_adc_raw_results(i); //ovo ce procitati iz SRAMA i smjestiti na SD card
            //Test:                        
            //ADC_8bit_results.CURRENT_H[0]=123;
            /* write ADC data to the file, without the NULL string terminator sizeof(data)-1 */
					//TODO:return error
            if ((f_write(&file,&ADC_8bit_results,sizeof(ADC8BIT_STRUCT)-1,&nbytes))!=FR_OK){return(ERROR_SDC);}
            
            #if 1
            if((i==progress_20_percent)||(i==progress_40_percent)||(i==progress_60_percent)||(i==progress_80_percent))
            {
                my_putchar(0x1B);         
            }
            if((i==progress_20_percent+1)||(i==progress_40_percent+1)||(i==progress_60_percent+1)||(i==progress_80_percent+1))
            {
                my_putchar('m');         
            }
            if((i==progress_20_percent+2)||(i==progress_40_percent+2)||(i==progress_60_percent+2)||(i==progress_80_percent+2))
            {
                my_putchar('P');         
            }                              
            if((i==progress_20_percent+3)||(i==progress_40_percent+3)||(i==progress_60_percent+3)||(i==progress_80_percent+3))
            {
                my_putchar('B');         
            }                              
            if((i==progress_20_percent+4)||(i==progress_40_percent+4)||(i==progress_60_percent+4)||(i==progress_80_percent+4))
            {
                my_putchar(0x00);         
            }    
            #endif  
            //if(((check_for_stop_button())!=MAIN_OK)){reset_this_mcu();}            
        }         
        #if DEBUG
            sprintf(OutputBuffer, "Gr. results stored to SD card\r\n");
            SendOutputBuffer('B');
        #endif    
    
    break;
    case FILE_TYPE_NUMERIC_RESULTS:
			
        /* write data to file */                                                                         
        #if DEBUG
        //TESTPAR.op_sequence=CLOSE_TEST;
        #endif   
        
        /* write TESTPAR data to the file, without the NULL string terminator sizeof(data)-1 */                  
			  //TODO: return res
        if ((f_write(&file,&TESTPAR,sizeof(TESTPAR_STRUCT)-1,&nbytes))!=FR_OK){return(ERROR_SDC);}
        if ((f_write(&file,&BDATA,sizeof(BDATA_STRUCT)-1,&nbytes))!=FR_OK){return(ERROR_SDC);}                 
        if ((f_write(&file,&TDATA,sizeof(TDATA_STRUCT)-1,&nbytes))!=FR_OK){return(ERROR_SDC);}
        if ((f_write(&file,&TIMEDATE,sizeof(TIMEDATE_STRUCT)-1,&nbytes))!=FR_OK){return(ERROR_SDC);}
        if ((f_write(&file,&MainC_Results,sizeof(MC_RESULTS_STRUCT)-1,&nbytes))!=FR_OK){return(ERROR_SDC);}
        if ((f_write(&file,&Current_Results,sizeof(CURR_RESULTS_STRUCT)-1,&nbytes))!=FR_OK){return(ERROR_SDC);}
        if ((f_write(&file,&AUX_Results,sizeof(AUX_RESULTS_STRUCT)-1,&nbytes))!=FR_OK){return(ERROR_SDC);}
        if ((f_write(&file,&AnalogCH_Results,sizeof(ANALOG_RESULTS_STRUCT)-1,&nbytes))!=FR_OK){return(ERROR_SDC);}
        if ((f_write(&file,&LTransd_Results, sizeof(LINEAR_RESULTS_STRUCT)-1,&nbytes))!=FR_OK){return(ERROR_SDC);}
        if ((f_write(&file,&DTransd_Results, sizeof(DIGITAL_RESULTS_STRUCT)-1,&nbytes))!=FR_OK){return(ERROR_SDC);}     
        if ((f_write(&file,&RMO_Results, sizeof(RMO_RESULTS_STRUCT)-1, &nbytes))!=FR_OK){return(ERROR_SDC);} //RMO static resistance results!
                                    
        #if DEBUG
            sprintf(OutputBuffer, "Numerical results stored to SD card\r\n");
            SendOutputBuffer('B');
        #endif 
				
    break;   
				
    case FILE_TYPE_MEM_ORGANISATION:            
			//radi:
		//if ((f_write(&file, mem_loc_res, sizeof(mem_loc_res2), &nbytes))!=FR_OK){return(ERROR_SDC);}
		if ((f_write(&file, mem_loc_res, size_of_mem_loc_res, &nbytes))!=FR_OK){return(ERROR_SDC);}

        
        #if DEBUG
            sprintf(OutputBuffer, "New result added to the list!\r\n");
            SendOutputBuffer('B');
        #endif  
    break;    
    }  

return MAIN_OK;
}

unsigned int  store_list_array_into_mem(char mem_loc_res[])
{
unsigned int retVal;

    if((retVal=init_sd_card())!=MAIN_OK)                                          {return retVal;}        
    if((retVal=write_to_file(FILE_TYPE_MEM_ORGANISATION, mem_loc_res))!=MAIN_OK)  {return retVal;}
    if((retVal=close_file_and_unmount())!=MAIN_OK)                                {return retVal;}

return MAIN_OK;
}

unsigned int erase_all_mem_locations(void)
{
unsigned int i, retVal;
char mem_loc_res[MAX_NUMBER_MEM_LOC];

size_of_mem_loc_res=sizeof(mem_loc_res);
	
//define file path:
define_path(FILE_TYPE_MEM_ORGANISATION, 0);

#if 1
//delete all memory locations:
for(i=0; i<size_of_mem_loc_res; i++){mem_loc_res[i]=NO_TEST;}

#if DEBUG
    sprintf(OutputBuffer, "***** SD memory card ERASE MEMORY *****\r\n");
    SendOutputBuffer('B');
#endif

//store list into the file:
retVal=store_list_array_into_mem(mem_loc_res);
#endif

return retVal;
}


unsigned int store_graphics_to_sd_card(unsigned int mem_loc)
{
unsigned int retVal;

if(TESTPAR.op_sequence==STATIC_RESISTANCE){return MAIN_OK;}

define_path(FILE_TYPE_GRAPHIC_RESULTS, mem_loc);

#if DEBUG
//sprintf(OutputBuffer, "***** SD memory card graphical res. *****\r\n");
//SendOutputBuffer('B');
#endif

if((retVal=init_sd_card())!=MAIN_OK)                              {return retVal;}
if((retVal=write_to_file(FILE_TYPE_GRAPHIC_RESULTS, 0))!=MAIN_OK) {return retVal;}
if((retVal=close_file_and_unmount())!=MAIN_OK)                    {return retVal;}   

return retVal;
}

unsigned int store_numerics_to_sd_card(unsigned int mem_loc)
{
unsigned int retVal;

define_path(FILE_TYPE_NUMERIC_RESULTS, mem_loc);

#if DEBUG
//sprintf(OutputBuffer, "***** SD memory card numerical res. *****\r\n");
//SendOutputBuffer('B');
#endif

if((retVal=init_sd_card())!=MAIN_OK)                              {return retVal;}
if((retVal=write_to_file(FILE_TYPE_NUMERIC_RESULTS, 0))!=MAIN_OK) {return retVal;}
if((retVal=close_file_and_unmount())!=MAIN_OK)                    {return retVal;}   

return retVal;
}

extern void get_real_adc_values(void);

unsigned int read_from_file(unsigned char file_type, char mem_loc_res[])
{
unsigned int i;
unsigned int proba;
char mem_loc_res2[MAX_NUMBER_MEM_LOC]; //nece da ucita drugacije...
/* open a new file and set read access mode */
if ((f_open(&file,path,FA_READ))!=FR_OK){return(0);}

#if DEBUG
sprintf(OutputBuffer, "File opened...:\r\n");
SendOutputBuffer('B');
#endif 

if(file_type==FILE_TYPE_MEM_ORGANISATION)
{
	#if DEBUG
sprintf(OutputBuffer, "PROLAZ:\r\n");
SendOutputBuffer('B');
#endif 
}

//IZ NEKOGA RAZLOGA OVAJ SWITCH USLOV NE RADI KAKO TREBA!?!?!
//zato je napisan dole preko if-a
switch(file_type)
{
	case FILE_TYPE_GRAPHIC_RESULTS:    
			
						#if DEBUG
           sprintf(OutputBuffer, "GRAPHIC results...\r\n");                                      
           SendOutputBuffer(COMM.port);   
           #endif   			
		
        /* write data to file */  
        //Ovdje treba napraviti isto razvrstavnje, da li se trazi sa PC-a ili od drugog kontrolera:
        //Za sada napravljeno samo sa PC-a
        //wait 'A' from PC before sending data:         
        wait_char_from_pc(1);                                                                                                                                                                                                                                                                         
    
        for(i=0; i<TESTPAR.test_duration; i++)
        {                                            
            //ADC_8bit_results.CURRENT_H[0]=0;              
            if ((f_read(&file,&ADC_8bit_results,sizeof(ADC8BIT_STRUCT)-1,&nbytes))!=FR_OK){return(0);} 
						//ovo ce da procita sa SD kartice u ADC_8bit_results stucturu i da razvrsta podatke!
						//TODO: getreal adc values
            get_real_adc_values(); 
            //Ovdje sada treba razvrsati da li rezultat ide direkno prema PC-u ili se storira nazad u SRAM!
            //za sada je napravljeno samo kao da ide iz DV Wina:
            send_measuring_samples_to_pc(); 
                                                                      
            #if 0
            //Ovdje treba snimiti rezultate nazad u memoriju ili ih direktno slati na DV Win.                        
            if(ADC_8bit_results.CURRENT_H[0]!=123)
            {
               sprintf(OutputBuffer, "Error Result:%u\r\n", i);                                      
               SendOutputBuffer('B');                                                                
            }    
            else
            {
               sprintf(OutputBuffer, "Result:%u OK\r\n", i);                                      
               SendOutputBuffer('B');          
            }
            #endif                           
        }         
        #if DEBUG
            sprintf(OutputBuffer, "Gr. results loaded from SD card\r\n");
            SendOutputBuffer('B');
        #endif                           
        //Razvrstati da li ide iz DV WINa ili sa PC.
        //Za sada samo sa PC-a:
        send_data_transfer_over_status();
    
    break;
    case FILE_TYPE_NUMERIC_RESULTS:     
            
           #if DEBUG
                //TESTPAR.op_sequence=OPEN_TEST;
           #endif  

					 #if DEBUG
           sprintf(OutputBuffer, "Numerical results...\r\n");                                      
           SendOutputBuffer(COMM.port);   
           #endif   			
                              
           if ((f_read(&file,&TESTPAR,sizeof(TESTPAR_STRUCT)-1,&nbytes))!=FR_OK){return(0);}
           if ((f_read(&file,&BDATA,sizeof(BDATA_STRUCT)-1,&nbytes))!=FR_OK){return(0);}
           if ((f_read(&file,&TDATA,sizeof(TDATA_STRUCT)-1,&nbytes))!=FR_OK){return(0);}
           if ((f_read(&file,&TIMEDATE,sizeof(TIMEDATE_STRUCT)-1,&nbytes))!=FR_OK){return(0);}
           if ((f_read(&file,&MainC_Results,sizeof(MC_RESULTS_STRUCT)-1,&nbytes))!=FR_OK){return(0);}
           if ((f_read(&file,&Current_Results,sizeof(CURR_RESULTS_STRUCT)-1,&nbytes))!=FR_OK){return(0);}
           if ((f_read(&file,&AUX_Results,sizeof(AUX_RESULTS_STRUCT)-1,&nbytes))!=FR_OK){return(0);}
           if ((f_read(&file,&AnalogCH_Results,sizeof(ANALOG_RESULTS_STRUCT)-1,&nbytes))!=FR_OK){return(0);}
           if ((f_read(&file,&LTransd_Results,sizeof(LINEAR_RESULTS_STRUCT)-1,&nbytes))!=FR_OK){return(0);}
           if ((f_read(&file,&DTransd_Results,sizeof(DIGITAL_RESULTS_STRUCT)-1,&nbytes))!=FR_OK){return(0);} 
           if ((f_read(&file,&RMO_Results,sizeof(RMO_RESULTS_STRUCT)-1,&nbytes))!=FR_OK){return(0);}   
  
                      
           #if 0
           sprintf(OutputBuffer, "Numerical results loaded from SD card\r\n");                                      
           SendOutputBuffer('B');                                                                  
           if(TESTPAR.op_sequence!=CLOSE_TEST)
           {
           sprintf(OutputBuffer, "Checking loaded values...\r\n");                                      
           SendOutputBuffer(COMM.port);
           delay_ms(1000);
           sprintf(OutputBuffer, "Everything OK!...\r\n");                                      
           SendOutputBuffer(COMM.port);                                 
           }
           #endif    
    break;            
    case FILE_TYPE_MEM_ORGANISATION:
		default:
						
					 #if DEBUG
           sprintf(OutputBuffer, "READING MEMORY ORGANISATION!\r\n");                                      
           SendOutputBuffer(COMM.port);   
           #endif   	
		
						for(i=0; i<size_of_mem_loc_res; i++)
						{
							mem_loc_res[i]=0;
						}
		
							//size_of_mem_loc_res2=sizeof(&mem_loc_res);
            //if ((f_read(&file,&mem_loc_res,MAX_NUMBER_MEM_LOC,&nbytes))!=FR_OK)		

						//proba=sizeof(mem_loc_res2);
						//ovo radi:
						  //if ((f_read(&file, &mem_loc_res2, sizeof(mem_loc_res2), &nbytes))!=FR_OK)
						//ovo radi:
							//if ((f_read(&file, &mem_loc_res2, proba, &nbytes))!=FR_OK)
							if ((f_read(&file, &mem_loc_res2, size_of_mem_loc_res, &nbytes))!=FR_OK)
							{
									#if DEBUG
									sprintf(OutputBuffer, "We have a problem");
									SendOutputBuffer('B');
									#endif
									return(ERROR_SDC);
							}
							
							#if 1
							for(i=0; i<sizeof(mem_loc_res2); i++)
							{
									mem_loc_res[i]=mem_loc_res2[i];
							}
							#endif
							
							#if DEBUG
							for(i=0; i<size_of_mem_loc_res; i++)
								{										
								if(mem_loc_res[i]!=NO_TEST)
								{
										sprintf(OutputBuffer, "PROBLEM: %05u\r\n", i);
										SendOutputBuffer('B');
								}
								HAL_Delay(1);
								}
							#endif

            #if DEBUG
           sprintf(OutputBuffer, "Mem locations loaded from file\r\n");                                      
           SendOutputBuffer(COMM.port);   
           #endif   
    break;  
}

return MAIN_OK;
}

unsigned int add_new_res_to_list(unsigned int mem_loc, unsigned int result_type)
{
unsigned int retVal;
char mem_loc_res[MAX_NUMBER_MEM_LOC];
	
size_of_mem_loc_res=sizeof(mem_loc_res);
	
#if DEBUG
sprintf(OutputBuffer, "***** START *****\r\n");
SendOutputBuffer('B');
#endif

define_path(FILE_TYPE_MEM_ORGANISATION, 0);

#if DEBUG
sprintf(OutputBuffer, "***** SD memory card loading result list *****\r\n");
SendOutputBuffer('B');
#endif

if((retVal=init_sd_card())!=MAIN_OK)                                          {return retVal;}

if((retVal=read_from_file(FILE_TYPE_MEM_ORGANISATION, mem_loc_res))!=MAIN_OK) {return retVal;}

if((retVal=close_file_and_unmount())!=MAIN_OK)                                {return retVal;}

#if DEBUG
sprintf(OutputBuffer, "***** SD memory card adding new result *****\r\n");
SendOutputBuffer('B');
#endif

mem_loc_res[mem_loc]=result_type;

#if DEBUG
sprintf(OutputBuffer, "Snimam u lokaciju %05u rezultat %c", mem_loc, mem_loc_res[mem_loc]);
SendOutputBuffer('B');
#endif

retVal=store_list_array_into_mem(mem_loc_res);

return retVal;
}

unsigned int delete_result_from_the_list(unsigned int mem_loc)
{
unsigned int retVal;
retVal=add_new_res_to_list(mem_loc, NO_TEST);
return retVal;
}

unsigned int store_data_to_sd_card(unsigned int mem_loc)
{
unsigned int retVal;

#if DEBUG
AUX_Results.AUX1Time=704;
AUX_Results.AUX2Time=699;
AUX_Results.AUX3Time=710;
AUX_Results.AUX4Time=712;
AUX_Results.AUX5Time=698;
AUX_Results.AUX6Time=695;
#endif

SendProgressBar();
SendProgressBar();
//SendProgressBar();
if((retVal=store_numerics_to_sd_card(mem_loc))!=MAIN_OK){return retVal;}
#if DEBUG
sprintf(OutputBuffer, "Snimio numeriku\n");
SendOutputBuffer('B');
#endif
//SendProgressBar();
SendProgressBar();
if((retVal=store_graphics_to_sd_card(mem_loc))!=MAIN_OK){return retVal;}
#if DEBUG
sprintf(OutputBuffer, "Snimio grafiku\n");
SendOutputBuffer('B');
#endif
SendProgressBar();
#if DEBUG
sprintf(OutputBuffer, "Poslao progress bar\n");
SendOutputBuffer('B');
#endif
if((retVal=add_new_res_to_list(mem_loc, TESTPAR.op_sequence))!=MAIN_OK){return retVal;}
#if DEBUG
sprintf(OutputBuffer, "Snimio u listu\n");
SendOutputBuffer('B');
#endif
//SendProgressBar();
//SendProgressBar();
SendProgressBar();
return retVal;
}

unsigned int read_data_from_sd_card(unsigned int mem_loc, unsigned char graphic_numeric)
{
unsigned int retVal;

#if DEBUG
AUX_Results.AUX1Time=0;
AUX_Results.AUX2Time=0;
AUX_Results.AUX3Time=0;
AUX_Results.AUX4Time=0;
AUX_Results.AUX5Time=0;
AUX_Results.AUX6Time=0;
#endif


if(graphic_numeric==GRAPHICS)
{
    define_path(FILE_TYPE_GRAPHIC_RESULTS, mem_loc);
    #if DEBUG
    sprintf(OutputBuffer, "***** SD memory card graphical res. *****\r\n");
    SendOutputBuffer('B');
		
    #endif
}
else
{
    define_path(FILE_TYPE_NUMERIC_RESULTS, mem_loc);
    #if DEBUG
    sprintf(OutputBuffer, "***** SD memory card numerical res. *****\r\n");
    SendOutputBuffer('B');
	
    #endif
}

if((retVal=init_sd_card())!=MAIN_OK)                            {return retVal;}
if(graphic_numeric==GRAPHICS)
{
    if((retVal=read_from_file(FILE_TYPE_GRAPHIC_RESULTS, 0))!=MAIN_OK) {return retVal;}
}
else
{
    if((retVal=read_from_file(FILE_TYPE_NUMERIC_RESULTS, 0))!=MAIN_OK) {return retVal;}
}

if((retVal=close_file_and_unmount())!=MAIN_OK)                  {return retVal;}   

return retVal;
}

void load_mem_loc_info(char mem_loc_res_aux, unsigned int mem_loc)
{
    if((mem_loc_res_aux>=CLOSE_TEST)&&(mem_loc_res_aux<=STATIC_RESISTANCE))
    {                                                                         
        sprintf(OutputBuffer, "%03d:%c\n", mem_loc, mem_loc_res_aux);
    }                        
    else
    {
        sprintf(OutputBuffer, "%03d:%c\n", mem_loc, NO_TEST);
    }        
}        

unsigned int list_all_mem_locations(void)
{
unsigned int retVal, i;
char mem_loc_res[MAX_NUMBER_MEM_LOC];
	
size_of_mem_loc_res=sizeof(mem_loc_res);

define_path(FILE_TYPE_MEM_ORGANISATION, 0);

#if DEBUG
sprintf(OutputBuffer, "***** SD memory card List mem loc *****\r\n");
SendOutputBuffer('B');
#endif

    if((retVal=init_sd_card())!=MAIN_OK)                                          {return retVal;}
    if((retVal=read_from_file(FILE_TYPE_MEM_ORGANISATION, mem_loc_res))!=MAIN_OK) {return retVal;}
    if((retVal=close_file_and_unmount())!=MAIN_OK)                                {return retVal;}
		
		#if DEBUG
							for(i=0; i<size_of_mem_loc_res; i++)
								{							
								if(mem_loc_res[i]!=NO_TEST)
								{
										sprintf(OutputBuffer, "PROBLEM: %05u\r\n", i);
										SendOutputBuffer('B');
								}
								HAL_Delay(1);
								}
		 #endif

if(retVal==MAIN_OK)  	
{
    for(i=0; i<size_of_mem_loc_res; i++)
    {
           load_mem_loc_info(mem_loc_res[i], i); 
           SendOutputBuffer(COMM.port);					
    }
}

return retVal;
}

unsigned int mem_location_info(unsigned int mem_loc)
{
unsigned int retVal;
char mem_loc_res[MAX_NUMBER_MEM_LOC];
	
size_of_mem_loc_res=sizeof(mem_loc_res);

define_path(FILE_TYPE_MEM_ORGANISATION, 0);

#if DEBUG
sprintf(OutputBuffer, "***** SD memory card List mem loc *****\r\n");
SendOutputBuffer('B');
#endif

    if((retVal=init_sd_card())!=MAIN_OK)                                          {return retVal;}
    if((retVal=read_from_file(FILE_TYPE_MEM_ORGANISATION, mem_loc_res))!=MAIN_OK) {return retVal;}
    if((retVal=close_file_and_unmount())!=MAIN_OK)                                {return retVal;}     
    
    load_mem_loc_info(mem_loc_res[mem_loc], mem_loc); 

return retVal;
}
