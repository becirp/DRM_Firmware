extern void BSGRelayControl(unsigned char BSG_RELAY, unsigned char On_Off);
extern void BSGBuckControl(unsigned char On_Off);
extern void BSGChargeDischargeRELControl(unsigned int CHARGE_DISCHARGE_REL, unsigned char On_Off);
extern void BSGSOneBPPF_ON_Sequence(void);
extern void BSGSTwoBPPF_ON_Sequence(void);
extern void BSG_OFF_Sequence(void);
extern void MainContactsInitSequence(void);
extern void OSGConfigON(void);
extern void BSGConfigON(void);
extern void SetAnalogCHRange(unsigned char Channel, unsigned char Range); 
extern void CC_RelayControl(unsigned char CC_RELAY, unsigned char On_Off);
extern void AUX_dry_wet_control(void);
extern void enable_printer_communication(void);
extern void disable_printer_communication(void);

extern void I2C_OPEN_LED_control(unsigned char LED_CONTROLER, unsigned char On_Off);
extern void I2C_CLOSE_LED_control(unsigned char LED_CONTROLER, unsigned char On_Off);

#define LINEAR_TRANDUCER_ADC_6_CHS 0

#define REMOTE_ENABLE  (unsigned char) 'E'
#define REMOTE_DISABLE (unsigned char) 'D'

#define COMM_PORT_A (unsigned char) 'A'
#define COMM_PORT_B (unsigned char) 'B'

#define NUMBER_OF_MAIN_CONTACTS 12
#define NUMBER_OF_CURRENT_CHS    6
#define NUMBER_OF_ANALOG_CHS     6
#define NUMBER_OF_LT_CHS         3
#define NUMBER_OF_DT_CHS         3
#define NUMBER_OF_AUX_CHS        6

#define USE_60V_ANALOG_CH_RANGE  1 //if 1 analog ch range 60V will not be used, but instead measurement will be conducted on 300V analog CH range

#define MIN_CONSECUTIVE_SAMPLES_FOR_TRIGGER 7 

#define NUMBER_OF_ANALOG_CH_RANGES 4
#define NUMBER_OF_CURRENT_CH_RANGES 1

extern unsigned char language;

#ifndef TESTPARAMETERS_STRUCTURE
#define TESTPARAMETERS_STRUCTURE
typedef struct _TESTPAR_
{
//SREDJENO:
unsigned char device;            //Device identificator
//test flow control
unsigned int  test_duration;     //Test duration in [ms]
unsigned char sampl_resol;       //Sampling resolution
unsigned char break_units;       //Number of break units per phase
unsigned char mains_freq;        //Mains frequency (50/60Hz)
unsigned char time_base;         //Time base (ms/s/cy)
unsigned char bs_grounded;       //Both Sides Grounded 
//analog channels:
unsigned char analog1_gain;      //Analog Channel 1 Gain
unsigned char analog2_gain;      //Analog Channel 2 Gain
unsigned char analog3_gain;      //Analog Channel 3 Gain
unsigned char analog4_gain;      //Analog Channel 4 Gain
unsigned char analog5_gain;      //Analog Channel 5 Gain
unsigned char analog6_gain;      //Analog Channel 6 Gain
unsigned int  analog1tf;         //Analog 1 Transfer Function, mV/A  
unsigned int  analog2tf;         //Analog 2 Transfer Function, mV/A
unsigned int  analog3tf;         //Analog 3 Transfer Function, mV/A
unsigned int  analog4tf;         //Analog 4 Transfer Function, mV/A
unsigned int  analog5tf;         //Analog 5 Transfer Function, mV/A
unsigned int  analog6tf;         //Analog 6 Transfer Function, mV/A
unsigned char analog1tf_unit[3]; //Ovo je jedinca za prikaz prenosnog odnosa. umjesto mV/A, moze biti jedincia /0C itd.
unsigned char analog2tf_unit[3]; //Ovo je jedinca za prikaz prenosnog odnosa. umjesto mV/A, moze biti jedincia /0C itd. 
unsigned char analog3tf_unit[3]; //Ovo je jedinca za prikaz prenosnog odnosa. umjesto mV/A, moze biti jedincia /0C itd. 
unsigned char analog4tf_unit[3]; //Ovo je jedinca za prikaz prenosnog odnosa. umjesto mV/A, moze biti jedincia /0C itd.
unsigned char analog5tf_unit[3]; //Ovo je jedinca za prikaz prenosnog odnosa. umjesto mV/A, moze biti jedincia /0C itd.
unsigned char analog6tf_unit[3]; //Ovo je jedinca za prikaz prenosnog odnosa. umjesto mV/A, moze biti jedincia /0C itd.
//external trigger:
unsigned char ext_trigg_wtm;      //Waiting time for external trigger function
unsigned char ext_trigg_src;     //External trigger source 
unsigned int  ext_trigg_lev;     //External trigger level
unsigned char ext_trigg_edg;     //External trigger edge
//auxilliary channels:
unsigned char aux1_dry_wet;      //AUX CH1 Dry/Wet selection
unsigned char aux2_dry_wet;      //AUX CH2 Dry/Wet selection
unsigned char aux3_dry_wet;      //AUX CH3 Dry/Wet selection
unsigned char aux4_dry_wet;      //AUX CH4 Dry/Wet selection
unsigned char aux5_dry_wet;      //AUX CH5 Dry/Wet selection
unsigned char aux6_dry_wet;      //AUX CH6 Dry/Wet selection
//transducers:
unsigned int  transd1_tf;        //Transducer 1 Transfer function
unsigned int  transd2_tf;        //Transducer 2 Transfer function
unsigned int  transd3_tf;        //Transducer 3 Transfer function
unsigned char travel_unit;       //Transducer Travel unit (mm/inch)
unsigned char transd_type;       //Motion Transducer type  
unsigned char transd_ch;         //Motion Transducer channel
unsigned char oper_mech;         //Circuit Breaker Operating Mechanism 
//coil control:
unsigned char op_sequence;       //Breaker Opearting Sequence
unsigned char o_coil_phase;      //Open Coil Phase to drive
unsigned char c_coil_phase;      //Close Coil Phase to drive
unsigned int o_pulse_lenght;     //Open Coil pulse lenght
unsigned int c_pulse_lenght;     //Close Coil pulse lenght
unsigned int trip_free_delay;    //Trip Free delay time
unsigned int reclosing_delay;    //Reclosing delay time
unsigned int oco_delay1;         //OCO delay 1 time
unsigned int oco_delay2;         //OCO delay 2 time
//drm test:
unsigned char drm_test;          //Dynamic Resistance Measurement
unsigned char drm_current;       //DRM Test current
////////////////////////////////////
unsigned int tr_lenght_mm[3];
unsigned int tr_lenght_inch[3];
unsigned int tr_gain[3];
unsigned int tr_offset[3];
//Tri naknadno ubacene varijable:
unsigned int TriggStart; 
unsigned int TriggStop;
unsigned int VrijemeResetovanjaAdrese;
}TESTPAR_STRUCT;

#endif

#ifndef COMMUNICATION_STRUCTURE
#define COMMUNICATION_STRUCTURE
typedef struct _COMM_
{
    unsigned char remote_enable;
    unsigned char port;
    unsigned char change_port;
}COMM_STRUCT;
#endif

