/*
 * Report_Coordinate

 * SPI_write_DATAs
 */
#include "include/CN1100_common.h"
#include "include/CN1100_linux.h"
#include "include/CN1100_Function.h"

int SCREEN_HIGH = 1280;
int SCREEN_WIDTH = 720;
int CN1100_RESET_PIN = -1;
int CN1100_INT_PIN = -1;


static int pwr_en;

extern struct ctp_config_info config_info;

static int screen_max_x = 0;
static int screen_max_y = 0;
static int revert_x_flag = 0;
static int revert_y_flag = 0;
static int twi_id = 0;
static int exchange_x_y_flag = 0;


#ifdef PRESS_KEY_DETECT
uint16_t key_pressed = 0;
struct keys{
	uint16_t key;
	uint16_t value;
};
static struct keys chm_ts_keys[]={
	{KEY_BACK,TOUCH_KEY_1},
	{KEY_HOMEPAGE,TOUCH_KEY_2},
	{KEY_MENU,TOUCH_KEY_3},
	{KEY_ENTER,TOUCH_KEY_4},
};
#define MAX_KEY_NUM ((sizeof(chm_ts_keys)/sizeof(chm_ts_keys[0])))

static int touch_key_pressed = 0;
#endif

#define CTP_IRQ_NUMBER                  (config_info.irq_gpio_number)

static const unsigned short normal_i2c[2] = {0x20,I2C_CLIENT_END};

#define REPORT_DATA_ANDROID_4_0
struct cn1100_spi_dev *spidev = NULL;
uint16_t chip_addr = 0x5d;

#ifdef CONFIG_HAS_EARLYSUSPEND
static void chm_ts_early_suspend(struct early_suspend *h);
static void chm_ts_late_resume(struct early_suspend *h);
#endif
static void out_gpio(unsigned handle,int on)
{      
	u32 gpio_status;        
	gpio_status = sw_gpio_getcfg(handle);
	if(gpio_status != 1){
		sw_gpio_setcfg(handle,1);
	}		
	if(on != 0)
	{
		__gpio_set_value(handle, 1);
	}
	else{
		__gpio_set_value(handle, 0);
	}
}

void update_cfg(void){
	int i = 0;
	cn1100_t config[] = { 
#include "chm_ts.cfg" 
	};  
	for(i = 0;i < ARRAY_SIZE(config);i++){
		chm_ts_write_config(config[i]);
		msleep(10);
	}   
	bdt.BFD.InitCount = 0;
}

int SPI_read_singleData(uint32_t addr)
{
	/*Test to see if sth wrong,change FAST_READ_DATA to READ_CHIP_ID*/
	int16_t ret = 0;
	uint8_t tmp[] = {
		((addr>>8)&0xff),addr&0xff,
	};
	struct i2c_msg *msgs;
	int status = 0;
	uint8_t rx[8] = {0};
	msgs = kmalloc(sizeof(struct i2c_msg)*I2C_MSGS,GFP_KERNEL);
	msgs[0].addr = chip_addr;
	msgs[0].flags = 0;
	msgs[0].buf = tmp;
	msgs[0].len = ARRAY_SIZE(tmp);

	msgs[1].addr = chip_addr;
	msgs[1].flags =  I2C_M_RD;
	msgs[1].buf = rx;
	msgs[1].len = 2; 

	if(!spidev->client){
		CN1100_print("cannot get i2c adapter\n");
		ret = -1;
		goto out;
	}
	ret = 0;
	while(ret < 3){
		status = i2c_transfer(spidev->client->adapter,msgs,2);
		if(status !=  2){
			ret++;
			CN1100_print("retry:%d\n",ret);
		}else{
			break;
		}
	}
	if(ret >= 3){
		CN1100_print("failed to send i2c message:%d\n",status);
		ret = status;
		goto out;
	}
	ret = rx[0] << 8;
	ret +=rx[1];
out:
	kfree(msgs);
	return ret;
}

