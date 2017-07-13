#include "include/CN1100_common.h"
#include "include/CN1100_linux.h"
#include "include/CN1100_Function.h"


static bool cn1100_need_write = false;
static bool cn1100_need_read = false;
static int  read_value = 0;


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void chm_ts_reset_func(struct work_struct *work)
{
    cn1100_reset();
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
int get_chip_addr(void)
{
    int retry = 0;  
    int ret = 0;
    chip_addr = 0x5d;
    while(retry < 1)
    { 
        ret = SPI_read_singleData(0x400);
        if(ret < 0)
        { 
            retry++;
            msleep(10);
        }
        else
        {
            break;
        }   
    }   
    if(retry >= 1)
    { 
        chip_addr = 0x20;
        retry = 0;
        while(retry < 1)
        { 
            ret = SPI_read_singleData(0x400);
            if(ret < 0)
            { 
                retry++;
                msleep(10);
            }
            else
            {
                retry = 0;
                break;
            }   
            
        }   
        if(retry >= 1)
        { 
            spidev->i2c_ok = false;
            return -1;
        }   
            
    }   
    spidev->i2c_ok = true;
    CN1100_print("get chip addr:0x%x\n",chip_addr);
    return 0;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
s32 atoi(char *psz_buf)
{
    char *pch = psz_buf;
    s32 base = 0;  
    
    while (isspace(*pch))
    pch++;
    
    if (('-' == *pch) || ('+' == *pch)) 
    {
        base = 10; 
        pch++;
    } 
    else if ('h' == (*pch && tolower(pch[strlen(pch) - 1]))) 
    {
        base = 16; 
    }    
    
    return simple_strtoul(pch, NULL, base);
}

#ifdef DEBUG_PROC_USED //Use PROC interface to debug chip

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
int chm_ts_write_config(cn1100_t config)
{
    unsigned int *pp = &(bdt.PCBA.ProtectTime);
    switch(config.type)
    {
        case 0:
        {
            pp[config.index] = config.value;
            if(config.index >= 6 && config.index <= 11){
                //TC1126_Init_VarRegSetting();              // unused
                ;
            }else if(config.index == 12){
            CN1100_print("write config:%d,%d\n",config.index,config.value);
            if(1 == config.value)
            {    
            SPI_write_singleData(TPL1_REG, 0xffff); /*  cfg_reg30, 12'h000 */
            }    
            else if(0 == config.value)
            {    
            SPI_write_singleData(TPL1_REG, 0x0000); /*  cfg_reg30, 12'h000 */
            }    
            }
            break;
        }
    
        case 1:
        {
            SPI_write_singleData(config.index,config.value);
            break;
        }
    }
    return 0;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
int chm_ts_read_config(cn1100_t config)
{
    unsigned int *pp = &(bdt.PCBA.ProtectTime);
    switch(config.type)
    {
        case 0:
        {
        read_value = pp[config.index];
        break;
        }
        case 1:
        {
        read_value = SPI_read_singleData(config.index);
        break;
        } 
    }
    spidev->mode |= SHELL_READ_MODE;
    return 0;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
static char config_type = 0;
ssize_t chm_proc_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
    int len = 0,i = 0,j = 0;
    uint32_t X = 0,Y = 0;
    uint32_t tmp[FINGER_NUM_MAX*2] = {0};
    #ifdef CAL_TIME_CONSUMED
    uint32_t time_used[10] = {0};
    #endif
    data_t cn1100;
    switch((spidev->mode &(0xffff)))
    {
        #ifdef CAL_TIME_CONSUMED
        case TIME_READ_MODE:
        {
            time_used[0] = spidev->irq_interval;
            time_used[1] = spidev->total_time;
            time_used[2] = spidev->i2c_time;
            len = sizeof(time_used);
            if(copy_to_user(buf,time_used,len)){
                CN1100_print("copy failed\n");
            };
            break;
        }
        #endif
        case CFG_READ_MODE:
        {
            len = sizeof(read_value);
            if(copy_to_user(buf,&read_value,len)){
                CN1100_print("copy failed\n");
            };
            break;
        }
        case SHELL_READ_MODE:
        {
            if(0 == config_type)
            {
                len = sprintf(buf,"%d\n",read_value);
            }else if(1 == config_type)
            {
                len = sprintf(buf,"0x%x\n",read_value);
            }
            break;
        }
        case DIFF_READ_MODE:
        {
            len = sizeof(bdt.DeltaDat16A);
            wait_event_interruptible(spidev->waitq,(spidev->mode & CN1100_DATA_PREPARED));
            #ifdef KEY_PRESS_DETECT
            for(j=SRECV_NUM; j<RECV_NUM; j++) 
                for(i=0;i<XMTR_NUM;i++)
                {    
				  bdt.DeltaDat16A[i][j] = bdt.DeltaDat_kp[i];
                }  	
            #endif
            if(copy_to_user(buf,&bdt.DeltaDat16A[0][0],len)){
                CN1100_print("copy failed\n");
            };
            spidev->mode &= ~(CN1100_DATA_PREPARED);
            break;
        }
        case RAW_READ_MODE:
        {
            len = sizeof(bdt.DeltaDat16A);
            wait_event_interruptible(spidev->waitq,(spidev->mode & CN1100_DATA_PREPARED));
            switch(bd->BufferID)
            {
                case 0:
                {
                    if(copy_to_user(buf,&bdt.FrameDatLoadA[0][0],len)){
                        CN1100_print("copy failed\n");
                    };
                    break;
                }
                case 1:
                {
                    if(copy_to_user(buf,&bdt.FrameDatLoadB[0][0],len)){
                        CN1100_print("copy failed\n");
                    };
                    break;
                }
            }
            spidev->mode &= ~(CN1100_DATA_PREPARED);
            break;
        }
        case BASE_READ_MODE:
        {
            len = sizeof(bdt.BFD.BaseDat);
            wait_event_interruptible(spidev->waitq,(spidev->mode & CN1100_DATA_PREPARED));
            if(copy_to_user(buf,&bdt.BFD.BaseDat[0][0],len)){
                CN1100_print("copy failed\n");
            }
            spidev->mode &= ~(CN1100_DATA_PREPARED);
            break;
        }
        case DEBUG_READ_MODE:
        {
            len = sizeof(bdt.SigDeltaDat);
            wait_event_interruptible(spidev->waitq,(spidev->mode & CN1100_DATA_PREPARED));
            if(copy_to_user(buf,&bdt.SigDeltaDat[0][0],len)){
                CN1100_print("copy failed\n");
            }
            spidev->mode &= ~(CN1100_DATA_PREPARED);
            break;
        }
        case ALL_READ_MODE:
        {
            len = sizeof(data_t);
            wait_event_interruptible(spidev->waitq,(spidev->mode & CN1100_DATA_PREPARED));
            memcpy(&cn1100.diff[0][0],&bdt.DeltaDat16A[0][0],sizeof(bd->bdt->DeltaDat16A));
            memcpy(&cn1100.base[0][0],&bdt.BFD.BaseDat[0][0],sizeof(bd->bdt->BFD.BaseDat));
            if(0 == bd->BufferID)
            {
                memcpy(&cn1100.raw[0][0],&bdt.FrameDatLoadA[0][0],sizeof(bd->bdt->FrameDatLoadA));
            }else if(1 == bd->BufferID)
            {
                memcpy(&cn1100.raw[0][0],&bdt.FrameDatLoadB[0][0],sizeof(bd->bdt->FrameDatLoadB));
            }
        
            j = 0;
            for (i = 0;i < FINGER_NUM_MAX;i++ )
            {
                Y  = bdt.DPD[i].Finger_Y_Reported; /*  Y -> RECV (480) */
                X  = bdt.DPD[i].Finger_X_Reported; /*  X -> XTMR (800)  */
                #if 0
                Y  = (uint32_t)(( ((uint32_t)Y) * RECV_SCALE )>>16);
                X  = (uint32_t)(( ((uint32_t)X) * XMTR_SCALE )>>16);
                if(X > 0 || Y > 0){
                X = SCREEN_HIGH-X;
                Y = SCREEN_WIDTH-Y;
                }
                #endif
                cn1100.base[XMTR_NUM-1][j++] = X;
                cn1100.base[XMTR_NUM-1][j++] = Y;
            }
            
            if(copy_to_user(buf,&cn1100,len)){
                CN1100_print("copy failed\n");
            }
            spidev->mode &= ~(CN1100_DATA_PREPARED);
            break;
        }
        case COOR_READ_MODE:
        {
            len = sizeof(tmp);
            wait_event_interruptible(spidev->waitq,(spidev->mode & CN1100_DATA_PREPARED));
            for (i = 0;i < FINGER_NUM_MAX;i++ )
            {
                Y  = bdt.DPD[i].Finger_Y_Reported; /*  Y -> RECV (480) */
                X  = bdt.DPD[i].Finger_X_Reported; /*  X -> XTMR (800)  */
                Y  = (uint32_t)(( ((uint32_t)Y) * RECV_SCALE )>>16);
                X  = (uint32_t)(( ((uint32_t)X) * XMTR_SCALE )>>16);
                if(X > 0 || Y > 0)
                {
                    X = SCREEN_HIGH-X;
                    Y = SCREEN_WIDTH-Y;
                }
                tmp[j++] = X;
                tmp[j++] = Y;
            } 
            if(copy_to_user(buf,tmp,len)){
                CN1100_print("copy failed\n");
            }
            spidev->mode &= ~(CN1100_DATA_PREPARED);
            break;
        }
        case MANUAL_READ_MODE:
        {
            break;
        }
        case INFO_READ_MODE:
        {
            len = sizeof(tmp);
            #ifdef TC1126 
            tmp[0] = 1;
            #else
            #endif
            tmp[1] = XMTR_NUM;tmp[2] = RECV_NUM;
            tmp[3] = 1024;
            if(copy_to_user(buf,tmp,len)){
                CN1100_print("copy failed\n");
            }
            break;
        }
        default:
        len = 0;
        break;
    }
    
    return len;
}




/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
int chm_proc_write(struct file *file, const char __user *buffer, size_t size, loff_t *ppos)
{
    int buff[16];
    char *p = NULL;
    char *rp = NULL;
    cn1100_t config;
    unsigned int *pp = &(bdt.PCBA.ProtectTime);
    bool err_cmd = true;
    
    memset(buff,0,sizeof(buff));
    if(copy_from_user(buff,buffer,size) != 0)
    {
        CN1100_print("failed to copy content from user space\n");
    }
    if(buff[0] >= PROC_CMD_MIN && buff[0] <= PROC_CMD_MAX)
    {
        spidev->mode &= (~0xffff);
        config_type = buff[1];
        switch(buff[0])
        {
            case PROC_WRITE_CFG:
            {
                config.type = config_type;
                config.index = buff[2];
                config.value = buff[3];
                chm_ts_write_config(config);
                /* *(pp+buff[2]) = buff[3]; */
                bdt.BFD.InitCount = 0;
                break;
            }
            case PROC_READ_CFG:
            {
                if(0 == config_type)
                {
                    read_value = *(pp+buff[2]);
                }else if(1 == config_type)
                {
                    read_value = SPI_read_singleData(buff[2]);
                }
                spidev->mode |= CFG_READ_MODE;
                break;
            }
            case PROC_READ_COORDINATE:
            {
                spidev->mode |= COOR_READ_MODE;
                break;
            }
            case PROC_READ_ALL:
            {
                spidev->mode |= ALL_READ_MODE;
                break;
            }
            case PROC_READ_DIFF:
            {
                spidev->mode |= DIFF_READ_MODE;
                break;
            }
            case PROC_READ_RAW:
            {
                spidev->mode |= RAW_READ_MODE;
                break;
            }
            case PROC_READ_BASE:
            {
                spidev->mode |= BASE_READ_MODE;
                break;
            }
            case PROC_READ_MANUAL:
            {
                spidev->mode |= MANUAL_READ_MODE;
                break;
            }
            case PROC_READ_DEBUG:
            {
                spidev->mode |= DEBUG_READ_MODE;
                break;
            }
            case PROC_READ_INFO:
            {
                CN1100_print("read driver infomation\n");
                spidev->mode |= INFO_READ_MODE;
                break;
            }
            case PROC_READ_TIME:
            {
                spidev->mode |= TIME_READ_MODE;
                break;
            }
        }
        err_cmd = false;
        
        if(err_cmd)
        {
           CN1100_print("erro command\n");
        }
        return size;
    }
    
    if(!strncmp("cn1100_write",(char*)buff,12))
    {
        if(cn1100_need_read)
        {
            cn1100_need_read = false;
        }
        CN1100_print("start to write cn1100\n");
        cn1100_need_write = true;
        err_cmd = false;

        if(err_cmd)
        {
           CN1100_print("erro command\n");
        }
        return size;
    }
    
    if(!strncmp("write_complete",(char*)buff,14))
    {
        CN1100_print("write cn1100 complete\n");
        cn1100_need_write = false;
        err_cmd = false;

        if(err_cmd)
        {
           CN1100_print("erro command\n");
        }
        return size;
    }
    
    if(!strncmp("cn1100_read",(char*)buff,11))
    {
        if(cn1100_need_write)
        {
            cn1100_need_write = false;
        }
        CN1100_print("start to read cn1100\n");
        cn1100_need_read = true;
        err_cmd = false;

        if(err_cmd)
        {
           CN1100_print("erro command\n");
        }
        return size;
    }
    
    if(!strncmp("read_complete",(char*)buff,13))
    {
        CN1100_print("read cn1100 complete\n");
        cn1100_need_read = false;
        err_cmd = false;

        if(err_cmd)
        {
           CN1100_print("erro command\n");
        }
    return size;
    }
    
    if((cn1100_need_read) || (cn1100_need_write))
    {
        p = (char *)buff;	
        rp = strsep(&p,",");
        if(!rp)
        {
            if(err_cmd)
            {
               CN1100_print("erro command\n");
            }
            return size;
        }
        config.type = atoi(rp);
        config_type = config.type;
        rp = strsep(&p,",");
        if(!rp)
        {
            if(err_cmd)
            {
               CN1100_print("erro command\n");
            }
            return size;
        }
        config.index = atoi(rp);
        if(1 == config.type)
        {
            if(1 == pp[13])
            {
                CN1100_print("in frequency scan mode,cannot config registers\n");
                if(err_cmd)
                {
                    CN1100_print("erro command\n");
                }
                return size;
            }
        }
        if(cn1100_need_write)
        {
            rp = strsep(&p,",");
            if(!rp)
            {
                if(err_cmd)
                {
                    CN1100_print("erro command\n");
                }
                return size;
            }
            config.value = atoi(rp);
            chm_ts_write_config(config);
            bdt.BFD.InitCount = 0;
        }else if(cn1100_need_read)
        {
            config.value = 0;
            chm_ts_read_config(config);
        }
        err_cmd = false;
    }
    if(err_cmd)
    {
        CN1100_print("erro command\n");
    }
    return size;
}
#endif



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void cn1100_init(void)
{
    TC1126_Init_GlobalVariables();
    TC1126_Init_HardwareRegs();
    spidev->ticks = 0;
    spidev->irq_count = 5;
    #ifdef CAL_TIME_CONSUMED
    spidev->irq_interval = 0;
    #endif
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void cn1100_reset(void)
{
    #if 1
    hrtimer_cancel(&spidev->systic);
    if(spidev->mode & CN1100_USE_IRQ)
    {
        disable_irq_nosync(spidev->irq);
    }
    cancel_work_sync(&spidev->main);
    
    if(CN1100_RESET_PIN != -1)
    {
        gpio_set_value(CN1100_RESET_PIN,0);
        msleep(50);
        gpio_set_value(CN1100_RESET_PIN,1);
        msleep(10);
    }
    
    enable_irq(spidev->irq);
    get_chip_addr();
    if(spidev->i2c_ok)
    {
        cn1100_init();
    }
    hrtimer_start(&spidev->systic, ktime_set(0, SCAN_SYSTIC_INTERVAL), HRTIMER_MODE_REL);
    #endif
}

void cn1100_set_irq(bool enable){

	if(enable){
  #ifdef CONFIG_ARCH_SUN8IW1P1
  	   sw_gpio_eint_set_enable(spidev->irq,1);
  #else
  	   enable_irq(spidev->irq);
  #endif
  	}else{
  #ifdef CONFIG_ARCH_SUN8IW1P1
  	   sw_gpio_eint_set_enable(spidev->irq,0);
  #else
  	   disable_irq_nosync(spidev->irq);
  #endif

	}

}