#ifndef BDATA_STRUCTURE
#define BDATA_STRUCTURE
// BREAKER DATA
typedef struct _BDATA_
{
    char BreakerID[20];
    char StationName[20];
    char Compartment[20];
    char Manufacturer[20];
    char SerialNumer[20];
    char BreakerType[20];
}BDATA_STRUCT;
#endif

#ifndef TDATA_STRUCTURE
#define TDATA_STRUCTURE
// TEST DATA
typedef struct _TDATA_
{
    char TypeOfTest[20];
    char CompanyName[20];
    char Operater[20];
}TDATA_STRUCT;
#endif

#ifndef TIMEDATE_STRUCTURE
#define TIMEDATE_STRUCTURE
// TIME & DATE
typedef struct _TIMEDATE_
{
    unsigned char yy;
    unsigned char mm;
    unsigned char dd;
    unsigned char hh;
    unsigned char min;
    unsigned char sec;
}TIMEDATE_STRUCT;
#endif

#ifndef FLASH_DRIVE_STRUCTURE
#define FLASH_DRIVE_STRUCTURE
// USB FLASH DRIVE:
typedef struct _STICK_
{
	unsigned char dt0;
	unsigned char dt1;
	unsigned char dt2;
	unsigned char dt3; 
	unsigned char filen[13];
    unsigned char dirname[13];
	unsigned char s; 
    char ModelInfo[20];
}STICK_STRUCT;
#endif

#ifndef CALIB_STRUCTURE
#define CALIB_STRUCTURE
typedef struct _CALIB_
{
    unsigned int  analogch_gain[NUMBER_OF_ANALOG_CHS][NUMBER_OF_ANALOG_CH_RANGES];
    unsigned int  analogch_offset[NUMBER_OF_ANALOG_CHS][NUMBER_OF_ANALOG_CH_RANGES];
    unsigned char analogch_sign[NUMBER_OF_ANALOG_CHS][NUMBER_OF_ANALOG_CH_RANGES];    
    unsigned int  analogch_CC_offset[NUMBER_OF_ANALOG_CHS];
    unsigned char analogch_CC_sign[NUMBER_OF_ANALOG_CHS];        
    unsigned int  currentch_gain[NUMBER_OF_CURRENT_CHS];
    unsigned int  currentch_offset[NUMBER_OF_CURRENT_CHS];
    unsigned char currentch_sign[NUMBER_OF_CURRENT_CHS];
    unsigned int  linear_tr_ch_gain[NUMBER_OF_LT_CHS];
    unsigned int  linear_tr_ch_offset[NUMBER_OF_LT_CHS];
    unsigned char linear_tr_ch_sign[NUMBER_OF_LT_CHS];            
}CALIB_STRUCT;

#endif


#ifndef MC_RESULTS_STRUCTURE
#define MC_RESULTS_STRUCTURE
typedef struct _MC_RES_
{
unsigned int FirstOpeningTime[6][2];  
unsigned int SecondOpeningTime[6][2];   
unsigned int ClosingTime[6][2];
unsigned int PreinsertionTime[6][2];
unsigned int PreinsertionResistance[6][2];
}MC_RESULTS_STRUCT;
#endif

#ifndef CURR_RESULTS_STRUCTURE
#define CURR_RESULTS_STRUCTURE
typedef struct _CURR_RES_
{
unsigned int MaxCurrentBreaking[3];
unsigned int MaxCurrentClosing[3];
unsigned int MaxCurrentBreakingTime[3];
unsigned int MaxCurrentClosingTime[3];
}CURR_RESULTS_STRUCT;
#endif

#ifndef AUX_RESULTS_STRUCTURE
#define AUX_RESULTS_STRUCTURE
typedef struct _AUX_RES_
{
unsigned int AUX1Time;
unsigned int AUX2Time;
unsigned int AUX3Time;
unsigned int AUX4Time;
unsigned int AUX5Time;
unsigned int AUX6Time;
}AUX_RESULTS_STRUCT;
#endif

#ifndef ANALOG_RESULTS_STRUCTURE
#define ANALOG_RESULTS_STRUCTURE
typedef struct _ANALOG_RES_
{
unsigned int MaxVoltageCH[6];
unsigned int MinVoltageCH[6];
unsigned int MaxVoltageCHTime[6];
unsigned int MinVoltageCHTime[6]; 
}ANALOG_RESULTS_STRUCT;
#endif

#ifndef LINEAR_RESULTS_STRUCTURE
#define LINEAR_RESULTS_STRUCTURE
typedef struct _LINEAR_RES_
{
unsigned int TotalTravel[3];
unsigned int OverTravel[3];
unsigned int OverTravelTime[3];
unsigned int Rebound[3];
unsigned int ReboundTime[3];
unsigned int ContactWipe[6][2];
signed int Max[3];
signed int Min[3];
}LINEAR_RESULTS_STRUCT;
#endif

#ifndef DIGITAL_RESULTS_STRUCTURE
#define DIGITAL_RESULTS_STRUCTURE
typedef struct _DIGITAL_RES_
{
signed long TotalTravel[3];
signed long OverTravel[3];
unsigned int OverTravelTime[3];
signed long Rebound[3];
unsigned int ReboundTime[3];
signed long ContactWipe[6][2];
signed long Max[3];
signed long Min[3];
}DIGITAL_RESULTS_STRUCT;
#endif

#ifndef RMO_RESULTS_STRUCTURE
#define RMO_RESULTS_STRUCTURE
typedef struct _RMO_RES_
{
unsigned char static_res[150];
}RMO_RESULTS_STRUCT;
#endif

#ifndef FLASH_DRIVE_STRUCTURE
#define FLASH_DRIVE_STRUCTURE
// USB FLASH DRIVE:
typedef struct _STICK_
{
	unsigned char dt0;
	unsigned char dt1;
	unsigned char dt2;
	unsigned char dt3; 
	unsigned char filen[13];
    unsigned char dirname[13];
	unsigned char s; 
    char ModelInfo[20];
}STICK_STRUCT;
#endif


extern TESTPAR_STRUCT TESTPAR;
extern COMM_STRUCT COMM;
extern BDATA_STRUCT BDATA;
extern TDATA_STRUCT TDATA;
extern TIMEDATE_STRUCT TIMEDATE;
extern STICK_STRUCT STICK;
extern CALIB_STRUCT CALIB;
extern MC_RESULTS_STRUCT      MainC_Results;
extern CURR_RESULTS_STRUCT    Current_Results;
extern AUX_RESULTS_STRUCT     AUX_Results;
extern ANALOG_RESULTS_STRUCT  AnalogCH_Results;
extern LINEAR_RESULTS_STRUCT  LTransd_Results;
extern DIGITAL_RESULTS_STRUCT DTransd_Results;
extern RMO_RESULTS_STRUCT     RMO_Results;
extern STICK_STRUCT           STICK;

#define MAX_NUMBER_MEM_LOC 500
#define GRAPHICS 'G'
#define NUMERICS 'N'

#define ATMEL_STUDIO_DEBUG 0
#define ST_LINK_DEBUG 0
#define DEBUG 0
#define FAKE_ADC_RESULTS 0