int SPI_read_DATAs(uint32_t addr, uint16_t num, uint16_t *data)
{
	uint32_t i = 0,j = 0;

	uint8_t rx[512] = {0};
	uint8_t tmp[] = {
		(addr>>8)&0xff,(addr)&0xff,
	};
	struct i2c_msg *msgs;
	int status = 0,ret = 0;

	msgs = kmalloc(sizeof(struct i2c_msg)*I2C_MSGS,GFP_KERNEL);

	msgs[0].addr = chip_addr;
	msgs[0].flags = 0;
	msgs[0].buf = tmp;
	msgs[0].len = ARRAY_SIZE(tmp);

	msgs[1].addr = chip_addr;
	msgs[1].flags =  I2C_M_RD;
	msgs[1].buf = rx;
	msgs[1].len = 2*num;

	if(!spidev->client->adapter){
		CN1100_print("cannot get i2c adapter\n");
		status = -1;
		goto out;
	}
	ret = 0;
	while(ret < 3){
		status = i2c_transfer(spidev->client->adapter,msgs,2);
		if(status !=  2){
			ret++;
			CN1100_print("retry:%d\n",ret);
		}else{
			break;
		}
	}
	if(ret >= 3){
		CN1100_print("failed to send i2c message:%d\n",status);
		ret = status;
		goto out;
	}
	j = 0;
	for(i=0;i<num;i++){
		data[i] = (uint16_t)rx[j++]<<8;
		data[i] += (uint16_t)rx[j++];
	}
out:
	kfree(msgs);
	return status;
}

int SPI_write_singleData(uint32_t addr, uint16_t data)
{
	uint8_t tmp[] = {
		((addr>>8)&0xff),addr&0xff,((data>>8)&0xff),data&0xff,
	};
	struct i2c_msg msg;
	int status = 0,ret = 0;
	msg.addr = chip_addr;
	msg.flags = 0;
	msg.buf = tmp;
	msg.len = ARRAY_SIZE(tmp);

	if(!spidev->client->adapter){
		CN1100_print("cannot get i2c adapter\n");
		status = -1;
		goto out;
	}
	ret = 0;
	while(ret < 3){
		status = i2c_transfer(spidev->client->adapter,&msg,1);
		if(status !=  1){
			ret++;
			CN1100_print("retry:%d\n",ret);
		}else{
			break;
		}
	}
	if(ret >= 3){
		CN1100_print("failed to send i2c message:%d\n",status);
	}
out:
	return status;
}

int SPI_write_DATAs(uint32_t addr, uint16_t num, uint16_t *data)
{
	uint32_t i =0 ,count = 0,j = 0;
	int status = 0,ret = 0;
	uint8_t tmp[] = {
		((addr>>8)&0xff),(addr&0xff),
	};
	struct i2c_msg msg;
	uint8_t tx[512] = {0};
	for(i=0;i<ARRAY_SIZE(tmp);i++){
		tx[i] = tmp[i];
	}

	for(j=0;j<num;j++){
		tx[i++] = ((data[j]>>8)&0xff);
		tx[i++] = (data[j])&0xff;
	}
	count = i;
	msg.addr = chip_addr;
	msg.flags = 0;
	msg.buf = tx;
	msg.len = count;

	if(!spidev->client->adapter){
		CN1100_print("cannot get i2c adapter\n");
		status = -1;
		goto out;
	}
	ret = 0;
	while(ret < 3){
		status = i2c_transfer(spidev->client->adapter,&msg,1);
		if(status !=  1){
			ret++;
			CN1100_print("retry:%d\n",ret);
		}else{
			break;
		}
	}
	if(ret >= 3){
		CN1100_print("failed to send i2c message:%d\n",status);
	}
out:
	return status;
}

#ifdef PRESS_KEY_DETECT
void set_finger_num(int num)
{
	if(bdt.FingerReqNum != num){
		bdt.FingerReqNum = num;
	}else{
		bdt.FingerReqNum = 1;
	}
	printk("bdt.FingerReqNum:%d\n",bdt.FingerReqNum);
}

