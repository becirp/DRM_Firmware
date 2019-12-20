//PRINTING LINES:
#ifndef PRINTER_TEXT_STRUCTURE
#define PRINTER_TEXT_STRUCTURE
typedef struct _PRINTER_TEXT_
{
    char Language;                              //1
    //Text:
    char Out_Of_Range[24];                      //2
    char TestRaport[16];                        //3
    char CircuitBreakerAnalyzerAndTimer[40];    //4
    char Date[6];                               //5
    char Time[9];                               //6
    char BreakerData[17];                       //7
    char Station[15];                           //8
    char BreakerID[15];                         //9
    char Compartment[14];                       //10
    char SerialNumber[16];                      //11
    char Manufacturer[14];                      //12
    char BreakerType[20];                       //13
    char TestData[18];                          //14
    char TypeOfTest[15];                        //15
    char CompanyName[16];                       //16
    char Operater[12];                          //17
    char Comments[13];                          //18
    ////////////////////////////////     
    char GeneralTestConditions[30];             //19
    char Sequence[9];                           //20
    char MeasuringTime[16];                     //21
    char ClosePulse[23];                        //22
    char CO_Delay[19];                          //23
    char FirstContactTouch[20];                 //24
    char OpenPulse[20];                         //25
    char OC_Delay[19];                          //26
    char OCO_Delay[20];                         //27
    char Trigger[12];                           //28
    char External[9];                           //29
    char Internal[9];                           //30
    char Transducer[11];                        //31
    char Digital[10];                           //32
    char Linear[9];                             //33
    char BreakUnitsPerPhase[40];                //34
    char TimingResults[20];                     //35
    //////////////////////////////// 
    char ResistanceMeasurement[23];             //36
    ///////////////////////////////
    char OpeningTime[21];                       //37
    char ClosingTime[21];                       //38
    char PhaseOpeningTime[21];                  //39
    char PhaseClosingTime[21];                  //40
    char PhaseSync[21];                         //41
    char BreakerOpeningTime[21];                //42
    char BreakerClosingTime[21];                //43
    char AUX_OpeningTime[21];                   //44
    char AUX_ClosingTime[21];                   //45
    char O_Sync[21];                            //46
    char C_Sync[21];                            //47
    char CloseOpenTime[21];                     //48
    char Phase_CO_Time[21];                     //49
    char Breaker_CO_Time[21];                   //50
    char ReclosingTime[21];                     //51
    char PhaseReclosingTime[21];                //52
    char BreakerReclosingTime[21];              //53
    char OpenCloseTime[21];                     //54
    char Phase_OC_Time[21];                     //55
    char Breaker_OC_Time[21];                   //56
    char O1_Sync[21];                           //57
    char O2_Sync[21];                           //58
    char PreinsertionRTime[21];                 //59
    char ResistorOnTime[21];                    //60
    char ResistanceValue[21];                   //61
    char BreakerRSync[21];                      //62
    char Phase_R_ON_Time[21];                   //63
    ////////////////////////////////
    char MaximumCurrent[16];                    //64
    char ClosingCoil[20];                       //65
    char TripCoil[19];                          //66
    //////////////////////////////// 
    char Analog[11];                             //67
    char Range[9];                              //68
    ////////////////////////////////
    char MotionResults[17];                     //69
    char ContactWipe[21];                       //70
    char MotionResultsAre[21];                  //71
    char AvailableOnlyFor[21];                  //72
    char OpenAndCloseTest[21];                  //73
    char txtCurrentResults[25];                 //74
    char txtAnalogChannelResults[25];           //75 
    char txtTransducerResults[25];              //76 
    char txtPreInsertionResistor[30];           //77
    char txtStroke[3];                          //78
    char txtOvertravel[3];                      //79
    char txtRebound[3];                         //80 
    char txtGraph[20];                          //81
    char txtHorScale[25];                       //82
    char txtVerScale[25];                       //83 
    char txtClosingCoilCurrent[35];             //84 
    char txtCurrent[10];                        //85 
    char txtMainContacts[20];                   //86
    char txtAuxCurrents[40];                    //87
    char txtCurrentsAux[40];                    //88
}PRINTER_TEXT_STRUCT;
#endif

extern PRINTER_TEXT_STRUCT activetext;
//extern __eeprom PRINTER_TEXT_STRUCT printer_texts[1];
extern PRINTER_TEXT_STRUCT printer_texts[2];