#define VIBRO_BOX 0
#define DELAY_1_CY #asm("nop");
//#define DELAY_1_CY {#asm("nop"); #asm("nop");}
//#define DELAY_1_CY delay_us(1);

#define COM_PORT_A (unsigned char)'A'
#define COM_PORT_B (unsigned char)'B'

#define MAX_SAMPLES (unsigned int) 20001

#define MAX_ADC_VALUE (unsigned int) 65535

#define ACH_RANGE_LL_SCAL_FACTOR (float) 32355.1748
#define ACH_RANGE_LH_SCAL_FACTOR (float) 6553.6
#define ACH_RANGE_HL_SCAL_FACTOR (float) 529.4921
#define ACH_RANGE_HH_SCAL_FACTOR (float) 107.24959  
#define CURRENT_CH_SCAL_FACTOR   (float) 917.49

//granica se racuna prema:
//GRANICA[V]=(18V/(R+380))*100                          
//GRANICA[ADC]=(GRANICA[V]*65535)/5
//Modification request #4179:  granice 4.19V i 0.7V
#define MAIN_THRESHOLD_LOW  (unsigned int)  9175 
#define MAIN_THRESHOLD_HIGH (unsigned int) 54918 

#define MEASURING_RES_50us   (unsigned char) '@' //Naknadno ubaceno!
#define MEASURING_RES_01ms   (unsigned char) 'A'
#define MEASURING_RES_1ms    (unsigned char) 'B'
#define MEASURING_RES_10ms   (unsigned char) 'C'

#define Freq50Hz             (unsigned char) '1'
#define Freq60Hz             (unsigned char) '2'

#define CLOSE_TEST            (unsigned char) 'A'   
#define OPEN_TEST             (unsigned char) 'B'   
#define TRIPFREE_DELAY_TEST   (unsigned char) 'C'  
#define TRIPFREE_FCT_TEST     (unsigned char) 'D'   //TRIP FREE, First Contact Touch
#define O_03s_C_TEST          (unsigned char) 'E'   
#define O_03s_CO_TEST         (unsigned char) 'F'    
#define CdelayO_TEST          (unsigned char) 'G'    
#define OdelayC_TEST          (unsigned char) 'H'    
#define OdelayCO_DELAY_TEST   (unsigned char) 'I'    
#define OdelayCO_FCT_TEST     (unsigned char) 'J'    //OdelayC, First Contact Touch
#define O_03s_CO_FCT_TEST     (unsigned char) 'K'    //O03sCO, First Contact Touch
#define FIRST_TRIP_TEST		  (unsigned char) 'L'	
#define DRM_TEST              (unsigned char) 'M'
#define NO_TEST               (unsigned char) 'N'
#define STATIC_RESISTANCE     (unsigned char) 'R'

#define TIME_BASE_ms         (unsigned char) '1'
#define TIME_BASE_s          (unsigned char) '2'
#define TIME_BASE_Cy         (unsigned char) '3' 

#define FREQ_50Hz            (unsigned char) '1'
#define FREQ_60Hz            (unsigned char) '2'

#define TRAVEL_UNIT_mm       (unsigned char) '1'
#define TRAVEL_UNIT_inch     (unsigned char) '2'
#define TRAVEL_UNIT_deg      (unsigned char) '3'

//OverCurrent
#define OverCurrentNO        (unsigned char) '0'
#define OverCurrentYES       (unsigned char) '1'

//Extrenal Trigger
#define EXT_TRIG_OFF         (unsigned char) '0'
#define EXT_TRIG_AUX1        (unsigned char) '1'
#define EXT_TRIG_AUX2        (unsigned char) '2'
#define EXT_TRIG_AUX3        (unsigned char) '3'
#define EXT_TRIG_AUX4        (unsigned char) '4'
#define EXT_TRIG_AUX5        (unsigned char) '5'
#define EXT_TRIG_AUX6        (unsigned char) '6'
#define EXT_TRIG_ANALOG1     (unsigned char) '7'
#define EXT_TRIG_ANALOG2     (unsigned char) '8'
#define EXT_TRIG_ANALOG3     (unsigned char) '9'
#define EXT_TRIG_ANALOG4     (unsigned char) 'A'
#define EXT_TRIG_ANALOG5     (unsigned char) 'B'
#define EXT_TRIG_ANALOG6     (unsigned char) 'C'
#define EXT_TRIG_CLOSING1    (unsigned char) 'D'
#define EXT_TRIG_CLOSING2    (unsigned char) 'E'
#define EXT_TRIG_CLOSING3    (unsigned char) 'F'
#define EXT_TRIG_BREAKING1   (unsigned char) 'G'
#define EXT_TRIG_BREAKING2   (unsigned char) 'H'
#define EXT_TRIG_BREAKING3   (unsigned char) 'I'
#define EXT_TRIG_CH          (unsigned char) 'J'

                           
#define NOEXTERNALTRIGG      (unsigned char) 'N'

#define PREINSION_ERROR      (unsigned int) 60000

#define ACH_RANGE_LL 		(unsigned char) '0'
#define ACH_RANGE_LH 		(unsigned char) '1'
#define ACH_RANGE_HL	  (unsigned char) '2'
#define ACH_RANGE_HH 		(unsigned char) '3'
      
#define AUX_DRY                (unsigned char) '0'
#define AUX_WET                (unsigned char) '1'

//#define CAT4XTR_DIGITAL        (unsigned char) 'A'
//#define CAT4XTR_LINEAR         (unsigned char) 'B'

#define TRANSDUCER_DIGITAL     (unsigned char) 'A'
#define TRANSDUCER_ANALOG      (unsigned char) 'B'

#define CAT6xAs3x_DA           (unsigned char) '1'
#define CAT6xAs3x_NE           (unsigned char) '2'

#define ExtTriggWT_10          (unsigned char) '0'
#define ExtTriggWT_20          (unsigned char) '1'
#define ExtTriggWT_INF         (unsigned char) '2'

#define COIL_PULSE_ALL_PHASES  (unsigned char) '0'
#define COIL_PULSE_PHASE1      (unsigned char) '1'
#define COIL_PULSE_PHASE2      (unsigned char) '2'
#define COIL_PULSE_PHASE3      (unsigned char) '3'

#define LANG_ENGLISH (unsigned char) '0'
#define LANG_OTHER   (unsigned char) '1'

#define NO   (unsigned char) '0'
#define YES  (unsigned char) '1'

#define ONE_BREAK_UNIT_PER_PHASE  (unsigned char) '1'
#define TWO_BREAK_UNITS_PER_PHASE  (unsigned char) '2'
#define FOUR_BREAK_UNITS_PER_PHASE (unsigned char) '4'

//uC PIN CONTROL:

#define PIN0 0b00000001
#define PIN1 0b00000010
#define PIN2 0b00000100
#define PIN3 0b00001000
#define PIN4 0b00010000
#define PIN5 0b00100000
#define PIN6 0b01000000
#define PIN7 0b10000000

#define GPIO_EXPANSION_U607 0x40
#define GPIO_EXPANSION_U608 0x42
#define GPIO_EXPANSION_U609 0x48
#define GPIO_EXPANSION_U610 0x44
#define GPIO_EXPANSION_U612 0x4E


