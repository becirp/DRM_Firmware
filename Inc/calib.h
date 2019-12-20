//Commands:

extern unsigned int Read_Calib_Coef_from_EEPROM_Comm(void);
extern unsigned int Store_Calib_Coef_to_EEPROM_Comm(void);
extern unsigned int Write_Calib_Coef_Comm(unsigned char coef_type, unsigned char channel_type, unsigned char channel_number, unsigned int channel_range);
extern unsigned int Read_Calib_Coef_Comm(unsigned char coef_type, unsigned char channel_type, unsigned char channel_number, unsigned int channel_range);
extern unsigned int Reset_Calibration_COMM(void);
extern unsigned int Get_Active_Calibration_Coef_COMM(void);

//Other:
extern void apply_calibration_to_analog_chs(void);
extern float calculate_gained_value(unsigned int gain, float value, char signed_int_value);
extern unsigned int apply_offset_correction(unsigned int input_value, unsigned int offset, unsigned char offset_sign);
extern void calculate_offset_for_CC_channels(void);