#define KEY1_THRESH 400
#define KEY2_THRESH 400	
#define KEY3_THRESH 400
#define KEY4_THRESH 400
void DataProc_PressKeyDetect()
{
	bdt.PressKeyFlag1          = NO_KEY_PRESSED;
	//   printk("KEY1:(%-5d,%-5d),KEY2:(%-5d,%-5d),KEY3:(%d),KEY4:(%-5d,%-5d)\n",bdt.DeltaDat_kp[2],bdt.DeltaDat_kp[3],bdt.DeltaDat_kp[7],bdt.DeltaDat_kp[8],bdt.DeltaDat_kp[5],bdt.DeltaDat_kp[12],bdt.DeltaDat_kp[13]);
	if((bdt.DeltaDat_kp[2] > KEY1_THRESH)|| (bdt.DeltaDat_kp[3] > KEY1_THRESH))
	{
		bdt.PressKeyFlag1 = TOUCH_KEY_1;
	}

	if((bdt.DeltaDat_kp[7] > KEY2_THRESH)|| (bdt.DeltaDat_kp[8] > KEY2_THRESH))
	{
		bdt.PressKeyFlag1 = TOUCH_KEY_2;
	}

	if((bdt.DeltaDat_kp[5] > KEY3_THRESH))
	{
		bdt.PressKeyFlag1 = TOUCH_KEY_3;
	}

	if((bdt.DeltaDat_kp[12] > KEY4_THRESH)|| (bdt.DeltaDat_kp[13] > KEY4_THRESH))
	{
		bdt.PressKeyFlag1 = TOUCH_KEY_4;
	}
}

unsigned int mouse_key=~0;
unsigned int mouse_x=0;
unsigned int mouse_y=0;
unsigned int mouse_up=0;

unsigned int key_value;
void report_key(void)
{
	int i = 0;
	for(i = 0;i < MAX_KEY_NUM;i++){
		if(chm_ts_keys[i].value == bdt.PressKeyFlag1){

			key_pressed = chm_ts_keys[i].key;
			key_value=chm_ts_keys[i].value;
			if(key_value==TOUCH_KEY_2)
			{  
				mouse_key=~mouse_key;	
				printk("mouse_key=%2d\n",mouse_key);
			}
			else
			{
				if(mouse_key)
				{
					if(key_value==TOUCH_KEY_4)
						input_report_key(spidev->mouse_dev, BTN_LEFT,  1);
					else if(key_value==TOUCH_KEY_1)
						input_report_key(spidev->mouse_dev, BTN_RIGHT,  1);
					printk("key mouse =%2d\n",key_value);
					input_sync((spidev->mouse_dev));
				}
				else
				{
					input_report_key(spidev->dev,chm_ts_keys[i].key,1);
					input_sync((spidev->dev));
				}
			}

			touch_key_pressed = 1;
			printk("KEY_PRESSED:%d\n",chm_ts_keys[i].key);
			break;
		}
	}
}
#endif