//////////////////////////////////////////////////////////
//BSG CONTROL PINS
//////////////////////////////////////////////////////////
#define BSG_CONTROL_PINS GPIO_EXPANSION_U612

#define OFF 0
#define ON  1

#define SET_LED_CLOSE      I2C_CLOSE_LED_control(I2C_EXPANDER_LED_CTRL, ON); //PORTH.OUTSET=PIN0; //ovo je prebceno na I2C expander, treba definisati funkciju u bios.c
#define CLR_LED_CLOSE      I2C_CLOSE_LED_control(I2C_EXPANDER_LED_CTRL, OFF); //PORTH.OUTCLR=PIN0; //ovo je prebceno na I2C expander, treba definisati funkciju u bios.c

#define SET_LED_OPEN       I2C_OPEN_LED_control(I2C_EXPANDER_LED_CTRL, ON);//mora se definisati port I2C_CLOSE_LED_control(I2C_EXPANDER_LED_CTRL, ON);//PORTH.OUTSET=PIN1; //ovo je prebceno na I2C expander, treba definisati funkciju u bios.c
#define CLR_LED_OPEN       I2C_OPEN_LED_control(I2C_EXPANDER_LED_CTRL, OFF);//I2C_CLOSE_LED_control(I2C_EXPANDER_LED_CTRL, OFF);//PORTH.OUTCLR=PIN1; //ovo je prebceno na I2C expander, treba definisati funkciju u bios.c

#define BSG_RELAY12       (unsigned char) 4  //0b00000100
#define BSG_RELAY3        (unsigned char) 2  //0b00000010
#define BSG_3V3_BUCK      (unsigned char) 1  //0b00000001
#define BSG_DISCHARGE_REL (unsigned char) 8  //0b00001000
#define BSG_CHARGE_REL    (unsigned char) 16 //0b00010000
#define OSG_BSG_REL_CTRL  (unsigned char) 32 //0b00100000

#define I2C_EXPANDER_LED_CTRL (unsigned char) 128//0b10000000

#define MAIN_CONTACTS_INIT_SEQ MainContactsInitSequence();      
#define BSG_REL12_ON           BSGRelayControl(BSG_RELAY12, ON);
#define BSG_REL12_OFF          BSGRelayControl(BSG_RELAY12, OFF);
#define BSG_REL3_ON            BSGRelayControl(BSG_RELAY3, ON);
#define BSG_REL3_OFF           BSGRelayControl(BSG_RELAY3, OFF);
//IZBACEN TRANZISTOR, OBRNULA SE LOGIKA:  
#define BSG_BUCK_ON            BSGBuckControl(OFF);
#define BSG_BUCK_OFF           BSGBuckControl(ON);
//IZBACENI TRANZISTORI, OBRNULA SE LOGIKA:
#define BSG_DISCHARGE_REL_ON   BSGChargeDischargeRELControl(BSG_DISCHARGE_REL, OFF); //prazni kondenzator!
#define BSG_DISCHARGE_REL_OFF  BSGChargeDischargeRELControl(BSG_DISCHARGE_REL, ON);  //ne prazni kondenzator!
//OVDJE NE TREBA OBRTATI LOGIKU JER JE PO DEFAULTU ZATVOREN RELEJ:
#define BSG_CHARGE_REL_ON      BSGChargeDischargeRELControl(BSG_CHARGE_REL, ON);  
#define BSG_CHARGE_REL_OFF     BSGChargeDischargeRELControl(BSG_CHARGE_REL, OFF); 

#define BSG_OFF_OSG_ON         OSGConfigON();
#define OSG_OFF_BSG_ON         BSGConfigON();

#define BSG_SEQUENCE_ONE_BPPF  BSGSOneBPPF_ON_Sequence();
#define BSG_SEQUENCE_TWO_BPPF  BSGSTwoBPPF_ON_Sequence();
#define TURN_OFF_BSG           BSG_OFF_Sequence();    
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
//ANALOG CHANNELS CONTROL PINS
//////////////////////////////////////////////////////////
#define ANALOGCH1                 (unsigned char) '1'
#define ANALOGCH2                 (unsigned char) '2'
#define ANALOGCH3                 (unsigned char) '3'
#define ANALOGCH4                 (unsigned char) '4'
#define ANALOGCH5                 (unsigned char) '5'
#define ANALOGCH6                 (unsigned char) '6'
#define ANALOG_CH1_GAIN_CTRL      (unsigned char) 0x40 //0b01000000  //PIN6
#define ANALOG_CH1_RELAY_CTRL     (unsigned char) 0x01 //0b00000001  //PIN0
#define ANALOG_CH2_GAIN_CTRL      (unsigned char) 0x80 //0b10000000  //PIN7
#define ANALOG_CH2_RELAY_CTRL     (unsigned char) 0x02 //0b00000010  //PIN1
#define ANALOG_CH3_GAIN_CTRL      (unsigned char) 0x40 //0b01000000  //PIN6
#define ANALOG_CH3_RELAY_CTRL     (unsigned char) 0x04 //0b00000100  //PIN2
#define ANALOG_CH4_GAIN_CTRL      (unsigned char) 0x80 //0b10000000  //PIN7
#define ANALOG_CH4_RELAY_CTRL     (unsigned char) 0x08 //0b00001000  //PIN3
#define ANALOG_CH5_GAIN_CTRL      (unsigned char) 0x40 //0b01000000  //PIN6
#define ANALOG_CH5_RELAY_CTRL     (unsigned char) 0x10 //0b00010000  //PIN4
#define ANALOG_CH6_GAIN_CTRL      (unsigned char) 0x80 //0b10000000  //PIN7
#define ANALOG_CH6_RELAY_CTRL     (unsigned char) 0x20 //0b00100000  //PIN5
#define ANALOG_CH12_CONTROL       GPIO_EXPANSION_U607
#define ANALOG_CH34_CONTROL       GPIO_EXPANSION_U608
#define ANALOG_CH56_CONTROL       GPIO_EXPANSION_U609
#define ANALOG_CH1TO6_REL_CONTROL GPIO_EXPANSION_U609
#define ANALOG_CH_RANGE_LL        (unsigned char) '0'
#define ANALOG_CH_RANGE_LH        (unsigned char) '1'
#define ANALOG_CH_RANGE_HL        (unsigned char) '2'
#define ANALOG_CH_RANGE_HH        (unsigned char) '3'

//#define SET_ANALOG_CH1_RANGE_LL SetAnalogCHRange(ANALOGCH1, ANALOG_CH_RANGE_LL);
//#define SET_ANALOG_CH1_RANGE_LH SetAnalogCHRange(ANALOGCH1, ANALOG_CH_RANGE_LH);
//#define SET_ANALOG_CH1_RANGE_HL SetAnalogCHRange(ANALOGCH1, ANALOG_CH_RANGE_HL);
//#define SET_ANALOG_CH1_RANGE_HH SetAnalogCHRange(ANALOGCH1, ANALOG_CH_RANGE_HH);

//////////////////////////////////////////////////////////

#define bis(ADDRESS,BIT)  (ADDRESS & (1<<BIT)) // bit is set?
#define bic(ADDRESS,BIT) !(ADDRESS & (1<<BIT)) // bit is clear?

#define MAX_COMM_BUFF_LENGHT 50
      
