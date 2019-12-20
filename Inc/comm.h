extern char OutputBuffer[];
extern char InputBuffer[];

extern unsigned char GetDataFromMemory;
extern unsigned char GMD;

extern void putcharB(unsigned char);
extern unsigned char getcharB(void);
extern void SendOutputBuffer(unsigned char);
extern void my_putchar(unsigned char);
extern unsigned char GetInputBuffer(unsigned char);
extern void send_test_data_to_pc(void);
extern void wait_char_from_pc(unsigned char memory);
extern void send_data_transfer_over_status(void);
extern void send_measuring_samples_to_pc(void);
extern void send_measuring_samples_to_pc_DRM_test(unsigned char sensor, unsigned char measuring_channel);

extern unsigned char spie_master_tx_rx(unsigned char shOut);
extern unsigned char spie_master_tx_fast(unsigned char shOut, unsigned char delay);