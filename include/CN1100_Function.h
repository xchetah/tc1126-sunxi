#ifndef CN1000_FUNCTION_H
#define  CN1000_FUNCTION_H

#include "CN1100_GlobalVar.h"


extern void BufferAHandle(void);
extern void BufferBHandle(void);
extern void FrameBuffer_Full_Handled(uint16_t *buffer);
extern void DataProc_CalculateDeltaData(uint16_t *buffer);

#ifdef SCREEN_FULL_ADAPTIVE
extern uint32_t DataProc_CircleScreenAdaptive(void);
extern void DataProc_CalRefHLSetAverageValue(uint16_t *buffer);
extern void DataProc_SetRefHLEnd(void);
#endif

extern void DataProc_FindSpecialValue(uint16_t *buffer, uint16_t *max, uint16_t *max1, uint16_t *min, uint16_t *min1);
#ifdef CHANNEL_ADAPTIVE
extern void DataProc_CalTxAverageValue(uint16_t *buffer);
extern void DataProc_CalRxAverageValue(uint16_t *buffer);
extern void DataProc_FindAbnormalPonit(uint16_t max,uint16_t min,uint16_t max1,uint16_t min1);
extern void DataProc_FindAbnormalChannel(void);
extern void DataProc_AdjustAbnormalChannel(void);
extern void DataProc_AdjustAbnormalPoint(void);
extern void TC1126_ChAdaptive_TransModeSetting(uint32_t TxorRxFlag);
#endif
extern void DataProc_PressKeyDetect(void);
extern void DataProc_HandleFingerInfo(void);
extern void DataProc_FixSettledPointExistLong(uint16_t *buffer);
extern void Baseline_BaseBufferHandled(uint16_t *buffer);

extern void TC1126_Init_GlobalVariables(void);
extern void TC1126_Init_HardwareRegs(void);
extern void TC1126_Init_AllRegisters(void);

extern void Stupid_Delay(uint32_t TimeCount);
extern void Tiny_Delay(uint32_t TimeCount);

void CN1100_FrameScanDoneInt_ISR(struct work_struct *work);

enum hrtimer_restart CN1100_SysTick_ISR(struct hrtimer *timer);
void chm_ts_reset_func(struct work_struct *work);
int  get_chip_addr(void);
s32  atoi(char *psz_buf);
void update_cfg(void);
int  chm_ts_write_config(cn1100_t config);
int  chm_ts_read_config(cn1100_t config);
int  chm_ts_read_proc(char *page, char **start, off_t off,int count, int *eof, void *data);
int  chm_ts_write_proc(struct file *file, const char __user *buffer,unsigned long count, void *data);
void cn1100_init(void);
void cn1100_reset(void);
void Report_Coordinate(void);
int  SPI_read_singleData(uint32_t addr);
int  SPI_read_DATAs(uint32_t addr, uint16_t num, uint16_t *data);
int  SPI_write_singleData(uint32_t addr, uint16_t data);
int  SPI_write_DATAs(uint32_t addr, uint16_t num, uint16_t *data);


#endif
