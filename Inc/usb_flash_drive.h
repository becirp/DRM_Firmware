// USB Stick
#define NO_DISK         1
#define FILE_ERROR      2
#define FILE_EXIST      3
#define STICK_ERROR     22

extern unsigned int InitStick_Comm(unsigned char tt);
extern void reset_flash_drive_MCU(void);
