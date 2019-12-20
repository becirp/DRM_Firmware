// Standard Input/Output functions
#include <stdio.h>
#include "main.h"
#include "remote.h"
#include "bios.h"
#include "comm.h"
#include "control.h"
#include "calib.h"
#include "adc.h"
#include "printer.h"
#include "usb_flash_drive.h"

// Remote Commands Definition:
unsigned int RemoteCommandSwitch(void)
{
unsigned char byte;
unsigned int retVal=MAIN_NOK; 	
             
if (InputBuffer[0]=='m')
{
					//Ubaciti Get_Predif_Number_of_Samples_Comm funkciju za skupljanje samplova.
					//Battery charger control
					if((InputBuffer[1]=='B')&&(InputBuffer[2]=='A')&&(InputBuffer[3]=='T'))
					{
									return (Battery_Charger_Control());
					}
					//DRM Read function
					if((InputBuffer[1]=='A')&&(InputBuffer[2]=='D')&&(InputBuffer[3]=='C'))
					{
									return (Read_DRM_ADC_Results());
					}
					//DRM DIN function
					if((InputBuffer[1]=='D')&&(InputBuffer[2]=='A'))
					{
									return (Write_DRM_CH1());
					}
					//Test pin toggle
					if((InputBuffer[1]=='T')&&(InputBuffer[2]=='O')&&(InputBuffer[3]=='G'))
					{             
									return(Toggle_Pins());
					}
					//ADC Read functions:
					if((InputBuffer[1]=='A')&&(InputBuffer[2]=='D')&&(InputBuffer[3]=='R'))
					{             
									return (Read_ADC_Res_COMM());                                                                                
					}     
					//Read One ADC Channel                  
					if((InputBuffer[1]=='A')&&(InputBuffer[2]=='D')&&(InputBuffer[3]!='R'))
					{                                                   
									return (Read_One_ADC_CH_COMM());
					}
					//Remote Enable:
					if((InputBuffer[1]=='R')&&(InputBuffer[2]=='M')&&(InputBuffer[3]=='E'))
					{                            
									return (SetRemoteControl_COMM(REMOTE_ENABLE));
					}               
					//Remote Disable:    
					if((InputBuffer[1]=='R')&&(InputBuffer[2]=='M')&&(InputBuffer[3]=='D'))
					{                        
									return (SetRemoteControl_COMM(REMOTE_DISABLE));               
					}
					//Reset Digital Transducer counter:                
					if((InputBuffer[1]=='A')&&(InputBuffer[2]=='R')&&(InputBuffer[3]=='D'))
					{                               
								 return (DT_Reset_Counter_COMM());                       
					}       					
					//AUX DRY / WET Control (used by CAT testing software):
					if((InputBuffer[1]=='A')&&(InputBuffer[2]=='X')&&((InputBuffer[3]=='D')||(InputBuffer[3]=='W')))
					{
									return(AUX_DryWetControl_Command(InputBuffer[3]));
					}      					
					//Set COMM port A or B:    
					if((InputBuffer[1]=='P')&&((InputBuffer[2]=='A')||(InputBuffer[2]=='B')))
					{       
								 return (SetCommPort_COMM());          
					}  
					// Check Breaker Status:
					//TODO: REQUIRES MODIFICATION
					if((InputBuffer[1]=='B')&&(InputBuffer[2]=='S')&&((InputBuffer[3]==ONE_BREAK_UNIT_PER_PHASE)||(InputBuffer[3]==TWO_BREAK_UNITS_PER_PHASE)||(InputBuffer[3]==FOUR_BREAK_UNITS_PER_PHASE)))
					{                                       
									return (CheckBreakerStatus_COMM(InputBuffer[3]));                                        
					}
					// BSG test commands:
					if((InputBuffer[1]=='B')&&(InputBuffer[2]=='S')&&(InputBuffer[3]=='G'))
					{                                                               
									return (BSGTest_COMM());                                        
					}                      
					//Set BSG Mode (koristi se za CAT Tester):                
					if((InputBuffer[1]=='B')&&(InputBuffer[2]=='G')&&((InputBuffer[3]=='0')||(InputBuffer[3]=='1')||(InputBuffer[3]=='2')))
					{
									return (Set_Test_Mode_COMM(InputBuffer[3]));            
					}					
					//Set Measuring Parameters:
					if((InputBuffer[1]=='S')&&((InputBuffer[2]>='1')&&(InputBuffer[2]<='4')))
					{                                                         
									return (SetMeasParameters_COMM());   
					}     
          // Enter Breaker Test Data:
          if((InputBuffer[1]=='B')&&(InputBuffer[2]=='T'))
          {                                                                  
                  return (EnterBreakerTestDataFileName_COMM());                               
          }   					
					//Units for transfer functions on analog CH's:
					if((InputBuffer[1]=='U')&&((InputBuffer[2]>='1')&&(InputBuffer[2]<='6')))
					{                                                         
									return (EnterTFUnits_COMM());                                          
					}   
					//Set Control Pulse Lenghts:
					if((InputBuffer[1]=='T')&&((InputBuffer[2]>='1')&&(InputBuffer[2]<='6'))) 
					{                                                         
									return (SetPulseLenghts_COMM()); 
					}                                        
					//Set Parameters for Linear transducer:
					if((InputBuffer[1]=='L')&&((InputBuffer[2]>='0')&&(InputBuffer[2]<='2')))
					{                                                         
									return (SetLTransdParameters_COMM());                
					}    	
					//Enter Time & Date
					if((InputBuffer[1]=='T')&&(InputBuffer[2]=='D'))
					{                                                          
									return (EnterTimeDate_COMM());
					}   
					//Measurement Start Command:
					if((InputBuffer[1]=='M')&&(InputBuffer[2]=='S')&&(InputBuffer[3]=='C'))
					{                                                       
									retVal=MeasurementStart_Comm();  									

									#if 1
									if(retVal==NO_EXTERNAL_TRIGGER)
									{                               
													sprintf(OutputBuffer, "MS5"); //Ovo vraca u slucaju ako nema externog triggera
													retVal=MAIN_OK;
													return retVal;    
									}                        
									
									if(retVal==STOP_BUTTON)
									{     
													sprintf(OutputBuffer, "MS6");
													retVal=MAIN_OK;                                   
													return retVal;
									}    
									#endif									
					return retVal;                
					} 					
					//Get Test Parameters (provjeriti da li ova komanda uopste treba!
					if((InputBuffer[1]=='R')&&(InputBuffer[2]=='T'))
					{          
							return(SendMeasurementParameters_Comm());                         
					}     					
					//Relay & IGBT control commands:
					if((InputBuffer[1]=='R')&&((InputBuffer[2]=='A')||(InputBuffer[2]=='C')||(InputBuffer[2]=='X')))
					{                                              
							return (Relay_IGBT_Control());
					}       
					//Send test parameters:
					if((InputBuffer[1]=='V')&&(InputBuffer[2]=='M')&&(InputBuffer[3]=='D'))
					{
							return(Get_Test_Settings_Comm());
					}  
					//Get measurement data command:
					if((InputBuffer[1]=='G')&&(InputBuffer[2]=='S')&&(InputBuffer[3]=='0'))
					{
							return(Get_Measurement_Data_Comm());   
					}   					
					//Set RMO results:
					if((InputBuffer[1]=='S')&&(InputBuffer[2]=='D')&&(InputBuffer[3]!='C'))
					{
							return(Set_RMO_Results_Comm());                                      
					}  
					//Get RMO results:    
					if((InputBuffer[1]=='S')&&(InputBuffer[2]=='M')&&(InputBuffer[3]=='N'))
					{                                   
							return(Get_RMO_Results_Comm());
					}     
					//Internal memory SD card commands:                        
					if((InputBuffer[1]=='S')&&(InputBuffer[2]=='D')&&(InputBuffer[3]=='C'))
					{     
							//deselect USB flash drive:
								 
							return(Internal_SD_Mem_Comm());
					}   					
					//Set active language:                        
					if((InputBuffer[1]=='S')&&(InputBuffer[2]=='A')&&(InputBuffer[3]=='L'))
					{                                            
							return(Set_Active_Language_Comm());
					}        
					// CALIBRATION /////////////////////////////////////////////////////
					#if 1
					if(((InputBuffer[1]=='U')||(InputBuffer[1]=='V'))&&((InputBuffer[2]=='R')||(InputBuffer[2]=='W')||(InputBuffer[2]=='U')||(InputBuffer[2]=='P')))
					{    
							//write calibration coeficient:
							if(InputBuffer[2]=='W')
							{                                     
									return(Write_Calib_Coef_Comm(InputBuffer[1], InputBuffer[3], InputBuffer[4], InputBuffer[5]));                       
							}                  
							
							//read calibration coeficient:                                                                              
							if(InputBuffer[2]=='R')
							{
									return(Read_Calib_Coef_Comm(InputBuffer[1], InputBuffer[3], InputBuffer[4], InputBuffer[5]));
							} 
																																 
							//store calibration coeficients from SRAM to EEPROM:
							if((InputBuffer[2]=='P')&&(InputBuffer[3]=='A'))
							{                                             
									OK_Response(); 
									return(Store_Calib_Coef_to_EEPROM_Comm());
							}
							
							//load calibration coeficients from EEPROM to SRAM:
							if((InputBuffer[2]=='U')&&(InputBuffer[3]=='A'))
							{
									OK_Response(); 
									return(Read_Calib_Coef_from_EEPROM_Comm());                    
							}
					}                                  
					//Reset calibration (set unity calibration coeficients RSTC:          
					if((InputBuffer[1]=='R')&&(InputBuffer[2]=='S')&&(InputBuffer[3]=='T')&&(InputBuffer[4]=='C'))
					{                    
							OK_Response();
							return(Reset_Calibration_COMM());
					}                  
					//Get active calibration coeficients:
					if((InputBuffer[1]=='G')&&(InputBuffer[2]=='A')&&(InputBuffer[3]=='C')&&(InputBuffer[4]=='C'))
					{
							OK_Response();
							return(Get_Active_Calibration_Coef_COMM());                
					}		
					// END CALIBRATION //////////////////////////////////////////////////
					#endif		
					
					
					// DISPLAY RESULTS //////////////////////////////////////////////////
					if((InputBuffer[1]=='R')&&(InputBuffer[2]=='R'))
					{
									PuniPrintBuffer(InputBuffer[3], InputBuffer[4], InputBuffer[5], 0, InputBuffer[6]); //zadnji InputBuffer[6] se odnosi na Preinsertion R
									retVal=MAIN_OK;    
					}
					if((InputBuffer[1]=='R')&&(InputBuffer[2]=='2'))
					{
									PuniPrintBufferCurrent(InputBuffer[3], InputBuffer[4], 0);
									retVal=MAIN_OK;                
					}                                               
					if((InputBuffer[1]=='R')&&(InputBuffer[2]=='3'))
					{                        
									PuniPrintBufferAnalogCH(InputBuffer[3], InputBuffer[4], 0);
									retVal=MAIN_OK;                        
					}     
					if((InputBuffer[1]=='R')&&(InputBuffer[2]=='4'))
					{
									PuniPrintBufferMotion(InputBuffer[3], InputBuffer[4], 0);
									retVal=MAIN_OK;                
					}    					
					// END DISPLAY RESULTS //////////////////////////////////////////////
					
					
					/////////////////////////////////////////
					// SAMO PRIVREMENO STAVLJENO DA MOZE KOMUNICICATI SA CAT66:
					/////////////////////////////////////////                   
					//Komanda koja se odnosila na upravljanje Open Close diodicama:					
					if((InputBuffer[1]=='K')&&(InputBuffer[2]=='C')&&(InputBuffer[3]=='0')&&(InputBuffer[4]=='0')&&(InputBuffer[5]=='0')&&(InputBuffer[6]=='0'))
					{          
									//ugasiti odgovarajuce diode!
									OK_Response();                                                                                                                                                                                                              
									retVal=MAIN_OK;                                                                                                                                                                                       
									return(MAIN_OK);                
					}              
					//Komanda koja se odnosila na reset slave uredjaja (Nema potrebe da vise bude prisutna!:
					if((InputBuffer[1]=='R')&&(InputBuffer[2]=='S')&&(InputBuffer[3]=='D'))
					{                                                      
							OK_Response();                                                                                                                                                                                                              
							return(MAIN_OK);
					}      
					if((InputBuffer[1]=='R')&&(InputBuffer[2]=='G')&&(InputBuffer[3]=='F'))
          {
              sprintf(OutputBuffer, "DRM Control Board V1.3");
              return (MAIN_OK);
          }   
					if((InputBuffer[1]=='M')&&(InputBuffer[2]=='T')&&(InputBuffer[3]=='C'))
					{
							sprintf(OutputBuffer, "SRAM_TEST_PROCEDURE_STARTS");
						  SendOutputBuffer('B');
						
							retVal=MemoryTest_Comm();						
						
							if(retVal==MAIN_OK)
							{
								sprintf(OutputBuffer, "SRAM_MEM_OK");                                                                                                                                                                                        
							}
							else
							{
								sprintf(OutputBuffer, "SRAM_MEM_ERROR");
							}
					}
					//Memory Write Byte                      
					//Format: MRB+XXXXXXX+YYYYY	
					//XXXXXXX
					//YYYYY - data 0-65535  					
					if((InputBuffer[1]=='M')&&(InputBuffer[2]=='W')&&(InputBuffer[3]=='B'))
					{
									return(MemoryWriteByte_Comm());
					}                                                       
					//Memory Read Byte                      
					//Format: MRB+XXXXXXX					
					//XXXXXXX - address 0-2097152                					
					if((InputBuffer[1]=='M')&&(InputBuffer[2]=='R')&&(InputBuffer[3]=='B'))
					{
									return(MemoryReadByte_Comm());                     
					}   
					/////////////////////////////////////////
					// PRINT RESULTS:
					/////////////////////////////////////////                              
					if((InputBuffer[1]=='P')&&(InputBuffer[2]=='R'))
					{                                    
									retVal=PrintResults_Comm(InputBuffer[3]);   
									switch(retVal)
									{                          
											case MAIN_OK: OK_Response();             break; //OK    
											case     1: sprintf(OutputBuffer, "NP"); break; //No paper
											default   : sprintf(OutputBuffer, "HE"); break; //Printer Error
									}
									retVal=MAIN_OK;                
					} 
					#if 1
					///USB FLASH DRIVE:
					//Inicijalizacija USB Stick:                        
					if((InputBuffer[1]=='U')&&(InputBuffer[2]=='S')&&(InputBuffer[3]=='B'))
					{                                                      
							 																																						 
							if(InputBuffer[4]=='F')
							{                                  
									reset_flash_drive_MCU(); 
									
									HAL_Delay(100);
														 
									retVal=InitStick_Comm(1); 
									
									if(retVal==STICK_ERROR)    {sprintf(OutputBuffer, "SE"); reset_flash_drive_MCU();} //Stick Error
									else if(retVal==FILE_EXIST){sprintf(OutputBuffer, "FE"); } //File  Exists
									else                       {sprintf(OutputBuffer, "NF"); reset_flash_drive_MCU();} //No File  
																					
									return(MAIN_OK);
							}
							#if 1
							else if(InputBuffer[4]=='W')
							{                       
									retVal=InitStick_Comm(2);
									if(retVal==STICK_ERROR){sprintf(OutputBuffer, "SE"); reset_flash_drive_MCU(); } // Stick Error
									else{OK_Response();}                                   // Upis OK!                        
									return(MAIN_OK);

							}
							#endif
					}
					#endif      					

}
return retVal;
}