#define MAIN_OK  (unsigned int)    0
#define MAIN_NOK (unsigned int)   30
#define ERROR_SRAM (unsigned int) 40
#define ERROR_SDC  (unsigned int) 50

#define NO_EXTERNAL_TRIGGER       60

/////////////////////////////////////////////
// Thermal printer definitions:
/////////////////////////////////////////////

#define SMALL  (unsigned char) 0x00
#define LOW    (unsigned char) 0x01
#define NARROW (unsigned char) 0x02
#define NORMAL (unsigned char) 0x03
#define WIDE   (unsigned char) 0x04
#define HIGH   (unsigned char) 0x05
#define LARGE  (unsigned char) 0x06

#define ITALIC_OFF     (unsigned char) 0x14
#define BOLD_OFF       (unsigned char) 0x12
#define UNDERLINE_OFF  (unsigned char) 0x10
#define REVERSE_OFF    (unsigned char) 0x0E

#define KEY_STOP   (unsigned char) 0xFF
#define PRINTER_OK (unsigned char) 0x81 //0xA1
#define NO_PAPER   (unsigned char) 0xA3
#define OPEN_HEAD  (unsigned char) 0xA9

/////////////////////////////////////////////
//STOP BUTTON FROM Control Board:
/////////////////////////////////////////////
#define STOP_BUTTON          (unsigned int) 100
#define CB_STOP_BUTT_PORT    PORTC.IN
#define CB_STOP_BUTT_PIN            4

/////////////////////////////////////////////
//AUX DRY Wet CONTROL:
/////////////////////////////////////////////
#define SET_AUX_CHANNELS AUX_dry_wet_control();

////////////////////////////////////////////
////////////////////////////////////////////
//// Multiple channel analog to digital ////
////////////////////// conveters  //////////
////////////////////////////////////////////

////////////////////////////////////////////
//Multiple channel ADC's control PINS:
////////////////////////////////////////////

//PH4 LINEAR TRANSDUCER ADC nCS
#define SET_LT_nADC_CS HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);//PORTH.OUTSET=PIN4;
#define CLR_LT_nADC_CS HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);//PORTH.OUTCLR=PIN4;

//PH5 LINEAR TRANSDUCER ADC RD/SCLK
#define SET_LT_ADC_RD_SCLK HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);//PORTH.OUTSET=PIN5;
#define CLR_LT_ADC_RD_SCLK HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);//PORTH.OUTCLR=PIN5;

//PH6 CURRENT MEASUREMENT ADC nCS
#define SET_CM_nADC_CS HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET); //PORTH.OUTSET=PIN6;
#define CLR_CM_nADC_CS HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);//PORTH.OUTCLR=PIN6;

//PH7 CURRENT MEASUREMENT ADC RD/SCLK
#define SET_CM_ADC_RD_SCLK HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);// PORTH.OUTSET=PIN7;
#define CLR_CM_ADC_RD_SCLK HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);//PORTH.OUTCLR=PIN7;

//PH3  MULTIPLE CHANNEL ADC CONVST_AB
#define SET_MC_ADC_CONVST_AB HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); //PORTH.OUTSET=PIN3;
#define CLR_MC_ADC_CONVST_AB HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET); //PORTH.OUTCLR=PIN3;

//PJ1  MULTIPLE CHANNEL ADC RESET
#define SET_MC_ADC_RESET HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET); //PORTJ.OUTSET=PIN1;
#define CLR_MC_ADC_RESET HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);//PORTJ.OUTCLR=PIN1;

//PJ2  MULTIPLE CHANNEL ADC nSTBY
#define SET_MC_ADC_nSTBY 0//PORTJ.OUTSET=PIN2;
#define CLR_MC_ADC_nSTBY 0//PORTJ.OUTCLR=PIN2;

////PH0 MULTIPLE CHANNEL ADC OS0
//#define SET_MC_ADC_OS0 PORTH.OUTSET=PIN0;
//#define CLR_MC_ADC_OS0 PORTH.OUTCLR=PIN0;

////PH1 MULTIPLE CHANNEL ADC OS1
//#define SET_MC_ADC_OS1 PORTH.OUTSET=PIN1;
//#define CLR_MC_ADC_OS1 PORTH.OUTCLR=PIN1;

////PH2 MULTIPLE CHANNEL ADC OS2
//#define SET_MC_ADC_OS2 PORTH.OUTSET=PIN2;
//#define CLR_MC_ADC_OS2 PORTH.OUTCLR=PIN2;

//MULTIPLE CHANNEL ADC RANGE CONTROL PIN IS CONNECTED TO U607/P0 (GPIO EXPANSION)!.

////////////////////////////////////////////
//Multiple channel ADC's input PORTS:
////////////////////////////////////////////

//PJ0 MC ADC BUSY
//#define MC_ADC_BUSY_PORT    PORTJ.IN 
//#define MC_ADC_BUSY_PIN            0

//PJ3 MC ADC FIRST DATA
//#define MC_ADC_FIRSTDATA_PORT    PORTJ.IN 
//#define MC_ADC_FIRSTDATA_PIN            3 

////////////////////////////////////////////
////////////////////////////////////////////
//// Single channel analog to digital  /////
////////////////////// converters //////////
////////////////////////////////////////////

#if 0
#define GPIO_PIN_0                 ((uint16_t)0x0001)  /* Pin 0 selected    */
#define GPIO_PIN_1                 ((uint16_t)0x0002)  /* Pin 1 selected    */
#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* Pin 2 selected    */
#define GPIO_PIN_3                 ((uint16_t)0x0008)  /* Pin 3 selected    */
#define GPIO_PIN_4                 ((uint16_t)0x0010)  /* Pin 4 selected    */
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* Pin 7 selected    */
#define GPIO_PIN_8                 ((uint16_t)0x0100)  /* Pin 8 selected    */
#define GPIO_PIN_9                 ((uint16_t)0x0200)  /* Pin 9 selected    */
#define GPIO_PIN_10                ((uint16_t)0x0400)  /* Pin 10 selected   */
#define GPIO_PIN_11                ((uint16_t)0x0800)  /* Pin 11 selected   */
#define GPIO_PIN_12                ((uint16_t)0x1000)  /* Pin 12 selected   */
#define GPIO_PIN_13                ((uint16_t)0x2000)  /* Pin 13 selected   */
#define GPIO_PIN_14                ((uint16_t)0x4000)  /* Pin 14 selected   */
#define GPIO_PIN_15                ((uint16_t)0x8000)  /* Pin 15 selected   */
#define GPIO_PIN_All               ((uint16_t)0xFFFF)  /* All pins selected */
#endif