unsigned long  down_ms;
unsigned long up_ms;
unsigned int key_press=0;
void Report_Coordinate_Wait4_SingleTime(int id,int X, int Y)
{
	int x1,y1;
	unsigned long t1;
	Y  = (uint16_t)(( ((uint32_t)Y) * RECV_SCALE )>>16);
	X  = (uint16_t)(( ((uint32_t)X) * XMTR_SCALE )>>16);

	if((X>=5)&&(X<=9)&&(Y>=72)&&(Y<=77))
		return ;


        #ifdef PRESS_KEY_DETECT
	if(mouse_key)
	{

		if((X>0)||(Y>0))
		{
			X=SCREEN_HIGH-X;
			Y=SCREEN_WIDTH-Y;

			if(mouse_up)
			{
				mouse_x=X;
				mouse_y=Y;
				down_ms=jiffies;
				mouse_up=0;
				return ;
			}

			x1=X-mouse_x;
			y1=mouse_y-Y;

			input_report_rel(spidev->mouse_dev, REL_X,     x1);
			input_report_rel(spidev->mouse_dev, REL_Y,     y1);
			input_sync(spidev->mouse_dev);  
			printk("mouse X = %d, Y = %d,rel x=%2d, rel y=%2d, up=%2d\n",X,Y, x1,y1,mouse_up);
			mouse_x=X;
			mouse_y=Y;
			mouse_up=0;
			down_ms=jiffies;
			t1=jiffies_to_msecs(jiffies -up_ms);
			printk("T1=%2lu\n",t1);
			if(t1<500)
			{
				//	key_press++;
			}


		}
		else
		{
			up_ms = jiffies;
			t1=jiffies_to_msecs(jiffies -down_ms);
			//printk("T1111=%2d\n",t1);
			if(t1>200)
				mouse_up=1;

			if(key_press)
			{
				//	input_report_key(spidev->mouse_dev, BTN_LEFT,  1);
				//	input_sync(spidev->mouse_dev);  
			}
		}

	}
	else
        #endif
	{
		if(X > 0 || Y > 0){ 

			printk("X = %d, Y = %d\n", X, Y);
#ifdef REPORT_DATA_ANDROID_4_0
			input_mt_slot(spidev->dev,id);
			input_mt_report_slot_state(spidev->dev,MT_TOOL_FINGER,true);
#endif
			input_report_abs(spidev->dev, ABS_MT_TRACKING_ID,id);
			input_report_abs(spidev->dev, ABS_MT_TOUCH_MAJOR, 5); 
#if 0
			input_report_abs(spidev->dev, ABS_MT_POSITION_X, X);
#else
			input_report_abs(spidev->dev, ABS_MT_POSITION_X, SCREEN_HIGH-X);
#endif
#if 0
			input_report_abs(spidev->dev, ABS_MT_POSITION_Y, SCREEN_WIDTH-Y); 
#else
			input_report_abs(spidev->dev, ABS_MT_POSITION_Y, Y); 
#endif
			input_report_abs(spidev->dev, ABS_MT_WIDTH_MAJOR, 5); 
#ifndef REPORT_DATA_ANDROID_4_0
			input_mt_sync(spidev->dev);
#endif
		}else{
#ifdef REPORT_DATA_ANDROID_4_0
			input_mt_slot(spidev->dev,id);
			input_mt_report_slot_state(spidev->dev,MT_TOOL_FINGER,false);
#else
			input_mt_sync(spidev->dev);
#endif

		}   
		input_sync(spidev->dev);
	}
}
uint16_t FingProc_Dist2PMeasure(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void Report_Coordinate()
{
	int fnum = FINGER_NUM_MAX;
	int X=0, Y=0, i, count;
	int Wait4Flag = 0;

	for(i=0; i<fnum; i++) {
		if(bdt.DPD[i].JustPassStateFlag4) Wait4Flag = 1;
	}
#ifdef PRESS_KEY_DETECT
	if(bdt.PressKeyFlag1){
		report_key();    
		bdt.PressKeyFlag1 = 0;
		return;
	}
	if(touch_key_pressed&&(!bdt.PressKeyFlag1)){


		if(key_value!=TOUCH_KEY_2)	
		{
			if(mouse_key)
			{

				if(key_value==TOUCH_KEY_4)
					input_report_key(spidev->mouse_dev, BTN_LEFT,  0);
				else if(key_value==TOUCH_KEY_1)
					input_report_key(spidev->mouse_dev, BTN_RIGHT,  0);
				input_sync((spidev->mouse_dev));
			}
			else
			{
				if(key_pressed == KEY_MENU){
					set_finger_num(5);
				}
				input_report_key(spidev->dev,key_pressed,0);
				input_sync((spidev->dev));	  
			}
		}


		key_pressed = 0;
		key_value=0;
		touch_key_pressed = 0;
	}

#endif

	if(Wait4Flag)
	{
		int LongEnoughFlag = 0;
		for(i=0; i<fnum; i++)
		{
			if(bdt.DPD[i].JustPassStateFlag4)
			{
				if(FingProc_Dist2PMeasure(bdt.DPD[i].Prev_Finger_X[3], bdt.DPD[i].Prev_Finger_Y[3], bdt.DPD[i].Prev_Finger_X[0], bdt.DPD[i].Prev_Finger_Y[0]) > 100)
					LongEnoughFlag = 3;  // 4 points should be reported

			}
		}

		//*************************************
		// Report Old 4 Point with others
		//*************************************
		for(count=LongEnoughFlag; count>=0; count--)
		{
			for(i=0; i<fnum; i++)
			{

				if(bdt.DPD[i].JustPassStateFlag4)
				{
					Y  = bdt.DPD[i].Prev_Finger_Y[count];
					X  = bdt.DPD[i].Prev_Finger_X[count];
				}
				else
				{
					Y  = bdt.DPD[i].Finger_Y_Reported; // Y -> RECV (480)
					X  = bdt.DPD[i].Finger_X_Reported; // X -> XTMR (800) 
				}
				Report_Coordinate_Wait4_SingleTime(i,X, Y);
			}
			//  input_sync(spidev->dev);
			mdelay(1);
		}
	}

	//*************************************
	// Report Old 4 Point with others
	//*************************************
	for(i=0; i<fnum; i++) 
	{    
		Y  = bdt.DPD[i].Finger_Y_Reported; // Y -> RECV (480)
		X  = bdt.DPD[i].Finger_X_Reported; // X -> XTMR (800) 
		Report_Coordinate_Wait4_SingleTime(i,X, Y);

	}


	// input_sync(spidev->dev);
}


//static irqreturn_t cn1100_irq_handler(int irq, void *dev_id)
static irqreturn_t cn1100_irq_handler(void *dev_id)
{
	if(spidev->i2c_ok){
		sw_gpio_eint_set_enable(CTP_IRQ_NUMBER,0);
#ifdef CAL_TIME_CONSUMED
		if(0 == spidev->irq_interval){
			do_gettimeofday(&spidev->pretime);
			spidev->irq_interval = 1;
		}else{
			do_gettimeofday(&spidev->curtime);
			spidev->irq_interval = (spidev->curtime.tv_sec-spidev->pretime.tv_sec)*1000000+(spidev->curtime.tv_usec-spidev->pretime.tv_usec);
			spidev->pretime = spidev->curtime;
		}   
#endif
		queue_work(spidev->workqueue,&spidev->main);
		//	printk( "cn1100_irq_handler=======\n");
	}   
	return 0;

	// return IRQ_RETVAL(IRQ_HANDLED);
}
#ifdef DEBUG_PROC_USED
static int chm_proc_open(struct inode *inode, struct file *file)
{
	return 0;
}

int chm_proc_release(struct inode *inode, struct file *file)
{
	return 0;
}

static const struct file_operations chm_proc_fops = {
	.owner      = THIS_MODULE,
	.open       = chm_proc_open,
	.read       = chm_proc_read,
	.write      = chm_proc_write,
	.release    = chm_proc_release,
};
#endif


static int chm_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int status = 0;
	int value = 0;
	/****************csvc add txh start*********************/
	script_item_u   val;
	script_item_value_type_e   type;
	printk("==========chm_ts_probe 1=============\n");

	type = script_get_item("ctp_para", "ctp_pwr_en", &val);
	printk("type: %d\n", type);
	if (SCIRPT_ITEM_VALUE_TYPE_PIO != type) {
		printk("failed to fetch ctp_pwr_en\n");
		return -1;
	}
	pwr_en = val.gpio.gpio;

	type = script_get_item("ctp_para", "ctp_reset", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_PIO != type) {
		printk("script_get_item ctp_reset type err\n");
		return -1;
	}
	CN1100_RESET_PIN = val.gpio.gpio;

	out_gpio(pwr_en,1);

	out_gpio(CN1100_RESET_PIN,0);
	mdelay(1000);
	out_gpio(CN1100_RESET_PIN,1);
	/****************csvc add txh end*********************/
	spidev->mode = CN1100_USE_IRQ;
	spidev->client = client;
	if (!i2c_check_functionality(spidev->client->adapter, I2C_FUNC_I2C)) {
		dev_err(&spidev->client->dev, "I2C functionality not supported\n");
		return -ENODEV;
	}    
	chip_addr = 0x20;
	value = SPI_read_singleData(0x20);
	printk("=============0x%x===========\n",value);

	/*
	 *Get reset pin,and initialize the as an output
	 *with value 0 to disable chip function
	 *Maybe RESET PIN will not needed anymore
	 */

	spidev->workqueue = create_singlethread_workqueue("spi_read_cn1100");
	if(spidev -> workqueue == NULL){
		printk("Unable to create workqueue\n");
		goto fail2;
	}

	INIT_WORK(&spidev->main,CN1100_FrameScanDoneInt_ISR);
	INIT_WORK(&spidev->reset_work,chm_ts_reset_func);

	/*Allocate an input device to report coordinate to android*/
	spidev->dev = input_allocate_device();
	if(!spidev->dev){
		printk("cannot get input device\n");
		goto fail3;
	}
	spidev->mouse_dev = input_allocate_device();
	if(!spidev->mouse_dev){
		printk("cannot get input device\n");
		goto fail3;
	}



#ifdef REPORT_DATA_ANDROID_4_0
	__set_bit(EV_ABS,spidev->dev->evbit);
	__set_bit(EV_KEY,spidev->dev->evbit);
	__set_bit(EV_REP,spidev->dev->evbit);
	__set_bit(INPUT_PROP_DIRECT,spidev->dev->propbit);

	///



	input_mt_init_slots(spidev->dev,11);
#else
	input_set_abs_params(spidev->dev,ABS_MT_TRACKING_ID,0,10,0,0);
	set_bit(EV_ABS, spidev->dev->evbit);                  
	set_bit(EV_KEY, spidev->dev->evbit);       
#endif

	set_bit(ABS_MT_TOUCH_MAJOR, spidev->dev->absbit);
	set_bit(ABS_MT_POSITION_X, spidev->dev->absbit);
	set_bit(ABS_MT_POSITION_Y, spidev->dev->absbit);              
	set_bit(ABS_MT_WIDTH_MAJOR, spidev->dev->absbit);                     
	set_bit(ABS_MT_TRACKING_ID,spidev->dev->absbit);

	////mouse
	spidev->mouse_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REL);
	__set_bit(BTN_LEFT, spidev->mouse_dev->keybit);
	__set_bit(BTN_RIGHT, spidev->mouse_dev->keybit);

	__set_bit(EV_REL, spidev->mouse_dev->evbit);
	__set_bit(REL_X,	spidev->mouse_dev->relbit);
	__set_bit(REL_Y, spidev->mouse_dev->relbit);

