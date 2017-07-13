#ifndef __CN1100_H__ //CN1100_LINUX_H
#define __CN1100_H__

#include <linux/miscdevice.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/input/mt.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/ctype.h>
#include <linux/err.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/spi/spi.h>
#include <linux/earlysuspend.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <mach/gpio.h>

//#ifdef CN1100_WINNER
#if ( defined (CONFIG_ARCH_SUN8IW3P1) || defined (CONFIG_ARCH_SUN8IW5P1) )// A23 and A33
	#include <linux/init-input.h>
	#include <asm/gpio.h>
#else
	#if defined (CONFIG_ARCH_SUN8IW1P1) //A31
		#include <linux/ctp.h>
		#include <mach/sys_config.h>
		#include <linux/regulator/consumer.h>
	#else
		#error "Only A23 A33 A31 SoCs is Supported by this Driver !!"
	#endif
#endif

#include <asm/uaccess.h>



#define DEBUG_PROC_USED

#define PROC_CMD_MIN 0
#define PROC_WRITE_CFG 0
#define PROC_READ_CFG 1
#define PROC_READ_ALL 2
#define PROC_READ_RAW 3
#define PROC_READ_DIFF 4
#define PROC_READ_BASE 5
#define PROC_READ_MANUAL 7
#define PROC_READ_DEBUG 8
#define PROC_READ_INFO 9
#define PROC_READ_TIME 10
#define PROC_READ_COORDINATE 6
#define PROC_CMD_MAX 20

//define all mode cn1100 need
#define RAW_READ_MODE		(1 << 0)
#define DIFF_READ_MODE		(1 << 1)
#define BASE_READ_MODE		(1 << 2)
#define ALL_READ_MODE		(1 << 3)
#define CFG_READ_MODE		(1 << 4) 
#define SHELL_READ_MODE		(1 << 5)
#define COOR_READ_MODE		(1 << 6)
#define MANUAL_READ_MODE	(1 << 7)
#define DEBUG_READ_MODE		(1 << 8)
#define INFO_READ_MODE		(1 << 9)
#define TIME_READ_MODE		(1 << 10)

#define CN1100_USE_IRQ		(1 << 16)
#define CN1100_DATA_PREPARED	(1 << 17)
#define CN1100_IRQ_DEAD		(1 << 18)
#define CN1100_CHIP_DEAD	(1 << 19)
#define CN1100_IS_SUSPENDED	(1 << 20)
#define CN1100_IS_DOZE		(1 << 21)
//define mode end
//

#define CN1100_IRQ_ENABLE(status) do{\
					if(status){\
						enable_irq(spidev->irq);\
					}else{\
						disable_irq_nosync(spidev->irq);\
					}\
				    }while(0);

#define FRAME_HANDLE_SPEED_NORMAL 12500000
#define SCAN_SYSTIC_INTERVAL 1000000
#define CN1100_RESET_ENABLE

#define CN1100_HANDLE_INTERVAL 25000000

#define I2C_MSGS 2

#define CAL_TIME_CONSUMED 

#ifdef CN1100_LX //CN1100_LX

#define TPD_PROXIMITY

#ifdef TPD_PROXIMITY
#include <linux/wakelock.h>
#include <linux/miscdevice.h>
#include <linux/poll.h>
#include "lightprox.h"
extern u8 tpd_proximity_flag;  
extern u8 tpd_proximity_detect;  
extern int last_tpd_proximity_state;  
struct ws_data {
    struct i2c_client *client;
    struct ws_cfg *ws_cfg_data;
    struct work_struct work;
    struct wake_lock ws_wake_lock;
    struct semaphore update_lock;
    struct miscdevice prox_dev;
    wait_queue_head_t light_event_wait;
    wait_queue_head_t proximity_event_wait;
    #ifdef CONFIG_HAS_EARLYSUSPEND
    struct early_suspend early_suspend;
    #endif
    int prox_state;
    int prox_event;
    int prox_poll_delay;
    int irq; 
};

extern struct ws_data *ws_datap;
#endif

#endif //CN1100_LX

typedef struct {
	uint16_t base[XMTR_NUM][RECV_NUM];
	uint16_t raw[XMTR_NUM][RECV_NUM];
	int16_t diff[XMTR_NUM][RECV_NUM];
}data_t;

typedef struct {
	char		type;
	int16_t		index;
	int		value;
} cn1100_t;

struct cn1100_spi_dev {
	bool i2c_ok;
	int irq;
	uint32_t ticks;
	uint32_t mode;
	uint32_t irq_count;
#ifdef CAL_TIME_CONSUMED
	struct timeval pretime;
	struct timeval curtime;
	int irq_interval;
	int total_time;
	int i2c_time;
#endif


	wait_queue_head_t waitq;
	struct input_dev *dev;
	struct input_dev *mouse_dev;
	struct proc_dir_entry *chm_ts_proc;
	struct i2c_client *client;
	struct hrtimer systic;
	struct early_suspend early_suspend;

	struct workqueue_struct *workqueue;
	struct work_struct main;
	struct work_struct reset_work;
};


extern struct cn1100_spi_dev *spidev;
extern uint16_t chip_addr;


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
uint16_t DataProc_FDGetValidSampNumber(uint32_t *buf);
void DataProc_FaceDetectProcess(void);
ssize_t chm_proc_read(struct file *file, char __user *buf, size_t size, loff_t *ppos);
int chm_proc_write(struct file *file, const char __user *buffer, size_t size, loff_t *ppos);
void DataProc_DeltaMatrix2Array(uint32_t *BitDat, int16_t thr);
void DataProc_PullBitmapApart(uint32_t *buf);
void cn1100_set_irq(bool enable);

#endif//CN1100_LINUX_H