#define GPIO_PIN_0_SET                 ((uint32_t)0x0001)  /* Pin 0 set    */
#define GPIO_PIN_1_SET                 ((uint32_t)0x0002)  /* Pin 1 set    */
#define GPIO_PIN_2_SET                 ((uint32_t)0x0004)  /* Pin 2 set    */
#define GPIO_PIN_3_SET                 ((uint32_t)0x0008)  /* Pin 3 set    */
#define GPIO_PIN_4_SET                 ((uint32_t)0x0010)  /* Pin 4 set    */
#define GPIO_PIN_5_SET                 ((uint32_t)0x0020)  /* Pin 5 set    */
#define GPIO_PIN_6_SET                 ((uint32_t)0x0040)  /* Pin 6 set    */
#define GPIO_PIN_7_SET                 ((uint32_t)0x0080)  /* Pin 7 set    */
#define GPIO_PIN_8_SET                 ((uint32_t)0x0100)  /* Pin 8 set    */
#define GPIO_PIN_9_SET                 ((uint32_t)0x0200)  /* Pin 9 set    */
#define GPIO_PIN_10_SET                ((uint32_t)0x0400)  /* Pin 10 set   */
#define GPIO_PIN_11_SET                ((uint32_t)0x0800)  /* Pin 11 set   */
#define GPIO_PIN_12_SET                ((uint32_t)0x1000)  /* Pin 12 set   */
#define GPIO_PIN_13_SET                ((uint32_t)0x2000)  /* Pin 13 set   */
#define GPIO_PIN_14_SET                ((uint32_t)0x4000)  /* Pin 14 set   */
#define GPIO_PIN_15_SET                ((uint32_t)0x8000)  /* Pin 15 set   */

#define GPIO_PIN_0_RESET               ((uint32_t)0x10000)  /* Pin 0 RESET    */ //=GPIO_PIN1_SET<<16
#define GPIO_PIN_1_RESET               ((uint32_t)0x20000)  /* Pin 1 RESET    */
#define GPIO_PIN_2_RESET               ((uint32_t)0x40000)  /* Pin 2 RESET    */
#define GPIO_PIN_3_RESET               ((uint32_t)0x80000)  /* Pin 3 RESET    */
#define GPIO_PIN_4_RESET               ((uint32_t)0x100000)  /* Pin 4 RESET    */
#define GPIO_PIN_5_RESET               ((uint32_t)0x200000)  /* Pin 5 RESET    */
#define GPIO_PIN_6_RESET               ((uint32_t)0x400000)  /* Pin 6 RESET    */
#define GPIO_PIN_7_RESET               ((uint32_t)0x800000)  /* Pin 7 RESET    */
#define GPIO_PIN_8_RESET               ((uint32_t)0x1000000)  /* Pin 8 RESET    */
#define GPIO_PIN_9_RESET               ((uint32_t)0x2000000)  /* Pin 9 RESET    */
#define GPIO_PIN_10_RESET              ((uint32_t)0x4000000)  /* Pin 10 RESET   */
#define GPIO_PIN_11_RESET              ((uint32_t)0x8000000)  /* Pin 11 RESET   */
#define GPIO_PIN_12_RESET              ((uint32_t)0x10000000)  /* Pin 12 RESET   */
#define GPIO_PIN_13_RESET              ((uint32_t)0x20000000)  /* Pin 13 RESET   */
#define GPIO_PIN_14_RESET              ((uint32_t)0x40000000)  /* Pin 14 RESET   */
#define GPIO_PIN_15_RESET              ((uint32_t)0x80000000)  /* Pin 15 RESET   */

//PA0 ADC GROUP MAIN CONTACTS, ENABLE
#define SET_MC_ADC_ENABLE GPIOA->BSRR=GPIO_PIN_0_SET; //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //PORTJ.OUTSET=PIN7;
#define CLR_MC_ADC_ENABLE GPIOA->BSRR=GPIO_PIN_0_RESET; //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); //PORTJ.OUTCLR=PIN7;

//PA0 ADC GROUP ANALOG CHANNELS, ENABLE
#define SET_AC_ADC_ENABLE GPIOA->BSRR=GPIO_PIN_0_SET; //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //PORTJ.OUTSET=PIN6;
#define CLR_AC_ADC_ENABLE GPIOA->BSRR=GPIO_PIN_0_RESET; //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); //PORTJ.OUTCLR=PIN6;

//PA2 SINGLE CHANNEL ADC #CS
#define SET_SC_ADC_nCS GPIOA->BSRR=GPIO_PIN_2_SET; //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET); //PORTJ.OUTSET=PIN5;
#define CLR_SC_ADC_nCS GPIOA->BSRR=GPIO_PIN_2_RESET; //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET); //PORTJ.OUTCLR=PIN5;

//PA3 SINGLE CHANNEL ADC CLCK
#define SET_SC_ADC_CLCK GPIOA->BSRR=GPIO_PIN_3_SET;//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);  //PORTJ.OUTSET=PIN4;
#define CLR_SC_ADC_CLCK GPIOA->BSRR=GPIO_PIN_3_RESET;//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET); //PORTJ.OUTCLR=PIN4;

////////////////////////////////////////////
////////////////////////////////////////////
//// Digital transducer control   //////////
//////////////////////  pins  //////////////
////////////////////////////////////////////

//PG15 DIGITAL TRANSDUCER#3 nOE SIGNAL
#define SET_DT3_nOE GPIOG->BSRR=GPIO_PIN_15_SET; //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_SET); //PORTK.OUTSET=PIN1;
#define CLR_DT3_nOE GPIOG->BSRR=GPIO_PIN_15_RESET;//HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_RESET); //PORTK.OUTCLR=PIN1;

//PG14 DIGITAL TRANSDUCER#2 nOE SIGNAL
#define SET_DT2_nOE GPIOG->BSRR=GPIO_PIN_14_SET; //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET); //PORTK.OUTSET=PIN2;
#define CLR_DT2_nOE GPIOG->BSRR=GPIO_PIN_14_RESET; //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET); //PORTK.OUTCLR=PIN2;

//PG13 DIGITAL TRANSDUCER#1 nOE SIGNAL
#define SET_DT1_nOE GPIOG->BSRR=GPIO_PIN_13_SET; //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET); //PORTK.OUTSET=PIN3;
#define CLR_DT1_nOE GPIOG->BSRR=GPIO_PIN_13_RESET; //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET); //PORTK.OUTCLR=PIN3;

//PG12 DIGITAL TRANSDUCER RESET SIGNAL
#define SET_DT_RES GPIOG->BSRR=GPIO_PIN_12_SET; //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_SET); //PORTK.OUTSET=PIN4;
#define CLR_DT_RES GPIOG->BSRR=GPIO_PIN_12_RESET; //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_RESET); //PORTK.OUTCLR=PIN4;

//PG11 DIGITAL TRANSDUCER SEL2 SIGNAL
#define SET_DT_SEL2 GPIOG->BSRR=GPIO_PIN_11_SET; //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_11, GPIO_PIN_SET);//PORTK.OUTSET=PIN5;
#define CLR_DT_SEL2 GPIOG->BSRR=GPIO_PIN_11_RESET; //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_11, GPIO_PIN_RESET);//PORTK.OUTCLR=PIN5;

//PG10 DIGITAL TRANSDUCER SEL1 SIGNAL
#define SET_DT_SEL1 GPIOG->BSRR=GPIO_PIN_10_SET; //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET); //PORTK.OUTSET=PIN6;
#define CLR_DT_SEL1 GPIOG->BSRR=GPIO_PIN_10_RESET; //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET); //PORTK.OUTCLR=PIN6;