#ifdef PRESS_KEY_DETECT
	for (status = 0; status < MAX_KEY_NUM; status++)
	{    
		input_set_capability(spidev->dev,EV_KEY,chm_ts_keys[status].key);  
	}    
#endif

	input_set_abs_params(spidev->dev, ABS_MT_POSITION_X, 0, screen_max_x, 0, 0);  
	input_set_abs_params(spidev->dev, ABS_MT_POSITION_Y, 0, screen_max_y, 0, 0);  
	input_set_abs_params(spidev->dev, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
	input_set_abs_params(spidev->dev, ABS_MT_WIDTH_MAJOR, 0, 200, 0, 0); 


	spidev->dev->name = "chm_ts";
	spidev->dev->phys = "input/ts";
	spidev->dev->id.bustype = BUS_I2C;
	spidev->dev->id.vendor = 0xDEAD;
	spidev->dev->id.product = 0xBEEF;
	spidev->dev->id.version = 0x0101;

	status = input_register_device(spidev->dev);
	if(status){
		printk("Cannot register input device\n");
		goto fail4;
	}	

	spidev->mouse_dev->name="tp_mouse";
	status = input_register_device(spidev->mouse_dev);
	if(status){
		printk("Cannot register input device\n");
		goto fail4;
	}	


	hrtimer_init(&spidev->systic,CLOCK_MONOTONIC,HRTIMER_MODE_REL);
	spidev->systic.function = CN1100_SysTick_ISR;


#ifdef CONFIG_HAS_EARLYSUSPEND
	spidev->early_suspend.level =  EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	spidev->early_suspend.suspend = chm_ts_early_suspend;
	spidev->early_suspend.resume = chm_ts_late_resume;
	register_early_suspend(&spidev->early_suspend);
#endif


	get_chip_addr();
	if(spidev->i2c_ok){
		cn1100_init();
		update_cfg();
	}
	printk( "tc1126_probe: request \n");
	/*Related to specified hardware*/
	if(spidev->mode & CN1100_USE_IRQ){
		spidev->irq = config_info.irq_gpio_number;
		// spidev->irq = gpio_to_irq(config_info.irq_gpio_number);
		// status = sw_gpio_irq_request(config_info.irq_gpio_number,TRIG_LEVL_HIGH,(peint_handle)cn1100_irq_handler,spidev);

		status = sw_gpio_irq_request(config_info.irq_gpio_number,TRIG_LEVL_HIGH,(peint_handle)cn1100_irq_handler,NULL); 
		if (!status) {
			printk( "tc1126_probe: request irq failed\n");
			goto fail5;
		}

	}
	printk( "tc1126_probe: request2 \n");


#ifdef DEBUG_PROC_USED
	spidev->chm_ts_proc = proc_create("chm_ts",0666,NULL,&chm_proc_fops);
	if(!spidev->chm_ts_proc){
		printk("failed to create proc directory\n");
	}
#endif

	hrtimer_start(&spidev->systic, ktime_set(0, SCAN_SYSTIC_INTERVAL), HRTIMER_MODE_REL);
	return 0;
fail5:
	unregister_early_suspend(&spidev->early_suspend);
	input_unregister_device(spidev->dev);
	input_unregister_device(spidev->mouse_dev);
fail4:
	input_free_device(spidev->dev);	
	input_free_device(spidev->mouse_dev);	
fail3:
	destroy_workqueue(spidev->workqueue);
fail2:
	kfree(spidev);
	return status;
}



