/******************************************************************************
 * 版权所有(C) TRUECOREING
 * DEPARTMENT:
 * MANUAL_PERCENT:
 * 文件名称: CN1000_data.c 
 * 文件标识:    
 * 内容摘要: 
 * 其它说明:
 * 当前版本: 
 * 作    者: 
 * 完成日期:
 * 当前责任人: 
 *
 * 修改记录1: 代码合规
 * 修改日期: 2014-09-15
 * 版 本 号:
 * 修 改 人: Wangpc
 * 修改内容: 
 *
 * 修改记录2: Add one feature that acquire 10 fingers then parse 5 fingers 
 * 修改日期: 2014-11-12
 * 版 本 号:
 * 修 改 人: Wangpc(R01)
 * 修改内容: 
 *
 * 修改记录3: Rebuild structure of register
 * 修改日期: 2014-11-19
 * 版 本 号:
 * 修 改 人: Wangpc(R02)
 * 修改内容: 
 *****************************************************************************/


#ifndef CN1100_DATA_C
#define CN1100_DATA_C

#include "include/CN1100_common.h"

//#ifdef CN1100_LINUX
#include "include/CN1100_linux.h"
//#endif
#include "include/CN1100_Function.h"

inline int16_t abs16(int16_t x) { return (x < 0) ? -x : x; }
inline uint32_t multiply16by16(uint16_t x, uint16_t y) {  return (uint32_t) x * y; }
RegTab RegTab_t;
BasData_t  bdt;
bd_t *bd;

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
/*this function is used for reducing the time of  divide*/
int16_t divideINT16byUINT8(int16_t num, uint8_t den)
{
    uint8_t para_div[13] ={85,64,51,42,36,32,28,25,23,21,19,18,17};
    
    if(0 == den)
    {
        return MAX_INT16;
    }
    else if(den >= 1 && den <= 2)
    {
    
        return num>>(den-1);
    }
    else if(den >= 3 && den <= 15)
    {
    
        return (num * para_div[den - 3])>>8;
    }
    else
    {
        return num/den;
    }
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint16_t msbPos(uint32_t val) 
{
    uint16_t pos = 0;
    
    if ((val & 0xFFFF0000) != 0)
    {
        pos += 16;
        val >>= 16;
    }
    if ((val & 0xFFFFFF00) != 0) 
    {
        pos += 8;
        val >>= 8;
    }
    if ((val & 0xFFFFFFF0) != 0) 
    {
        pos += 4;
        val >>= 4;
    }
    if ((val & 0xFFFFFFFC) != 0)
    {
        pos += 2;
        val >>= 2;
    }
    if ((val & 0xFFFFFFFE) != 0)
    {
        pos += 1;
    }

    return (pos);
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint16_t lsbPos(uint32_t val) 
{

    uint16_t pos = 0;
    
    if ((val & 0x0000FFFF) == 0)
    {
        pos += 16;
        val >>= 16;
    }
    if ((val & 0x000000FF) == 0) 
    {
        pos += 8;
        val >>= 8;
    }
    if ((val & 0x0000000F) == 0)
    {
        pos += 4;
        val >>= 4;
    }
    if ((val & 0x00000003) == 0)
    {
        pos += 2;
        val >>= 2;
    }
    if ((val & 0x00000001) == 0)
    {
        pos += 1;
    }
    
    return (pos);
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void clearArray(uint16_t* p, uint16_t size)
{
    uint16_t i;
    for (i = 0;i< size;i++)
    {
        *p = 0;
        p++;
    }
}


/*******************************************************************************
* Function Name  : Baseline_FingerupdateTX
* Description    : When finger is touching the cover od SD card, NOISE is huge
*                  in some special TX or RX lines
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void Baseline_FingerupdateTX(void)
{
    uint16_t Max_num,i,j,average;
    int16_t Max_sum;
    uint16_t Max_cnt, Max_maxcnt, IvdCnt,IvdCnt1;
    IvdCnt = 0;
    IvdCnt1 = 0;
    for(j=0; j<RECV_NUM; j++)
    {
        Max_sum = 0;
        average = 0;      // averagesum
        Max_num = 0;
        Max_cnt    = 0;
        Max_maxcnt = 0;
        for(i=0; i<XMTR_NUM; i++)
        {
           // if(bdt.DeltaDat16A[i][j] < Min_sum) Min_sum = bdt.DeltaDat16A[i][j];
            if(bdt.DeltaDat16A[i][j] > BASE_UPDATETXRXTHR)
            {
                Max_sum += bdt.DeltaDat16A[i][j];
                Max_num++;
                Max_cnt++;
            }
            else
            {
               if(Max_cnt > Max_maxcnt)
               {
                   Max_maxcnt = Max_cnt;
                   average    = Max_sum/Max_cnt;
               }
               Max_cnt = 0;
               Max_sum = 0;
            }
        }
        if(average > 60)
        { 
            if(Max_maxcnt == 15)
            {
                bdt.PowerOnWithFinger  = 1;
                return ;
            }
           else if(Max_maxcnt >= 7)
            {   
                IvdCnt++;
                if(IvdCnt>=2)
                {
                    bdt.PowerOnWithFinger  = 1;
                    return ;
                }
            }
            else if(Max_maxcnt >=5 ) IvdCnt1++;

            if(IvdCnt1 >=4 )
            {
                bdt.PowerOnWithFinger = 1;
                return;
            }
        }
    }
}


/*******************************************************************************
* Function Name  : Baseline_FingerupdateRX
* Description    : When finger is touching the cover od SD card, NOISE is huge
*                  in some special TX or RX lines
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void Baseline_FingerupdateRX(void)
{
    uint16_t Max_num,i,j,average;
    int16_t Min_sum,Max_sum;
    uint16_t Max_cnt, Max_maxcnt, IvdCnt;
    Min_sum = 0;
    Max_num = 0;
    Max_sum = 0;
    Max_cnt    = 0;
    Max_maxcnt = 0;
    IvdCnt     = 0;
    for(i=0; i<XMTR_NUM; i++)
    {
        Max_sum = 0;
        average = 0;      // averagesum
        Min_sum = 32767; // mini
        Max_num = 0;
        Max_cnt    = 0;
        Max_maxcnt = 0;
        for(j=0; j<RECV_NUM; j++)
        {
            if(bdt.DeltaDat16A[i][j] < Min_sum)
                Min_sum = bdt.DeltaDat16A[i][j];
            if(bdt.DeltaDat16A[i][j] > BASE_UPDATETXRXTHR)
            {
                Max_sum += bdt.DeltaDat16A[i][j];
                Max_num++;
                Max_cnt++;
            }
            else
            {
               if(Max_cnt > Max_maxcnt)
               {
                   Max_maxcnt = Max_cnt;
                   average    = Max_sum/Max_cnt;
               }
               Max_cnt = 0;
               Max_sum = 0;
            }
        }
        if(average > 60)
        {
            if(Max_maxcnt == 10)
            {
                bdt.PowerOnWithFinger  = 1;
                return ;
            }
           else if(Max_maxcnt >= 7)
            {
                IvdCnt++;
                if(IvdCnt>=2)
                {
                    bdt.PowerOnWithFinger  = 1;
                    return ;
                }
            }
            else if(Max_maxcnt >=5 )
                IvdCnt++;
            if(IvdCnt >= 3)
            {
                bdt.PowerOnWithFinger = 1;
                return;
            }
        }
    }
}


/*******************************************************************************
* Function Name  : Baseline_Fingerupdatenoise
* Description    : When Palm is coving the Touch Panel, Big Noise is appeared
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void Baseline_Fingerupdatenoise(void)
{
    if(bdt.MaxNoise_Sum > BIGNOISE )
    {
        bdt.MaxNoise_SumCount++;
        bdt.PowerOnWithFinger = 0;
    }

    if(bdt.MaxNoise_SumCount>80)
    {
        bdt.MaxNoise_SumCount = 0;
        bdt.MaxNoise_Sum = 0;
        bdt.updatecount  = 100;
        bdt.PowerOnWithFinger = 1;
        return;
    }
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void Baseline_FingerJuge(uint16_t *buffer)
{
    #ifdef BSLN_WATERJUDGE
    uint16_t num,flag,x,y;
    if(  bdt.BFD.BufDeltSum > 10  )   // ??????
    {
        bdt.PowerOnWithFinger = 1;
        bdt.BFD.BufAbnomalProtectflag = 1;  //异常保护时间标志
        bdt.BFD.BufAbnomalflag = 1;
        bdt.BFD.WaterProtectTime = 0;
        return;
    }  // 处理有水上电
    if(bdt.BFD.WaterProtectTime < 100 && bdt.BFD.BufAbnomalProtectflag == 1)
    {
        bdt.PowerOnWithFinger = 1;
        return;
    }
    if(bdt.BFD.BufAbnomalflag == 1)
    {
        flag = 0;
        x = bdt.FRM_MAX_X_XMTR;
        y = bdt.FRM_MAX_Y_RECV;
        num = abs16(bdt.BFD.BufDatSaved[x][y] - buffer[x*RECV_NUM + y]);
        if(num < (bdt.MaxValueInFrame - (bdt.MaxValueInFrame >> 2)))
        {
            flag = 1;
        }
        if(flag == 1)
        {
             bdt.PowerOnWithFinger = 1;
             bdt.BFD.BufAbnomalProtectflag = 1;  //异常保护时间标志
             bdt.BFD.WaterProtectTime = 0; 
             return;
        }
        else
        {
            bdt.BFD.BufAbnomalflag = 0;
            bdt.BFD.BufAbnomalProtectflag = 0;  //无异常清零
        }
    }
    #endif
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void Baseline_FingerExistedHandled(uint16_t *buffer)
{
    uint16_t Min_num,Max_num,i,j;
    int16_t Min_sum,Max_sum;
    Min_num = 0;
    Min_sum = 0;
    Max_num = 0;
    Max_sum = 0;

    Baseline_Fingerupdatenoise();
    Baseline_FingerupdateTX();
    Baseline_FingerupdateRX();
    #ifdef BSLN_WATERJUDGE
    Baseline_FingerJuge(buffer);
    #endif
    if(bdt.PowerOnWithFinger)
        return;
     if(bdt.MinValueInFrame < (0-(bdt.PCBA.FrameMaxSample<<1)))
    {
        if(bdt.MEM_MIN_XY_Count > ABNORMAL_HOLD_TIME)
        {
            if((ABNORMAL_HOLD_TIME+1) == bdt.MEM_MIN_XY_Count)
            {
                bdt.PowerOnWithFinger = 1;
                return ;
            }
        
        }
        
       /*****************************************************************************
        * 完全相等，似乎是有些太强的条件约束了
        *****************************************************************************/
        if((bdt.MEM_MIN_X_XMTR == bdt.FRM_MIN_X_XMTR) && (bdt.MEM_MIN_Y_RECV == bdt.FRM_MIN_Y_RECV))
        {
            for(i=0;i<XMTR_NUM;i++)
            for(j=0;j<RECV_NUM;j++)
            {
                if(bdt.DeltaDat16A[i][j] >= bdt.PCBA.FrameMaxSample)
                {
                    Max_num++;
                    Max_sum += bdt.DeltaDat16A[i][j];
                }else if(bdt.DeltaDat16A[i][j] <= (0 - bdt.PCBA.FrameMaxSample))
                {
                    Min_num++;
                    Min_sum += bdt.DeltaDat16A[i][j];
                }
            }
            
            if(0 ==  Max_num)
            {
                bdt.MEM_MIN_XY_Count++;
            }
            else if(Min_num != 0)
            {
                if((Max_sum/Max_num) + (Min_sum/Min_num) < 5)
                {
                    bdt.MEM_MIN_XY_Count++;
                }
            }
        }
        else
        {
            bdt.MEM_MIN_XY_Count = 0;
        }
    }
    bdt.PowerOnWithFinger  = 0;
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint16_t Baseline_TwoFrameBufDelta(uint16_t *buffer)
{
	#ifdef BSLN_WATERJUDGE
    uint16_t i,j;
    uint16_t buf[XMTR_NUM * RECV_NUM];
    bdt.BFD.BufDeltSum = 0;
    bdt.BFD.BufDeltMax = 0;
    for (i=0;i<XMTR_NUM;i++)
        for (j=0;j<RECV_NUM;j++)
        {
            buf[ (i*RECV_NUM) + j] = abs16(buffer[(i*RECV_NUM) + j] - bdt.BFD.BufDatSaved[i][j]);
            bdt.BFD.BufDeltSum+=buf[ (i*RECV_NUM) + j];
            if(bdt.BFD.BufDeltMax < buf[ (i*RECV_NUM) + j])
            {
                bdt.BFD.BufDeltMax = buf[ (i*RECV_NUM) + j];
            }
        }
                
    bdt.BFD.BufDeltSum =  bdt.BFD.BufDeltSum >> 7;  //相邻两帧无指原始值的差值
    if(bdt.BFD.BufDeltSum > 5 || bdt.BFD.BufDeltMax > 25)
        return 1;
    else 
        return 0;
                                  //根据原始值变化判断是否有水滴被擦除
    #endif
return 0;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint8_t Baseline_HugeInvalidDatainDeltaFrame(void)
{
    uint8_t result = BASE_FRAME_HOLD;
    uint16_t Min_num,Max_num,i,j;
    int16_t Min_sum,Max_sum;
    Min_num = 0;
    Min_sum = 0;
    Max_num = 0;
    Max_sum = 0;

   /*********************************************************************************
   * If we have large value ,update base data, +100/-100
   *********************************************************************************/
    if(bdt.MaxValueInFrame > bdt.PCBA.FrameMaxSample)
    {
        if(bdt.MEM_MAX_XY_Count > ABNORMAL_HOLD_TIME)
        {
           /*bdt.MEM_MAX_XY_Count = 0;*/
            result = BASE_FRAME_UPDATE;
        }
        
       /*****************************************************************************
       * 完全相等，似乎是有些太强的条件约束了
       *****************************************************************************/
        if((bdt.MEM_MAX_X_XMTR == bdt.FRM_MAX_X_XMTR) && (bdt.MEM_MAX_Y_RECV == bdt.FRM_MAX_Y_RECV))
        {
            bdt.MEM_MAX_XY_Count++;
        }
        else
        {
            bdt.MEM_MAX_XY_Count = 0;
        }
    }
    else 
    {
        bdt.MEM_MAX_XY_Count = 0;
    }
    
    if(bdt.MinValueInFrame<(0-bdt.PCBA.FrameMaxSample))
    {
       /*CN1100_print("2."); */
        if(bdt.MEM_MIN_XY_Count > ABNORMAL_HOLD_TIME)
        {
           /*bdt.MEM_MIN_XY_Count = 0; */
            result = BASE_FRAME_UPDATE;
        }
        
       /*****************************************************************************
       * 完全相等，似乎是有些太强的条件约束了
       *****************************************************************************/
       /*CN1100_print("X0,X1,Y0,Y1=%d,%d,%d,%d\n",bdt.MEM_MIN_X_XMTR,bdt.FRM_MIN_X_XMTR,bdt.MEM_MIN_Y_RECV,bdt.FRM_MIN_Y_RECV);*/
        
        if((bdt.MEM_MIN_X_XMTR == bdt.FRM_MIN_X_XMTR) && (bdt.MEM_MIN_Y_RECV == bdt.FRM_MIN_Y_RECV))
        {
            for(i=0;i<XMTR_NUM;i++)
            for(j=0;j<RECV_NUM;j++)
            {
                if(bdt.DeltaDat16A[i][j] >= bdt.PCBA.FrameMaxSample)
                {
                    Max_num++;
                    Max_sum += bdt.DeltaDat16A[i][j];
                }else if(bdt.DeltaDat16A[i][j] <= (0 - bdt.PCBA.FrameMaxSample))
                {
                    Min_num++;
                    Min_sum += bdt.DeltaDat16A[i][j];
                }
            }
            if(0 ==  Max_num)
            {
                bdt.MEM_MIN_XY_Count++;
            }
            else if(Min_num != 0)
            {
                if((Max_sum/Max_num) + (Min_sum/Min_num) < 5)
                {
                    bdt.MEM_MIN_XY_Count++;
                }
            }
        }
        else
        {
            bdt.MEM_MIN_XY_Count = 0;
        }
    }
    else
    {
        bdt.MEM_MIN_XY_Count = 0;
    }
    return result;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint8_t Baseline_ManyBigInvalidSampleinDeltaFrame(void)
{
    uint16_t i, j;
    uint32_t temp_count = 0;
    
  /***********************************************************************
       * 如果一个差分帧内:
       *    数值超过"bdt.PCBA.AbnormalMaxDiff = ABNORMAL_MAX_DIFF(15)"
       *    个数超过"bdt.PCBA.AbnormalNumDiff = ABNORMAL_NUM_DIFF(1/4帧)"
       *    次数超过 ABNORMAL_HOLD_TIME(5)
       ***********************************************************************/
    for (i=0;i<XMTR_NUM;i++)
    for (j=0;j<RECV_NUM;j++)
    {
        if(abs16(bdt.DeltaDat16A[i][j]) <= bdt.ThresholdInFrame)
        if(abs16(bdt.DeltaDat16A[i][j]) >= bdt.PCBA.AbnormalMaxDiff)
            temp_count++;
    }
    
    if(temp_count > bdt.PCBA.AbnormalNumDiff)
    {
        bdt.BFD.AbnormalUpdateDelay++;
        if(bdt.BFD.AbnormalUpdateDelay > ABNORMAL_HOLD_TIME)
        {
            return BASE_FRAME_UPDATE;
        }
    }
    else
    {
        bdt.BFD.AbnormalUpdateDelay = 0;
    }
    return BASE_FRAME_HOLD;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

uint8_t Baseline_IsNeedUpdateBaseBuffer(void)
{
    uint16_t i, j;
    uint8_t  result = BASE_FRAME_HOLD;
    
   /*********************************************************************************
    * Step 1: Compute the sum of delta data
    *********************************************************************************/
    bdt.BFD.DeltaSum = 0;
    for (i=0;i<XMTR_NUM;i++)
    for (j=0;j<RECV_NUM;j++)
    {
        bdt.BFD.DeltaSum += abs16(bdt.DeltaDat16A[i][j]);
    }

    
   /*********************************************************************************
   * Step 2: We have two cases to handle
   *      Case 1: It's time to update, we should make sure that we do it or not.
   *      Case 2: It's Time to hold, but already updated base for a while
   *********************************************************************************/
    switch(bdt.BFD.bbdc.BaseUpdateCase)
    {
        case BASELINE_UPDATING_CASE:
        {
           /*************************************************************************
           * 这是主动要Update Baseline的时刻
           *************************************************************************/
            if(bdt.BFD.AfterBaseUpdatedTime < MAX_HOLDTIME_AFTERUPDATE)
            {
                return BASE_FRAME_HOLD;
            }
            
            if(bdt.BFD.FingerLeftProtectTime < bdt.PCBA.ProtectTime) 
            {
                return BASE_FRAME_HOLD;
            }
            
            if(bdt.BFD.TooLongTime4BaseUpdate >= bdt.PCBA.MaxUpdateTime)
            {
               /**********************************************************
               * 主动要Update的时刻，差分值一直都够小，很久了...
               **********************************************************/
                bdt.BFD.bbdc.BaseUpdateCase = BASELINE_HOLDING_CASE;
                bdt.BFD.TooLongTime4BaseUpdate = 0;
            }
            else
            {
               /**********************************************************
               * 主动要Update的时刻，必须差分值够小才可以做这件事
               **********************************************************/
                if(bdt.BFD.DeltaSum < (bdt.BFD.DeltaSumMaxThreshold) ) 
                {
                    return BASE_FRAME_UPDATE;
                }
                else
                {
                    return BASE_FRAME_HOLD;
                }
            }
            break;
        }
        case BASELINE_HOLDING_CASE:
        {
            if(BASE_FRAME_UPDATE == Baseline_HugeInvalidDatainDeltaFrame())
            {
                return BASE_FRAME_UPDATE;
            }
            
            if(BASE_FRAME_UPDATE == Baseline_ManyBigInvalidSampleinDeltaFrame())
            {
                return BASE_FRAME_UPDATE;
            }
            
           /*********************************************************************************
           *  If Update base data is not happened for more than MaxUpdateTime,
           *  update base data shall be happened 
           *********************************************************************************/
            if(bdt.BFD.TooLongTime4BaseUpdate >= bdt.PCBA.MaxUpdateTime)
            {
               /*****************************************************************************
               * It is too loog without update, we have to update base now;
               *****************************************************************************/
                bdt.BFD.bbdc.BaseUpdateCase = BASELINE_UPDATING_CASE;
                bdt.BFD.TooLongTime4BaseUpdate = 0;
            }
            break;
        }
        default:
            break;
    }
    
    return result;
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void Baseline_BaseBufferHandled(uint16_t *buffer)
{
    int i, j;
   /************************************************************************
   *    if bufdata is abnormal, Hold for Protection Time
   ************************************************************************/
    #ifdef BSLN_WATERJUDGE
    if( bdt.BFD.BufAbnomalProtectflag == 1)
    {
        bdt.BFD.WaterProtectTime++;
    }
    #endif
   /************************************************************************
   * After finger is just left, Hold for Protection Time
   ************************************************************************/
    if(bdt.BFD.FingerLeftProtectTime  < bdt.PCBA.ProtectTime)
    {    
        bdt.BFD.FingerLeftProtectTime++;
    }
   /************************************************************************
   * Too Long for us not to Update Baseline: MAX_MUST_UPDATE_PERIOD(4000)
   ************************************************************************/
    if(bdt.BFD.TooLongTime4BaseUpdate < bdt.PCBA.MaxUpdateTime)
    {
        bdt.BFD.TooLongTime4BaseUpdate++;
    }    
   /************************************************************************
   * After Updating Baseline, Hold for: MAX_HOLDTIME_AFTERUPDATE (2000)
   ************************************************************************/
    if(bdt.BFD.AfterBaseUpdatedTime < MAX_HOLDTIME_AFTERUPDATE) 
    {
        bdt.BFD.AfterBaseUpdatedTime++;
    }

    #ifdef TPD_PROXIMITY

    if(FACE_DETECT_NEAR == bdt.FDC.Flag) return;

    #endif
    
    if(bdt.BFD.bbdc.FingerExist == NO_FINGER)
    {
        #if ( defined (CONFIG_ARCH_SUN8IW3P1) || defined (CONFIG_ARCH_SUN8IW5P1) )// A23 and A33 || defined(CN1100_ATM)
        if(bdt.updatecount)
            bdt.updatecount--;
        #endif
        #ifdef BSLN_WATERJUDGE
        k = Baseline_TwoFrameBufDelta(buffer); 
        #endif
       /*******************************************************
       * Record how many time for the case of NO_FINGER
       *******************************************************/
        bdt.MTD.NoFingerCnt4Doze++;
        
        if(BASE_FRAME_HOLD == Baseline_IsNeedUpdateBaseBuffer())
        {
           /***************************************************
            * 基准值不需要更新, 于此故意设为有手指
            ***************************************************/
            bdt.BFD.bbdc.PreFingerExt = FINGER_SHOW;
            return;
        }
        #ifdef BSLN_WATERJUDGE
        if(k > 0)
        {  
            /***************************************************
             *              原始值错误停止更新
            ***************************************************/
            bdt.BFD.BufAbnomalflag = 1;  
            bdt.BFD.bbdc.NoFingerCnt4Base = 0;
            bdt.BFD.bbdc.PreFingerExt = FINGER_SHOW;
            return;
        }
        #endif
        if(bdt.BFD.bbdc.PreFingerExt != bdt.BFD.bbdc.FingerExist)
        {
           /*******************************************************
            * 更新基准数据，于此是第一帧，总共要4帧才得完成
            *******************************************************/
            bdt.BFD.bbdc.NoFingerCnt4Base = 0;
            for (i=0;i<XMTR_NUM;i++)
            for (j=0;j<RECV_NUM;j++)
            {
                bdt.BFD.SumBaseDat[i][j] = buffer[ (i*RECV_NUM) + j];
            }
        }
        else
        {
            for (i=0;i<XMTR_NUM;i++)
            for (j=0;j<RECV_NUM;j++)
            {
                bdt.BFD.SumBaseDat[i][j] += buffer[ (i*RECV_NUM) + j];
            }
        }
        
        bdt.BFD.bbdc.NoFingerCnt4Base++;
        
        if(BASE_COUNT_MAX == bdt.BFD.bbdc.NoFingerCnt4Base)
        {
          /****************************************************************
          * 四帧无手指原始数据求和，得到基准数据
          ****************************************************************/
            if(bdt.BaseChangeFlag < 65535)  bdt.BaseChangeFlag++;               /* Counting 1 for baseline updating */

            if(bdt.BaseChangeFlag<=3)
            {
                bdt.BFD.TooLongTime4BaseUpdate = MAX_MUST_UPDATE_PERIOD - 20;    /*  Reset the Timing Count*/
                bdt.BFD.AfterBaseUpdatedTime   = MAX_HOLDTIME_AFTERUPDATE - 20;  /*  Reset the Timing Count*/
            }
            else
            {
                bdt.BFD.TooLongTime4BaseUpdate = 0; /* Reset the Timing Count */
                bdt.BFD.AfterBaseUpdatedTime   = 0; /* Reset the Timing Count */
            }
            bdt.MEM_MAX_XY_Count = 0;
            bdt.BFD.bbdc.BaseUpdateCase    = BASELINE_HOLDING_CASE;
            
        #ifdef STM32VC_LCD
        #ifdef FHBS_DEBUG_INFOSHOW
            TFT_ShowNum(10,  20, bdt.BaseChangeFlag, LCD_COLOR_BLUE, LCD_COLOR_GREEN);
        #endif
        #endif
            for(i=0;i<XMTR_NUM;i++)
            for(j=0;j<RECV_NUM;j++)
            {
                /************************************************
                * 1. 利用上次得到的基准数据
                * 2. 记录这次得到的基准数据，以备下次使用
                ************************************************/
                bdt.BFD.BaseDat[i][j]      = bdt.BFD.BaseDatSaved[i][j]; 
                bdt.BFD.BaseDatSaved[i][j] = (bdt.BFD.SumBaseDat[i][j])>>BASE_FRAME_SHIFT;
                bdt.BFD.SumBaseDat[i][j]   = 0;
            }
            
           /*****************************************************************
           * Reset the Base Data Frame Integration
           *****************************************************************/
            bdt.BFD.bbdc.NoFingerCnt4Base = 0;
        }
    }
    else
    {
        bdt.BFD.bbdc.NoFingerCnt4Base = 0;
        bdt.BFD.FingerLeftProtectTime = 0;
        
        Baseline_FingerExistedHandled(buffer);
        
        if(0 == bdt.PrevFingerDetectNum2)
        {
            for (i=0;i<XMTR_NUM;i++)
            for (j=0;j<RECV_NUM;j++)
            {
                bdt.BFD.BaseDatSaved[i][j] = bdt.BFD.BaseDat[i][j];
            }
        }
        
       /*******************************************************
       * Reset the Record = 0 when any finger is coming
       *******************************************************/
        bdt.MTD.NoFingerCnt4Doze = 0;
    }
    
   /****************************************
   * Save the Finger existing status
   ****************************************/
    
    bdt.BFD.bbdc.PreFingerExt = bdt.BFD.bbdc.FingerExist;
}

/*******************************************************************************
* Function Name  : FingProc_Dist2PMeasure
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint16_t FingProc_Dist2PMeasure(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    int16_t x, y, t; 
    int16_t distance;
    
    x = x1 - x2;
    y = y1 - y2;
    if(x<0) 
    {
        x = 0 - x;
    }
    if(y<0)
    {
        y = 0 - y;
    }
    t = y - x;
    if(t>=0)
    {
        y -= t; 
        x += t;
    }
    distance = x + (y>>3) + (y>>2);
    return ((uint16_t)distance);
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint16_t FingProc_Dist4Uint16Var(uint16_t x0, uint16_t x1)
{
    if(x0>x1) 
    {
        return (x0-x1);
    }
    else
    { 
        return (x1-x0);
    }
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void FingProc_DistanceFilter(uint16_t i, uint16_t x1, uint16_t y1, uint16_t *pX, uint16_t *pY)
{
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void FingProc_DF0ExceptionHandle(uint16_t i, uint16_t *pX, uint16_t *pY)
{
    #define INTEGR_SHFT  3
    #define INTEGR_NUM   (1<<INTEGR_SHFT)

    //if(bad power suplly)
    //{
    //               *pX = bdt.DPD[i].AdjustOrigin_x;
    //               *pY = bdt.DPD[i].AdjustOrigin_y;
    //}
    //else
    {
           uint16_t td, xms, yms; // ms means middle speed
           bdt.DPD[i].StayCount++;
           bdt.DPD[i].Stay_XSum += *pX;
           bdt.DPD[i].Stay_YSum += *pY;
           *pX = bdt.DPD[i].AdjustOrigin_x;
           *pY = bdt.DPD[i].AdjustOrigin_y;
           if(bdt.DPD[i].StayCount >= INTEGR_NUM)
           {
               bdt.DPD[i].Stay_XSum = bdt.DPD[i].Stay_XSum>>INTEGR_SHFT;
               bdt.DPD[i].Stay_YSum = bdt.DPD[i].Stay_YSum>>INTEGR_SHFT;
               td = FingProc_Dist2PMeasure((int16_t)bdt.DPD[i].Stay_XSum, (int16_t)bdt.DPD[i].Stay_YSum, (int16_t)bdt.DPD[i].AdjustOrigin_x, (int16_t)bdt.DPD[i].AdjustOrigin_y);
               if(td > (bdt.DPD[i].AdjustDistance>>2))
               {
                   *pX = bdt.DPD[i].Stay_XSum;
                   *pY = bdt.DPD[i].Stay_YSum;
                   bdt.DPD[i].AdjustOrigin_x = *pX;
                   bdt.DPD[i].AdjustOrigin_y = *pY;
               }
               bdt.DPD[i].StayCount = 0;
               bdt.DPD[i].Stay_XSum = 0;
               bdt.DPD[i].Stay_YSum = 0;
           }
           #if 1
           else if(bdt.DPD[i].StayCount == (INTEGR_NUM>>1))
           {
               xms = (uint16_t)(bdt.DPD[i].Stay_XSum>>(INTEGR_SHFT-1));
               yms = (uint16_t)(bdt.DPD[i].Stay_YSum>>(INTEGR_SHFT-1));
               td  = FingProc_Dist2PMeasure((int16_t)xms, (int16_t)yms, (int16_t)bdt.DPD[i].AdjustOrigin_x, (int16_t)bdt.DPD[i].AdjustOrigin_y);
               if(td > (bdt.DPD[i].AdjustDistance>>1))
               {
                   *pX = xms; //bdt.DPD[i].Stay_XSum;
                   *pY = yms; //bdt.DPD[i].Stay_YSum;
                   bdt.DPD[i].AdjustOrigin_x = *pX;
                   bdt.DPD[i].AdjustOrigin_y = *pY;
                   bdt.DPD[i].StayCount = 0;
                   bdt.DPD[i].Stay_XSum = 0;
                   bdt.DPD[i].Stay_YSum = 0;
               }
           }
           #endif
    }
}

void FingProc_DF0GotoMovingState(uint16_t i, uint16_t *pX, uint16_t *pY)
{
#if 1
    bdt.DPD[i].StayCount      = 0;
    bdt.DPD[i].Stay_XSum      = 0;
    bdt.DPD[i].Stay_YSum      = 0;
    bdt.DPD[i].AdjustOrigin_x = *pX;
    bdt.DPD[i].AdjustOrigin_y = *pY;
    bdt.DPD[i].AdjustState    = STATE_MOVING;
 #endif
}


void FingProc_DistanceFilter0(uint16_t i, uint16_t x1, uint16_t y1, uint16_t *pX, uint16_t *pY)
{
    #if 1
    uint16_t distance;

    if((0 == bdt.DPD[i].AdjustOrigin_x) && (0 == bdt.DPD[i].AdjustOrigin_y))
    {
        bdt.DPD[i].AdjustOrigin_x = x1;
        bdt.DPD[i].AdjustOrigin_y = y1;
    }
    
    distance = FingProc_Dist2PMeasure((int16_t)bdt.DPD[i].AdjustOrigin_x, (int16_t)bdt.DPD[i].AdjustOrigin_y, (int16_t)(*pX), (int16_t)(*pY));
   
    #ifdef STM32VC_LCD
    #ifdef SUPER_FILTER4EDGE_DEBUGSHOW
    TFT_ShowNum(3+(i<<2), 28, bdt.DPD[i].AdjustState, LCD_COLOR_BLUE, LCD_COLOR_GREEN);
    #endif
    #endif
    
    switch(bdt.DPD[i].AdjustState)
    {
        /*************************************************************
        * 分成两种CASE, 是为了 在中途case "STATE_STICK_HERE" 可以更好
        * 地安排 手指报告点 接近 手指触摸物理点的 真实位置。这还没做!
        **************************************************************/
        case STATE_STICK_START:
        case STATE_STICK_HERE:
        {
            bdt.DPD[i].AdjustDistance = bdt.ThrLow4DistanceFilter;
            if(distance <= bdt.DPD[i].AdjustDistance)
            {                
                FingProc_DF0ExceptionHandle(i, pX, pY);
            }
            else
            {
                if(distance <= bdt.ThrHigh4DistanceFilter) 
                {
                    FingProc_DF0ExceptionHandle(i, pX, pY);
                    bdt.DPD[i].AdjustState = STATE_PREPARE_MOVE;
                }
                else 
                {
                    FingProc_DF0GotoMovingState(i, pX, pY);
                }
            }
            break;
        }
        case STATE_PREPARE_MOVE:
        {
            if(distance <= bdt.DPD[i].AdjustDistance)
            {
                FingProc_DF0ExceptionHandle(i, pX, pY);
                bdt.DPD[i].AdjustState = STATE_STICK_HERE;
            }
            else if(distance <= bdt.ThrHigh4DistanceFilter) 
            {
                if((FingProc_Dist4Uint16Var(*pX,x1) > bdt.DPD[i].AdjustDistance) && (FingProc_Dist4Uint16Var(*pY,y1) < THR024))
                {
                    FingProc_DF0ExceptionHandle(i, pX, pY);
                }
                else 
                {
                    FingProc_DF0GotoMovingState(i, pX, pY);
                }
            }
            else
            {
                FingProc_DF0GotoMovingState(i, pX, pY);
            }
            break;
        }
        case STATE_MOVING:
        {
            bdt.DPD[i].AdjustDistance = THR024;
            bdt.DPD[i].AdjustOrigin_x = *pX;
            bdt.DPD[i].AdjustOrigin_y = *pY;
            if(distance <= bdt.DPD[i].AdjustDistance)
            {
                bdt.DPD[i].AdjStickCounter = 0;  
                bdt.DPD[i].AdjustState = STATE_PREPARE_STICK;
                if(distance <= 12)
                    bdt.DPD[i].AdjStickCounter = 2;   
            }
            break ;
        }
        case STATE_PREPARE_STICK:
        {
            bdt.DPD[i].AdjustDistance = bdt.ThrLow4DistanceFilter;
            bdt.DPD[i].AdjustOrigin_x = *pX;
            bdt.DPD[i].AdjustOrigin_y = *pY;
            if(distance <= bdt.DPD[i].AdjustDistance)
            {
                //*****************************************************************
                // Moving --- Small Moving --- Stick
                //*****************************************************************
                bdt.DPD[i].AdjStickCounter++;
                if(bdt.DPD[i].AdjStickCounter > 3)
                {
                    bdt.DPD[i].AdjStickCounter = 0;          /* 静止有时, 可以换挡*/
                    bdt.DPD[i].AdjustState = STATE_STICK_HERE;
                }
            }
            else if(distance <= THR144)
            {
                //*****************************************************************
                // Moving --- Small Moving --- Bigger Moving --- Moving
                //*****************************************************************
                if((FingProc_Dist4Uint16Var(*pX,x1) > THR048) && (FingProc_Dist4Uint16Var(*pY,y1) < THR024))
                {
                }
                else 
                {
                    bdt.DPD[i].AdjStickCounter = 0;
                    bdt.DPD[i].AdjustState = STATE_MOVING;
                }
            }
            else
            {
                //*****************************************************************
                // Moving --- Small Moving --- Biggest Moving --- Moving
                //*****************************************************************
                bdt.DPD[i].AdjStickCounter = 0;
                bdt.DPD[i].AdjustState = STATE_MOVING;
            }
            break;
        }
    }
    #endif
}

/*************************************************************
* i: 手指 ID
* x1,y1: 手指在前一帧的坐标
* pX,pY: 手指在本帧的坐标的数据的变量指针
**************************************************************/
/*******************************************************************************
 * Function Name  : 
 * Description    : 
 * Input          : 
 * Output         : 
 * Return         : 
 *******************************************************************************/

void FingProc_DistanceFilter1(uint16_t i, uint16_t x1, uint16_t y1, uint16_t *pX, uint16_t *pY, uint16_t SideFlag)
{
#if 0
    int16_t xv, yv;
    int16_t xThr = 3, yThr = 3;   
    //************************************************
    // SideFlag == 1: XMTR_SIDE
    // SideFlag == 2: RECV_SIDE
    //************************************************
    if(1 == SideFlag) yThr = 6;
    else if(2 == SideFlag) xThr = 6;
    
    if(x1 > (*pX)) xv = x1 - (*pX);
    else xv = (*pX) - x1;

    if(y1 > (*pY)) yv = y1 - (*pY);
    else yv = (*pY) - y1;

    if( xv < xThr) (*pX) = x1; 
    if( yv < yThr) (*pY) = y1; 
#endif

}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

uint16_t FingProc_SingleForwardFilter(uint16_t i, uint16_t xoldest, uint16_t xolder, uint16_t curx, uint16_t MAX)
{
    int16_t px;
    
    if(bdt.DPD[i].FingerStateFlag >= STATE_LOST_1_FINGER)
    {
        px = (int16_t)xolder;
    }
    else
    {
        if(FINGER_FINGER_SPEED_FAST == bdt.DPD[i].FingMovingSpeed)
        {
            px = (int16_t)curx;
        }
        else 
        {
            px   = (int16_t)(xolder<<1) - (int16_t)xoldest; 
            px   = ((int16_t)curx + px)>>1;
        }
    }
    if(px <= 0)   
    {
        px = 1;
    }
    if(px >= MAX) 
    {
        px = MAX - 1;
    }
    return (uint16_t)px;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void FingProc_ForwardSmoothLine(uint16_t i, uint16_t *xs, uint16_t *ys)
{
    int16_t x1, y1, x2, y2;
    int16_t curx, cury, dist;
    int16_t x4, y4, x5, y5;
    
    x1   = bdt.DPD[i].Finger_X_S[1]; /* Oldest*/
    y1   = bdt.DPD[i].Finger_Y_S[1]; /* Oldest*/
    x2   = bdt.DPD[i].Finger_X_S[0]; /* Older*/
    y2   = bdt.DPD[i].Finger_Y_S[0]; /* Older*/
    curx = bdt.DPD[i].Finger_X_XMTR; /* Current*/
    cury = bdt.DPD[i].Finger_Y_RECV; /* Current*/
    dist = FingProc_Dist2PMeasure(x2, y2, curx, cury);
    
    if(bdt.DPD[i].FingerStateFlag >= STATE_LOST_1_FINGER) /* LOST Finger Point*/
    {
        x5 = x2;
        y5 = y2;
    }
    else if(FINGER_FINGER_SPEED_FAST == bdt.DPD[i].FingMovingSpeed) /* Too Fast Finger Moving*/
    {
        x5 = curx;
        y5 = cury;
    }
    else if( (dist>>2) > bdt.DPD[i].FF_LastDist) /* Too High Speed for time being*/
    {
        x5 = curx; 
        y5 = cury;
    }
    else
    {
        x4 = (x2<<1) - x1;
        y4 = (y2<<1) - y1;
        x5 = (curx + x4)>>1;
        y5 = (cury + y4)>>1;
        if(x5 < 0)
        { 
            x5 = 1;
        }
        if(y5 < 0)
        { 
            y5 = 1;
        }
        if(x5 >= (XMTR_NUM<<8)) 
        { 
            x5 = (XMTR_NUM<<8) - 1;
        }
        if(y5 >= (RECV_NUM<<8))
        { 
        y5 = (RECV_NUM<<8) - 1;
        }
    }
    
    bdt.DPD[i].Finger_X_S[0] = x5;
    bdt.DPD[i].Finger_Y_S[0] = y5;
    bdt.DPD[i].Finger_X_S[1] = x2;
    bdt.DPD[i].Finger_Y_S[1] = y2;
    bdt.DPD[i].FF_LastDist   = FingProc_Dist2PMeasure(x2,y2,x5,y5);
    
    bdt.DPD[i].Finger_X_XMTR = x5;
    bdt.DPD[i].Finger_Y_RECV = y5;
}


/*******************************************************************************
* Function Name  : FingProc_TapFilterEdgeProcess
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void FingProc_TapFilterEdgeProcess(uint16_t i, uint16_t curx, uint16_t cury, uint16_t *x, uint16_t *y)
{
    bdt.DPD[i].Finger_X_XMTR = (x[6]>>7) + (x[5]>>7) + (x[4]>>6) 
                               + (x[3]>>5) + (x[2]>>4) + (x[1]>>3)  + (x[0]>>2) +(curx>>1);  /*7阶 IIR 滤波*/
                               
    bdt.DPD[i].Finger_Y_RECV = (y[6]>>7) + (y[5]>>7) + (y[4]>>6) 
                               + (y[3]>>5) + (y[2]>>4) + (y[1]>>3)  + (y[0]>>2) +(cury>>1);  /* 7阶 IIR 滤波*/
}


/*******************************************************************************
* Function Name  : FingProc_TapFilterProcess
* Description    :  3阶 IIR 滤波
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void FingProc_TapFilterProcess(uint16_t i, uint16_t curx, uint16_t cury, uint16_t *x, uint16_t *y)
{
#if 1
    //bdt.DPD[i].Finger_X_XMTR = (x[2]>>3) + (x[1]>>3) + (x[0]>>2) + (curx>>1);  // 3阶 IIR 滤波
    //bdt.DPD[i].Finger_Y_RECV = (y[2]>>3) + (y[1]>>3) + (y[0]>>2) + (cury>>1);  // 3阶 IIR 滤波
    bdt.DPD[i].Finger_X_XMTR = (x[2] + x[1] + (x[0]<<1) + (curx<<2))>>3;  // 3?× IIR ??2¨
    bdt.DPD[i].Finger_Y_RECV = (y[2] + y[1] + (y[0]<<1) + (cury<<2))>>3;  // 3?× IIR ??2¨
#else
    bdt.DPD[i].Finger_X_XMTR = (curx>>3) + (x[1]>>3) + (x[0]>>2) + (curx>>1);  // 2阶 IIR 滤波
    bdt.DPD[i].Finger_Y_RECV = (cury>>3) + (y[1]>>3) + (y[0]>>2) + (cury>>1);  // 2阶 IIR 滤波
#endif
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void FingProc_TapFilterStateReset(uint16_t index, uint8_t FilterState)
{
    uint16_t  i;
    bdt.DPD[index].FingerStateFlag   = FilterState;
    for (i = 0; i < FINGER_INFO_SAVE_NUM; i++)
    {
        bdt.DPD[index].Prev_Finger_X[i]  = 0;
        bdt.DPD[index].Prev_Finger_Y[i]  = 0;
    }
    bdt.DPD[index].Finger_X_Reported = 0;
    bdt.DPD[index].Finger_Y_Reported = 0;
    bdt.DPD[index].LifeNumber        = 0;
    bdt.DPD[index].FingMovingSpeed   = FINGER_FINGER_SPEED_LOW;
    bdt.DPD[index].AdjustState       = STATE_STICK_START;

    bdt.DPD[index].StayCount         = 0;
    bdt.DPD[index].Stay_XSum         = 0;
    bdt.DPD[index].Stay_YSum         = 0;

}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void FingProc_TapFilterStateUpdate(uint16_t i)
{
    uint16_t  j;
    for (j = (FINGER_INFO_SAVE_NUM-1); j>0; j--)
    {
        bdt.DPD[i].Prev_Finger_X[j] = bdt.DPD[i].Prev_Finger_X[j-1];
        bdt.DPD[i].Prev_Finger_Y[j] = bdt.DPD[i].Prev_Finger_Y[j-1];
    }
    bdt.DPD[i].Prev_Finger_X[0] = bdt.DPD[i].Finger_X_XMTR;
    bdt.DPD[i].Prev_Finger_Y[0] = bdt.DPD[i].Finger_Y_RECV;
}

void FingProc_ShowXYResultOnLine(uint16_t line)
{
#ifdef STM32VC_LCD
    #ifdef SUPER_FILTER4EDGE_DEBUGSHOW
    uint16_t i;
    for (i=0; i<bdt.FingerDetectNum; i++)
    {
        if(bdt.DPD[i].Finger_X_XMTR != 0)
        if(bdt.DPD[i].Finger_Y_RECV != 0)
        {
            uint16_t posX=0+(i<<4), posY=line;
            TFT_ShowChar(posX,posY,'(',LCD_COLOR_BLUE,LCD_COLOR_GREEN);
            posX++;
            TFT_ShowNum(posX,posY,bdt.DPD[i].Finger_X_XMTR,LCD_COLOR_BLUE,LCD_COLOR_GREEN); 
            posX += 4;
            TFT_ShowChar(posX,posY,',',LCD_COLOR_BLUE,LCD_COLOR_GREEN);
            posX++;
            TFT_ShowNum(posX,posY,bdt.DPD[i].Finger_Y_RECV,LCD_COLOR_BLUE,LCD_COLOR_GREEN); 
            posX += 4;
            TFT_ShowChar(posX,posY,')',LCD_COLOR_BLUE,LCD_COLOR_GREEN);
            posX++;
            TFT_ShowChar(posX++,posY,' ',LCD_COLOR_BLUE,LCD_COLOR_GREEN);
            posX++;
        }
    }
#endif
#endif
}

/*******************************************************************************
* Function Name  : FingProc_MultiFilterProcess
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void FingProc_MultiFilterProcess(uint16_t i, uint16_t curx, uint16_t cury, uint16_t *x, uint16_t *y)
{
    FingProc_ForwardSmoothLine(i,x,y);

    curx = bdt.DPD[i].Finger_X_XMTR;
    cury = bdt.DPD[i].Finger_Y_RECV;
    FingProc_TapFilterProcess(i, curx,cury,x,y);

    FingProc_DistanceFilter0(i, x[0], y[0], &bdt.DPD[i].Finger_X_XMTR, &bdt.DPD[i].Finger_Y_RECV);
    
    FingProc_TapFilterStateUpdate(i);
    
    bdt.DPD[i].Finger_X_Reported = bdt.DPD[i].Finger_X_XMTR;
    bdt.DPD[i].Finger_Y_Reported = bdt.DPD[i].Finger_Y_RECV;
    #if 0
    {
        uint16_t max=0, mini=0xffff, cury;
        uint16_t d01;
        
        if(bdt.FingerDetectNum>2)
        {
            for (i=0;i<bdt.FingerDetectNum;i++)
            {
                cury = bdt.DPD[i].Finger_Y_Reported;    /* Just calculated from raw data*/
                if(cury>max)
                {
                    max = cury;
                }
                if(cury<mini)
                {
                    mini = cury;
                }
            }
            max = max - mini;
            if(max<128)
            {
                bdt.FingerRealNum1=7;
                d01  = FingProc_Dist4Uint16Var(x[0], x[1]);
                if(d01 > 64)
                {    
                    FingProc_TapFilterStateReset(i, STATE_NONE_FINGER);
                }
            }
        }
        else
        {
            bdt.FingerRealNum1=2;
        }
    }
    #endif
    
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void FingProc_LCD_Show_Line(uint16_t i, uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye)
{
    #ifdef STM32VC_LCD
    uint16_t xslcd;
    uint16_t yslcd;
    uint16_t xelcd;
    uint16_t yelcd;
    
    /*get the coordinate in the screen*/
    xslcd  = (uint16_t)(( ((uint32_t)xs) * LCD_XMTR_SCALE )>>16);
    yslcd  = (uint16_t)(( ((uint32_t)ys) * LCD_RECV_SCALE )>>16);
    xelcd  = (uint16_t)(( ((uint32_t)xe) * LCD_XMTR_SCALE )>>16);
    yelcd  = (uint16_t)(( ((uint32_t)ye) * LCD_RECV_SCALE )>>16);
    if(LCD_SCREEN_HIGH  <= xs)
    {
        xs = LCD_SCREEN_HIGH - 1;
    }
    if(LCD_SCREEN_WIDTH <= ys)
    { 
        ys = LCD_SCREEN_WIDTH - 1;
    }
    if(LCD_SCREEN_HIGH  <= xe)
    { 
        xe = LCD_SCREEN_HIGH - 1;
    }
    if(LCD_SCREEN_WIDTH <= ye)
    {
        ye = LCD_SCREEN_WIDTH - 1;
    }
    
    if(0 == bdt.LcdIsrFlag)
    {
        LCD_DisplayLine(xslcd, yslcd, xelcd, yelcd, LCD_COLOR_WHITE);
        bdt.DPD[i].Finger_X_LCD = xelcd;
        bdt.DPD[i].Finger_Y_LCD = yelcd;
    }
    #endif
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void FingProc_First4PointsFilter(uint16_t *x, uint16_t *y, uint16_t curx, uint16_t cury)
{
    // x[3] = x[3]; // The first point should not be changed
    // y[3] = y[3]; // The first point should not be changed
    
    x[2] = (x[2]>>1) + (x[2]>>3) + (x[3]>>2) + (x[3]>>3); /*(5/8 , 3/8)*/
    y[2] = (y[2]>>1) + (y[2]>>3) + (y[3]>>2) + (y[3]>>3); /* (5/8 , 3/8)*/

    x[1] = (x[1]>>1) + (x[1]>>3) + (x[2]>>2) + (x[3]>>3); /* (5/8 , 1/4, 1/8)*/
    y[1] = (y[1]>>1) + (y[1]>>3) + (y[2]>>2) + (y[3]>>3); /* (5/8 , 1/4, 1/8)*/

#if 1
    x[0] = (x[0]>>1) + (x[1]>>2) + (x[2]>>3) + (x[0]>>3); // (5/8 , 1/4, 1/8)
    y[0] = (y[0]>>1) + (y[1]>>2) + (y[2]>>3) + (y[0]>>3); // (5/8 , 1/4, 1/8)
#else
    x[0] = (x[0]>>1) + (x[1]>>2) + (x[2]>>3) + (x[3]>>3); // (1/2 , 1/4, 1/8, 1/8)
    y[0] = (y[0]>>1) + (y[1]>>2) + (y[2]>>3) + (y[3]>>3); // (1/2 , 1/4, 1/8, 1/8)
#endif

}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void  FingProc_TringleProcessFilter(uint16_t x0, uint16_t y0, uint16_t *x1, uint16_t *y1, uint16_t x2, uint16_t y2)
{
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

int16_t FingProc_BackForthLineJudge(uint16_t x2, uint16_t y2, uint16_t x1, uint16_t y1, uint16_t x0, uint16_t y0)
{
    int16_t  result = 0; 
    uint16_t d01, d12, d02, d_2Side, d_1Side;

    d01  = FingProc_Dist2PMeasure(x2, y2, x1, y1);
    d12  = FingProc_Dist2PMeasure(x1, y1, x0, y0);
    /******************************************************
          * Make sure d01 is longer than d12
          *******************************************************/
    if(d01<d12) 
    {
        d02=d01;
        d01=d12;
        d12=d02;
    }
    d02 = FingProc_Dist2PMeasure(x2, y2, x0, y0);
    d_2Side = d12 + d02;       /*(A + B)*/
    d_1Side = d01 + (d01>>2);  /* C * 1.0625; Since 0.0625 = COS(20);*/
    
    if(d_1Side > d_2Side)   /* Special Tringle  */
    if(d12 > (FINGER_ADJUST_DISTANCE_MAX<<2))       /* Short Side is long enough*/
    {
        result = 1;
    }
    return result;
}

/*******************************************************************************
* Function Name  : FingProc_JustBeginingProcess
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void FingProc_JustBeginingProcess(uint16_t i, uint16_t FF, uint8_t StateFlagWithFinger, uint8_t StateFlagWithoutFinger)
{
    uint16_t *x, *y, j;
#ifdef REDUCE_FTSENSING
    uint16_t Flag = 0;
#endif

   /**********************************
   * P0 is appeared, no reported
   ***********************************/
    if(FF != 0) 
    {
        bdt.DPD[i].FingerStateFlag = StateFlagWithFinger;        /*First Finger Point is found   */
        /*******************************************************
        * State0 save the current Piont P0, 常规处理
        ********************************************************/
        FingProc_TapFilterStateUpdate(i);
        bdt.DPD[i].Finger_X_Reported = 0;
        bdt.DPD[i].Finger_Y_Reported = 0;
        bdt.DPD[i].LifeNumber        = 1;
        
        /*******************************************************
        * Only One Finger, Just for this Finger "i", 特别处理
        ********************************************************/
        
      #ifdef REDUCE_FTSENSING
        if((bdt.NDD.Battery_Level >= BATTERY_WORSE) && (bdt.DPD[i].Finger_Y_RECV < (RECV_NUM<<7)))
        {
            if(STATE_THIRD_FINGER == StateFlagWithFinger)
                   Flag = 3;
        }
        else
        {
            if(STATE_FIRST_FINGER == StateFlagWithFinger) Flag = 1;
        }
        
        if(Flag > 0)
      #endif
        if(1 == bdt.FingerRealNum)
        {
            x = bdt.DPD[i].Prev_Finger_X;
            y = bdt.DPD[i].Prev_Finger_Y;
            bdt.DPD[i].FingerStateFlag   = STATE_SERIAL_FINGER; /* STATE_FORTH_FINGER;*/
            #ifdef REDUCE_FTSENSING
            for (j = Flag; j<FINGER_INFO_SAVE_NUM; j++)
            #else
            for (j = 1; j<FINGER_INFO_SAVE_NUM; j++)
            #endif
            {
                x[j] = bdt.DPD[i].Finger_X_XMTR;
                y[j] = bdt.DPD[i].Finger_Y_RECV;
            }
            bdt.DPD[i].Finger_X_S[0] = x[0];
            bdt.DPD[i].Finger_Y_S[0] = y[0];
            bdt.DPD[i].Finger_X_S[1] = x[1];
            bdt.DPD[i].Finger_Y_S[1] = y[1];
            bdt.DPD[i].FF_LastDist   = 0;
            bdt.DPD[i].Finger_X_Reported = x[0]; /*对于孤独第一点, 报告之*/
            bdt.DPD[i].Finger_Y_Reported = y[0]; /* 对于孤独第一点, 报告之*/
            #ifdef CN1100_STM32
            FingProc_LCD_Show_Line(i, x[1], y[1], x[0], y[0]); /* Report X/Y[1]*/
            #endif
            return;
        }
    }
    else 
    {
        FingProc_TapFilterStateReset(i, StateFlagWithoutFinger);  /* P1 is lost again*/
    }
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void FingProc_SmallAngleTringleProcess(uint16_t i, uint16_t *x, uint16_t *y, uint8_t SFWithoutF)
{
    uint16_t d01, d12, d02, d_2Side, d_1Side, j;

    d01  = FingProc_Dist2PMeasure(x[2], y[2], x[1], y[1]); /* 第0点 到 第1点的距离*/
    d12  = FingProc_Dist2PMeasure(x[1], y[1], x[0], y[0]); /* 第1点 到 第2点的距离*/

    if((d12>256) && (d12 > (d01<<2)))
    {
        /*****************************************************************
        *  第1点 到 第2点的距离 太大 (画线太快了) 且 大过 前一段线距 4倍
        *  这种情况，被认为是 有问题的
        *******************************************************************/
        if(bdt.DPD[i].FingerStateFlag < STATE_SERIAL_FINGER)
        FingProc_TapFilterStateReset(i, SFWithoutF);  /* Px is lost again*/
        else
        {
            for (j = 0; j < FINGER_INFO_SAVE_NUM; j++)
            {
                bdt.DPD[i].Prev_Finger_X[j] = bdt.DPD[i].AdjustOrigin_x;
                bdt.DPD[i].Prev_Finger_Y[j] = bdt.DPD[i].AdjustOrigin_y;
                bdt.DPD[i].FingerStateFlag  = SFWithoutF;
            }
        }
    }
    else
    {
       /*****************************************************************
       * 前两段线距，安排 d01 放置 长线段; d12 放置 短线段;
       *****************************************************************/
        if(d01<d12) 
        {
            d02=d01;
            d01=d12; 
            d12=d02;
        }
        
        
       /*****************************************************************
       *  虚拟的当前的 线距 d02 : 第0点 到 第2点的距离
       ******************************************************************/
        d02  = FingProc_Dist2PMeasure(x[0], y[0], x[2], y[2]);
        
       /*****************************************************************
       *  虚拟的当前的 线距 d02 + 短线距 ===> d_2side
       *  长线距 * (1.25)                ===> d_1side
       ******************************************************************/
        d_2Side = d12 + d02;       // 三角形两边之和(A + B)
        d_1Side = d01 + (d01>>2);  // C * 1.0625; Since 0.0625 = COS(20);
        
       /*****************************************************************
       *  若长线距 * (1.25) 大过 双线距离, 且短边大过 256, 则抛弃该手指
       ******************************************************************/
        if((d_1Side > d_2Side) && (d12 > THR024))
        {
           /******************************************************************
           * 这是一个奇怪的来回三角形, 禁止出现这种连线，这里其被认为是飞点
           ******************************************************************/
            if(bdt.DPD[i].FingerStateFlag < STATE_SERIAL_FINGER)
            {    
                FingProc_TapFilterStateReset(i, SFWithoutF);  // Px is lost again
            }
            else
            {
            
                for (j = 0; j < FINGER_INFO_SAVE_NUM; j++)
                {
                    bdt.DPD[i].Prev_Finger_X[j] = bdt.DPD[i].AdjustOrigin_x;
                    bdt.DPD[i].Prev_Finger_Y[j] = bdt.DPD[i].AdjustOrigin_y;
                    bdt.DPD[i].FingerStateFlag  = SFWithoutF;
                }
            }
        }
    }
}


/*******************************************************************************
* Function Name  : FingProc_unreasonblespeedProcess
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void FingProc_unreasonblespeedProcess(uint16_t i, uint16_t *x, uint16_t *y, uint8_t SFWithoutF)
{
    uint16_t d01, d12, d23, j;
    
    d01  = FingProc_Dist2PMeasure(x[3], y[3], x[2], y[2]); 
    d12  = FingProc_Dist2PMeasure(x[2], y[2], x[1], y[1]); 
    d23  = FingProc_Dist2PMeasure(x[1], y[1], x[0], y[0]);  
    
    if(d01>128)
    if(d23>128)
    {
        d01=d01<<1;
        d23=d23<<1;
        if((d12>d23)&&(d12>d01))
        { 
            if(bdt.DPD[i].FingerStateFlag < STATE_SERIAL_FINGER)
            {
                FingProc_TapFilterStateReset(i, SFWithoutF);  /*Px is lost again*/
            }
            else
            {
                for (j = 0; j < FINGER_INFO_SAVE_NUM; j++)
                {
                    bdt.DPD[i].Prev_Finger_X[j] = bdt.DPD[i].AdjustOrigin_x;
                    bdt.DPD[i].Prev_Finger_Y[j] = bdt.DPD[i].AdjustOrigin_y;
                    bdt.DPD[i].FingerStateFlag  = SFWithoutF;
                }
            }
        }
    }
    if(d12>128)
    {
        d01=d01>>1;
        d23=d23>>1;
        d12=d12<<1;
        if((d12<d01)&&(d12<d23))
        { 
            if(bdt.DPD[i].FingerStateFlag < STATE_SERIAL_FINGER)
            {
                FingProc_TapFilterStateReset(i, SFWithoutF);  /* Px is lost again*/
            }
            else
            {
                for (j = 0; j < FINGER_INFO_SAVE_NUM; j++)
                {
                    bdt.DPD[i].Prev_Finger_X[j] = bdt.DPD[i].AdjustOrigin_x;
                    bdt.DPD[i].Prev_Finger_Y[j] = bdt.DPD[i].AdjustOrigin_y;
                    bdt.DPD[i].FingerStateFlag  = SFWithoutF;
                }
            }
        }
    }
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void FingProc_AlreadyBeginingProcess(uint16_t i, uint16_t FF, uint8_t StateFlagWithFinger, uint8_t StateFlagWithoutFinger)
{
    uint16_t *x, *y;
    uint16_t d01;

    if(FF != 0) 
    {
        bdt.DPD[i].FingerStateFlag = StateFlagWithFinger;
        FingProc_TapFilterStateUpdate(i); 
        bdt.DPD[i].Finger_X_Reported = 0;
        bdt.DPD[i].Finger_Y_Reported = 0;
        bdt.DPD[i].LifeNumber++;

        x = bdt.DPD[i].Prev_Finger_X;
        y = bdt.DPD[i].Prev_Finger_Y;
        if((x[2]!=0) || (y[2]!=0))
        {     
            FingProc_SmallAngleTringleProcess(i, x, y, StateFlagWithoutFinger);
            FingProc_unreasonblespeedProcess(i, x, y, StateFlagWithoutFinger);
        }
        else
        {
            d01  = FingProc_Dist2PMeasure(x[0], y[0], x[1], y[1]);
             /*****************************************************************
                            * 这是一个太快速的开始点，这里其被认为是飞点
                            ******************************************************************/
            if(d01 > 256)
            {
                FingProc_TapFilterStateReset(i, StateFlagWithoutFinger);  /* Px is lost again*/
            }
        }
    }
    else 
    {
        FingProc_TapFilterStateReset(i, StateFlagWithoutFinger);  /* P1 is lost again*/
    }
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void FingProc_FromStickCaseProcess(uint16_t i, uint16_t FF, uint16_t curx, uint16_t cury, uint8_t NextFingerState)
{
    uint16_t j, *x, *y;

    x = bdt.DPD[i].Prev_Finger_X;
    y = bdt.DPD[i].Prev_Finger_Y;
    
    if(0 == FF) 
    {
        bdt.DPD[i].FingerStateFlag = STATE_LOST_1_FINGER;
        curx = bdt.DPD[i].Finger_X_Raw; /* Load the previous RAW data X*/
        cury = bdt.DPD[i].Finger_Y_Raw; /* Load the previous RAW data Y*/
        if((i + 1) > bdt.FingerDetectNum)
        {
            bdt.FingerDetectNum = i + 1;
        }
        #ifdef RAWPOINT4FINGMATCH
        bdt.DPD[i].Point_Hold     = FINGER_ID_HOLD_TIME;
        bdt.DPD[i].Finger_XRAW[0] = curx;
        bdt.DPD[i].Finger_YRAW[0] = cury;
        #endif
        FingProc_MultiFilterProcess(i, curx, cury, x, y);
    }
    else 
    {
        if(bdt.DPD[i].LifeNumber < 100)
        {
            bdt.DPD[i].LifeNumber++;
        }
        /*****************************************************************
                  * 正常计算 手指点滤波过程
                  ******************************************************************/
        FingProc_MultiFilterProcess(i, curx, cury, x, y);
        
        /*****************************************************************
                  * 但是不报告 到HOST系统 目前的手指点，报告到HOST的依旧是以前的点
                  ******************************************************************/
        bdt.DPD[i].Finger_X_Reported = x[bdt.DPD[i].FingerStateFlag - STATE_S_OUT1_FINGER+2]; 
        bdt.DPD[i].Finger_Y_Reported = y[bdt.DPD[i].FingerStateFlag - STATE_S_OUT1_FINGER+2]; 
        
        /*****************************************************************
                  * 首先判断 最新手指点 到 基地手指点之间的距离 是否太近了?
                  ******************************************************************/ 
        if(FingProc_Dist2PMeasure(curx, cury, bdt.DPD[i].Finger_X_Reported, bdt.DPD[i].Finger_Y_Reported) < bdt.ThrLow4DistanceFilter)
        {
            /********************************************
            * 发现手指点又回到基地
            *********************************************/
                       
            bdt.DPD[i].FingerStateFlag  = STATE_STAYAT_FINGER; /* 回到STICK点，此前手指点是飞点耳*/
            for(j = 0; j<FINGER_INFO_SAVE_NUM; j++)
            {
                bdt.DPD[i].Prev_Finger_X[j] = bdt.DPD[i].Finger_X_Reported; /*清除飞点 对 状态变量 造成的 不好的影响*/
                bdt.DPD[i].Prev_Finger_Y[j] = bdt.DPD[i].Finger_Y_Reported; /* 清除飞点 对 状态变量 造成的 不好的影响*/
            }
        }
        else
        {
         /********************************************
         * 手指点依旧离基地有距离
         **********************************************/
            bdt.DPD[i].FingerStateFlag  = NextFingerState;
            FingProc_SmallAngleTringleProcess(i, x, y, STATE_STAYAT_FINGER);
            FingProc_unreasonblespeedProcess(i, x, y, STATE_STAYAT_FINGER);
        }
    }
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void FingProc_ImproveByMultiFilters(void)
{
    uint16_t *x, *y, curx, cury;
    uint16_t i;
    uint16_t FINGER_FLAG = 0;
    
    for (i = 0;i < bdt.FingerReqNum;i++)
    {
        x    = bdt.DPD[i].Prev_Finger_X;    /* Point to the saving array*/
        y    = bdt.DPD[i].Prev_Finger_Y;
        curx = bdt.DPD[i].Finger_X_XMTR;    /* Just calculated from raw data*/
        cury = bdt.DPD[i].Finger_Y_RECV;    /* Just calculated from raw data */
        if(curx || cury)
        {
            FINGER_FLAG = 1;   /* Make sure there is finger @ i (index)*/
        }
        else
        {    
            FINGER_FLAG = 0;
        }
        
        switch(bdt.DPD[i].FingerStateFlag)
        {
            case STATE_NONE_FINGER:
            {
                bdt.DPD[i].AdjustOrigin_x = 0;
                bdt.DPD[i].AdjustOrigin_y = 0;
                FingProc_JustBeginingProcess(i, FINGER_FLAG, STATE_FIRST_FINGER, STATE_NONE_FINGER);
                break;
            }
            
            case STATE_FIRST_FINGER:
            {
                FingProc_AlreadyBeginingProcess(i, FINGER_FLAG, STATE_SECOND_FINGER, STATE_NONE_FINGER);
                break;
            }
            
            case STATE_SECOND_FINGER:
            {
                #ifdef REDUCE_FTSENSING
                FingProc_JustBeginingProcess(i, FINGER_FLAG, STATE_THIRD_FINGER, STATE_NONE_FINGER);
                #else
                FingProc_AlreadyBeginingProcess(i, FINGER_FLAG, STATE_THIRD_FINGER, STATE_NONE_FINGER);
                #endif
                break;
            }
            
            case STATE_THIRD_FINGER:
            {
                FingProc_AlreadyBeginingProcess(i, FINGER_FLAG, STATE_FORTH_FINGER, STATE_NONE_FINGER);
                bdt.DPD[i].JustPassStateFlag4 = 0;
                break;
            }
            
            case STATE_FORTH_FINGER:
            {
                /*************************************************************
                * When P4 is appeared, reported all (P0, P1, P2, P3, P4 ....)
                **************************************************************/
                if(FINGER_FLAG != 0) 
                {
                    bdt.DPD[i].JustPassStateFlag4 = 1;
                    bdt.DPD[i].FingerStateFlag = STATE_SERIAL_FINGER;
                    
                    bdt.DPD[i].Finger_X_S[0]     = x[0];
                    bdt.DPD[i].Finger_Y_S[0]     = y[0];
                    bdt.DPD[i].Finger_X_S[1]     = x[1];
                    bdt.DPD[i].Finger_Y_S[1]     = y[1];
                    bdt.DPD[i].FF_LastDist       = 0;
                    /*******************************************************
                    * Process the four point and Cur Point
                    ********************************************************/
                    FingProc_First4PointsFilter(x, y, curx, cury);
                    
                    #ifdef CN1100_STM32
                    FingProc_LCD_Show_Line(i, x[3], y[3], x[2], y[2]); /* Report X/Y[3]*/
                    FingProc_LCD_Show_Line(i, x[2], y[2], x[1], y[1]); /* Report X/Y[2]*/
                    FingProc_LCD_Show_Line(i, x[1], y[1], x[0], y[0]); /* Report X/Y[1]*/
                    #endif
                    
                    FingProc_MultiFilterProcess(i, curx, cury, x, y); 
                    
                    #ifdef CN1100_STM32
                    FingProc_LCD_Show_Line(i, x[1], y[1], x[0], y[0]); /*Report X/Y[0]*/
                    #endif
                } 
                else
                {
                    FingProc_TapFilterStateReset(i, STATE_NONE_FINGER);  /* P1 is lost again*/
                }
                break;
            }
            
            case STATE_SERIAL_FINGER:
            {
                /******************************************
                * P3 is appeared, reported p3', so on;
                *******************************************/
                bdt.DPD[i].JustPassStateFlag4 = 0;
                if(0 == FINGER_FLAG) 
                {
                    if(bdt.DPD[i].LifeNumber < 2)
                    {
                        FingProc_TapFilterStateReset(i, STATE_NONE_FINGER);  /*P1 is lost again*/
                        break;
                    }
                    else
                    {
                        bdt.DPD[i].FingerStateFlag = STATE_LOST_1_FINGER;
                        curx = bdt.DPD[i].Finger_X_Raw; /* Load the previous RAW data X*/
                        cury = bdt.DPD[i].Finger_Y_Raw; /* Load the previous RAW data Y*/
                        if((i + 1) > bdt.FingerDetectNum)    bdt.FingerDetectNum = i + 1;
                       
                    }
                    #ifdef RAWPOINT4FINGMATCH
                    bdt.DPD[i].Point_Hold     = FINGER_ID_HOLD_TIME;
                    bdt.DPD[i].Finger_XRAW[0] = curx;
                    bdt.DPD[i].Finger_YRAW[0] = cury;
                    #endif
                }
                else 
                {
                    
                    if(bdt.DPD[i].LifeNumber < 100) 
                    {
                        bdt.DPD[i].LifeNumber++;
                    }
                    
                    #ifdef FOURPOINTS4STICKMOVING
                    if(STATE_STICK_HERE == bdt.DPD[i].AdjustState) 
                    { 
                        bdt.DPD[i].FingerStateFlag  = STATE_STAYAT_FINGER;
                    }
                    else if((bdt.DPD[i].LifeNumber > 8) && (STATE_STICK_START == bdt.DPD[i].AdjustState))
                    {
                        bdt.DPD[i].FingerStateFlag  = STATE_STAYAT_FINGER;
                    }
                    #endif
                }
                FingProc_MultiFilterProcess(i, curx, cury, x, y);
                break;
            }
            
            case STATE_STAYAT_FINGER:
            {
                if(0 == FINGER_FLAG) 
                {
                    bdt.DPD[i].FingerStateFlag = STATE_LOST_1_FINGER;
                    curx = bdt.DPD[i].Finger_X_Raw; /* Load the previous RAW data X*/
                    cury = bdt.DPD[i].Finger_Y_Raw; /* Load the previous RAW data Y*/
                    
                    if((i + 1) > bdt.FingerDetectNum)
                    {
                        bdt.FingerDetectNum = i + 1;
                    }
                    #ifdef RAWPOINT4FINGMATCH
                    bdt.DPD[i].Point_Hold     = FINGER_ID_HOLD_TIME;
                    bdt.DPD[i].Finger_XRAW[0] = curx;
                    bdt.DPD[i].Finger_YRAW[0] = cury;
                    #endif
                    FingProc_MultiFilterProcess(i, curx, cury, x, y);
                }
                else 
                {
                    if(bdt.DPD[i].LifeNumber < 100) 
                    {    
                        bdt.DPD[i].LifeNumber++;
                    }
                    FingProc_MultiFilterProcess(i, curx, cury, x, y);
                    if(bdt.DPD[i].AdjustState > STATE_STICK_HERE)
                    {
                        /**********************************************************************
                        * 检测到手指可能要"大"动作，注意，"小动作"外部应该看不出来, STICK嘛
                        ***********************************************************************/
                        bdt.DPD[i].Finger_X_Reported = x[1]; /* 强行报告老的手指点数据*/
                        bdt.DPD[i].Finger_Y_Reported = y[1]; /* 强行报告老的手指点数据*/
                        bdt.DPD[i].FingerStateFlag = STATE_S_OUT1_FINGER;
                    }
                }
                break;
            }
            
            case STATE_S_OUT1_FINGER:
            {
                /**************************************************************************
                * 这个状态时过渡状态，会回到 (1)STATE_STAYAT_FINGER (2)STATE_LOST_1_FINGER
                * 或者 (3)STATE_S_OUT2_FINGER 各类状态，依不同的条件
                ***************************************************************************/
                FingProc_FromStickCaseProcess(i, FINGER_FLAG, curx, cury, STATE_S_OUT2_FINGER);
                break;
            }
            case STATE_S_OUT2_FINGER:
            {
                /**************************************************************************
                * 这个状态时过渡状态，会回到 (1)STATE_STAYAT_FINGER (2)STATE_LOST_1_FINGER
                * 或者 (3)STATE_S_OUT3_FINGER 各类状态，依不同的条件
                ***************************************************************************/
                FingProc_FromStickCaseProcess(i, FINGER_FLAG, curx, cury, STATE_S_OUT3_FINGER);
                bdt.DPD[i].JustPassStateFlag4 = 0;
                break;
            }
            case STATE_S_OUT3_FINGER:
            {
                /**********************************************************************
                * So Far, 我们已经发现了3个手指点都是正常的偏离基地点而去的手指点,所以
                * 如果这次(第四点)依旧是一个这样的手指点，那么就认为这是一次正常的手指
                * 移动了, 并且把之前的有效移动的手指点补偿性地重现出来
                ***********************************************************************/
                if(0 == FINGER_FLAG) 
                {
                    bdt.DPD[i].FingerStateFlag = STATE_LOST_1_FINGER;
                    curx = bdt.DPD[i].Finger_X_Raw; /* Load the previous RAW data X*/
                    cury = bdt.DPD[i].Finger_Y_Raw; /* Load the previous RAW data Y*/
                    if((i + 1) > bdt.FingerDetectNum)
                    {
                        bdt.FingerDetectNum = i + 1;
                    }
                    
                    #ifdef RAWPOINT4FINGMATCH
                    bdt.DPD[i].Point_Hold     = FINGER_ID_HOLD_TIME;
                    bdt.DPD[i].Finger_XRAW[0] = curx;
                    bdt.DPD[i].Finger_YRAW[0] = cury;
                    #endif
                    FingProc_MultiFilterProcess(i, curx, cury, x, y);
                }
                else 
                {
                     if(bdt.DPD[i].LifeNumber < 100)
                     {
                         bdt.DPD[i].LifeNumber++;
                     }
                     
                     if(FingProc_Dist2PMeasure(curx, cury, bdt.DPD[i].Finger_X_Reported, bdt.DPD[i].Finger_Y_Reported) < bdt.ThrLow4DistanceFilter)
                     {
                         /********************************************
                         * 发现手指点又回到基地
                         *********************************************/
                         uint16_t j;
                         bdt.DPD[i].FingerStateFlag  = STATE_STAYAT_FINGER; /* 回到STICK点，此前手指点是飞点耳*/
                         for (j = 0; j<FINGER_INFO_SAVE_NUM; j++)
                         {
                             bdt.DPD[i].Prev_Finger_X[j] = bdt.DPD[i].Finger_X_Reported; /* 清除飞点 对 状态变量 造成的 不好的影响*/
                             bdt.DPD[i].Prev_Finger_Y[j] = bdt.DPD[i].Finger_Y_Reported; /* 清除飞点 对 状态变量 造成的 不好的影响*/
                         }
                         break;
                     }
                     else
                     {
                         /********************************************
                         * 手指点依旧离基地有距离
                         *********************************************/
                         bdt.DPD[i].FingerStateFlag  = STATE_SERIAL_FINGER;
                         FingProc_SmallAngleTringleProcess(i, x, y, STATE_STAYAT_FINGER);
                         FingProc_unreasonblespeedProcess(i, x, y, STATE_STAYAT_FINGER);
                     }
                     
                     if(STATE_SERIAL_FINGER == bdt.DPD[i].FingerStateFlag)
                     {   
                         bdt.DPD[i].JustPassStateFlag4 = 1;
                     }
                     else 
                     {
                         break;
                     }
                     
                     #ifdef CN1100_STM32
                     FingProc_LCD_Show_Line(i, x[3], y[3], x[2], y[2]); /*Report X/Y[3]*/
                     FingProc_LCD_Show_Line(i, x[2], y[2], x[1], y[1]); /* Report X/Y[2]*/
                     FingProc_LCD_Show_Line(i, x[1], y[1], x[0], y[0]); /* Report X/Y[1]*/
                     #endif
                     FingProc_MultiFilterProcess(i, curx, cury, x, y);
                     #ifdef CN1100_STM32
                     FingProc_LCD_Show_Line(i, x[1], y[1], x[0], y[0]); /* Report X/Y[1]*/
                     #endif
                 
                }
                break;
            }
            case STATE_LOST_1_FINGER:
            {
                if(FINGER_FLAG) 
                {
                    bdt.DPD[i].FingerStateFlag  = STATE_SERIAL_FINGER;
                    FingProc_MultiFilterProcess(i, curx, cury, x, y); 
                }
                else
                {       
                    bdt.DPD[i].FingerStateFlag = STATE_LOST_2_FINGER;
                    
                    if((STATE_STICK_HERE == bdt.DPD[i].AdjustState) || (STATE_STICK_START == bdt.DPD[i].AdjustState)) 
                    {    
                        break;
                    }
                    
                    curx = bdt.DPD[i].Finger_X_Raw;
                    cury = bdt.DPD[i].Finger_Y_Raw;
                    if((i + 1) > bdt.FingerDetectNum)
                    {
                        bdt.FingerDetectNum = i + 1;
                    }
                    
                    FingProc_MultiFilterProcess(i, curx, cury, x, y); 
                    bdt.DPD[i].Finger_X_Reported  += bdt.DPD[i].Finger_X_Raw;
                    bdt.DPD[i].Finger_Y_Reported  += bdt.DPD[i].Finger_Y_Raw;
                    bdt.DPD[i].Finger_X_Reported >>= 1;
                    bdt.DPD[i].Finger_Y_Reported >>= 1;
                    x[0] = bdt.DPD[i].Finger_X_Reported;
                    y[0] = bdt.DPD[i].Finger_Y_Reported;
                    
                    
                    #ifdef RAWPOINT4FINGMATCH
                    bdt.DPD[i].Point_Hold     = FINGER_ID_HOLD_TIME;
                    bdt.DPD[i].Finger_XRAW[0] = curx;
                    bdt.DPD[i].Finger_YRAW[0] = cury;
                    #endif
                }
                break;
            }
            case STATE_LOST_2_FINGER:
            {
                if(FINGER_FLAG != 0) 
                {
                    bdt.DPD[i].FingerStateFlag   = STATE_SERIAL_FINGER;
                    FingProc_MultiFilterProcess(i, curx, cury, x, y);
                }
                else
                {
                    FingProc_TapFilterStateReset(i, STATE_NONE_FINGER);
                    
                    if((STATE_STICK_HERE == bdt.DPD[i].AdjustState) || (STATE_STICK_START == bdt.DPD[i].AdjustState)) 
                    { 
                        break;
                    }
                    bdt.DPD[i].Finger_X_Reported = bdt.DPD[i].Finger_X_Raw;
                    bdt.DPD[i].Finger_Y_Reported = bdt.DPD[i].Finger_Y_Raw;
                    bdt.DPD[i].Point_Hold        = FINGER_ID_HOLD_TIME;
                    bdt.DPD[i].AdjustDistance    = FINGER_ADJUST_DISTANCE_MAX;
                    if((i + 1) > bdt.FingerDetectNum)
                    { 
                        bdt.FingerDetectNum = i + 1;
                    }
                    
                
                    
                    #ifdef RAWPOINT4FINGMATCH
                    bdt.DPD[i].Finger_XRAW[0] = 0;
                    bdt.DPD[i].Finger_YRAW[0] = 0;
                    bdt.DPD[i].Finger_XRAW[1] = 0;
                    bdt.DPD[i].Finger_YRAW[1] = 0;
                    bdt.DPD[i].Finger_XRAW[2] = 0;
                    bdt.DPD[i].Finger_YRAW[2] = 0;
                    #endif
                }
                break;
            }
            default:
            {
                FingProc_TapFilterStateReset(i, STATE_NONE_FINGER);
                break;
            }
        }
        
        /*******************************************************
        * Save the raw data (Point - Axial data) for the next
        ********************************************************/
        if(FINGER_FLAG != 0)
        {
            bdt.DPD[i].Finger_X_Raw     = curx;
            bdt.DPD[i].Finger_Y_Raw     = cury;
        }
    }
    
    
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint16_t FingProc_SuperF4EAGE_DualDirMeasure(uint16_t *p, uint16_t Rpt)
{
    int16_t i = 0;
#ifdef SUPER_FILTER4EDGE
    if(((int16_t)Rpt - (int16_t)p[0]) > 0)
    {
        if(((int16_t)p[0] - (int16_t)p[1]) > 0)
        {
            i++;
            if(((int16_t)p[1] - (int16_t)p[2]) > 0)
            {
                i++;
                if(((int16_t)p[2] - (int16_t)p[3]) > 0) i++;
            }
        }
    }
    else
    {
        if(((int16_t)p[0] - (int16_t)p[1]) < 0)
        {
            i++;
            if(((int16_t)p[1] - (int16_t)p[2]) > 0)
            {
                i++;
                if(((int16_t)p[2] - (int16_t)p[3]) > 0) i++;
            }
        }
    }
#endif
    return ((uint16_t)(i));
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint16_t FingProc_SuperF4EAGE_ShiftCalc(uint16_t OrigShiftVal, uint16_t curp, uint16_t *p)
{
    uint16_t ShiftResult = OrigShiftVal;
#ifdef SUPER_FILTER4EDGE
    uint16_t DirVal;

    DirVal = FingProc_SuperF4EAGE_DualDirMeasure(p,curp); // DirVal could be 0,1,2,3

    //***********************************************************************
    // 0, Just change direction
    // 1, Or only has one more same direction 
    // 2, Existed 2 more same direction
    // 3, Existed 3 more same direction, means 4 same direction is existed
    //***********************************************************************
    if(0 == DirVal) 
    {
            //****************************************************
            // Finger Moving Direction is not fixed
            //****************************************************
            if(p[0] == p[1] && p[1]== p[2])
            {
                //****************************************************
                // The Finger Moving is locked
                //****************************************************
                uint16_t tempflag = 0;
                if((curp < SUPFIL_RANGE) && ((p[0]-curp)>16)) tempflag = 1;
                else if((curp > (SUPFIL_RANGE<<1)) && ((curp-p[0])>16)) tempflag = 1;

                if((ShiftResult > 0) && 1 == tempflag)
                {  
                    ShiftResult -= 1;
                }
            }
            else if(ShiftResult < 3)
            {
                //****************************************************
                // We are going to make filter more strong
                //****************************************************
                ShiftResult += 1;
            }
        }
        else if(DirVal > 1)
        {
            //********************************************************
            // Finger Moving Direction show a kind of fixing method
            // We are going to make filter less strong
            //********************************************************
            if(ShiftResult > 0)
            {  
                ShiftResult -= 1;
            }
    }
#endif
    return ShiftResult;
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void FingProc_SuperF4EAGE_LR(uint16_t i,uint16_t *x, uint16_t *y)
{
#ifdef SUPER_FILTER4EDGE
    uint16_t xRpt, yRpt;
    uint16_t dx, dy;
    //uint16_t DirVal;
    uint16_t OffsetTab[4] = {64,32,16,8};

    xRpt = bdt.DPD[i].Finger_X_XMTR;    /* Just calculated from raw data */
    yRpt = bdt.DPD[i].Finger_Y_RECV;    /* Just calculated from raw data */

    dx = FingProc_Dist4Uint16Var(xRpt, x[0]);
    dy = FingProc_Dist4Uint16Var(yRpt, y[0]);

    if((dx > (dy>>2)) && (dy < SUPFIL_RANGE))
    {
        bdt.DPD[i].EdgeShift_LR  = FingProc_SuperF4EAGE_ShiftCalc(bdt.DPD[i].EdgeShift_LR, xRpt, x);
        bdt.DPD[i].EdgeOffset_LR = OffsetTab[bdt.DPD[i].EdgeShift_LR];

        if(dx<128) 
        {
            dx = dx>>(bdt.DPD[i].EdgeShift_LR);
        }
        else  
        {
            dx = bdt.DPD[i].EdgeOffset_LR + (dx>>(bdt.DPD[i].EdgeShift_LR+1));
        }
    }
        
    if(xRpt > x[0])
    {
        bdt.DPD[i].Finger_X_XMTR = x[0]+dx;
    }
    else
    { 
        bdt.DPD[i].Finger_X_XMTR = x[0]-dx;
    }
#endif
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void FingProc_SuperF4EAGE_TB(uint16_t i,uint16_t *x, uint16_t *y)
{
#ifdef SUPER_FILTER4EDGE
    uint16_t xRpt, yRpt;
    uint16_t dx, dy;
    //uint16_t DirVal;
    uint16_t OffsetTab[4] = {64,32,16,8};

    xRpt = bdt.DPD[i].Finger_X_XMTR;    /* Just calculated from raw data*/
    yRpt = bdt.DPD[i].Finger_Y_RECV;    /* Just calculated from raw data */

    dx =FingProc_Dist4Uint16Var(xRpt, x[0]);
    dy =FingProc_Dist4Uint16Var(yRpt, y[0]);
    if((dy > (dx>>2)) && (dx < SUPFIL_RANGE))
    {
        bdt.DPD[i].EdgeShift_TB  = FingProc_SuperF4EAGE_ShiftCalc(bdt.DPD[i].EdgeShift_TB, xRpt, x);
        bdt.DPD[i].EdgeOffset_TB = OffsetTab[bdt.DPD[i].EdgeShift_TB];

        if(dy<128) 
        { 
            dy = dy>>(bdt.DPD[i].EdgeShift_TB);
        }
        else  
        { 
            dy = bdt.DPD[i].EdgeOffset_TB + (dy>>(bdt.DPD[i].EdgeShift_TB + 1));
        }
    }
                  
    if(yRpt>y[0])
    {
        bdt.DPD[i].Finger_Y_RECV = y[0]+dy ;
    }
    else
    {
        bdt.DPD[i].Finger_Y_RECV = y[0]-dy;
    }
#endif
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void FingProc_SuperFilter4Edge(void)
{
#ifdef SUPER_FILTER4EDGE
    uint16_t i, xRpt, yRpt;
    uint16_t *x, *y;

    for (i=0; i<bdt.FingerDetectNum; i++)
    {

        FingProc_ShowXYResultOnLine(1); // Line 1

        if(bdt.DPD[i].FingerStateFlag < STATE_SERIAL_FINGER) 
        {    
            break;
        }

        #ifdef FROMOUT2IN_INORDER
        x = bdt.DPD[i].Finger_X_Erpt;   /* Point to the saving array     */
        y = bdt.DPD[i].Finger_Y_Erpt;
        #else 
        x = bdt.DPD[i].Prev_Finger_X;    /* Point to the saving array     */
        y = bdt.DPD[i].Prev_Finger_Y;
        #endif
        xRpt = bdt.DPD[i].Finger_X_XMTR;    /* Just calculated from raw data */
        yRpt = bdt.DPD[i].Finger_Y_RECV;    /* Just calculated from raw data */

        if( (xRpt > SUPFIL_RANGE) && (xRpt < ((SXMTR_NUM<<8)-SUPFIL_RANGE)) )
        { 
            //************************************************************
            // Just from Edge to AA area, (Less than 10 points)
            //************************************************************
            if(bdt.DPD[i].FingerRealNum1_X < 10) 
            {
                bdt.DPD[i].FingerRealNum1_X++;
            } 
            else
            {
                //********************************************************
                // Finger really @ Out from SideArea, Num1_X Counting start
                //********************************************************
                bdt.DPD[i].FingerRealNum2_X  = 0;
                bdt.DPD[i].EdgeShift_LR       = 0;
                bdt.DPD[i].EdgeOffset_LR      = 64;
            }
        }
        else
        {
            //********************************************************
            // Finger @ SideArea, Num2_X Counting start
            //********************************************************
            if(xRpt != 0)
                if(yRpt != 0)
                { 
                    bdt.DPD[i].FingerRealNum1_X = 0;
                    //************************************************************
                    // Just from AA to Edge area, (Less than 20/35 points)
                    //************************************************************
                    if(bdt.DPD[i].FingerRealNum2_X > 20)  
                    { 
                        if(xRpt < SUPFIL_RANGE && x[0] < SUPFIL_RANGE && x[1] < SUPFIL_RANGE && x[2] < SUPFIL_RANGE)
                            FingProc_SuperF4EAGE_LR(i,x,y);

                        if(bdt.DPD[i].FingerRealNum2_X > 35)  
                        {
                            #define RIEL_VAL  ((SXMTR_NUM<<8) - SUPFIL_RANGE)
                            if(xRpt > RIEL_VAL && x[0] > RIEL_VAL && x[1] > RIEL_VAL && x[2] > RIEL_VAL)
                                FingProc_SuperF4EAGE_LR(i,x,y);
                        }
                        else bdt.DPD[i].FingerRealNum2_X++;
                    }
                    else bdt.DPD[i].FingerRealNum2_X++;
                }
        }

        if( (yRpt > SUPFIL_RANGE) && (yRpt < ((SRECV_NUM<<8)-SUPFIL_RANGE)) )
        { 
            #if 1
            //************************************************************
            // Just from Edge to AA area, (Less than 10 points)
            //************************************************************
            if(bdt.DPD[i].FingerRealNum1_Y < 10) 
            { 
                bdt.DPD[i].FingerRealNum1_Y++;
            }
            else
            {
                //********************************************************
                // Finger @ Out from SideArea, Num1_Y Counting start
                //********************************************************
                bdt.DPD[i].FingerRealNum2_Y  = 0;
                bdt.DPD[i].EdgeShift_TB      = 0;
                bdt.DPD[i].EdgeOffset_TB     = 64;
            }

            #endif
        }
        else
        {
            //********************************************************
            // Finger @ SideArea, Num2_Y Counting start
            //********************************************************
            if(bdt.DPD[i].Finger_X_XMTR != 0)
                if(bdt.DPD[i].Finger_Y_RECV != 0)
                {
                    bdt.DPD[i].FingerRealNum1_Y = 0;
                    //************************************************************
                    // Just from AA to Edge area, (Less than 20 points)
                    //************************************************************
                    if(bdt.DPD[i].FingerRealNum2_Y > 20)
                    {
                        #define BIEL_VAL  ((SRECV_NUM<<8)-SUPFIL_RANGE)
                        if(yRpt < SUPFIL_RANGE && y[0] < SUPFIL_RANGE && y[1] <SUPFIL_RANGE && y[2] < SUPFIL_RANGE)
                            FingProc_SuperF4EAGE_TB(i,x,y);
                        else if( yRpt > BIEL_VAL && y[0] > BIEL_VAL && y[1] > BIEL_VAL && y[2] > BIEL_VAL ) 
                            FingProc_SuperF4EAGE_TB(i,x,y);
                    }
                    else bdt.DPD[i].FingerRealNum2_Y++;
                }
        }

        #ifdef FROMOUT2IN_INORDER
        x[2]=x[1]; x[1]=x[0]; x[0]=bdt.DPD[i].Finger_X_XMTR;
        y[2]=y[1]; y[1]=y[0]; y[0]=bdt.DPD[i].Finger_Y_RECV;
        #endif
    }
#endif
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
#ifdef PRESS_KEY_DETECT
/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
#ifdef CN1100_STM32 //CN1100_STM32
void DataProc_PressKeyDetect()
{
    bdt.PressKeyFlag1          = NO_KEY_PRESSED;
    bdt.PressKeyFlag.MenuKey   = NO_KEY_PRESSED;
    bdt.PressKeyFlag.HomeKey   = NO_KEY_PRESSED;
    bdt.PressKeyFlag.ReturnKey = NO_KEY_PRESSED;
    
    if(bdt.FingerDetectNum)
    {
    #if (KXMTR_NUM == 1)
        if(bdt.DeltaDat16A[KEY_TX_LOC][KEY_MENU_RXLOC] > KEY_MENU_RXLOCATION) 
        {
            bdt.PressKeyFlag.MenuKey   = 1;
        }
        if(bdt.DeltaDat16A[KEY_TX_LOC][KEY_HOME_RXLOC] > KEY_HOME_RXLOCATION)
        {   
            bdt.PressKeyFlag.HomeKey   = 1;
        }
        if(bdt.DeltaDat16A[KEY_TX_LOC][KEY_RET_RXLOC]  > KEY_RET_RXLOCATION) 
        {
            bdt.PressKeyFlag.ReturnKey = 1;
        }
        
        if(1 == bdt.PressKeyFlag.MenuKey)
        { 
            bdt.PressKeyFlag1 = MENU_KEY_PRESSED;
        }
        else if(1 == bdt.PressKeyFlag.HomeKey)
        {
            bdt.PressKeyFlag1 = HOME_KEY_PRESSED;
        }
        else if(1 == bdt.PressKeyFlag.ReturnKey)
        {
            bdt.PressKeyFlag1 = RETURN_KEY_PRESSED;
        }
    #endif


    #if 0 // (KRECV_NUM == 1)
        if(   (bdt.DeltaDat_kp[3] > KEY_MENU_RXLOCATION) 
           || (bdt.DeltaDat_kp[4] > KEY_MENU_RXLOCATION) 
           || (bdt.DeltaDat_kp[3]+bdt.DeltaDat_kp[4] > KEY_MENU_RXLOCATION) 
        {
            bdt.PressKeyFlag.MenuKey   = 1;
        }
        if(bdt.DeltaDat16A[KEY_TX_LOC][KEY_HOME_RXLOC] > KEY_HOME_RXLOCATION)
        {   
            bdt.PressKeyFlag.HomeKey   = 1;
        }
        if(bdt.DeltaDat16A[KEY_TX_LOC][KEY_RET_RXLOC]  > KEY_RET_RXLOCATION) 
        {
            bdt.PressKeyFlag.ReturnKey = 1;
        }
        
        if(1 == bdt.PressKeyFlag.MenuKey)
        { 
            bdt.PressKeyFlag1 = MENU_KEY_PRESSED;
        }
        else if(1 == bdt.PressKeyFlag.HomeKey)
        {
            bdt.PressKeyFlag1 = HOME_KEY_PRESSED;
        }
        else if(1 == bdt.PressKeyFlag.ReturnKey)
        {
            bdt.PressKeyFlag1 = RETURN_KEY_PRESSED;
        }
    #endif


    }
}
#endif //CN1100_STM32

#endif


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void FingProc_CheckCrossedLines(void)
{
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void FingProc_FigureOutLeastFromMatrix(uint16_t dist[FINGER_NUM_MAX][FINGER_NUM_MAX], uint16_t Dim2idx[2])
{
    uint16_t i, j;
    uint16_t distmin = MAX_UINT16;

    Dim2idx[0] = FINGER_NUM_MAX;    /* Previous Point*/
    Dim2idx[1] = FINGER_NUM_MAX;    /* Current Point*/
    for (j = 0; j < bdt.PrevFingerDetectNum; j++)
        for (i = 0; i < bdt.FingerDetectNum; i++)
        {
            if(distmin > dist[j][i])
            {
                Dim2idx[0] = j;
                Dim2idx[1] = i;
                distmin = dist[j][i];
            }
        }
    /**************************************
    * Erase the Matrix 
    ***************************************/
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

uint16_t FingProc_TriangleDist3P(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
    int16_t  mx, my;
#ifdef RAWPOINT4FINGMATCH
    mx = (int16_t)((x1+x3)>>1);
    my = (int16_t)((y1+y3)>>1);
#else
    /*************************************
    * x1, y1 is the oldest point
    * x3, y3 i sthe newest point
    * "1/3" is about equals to "5/16"
    **************************************/
    mx = (int16_t)x3 - (int16_t)x1;
    my = (int16_t)y3 - (int16_t)y1;
    mx = (mx>>2) + (mx>>4) + x1;
    my = (my>>2) + (my>>4) + y1;
#endif
    return FingProc_Dist2PMeasure((int16_t)x2, (int16_t)y2, mx, my);
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void FingProc_SortFingersWithMatrix(void)
{
    uint16_t i,  j;
    uint16_t *x, *y;
    uint16_t dist[FINGER_NUM_MAX][FINGER_NUM_MAX], Dim2idx[2];
    uint16_t Pos_X[FINGER_NUM_MAX];      
    uint16_t Pos_Y[FINGER_NUM_MAX];      
    uint16_t Prev2CurID[FINGER_NUM_MAX]; 
    uint16_t Cur2PrevID[FINGER_NUM_MAX]; 
    uint16_t flag    = 0, tdist,tdist2,tdist1;
    uint16_t MaxPos  = 0, minNum;
    
    /**************************************************
    * Step 1: Initialization of VARs
    ***************************************************/
    for (i = 0; i < bdt.FingerDetectNum; i++)
    {
        Pos_X[i] = bdt.DPD[i].Finger_X_XMTR;
        Pos_Y[i] = bdt.DPD[i].Finger_Y_RECV;
    }
    for (i = 0; i < FINGER_NUM_MAX; i++)    
    {
        for (j = 0; j < FINGER_NUM_MAX; j++)
        {
            dist[i][j]   = MAX_UINT16; 
        }
        Prev2CurID[i] = FINGER_NUM_MAX;
        Cur2PrevID[i] = FINGER_NUM_MAX;
        bdt.DPD[i].Finger_X_XMTR = 0;  /* SAVE 配对最后的结果*/
        bdt.DPD[i].Finger_Y_RECV = 0;  /* SAVE 配对最后的结果*/
    }
    for (i = bdt.FingerDetectNum; i < FINGER_NUM_MAX; i++)
    {
        Pos_X[i] = 0;
        Pos_Y[i] = 0;
    }
    
    if(0 == bdt.FingerDetectNum)
    {
        return;
    }
    
    /**************************************************
    * Step 2A: Triangle Algorithm [1], [0], CUR
    ***************************************************/
    for (j = 0; j < bdt.PrevFingerDetectNum; j++)
    {
        #ifdef RAWPOINT4FINGMATCH
        x     = bdt.DPD[j].Finger_XRAW;
        y     = bdt.DPD[j].Finger_YRAW;
        #else
        x     = bdt.DPD[j].Prev_Finger_X;
        y     = bdt.DPD[j].Prev_Finger_Y;
        #endif
        
        if(x[1] || y[1])                 /* 前帧是有手指的*/
        {
            for (i = 0; i < bdt.FingerDetectNum; i++)
            {
                dist[j][i] = FingProc_TriangleDist3P(x[1], y[1], x[0], y[0], Pos_X[i], Pos_Y[i]);
            }
        }
    }
    
    minNum = bdt.PrevFingerDetectNum;
    if(minNum > bdt.FingerDetectNum) 
    {
        minNum = bdt.FingerDetectNum; 
    }

    for (j = 0; j < minNum; j++)
    {
        /*****************************************************
        * Extract the possible MAPPING
        ******************************************************/
        FingProc_FigureOutLeastFromMatrix(dist, Dim2idx);
        
        if(Dim2idx[0] < FINGER_NUM_MAX)
        {
            #if 1
            flag   = 1;
            
            #ifdef RAWPOINT4FINGMATCH
            x = bdt.DPD[Dim2idx[0]].Finger_XRAW;
            y = bdt.DPD[Dim2idx[0]].Finger_YRAW;
            #else
            x = bdt.DPD[Dim2idx[0]].Prev_Finger_X;
            y = bdt.DPD[Dim2idx[0]].Prev_Finger_Y;
            #endif
            tdist1  = FingProc_Dist2PMeasure(x[0], y[0], Pos_X[Dim2idx[1]], Pos_Y[Dim2idx[1]]); /* 当前距离*/
            tdist2 = FingProc_Dist2PMeasure(x[0], y[0], x[1], y[1]);
            tdist  = (tdist1 + tdist2)>>1;     /* 两条三角边长的平均值*/
            if(tdist > FINGER_ADJUST_DISTANCE_MIN)
            {
               
                tdist = (tdist>>1);                 /* 两条三角边长的平均值 的 0.50(60度)*/
                /**********************************************************************************
                * 如果 三角距离 大于 两条三角边长的平均值 的 3/4, 夹角近弱90°，则趋势判断不好用
                ***********************************************************************************/
                if(dist[Dim2idx[0]][Dim2idx[1]] > tdist) 
                {
                    flag = 0;
                   
                }
            }
            
            /**********************************************************************************
            * 如果 X 方向位移 突然过大 也不行
            ***********************************************************************************/
                        
            if(flag != 0)
            #endif
            {
                           
                /***************************************
                * 手指配对成功
                ****************************************/
                //R01 -a
               do{
                    if(j < bdt.FingerReqNum)
                    { 
                        //R01-e
                        Prev2CurID[Dim2idx[0]] = Dim2idx[1];  /* From Previous to Current*/
                        Cur2PrevID[Dim2idx[1]] = Dim2idx[0];

                        bdt.DPD[Dim2idx[0]].Finger_X_XMTR = Pos_X[Dim2idx[1]];
                        bdt.DPD[Dim2idx[0]].Finger_Y_RECV = Pos_Y[Dim2idx[1]];

                        if(MaxPos < Dim2idx[0]) 
                        {
                            MaxPos = Dim2idx[0];
                        }
                        //R01-a
                    }
                    else 
                        break;
                } while(0);
                //R01 -e
            }
            
            for (i = 0; i < bdt.PrevFingerDetectNum; i++) 
            {
                dist[i][Dim2idx[1]] = MAX_UINT16; /* Based on Previous Line*/
            }
            for (i = 0; i < bdt.FingerDetectNum; i++)    
            { 
                dist[Dim2idx[0]][i] = MAX_UINT16; /* Based on Current Line*/
            }
        }
    }
    
    /**********************************************************
    * Step 2B: Distance Algorithm      [0], CUR
    ***********************************************************/
    for (j = 0; j < bdt.PrevFingerDetectNum; j++)
        for (i = 0; i < bdt.FingerDetectNum; i++)
        {
            dist[j][i] = MAX_UINT16;
        }
        
    for (j = 0; j < bdt.PrevFingerDetectNum; j++)
    {   /* Distance Algorithm*/
        #ifdef RAWPOINT4FINGMATCH
        x = bdt.DPD[j].Finger_XRAW;
        y = bdt.DPD[j].Finger_YRAW;
        #else
        x = bdt.DPD[j].Prev_Finger_X;
        y = bdt.DPD[j].Prev_Finger_Y;
        #endif
        if(x[0] || y[0])   
        {    /* Finger Point*/
            if(FINGER_NUM_MAX == Prev2CurID[j]) /* Unmatched Point*/
            {
                for (i = 0; i < bdt.FingerDetectNum; i++)
                {
                    if(FINGER_NUM_MAX == Cur2PrevID[i]) /* Unmatched Point*/
                    {
                    dist[j][i] = FingProc_Dist2PMeasure(x[0], y[0], Pos_X[i], Pos_Y[i]);
                    }
                }
            }
        }    
    }
    
    for (j = 0; j < bdt.PrevFingerDetectNum; j++)
    {
        FingProc_FigureOutLeastFromMatrix(dist, Dim2idx);
        
        if(Dim2idx[0] < FINGER_NUM_MAX)
        {
            #if 1
            flag   = 0;
            #ifdef RAWPOINT4FINGMATCH
            x = bdt.DPD[Dim2idx[0]].Finger_XRAW;
            y = bdt.DPD[Dim2idx[0]].Finger_YRAW;
            #else
            x = bdt.DPD[Dim2idx[0]].Prev_Finger_X;
            y = bdt.DPD[Dim2idx[0]].Prev_Finger_Y;
            #endif
            
            if(x[1] || y[1])
            {
                /****************************************************
                * 如果 有 来回过速 的嫌疑，取消 手指匹配
                *****************************************************/
                if(FingProc_BackForthLineJudge(x[1],y[1],x[0],y[0],Pos_X[Dim2idx[1]],Pos_Y[Dim2idx[1]]))
                {
                    flag  = 1;
                }
                else if(dist[Dim2idx[0]][Dim2idx[1]] > FINGER_ADJUST_DISTANCE_MAX) 
                {
                    uint16_t lost;
                    
                    tdist2 = FingProc_Dist2PMeasure(x[0], y[0], x[1], y[1]);
                    if(x[2] || y[2])
                    {   /*lost变量在这里被临时借用(QFU）*/
                        lost = FingProc_Dist2PMeasure(x[1], y[1], x[2], y[2]);
                        if(tdist2 < lost) 
                        { 
                            tdist2 = lost;
                        }
                    }
                    
                    lost= 0;
                    
                    if((x[0]==x[1]) && (y[0]==y[1])) 
                    { 
                        lost = 1;
                    }
                    /****************************************************
                    * 如果直线距离 大于 原先距离5倍，则距离判断要谨慎
                    * 类似在 AvoidLongLine() 函数里的处理过程
                    *****************************************************/
                    #ifdef RAWPOINT4FINGMATCH
                    if(bdt.DPD[Dim2idx[0]].LifeNumber < 4) 
                    {
                        tdist2 = (tdist2<<2) + tdist2; /* 5倍*/
                        if(tdist2 < 384) 
                        {
                            tdist2 = 384;
                        }
                    }
                    else 
                    { 
                        tdist2 = (tdist2<<1) + tdist2; /* 3倍*/
                        if(tdist2 < 256)
                        {
                            tdist2 = 256;
                        }
                    }
                    #else
                    tdist2 = (tdist2<<2) + tdist2; /* 5倍*/
                    if(tdist2 < 256)
                    {
                        tdist2 = 256;
                    }
                    #endif
                    
                    if(lost != 0)
                    {
                        tdist2 = tdist2<<1;
                    }
                    if(dist[Dim2idx[0]][Dim2idx[1]] > tdist2)
                    {  
                        flag  = 2;
                    }
                    else
                    {
                        /*****************************************************
                        * 如果 X 方向位移 突然过大 也不行
                        *****************************************************/
                        tdist  = FingProc_Dist4Uint16Var(x[0], Pos_X[Dim2idx[1]]); /* 当前X方向距离*/
                        tdist2 = FingProc_Dist4Uint16Var(x[0], x[1]);
                        #ifdef RAWPOINT4FINGMATCH
                        tdist2 = tdist2<<2;
                        if(lost != 0)
                        {
                            tdist2 = tdist2<<1;
                        }
                        #else
                        if(x[2] || y[2]) 
                        {
                            tdist2 += FingProc_Dist4Uint16Var(x[2], x[1]);
                        }
                        tdist2 = tdist2<<3;
                        #endif
                        
                        if(bdt.DPD[i].LifeNumber > 6)
                        {
                            if((tdist > tdist2) && (tdist>256)) 
                            {
                                flag = 3;
                            }
                        }
                    }
                }
            }
            
            if(flag == 0)
            #endif
            {      //R01 -a
                   do{
                    if(j < bdt.FingerReqNum)
                    {
                        //R01 -e
                        Prev2CurID[Dim2idx[0]] = Dim2idx[1];  /* Whatever, we found one match point*/
                        Cur2PrevID[Dim2idx[1]] = Dim2idx[0];  /* Whatever, we found one match point*/
                        bdt.DPD[Dim2idx[0]].Finger_X_XMTR = Pos_X[Dim2idx[1]];
                        bdt.DPD[Dim2idx[0]].Finger_Y_RECV = Pos_Y[Dim2idx[1]];
                     
                        if(MaxPos < Dim2idx[0])
                        {
                            MaxPos = Dim2idx[0];
                        }  
                        //R01 -a
                    }
                    else
                        break;
                } while(0);
                //R01 -e
                if(MaxPos < Dim2idx[0])
                {
                    MaxPos = Dim2idx[0];
                }
            } 
            for (i = 0; i < bdt.PrevFingerDetectNum; i++)
            {
                dist[i][Dim2idx[1]] = MAX_UINT16; /* Based on Previous Line*/
            }
            for (i = 0; i < bdt.FingerDetectNum; i++)   
            {   
                dist[Dim2idx[0]][i] = MAX_UINT16; /* Based on Current Line*/
            }
        }
    }
    
    /*************************************************
    * Step 3: Give a index to the unmatched Point
    **************************************************/
    for (i = 0;i < bdt.FingerDetectNum;i++)
    {
        /*********************************************
        * 如果 flag = 1, 目前手指已经找到匹配
        * 其中配对的"i"是 "手指的ID"
        **********************************************/
        flag = 0;
        for (j = 0; j < bdt.PrevFingerDetectNum; j++)
        {
            if(i == Prev2CurID[j]) 
            {
                flag = 1;
            }
        }
        
        /*********************************************
        * 对未配对的 当前帧 手指 分配一个自由的 ID
        **********************************************/
        if(0 == flag)
        for (j = 0; j < FINGER_NUM_MAX; j++)
        {
            if(FINGER_NUM_MAX == Prev2CurID[j])
            #ifdef RAWPOINT4FINGMATCH
            if(!(bdt.DPD[j].Finger_XRAW[0]||bdt.DPD[j].Finger_YRAW[0]))
            #else
            if(!(bdt.DPD[j].Prev_Finger_X[0]||bdt.DPD[j].Prev_Finger_Y[0]))
            #endif
            if(0 == bdt.DPD[j].Point_Hold)
            {
                Prev2CurID[j] = i; /* j 为 手指 ID*/
                bdt.DPD[j].Finger_X_XMTR = Pos_X[i];
                bdt.DPD[j].Finger_Y_RECV = Pos_Y[i];
                if(MaxPos < j) MaxPos = j;
                break;
            }
        }
    }
    
    /****************************************************
    * Step 4: 得到当前帧的 最大ID,对应出 FingerDetectNum
    *****************************************************/
    if(bdt.FingerDetectNum != 0)
    {
        bdt.FingerDetectNum = MaxPos + 1;
    }
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void FingProc_SortFingers(void)
{
    FingProc_SortFingersWithMatrix();
    
    /*************************************************************
    * 下述code, 是为了检验手指点有效性 而设置的 (QFU)
    **************************************************************/
#if 0
    bdt.LFrame_NUM      = 0;
    for (i = 0; i < FINGER_NUM_MAX;i++)
    {
        bdt.LFrame_X_XMTR[i] = XMTR_NUM;
        bdt.LFrame_Y_RECV[i] = RECV_NUM;
        bdt.DPD[i].EdgeInfo.PrevEdgeNum = bdt.DPD[i].EdgeInfo.EdgeNum;
    }

    for (i = 0;i < FINGER_NUM_MAX;i++)
    {
        if(bdt.DPD[i].Finger_X_XMTR || bdt.DPD[i].Finger_Y_RECV)
        {
            bdt.LFrame_X_XMTR[bdt.LFrame_NUM] = (bdt.DPD[i].Finger_X_XMTR)>>8;
            bdt.LFrame_Y_RECV[bdt.LFrame_NUM] = (bdt.DPD[i].Finger_Y_RECV)>>8;
            bdt.LFrame_NUM++;

            bdt.DPD[i].EdgeInfo.EdgeNum = FINGER_SHOW;
            if(bdt.DPD[i].Finger_X_XMTR < MAX_MAP_VALUE))
            {   // Left Side
                bdt.DPD[i].EdgeInfo.EdgeNum = FINGER_EDGE_LEFT;
            }
            else if(bdt.DPD[i].Finger_X_XMTR > ((SXMTR_NUM<<8) - MAX_MAP_VALUE))
            {   // Right Side
                bdt.DPD[i].EdgeInfo.EdgeNum = FINGER_EDGE_RIGHT;
            }
            else if(bdt.DPD[i].Finger_Y_RECV < MAX_MAP_VALUE))
            {   // Top Side
                bdt.DPD[i].EdgeInfo.EdgeNum = FINGER_EDGE_TOP;
            }
            else if(bdt.DPD[i].Finger_Y_RECV > ((SRECV_NUM<<8) - MAX_MAP_VALUE))
            {   // Bottom Side
                bdt.DPD[i].EdgeInfo.EdgeNum = FINGER_EDGE_BOTTOM;
            }
        }
        else
            bdt.DPD[i].EdgeInfo.EdgeNum = NO_FINGER;
    }
#endif

}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void FingProc_PostSortFingers(void)
{
    uint16_t i;
    uint16_t j,n,m;
    uint16_t  MaxDeltaDat16A;
    MaxDeltaDat16A=0;

    bdt.FingerRealNum = 0;
    bdt.LFrame_NUM    = 0;
    for (i = 0; i < bdt.FingerReqNum;i++)
    {
        bdt.DPD[i].EdgeInfo.PrevEdgeNum = bdt.DPD[i].EdgeInfo.EdgeNum;
        if(bdt.DPD[i].Finger_X_XMTR || bdt.DPD[i].Finger_Y_RECV)
        {
            //***************************************************************
            // The code is used for Data Validation in data.c
            //***************************************************************
            bdt.LFrame_X_XMTR[bdt.LFrame_NUM] = (bdt.DPD[i].Finger_X_XMTR)>>8;
            bdt.LFrame_Y_RECV[bdt.LFrame_NUM] = (bdt.DPD[i].Finger_Y_RECV)>>8;
            bdt.LFrame_NUM++;
            bdt.FingerRealNum++;

            //***************************************************************
            // The code is used for record edge flag somehow
            //***************************************************************
            bdt.DPD[i].EdgeInfo.EdgeNum = FINGER_SHOW;
            if(bdt.DPD[i].Finger_X_XMTR < MAX_MAP_VALUE)
            {   // Left Side
                bdt.DPD[i].EdgeInfo.EdgeNum = FINGER_EDGE_LEFT;
            }
            else if(bdt.DPD[i].Finger_X_XMTR > ((SXMTR_NUM<<8) - MAX_MAP_VALUE))
            {   // Right Side
                bdt.DPD[i].EdgeInfo.EdgeNum = FINGER_EDGE_RIGHT;
            }
            else if(bdt.DPD[i].Finger_Y_RECV < MAX_MAP_VALUE)
            {   // Top Side
                bdt.DPD[i].EdgeInfo.EdgeNum = FINGER_EDGE_TOP;
            }
            else if(bdt.DPD[i].Finger_Y_RECV > ((SRECV_NUM<<8) - MAX_MAP_VALUE))
            {   // Bottom Side
                bdt.DPD[i].EdgeInfo.EdgeNum = FINGER_EDGE_BOTTOM;
            }
        }
        else
            bdt.DPD[i].EdgeInfo.EdgeNum = NO_FINGER;
    }


    //****************************************************************
    //  Redefined the Max Value requirement
    //****************************************************************
    if(bdt.FingerDetectNum != 0)
    {
        int16_t temp;
        for (j = 0; j < bdt.FingerDetectNum; j++)
        {
            n=bdt.DPD[j].Finger_X_XMTR>>8;
            m=bdt.DPD[j].Finger_Y_RECV>>8;
            temp = (bdt.DeltaDat16A[n][m]>>1)+(bdt.DeltaDat16A[n][m]>>3);
            if(temp > MaxDeltaDat16A)
            {
                MaxDeltaDat16A = temp;
            }
        }
        if(MaxDeltaDat16A>200) 
        {
            bdt.PCBA.MaxValueNoFinger=200;
        }
        else if(MaxDeltaDat16A< 120) 
        {  
            bdt.PCBA.MaxValueNoFinger= 120;
        }
        else 
        {
            bdt.PCBA.MaxValueNoFinger = MaxDeltaDat16A;
        }
    }
    else
    {
        bdt.PCBA.MaxValueNoFinger=200;
    }
    
    for(j = 0; j < bdt.FingerReqNum; j++)
    {
        if(bdt.DPD[j].Point_Hold > 0)
        {
            bdt.DPD[j].Point_Hold--;
        }
        #ifdef RAWPOINT4FINGMATCH
        bdt.DPD[j].Finger_XRAW[2] = bdt.DPD[j].Finger_XRAW[1];
        bdt.DPD[j].Finger_YRAW[2] = bdt.DPD[j].Finger_YRAW[1];
        bdt.DPD[j].Finger_XRAW[1] = bdt.DPD[j].Finger_XRAW[0];
        bdt.DPD[j].Finger_YRAW[1] = bdt.DPD[j].Finger_YRAW[0];
        bdt.DPD[j].Finger_XRAW[0] = bdt.DPD[j].Finger_X_XMTR;
        bdt.DPD[j].Finger_YRAW[0] = bdt.DPD[j].Finger_Y_RECV;
        #endif
    }
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void FingProc_AvoidLongLine(void)
{
    uint16_t i, j, flag_dist; 
    uint16_t curx, cury, x2, y2;
    uint16_t *x, *y;
    uint16_t dist0, dist1;
        
    for (i = 0; i < bdt.FingerReqNum; i++)
    {
        curx = bdt.DPD[i].Finger_X_XMTR;
        cury = bdt.DPD[i].Finger_Y_RECV;
        /*******************************************************
        * If it is not a FINGER POINT @ the index now
        ********************************************************/
        if((0 == curx) && (0 == cury)) 
        {
            continue;
        }
        x2 = bdt.DPD[i].Finger_X_Reported;
        y2 = bdt.DPD[i].Finger_Y_Reported;
        /*******************************************************
        * If it was not a FINGER POINT @ the index last time 
        ********************************************************/
        if((0 == x2) && (0 == y2) && (STATE_NONE_FINGER == bdt.DPD[i].FingerStateFlag))
        {
            continue;
        }
        #ifdef RAWPOINT4FINGMATCH
        x = bdt.DPD[i].Finger_XRAW;
        y = bdt.DPD[i].Finger_YRAW;
        #else
        x = bdt.DPD[i].Prev_Finger_X;
        y = bdt.DPD[i].Prev_Finger_Y;
        #endif
        
        /*******************************************************
        * Distance (Now_Raw_Position - Last_RAW_Position)
        * The distance of the match fingers is too long
        * Maybe it will bring ping-pong line up
        ********************************************************/
        flag_dist = 0;
        
        if(x[0] || y[0]) 
        {
            dist0 = FingProc_Dist2PMeasure(curx, cury, x[0], y[0]);
        }
        else
        { 
            dist0 = 0;
        }

        if(x[1] || y[1]) 
        {
            dist1 = FingProc_Dist2PMeasure(x[1], y[1], x[0], y[0]);
        }
        else
        {
            dist1 = 0;
        }
        
        dist1 = (dist1<<2) + dist1;
        if(dist1 < 512) 
        {
            dist1 = bdt.DPD[i].FingMovingSpeed;
        }
        
        if(dist0 > dist1) 
        { 
            flag_dist = 1;
        }
        
        #if 0
        if(bdt.DPD[i].FingerStateFlag>=STATE_SERIAL_FINGER)
                if(dist1 < bdt.DPD[i].AdjustDistance)
        if(dist0 > 256) flag_dist = 1;
        #endif
        /*******************************************************
        * Threshold Adaptive based on the moving speed
        ********************************************************/
        if(dist0 < FINGER_MOVING_SPEED_THRD_LOW)                      /* DIST = 0 - 200*/
        {
            bdt.DPD[i].FingMovingSpeed = FINGER_FINGER_SPEED_LOW;     /* Slow Speed*/
        }
        else if(dist0 < FINGER_MOVING_SPEED_THRD_MIDDLE)             /* DIST = 200 - 400*/
        {   
            bdt.DPD[i].FingMovingSpeed = FINGER_FINGER_SPEED_MIDDLE;  /* Middle Speed*/
        }
        else                                                         /* DIST = 200 - 400*/
        {
            bdt.DPD[i].FingMovingSpeed = FINGER_FINGER_SPEED_FAST;    /* Fast Speed*/
        }
        
        /**********************************************************************
        * The FINGER Point is moved too long @ the index
        * Then, it is moved to a starting point of another line
        ***********************************************************************/
        if(flag_dist != 0)
        {
            bdt.DPD[i].Finger_X_XMTR = 0;
            bdt.DPD[i].Finger_Y_RECV = 0;
            for (j = 0; j < bdt.FingerReqNum; j++)
            {
                if(0 == bdt.DPD[j].Finger_X_Reported)
                if(0 == bdt.DPD[j].Finger_Y_Reported)
                if(0 == bdt.DPD[j].Finger_X_XMTR)
                if(0 == bdt.DPD[j].Finger_Y_RECV)
                if(bdt.DPD[j].FingerStateFlag == STATE_NONE_FINGER)
                {
                    bdt.DPD[j].Finger_X_XMTR = curx;
                    bdt.DPD[j].Finger_Y_RECV = cury;
                    if((j + 1) > bdt.FingerDetectNum) 
                    {
                        bdt.FingerDetectNum = j + 1;
                    }
                    break;
                }
            }
        }
    }
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint16_t FingProc_IEdgeP_Mapping(uint16_t h, uint16_t AsCloseAsPossible)
{
    uint16_t result = h;
#ifdef BORDER_SIMPLE_ADJUSTABLE
    //************************************************************************
    // Smaller "AsCloseAsPossible" is , closer Finger move to the side
    //************************************************************************
    if(h > AsCloseAsPossible)
    {
        h = h - AsCloseAsPossible;
        result += (h>>1);
        if(h >= 16)
        {
            do 
            {
                h -= 16;
                result += h;
            } while(h >= 16);
            result += h;
        }
    }
    if(result > 240) result = 240;
#endif
    return result;
}
/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint16_t FingProc_IEdgeP_Left(uint16_t h)
{
    uint16_t result;
    #ifdef SHOW_H_DATA
    bdt.Left_h=h;
    #endif
    
#ifdef BORDER_SIMPLE_ADJUSTABLE
  #if 0
    h = h<<1;
    result = (((h>>5)+1)<<5)-16;
    if(result > 230) result = 230;
  #else
    #define ASCLOSEASPOSSIBLE_LEFT 64
    result = FingProc_IEdgeP_Mapping(h, ASCLOSEASPOSSIBLE_LEFT);
  #endif
#else
    #if ( defined (CONFIG_ARCH_SUN8IW3P1) || defined (CONFIG_ARCH_SUN8IW5P1) )// A23 and A33                    
    if(h < 16)    
    {
        result = h + (h>>2);
    }                           /* 1.25*/
    else if(h<32)
    {
        h -= 16;
        result = 20  + h + (h>>1);
    }                           /* 1.50*/
    else if(h<48)
    {
        h -= 32; 
        result = 44  + (h<<1);
    }                           /* 2.00*/
    else if(h<64)
    {
        h -= 48; 
        result = 76  + (h<<1) + (h>>1) + (h>>2);
    }                           /* 2.75*/
    else if(h<80)
    {
        h -= 64; 
        result = 120  + (h<<1) + h +(h>>1);
    }                           /* 3.50 */
    else         
    {
        h -= 80;
        result = 176  + (h<<2) + h;
    }                          /* 5.00*/
    
    if(result >= (MAX_MAP_VALUE-25)) 
    {
        result = MAX_MAP_VALUE - 25;/*使边界值保持在合理范围内*/
    }
    #else 
    
    #ifdef CN1100_MTK     
    
    if(h < 16)    
    {
        result = h + (h>>3);
    }                           /*  1.125 */
    else if(h<32) 
    {
        h -= 16; 
        result = 18  + h + (h>>2);
    }                           /*  1.25 */
    else if(h<48) 
    {
        h -= 32;
        result = 38  + h + (h>>1);
    }                           /*  1.50 */
    else if(h<64)
    {
        h -= 48; 
        result = 62  + h + (h>>1) + (h>>2);
    }                           /*  1.75 */
    else if(h<80) 
    {
        h -= 64;
        result = 90  + (h<<1);
    }                           /*  2.00 */
    else          
    {
        h -= 80; 
        result = 122  + (h<<1) + (h>>1);
    }                           /*  2.50 */
    
    if(result >= (MAX_MAP_VALUE-60))
    {
        result = MAX_MAP_VALUE - 60;/* 使边界值保持在合理范围内 */
    }
    #else
    #ifdef SH_FEATUREPHONE_13X9
    if(h < 16)   
    {
        result = h+(h>>1);
    }                      /*1.5 */
    else if(h<32) 
    {
        h -= 16; 
        result = 24 + (h<<1);
    }                      /*  2 */
    else if(h<48) 
    {
        h -= 32; 
        result = 56  + (h<<1)+(h>>1);
    }                      /* 2.5 */
    else if(h<64)
    {
        h -= 48;
        result = 96  + (h<<1) + h;
    }                      /*  3 */
    else if(h<80)
    {
        h -= 64;
        result = 144  + (h<<2) + h+(h>>1);
    }                      /*  3.5 */
    else         
    {
        h -= 80; 
        result = 200  + (h<<2);
    }                     /*  4 */
    if(result >= (MAX_MAP_VALUE-5)) 
    {
        result = MAX_MAP_VALUE - 5;/*使边界值保持在合理范围内   */
    }
    #else
    if(h < 16)   
    {
        result = h + (h>>2);
    }                  /*  1.25 */
    else if(h<32) 
    {
        h -= 16; 
        result = 20  + h + (h>>1) + (h>>2);
    }                  /*  1.75 */
    else if(h<48) 
    {
        h -= 32; 
        result = 48  + (h<<1) + (h>>2);
    }                  /*  2.25 */
    else if(h<64)
    {
        h -= 48;
        result = 84  + (h<<1) + h;
    }                  /*  3.00 */
    else if(h<80)
    {
        h -= 64;
        result = 132  + (h<<1) + h + (h>>1) + (h>>2);
    }                  /*  3.75 */
    else         
    {
        h -= 80; 
        result = 192  + (h<<2) + (h>>1);
    }                  /*  4.50 */
    //else          {h -= 96; result = 264  + (h<<2) + (h>>1);}                   /*  5.25 */
    //else          {h -= 112; result = 270 + (h<<2)  + (h>>1);}                  /*  4.50 */
    if(result >= (MAX_MAP_VALUE-25)) 
    {
        result = MAX_MAP_VALUE - 25;/*使边界值保持在合理范围内   */
    }
    #endif
    #endif
    #endif
#endif   // BORDER_SIMPLE_ADJUSTABLE

    return result;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint16_t FingProc_IEdgeP_Right(uint16_t h)
{
    uint16_t result;
    #ifdef SHOW_H_DATA
    bdt.Right_h=h;
    #endif

#ifdef BORDER_SIMPLE_ADJUSTABLE
  #if 0
    h = h<<1;
    result = (((h>>5)+1)<<5)-16;
    if(result > 230) result = 230;
  #else
    #define ASCLOSEASPOSSIBLE_RIGHT 64
    result = FingProc_IEdgeP_Mapping(h, ASCLOSEASPOSSIBLE_RIGHT);
  #endif
#else
    #if ( defined (CONFIG_ARCH_SUN8IW3P1) || defined (CONFIG_ARCH_SUN8IW5P1) )// A23 and A33                    
    if(h < 16)
    {  
        result = h + (h>>2);
    }                           /*  1.25 */
    else if(h<32) 
    {  
        h -= 16;
        result = 20  + h + (h>>1);
    }                           /*  1.50 */
    else if(h<48)
    {   
        h -= 32;
        result = 48  + h + (h>>1) + (h>>2);
    }                           /*  1.75 */
    else if(h<64) 
    {   
        h -= 48; 
        result = 80  + (h<<1);
    }                           /*  2.00 */
    else if(h<80)
    { 
        h -= 64;
        result = 112  + (h<<1) + (h>>2);
    }                           /*  2.25 */
    else if(h<96)
    {
        h -= 80;
        result = 148  + (h<<1) +(h>>1) + (h>>2);
    }                           /*  2.75 */
    else       
    {
        h -= 96;
        result = 192  + h + (h<<1) + (h>>1) + (h>>2);
    }                           /*  3.75     h=110-> H=225.5 */
    
    if(result >= (MAX_MAP_VALUE-25)) 
    {
        result = MAX_MAP_VALUE - 25;/* 使边界值保持在合理范围内 */
    }
    
    #else
    
    #ifdef CN1100_MTK                    
    if(h < 16)   
    {
        result = h + (h>>3);
    }                           /*  1.125 */
    else if(h<32) 
    {
        h -= 16;
        result = 18  + h + (h>>2);
    }                           /*  1.25 */
    else if(h<48)
    { 
        h -= 32;
        result = 38  + h + (h>>1);
    }                           /*  1.50 */
    else if(h<64)
    {
        h -= 48; 
        result = 62  + h + (h>>1) + (h>>2);
    }                           /*  1.75 */
    else if(h<80)
    { 
        h -= 64;
        result = 90  + (h<<1);
    }                           /*  2.00 */
    else         
    {
        h -= 80;
        result = 122  + (h<<1) + (h>>1);
    }                           /*  2.50 */
    
    
    if(result >= (MAX_MAP_VALUE-60)) 
    {
        result = MAX_MAP_VALUE - 60;/* 使边界值保持在合理范围内 */
    }
    
    #else 
    #ifdef SH_FEATUREPHONE_13X9
    if(h < 16)   
    {
        result = h+(h>>1);
    }                 /*1.5 */
    else if(h<32) 
    {
        h -= 16; 
        result = 24 + (h<<1);
    }                  /*  2 */
    else if(h<48) 
    {
        h -= 32; 
        result = 56  + (h<<1)+(h>>1);
    }                  /* 2.5 */
    else if(h<64)
    {
        h -= 48;
        result = 96  + (h<<1) + h;
    }                  /*  3 */
    else if(h<80)
    {
        h -= 64;
        result = 144  + (h<<1) + h+(h>>1);
    }                  /*  3.5 */
    else         
    {
        h -= 80; 
        result = 200  + (h<<2);
    }                  /*  4 */
    if(result >= (MAX_MAP_VALUE-5)) 
    {
        result = MAX_MAP_VALUE - 5;/*使边界值保持在合理范围内   */
    }
    #else
    if(h < 16)  
    {
        result = h + (h>>3);
    }                   /*  1.125 */
    else if(h<32) 
    {
        h -= 16; 
        result = 18  + h + (h>>2)+ (h>>3);
    }                   /*  1.375 */
    else if(h<48)
    {
        h -= 32;
        result = 40  + h + (h>>1) + (h>>2);
    }                  /*  1.75 */
    else if(h<64) 
    {
        h -= 48; 
        result = 68  + (h<<1);
    }                  /*  2.00 */
    else if(h<80)
    {
        h -= 64; 
        result = 100  + (h<<1) + (h>>2);
    }                  /*  2.25 */
    else if(h<96)
    {
        h -= 80;
        result = 136  + (h<<1) + (h>>1);
    }                  /*  2.50 */
    else if(h<112)
    {
        h -= 96;
        result = 176  + h + (h<<1);
    }                  /*  3.00 */
    else        
    {
        h -= 112; 
        result = 224 + (h<<2) + h + (h>>1);
    }                  /*  5.50 */
    if(result >= (MAX_MAP_VALUE-25)) 
    {
        result = MAX_MAP_VALUE - 25;/* 使边界值保持在合理范围内   */
    }
    #endif
    #endif
    #endif
#endif
    
    return result;
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint16_t FingProc_IEdgeP_Top(uint16_t h)
{
    uint16_t result;
    #ifdef SHOW_H_DATA
    bdt.Top_h=h;  
    #endif

#ifdef BORDER_SIMPLE_ADJUSTABLE
  #if 0
    h = h<<1;
    result = (((h>>5)+1)<<5)-16;
    if(result > 230) result = 230;
  #else
    #define ASCLOSEASPOSSIBLE_TOP 64
    result = FingProc_IEdgeP_Mapping(h, ASCLOSEASPOSSIBLE_TOP);
  #endif
#else
    #if ( defined (CONFIG_ARCH_SUN8IW3P1) || defined (CONFIG_ARCH_SUN8IW5P1) )// A23 and A33                    
    if(h < 16)   
    {
        result = h + (h>>2);
    }                      /*  1.25 */
    else if(h<32)
    {
        h -= 16; 
        result = 20  + h + (h>>1);
    }                      /*  1.50 */
    else if(h<48)
    {
        h -= 32; 
        result = 44  + (h<<1);
    }                      /*  2.00 */
    else if(h<64)
    {
        h -= 48; 
        result = 76  + (h<<1) + (h>>1) + (h>>2);
    }                      /*  2.75 */
    else if(h<80)
    {
        h -= 64; 
        result = 120  + (h<<1) + h +(h>>1);
    }                      /*  3.50 */
    else         
    {
        h -= 80; 
        result = 176  + (h<<2) + h;
    }                      /*  5.00     */
    
    if(result >= (MAX_MAP_VALUE-25))
    { 
        result = MAX_MAP_VALUE - 25;/*  使边界值保持在合理范围内 */
    }
    
    #else
    #ifdef CN1100_MTK                   
    if(h < 16)   
    {
        result = h + (h>>3);
    }                      /*  1.125 */
    else if(h<32)
    {
        h -= 16; 
        result = 18  + h + (h>>2);
    }                      /*  1.25 */
    else if(h<48)
    {
        h -= 32; 
        result = 38  + h + (h>>1);
    }                      /*  1.50 */
    else if(h<64)
    {
        h -= 48; 
        result = 62  + h + (h>>1) + (h>>2);
    }                      /*  1.75 */
    else if(h<80)
    {
        h -= 64; 
        result = 90  + (h<<1);
    }                      /*  2.00 */
    else         
    {
        h -= 80;
        result = 122  + (h<<1) + (h>>1);
    }                      /*  2.50 */
    
    
    if(result >= (MAX_MAP_VALUE-60))
    {
        result = MAX_MAP_VALUE - 60;/*使边界值保持在合理范围内 */
    }
    #else  
    #ifdef SH_FEATUREPHONE_13X9
    if(h < 16)   
    {
        result = h+(h>>1);
    }                 /*1.5 */
    else if(h<32) 
    {
        h -= 16; 
        result = 24 + (h<<1);
    }                  /*  2 */
    else if(h<48) 
    {
        h -= 32; 
        result = 56  + (h<<1)+(h>>1);
    }                  /* 2.5 */
    else if(h<64)
    {
        h -= 48;
        result = 96  + (h<<1) + h;
    }                  /*  3 */
    else if(h<80)
    {
        h -= 64;
        result = 144  + (h<<1) + h+(h>>1);
    }                  /*  3.5 */
    else         
    {
        h -= 80; 
        result = 200  + (h<<2);
    }                  /*  4 */
    if(result >= (MAX_MAP_VALUE-25)) 
    {
        result = MAX_MAP_VALUE - 25;/*使边界值保持在合理范围内   */
    }
//  result = (h<<1)+(h>>1);
//  if(result >= (MAX_MAP_VALUE-25)) 
//  {
//      result = MAX_MAP_VALUE - 25;/*使边界值保持在合理范围内   */
//  }
    #else
    if(h < 16)   
    {
        result = h + (h>>2);
    }                   /*  1.25 */
    else if(h<32)
    {
        h -= 16; 
        result = 20  + h + (h>>1);
    }                   /*  1.5 */
    else if(h<48)
    {
        h -= 32;
        result = 44  + h + (h>>1) + (h>>2);
    }                   /*  1.75 */
    else if(h<64)
    {
        h -= 48;
        result = 72  + (h<<1) + (h>>1);
    }                   /*  2.5 */
    else if(h<80)
    {
        h -= 64; 
        result = 112  + (h<<1) + h;
    }                   /*  3.00 */
    else if(h<96)
    {
        h -= 80; 
        result = 160  + (h<<1) + h + (h>>1);
    }                   /*  3.50 */
    else         
    {
        h -= 96;
        result = 216  + (h<<2) + (h>>1);
    }                   /*  4.50 */
    if(result >= (MAX_MAP_VALUE-25))
    {
        result = MAX_MAP_VALUE - 25;/* 使边界值保持在合理范围内 */
    }
    #endif
    #endif
    #endif
#endif
    
    return result;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint16_t FingProc_IEdgeP_Bottom(uint16_t h)
{
    uint16_t result;
    #ifdef SHOW_H_DATA
    bdt.Bottom_h=h;
    #endif

#ifdef BORDER_SIMPLE_ADJUSTABLE
  #if 0
    h = h<<1;
    result = (((h>>5)+1)<<5)-16;
    if(result > 230) result = 230;
  #else
    #define ASCLOSEASPOSSIBLE_BOTTOM 64
    result = FingProc_IEdgeP_Mapping(h, ASCLOSEASPOSSIBLE_BOTTOM);
  #endif
#else
    #if ( defined (CONFIG_ARCH_SUN8IW3P1) || defined (CONFIG_ARCH_SUN8IW5P1) )// A23 and A33                     
    if(h < 16)   
    {
        result = h + (h>>2);
    }                  /*  1.25 */
    else if(h<32) 
    {
        h -= 16;
        result = 20  + h + (h>>1);
    }                  /*  1.50 */
    else if(h<48)
    {
        h -= 32; 
        result = 48  + h + (h>>1) + (h>>2);
    }                  /*  1.75 */
    else if(h<64) 
    {
        h -= 48;
        result = 80  + (h<<1);
    }                  /*  2.00 */
    else if(h<80)
    {
        h -= 64;
        result = 112  + (h<<1) + (h>>1);
    }                  /*  2.50 */
    else if(h<96)
    {
        h -= 80;
        result = 152  + (h<<1) + h;
    }                  /*  3.00 */
    else         
    {
        h -= 96;
        result = 200  + h + (h<<1) + (h>>1) + (h>>2);
    }                  /*  3.75     h=105-> H=225.75 */
    
    if(result >= (MAX_MAP_VALUE-25))
    {
        result = MAX_MAP_VALUE - 25; /* 使边界值保持在合理范围内 */
    }
    
    #else
    #ifdef CN1100_MTK                    
    if(h < 16)   
    {
        result = h + (h>>3);
    }                  /*  1.125 */
    else if(h<32)
    {
         h -= 16; 
         result = 18  + h + (h>>2);
    }                  /*  1.25 */
    else if(h<48) 
    {
         h -= 32; 
         result = 38  + h + (h>>1);
    }                  /*  1.50 */
    else if(h<64) 
    {
         h -= 48; 
         result = 62  + h + (h>>1) + (h>>2);
    }                  /*  1.75 */
    else if(h<80)
    {     
        h -= 64;
        result = 90  + (h<<1);
    }                  /*  2.00 */
    else         
    {
        h -= 80;
        result = 122  + (h<<1) + (h>>2);
    }                  /*  2.25 */
    
    
    if(result >= (MAX_MAP_VALUE-60))
    {
        result = MAX_MAP_VALUE - 60;/* 使边界值保持在合理范围内 */
    }
    
    #else
    #ifdef SH_FEATUREPHONE_13X9
    if(h < 16)   
    {
        result = h+(h>>1);
    }                 /*1.5 */
    else if(h<32) 
    {
        h -= 16; 
        result = 24 + (h<<1);
    }                  /*  2 */
    else if(h<48) 
    {
        h -= 32; 
        result = 56  + (h<<1)+(h>>1);
    }                  /* 2.5 */
    else if(h<64)
    {
        h -= 48;
        result = 96  + (h<<1) + h;
    }                  /*  3 */
    else if(h<80)
    {
        h -= 64;
        result = 144  + (h<<1) + h+(h>>1);
    }                  /*  3.5 */
    else         
    {
        h -= 80; 
        result = 200  + (h<<2);
    }                  /*  4 */
    if(result >= (MAX_MAP_VALUE-5)) 
    {
        result = MAX_MAP_VALUE - 5;/*使边界值保持在合理范围内   */
    }
    #else
    if(h < 16)  
    {
        result = h + (h>>3);
    }                 /*  1.125 */
    else if(h<32)
    {
        h -= 16; 
        result = 18  + h + (h>>2);
    }                 /*  1.25 */
    else if(h<48)
    {
        h -= 32; 
        result = 38  + h + (h>>1);
    }                 /*  1.5 */
    else if(h<64)
    {
        h -= 48; 
        result = 62  + (h<<1) + (h>>1);
    }                 /*  2.5 */
    else if(h<80)
    {
        h -= 64; 
        result = 102  + (h<<1) + h;
    }                 /*  3.00   */
    else if(h<96)
    {
        h -= 80; 
        result = 150  + (h<<1) + h + (h>>1);
    }                 /*  3.50 */
    else         
    {
        h -= 96;
        result = 206  + (h<<2) + (h>>1);
    }                 /*  4.50 */
    if(result >= (MAX_MAP_VALUE-15))
    { 
        result = MAX_MAP_VALUE - 15; /* 使边界值保持在合理范围内 距离 */
    }
    #endif
    #endif
    #endif
#endif
    
    return result;
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void FingProc_ImproveEdgePoint(void)
{
    uint16_t index, h;
    
    for (index = 0; index < bdt.FingerDetectNum; index++)
    {
        /************************************************
        * Process XMTR direction
        ************************************************/
        if((bdt.DPD[index].Finger_X_XMTR < MAX_MAP_VALUE) && bdt.DPD[index].Finger_X_XMTR)
        {   // Left Side
            h = MAX_MAP_VALUE - bdt.DPD[index].Finger_X_XMTR;
            #ifdef SHOW_H_DATA
            bdt.Left_h=h;
            #endif
            bdt.DPD[index].Finger_X_XMTR = MAX_MAP_VALUE - FingProc_IEdgeP_Left(h);
        }
        else if(bdt.DPD[index].Finger_X_XMTR > ((SXMTR_NUM<<8) - MAX_MAP_VALUE))
        {   // Right Side
            h = bdt.DPD[index].Finger_X_XMTR - ((SXMTR_NUM<<8) - MAX_MAP_VALUE);
            #ifdef SHOW_H_DATA
            bdt.Right_h=h;
            #endif
            bdt.DPD[index].Finger_X_XMTR = ((SXMTR_NUM<<8) - MAX_MAP_VALUE) + FingProc_IEdgeP_Right(h);
        }
        
        /******************************************************************************
        * Process RECV direction
        *******************************************************************************/
        if((bdt.DPD[index].Finger_Y_RECV < MAX_MAP_VALUE) && bdt.DPD[index].Finger_Y_RECV)
        {   // Top Side
            h = MAX_MAP_VALUE - bdt.DPD[index].Finger_Y_RECV;
            #ifdef SHOW_H_DATA
            bdt.Top_h=h;
            #endif
            bdt.DPD[index].Finger_Y_RECV = MAX_MAP_VALUE - FingProc_IEdgeP_Top(h);
        }
        else if(bdt.DPD[index].Finger_Y_RECV > ((SRECV_NUM<<8) - MAX_MAP_VALUE))
        {   // Bottom Side
            h = bdt.DPD[index].Finger_Y_RECV - ((SRECV_NUM<<8) - MAX_MAP_VALUE);
            #ifdef SHOW_H_DATA
            bdt.Bottom_h=h;
            #endif
            bdt.DPD[index].Finger_Y_RECV = ((SRECV_NUM<<8) - MAX_MAP_VALUE) + FingProc_IEdgeP_Bottom(h);
        }
    }
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void FingProc_MergeClosingPoints(void)
{
    uint16_t i, j, k;
    uint16_t x1, y1, x2, y2;
    uint16_t distance, distThr;
    uint16_t prevNum;
    uint16_t flag = 1;
    
    /********************************************************************
    *alculating total finger number in the last Frame (Previous Frame)
    *********************************************************************/
    prevNum = 0;
    for (k = 0; k < bdt.PrevFingerDetectNum; k++)
    {
        if(bdt.DPD[k].Prev_Finger_X[0] || bdt.DPD[k].Prev_Finger_Y[0]) 
        {
            prevNum++;
        }
    }
    
    /********************************************************************
    * The Processing for the mergement is step by step
    *********************************************************************/
    while(flag)
    {
        flag = 0;
        
        /****************************************************************
        * Threshold used in Judgement for finger is set. 
        * If more finger this time, threshold will be longer
        *****************************************************************/
        distThr = FINGER_UNITE_THRESHOLD;  /* = 192, about 6mm*/
        if(bdt.FingerDetectNum > prevNum)
        {
            distThr = FINGER_SEPATATE_THRESHOLD; /* 380-12mm*/
        }
        if(bdt.FingerDetectNum > 1)
        for (i = 0; i < bdt.FingerDetectNum;i++)
        for (j = i+1;j < bdt.FingerDetectNum;j++)
        {
            x1 = bdt.DPD[i].Finger_X_XMTR;
            y1 = bdt.DPD[i].Finger_Y_RECV;
            x2 = bdt.DPD[j].Finger_X_XMTR;
            y2 = bdt.DPD[j].Finger_Y_RECV;
            distance = FingProc_Dist2PMeasure(x1, y1, x2, y2);
           /****************************************************************
            * If DIST is too short, merge them; this will avoid some NOISE
            *****************************************************************/
            if(distance <= distThr)
            {
                flag = 1;
                bdt.FingerDetectNum--;
                
               /************************************************************
                * Reset the threshold based on the new condition
                *************************************************************/
                distThr = FINGER_UNITE_THRESHOLD;
                if(bdt.FingerDetectNum > prevNum) 
                {
                    distThr = FINGER_SEPATATE_THRESHOLD;
                }
                
               /************************************************************
                * Merge two points into one point
                ************************************************************/
                bdt.DPD[i].Finger_X_XMTR = (x1+x2)>>1;
                bdt.DPD[i].Finger_Y_RECV = (y1+y2)>>1;
                
               /************************************************************
                * Move the rest point to new position
                *************************************************************/
                for (k = j; k < bdt.FingerDetectNum; k++)
                {
                    bdt.DPD[k].Finger_X_XMTR = bdt.DPD[k + 1].Finger_X_XMTR;
                    bdt.DPD[k].Finger_Y_RECV = bdt.DPD[k + 1].Finger_Y_RECV;
                }
            }
        }
    }
    
    /********************************************************************
    * Set the remaining Finger Position as empty space
    *********************************************************************/
    for (i = bdt.FingerDetectNum; i < FINGER_NUM_MAX;i++)
    {
        bdt.DPD[i].Finger_X_XMTR = 0;
        bdt.DPD[i].Finger_Y_RECV = 0;
    }
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void FingProc_FingerInfoReorder(void)
{
    uint16_t i, FingNum;
 
    /* valid finger number */
    FingNum = 0;
    for (i = 0; i < bdt.FingerDetectNum;i++)
    {
        if(bdt.DPD[i].Finger_X_XMTR || bdt.DPD[i].Finger_Y_RECV)
        {
            bdt.DPD[FingNum].Finger_X_XMTR = bdt.DPD[i].Finger_X_XMTR;
            bdt.DPD[FingNum].Finger_Y_RECV = bdt.DPD[i].Finger_Y_RECV;
            FingNum++;
        }
    }
    bdt.FingerDetectNum = FingNum;
    for(i = FingNum; i < FINGER_NUM_MAX;i++)
    {
        bdt.DPD[i].Finger_X_XMTR = 0;
        bdt.DPD[i].Finger_Y_RECV = 0;
    } 
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_HandleFingerInfo(void)
{
    FingProc_FingerInfoReorder();
    FingProc_MergeClosingPoints();  
    FingProc_ImproveEdgePoint();  /* optimize edge finger XY value */
    FingProc_SortFingers();
    FingProc_CheckCrossedLines(); /* Useless now, empty now*/
    FingProc_AvoidLongLine();
    
    FingProc_PostSortFingers();

    FingProc_SuperFilter4Edge();
    FingProc_ImproveByMultiFilters();
    bdt.PrevFingerDetectNum2 = bdt.PrevFingerDetectNum;
    bdt.PrevFingerDetectNum  = bdt.FingerDetectNum;
}

#ifdef SCREEN_FULL_ADAPTIVE
/*******************************************************************************
* Function Name  : DataProc_CircleScreenAdaptive
* Description    : 穷举Ref high和Ref low参数的各个值
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint32_t DataProc_CircleScreenAdaptive(void)
{
    for(; bdt.PCBA.HighRefGainSet<4;)
    {
        if((bdt.PCBA.HighRefGainSet == 3)&&(bdt.PCBA.LowRefGainSet == 3))    // 标记穷举结束
        {
            bdt.PCBA.HighRefGainSet = 0;                        // 恢复到初始化值
            bdt.PCBA.LowRefGainSet = 0;
            bdt.PCBA.RefHLSetCount = CIRCLE_MAXCOUNT+1;            // 置为无效的数据
            return 2;                                            // 穷举结束
        }
        else
        {
            for(;bdt.PCBA.LowRefGainSet<4;)            
            {
                if(bdt.PCBA.LowRefGainSet == 3)
                {
                    bdt.PCBA.HighRefGainSet++;        // 一轮内层循环结束，外层计数加一
                    bdt.PCBA.LowRefGainSet = -1;    // 保证下一次计数LowRefGainSet从0开始
                    break;
                }
                else
                {
                    bdt.PCBA.LowRefGainSet++;
                    bdt.PCBA.RefHLSetCount++;        // 计数加一
                    return 1;                        // 一次结束
                }
            }
        }
    }
    return 2;
}
/*******************************************************************************
* Function Name  : DataProc_CalRefHLSetAverageValue
* Description    : 每进行一次参考电压设置，就计算出整屏的平均值，放在bdt.PCBA.RefHLSetAve[]中
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_CalRefHLSetAverageValue(uint16_t *buffer)
{
    uint16_t i, j;
    uint32_t Allsum = 0;
    for(i=0; i<SXMTR_NUM; i++)
    {
        for(j=0; j<SRECV_NUM; j++)
            Allsum += (int16_t)(buffer[i*RECV_NUM+j]);
    }
    bdt.PCBA.RefHLSetAve[bdt.PCBA.RefHLSetCount] = Allsum/(SXMTR_NUM*SRECV_NUM);
}

/*******************************************************************************
* Function Name  : DataProc_SetRefHLEnd
* Description    : 穷举之后，根据每次的平均值与REASONABLE_VALUE的差值最小求出合理的参考电压参数
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_SetRefHLEnd(void)
{
    uint16_t i,index=0;
    uint16_t min = 4095;
    uint16_t temp = 0;    
    for(i=0; i<CIRCLE_MAXCOUNT; i++)
    {
        temp = abs16(bdt.PCBA.RefHLSetAve[i] - REASONABLE_VALUE);
        if(min > temp)
        {
            min = temp;
            index = i;
        }
    }
    bdt.PCBA.HighRefGainSet = index/4;            // 获取最合理的ref high和ref low的值
    bdt.PCBA.LowRefGainSet = index%4;
    bdt.PCBA.RefHLSetEndFlag = 1;                 // 标记获取最合理的值OK
}
#endif

#ifdef SCREEN_SIMPLE_ADAPTIVE
/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_Screen_Adaptive(uint16_t max,uint16_t min)
{
    #ifdef SH_FEATUREPHONE_13X9
    if(max>4000)
    {
        if(min>2357)
        {
            bdt.ScreenAdaptiveFlag  = 1; 
            bdt.PCBA.HighRefSet     = HIGH_REF_2PF;
            bdt.PCBA.HighRefGainSet = REF_GAIN_12PF;
            bdt.PCBA.LowRefSet      = LOW_REF_1PF; 
            bdt.PCBA.LowRefGainSet  = REF_GAIN_12PF;
        }
    }
    else if((max<4000)&&(max>=2128))
    {
        if(min>=1690)
        {
            bdt.ScreenAdaptiveFlag  = 1; 
            bdt.PCBA.LowRefSet      = LOW_REF_1PF; 
            bdt.PCBA.LowRefGainSet  = REF_GAIN_12PF;
        }
    }
    else if((max<2128)&&(max>=1300))
    {
        if(min>=0)
        {
            bdt.ScreenAdaptiveFlag  = 1; 
            bdt.PCBA.HighRefSet     = HIGH_REF_1PF;
            bdt.PCBA.HighRefGainSet = REF_GAIN_12PF;
            bdt.PCBA.LowRefSet      = LOW_REF_0PF; 
            bdt.PCBA.LowRefGainSet  = REF_GAIN_16PF;
        }
        else if(min>=333)
        {
            bdt.ScreenAdaptiveFlag  = 1; 
            bdt.PCBA.HighRefSet     = HIGH_REF_1PF;
            bdt.PCBA.HighRefGainSet = REF_GAIN_12PF;
            bdt.PCBA.LowRefSet      = LOW_REF_1PF; 
            bdt.PCBA.LowRefGainSet  = REF_GAIN_16PF;
        }
    }
    else if(max<1300)
    {
        bdt.ScreenAdaptiveFlag  = 1; 
        bdt.PCBA.HighRefSet     = HIGH_REF_1PF;
        bdt.PCBA.HighRefGainSet = REF_GAIN_16PF;
        bdt.PCBA.LowRefSet      = LOW_REF_0PF; 
        bdt.PCBA.LowRefGainSet  = REF_GAIN_16PF;
    }
    if(bdt.ScreenAdaptiveFlag!=0)
    {
        bdt.BFD.InitCount++;
    }
    #else
    if(max>=4000)
    {
        if(min>2535)
        {
            bdt.ScreenAdaptiveFlag  = 1; 
            bdt.PCBA.HighRefSet     = HIGH_REF_4PF;
            bdt.PCBA.HighRefGainSet = REF_GAIN_12PF;
            bdt.PCBA.LowRefSet      = LOW_REF_2PF; 
            bdt.PCBA.LowRefGainSet  = REF_GAIN_12PF;
        }
    }
    else if((max<4000)&&(max>=1950))
         {
             if(min>1517)
             {
                 bdt.ScreenAdaptiveFlag      = 1;
                 bdt.PCBA.LowRefSet            = LOW_REF_2PF; 
                 bdt.PCBA.LowRefGainSet        = REF_GAIN_16PF;
             }  
         }
    else if((max<1950)&&(max>=975))
         {
             bdt.ScreenAdaptiveFlag  = 1;
             bdt.PCBA.HighRefSet      = HIGH_REF_2PF;
             bdt.PCBA.HighRefGainSet = REF_GAIN_12PF;
             bdt.PCBA.LowRefSet      = LOW_REF_0PF; 
             bdt.PCBA.LowRefGainSet  = REF_GAIN_12PF;
         }
    else if(max<975)
         { 
             bdt.ScreenAdaptiveFlag  = 1;
             bdt.PCBA.HighRefSet      = HIGH_REF_2PF;
             bdt.PCBA.HighRefGainSet = REF_GAIN_16PF;
             bdt.PCBA.LowRefSet      = LOW_REF_0PF; 
             bdt.PCBA.LowRefGainSet  = REF_GAIN_16PF;  
         }
    #endif
}
#endif

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_DepressRecvLineNoise0(uint16_t TXi)
{
    uint8_t  j;
    uint8_t  tempcount = 0;
    int16_t  aveval    = 0;
    int16_t  maxval    = 0;
    int16_t  sum       = 0;
    int16_t  minval    = MAX_INT16;
    uint16_t tempnum;
    
    /********************************************************************************
    *Compute MAX value and SUM if the sample is belower than threshold
    ********************************************************************************/
    for (j = 0;j < SRECV_NUM;j++)
    {
        if(abs16(bdt.DeltaDat16A[TXi][j]) < bdt.ThresholdInFrame)
        {
            tempcount++;
            sum += bdt.DeltaDat16A[TXi][j];
            if(maxval < bdt.DeltaDat16A[TXi][j]) 
            {
                maxval = bdt.DeltaDat16A[TXi][j]; /*get max value position*/
            }
            if(minval > bdt.DeltaDat16A[TXi][j]) 
            {
                minval = bdt.DeltaDat16A[TXi][j]; /*get min value position*/
            }
        }
    }
    
    /********************************************************************************
    *Compute Average value if the sample is belower than threshold
    ********************************************************************************/
    if(tempcount != 0)
    {
        aveval = divideINT16byUINT8(sum, tempcount);
    }
    /********************************************************************************
    * If MAX value is too big, Recompute average value
    ********************************************************************************/
    if(((aveval > 10) && (maxval > (aveval<<1)))||((aveval < 10) && (maxval > 100)))
    {
        tempcount = 0;
        sum       = 0;
        for (j = 0; j < SRECV_NUM; j++)
        {   
            if((abs16(bdt.DeltaDat16A[TXi][j]) < aveval) && (abs16(bdt.DeltaDat16A[TXi][j]) < bdt.ThresholdInFrame))
            {
                tempcount++;
                sum += bdt.DeltaDat16A[TXi][j];
            }
        }
        /****************************************
         * Compute average value
         ****************************************/
        if(tempcount != 0) 
        {
            aveval = divideINT16byUINT8(sum, tempcount);
        }
        else
        {
            aveval = 0;
        }
    }
    /*****************************************************************************
    *if Min value is too big Recompute average value
    *****************************************************************************/
    if((aveval < -10 && minval < (aveval<<1))||((aveval > (-10)) && (minval < -100)))
    {
        tempcount = 0;
        sum       = 0;
        for (j = 0; j < SRECV_NUM; j++)
        { 
            if((abs16(bdt.DeltaDat16A[TXi][j]) > aveval)&&(abs16(bdt.DeltaDat16A[TXi][j]) < bdt.ThresholdInFrame))
            {
                tempcount++;
                sum += bdt.DeltaDat16A[TXi][j];
            }
        }
        /****************************************
        *Compute average value
        ****************************************/
        if(tempcount != 0) 
        {
            aveval = divideINT16byUINT8(sum, tempcount);
        }
        else
        {
            aveval = 0;
        }
    }
    /* Reseting threshold value to calculate average value   */
    if(abs16(minval) > abs16(aveval<<3) || abs16(maxval) > abs16(aveval<<3))
    {
        sum = 0;
        tempcount = 0;
        if(abs16(minval) > abs16(maxval))
        {
            tempnum = abs16(minval);
        }
        else
        {
            tempnum = abs16(maxval);
        }
        for (j = 0; j < SRECV_NUM; j++)
        {
            if(abs16(bdt.DeltaDat16A[TXi][j]) < (tempnum>>1))
            {
                tempcount++;
                sum += bdt.DeltaDat16A[TXi][j];
            }
        }    
        if(tempcount != 0) 
        {
            aveval = divideINT16byUINT8(sum, tempcount);
        }
        else
        {
            aveval = 0;
        }
    }
      /********************************************************************************
      * Adjust delta data by average value
      ********************************************************************************/
    for (j = 0; j < SRECV_NUM; j++) 
    {
        bdt.DeltaDat16A[TXi][j] -= aveval;
    }
      /****************************************
      * Record All these average value
      ****************************************/
    bdt.AveDeltaXmtr[TXi] = aveval;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void DataProc_DepressNoise(void)
{
    int16_t  i;
    
    for (i = 0;i < SXMTR_NUM;i++)
    {
        DataProc_DepressRecvLineNoise0(i);

    }
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_FindMaxAndMinValue(void)
{
    uint32_t i, j;
    int16_t  tempdat16;
    
    bdt.MaxValueInFrame = 0; /* Max Value*/
    bdt.MinValueInFrame = 0; /* Min Value*/
    bdt.FRM_MAX_X_XMTR  = 0; /* Max-X-Location*/
    bdt.FRM_MAX_Y_RECV  = 0; /* Max-Y-Location*/
    bdt.FRM_MIN_X_XMTR  = 0; /* Min-X-Location*/
    bdt.FRM_MIN_Y_RECV  = 0; /* Min-Y-Location*/
    
    for (i=0; i<XMTR_NUM; i++)
        for (j=0; j<RECV_NUM; j++)
        {
            tempdat16 = bdt.DeltaDat16A[i][j];
            /*******************************************************
            * Extract the Max/Min Value of the Delta Frame data
            *******************************************************/
            if(tempdat16 > bdt.MaxValueInFrame)
            {
                bdt.MaxValueInFrame   = tempdat16;
                bdt.FRM_MAX_X_XMTR    = i;
                bdt.FRM_MAX_Y_RECV    = j;
            }
            
            if(tempdat16 < bdt.MinValueInFrame)
            {
                bdt.MinValueInFrame = tempdat16;
                bdt.FRM_MIN_X_XMTR  = i;
                bdt.FRM_MIN_Y_RECV  = j;
            }
            
            /****************************************************************
            * The following code is useful, it will make sure DEBUG value 
            * shown in PC terminal will not keep the old debug value (QFU)
            *****************************************************************/
            #ifdef COMMUNICATION_WITH_PC
            bdt.SigDeltaDat[i][j] = 0;
            #endif
            
//            #ifdef CN1100_LINUX
            bdt.SigDeltaDat[i][j] = 0;
//            #endif
        }
}

/*******************************************************************************
* Function Name  : DataProc_FindValue
* Description    : 查找特殊值:最大值、最小值、次大值、次小值
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_FindSpecialValue(uint16_t *buffer, uint16_t *max, uint16_t *min, uint16_t *max1, uint16_t *min1)
{
    uint16_t i,j;
    *max=0;
    *min=4095;
    *max1=0;
    *min1=4095;

    for (i = 0;i < SXMTR_NUM;i++)                          // 查找最大值max，最小值min
    {
        for (j = 0;j < SRECV_NUM;j++)
        {
             if(buffer[i*RECV_NUM+j]>*max)
             { 
                 *max=buffer[i*RECV_NUM+j];
                #ifdef CHANNEL_ADAPTIVE
                 bdt.AbnormalPoint[0] = (i<<4)|j;          // 0：标记最大值的坐标
                #endif
             }
             if(buffer[i*RECV_NUM+j]<*min)
             { 
                 *min=buffer[i*RECV_NUM+j];
                #ifdef CHANNEL_ADAPTIVE
                 bdt.AbnormalPoint[1] = (i<<4)|j;          // 1: 标记最小值的坐标
                #endif
             }
         }
     }
    for(i=0; i<SXMTR_NUM; i++)                             // 查找次大值max1，次小值min1
    {
        for(j=0; j<SRECV_NUM; j++)
        {
            if(buffer[i*RECV_NUM+j]>*max1)
            {
                if(buffer[i*RECV_NUM+j] != *max)
                {
                    *max1=buffer[i*RECV_NUM+j];
                 #ifdef CHANNEL_ADAPTIVE
                   bdt.AbnormalPoint[2] = (i<<4)|j;       // 2: 标记次大值的坐标
                 #endif
                }
            }
            if(buffer[i*RECV_NUM+j]<*min1)
            {
                if(buffer[i*RECV_NUM+j] != *min)
                {
                    *min1=buffer[i*RECV_NUM+j];
                  #ifdef CHANNEL_ADAPTIVE
                    bdt.AbnormalPoint[3] = (i<<4)|j;      // 3: 标记次小值的坐标
                  #endif
                }
            }
        }
    }
}


#ifdef CHANNEL_ADAPTIVE
/*******************************************************************************
* Function Name  : DataProc_CalTxAverageValue
* Description    : 分别计算TX每个通道的平均值，存放在bdt.TxAveValue[]
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_CalTxAverageValue(uint16_t *buffer)
{
    uint16_t i, j;
    uint32_t Txsum = 0;
    uint32_t Allsum = 0;
    for(i=0; i<SXMTR_NUM; i++)
    {
        for(j=0; j<SRECV_NUM; j++)
            Txsum += (int16_t)(buffer[i*RECV_NUM+j]);
        Allsum += Txsum;
        bdt.TxAveValue[i] = Txsum/SRECV_NUM;
        Txsum = 0;
    }
    bdt.AllAveValue = Allsum/(SXMTR_NUM*SRECV_NUM);
}
/*******************************************************************************
* Function Name  : DataProc_CalRxAverageValue
* Description    : 分别计算RX每个通道的平均值，存放在bdt.RxAveValue[]
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_CalRxAverageValue(uint16_t *buffer)
{
    uint16_t i, j;
    uint32_t Rxsum = 0;
    uint32_t Allsum = 0;
    for(j=0; j<SRECV_NUM; j++)
    {
        for(i=0; i<SXMTR_NUM; i++)
            Rxsum += (int16_t)(buffer[i*RECV_NUM+j]);
        Allsum += Rxsum;
        bdt.RxAveValue[j] = Rxsum/SXMTR_NUM;
        Rxsum = 0;
    }
    bdt.AllAveValue = Allsum/(SXMTR_NUM*SRECV_NUM);
}


/*******************************************************************************
* Function Name  : DataProc_AdjustRxChannelFcap
* Description    : 分别调整每个RX通道的FCAP
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_AdjustRxChannelFcap(void)
{
    uint16_t j;
    int32_t temp;
    for(j=0; j<SRECV_NUM; j++)
    {
        temp = bdt.RxAveValue[j] - bdt.AllAveValue;
        if((temp>0) && ((temp)>RXCHANNEL_THRESHOLD))        // 原始值过大，FCAP加1
        {
            bdt.RxFcapValue[j] = bdt.PCBA.RcvmRcvrFcapSet+1;
            bdt.AdjustRxChFlag = 1;                             // 可以调整Rx通道
        }
        else if((temp<0) && ((-temp)>RXCHANNEL_THRESHOLD))    // 原始值过小，FCAP减1
        {
            bdt.RxFcapValue[j] = bdt.PCBA.RcvmRcvrFcapSet-1;
            bdt.AdjustRxChFlag = 1;
        }
        else
        {
            bdt.RxFcapValue[j] = bdt.PCBA.RcvmRcvrFcapSet;    // 原始值合理，FCAP不变
            bdt.AdjustRxChFlag = 0;                           // 无需调整Rx通道
        }
    }
}


/*******************************************************************************
* Function Name  : DataProc_FindAbnormalChannel
* Description    : 查找异常通道，使用bdt.TxAbnormalCh[]和bdt.RxAbnormalCh[]进行标记
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_FindAbnormalChannel(void)
{
    uint16_t i,j;
    for(i=0; i<SXMTR_NUM; i++)
    {
        if(bdt.TxAveValue[i] != 0)                                //排除由于接线不稳定导致通道为0的状况
        {
            if(abs16(bdt.AllAveValue-bdt.TxAveValue[i]) > ABCHVALUE_THRESHOLD)
            {
                if(bdt.AllAveValue > bdt.TxAveValue[i])
                    bdt.TxAbnormalCh[i] = bdt.PCBA.RcvmRcvrFcapSet-1;                            //标记i通道原始值偏小
                else
                    bdt.TxAbnormalCh[i] = bdt.PCBA.RcvmRcvrFcapSet+1;                            //标记i通道原始值偏大
                bdt.AbnormalTxChNum++;
                if(bdt.AbnormalTxChNum >= ABCHANDPOINT_MAXNUM)            //最多只处理其中的两个通道，其他的通道暂时无法支持
                    break;
            }
        }
    }   
    if(bdt.AbnormalTxChNum >= ABCHANDPOINT_MAXNUM)                    //当Tx异常通道数目>2之后不再进行Rx的相关计算
    {
        //do nothing
    }
    else
    {
        for(j=0; j<SRECV_NUM; j++)
        {
            if(bdt.RxAveValue[j] != 0 )
            {
                if(abs16(bdt.AllAveValue-bdt.RxAveValue[j]) > ABCHVALUE_THRESHOLD)
                {
                    if(bdt.AllAveValue > bdt.RxAveValue[j])
                        bdt.RxAbnormalCh[j] = bdt.RxFcapValue[j]-1;                        //标记j通道原始值偏小
                    else
                        bdt.RxAbnormalCh[j] = bdt.RxFcapValue[j]+1;                        //标记j通道原始值偏大
                    bdt.AbnormalRxChNum++;
                    if((bdt.AbnormalTxChNum+bdt.AbnormalRxChNum) >= ABCHANDPOINT_MAXNUM)
                        break;
                }
            }
        }
    }
    bdt.FindOkFlag = 1;
}
/*******************************************************************************
* Function Name  : DataProc_AdjustAbnormalChannel
* Description    : 调整异常通道，将需要调整的实际通道赋给bdt.RealAbnormalTxCh[]和bdt.RealAbnormalRxCh[]
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_AdjustAbnormalChannel(void)
{
    uint16_t i,j;
    for(i=0,j=0; i<SXMTR_NUM; i++)
    {
        if(-1 != bdt.TxAbnormalCh[i])
        {
            bdt.RealAbnormalTxCh[j] = i;
            j++;
            if(j >= bdt.AbnormalTxChNum)
                break;
        }
    }
    for(i=0,j=0; i<SRECV_NUM; i++)
    {
        if(-1 != bdt.RxAbnormalCh[i])
        {
            bdt.RealAbnormalRxCh[j] = i;
            j++;
            if(j >= bdt.AbnormalRxChNum)
                break;
        }
    }
}
/*******************************************************************************
* Function Name  : DataProc_FindAbnormalPonit
* Description    : 查找异常点，使用bdt.AbnormalPoint高8位存储FCAP
* Input          : max：最大值；min：最小值；max1：次大值；min1：次小值
* Output         : void
* Return         : void
*******************************************************************************/
void DataProc_FindAbnormalPonit(uint16_t max,uint16_t min,uint16_t max1,uint16_t min1)
{
    uint16_t fcap;
    uint16_t rxloc;
    if(max > ABPOINTMAX_THRESHOLD)                     // max过大，需把该点调小，FCAP增加
    {
        rxloc = (bdt.AbnormalPoint[0]%256)%16;         // 定位到max所在的Rx通道
        fcap = bdt.RxFcapValue[rxloc]+1;               // 获取该Rx通道的fcap值，增加FCAP
        bdt.AbnormalPointNum++;
        bdt.AbnormalPoint[0] = (fcap<<8)|bdt.AbnormalPoint[0];
    }
    if(max1 > ABPOINTMAX_THRESHOLD)                    //max1过大，需把该点调小
    {
        rxloc = (bdt.AbnormalPoint[2]%256)%16;         // 定位到max1所在的Rx通道
        fcap = bdt.RxFcapValue[rxloc]+1;
        bdt.AbnormalPointNum++;
        bdt.AbnormalPoint[2] = (fcap<<8)|bdt.AbnormalPoint[2];
    }
    
    if(bdt.AbnormalPointNum < ABCHANDPOINT_MAXNUM)     //最多只能调节2个异常点
    {
        if(min < ABPOINTMIN_THRESHOLD)                 //min过小，需把该点调大，FCAP减小
        {
            bdt.AbnormalPointNum++;
            rxloc = (bdt.AbnormalPoint[1]%256)%16;
            fcap = bdt.RxFcapValue[rxloc]-1;
            bdt.AbnormalPoint[1] = (fcap<<8)|bdt.AbnormalPoint[1];
        }
        if(bdt.AbnormalPointNum < ABCHANDPOINT_MAXNUM)
        {
            if(min1 < ABPOINTMIN_THRESHOLD)    //min过小，需把该点调大，FCAP=1
            {
                bdt.AbnormalPointNum++;
                rxloc = (bdt.AbnormalPoint[3]%256)%16;
                fcap = bdt.RxFcapValue[rxloc]-1;
                bdt.AbnormalPoint[3] = (fcap<<8)|bdt.AbnormalPoint[3];
            }
        }
    }
}

/*******************************************************************************
* Function Name  : DataProc_AdjustAbnormalPoint
* Description    : 调整异常点，获取异常点坐标，获取FCAP值
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_AdjustAbnormalPoint(void)
{
    uint16_t i,count=0;
    uint16_t temp=0;
    for(i=0; i<ABNORMALPOINT_MAXNUM; i++)
    {
        temp=(bdt.AbnormalPoint[i]>>8);                    //获取FCAP的值
        if(temp !=0 )
        {
            if(bdt.AbnormalPointNum == 1)                  // 只有1个异常点
            {
                bdt.TxRxiFCAP = temp;
                bdt.AbPointTxiCoord = ((bdt.AbnormalPoint[i]%256)/16);    // 获取异常点坐标
                bdt.AbPointRxiCoord = ((bdt.AbnormalPoint[i]%256)%16);
                break;
            }
            if(bdt.AbnormalPointNum == 2)                    //2个异常点
            {
                if(count == 0)
                {
                    bdt.TxRxiFCAP = temp;
                    bdt.AbPointTxiCoord = ((bdt.AbnormalPoint[i]%256)/16);
                    bdt.AbPointRxiCoord = ((bdt.AbnormalPoint[i]%256)%16);
                    count++;
                }
                else
                {
                    bdt.TxRxjFCAP = temp;
                    bdt.AbPointTxjCoord = ((bdt.AbnormalPoint[i]%256)/16);
                    bdt.AbPointRxjCoord = ((bdt.AbnormalPoint[i]%256)%16);
                }
            }
        }
    }
    bdt.FindOkFlag = 1;
}
#endif

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_CalculateDeltaData(uint16_t *buffer)
{
    uint32_t i, j;
    
    /*******************************************************
    * Get the Delta Value in a X/Y point
    *******************************************************/
    if(1 == TX_DRIVE_PL)
    {
        for (i=0; i<XMTR_NUM; i++)
            for (j=0; j<RECV_NUM; j++)
            {
                bdt.DeltaDat16A[i][j] = (int16_t)(bdt.BFD.BaseDat[i][j]) - (int16_t)(buffer[i*RECV_NUM+j]);
            }
    }
    else
    {
        for (i=0; i<XMTR_NUM; i++)
            for (j=0; j<RECV_NUM; j++)
            {
                bdt.DeltaDat16A[i][j] = (int16_t)(buffer[i*RECV_NUM+j]) - (int16_t)(bdt.BFD.BaseDat[i][j]);
            }
    }
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
int16_t DataProc_Ratio2Threshold(int16_t MaxValue, uint16_t flag)
{
    int16_t value = 0;
    
    switch(flag)
    {
        case BE20PERCENT:
        {
            value  = MaxValue>>3;    /* 0.125*/
            value += MaxValue>>4;    /* 0.0625*/
            value += MaxValue>>6;    /* 0.015625, Total: 0.203125*/
            break;
        }
        case BE25PERCENT:
        {
            value  = MaxValue>>2;    /* 0.25*/
            break;
        }
        
        case BE30PERCENT:
        {
            value  = MaxValue>>2;    /* 0.25*/
            value += MaxValue>>4;    /* 0.0625, Total: 0.3125*/
            break;
        }
        case BE40PERCENT:
        {
            value  = MaxValue>>2;    /* 0.25*/
            value += MaxValue>>3;    /* 0.125 */
            value += MaxValue>>5;    /*0.03125 Total:0.40625*/
            break;
        }
    }
    
    /**********************************************************************
    * It is defined as 50 at the begining, but it could be tuned by TOOLs
    ***********************************************************************/
    if(value < bdt.PCBA.FinThrMin) 
    {
        value = bdt.PCBA.FinThrMin;
    }
    return value;
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_DeltaMatrix2Array(uint32_t *BitDat, int16_t thr)
{
    uint32_t i, j;
    
    bdt.FingerDetectNum = 0;
    for (i=0; i<XMTR_NUM; i++)
    {
        BitDat[i] = 0;
        for (j=0; j<RECV_NUM; j++)
        {
            if((bdt.DeltaDat16A[i][j]) > thr)
            {
                BitDat[i]  |= (1<<j);    /* 逐行记录 */
                bdt.FingerDetectNum++;
            }
        }
    }
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint16_t DataProc_PixelR2RConnected(uint32_t pg, uint32_t bits)
{

    if((pg & bits) || ((pg&(bits>>1))||(pg&(bits<<1))))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_MergePixelGroup(uint16_t curXMTR, uint32_t pg)
{
    uint16_t i, p;
    uint16_t FlagDoesOverlap = 0;
    uint32_t bits;
    
    
    if (bdt.FingerDetectNum && (bdt.FingerDetectNum <= FINGER_NUM_MAX) && curXMTR)
    {
        i = bdt.FingerDetectNum;
        p = 0;
        
        while(i--)
        {
            bits = bdt.DPD[p].BitMapDat[curXMTR - 1];  
            if (DataProc_PixelR2RConnected(pg, bits))
            {
                bdt.DPD[p].BitMapDat[curXMTR] |= pg;
                FlagDoesOverlap = 1;
            }
            p++;  /* Point to the next pixel group*/
        }
    }
    
    /**************************************************
    * If this pixel group is not connected, then we 
    * need to start a new segmented image.
    ***************************************************/
    if(FlagDoesOverlap == 0)
    {
        if(bdt.FingerDetectNum < FINGER_NUM_MAX)
        {
            bdt.DPD[bdt.FingerDetectNum].BitMapDat[curXMTR] = pg;
            bdt.FingerDetectNum += 1;
        }
    }
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint16_t DataProc_bBitmapsIntersect(uint32_t *ar1, uint32_t *ar2)
{
    uint16_t i;
    for (i = 0;i < XMTR_NUM;i++)
    {
        if(ar1[i]&ar2[i]) 
        {
            return 1;
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
uint16_t DataProc_CompareAndMerge(uint32_t *ar1, uint32_t *ar2, uint16_t m1, uint16_t m2)
{
    if((bdt.FingerDetectBitFlag & m1) && DataProc_bBitmapsIntersect(ar1,ar2))
    {
        /* Merge the bitmaps together*/
        uint16_t i,j;
        for (i = 0;i < XMTR_NUM;i++)
        {
            ar1[i] |= ar2[i];
        }
        /*delete Merged bitmaps*/
        for (i = m2;i < bdt.FingerDetectNum - 1;i++)
            for (j = 0;j < XMTR_NUM;j++)
            { 
                bdt.DPD[i].BitMapDat[j] = bdt.DPD[i + 1].BitMapDat[j];
            }
        
        
        return 1;
    }
    return 0;
}

/*****************************************************************************
* Purpose: Loop through all of the segmented bitmaps and determine if any
*          intersect. If they do, then merge them together.
******************************************************************************/
/*******************************************************************************
  * Function Name  : 
  * Description    : 
  * Input          : 
  * Output         : 
  * Return         : 
  *******************************************************************************/

void DataProc_MergeApartBitmaps(void)
{
    uint16_t aMergeFlag, nAS, i, j, m;
    uint32_t *pSBM1, *pSBM2;
    
    VarSL(bdt.FingerDetectBitFlag, bdt.FingerDetectNum, FINGER_NUM_MAX);
    bdt.FingerDetectBitFlag--;
    
    if(bdt.FingerDetectNum > 1)
    {
        do
        {
            aMergeFlag = 0;
            nAS        = bdt.FingerDetectNum;
            for (i=0,m=1; i<nAS-1; i++,m<<=1)
            {
                pSBM1 = bdt.DPD[i].BitMapDat;
                if(bdt.FingerDetectBitFlag & m)
                {
                    uint16_t n = m<<1;
                    pSBM2 = bdt.DPD[i + 1].BitMapDat;
                    for (j=i+1; j<nAS; j++,n<<=1,pSBM2 = bdt.DPD[j].BitMapDat)
                    {
                        if (DataProc_CompareAndMerge(pSBM1, pSBM2, n, j)!= 0) 
                        {
                            /* The two bit maps intersect. So, get rid of one.*/
                            aMergeFlag = 1;
                            bdt.FingerDetectBitFlag ^= (1<<bdt.FingerDetectNum);
                            bdt.FingerDetectNum--;
                        }
                    }
                }
            }
        }while(aMergeFlag);   /*if  merge one time,check  if need more */
    }
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

uint32_t DataProc_GetExtents(uint32_t *colExtent,uint32_t index)
{
    uint32_t rExtent=0;    /* Row extents*/
    uint32_t cExtent=0;    /* Column extents*/
    int16_t i;
    
    for (i=0; i<XMTR_NUM; i++)
    {
        uint32_t r;    /* Current row*/
        r = bdt.DPD[index].BitMapDat[i];
        rExtent <<= 1;
        if(r != 0) 
        {
            rExtent |= 1;
            cExtent |= r; 
        }
    }
    *colExtent = cExtent;
    return rExtent;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

int16_t DataProc_computePosition(int16_t* buffer, uint16_t index, uint8_t axial, uint8_t Flag)
{
    uint16_t i;
    uint32_t iWeightedBufferSum=0;
    uint32_t iBufferSum = 0;
    int16_t  p = 0;
    uint16_t size, startp;
#ifdef ONLY2PNT4BORDERCONPUTE
    uint16_t istart = 0, iend = 0;
#endif

    /**********************************************
    * Length of Array is 1, return 1;
    * Length of Array is 0, return 0;
    **********************************************/
    if(XMTR_AXIAL == axial)
    {
        size   = bdt.DPD[index].len_x;
        startp = bdt.DPD[index].start_x;
    }
    else
    {
        size   = bdt.DPD[index].len_y;
        startp = bdt.DPD[index].start_y;
    }
    
    if (size < 2) 
    {
        return (size + (startp<<8));
    }
    
#ifdef ONLY2PNT4BORDERCONPUTE
    iend = istart + size;
    if(BORDER_POSITION == (Flag & BORDER_POSITION))
    {
        if(BORDER_POSITION_RB == Flag)
        {
            istart = iend - 2; // Only Last 2 samples are used
        }
    }
    
    for (i = istart; i < iend; i++) 
    {
        if (buffer[i] > 0) 
        {
            iWeightedBufferSum += multiply16by16(i + 1, (uint16_t)buffer[i]); // Step 1: i+1 is necessary, 
            iBufferSum += (uint32_t)buffer[i];
        }
    }
    
    if (iBufferSum != 0)
    {
        p = (int16_t)(((uint32_t)(iWeightedBufferSum<<8))/iBufferSum) - 128;   // Step 2: so finally we "minus 128" from result
    }
#else
    if(BORDER_POSITION == (Flag & BORDER_POSITION))
    {
        for (i = 0;i < size;i++) 
            if (buffer[i] > 0) 
            {
                iWeightedBufferSum += multiply16by16((uint16_t)buffer[i], (uint16_t)buffer[i]) * (i + 1);
                iBufferSum += (uint32_t)(buffer[i] * buffer[i]);
            }
        if (iBufferSum != 0)
        {
            p = (int16_t)(((uint32_t)(iWeightedBufferSum<<8))/iBufferSum) - 128;
        }
    }
    else
    {
        for (i = 0;i < size;i++) 
            if (buffer[i] > 0) 
            {
                iWeightedBufferSum += multiply16by16(i + 1, (uint16_t)buffer[i]);
                iBufferSum += (uint32_t)buffer[i];
            }
        if (iBufferSum != 0)
        {
            p = (int16_t)(((uint32_t)(iWeightedBufferSum<<8))/iBufferSum) - 128;
        }
    }
#endif
    return (p + (startp<<8));;
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void DataProc_GetPoint(uint16_t index)
{
    int16_t *pX,*pY;
    uint8_t xFlag, yFlag;
    uint16_t maxX, maxY;
    
    xFlag = NORMAL_POSITION; /* BORDER_POSITION is no longer existed*/
    yFlag = NORMAL_POSITION; /* BORDER_POSITION is no longer existed*/
    
    pX = bdt.DPD[index].pBuffer_X; /* Data used for calculating the location X*/
    pY = bdt.DPD[index].pBuffer_Y; /* Data used for calculating the location Y*/
    
    maxX = bdt.DPD[index].Extremum_Pos_X[0];
    maxY = bdt.DPD[index].Extremum_Pos_Y[0];

    if(0 == maxX)
    {
        xFlag = BORDER_POSITION;
    }
    if(0 == maxY) 
    {
        yFlag = BORDER_POSITION;
    }
#if 1
    if(maxX == (XMTR_NUM - 1))
    {
        xFlag = BORDER_POSITION_RB;
    }
    if(maxY == (RECV_NUM - 1))
    {
        yFlag = BORDER_POSITION_RB;
    }
#else
    if(maxX == bdt.DPD[index].start_x + bdt.DPD[index].len_x - 1)
    { 
        xFlag = BORDER_POSITION;
    }
    if(maxY == bdt.DPD[index].start_y + bdt.DPD[index].len_y - 1)
    { 
        yFlag = BORDER_POSITION;
    }
#endif

    #ifdef PRESS_KEY_DETECT

       #if (KXMTR_NUM == 1)
       if((SXMTR_NUM-1) == maxX)
       {    
          xFlag = BORDER_POSITION_RB;
       }
       #endif

       #if (KRECV_NUM == 1)
       if((SRECV_NUM-1) == maxY)
       {    
          yFlag = BORDER_POSITION_RB;
       }
       #endif

    #endif
    
    #ifndef ONLY2PNT4BORDERCONPUTE
    xFlag = NORMAL_POSITION; /* BORDER_POSITION is no longer existed*/
    yFlag = NORMAL_POSITION; /* BORDER_POSITION is no longer existed*/
    #endif 

    //bdt.DPD[index].Finger_X_XMTR = (bdt.DPD[index].start_x << 8) + DataProc_computePosition(pX, bdt.DPD[index].len_x, xFlag);
    //bdt.DPD[index].Finger_Y_RECV = (bdt.DPD[index].start_y << 8) + DataProc_computePosition(pY, bdt.DPD[index].len_y, yFlag);
    bdt.DPD[index].Finger_X_XMTR = DataProc_computePosition(pX, index, XMTR_AXIAL, xFlag); // 0 means X
    bdt.DPD[index].Finger_Y_RECV = DataProc_computePosition(pY, index, RECV_AXIAL, yFlag); // 1 means Y
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void DataProc_ProjectBuffers (uint16_t index, uint16_t threshold, int16_t SingleDelta[XMTR_NUM][RECV_NUM])
{
    int16_t  *pXaxis, *pYaxis;
    int16_t  i, pVal, *pX, *pY;
    uint16_t colIterator, rowIterator;
    uint32_t bitMask, r;
    
    pXaxis = bdt.DPD[index].pBuffer_X;
    pYaxis = bdt.DPD[index].pBuffer_Y;
    
    clearArray((uint16_t *)pXaxis, XMTR_NUM);
    clearArray((uint16_t *)pYaxis, RECV_NUM);
    
    pX = pXaxis;
    pY = pYaxis;
    colIterator = bdt.DPD[index].start_y;
    rowIterator = bdt.DPD[index].start_x;
    
    for (i=0; i<(int16_t)bdt.DPD[index].len_x; i++)
    {
        /***************************************************************
        *Grab one whole row of bits at a time and try to project them
        ***************************************************************/
        r = bdt.DPD[index].BitMapDat[rowIterator];
        if (r != 0)
        {
            colIterator = bdt.DPD[index].start_y;
            bitMask     = bdt.DPD[index].colLSB;      /* start on the left side of the image*/
            pY          = pYaxis;
            while (r)
            {
                if (r & bitMask)
                {
                    pVal = SingleDelta[rowIterator][colIterator];
                    if (pVal > threshold) 
                    { 
                        *pY += pVal; 
                        *pX += pVal; 
                    }
                    r ^= bitMask;
                }
                pY++;
                colIterator++;
                bitMask <<= 1;
            }
        }
        /***************************************************
        *Advance to the next row
        ***************************************************/
        rowIterator++;
        pX++;
 
         
    }
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void DataProc_PullBitmapApart(uint32_t *buf)
{
    uint16_t i, j;
    uint32_t r, lsp, pg;
    
    bdt.FingerDetectNum = 0;
    for (i=0; i<FINGER_NUM_MAX; i++)
    for (j=0; j<XMTR_NUM; j++) 
        bdt.DPD[i].BitMapDat[j] = 0;
    
    /********************************************************
    * Whole BitMap: bdt.DeltaBitDat[]
    *    |||||||||||||||||||||||
    *    VVVVVVVVVVVVVVVVVVVVVVV
    *   Sub BitMap: bdt.DPD[0~4].BitMapDat[]
    ********************************************************/
    for (i=0; i<XMTR_NUM; i++)
    {
        /********************************************************
        * Process one row in the bitmap. A pixel group is 1 or
        * more contiguous pixels, There may be multiple pixel
        * groups in a row. For each group, we check to see to 
        * which segmented image it belongs
        ********************************************************/
        r = buf[i];   
        while(r)
        {
            lsp = r & (~(r-1));    /* Extract the LSP*/
            pg  = r & (~(r+lsp));  /* Extract the group to which it belongs*/
            
            /************************************************************
            * If a pixel group was found, then merge it with the pixel
            * group(s) found thus far, then Remove this pixel group and 
            * loop again for the next.
            ************************************************************/
            if(pg != 0) 
            {   /* 逐列转换行列式，XMTR控制列 */
                DataProc_MergePixelGroup(i, pg); 
            }
            r ^= pg;
        }
    }
    
    /************************************************************
    * Merge together non-convex images that have been incorrectly
    * split into two (or more) segments.
    ************************************************************/
    DataProc_MergeApartBitmaps();  /* 逐行 向上合并矩阵抽象出finger point value */
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void DataProc_ExpandBorder1Bit(uint32_t *buf)
{
    int       i;
    uint32_t  tempdat32, oldVal;
    
        /*****************************************************
        * Process the first TX
        *****************************************************/
    tempdat32  = buf[0];
    tempdat32 |= ((tempdat32<<1) | (tempdat32>>1));
    tempdat32 |= buf[1];
    oldVal     = buf[0];
    buf[0]     = tempdat32;
    
        /*****************************************************
        * Process the Middle TX
        *****************************************************/
    for(i=1; i<XMTR_NUM-1; i++)
    {
        tempdat32  = buf[i];
        tempdat32 |= ((tempdat32<<1) | (tempdat32>>1));
        tempdat32 |= oldVal;
        tempdat32 |= buf[i+1];
        oldVal     = buf[i];
        buf[i]     = tempdat32;
    }
    
        /*****************************************************
        * Process the last TX
        *****************************************************/
    tempdat32  = buf[XMTR_NUM-1];
    tempdat32 |= ((tempdat32<<1) | (tempdat32>>1));
    tempdat32 |= oldVal;
    buf[XMTR_NUM-1] = tempdat32;
    
    /*****************************************************
    *Make the expand bitmap VALID
    *****************************************************/
    for(i = 0; i < XMTR_NUM;i++)
    {
        buf[i] &= BITMAP_MAX;
    }    
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
/*************************************************************
  * Make sure (x,y) is a peak point, actually just make sure it
  * is not smaller than its neigborhood, maybe equal to ...
  *************************************************************/
uint8_t DataProc_IsExtremum(uint8_t x,uint8_t y)
{
    uint16_t threshold = (bdt.ThresholdInFrame);
    if(bdt.DeltaDat16A[x][y] < threshold)
    {
        return 0; /* Make sure the peak value is big enough*/
    }
    if(x > 0 && bdt.DeltaDat16A[x][y] < bdt.DeltaDat16A[x - 1][y])
    {
        return 0; /* Not Located in the first XMTR line*/
    }
    if(x < XMTR_NUM - 1 && bdt.DeltaDat16A[x][y] < bdt.DeltaDat16A[x + 1][y])
    {
        return 0; /* Not Located in the last XMTR line*/
    }
    if(y > 0 && bdt.DeltaDat16A[x][y] < bdt.DeltaDat16A[x][y - 1])
    {
        return 0; /* Not Located in the first RECV line*/
    }
    if(y < RECV_NUM - 1 && bdt.DeltaDat16A[x][y] < bdt.DeltaDat16A[x][y + 1])
    {
        return 0; /* Not Located in the last RECV line*/
    }
    return 1;
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

uint8_t DataProc_IsAroundExtremum(uint8_t x,uint8_t y)
{
    if(x > 0 && DataProc_IsExtremum(x-1,y))    
    {    
        return 1;
    }
    if(y > 0 && DataProc_IsExtremum(x,y-1))     
    {
        return 1;
    }
    if(x < XMTR_NUM-1 && DataProc_IsExtremum(x+1,y)) 
    {
        return 1;
    }
    if(y < RECV_NUM-1 && DataProc_IsExtremum(x,y+1)) 
    {
        return 1;
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

void DataProc_DelInvalidExtremum(void)
{
    uint16_t i, j, k, m, fnum;
    uint16_t X_dist, Y_dist;
    
    fnum = bdt.FingerDetectNum;
    for(i = 0; i < bdt.FingerDetectNum; i++)
    {
        if(bdt.DPD[i].ExtremumCount > 1)
        {
            /**********************************************************************
            * "ExtremumCount > 1" shown there are more than 1 peaks in the BitMap
            **********************************************************************/
            for(j = 0;   j < bdt.DPD[i].ExtremumCount; j++)
                for(k = j+1; k < bdt.DPD[i].ExtremumCount; k++)
                {
                    X_dist = (uint16_t)(abs16(bdt.DPD[i].Extremum_Pos_X[j] - bdt.DPD[i].Extremum_Pos_X[k]));
                    Y_dist = (uint16_t)(abs16(bdt.DPD[i].Extremum_Pos_Y[j] - bdt.DPD[i].Extremum_Pos_Y[k]));
                    if((X_dist + Y_dist) <= 1)
                    {
                        /**************************************************************
                        * The both peaks are located on the same RECV_line or XMTR_line
                        ***************************************************************/
                        bdt.DPD[i].ExtremumCount--;
                        for(m = k; m < bdt.DPD[i].ExtremumCount; m++)
                        {
                            bdt.DPD[i].Extremum_Pos_X[m] = bdt.DPD[i].Extremum_Pos_X[m+1];
                            bdt.DPD[i].Extremum_Pos_Y[m] = bdt.DPD[i].Extremum_Pos_Y[m+1];
                        }
                        k--;
                     }
                }
        }
        else if(0 == bdt.DPD[i].ExtremumCount)
        {
            /**********************************************************************
            * Delete the finger without Extremum
            ***********************************************************************/
            bdt.FingerDetectNum--;
            fnum--;
            for(j = i; j < bdt.FingerDetectNum; j++)
            {
                /******************************************************************
                * All related information should be shifted to make it tight
                *******************************************************************/
                for(k = 0; k < XMTR_NUM; k++)
                {
                    bdt.DPD[j].BitMapDat[k] = bdt.DPD[j+1].BitMapDat[k];
                }
                bdt.DPD[j].ExtremumCount = bdt.DPD[j+1].ExtremumCount;
                
                for(k = 0; k < FINGER_NUM_MAX; k++)
                {
                    bdt.DPD[j].Extremum_Pos_X[k] = bdt.DPD[j+1].Extremum_Pos_X[k];
                    bdt.DPD[j].Extremum_Pos_Y[k] = bdt.DPD[j+1].Extremum_Pos_Y[k];
                }
            }
            i--;
            continue;
        }
        
        /*****************************************************************
        * Make sure all the peaks are not more than FINGER_NUM_MAX
        ******************************************************************/
        if(FINGER_NUM_MAX == fnum)
        {
            /*************************************************************
            * There is only one peak for this finger area
            **************************************************************/
            bdt.DPD[i].ExtremumCount = 1;
        }
        else if(fnum + bdt.DPD[i].ExtremumCount - 1 > FINGER_NUM_MAX)
        {
            /*************************************************************
            * Discard the unwanted peaks in this finger area
            **************************************************************/
            bdt.DPD[i].ExtremumCount = FINGER_NUM_MAX - fnum + 1;
            fnum = FINGER_NUM_MAX;
        }
        else
        {
            /*************************************************************
            * Accept all possible peaks in this finger area
            **************************************************************/
            fnum += bdt.DPD[i].ExtremumCount - 1;
        }
    }
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void DataProc_FindExtremum(void)
{
    uint16_t i, j, k;
    
    for(i = 0; i < bdt.FingerDetectNum; i++)
    {
        bdt.DPD[i].ExtremumCount = 0;
        
        /******************************************************
        * Search all possible Peaks in the whole Frame
        *If the Peak is satisfied , that is enough!
        *******************************************************/
        for(j = 0; j < XMTR_NUM; j++)
            for(k = 0; k < RECV_NUM; k++)
            {
                if(bdt.DPD[i].BitMapDat[j] & (1<<k))          /* Valid Data*/
                if(DataProc_IsExtremum(j, k))                         /* Local Peak*/
                if(bdt.DPD[i].ExtremumCount < FINGER_NUM_MAX) /* Peak Allocation OK*/
                {
                    bdt.DPD[i].Extremum_Pos_X[bdt.DPD[i].ExtremumCount] = j;
                    bdt.DPD[i].Extremum_Pos_Y[bdt.DPD[i].ExtremumCount] = k;
                    bdt.DPD[i].ExtremumCount++;
                }
            }
    }    
    
    DataProc_DelInvalidExtremum();
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

uint8_t DataProc_ExpandBitmap(uint8_t ExpandFlag[XMTR_NUM][RECV_NUM],uint16_t Srcindex,uint16_t DstIndex)
{
    uint16_t i,j;
    uint16_t flag = 0;
    
    for(i = 0;i < XMTR_NUM;i++)
        for(j = 0;j < RECV_NUM;j++)
        {
            /***************************************************************************
            * It should be the current BitMap first, from the point, expand the area;
            * Make sure it is processed before, 
            ****************************************************************************/
            if( bdt.DPD[DstIndex].BitMapDat[i] & (1<<j) )
            if( EXPAND_NEED == ExpandFlag[i][j] )
            {
                ExpandFlag[i][j] = EXPAND_DONE; /* Marked this location has been processed*/
                  if(i > 0)
                {
                    if(bdt.DPD[Srcindex].BitmapOrig[i-1] & (1<<j))          /* It has to be valid area originally*/
                    if(bdt.DeltaDat16A[i-1][j] <= bdt.DeltaDat16A[i][j])    /* From Left side, it has to be smaller*/
                    if(bdt.DeltaDat16A[i-1][j] > 0)                         /* It has to be bigger than zero*/
                    {
                        /************************************************
                        * It should be a peak point OR it is NOT a 
                        * neigborhood to a peak;
                        *************************************************/
                        if(DataProc_IsExtremum(i,j) || !DataProc_IsAroundExtremum(i-1,j))
                        {
                            bdt.DPD[DstIndex].BitMapDat[i-1] |= 1<<j;
                            flag = 1;
                        }
                    }
                }
                
                /************************************************
                * If this is not Right Side [ XMTR < (NUM-1) ]
                *************************************************/
                if(i < XMTR_NUM - 1)
                {
                    if(bdt.DPD[Srcindex].BitmapOrig[i+1] & (1<<j))          /* It has to be valid area originally*/
                    if(bdt.DeltaDat16A[i+1][j] <= bdt.DeltaDat16A[i][j])    /* From Right side, it has to be smaller*/
                    if(bdt.DeltaDat16A[i+1][j] > 0)                         /* It has to be bigger than zero*/
                    {
                        /*************************************************
                        * It should be a peak point OR it is NOT a 
                        * neigborhood to a peak;
                        **************************************************/
                        if(DataProc_IsExtremum(i,j) || !DataProc_IsAroundExtremum(i+1,j))
                        {
                            bdt.DPD[DstIndex].BitMapDat[i+1] |= 1<<j;
                            flag = 1;
                        }
                    }
                }
                
                /************************************************
                * If this is not Up Side (RECV > 0)
                *************************************************/
                if(j > 0)
                {
                    if(bdt.DPD[Srcindex].BitmapOrig[i] & (1<<(j-1)))
                    if(bdt.DeltaDat16A[i][j-1] <= bdt.DeltaDat16A[i][j])
                    if(bdt.DeltaDat16A[i][j-1] > 0)
                    {
                        if(DataProc_IsExtremum(i,j) || !DataProc_IsAroundExtremum(i,j-1))
                        {
                            bdt.DPD[DstIndex].BitMapDat[i] |= 1<<(j-1);
                            flag = 1;
                        }
                    }
                }
                
                /************************************************
                * If this is not Bottom Side [ RECV < (NUM-1) ]
                *************************************************/
                if(j < RECV_NUM - 1)
                {
                    if(bdt.DPD[Srcindex].BitmapOrig[i] & (1<<(j+1)))
                    if(bdt.DeltaDat16A[i][j+1] <= bdt.DeltaDat16A[i][j])
                    if(bdt.DeltaDat16A[i][j+1] > 0)
                    {
                        if(DataProc_IsExtremum(i,j) || !DataProc_IsAroundExtremum(i,j+1))
                        {
                            bdt.DPD[DstIndex].BitMapDat[i] |= 1<<(j+1);
                            flag = 1;
                        }
                    }
                }
            }
        }
    
    /****************************************************
    * Return 1, will make the funcation be called again
    *****************************************************/
    return flag;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void DataProc_ExtractOneFingerArea(uint16_t index, uint16_t ExIndex)
{
    uint16_t i, j, fnum;
    uint16_t DstIndex;
    uint8_t  MaxX, MaxY;
    uint8_t  ExpandFlag[XMTR_NUM][RECV_NUM];
    
        /*************************************************
        * Initialize MaxX, MaxY
        **************************************************/
    MaxX = bdt.DPD[index].Extremum_Pos_X[ExIndex];
    MaxY = bdt.DPD[index].Extremum_Pos_Y[ExIndex];
    
        /*************************************************
        * Compute DstIndex: ID of this Extremum
        **************************************************/
    fnum = bdt.FingerDetectNum;
    for(i = 0; i < index; i++)
    {
        fnum += bdt.DPD[i].ExtremumCount - 1;
    }
    if(ExIndex != 0)
    {
        DstIndex = fnum + ExIndex - 1;
        bdt.DPD[DstIndex].Extremum_Pos_X[0] = MaxX;
        bdt.DPD[DstIndex].Extremum_Pos_Y[0] = MaxY;
    }
    else 
    {
        DstIndex = index;
    }
        /*************************************************
        * Initialize Flag Arrays
        **************************************************/
    for (i = 0;i < XMTR_NUM;i++)
        for (j = 0;j < RECV_NUM;j++)
        {
            ExpandFlag[i][j] = EXPAND_NEED;
        }
        /*************************************************
        * Initialize  Bitmap
        **************************************************/
    for (i = 0; i < XMTR_NUM;i++)
    {
        bdt.DPD[DstIndex].BitMapDat[i] = 0;
    }
    bdt.DPD[DstIndex].BitMapDat[MaxX] |= 1<<MaxY;
    
        /*************************************************
        * Expand bitmap from index to DstIndex
        **************************************************/
    while(DataProc_ExpandBitmap(ExpandFlag, index, DstIndex));
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void DataProc_ExtractExtremumArea(void)
{
    uint16_t i,j;
    for (i = 0; i < bdt.FingerDetectNum; i++)
    {
        DataProc_ExpandBorder1Bit(bdt.DPD[i].BitMapDat);
        
        /**********************************************************************
        * Based on "ExtremumCount", Split it into differnt finger area
        **********************************************************************/
        if(bdt.DPD[i].ExtremumCount > 1)
        {
            for (j = 0; j < XMTR_NUM;j++)
            {
                bdt.DPD[i].BitmapOrig[j] = bdt.DPD[i].BitMapDat[j];
                bdt.DPD[i].BitMapDat[j]  = 0;
            }
            for (j = 0; j < bdt.DPD[i].ExtremumCount; j++)
            {
                DataProc_ExtractOneFingerArea(i, j);
            }
        }
    }
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void DataProc_ComputeOffset(uint16_t *FO, uint16_t *BO, uint16_t Pos)
{
    if(MIDDLE_OF_FIVE == Pos)      
    { 
        *FO = 2; 
        *BO = 2; 
    }
    else if(FRONT_OF_FOUR == Pos) 
    { 
        *FO = 1;
        *BO = 2;
    }
    else                           
    {
        *FO = 2; 
        *BO = 1;
    }
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void DataProc_BalanceValNearPeak(uint16_t index, int16_t buf[XMTR_NUM][RECV_NUM])
{
    uint16_t xShape, yShape, exception = 0;
    int16_t  maxXp, maxYp;
    int16_t  Value1, Value2;
    int16_t  Value3, Value4;
    uint16_t FOX,  BOX,  FOY,  BOY;
    int16_t  i, j, mxyv;
    int16_t  mxm1v=0, mxp1v=0;
    int16_t  mxm2v=0, mxp2v=0;
    int16_t  mym1v=0, myp1v=0;
    int16_t  mym2v=0, myp2v=0;
    
    
    maxXp  = bdt.DPD[index].Extremum_Pos_X[0];    /* Location of X*/
    maxYp  = bdt.DPD[index].Extremum_Pos_Y[0];    /* Location of Y*/
    
    for (i = 0;i < bdt.LFrame_NUM; i++)
    {
        Value1 = abs16((int16_t)bdt.LFrame_X_XMTR[i] - maxXp);
        Value2 = abs16((int16_t)bdt.LFrame_Y_RECV[i] - maxYp);
        if((Value1 <= 1) && (Value2 <= 1))
        {
            exception = 1;
            break;
        }
    }
    
    xShape = MIDDLE_OF_FIVE;                      /* Default setting is  MIDDLE_OF_FIVE*/
    yShape = MIDDLE_OF_FIVE;                      /* Default setting is  MIDDLE_OF_FIVE*/
    mxyv   = buf[maxXp][maxYp];
    Value3 = (mxyv>>3);                           /* MaxValue(*0.125)*/
    Value4 = 0 - Value3;                          /* Negative Value3*/
    Value2 = (mxyv>>1);                           /* MaxValue(*0.5)*/
    Value1 = Value2 + (mxyv>>2);                  /* MaxValue(*0.75)*/
    bdt.DPD[index].DataInvalidFlag = 0;           /* Default we think all data is valid*/
    
    if(maxXp > 0)
    {
        mxm1v = buf[maxXp-1][maxYp];
    }
    if(maxYp > 0)
    {
        mym1v = buf[maxXp][maxYp-1];
    }
    if(maxXp < (XMTR_NUM - 1))
    {
        mxp1v = buf[maxXp+1][maxYp];
    }
    if(maxYp < (RECV_NUM - 1))
    {
        myp1v = buf[maxXp][maxYp+1];
    }
    
    #if 1
        if(maxXp > 1) 
        {
            mxm2v = buf[maxXp-2][maxYp];
        }
        if(maxYp > 1)
        {
            mym2v = buf[maxXp][maxYp-2];
        }
        if(maxXp < (XMTR_NUM - 2))
        {
            mxp2v = buf[maxXp+2][maxYp];
        }
        if(maxYp < (RECV_NUM - 2))
        {   
            myp2v = buf[maxXp][maxYp+2];
        }
    #endif
    
   /**********************************************
         * Compute the positon of extremum
         ***********************************************/
    if(mxm1v > Value1) 
    {
        xShape += BACK_OF_FOUR;     /* 4 Sample is good choice; L, LH, HH, L*/
    }
    if(mxp1v > Value1) 
    {
        xShape += FRONT_OF_FOUR;       /* 4 Sample is good choice; L, HH, LH, L*/
    }
    if(mym1v > Value1)
    {
        yShape += BACK_OF_FOUR;   
    }
    if(myp1v > Value1) 
    {
        yShape += FRONT_OF_FOUR;      
    }
   /*************************************************************
   * For Shape = MIDDLE_OF_FIVE, p2v and m2V should be small
   **************************************************************/
    if(MIDDLE_OF_FIVE == xShape)
    {
        if(mxm1v < (mxm2v<<1)) 
        {
            buf[maxXp-2][maxYp] = 0;
            if(maxYp > 0)           
            { 
                buf[maxXp-2][maxYp-1] = 0;
            }
            if(maxYp < (RECV_NUM - 1)) 
            {
                buf[maxXp-2][maxYp+1] = 0;
            }
            if(maxYp > 1)           
            {
                buf[maxXp-2][maxYp-2] = 0;
            }
            if(maxYp < (RECV_NUM - 2)) 
            {
                buf[maxXp-2][maxYp+2] = 0;
            }
        }
        if(mxp1v < (mxp2v<<1)) 
        {
            buf[maxXp+2][maxYp] = 0;
            if(maxYp > 0)            
                buf[maxXp+2][maxYp-1] = 0;
            if(maxYp < (RECV_NUM - 1))
                buf[maxXp+2][maxYp+1] = 0;
            if(maxYp > 1)            
                buf[maxXp+2][maxYp-2] = 0;
            if(maxYp < (RECV_NUM - 2)) 
                buf[maxXp+2][maxYp+2] = 0;
        }
    }
    if(MIDDLE_OF_FIVE == yShape)
    {
        if(mym1v < (mym2v<<1)) 
        {
            buf[maxXp][maxYp-2] = 0;
            if(maxXp > 0)           
            {    
                buf[maxXp-1][maxYp-2] = 0;
            }
            if(maxXp < (XMTR_NUM - 1)) 
            {    
                buf[maxXp+1][maxYp-2] = 0;
            }
            if(maxXp > 1)           
            { 
                buf[maxXp-2][maxYp-2] = 0;
            }
            if(maxXp < (XMTR_NUM - 2))
            { 
                buf[maxXp+2][maxYp-2] = 0;
            }
        }
        if(myp1v < (myp2v<<1)) 
        {
            buf[maxXp][maxYp+2] = 0;
            if(maxXp > 0)            
            { 
                buf[maxXp-1][maxYp+2] = 0;
            }
            if(maxXp < (XMTR_NUM - 1))
            {
                buf[maxXp+1][maxYp+2] = 0;
            }
            if(maxXp > 1)
            {
                buf[maxXp-2][maxYp+2] = 0;
            }
            if(maxXp < (XMTR_NUM - 2))
            {
                buf[maxXp+2][maxYp+2] = 0;
            }
        }
    }
    
   /**********************************************
   * Check whether it is a pure finger point
   * All the following case should be invalid data
   ***********************************************/
    if(exception != 0) 
    {
        bdt.DPD[index].DataInvalidFlag = 0;
    }
    else
    {
        if(xShape > BACK_OF_FOUR)  
        {
            bdt.DPD[index].DataInvalidFlag = 1;                        /* 两个旁瓣同时大于75%*/
        }
        else if(yShape > BACK_OF_FOUR) 
        {    
            bdt.DPD[index].DataInvalidFlag = 1;                        /* 两个旁瓣同时大于75%*/
        }
        else if((mxp2v > Value2) && (FRONT_OF_FOUR == xShape))
        {
            bdt.DPD[index].DataInvalidFlag = 1; /* L HH LH L,两个旁瓣同时大于75%||50%*/
        }
        else if((mxm2v > Value2) && (BACK_OF_FOUR == xShape))
        {
            bdt.DPD[index].DataInvalidFlag = 1; /* L LH HH L, 两个旁瓣同时大于75%||50%*/
        }
        else if((myp2v > Value2) && (FRONT_OF_FOUR == yShape))
        {
            bdt.DPD[index].DataInvalidFlag = 1; /* L HH LH L, 两个旁瓣同时大于75%||50%*/
        }
        else if((mym2v > Value2) && (BACK_OF_FOUR == yShape)) 
        {
            bdt.DPD[index].DataInvalidFlag = 1; /* L LH HH L, 两个旁瓣同时大于75%||50%*/
        }
        else if((mxm1v < Value4)||(mxp1v < Value4)) 
        {
            bdt.DPD[index].DataInvalidFlag = 1; /* 两个旁瓣同时小于负的12.5%*/
        }
        else if((mym1v < Value4)||(myp1v < Value4)) 
        {
            bdt.DPD[index].DataInvalidFlag = 1; /* 两个旁瓣同时小于负的12.5%*/
        }
        
        #if 1
        if(mxyv < FRAME_MAX_SAMPLE)
        {
            bdt.DPD[index].DataInvalidFlag = 1;   
        }
        
        Value3  = (bdt.MaxValueInFrame>>2) + (bdt.MaxValueInFrame>>3);    /* 0.375*/
        
        if(mxyv < Value3)
        {
            bdt.DPD[index].DataInvalidFlag = 1;
        }
        #endif
    }
    
  /*************************************************************
  * Compute the range of the valid value around extremum
  **************************************************************/
    DataProc_ComputeOffset(&FOX, &BOX, xShape);
    DataProc_ComputeOffset(&FOY, &BOY, yShape);
    
    if(bdt.DPD[index].DataInvalidFlag != 0)
    {
        for (i = bdt.DPD[index].start_x; i < bdt.DPD[index].start_x + bdt.DPD[index].len_x; i++)
            for (j = bdt.DPD[index].start_y; j < bdt.DPD[index].start_y + bdt.DPD[index].len_y; j++)
            {
                buf[i][j] = 0;
            }
    }
    else
    {
        for (i = bdt.DPD[index].start_x; i < bdt.DPD[index].start_x + bdt.DPD[index].len_x; i++)
            for (j = bdt.DPD[index].start_y; j < bdt.DPD[index].start_y + bdt.DPD[index].len_y; j++)
            {
                /**********************************************
                * Remove delta data out of range
                ***********************************************/
                if(i < maxXp - FOX || i > maxXp + BOX || j < maxYp - FOY || j > maxYp + BOY)
                {
                    buf[i][j] = 0;
                }
                
                /**********************************************
                * Remove more noise data we judged as it is
                ***********************************************/
                if(MIDDLE_OF_FIVE == xShape)
                {
                    if(i == (maxXp - FOX))
                    if((maxXp + BOX) < XMTR_NUM)
                    if(buf[maxXp+BOX][maxYp] == 0)
                    {
                        buf[i][j] = 0;
                    }
                    if(i == (maxXp + BOX))
                    if((maxXp - FOX) >= 0)
                    if(0 == buf[maxXp-FOX][maxYp])
                    {
                        buf[i][j] = 0;
                    }
                }
                if(MIDDLE_OF_FIVE == yShape)
                {
                    if(j == (maxYp - FOY))
                    if((maxYp + BOY) < RECV_NUM)
                    if(0 == buf[maxXp][maxYp+BOY])
                    {
                        buf[i][j] = 0;
                    }
                    if(j == (maxYp + BOY))
                    if((maxYp - FOY) >= 0)
                    if(0 == buf[maxXp][maxYp-FOY])
                    {
                        buf[i][j] = 0;
                    }
                }
            }
    }
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void DataProc_ComputePoint(void)
{
    uint16_t fnum;
    uint16_t index;
    uint32_t rowExtent,colExtent;
    uint16_t xEnd,yEnd;
    int16_t  i, j;
    int16_t  SingleDelta[XMTR_NUM][RECV_NUM] = {{0}};
    
    /*******************************************************************
    * Make sure how many fingers does the system has now
    ********************************************************************/
    fnum = bdt.FingerDetectNum;
    for (i = 0;i < bdt.FingerDetectNum;i++)
    {
        fnum += bdt.DPD[i].ExtremumCount - 1;
    }
    bdt.FingerDetectNum   = fnum;
        bdt.FingerInvalidPNum = 0;
    for (index = 0; index < fnum; index++)
    {  
        /***************************************************************
        * The following function will set:
        * Input:  index
        * Output: colExtent and rowExtent at the same time actually
        ****************************************************************/
        rowExtent = DataProc_GetExtents(&colExtent, index);
        
        bdt.DPD[index].colLSB = (~(colExtent<<1)) & colExtent; /* Here it is LSB Actually;*/
        
        bdt.DPD[index].start_y = lsbPos(colExtent); 
        yEnd = msbPos(colExtent);
        
        bdt.DPD[index].start_x = XMTR_NUM - msbPos(rowExtent) - 1;
        xEnd = XMTR_NUM - lsbPos(rowExtent) - 1;
        
        bdt.DPD[index].len_x = xEnd - bdt.DPD[index].start_x + 1;
        bdt.DPD[index].len_y = yEnd - bdt.DPD[index].start_y + 1;
        
        /***************************************************************
        * Get Single Delta Data
        ****************************************************************/
        for (i=bdt.DPD[index].start_x; i<=xEnd; i++)
            for (j=bdt.DPD[index].start_y; j<=yEnd; j++)
            {
                SingleDelta[i][j] = 0;
                if(bdt.DPD[index].BitMapDat[i] & (1<<j))
                if(bdt.DeltaDat16A[i][j] > 0)
                SingleDelta[i][j] = bdt.DeltaDat16A[i][j];
            }
        
        /***************************************************************
        * Make sure the Delta Data Shape is correct
        ****************************************************************/
        DataProc_BalanceValNearPeak(index, SingleDelta);
        if(bdt.DPD[index].DataInvalidFlag)
        {
            /**********************************************
            * Invalid data case is appeared for this point
            ***********************************************/
            bdt.DPD[index].Finger_X_XMTR   = 0; /* NO-Finger*/
            bdt.DPD[index].Finger_Y_RECV   = 0; /* No-Finger*/
            bdt.DPD[index].DataInvalidFlag = 0; /* Recover for the next judgement*/
            bdt.FingerInvalidPNum++;
        }
        else
        {
            DataProc_ProjectBuffers(index, 0, SingleDelta);
            DataProc_GetPoint(index);
        }
        
        /**************************************************************
        *Delta Data Showing Information
        ****************************************************************/
        for(i=bdt.DPD[index].start_x; i<=xEnd; i++)
            for(j=bdt.DPD[index].start_y; j<=yEnd; j++)
            {
                #ifdef COMMUNICATION_WITH_PC
                bdt.SigDeltaDat[i][j] |= SingleDelta[i][j];
                #endif
                
//                #ifdef CN1100_LINUX
                bdt.SigDeltaDat[i][j] |= SingleDelta[i][j];
//                #endif
            }
    
    }
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_CVNRXMark(uint16_t RxLine, uint16_t *RX_Flag)
{
    int16_t Rs, Re;
    uint16_t i;
    
    Rs = (int16_t)RxLine - 1;
    Re = (int16_t)RxLine + 1;
    if(Rs < 0) 
    {
        Rs = 0;
    }
    if(Re > (RECV_NUM-1)) 
    {
        Re = RECV_NUM-1;
    }
    
    for (i=Rs; i<=Re; i++) 
    {
        RX_Flag[i] = 1;
    }
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void DataProc_PNLJCurrentValidNoise(uint32_t *d)
{
    uint16_t i, j;
    int16_t  temp;
    uint16_t RX_NoiseLine[RECV_NUM];
 
    /*****************************************************
    *Initial Processing
    ******************************************************/
    for (i=0; i<RECV_NUM; i++)
    {
        RX_NoiseLine[i] = 0;
    }
    /*****************************************************
    * Extract Receive Line for Noise Summary
    ******************************************************/
    for (i=0; i<bdt.FingerDetectNum; i++)
    {
        if(bdt.DPD[i].Finger_X_Reported || bdt.DPD[i].Finger_Y_Reported)
        {
            DataProc_CVNRXMark((bdt.DPD[i].Finger_Y_Reported)>>8, RX_NoiseLine);
        }
    }

    /*****************************************************
    * Calculation the NOISE Value
    ******************************************************/
    d[0] = 0;
    d[1] = 0;
    if(FINGER_SHOW == bdt.BFD.bbdc.FingerExist)
    {
        for(j = 0; j < RECV_NUM; j++)
        {
            if(RX_NoiseLine[j] != 0)
            for(i = 0; i < XMTR_NUM; i++)
            {
                if(bdt.SigDeltaDat[i][j]<=0)
                {
                    /**************************************
                    // Only Big Negative Sample is used
                    ***************************************/
                    if(bdt.DeltaDat16A[i][j] < 0)
                    {
                        temp  = abs16(bdt.DeltaDat16A[i][j]);
                        if(temp>9) 
                        { 
                            d[0] += temp; 
                            d[1]++; 
                        }
                    }
                }
            }
        }
    }

}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void DataProc_PowerNoiseLevelJudge(void)
{
    uint32_t dnoise[2]; 
    uint8_t tab_low[4]  = {36,48,72,108};
    uint8_t tab_high[4] = {48,72,144,192};
    
    /***************************************************************
    * Calculating Current Noise 
    ****************************************************************/
    DataProc_PNLJCurrentValidNoise(dnoise);
    if(0 == dnoise[1]) 
    {
        dnoise[0] = 0; // We think noise is small; all DeltaData are regulat value
        return;
    }
    else 
    {
        dnoise[0] = dnoise[0]/dnoise[1]; // Get the average value of abnormal DeltaData
    }

    if(16 == bdt.NDD.Noise_Count)
    {
        bdt.NDD.Noise_Sum = bdt.NDD.Noise_Sum>>4; // After 16 times Calculating, we get Average Noise value
        
        switch(bdt.NDD.Battery_Level)
        {
            case BATTERY_GOOD:
            {
                if(bdt.NDD.Noise_Sum > 24)
                {
                    bdt.NDD.Battery_Level = BATTERY_BAD;
                }
                break;
            }
            case BATTERY_BAD:
            {
                if(bdt.NDD.Noise_Sum > 32) 
                {
                    bdt.NDD.Battery_Level = BATTERY_WORSE;
                }
                else if(bdt.NDD.Noise_Sum < 16)
                {
                    bdt.NDD.Battery_Level = BATTERY_GOOD;
                }
                break;
            }
            case BATTERY_WORSE:
            {
                if(bdt.NDD.Noise_Sum > 40)
                {
                    bdt.NDD.Battery_Level = BATTERY_WORST;
                }
                else if(bdt.NDD.Noise_Sum < 24) 
                {
                    bdt.NDD.Battery_Level = BATTERY_BAD;
                }
                break;
            }
            case BATTERY_WORST:
            {
                if(bdt.NDD.Noise_Sum < 32)
                {
                    bdt.NDD.Battery_Level = BATTERY_WORSE;
                }
                break;
            }
        }
        if(bdt.NDD.Noise_Sum >bdt.MaxNoise_Sum) bdt.MaxNoise_Sum = bdt.NDD.Noise_Sum;
        bdt.Noise_Sum = bdt.NDD.Noise_Sum;
        #ifdef STM32VC_LCD
        TFT_ShowNum(0, 29, bdt.NDD.Battery_Level, 0xf800, LCD_COLOR_GREEN);
        TFT_ShowNum(3, 29, bdt.NDD.Noise_Sum, 0xf800, LCD_COLOR_GREEN);
        #endif
        bdt.NDD.Noise_Count = 0;
        bdt.NDD.Noise_Sum   = 0;

        //**************************************************
        // When Finger Num is 4/5, we think it is 
        //**************************************************
        if(bdt.FingerDetectNum >3)
        {
            bdt.NDD.Battery_Level = BATTERY_WORST;
        }
        bdt.ThrLow4DistanceFilter  = tab_low[bdt.NDD.Battery_Level]; /* THR072 = 36*/
        bdt.ThrHigh4DistanceFilter = tab_high[bdt.NDD.Battery_Level]; /* THR144 = 48*/
        
    }
    else 
    {
        bdt.NDD.Noise_Count++;
        bdt.NDD.Noise_Sum += dnoise[0];
    }
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_FHBSDeltaNoiseEstimation(int16_t *buf)
{
    #ifdef FREQHOP_BYSTRETCH
    uint16_t i, j, temp;
    //int16_t  xsum = 0;
    bdt.NoiseDataFrFrame = 0;
    for (i = 0;i < XMTR_NUM;i++)
        for (j = 0;j < RECV_NUM;j++)
        {
            if(bdt.FrameDatLoadA[i][j] > bdt.FrameDatLoadB[i][j])
                temp = bdt.FrameDatLoadA[i][j] - bdt.FrameDatLoadB[i][j];
            else
                temp = bdt.FrameDatLoadB[i][j] - bdt.FrameDatLoadA[i][j];
            //xsum += (buf[i*RECV_NUM+j]);
            bdt.NoiseDataFrFrame += (uint32_t)temp;
        }

    #ifdef STM32VC_LCD
    //if(xsum<0) xsum = 0 - xsum;
    //if((bdt.BFD.InitCount & 0xf) == 0)
    //    TFT_ShowNum(3,  20, xsum, LCD_COLOR_BLUE, LCD_COLOR_GREEN);
    #endif

    #endif
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint16_t DataProc_FHBSBTFSSideInfo(uint16_t index, uint16_t *buf)
{
    uint16_t result = 0;
#ifdef FREQHOP_BYSTRETCH
    if(0 == index) result = buf[1];
    else if((STRETCH_STNUM-1) == index) result = buf[STRETCH_STNUM-2];
    else result = (buf[index-1]+buf[index+1])>>1;
#endif
    return result;
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_FHBSBestTXFreqSearch(uint16_t *buf)
{
#ifdef FREQHOP_BYSTRETCH

    uint16_t i, t1, t2;
    uint16_t leastIndex, secondIndex;
    uint16_t leastValue, secondValue;

    if(buf[1] < buf[0])
    {
        leastIndex  = 1;
        leastValue  = buf[1];
        secondIndex = 0;
        secondValue = buf[0];
    }
    else
    {
        leastIndex  = 0;
        leastValue  = buf[0];
        secondIndex = 1;
        secondValue = buf[1];
    }

    for(i = 2; i< STRETCH_STNUM; i++)
    {
        if(buf[i] < secondValue)
        {
            if(buf[i] < leastValue)
            {
                secondIndex = leastIndex;
                secondValue = leastValue;
                leastIndex  = i;
                leastValue  = buf[i];
            }
            else
            {
                secondIndex = i;
                secondValue = buf[i];
            }
        }
    }

    //***********************************************************************
    // The following code will be an algorithm to decide which one is best
    //***********************************************************************
    if(bdt.StretchInReg == leastIndex)
    {
        //*******************************************************
        // We prefer the history data first
        //*******************************************************
        bdt.StretchInReg = leastIndex;
    }
    else if(bdt.StretchInReg == secondIndex)
    {
        //*******************************************************
        // We prefer the history data first
        //*******************************************************
        bdt.StretchInReg = secondIndex;
    }
    else
    {
        t1 = leastValue>>5;
        t2 = (secondValue - leastValue);
        if(t2 > t1)
        {
            //**************************************************************
            // The least one has obvious advantage
            //**************************************************************
            bdt.StretchInReg = leastIndex;
        }
        else
        {
            //**************************************************************
            // If the least one has obvious advantage, we judge based on 
            // more information from side stretch
            //**************************************************************
            t1 = DataProc_FHBSBTFSSideInfo(leastIndex, buf);
            t2 = DataProc_FHBSBTFSSideInfo(secondIndex, buf);

            if((t1+leastValue) < (t2+secondValue))
            {
                //**************************************************************
                // The least one has final advantage
                //**************************************************************
                bdt.StretchInReg = leastIndex;
            }
            else
            {
                //**************************************************************
                // The least one has final advantage
                //**************************************************************
                bdt.StretchInReg = leastIndex;
            }
        }
    }

#endif
}


uint16_t  DataProc_FHBSOperatingSwitch(void)
{
    uint16_t result = TXSCAN_DISABLE;

#ifdef FREQHOP_BYSTRETCH
    #ifdef STM32VC_LCD
    #ifdef FHBS_DEBUG_INFOSHOW
    uint16_t fcolor = LCD_COLOR_RED;
    #endif
    #endif
    if(FINGER_SHOW == bdt.BFD.bbdc.FingerExist)
    {
         //**************************************************************
         // If bdt.StretchValue is small, means TxScan is ongoing
         //**************************************************************
         if(bdt.StretchValue < STRETCH_STNUM)
         {
             RegTab_t.Reg25BitDef_t.DursRegConf = 0;
             RegTab_t.Reg25BitDef_t.DursReg_t.DURS_STRETCH_DUR = bdt.StretchInReg;
             RegTab_t.Reg25BitDef_t.DursReg_t.DURS_STRETCH_INC = STRETCH_INC_REG25;
             SPI_write_singleData(DURS_REG,RegTab_t.Reg25BitDef_t.DursRegConf);
             bdt.StretchValue = (STRETCH_STNUM)<<1; // Make sure we start from the 0 next time
         }
         bdt.CurNoiseSum = (bdt.CurNoiseThrHi + bdt.CurNoiseThrLo)>>1;
         bdt.FingerDelay = TXSCAN_FINGERDELAY;  // Make sure we start TxScan with a DELAY next time
         result = TXSCAN_DISABLE;               // Disable TxScan for sure
    }
    else
    {
        //**************************************************************
        // NO FINGER, NO FINGER, NO FINGER, NO FINGER, NO FINGER, 
        //**************************************************************
        if(0 != bdt.FingerDelay)
        {
            //***********************************************************
            // Finger is just gone, we should wait for a while
            //***********************************************************
            result = TXSCAN_DISABLE;   // Disable TxScan for sure
            bdt.FingerDelay--;         // Update the time of Delay
        }
        else if(bdt.TxScanValid > (TXSCAN_VALIDPERIOD - TXSCAN_FINGERDELAY))
        {
            bdt.TxScanValid--;
            result = TXSCAN_DISABLE;   // Disable TxScan for sure
            #ifdef STM32VC_LCD
            #ifdef FHBS_DEBUG_INFOSHOW
            TFT_ShowNum(17, 2+STRETCH_STNUM, bdt.TxScanValid,   fcolor, LCD_COLOR_GREEN);
            #endif
            #endif
        }
        else
        {
            //***********************************************************
            // Calculating the Noise in the frame @ NO-FINGER
            //***********************************************************
            DataProc_FHBSDeltaNoiseEstimation((int16_t *)(bdt.DeltaDat16A));
            if(bdt.StretchValue >= STRETCH_STNUM)
            {
                //*************************************
                // No TX-SCAN is NOT operating NOW
                //*************************************
                bdt.CurNoiseSum  = (bdt.CurNoiseSum) - (bdt.CurNoiseSum>>3);
                bdt.CurNoiseSum += (bdt.NoiseDataFrFrame>>3);

                #ifdef STM32VC_LCD
                #ifdef FHBS_DEBUG_INFOSHOW
                TFT_ShowNum(10, 2+STRETCH_STNUM, bdt.CurNoiseSum,   fcolor, LCD_COLOR_GREEN);
                #endif
                #endif

                if(bdt.CurNoiseSum > bdt.CurNoiseThrHi)
                {
                    //***********************************************************
                    // Judgement-1: The Noise is becoming a bigger, Tx Scan again
                    //***********************************************************
                    result = TXSCAN_ENABLE;
                    #ifdef STM32VC_LCD
                    #ifdef FHBS_DEBUG_INFOSHOW
                    TFT_ShowChar(24, 4+STRETCH_STNUM, 'B', LCD_COLOR_MAGENTA, LCD_COLOR_GREEN);/* 如果NUM数据为0则显示0 */
                    #endif
                    #endif
                }
                else if(bdt.CurNoiseSum < bdt.CurNoiseThrLo)
                {
                    //*************************************************************
                    // Judgement-2: The Noise is becoming a smaller, Tx Scan again
                    //*************************************************************
                    result = TXSCAN_ENABLE;
                    #ifdef STM32VC_LCD
                    #ifdef FHBS_DEBUG_INFOSHOW
                    TFT_ShowChar(10, 4+STRETCH_STNUM, 'S', LCD_COLOR_MAGENTA, LCD_COLOR_GREEN);/* 如果NUM数据为0则显示0 */
                    #endif
                    #endif
                }
                
                if(bdt.StretchValue > STRETCH_STNUM)
                {
                    //***********************************************************
                    // Judgement-3: Last time, TXSCAN is not finished
                    //***********************************************************
                    result = TXSCAN_ENABLE;
                }
                else if(0 == bdt.TxScanValid)
                {
                    //***********************************************************
                    // Judgement-4: Tx Scan again after long time waiting
                    //***********************************************************
                    //result = TXSCAN_ENABLE;
                }
                else
                {
                    //***********************************************************
                    // TxScan result is still Valid, we should wait for a while
                    //***********************************************************
                    bdt.TxScanValid--;
                    #ifdef STM32VC_LCD
                    #ifdef FHBS_DEBUG_INFOSHOW
                    TFT_ShowNum(17, 2+STRETCH_STNUM, bdt.TxScanValid,   fcolor, LCD_COLOR_GREEN);
                    #endif
                    #endif
                }

                if(TXSCAN_ENABLE == result)
                {
                    //*******************************************************************
                    // The first step, init Stretch for the first measurement
                    // We are prepare to do the measurement
                    //*******************************************************************
                    bdt.StretchValue        = 0;
                    bdt.SumNoiseDataFrFrame = 0;
                    bdt.TxScanNoiseCount    = 0;  // Init for next Stretch Measurement
                    RegTab_t.Reg25BitDef_t.DursRegConf = 0;
                    RegTab_t.Reg25BitDef_t.DursReg_t.DURS_STRETCH_DUR = bdt.StretchValue;
                    RegTab_t.Reg25BitDef_t.DursReg_t.DURS_STRETCH_INC = STRETCH_INC_REG25;	
                    SPI_write_singleData(DURS_REG, RegTab_t.Reg25BitDef_t.DursRegConf);
                }   // Prepare to TxScan again
            }
            else
            {
                //*************************************
                // TX-SCAN is operating NOW, GO ON...
                //*************************************
                result = TXSCAN_ENABLE;
            }
        } // No FingerDelay Case
    }     // No Finger Case
#endif
    return result;
}

void DataProc_FrequencyHopByStretch(uint16_t *buf)
{
  #ifdef FREQHOP_BYSTRETCH
    uint16_t temp;
    #ifdef STM32VC_LCD
    #ifdef FHBS_DEBUG_INFOSHOW
    uint16_t i, j, tempPx, tempPy;
    uint16_t fcolor = LCD_COLOR_RED;
    #endif
    #endif

    bdt.MTD.NoFingerCnt4Doze = 0;
    if(bdt.TxScanNoiseCount >= NOISECAL_SKIPNUM)
        bdt.SumNoiseDataFrFrame += bdt.NoiseDataFrFrame;

    if(bdt.TxScanNoiseCount < (NOISECAL_SKIPNUM+NOISECAL_NUM))
    {
        //*************************************************************
        // Record and Saving the Noise Level for many(16) times
        //*************************************************************
        bdt.TxScanNoiseCount++;
    }
    else
    {
        //*********************************************************************
        // We have saved the Noise Level with the same Stretch for 16 times
        // Based on the saving Noise Level, the Aveerage Value is calculated
        //*********************************************************************
        temp  = (uint16_t)((bdt.SumNoiseDataFrFrame)>>NOISECAL_SHIFT);
        bdt.NoiseDataTable[bdt.StretchValue]   = temp;

        #ifdef STM32VC_LCD
        //*********************************************************************
        // Show the result into LCD SCREEN
        //*********************************************************************
        #ifdef FHBS_DEBUG_INFOSHOW
        tempPy = 1 + bdt.StretchValue;
        tempPx = 0;
        TFT_ShowNum(0+tempPx, tempPy, bdt.StretchValue, fcolor, LCD_COLOR_GREEN);
        TFT_ShowNum(3+tempPx, tempPy, temp, fcolor, LCD_COLOR_GREEN);
        #endif
        #endif

        //*********************************************************************
        // Reset the value for the next measuement
        //*********************************************************************
        bdt.TxScanNoiseCount    = 0;  // Init for next Stretch Measurement
        bdt.SumNoiseDataFrFrame = 0;  // Init for next Stretch Measurement

        //***************************************************************
        // This is the routine TX SCAN now on
        //***************************************************************
        bdt.StretchValue += STRETCH_STEP; // 0,1,2,3,4,..., 14, "15 will be 0"
        RegTab_t.Reg25BitDef_t.DursRegConf = 0;
        if(bdt.StretchValue >= STRETCH_STNUM) 
        { 
            //*********************************************************************
            // All measuement are done
            //*********************************************************************
            DataProc_FHBSBestTXFreqSearch(bdt.NoiseDataTable); // bdt.StretchInReg
            RegTab_t.Reg25BitDef_t.DursReg_t.DURS_STRETCH_DUR = bdt.StretchInReg;
            bdt.CurNoiseSum     = bdt.NoiseDataTable[bdt.StretchInReg];
            temp = (bdt.CurNoiseSum>>2) + (bdt.CurNoiseSum>>3);
            bdt.CurNoiseThrHi   = bdt.CurNoiseSum + temp; // High Threshold should be lower based on testing
            bdt.CurNoiseThrLo   = bdt.CurNoiseSum - temp; //(bdt.CurNoiseSum>>2);
            bdt.TxScanValid     = TXSCAN_VALIDPERIOD; //

            #ifdef STM32VC_LCD
                #ifdef FHBS_DEBUG_INFOSHOW
                TFT_ShowNum(0,  2+STRETCH_STNUM, bdt.StretchInReg, fcolor, LCD_COLOR_GREEN);
                TFT_ShowNum(3,  2+STRETCH_STNUM, bdt.NoiseDataTable[bdt.StretchInReg], fcolor, LCD_COLOR_GREEN);

                TFT_ShowNum(3,  3+STRETCH_STNUM, bdt.CurNoiseThrLo, fcolor, LCD_COLOR_GREEN);
                TFT_ShowNum(10, 3+STRETCH_STNUM, bdt.CurNoiseSum,   fcolor, LCD_COLOR_GREEN);
                TFT_ShowNum(17, 3+STRETCH_STNUM, bdt.CurNoiseThrHi, fcolor, LCD_COLOR_GREEN);

                TFT_ShowChar(10, 4+STRETCH_STNUM,' ',LCD_COLOR_GREEN,LCD_COLOR_GREEN);/* 如果NUM数据为0则显示0 */
                TFT_ShowChar(24, 4+STRETCH_STNUM,' ',LCD_COLOR_GREEN,LCD_COLOR_GREEN);/* 如果NUM数据为0则显示0 */

                for(i=0; i<STRETCH_STNUM; i++)
                    TFT_ShowBar(17, 1+i, bdt.BiggestBar, LCD_COLOR_GREEN); 

                bdt.BiggestBar = 0;
                for(i=0; i<STRETCH_STNUM; i++)
                {
                    temp = bdt.NoiseDataTable[i] - bdt.NoiseDataTable[bdt.StretchInReg];
                    if(temp&0x8000) temp = 0;
                    if(bdt.BiggestBar < temp) bdt.BiggestBar = temp;
                }
                j = 0;
                while(bdt.BiggestBar > (LCD_SCREEN_HIGH - (17<<3))) 
                {
                    bdt.BiggestBar = bdt.BiggestBar>>1;
                    j++;
                }
                for(i=0; i<STRETCH_STNUM; i++)
                {
                    temp = bdt.NoiseDataTable[i] - bdt.NoiseDataTable[bdt.StretchInReg];
                    if(temp&0x8000) temp = 0;
                    TFT_ShowNum(10, 1+i, temp, fcolor, LCD_COLOR_GREEN);
                    TFT_ShowBar(17, 1+i, temp>>j, LCD_COLOR_BLUE); 
                }
                #endif
            #endif
        }
        else
        {
            //*********************************************************************
            // Now, we change the stretch to the new value
            //*********************************************************************
            RegTab_t.Reg25BitDef_t.DursReg_t.DURS_STRETCH_DUR = bdt.StretchValue;
        }
        RegTab_t.Reg25BitDef_t.DursReg_t.DURS_STRETCH_INC = STRETCH_INC_REG25;
        SPI_write_singleData(DURS_REG,RegTab_t.Reg25BitDef_t.DursRegConf);
    }
  #endif //FREQHOP_BYSTRETCH
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void DataProc_WholeFrameProcess(uint16_t *buffer)
{
    uint16_t i, j;

    /******************************************************
    *  Extract Delta Data for the current Buffer;
    *  Get the Maximum Value and its Location;
    *******************************************************/
    //if(1 == bdt.NDD.Frame_Count)
    if(bdt.updatecount)
    {
        if(1 == bdt.PowerOnWithFinger)				//hsqdebug
        {
            bdt.PowerOnWithFinger  = 0;
            bdt.MEM_MIN_XY_Count = 0;
            bdt.BaseChangeFlag++;                /* Counting 1 for baseline updating*/
            for(i = 0;i < XMTR_NUM;i++)
                for(j = 0;j < RECV_NUM;j++)
                {
                    bdt.BFD.BaseDat[i][j]      = buffer[(i*RECV_NUM) + j];
                    bdt.BFD.BaseDatSaved[i][j] = bdt.BFD.BaseDat[i][j];
                }
        }
    }
    
    DataProc_CalculateDeltaData(buffer); /* Extract Delta Data*//* Delta data == Origin data - Baseline data */

    #ifdef PRESS_KEY_DETECT
          #if (KXMTR_NUM == 1)
          for(i=SXMTR_NUM;i<XMTR_NUM;i++)
              for(j=0; j<RECV_NUM; j++)
              {
                bdt.DeltaDat_kp[j]    = bdt.DeltaDat16A[i][j];
                bdt.DeltaDat16A[i][j]=0;
              }
          #endif

          #if (KRECV_NUM == 1)
          for(j=SRECV_NUM; j<RECV_NUM; j++)
              for(i=0;i<XMTR_NUM;i++)
              {
                bdt.DeltaDat_kp[i]    = bdt.DeltaDat16A[i][j];
                bdt.DeltaDat16A[i][j]=0;
              }
          #endif
          DataProc_PressKeyDetect();
    #endif
    
    DataProc_FindMaxAndMinValue();       /* Find Max Value and its Location, also Min Value*/
    bdt.ThresholdInFrame  = DataProc_Ratio2Threshold(bdt.MaxValueInFrame, BE_PERCENT_RATIO);
    
    DataProc_DepressNoise();
    DataProc_FindMaxAndMinValue();       /* Same as the previous one*/
    bdt.ThresholdInFrame  = DataProc_Ratio2Threshold(bdt.MaxValueInFrame, BE_PERCENT_RATIO);

    bdt.PCBA.AbnormalMaxDiff = bdt.MaxValueInFrame>>3;               /* (1/8) of Max Value in the Frame*/
    if(bdt.PCBA.AbnormalMaxDiff < 20)
    {
        bdt.PCBA.AbnormalMaxDiff = 20; 
    }

    /******************************************************
    * Make sure the Max Value in the Frame is big enough
    * Normally it should be 200, if MAX value is smaller
    * than the value, we will not handle the finger PROC
    *******************************************************/
    #ifdef TPD_PROXIMITY
    DataProc_FaceDetectProcess();
    if((bdt.MaxValueInFrame < bdt.PCBA.MaxValueNoFinger) || (FACE_DETECT_NEAR == bdt.FDC.Flag))
    #else    
    if(bdt.MaxValueInFrame < bdt.PCBA.MaxValueNoFinger)
    #endif
    {
        bdt.FingerDetectNum = 0;
        bdt.BFD.bbdc.FingerExist = NO_FINGER;
    }
    else 
    {    
        DataProc_DeltaMatrix2Array(bdt.DeltaBitDat, bdt.ThresholdInFrame); /* mark finger point */
    }


    //if(UTIL_ValidPointsEnoughJudge4DataC(bdt.FingerDetectNum) != 0)  
    if(bdt.FingerDetectNum > MIN_VALUE_POINT) 
    {
        #ifdef PRESS_KEY_DETECT
        #endif

        /***********************************************************
        * We think there maybe one or more fingers detected now
        * 1. Extract a new "bdt.FingerDetectNum"
        * 2. Pull new BitMap array from old array
        ************************************************************/
        DataProc_PullBitmapApart(bdt.DeltaBitDat);
        DataProc_FindExtremum();
        DataProc_ExtractExtremumArea();
        DataProc_ComputePoint();
       
    }
    else
    {
        uint16_t i;
        bdt.FingerDetectNum = 0;
        for(i = 0; i < FINGER_NUM_MAX; i++)
        {
            bdt.DPD[i].Finger_X_XMTR   = 0; /* NO-Finger*/
            bdt.DPD[i].Finger_Y_RECV   = 0; /* No-Finger*/
        }
    }
    
    #if 0 
    if(bdt.CalibCount != 1)
    #endif
    DataProc_HandleFingerInfo();
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void DataProc_WholeFramePostProcess(uint16_t *buffer)
{
    /*******************************************
    * Baseline Processing for every frame
    ********************************************/
    #ifdef BSLN_WATERJUDGE
    uint16_t i,j;
    #endif
    #ifdef FREQHOP_BYSTRETCH
        if(TXSCAN_ENABLE == DataProc_FHBSOperatingSwitch())
        {
            DataProc_FrequencyHopByStretch(buffer);
        }
        else
        {
           Baseline_BaseBufferHandled(buffer);
           if(FINGER_SHOW == bdt.BFD.bbdc.FingerExist)
               DataProc_PowerNoiseLevelJudge();
        }
    #else
        Baseline_BaseBufferHandled(buffer);
        DataProc_PowerNoiseLevelJudge();
    #endif
    
    bdt.MEM_MAX_X_XMTR = bdt.FRM_MAX_X_XMTR;
    bdt.MEM_MAX_Y_RECV = bdt.FRM_MAX_Y_RECV;
    bdt.MEM_MIN_X_XMTR = bdt.FRM_MIN_X_XMTR;
    bdt.MEM_MIN_Y_RECV = bdt.FRM_MIN_Y_RECV;
    
    /********************************************************
    * Output the Frame Data to UART (PC showing)
    *********************************************************/
    #ifdef CN1100_STM32
    #ifdef COMMUNICATION_WITH_PC
    
    #ifdef SHOW_EVERY_FRAME_DATA
    Print_FrameAndDelta_Data(buffer);  // UART working only
    #else
    if(dbg.DebugInfoLevel >= DEBUG_INFO_DELTA)
    {
        Send_Delta_Data_To_PC(buffer);
    }
    #endif
    
    #endif
    #else
    #ifdef CN1100_PRINT
    Print_Select_Data(BufID);
    #else
    spidev->mode |= CN1100_DATA_PREPARED;
    bd->bdt = &bdt;
    wake_up_interruptible(&spidev->waitq);
    Report_Coordinate();
    #endif
    #endif
    
    #ifdef STM32VC_LCD
    LCD_Show_STM32(buffer);
    #endif
    #ifdef BSLN_WATERJUDGE
    if(bdt.BFD.bbdc.FingerExist == NO_FINGER)
    {
         for(i=0;i<XMTR_NUM;i++)
             for (j=0;j<RECV_NUM;j++)
             {
                bdt.BFD.BufDatSaved[i][j] = buffer[ (i*RECV_NUM) + j];
             }
    }// 无指时记录原始值供下帧用
    #endif
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void DataProc_FBFHStartFrameBaseLineInit(uint16_t *buffer)
{
#ifdef SCREEN_NONE_ADAPTIVE
    uint16_t i, j;

        switch(bdt.BFD.InitCount)
        {
            case FRAME_0000:
            {
                break;
            }
            case FRAME_0001:
            {
                /***********************************************
                * Initial Baseline
                ************************************************/
                for (i = 0;i < XMTR_NUM;i++)
                    for (j = 0; j < RECV_NUM;j++)
                    {
                        bdt.BFD.BaseDat[i][j] = buffer[i*RECV_NUM+j];
                    }
                break;
            }
            case FRAME_0002:
            {
                /***********************************************
                * Get The first Baseline
                ************************************************/
                bdt.BFD.TooLongTime4BaseUpdate = MAX_MUST_UPDATE_PERIOD - 20;    /*  Reset the Timing Count*/
                bdt.BFD.AfterBaseUpdatedTime   = MAX_HOLDTIME_AFTERUPDATE - 20;  /*  Reset the Timing Count*/
                for (i = 0;i < XMTR_NUM;i++)
                    for (j = 0; j < RECV_NUM;j++)
                    {
                        bdt.BFD.BaseDat[i][j]      = (bdt.BFD.BaseDat[i][j] + buffer[i*RECV_NUM+j])>>1;
                        bdt.BFD.BaseDatSaved[i][j] = bdt.BFD.BaseDat[i][j];
                    }
                break;
            }
        }
#endif
}

void DataProc_FBFHStartFrameSimpleAdaptive(uint16_t *buffer)
{
#ifdef SCREEN_SIMPLE_ADAPTIVE
    uint16_t i, j;
    uint16_t max = 0,min=4095;
    uint16_t max1=0,min1=4095;
        switch(bdt.BFD.InitCount)
        { 
            case FRAME_0000: 
            {

                break;
            }

            case FRAME_0001:  
            {
                break;
            }
             /* scan TP and config it */
            case FRAME_0002:
            {   
                DataProc_FindSpecialValue(buffer,&max,&min,&max1,&min1);
                DataProc_Screen_Adaptive(max,min);
                break;
            }
            case FRAME_0003:
            {
                break;
            }
            case FRAME_0004:
            {
                break;
            }
            case FRAME_0005:
            {
                #ifdef CHANNEL_ADAPTIVE
            DataProc_FindSpecialValue(buffer,&max,&min,&max1,&min1);            
                DataProc_FindAbnormalPonit(max,min,max1,min1);
                if(bdt.AbnormalPointNum!=0)
                    DataProc_AdjustAbnormalPoint();
                else
                {
                    DataProc_CalTxAverageValue(buffer);
                    DataProc_CalRxAverageValue(buffer);
                    DataProc_FindAbnormalChannel();
                    if((bdt.AbnormalTxChNum!=0)||(bdt.AbnormalRxChNum!=0))
                        DataProc_AdjustAbnormalChannel();
                }
                #endif
                break;
            }
            case FRAME_0006:
            {
                break;
            }
            case FRAME_0007:
            {
                break;
            }
            case FRAME_0008:
            {
                /***********************************************
                *Initial Baseline
                ************************************************/
                for (i = 0;i < XMTR_NUM;i++)
                    for (j = 0; j < RECV_NUM;j++)
                    {
                        bdt.BFD.BaseDat[i][j] = buffer[i*RECV_NUM+j];
                        bdt.BFD.BaseDatSaved[i][j] = buffer[i*RECV_NUM+j];
                    }
                break;
            }
            case FRAME_0009:
            {
                /***********************************************
                *Initial Baseline
                ************************************************/
                bdt.BaseChangeFlag++;                /*  Counting 1 for baseline updating*/
                bdt.BFD.TooLongTime4BaseUpdate = MAX_MUST_UPDATE_PERIOD - 20;    /*  Reset the Timing Count*/
                bdt.BFD.AfterBaseUpdatedTime   = MAX_HOLDTIME_AFTERUPDATE - 20;  /*  Reset the Timing Count*/
                bdt.BFD.bbdc.BaseUpdateCase    = BASELINE_HOLDING_CASE;
                for (i = 0;i < XMTR_NUM;i++)
                    for (j = 0; j < RECV_NUM;j++)
                    {
                        bdt.BFD.BaseDat[i][j]      = (bdt.BFD.BaseDat[i][j]+buffer[i*RECV_NUM+j])>>1;
                        bdt.BFD.BaseDatSaved[i][j] = (bdt.BFD.BaseDatSaved[i][j]+buffer[i*RECV_NUM+j])>>1;
                    }
                break;
            }
            case FRAME_00010:
            {
                break;
            }
        }
#endif
}

void DataProc_FBFHStartFrameFullAdaptive(uint16_t *buffer)
{
#ifdef SCREEN_FULL_ADAPTIVE
    uint16_t i, j;
    #ifdef CHANNEL_ADAPTIVE
    uint16_t max = 0,min=4095;
    uint16_t max1=0,min1=4095;
    #endif
    if(bdt.BFD.InitCount >= 3)                            // 间隔2帧之后才能对buffer进行处理
        {
            /********************************************************
            * 穷举每个ref high和ref low的值
            *********************************************************/
            if(((bdt.BFD.InitCount%3)==0)&&(bdt.PCBA.RefHLSetCount <= CIRCLE_MAXCOUNT))
            {
                DataProc_CalRefHLSetAverageValue(buffer);
                DataProc_CircleScreenAdaptive();
            }
        }
        switch(bdt.BFD.InitCount)
        {
            case 50:
            {
                DataProc_SetRefHLEnd();
                break;
            }
            // wait two frame
            case 53:
            {
                /********************************************************
                * 分别调整每个Rx通道FCAP的值
                *********************************************************/
                #ifdef CHANNEL_ADAPTIVE
                DataProc_CalRxAverageValue(buffer);
                DataProc_AdjustRxChannelFcap();
                #endif
                break;
            }
            // wait two frame
            case 56:
            {
                #ifdef CHANNEL_ADAPTIVE
            DataProc_FindSpecialValue(buffer,&max,&min,&max1,&min1);
                DataProc_FindAbnormalPonit(max,min,max1,min1);
                if(bdt.AbnormalPointNum!=0)
                    DataProc_AdjustAbnormalPoint();
                else
                {
                    DataProc_CalTxAverageValue(buffer);
                    DataProc_CalRxAverageValue(buffer);
                DataProc_FindAbnormalChannel();
                if((bdt.AbnormalTxChNum!=0)||(bdt.AbnormalRxChNum!=0))
                    DataProc_AdjustAbnormalChannel();
            }
                #endif
                break;
            }
            // wait two frame
            case 59:
            {
                /***********************************************
                *Initial Baseline
                ************************************************/
                bdt.BaseChangeFlag++;                /*  Counting 1 for baseline updating*/
                bdt.BFD.TooLongTime4BaseUpdate = MAX_MUST_UPDATE_PERIOD - 20;    /*  Reset the Timing Count*/
                bdt.BFD.AfterBaseUpdatedTime   = MAX_HOLDTIME_AFTERUPDATE - 20;  /*  Reset the Timing Count*/
                bdt.BFD.bbdc.BaseUpdateCase    = BASELINE_HOLDING_CASE;
                for (i = 0;i < XMTR_NUM;i++)
                    for (j = 0; j < RECV_NUM;j++)
                    {
                        bdt.BFD.BaseDat[i][j]      = buffer[i*RECV_NUM+j];
                        bdt.BFD.BaseDatSaved[i][j] = buffer[i*RECV_NUM+j];
                    }
                break;
            }
            // wait two frame
            default:
                break;
            }
#endif
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void FrameBuffer_Full_Handled(uint16_t *buffer)
{
    /*******************************************************
    * Discard the first frame when mode is changed
    * from other mode  to normal mode
    ********************************************************/
    if(BASE_FRAME_DISCARD == bdt.BFD.bbdc.BaseUpdateCase)
    {
        bdt.BFD.bbdc.BaseUpdateCase = BASE_FRAME_HOLD;
        return;
    }
    /********************************************************
    * If there is no finger in the Frame, update Base Frame
    *********************************************************/
    if(bdt.BFD.InitCount < FRAME_MAX)
    {
        #ifdef SCREEN_FULL_ADAPTIVE
        DataProc_FBFHStartFrameFullAdaptive(buffer);
        #endif

        #ifdef SCREEN_SIMPLE_ADAPTIVE
        DataProc_FBFHStartFrameSimpleAdaptive(buffer);
        #endif

        #ifdef SCREEN_NONE_ADAPTIVE
        DataProc_FBFHStartFrameBaseLineInit(buffer);
        #endif
        bdt.BFD.InitCount++;
        bdt.MTD.NoFingerCnt4Doze = 0;
        return;
    }
    else
    {
        /*******************************************
        * Data Processing for Buffer
        ********************************************/
        bdt.BFD.bbdc.FingerExist = FINGER_SHOW;
        DataProc_WholeFrameProcess(buffer);

        if(bdt.BFD.InitCount < 65535) bdt.BFD.InitCount++;
        #ifdef STM32VC_LCD
        #ifdef FHBS_DEBUG_INFOSHOW
        TFT_ShowNum(3,  25, bdt.BFD.InitCount, LCD_COLOR_BLUE, LCD_COLOR_GREEN); 
        TFT_ShowNum(10, 25, (bdt.BFD.InitCount>>5), LCD_COLOR_BLUE, LCD_COLOR_GREEN); 
        TFT_ShowNum(17, 25, (bdt.BFD.InitCount>>11), LCD_COLOR_BLUE, LCD_COLOR_GREEN); 
        #endif
        #endif
    }
    
    /******************************************************
    * 1. Process the base (update) handling
    * 2. Process the frequency hop based on NOISE
    *******************************************************/
    DataProc_WholeFramePostProcess(buffer);
    
    #ifdef CN1100_STM32
    /***********************************************
    * Show LED on GPIOA_1 (D1) when Finger existed
    ************************************************/
    if(bdt.FingerDetectNum != 0) 
    {
        STM32_LEDx_TURN_ONorOFF(LED1, LED_ON);
    }
    else
    {
        STM32_LEDx_TURN_ONorOFF(LED1, LED_OFF);
    }
    #endif
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void BufferAHandle(void)
{
    uint16_t *buffer;
    
    buffer = (uint16_t *)(bdt.FrameDatLoadA);
    /*********************************************
    * Read data from Buffer A
    **********************************************/
    #ifdef ONE_MORE_LINE_SCAN
    SPI_read_DATAs(0x400+RECV_NUM, XMTR_NUM*RECV_NUM, buffer); /*  Read data to Buffer A*/
    #else
    SPI_read_DATAs(0x400, XMTR_NUM*RECV_NUM, buffer); /*  Read data to Buffer A*/
    #endif

    FrameBuffer_Full_Handled(buffer);
    bdt.BSDSTS.iBuf_A_Fill = FRAME_UNFILLED;
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void BufferBHandle(void)
{
    uint16_t *buffer;
    
    buffer = (uint16_t *)(bdt.FrameDatLoadB);
    /*********************************************
    * Read data from Buffer B
    **********************************************/
    #ifdef ONE_MORE_LINE_SCAN
    SPI_read_DATAs(0x400+RECV_NUM+XMTR_NUM*RECV_NUM+RECV_NUM, XMTR_NUM*RECV_NUM, buffer); /*  Read data to Buffer A*/
    #else
    SPI_read_DATAs(0x400+XMTR_NUM*RECV_NUM, XMTR_NUM*RECV_NUM, buffer); /*  Read data to Buffer B*/
    #endif

    FrameBuffer_Full_Handled(buffer);
    bdt.BSDSTS.iBuf_B_Fill = FRAME_UNFILLED;
}
#endif