//PE2 DIGITAL TRANSDUCER nOE SIGNAL
#define SET_DT_nOE GPIOE->BSRR=GPIO_PIN_2_SET; //HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET); //PORTF.OUTSET=PIN1;
#define CLR_DT_nOE GPIOE->BSRR=GPIO_PIN_2_RESET; //HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET); //PORTF.OUTCLR=PIN1;

////////////////////////////////////////////
////////////////////////////////////////////
//// Auxilliary Channels 1-6  //////////////
////////////////////// (AUX)  //////////////
////////////////////////////////////////////

//PE6 Auxilliary Channels nOE SIGNAL
#define SET_AUX_nOE GPIOE->BSRR=GPIO_PIN_6_SET; //HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET); //PORTF.OUTSET=PIN0;
#define CLR_AUX_nOE GPIOE->BSRR=GPIO_PIN_6_RESET; //HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET); //PORTF.OUTCLR=PIN0; 

////////////////////////////////////////////
////////////////////////////////////////////
//// Static random access memory   /////////
////////////////////// control//////////////
////////////////////////////////////////////

////PD0 nMEMORY CHIP ENABLE:
//#define SET_MEM_nCE PORTD.OUTSET=PIN0; 
//#define CLR_MEM_nCE PORTD.OUTCLR=PIN0; 

////PD1 nMEMORY WRITE ENABLE:
//#define SET_MEM_nWE PORTD.OUTSET=PIN1; 
//#define CLR_MEM_nWE PORTD.OUTCLR=PIN1; 

////PD2 nMEMORY READ (OUTPUT) ENABLE:
//#define SET_MEM_nOE PORTD.OUTSET=PIN2; 
//#define CLR_MEM_nOE PORTD.OUTCLR=PIN2; 

////PD3 nMEMORY A16:
//#define SET_MEM_A16 PORTD.OUTSET=PIN3; 
//#define CLR_MEM_A16 PORTD.OUTCLR=PIN3; 

////PD4 nMEMORY A17:
//#define SET_MEM_A17 PORTD.OUTSET=PIN4; 
//#define CLR_MEM_A17 PORTD.OUTCLR=PIN4; 

////PD5 nMEMORY A18:
//#define SET_MEM_A18 PORTD.OUTSET=PIN5; 
//#define CLR_MEM_A18 PORTD.OUTCLR=PIN5; 

////PD6 nMEMORY A19:
//#define SET_MEM_A19 PORTD.OUTSET=PIN6; 
//#define CLR_MEM_A19 PORTD.OUTCLR=PIN6; 

////PD7 nMEMORY A20:
//#define SET_MEM_A20 PORTD.OUTSET=PIN7; 
//#define CLR_MEM_A20 PORTD.OUTCLR=PIN7;

//#define SECTORS_IN_SRAM 15 

////MEMORY ADDRESS LATCHES:
////#MCU1_OE
////NOT CONNECTED BY MISTAKE, CONNECT IT TO GND
////NOTE: PQ2 & PQ3 ARE FREE, USE THEM IN THE NEXT DESIGN

////PC6 MEMORY LATCH, LE1:
//#define SET_LATCH_LE1 PORTC.OUTSET=PIN6; 
//#define CLR_LATCH_LE1 PORTC.OUTCLR=PIN6; 

////PC7 MEMORY LATCH, LE2:
//#define SET_LATCH_LE2 PORTC.OUTSET=PIN7; 
//#define CLR_LATCH_LE2 PORTC.OUTCLR=PIN7; 

////////////////////////////////////////////
////////////////////////////////////////////
//// Coil control //////////////////////////
////////////////////// (IGBT's) ////////////
////////////////////////////////////////////

//PF2 Breaking1 IGBT CONTROL								
#define SET_BREAKING1_IGBT HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_SET); //PORTF.OUTSET=PIN2;
#define CLR_BREAKING1_IGBT HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_RESET); //PORTF.OUTCLR=PIN2;

//PF3 Breaking2 IGBT CONTROL
#define SET_BREAKING2_IGBT HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET); //PORTF.OUTSET=PIN3;
#define CLR_BREAKING2_IGBT HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET); //PORTF.OUTCLR=PIN3;

//PF4 Breaking3 IGBT CONTROL
#define SET_BREAKING3_IGBT HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET); //PORTF.OUTSET=PIN4;
#define CLR_BREAKING3_IGBT HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET); //PORTF.OUTCLR=PIN4;

//PF5 Closing1 IGBT CONTROL
#define SET_CLOSING1_IGBT HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_SET); //PORTF.OUTSET=PIN5;
#define CLR_CLOSING1_IGBT HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_RESET); //PORTF.OUTCLR=PIN5;

//PF6 Closing2 IGBT CONTROL
#define SET_CLOSING2_IGBT HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_SET); //PORTF.OUTSET=PIN6;
#define CLR_CLOSING2_IGBT HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_RESET);//PORTF.OUTCLR=PIN6;

//PF7 Closing3 IGBT CONTROL
#define SET_CLOSING3_IGBT HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET); //PORTF.OUTSET=PIN7;
#define CLR_CLOSING3_IGBT HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET); //PORTF.OUTCLR=PIN7;

//U608 GPIO EXPANDER, PINS 1-5 ARE USED FOR CC RELAY CONTROL:
//Breaking1 Relay
#define BREAKING1_RELAY 0x04 //0b00000100
//Breaking2 Relay
#define BREAKING2_RELAY 0x02 //0b00000010
//Breaking3 Relay
#define BREAKING3_RELAY 0x01 //0b00000001
//Closing1 Relay
#define CLOSING1_RELAY  0x20 //0b00100000
//Closing2 Relay
#define CLOSING2_RELAY  0x10 //0b00010000
//Closing3 Relay
#define CLOSING3_RELAY  0x08 //0b00001000

//BREAKING 1 RELAY CONTROL
#define SET_BREAKING1_RELAY CC_RelayControl(BREAKING1_RELAY, ON);
#define CLR_BREAKING1_RELAY CC_RelayControl(BREAKING1_RELAY, OFF);

//BREAKING 2 RELAY CONTROL
#define SET_BREAKING2_RELAY CC_RelayControl(BREAKING2_RELAY, ON);
#define CLR_BREAKING2_RELAY CC_RelayControl(BREAKING2_RELAY, OFF);

//BREAKING 3 RELAY CONTROL
#define SET_BREAKING3_RELAY CC_RelayControl(BREAKING3_RELAY, ON);
#define CLR_BREAKING3_RELAY CC_RelayControl(BREAKING3_RELAY, OFF);

//CLOSING1 RELAY CONTROL
#define SET_CLOSING1_RELAY CC_RelayControl(CLOSING1_RELAY, ON);
#define CLR_CLOSING1_RELAY CC_RelayControl(CLOSING1_RELAY, OFF);

//CLOSING2 RELAY CONTROL
#define SET_CLOSING2_RELAY CC_RelayControl(CLOSING2_RELAY, ON);
#define CLR_CLOSING2_RELAY CC_RelayControl(CLOSING2_RELAY, OFF);

//CLOSING3 RELAY CONTROL
#define SET_CLOSING3_RELAY CC_RelayControl(CLOSING3_RELAY, ON);
#define CLR_CLOSING3_RELAY CC_RelayControl(CLOSING3_RELAY, OFF);