static int chm_ts_remove(struct i2c_client *client)
{
	int status = 0;
	printk("driver is removing\n");
	remove_proc_entry("chm_ts", NULL);
#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&spidev->early_suspend);
#endif
	if(spidev->mode & (CN1100_USE_IRQ)){
		sw_gpio_eint_set_enable(CTP_IRQ_NUMBER,0);
	}
	hrtimer_cancel(&spidev->systic);
	flush_workqueue(spidev->workqueue);

	kfree(bd);
	spidev->client = NULL;
	input_unregister_device(spidev->dev);
	input_unregister_device(spidev->mouse_dev);
	destroy_workqueue(spidev->workqueue);
	if(spidev->mode & CN1100_USE_IRQ){
		free_irq(spidev->irq,NULL);
	}
	if(spidev){
		kfree(spidev);
	}
	return status;
}

int chm_ts_suspend(struct i2c_client *client,pm_message_t mesg)
{
	int ret = 0;
	spidev->mode |= CN1100_IS_SUSPENDED;
	hrtimer_cancel(&spidev->systic);
	flush_workqueue(spidev->workqueue);
	printk("%s:1\n",__func__);
	sw_gpio_eint_set_enable(CTP_IRQ_NUMBER,0);
	ctp_wakeup(0,0);
	printk("suspend done\n");
	return ret;
}