////////////////////////////////////////////
////////////////////////////////////////////
//// UNIVERSAL SERIAL BUS //////////////////
////////////////////// (FTDI) //////////////
////////////////////////////////////////////

//USB PORTS INIT:	
//DATA PORT (GPIOC0-C7) INPUT
//CONTROL PORT (G6, G7 - INPUT)
//CONTROL PORT (G8(=1), G9(=0) - OUTPUT)
	
#define GPIO_PIN_FIRST_BYTE              ((uint16_t)0x00FF)  /* First 8 pins selected */
	
#define USB_DATA_PORT GPIOC 
#define USB_CONTROL_PORT GPIOG

#define USB_BC0_PIN  GPIO_PIN_6
#define USB_BC1_PIN  GPIO_PIN_7

#define WAIT_FTDI_BCBUS0 {while(HAL_GPIO_ReadPin(USB_CONTROL_PORT, USB_BC0_PIN)==1){}}
#define WAIT_FTDI_BCBUS1 {while(HAL_GPIO_ReadPin(USB_CONTROL_PORT, USB_BC1_PIN)==1){}}

//PG8 FTDI SIGNAL BC2
#define SET_FTDI_BC2_nRD HAL_GPIO_WritePin(USB_CONTROL_PORT, GPIO_PIN_8, GPIO_PIN_SET);
#define CLR_FTDI_BC2_nRD HAL_GPIO_WritePin(USB_CONTROL_PORT, GPIO_PIN_8, GPIO_PIN_RESET);
	
//PG9 FTDI SIGNAL BC3
#define SET_FTDI_BC3_WR HAL_GPIO_WritePin(USB_CONTROL_PORT, GPIO_PIN_9, GPIO_PIN_SET);
#define CLR_FTDI_BC3_WR HAL_GPIO_WritePin(USB_CONTROL_PORT, GPIO_PIN_9, GPIO_PIN_RESET);
	
////////////////////////////////////////////
///////// USB FLASH DRIVE //////////////////
////////////////////////////////////////////

#define SET_USB_FLASH_nCS HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET)
#define CLR_USB_FLASH_nCS HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);

#define SET_USB_FLASH_RST HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET)
#define CLR_USB_FLASH_RST HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);

#define SET_USB_FLASH_MOSI HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET)
#define CLR_USB_FLASH_MOSI HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);

#define SET_USB_FLASH_SCK HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
#define CLR_USB_FLASH_SCK HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

#define USB_FLASH_READY_PORT GPIOA
#define USB_FLASH_READY_PIN  GPIO_PIN_4

#define USB_FLASH_MISO_PORT GPIOA
#define USB_FLASH_MISO_PIN  GPIO_PIN_6 


//USB FLASH DRIVE CS   = PD3  : OUTPUT
//USB FLASH DRIVE RST  = PD6  : OUTPUT
//USB FLASH DRIVE MOSI = PA7  : OUTPUT
//USB FLASH DRIVE SCK  = PA5  : OUTPUT
//USB FLASH DRIVE RDY  = PA4  : INPUT
//USB FLASH DRIVE MISO = PA6  : INPUT

//#define S_D8   {HAL_Delay(1);};


////////////////////////////////////////////
// 							DRM FUNCTIONS						  //
////////////////////////////////////////////

extern void DRM_DAC_Write(unsigned int data, unsigned int channel);
extern void DRM_Channel_Enable(unsigned int channel_number);
extern void DRM_Channel_Disable(unsigned int channel_number);
extern void MCU_Battery_Charger_Control(unsigned char port_control, unsigned int on_off);
extern void MCU_Coil_Control(unsigned char port_control, unsigned int on_off);
extern void DRM_Battery_Charger_Control(unsigned char port_control, unsigned int on_off);
extern void DRM_Pwr_Cfg_Ch1(unsigned int setOnOff);
extern void Coil_Control(unsigned char coil_select, unsigned int on_off);
////////////////////////////////////////////
// 							DRM MACROS							  //
////////////////////////////////////////////

#define DRM_SAMPLE_SIZE     (uint16_t)10000

#define CHANNEL1 (unsigned int)1
#define CHANNEL2 (unsigned int)2
#define CHANNEL3 (unsigned int)3

#define COIL_CLOSE (unsigned int)1
#define COIL_OPEN  (unsigned int)2
	
//SRAM
#define SRAM_BASE_ADDRESS   0x60000000

//DRM Control

#define CHANNEL1_ENABLE 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)
#define CHANNEL1_DISABLE 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)

#define CHANNEL2_ENABLE 		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET)
#define CHANNEL2_DISABLE 		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET)

#define CHANNEL3_ENABLE 		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET)
#define CHANNEL3_DISABLE 		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET)

#define DRM1_CLK_HIGH  			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET)
#define DRM1_CLK_LOW  			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET)

#define DRM1_DIN_HIGH				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET)
#define DRM1_DIN_LOW				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET)

#define DRM1_SYNC1_HIGH			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET)
#define DRM1_SYNC1_LOW			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET)

//definisati liniju za SYNC2, trenutno visi pin, nije povezan na procesor, iskoristiti SYNC1 ili SYNC3 pin konektora

#define DRM1_SYNC3_HIGH			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_SET)
#define DRM1_SYNC3_LOW			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_RESET)

#define DRM1_ADC_CLK_SET		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET)	
#define DRM1_ADC_CLK_RESET	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET)	

#define DRM1_ADC_CS_SET			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_SET)	
#define DRM1_ADC_CS_RESET		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_RESET)

#define DRM1_ADC_CURR1			HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6)
#define DRM1_ADC_VOUT1			HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_6)
#define DRM1_ADC_CURR2			HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_7)
#define DRM1_ADC_VOUT2			HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)
#define DRM1_ADC_CURR3			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)
#define DRM1_ADC_VOUT3			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)

//I2C Battery Control

#define GPIO_EXPANSION_U2			0x40
#define GPIO_EXPANSION_U3			0x48
#define GPIO_EXPANSION_U4			0x42
#define GPIO_EXPANSION_U5			0x44
#define GPIO_EXPANSION_U16		0x46

#define MCU_CHG_ON_OFF  			0x01
#define MCU_CHG_INFO					0x02
#define MCU_CHG_PWR_CTRL    	0x04
#define MCU_BAL_ON_OFF		  	0x08
#define COIL_IGBT1_CTRL		    0x10
#define COIL_IGBT2_CTRL   	  0x20

#define DRM1_ADPT_RLY_CH1			0x01
#define DRM1_BAL_EN_CH1				0x02
#define DRM1_PWR_EN_CH1				0x04
#define DRM1_CHG_DSB_CH1			0x08
#define DRM1_CHG_INFO_CH1			0x10

#define DRM1_ADPT_RLY_CH2			0x20
#define DRM1_BAL_EN_CH2				0x40
#define DRM1_PWR_EN_CH2				0x80
#define DRM1_CHG_DSB_CH2			0x01
#define DRM1_CHG_INFO_CH2			0x02

#define DRM1_ADPT_RLY_CH3			0x04
#define DRM1_BAL_EN_CH3				0x08
#define DRM1_PWR_EN_CH3				0x10
#define DRM1_CHG_DSB_CH3			0x20
#define DRM1_CHG_INFO_CH3			0x40