int chm_ts_resume(struct i2c_client *client)
{
	int ret = 0;
	printk("%s\n",__func__);
	spidev->mode &= ~(CN1100_IS_SUSPENDED);
	ctp_wakeup(1,0);

	printk("System Return From Deep Sleep\n");
	get_chip_addr();
	msleep(10);
	if(spidev->i2c_ok){
		cn1100_init();
		update_cfg();
	}
	msleep(10);
	if(spidev->mode & CN1100_USE_IRQ){
		sw_gpio_eint_set_enable(CTP_IRQ_NUMBER,0);
	}
	hrtimer_start(&spidev->systic, ktime_set(0, SCAN_SYSTIC_INTERVAL), HRTIMER_MODE_REL);

	return ret;
}

#ifdef CONFIG_HAS_EARLYSUSPEND
static void chm_ts_early_suspend(struct early_suspend *h){
	chm_ts_suspend(spidev->client,PMSG_SUSPEND);
}

static void chm_ts_late_resume(struct early_suspend *h){
	chm_ts_resume(spidev->client);
}
#endif

static const struct i2c_device_id chm_ts_id[] = {
	{"tc1126",0},
	{"cn1100",0},
	{},
};

MODULE_DEVICE_TABLE(i2c, chm_ts_id);
static int ctp_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;
	int ret; 

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -ENODEV;

	if(twi_id == adapter->nr){
		pr_info("%s: addr= %x\n",__func__,client->addr);
		ret = 1;
		if(!ret){
			pr_info("%s:I2C connection might be something wrong \n",__func__);
			return -ENODEV;
		}else{           
			pr_info("I2C connection sucess!\n");
			strlcpy(info->type, "tc1126", I2C_NAME_SIZE);
			return 0;            
		}    

	}else{
		return -ENODEV;
	}    
}


static struct i2c_driver chm_ts_driver = {
	.class          = I2C_CLASS_HWMON,
	.probe		= chm_ts_probe,
	.remove		= __devexit_p(chm_ts_remove),
	.id_table	= chm_ts_id,
#ifndef CONFIG_HAS_EARLYSUSPEND
	.suspend	= chm_ts_suspend,
	.resume		= chm_ts_resume,
#endif
	.address_list	= normal_i2c,
	.detect   = ctp_detect,
	.driver		= {
		.name	= "tc1126",
		.owner	= THIS_MODULE, 
	},
};

static int ctp_get_system_config(void)
{
	ctp_print_info(config_info,DEBUG_INIT);

	twi_id = config_info.twi_id;
	screen_max_x = config_info.screen_max_x;
	screen_max_y = config_info.screen_max_y;
	revert_x_flag = config_info.revert_x_flag;
	revert_y_flag = config_info.revert_y_flag;
	exchange_x_y_flag = config_info.exchange_x_y_flag;
	if((twi_id == 0) || (screen_max_x == 0) || (screen_max_y == 0)){
		printk("%s:read config error!\n",__func__);
		return 0;
	}
	return 1;
}


static int __init cn1100_spi_init(void)
{
	int status = 0;

	bd = kmalloc(sizeof(bd_t),GFP_KERNEL);
	if(!bd){
		goto fail1;
	}

	spidev = kmalloc(sizeof(*spidev),GFP_KERNEL);
	if(!spidev){
		goto fail2;
	}

	spidev->mode &= ~(CN1100_DATA_PREPARED);

	init_waitqueue_head(&spidev->waitq);

	if (!ctp_get_system_config()) {
		printk("%s:read config fail!\n",__func__);
		return status;
	}

	//	TODO:need confirm
	ctp_wakeup(1,0);

	msleep(20);
#ifdef BUILD_DATE
	printk("Build Time:%u\n",BUILD_DATE);
#endif
	status =  i2c_add_driver(&chm_ts_driver);
	if(status < 0){
		printk("Unable to add an i2c driver\n");
		goto fail3;
	}
	return 0;
fail3:
	kfree(spidev);
fail2:
	kfree(bd);
fail1:
	return status;
}

late_initcall(cn1100_spi_init);

static void __exit cn1100_spi_exit(void)
{
	i2c_del_driver(&chm_ts_driver);
	ctp_free_platform_resource();
}
module_exit(cn1100_spi_exit);

MODULE_DESCRIPTION("cn1100 spi transfer driver");
MODULE_LICENSE("GPL");
