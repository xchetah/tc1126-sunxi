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



#ifndef CN1100_HWSERVICE_C
#define  CN1100_HWSERVICE_C


#include "include/CN1100_common.h"
#include "include/CN1100_linux.h"
#include "include/CN1100_Function.h"
extern RegTab RegTab_t;
/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void TC1126_Init_StartADCByReg21(void)
{    
   //R02 -a
    RegTab_t.Reg21BitDef_t.AdcmRegConf = 0;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_ADC_SPEED = ADC_SPEED_SET;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_ACS = ACS_SPEED_SET;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_SHRT_CKT_EN = 1;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_TIMING_EN = 1;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_MB_EN = 1;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_ACTV_CONF = 1; 
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_XMTR_STR = XMTR_STRENGTH_SET;
    SPI_write_singleData(ADCM_REG,RegTab_t.Reg21BitDef_t.AdcmRegConf); 

    
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_SHRT_CKT_EN = 0;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_ACTV_CONF = 0; 
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_XMTR_STR_ENB=1;
    SPI_write_singleData(ADCM_REG,RegTab_t.Reg21BitDef_t.AdcmRegConf); 
    //R02 -e

    SPI_write_singleData(FLAG_REG,  0x001f);
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void TC1126_Init_StartADCAtBegin(void)
{
 /*******************************************************
    * Start the Burst and ADC
    ******************************************************* */
    switch(bdt.ModeSelect)
    {
        case iAUTOSCAN_MODE:
        case SLEEP_MODE:
        case DOZE_MODE:
        {
            TC1126_Init_StartADCByReg21();
            break;
        }
    }

    /*******************************************************
    * Setupthe first burst flag
    ********************************************************/
    bdt.BSDSTS.First_Burst = 0;              // Useless now
    bdt.BSDSTS.Buff_Select = BUFFER_A;       // Useless now
    bdt.BSDSTS.Buf_A_Ready = DATA_UNREADY;   // Useless now
    bdt.BSDSTS.Buf_B_Ready = DATA_UNREADY;   // Useless now
    bdt.BSDSTS.ADC_Release = ADC_RELEASED;   // Useless now
    bdt.BSDSTS.iBuf_A_Fill = FRAME_UNFILLED; // Useful
    bdt.BSDSTS.iBuf_B_Fill = FRAME_UNFILLED; // Useful
    bdt.BSDSTS.Buf_A_Pring = 0;              // Useless now
    bdt.BSDSTS.Buf_B_Pring = 0;              // Useless now
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void TC1126_Init_HardwareRegs(void)
{

    TC1126_Init_AllRegisters();

    
    #if 0
    {
        uint16_t  i, tempa;
        for(i=OSCC_REG; i<=FCTL_REG; i++)
        {
            if((i!=0x2E) && (i!=0x30) )
            {
                tempa = SPI_read_singleData(i);
                CN1100_print("REG(%x) = 0x%x\n", i, tempa);
            }
        }
        while(1);
    }
    #endif
    
    CN1100_print("===\n\n\n");
    TC1126_Init_StartADCAtBegin();    


    
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void TC1126_Init_GlobalVariables(void)
{
    uint16_t i, j;
    
    //bdt.XmtrDriveOut    = 1<<XMTR_STR;     /*  XTMR Input Signal Switch (1^Index) */
    //bdt.XmtrDriveCount  = 0;   /*  XTMR input switch index (0-27) */
    //bdt.int_wait        = 0;   /*  We may read data once every Several Interrupts; */
    
    bdt.BSDSTS.First_Burst = 0;              // Useless now
    bdt.BSDSTS.Buff_Select = BUFFER_A;       // Useless now
    bdt.BSDSTS.Buf_A_Ready = DATA_UNREADY;   // Useless now
    bdt.BSDSTS.Buf_B_Ready = DATA_UNREADY;   // Useless now
    bdt.BSDSTS.ADC_Release = ADC_RELEASED;   // Useless now
    bdt.BSDSTS.iBuf_A_Fill = FRAME_UNFILLED; // Useful
    bdt.BSDSTS.iBuf_B_Fill = FRAME_UNFILLED; // Useful
    bdt.BSDSTS.Buf_A_Pring = 0;              // Useless now
    bdt.BSDSTS.Buf_B_Pring = 0;              // Useless now

    bdt.MEM_MAX_XY_Count   = 0;
    bdt.MEM_MAX_X_XMTR     = XMTR_NUM;
    bdt.MEM_MAX_Y_RECV     = RECV_NUM;
    
    bdt.MEM_MIN_XY_Count   = 0;
    bdt.MEM_MIN_X_XMTR     = XMTR_NUM;
    bdt.MEM_MIN_Y_RECV     = RECV_NUM;
    bdt.PowerOnWithFinger  = 0;
    
    #ifdef CN1100_iSCANMODE
    bdt.ModeSelect      = iAUTOSCAN_MODE;
    #endif
    
    #ifdef CN1100_SLEEPMODE
    bdt.ModeSelect      = SLEEP_MODE;
    #endif
    
    #ifdef CN1100_DOZEMODE
    bdt.ModeSelect      = DOZE_MODE;
    #endif
    
    for (i = 0; i < XMTR_NUM; i++)
    for (j = 0; j < RECV_NUM; j++)
    {
        bdt.FrameDatLoadA[i][j]    = 0;
        bdt.FrameDatLoadB[i][j]    = 0;
        bdt.BFD.BaseDat[i][j]      = 0;
        bdt.BFD.BaseDatSaved[i][j] = 0;
        bdt.BFD.SumBaseDat[i][j]   = 0;
        bdt.FrameDataSave[i][j]    = 0;
    }

    #ifdef PRESS_KEY_DETECT

    #if (KXMTR_NUM == 1)
    for (j = 0; j < RECV_NUM; j++)
    #endif

    #if (KRECV_NUM == 1)
    for (j = 0; j < XMTR_NUM; j++)
    #endif
    {
        bdt.DeltaDat_kp[j] = 0;
    }
    #endif

    bdt.BFD.SettledPointExistTime  = 0;    
    bdt.BFD.AdjustCount            = 0;
    bdt.BFD.AbnormalUpdateDelay    = 0;
    bdt.BFD.InitCount              = 0;   /*  count for saving basedata; */
    bdt.BFD.bbdc.FingerExist       = NO_FINGER;
    bdt.BFD.bbdc.PreFingerExt      = FINGER_SHOW;
    bdt.BFD.bbdc.NoFingerCnt4Base  = 0;
    bdt.BFD.bbdc.BaseUpdateCase    = BASELINE_UPDATING_CASE;
    #ifdef BSLN_WATERJUDGE
    bdt.BFD.WaterProtectTime       = 0;
    bdt.BFD.BufDeltSum             = 0;                          //两帧无指原始值的差值
    bdt.BFD.BufDeltMax             = 0;   
    bdt.BFD.BufAbnomalflag         = 0;
    bdt.BFD.BufAbnomalProtectflag  = 0;
    for (i = 0; i < XMTR_NUM; i++)
    for (j = 0; j < RECV_NUM; j++)
        bdt.BFD.BufDatSaved[i][j]  = 0;
    #endif
    bdt.MTD.mtdc.Doze_FirstIn      = 0;
    bdt.MTD.mtdc.Doze_OddNum       = 0;
    bdt.MTD.NoFingerCnt4Doze       = 0;

    bdt.MaxValueInFrame            = 0;
    bdt.ThrLow4DistanceFilter      = THR072;
    bdt.ThrHigh4DistanceFilter     = THR144;
    
    
    bdt.NDD.Noise_Sum     = 0;
    bdt.NDD.Noise_Count   = 0;
    bdt.NDD.Frame_Count   = 0;
    bdt.NDD.Battery_Level = 0;
    
    #ifdef COMMUNICATION_WITH_PC
    dbg.DebugInfoLevel    = DEBUG_INFO_NONE;
    dbg.validCommand      = USART_RECEIVED_CMD;
    dbg.UartParaNum       = 0;
    dbg.debug_value_num   = DEBUG_VALUE_NUM_MAX;
    for (i = 0; i < DEBUG_VALUE_NUM_MAX; i++) 
    {
        dbg.debug_value[i] = 0;
    }
    #ifdef USB_COMMUNICATION
    dbg.USB_ReceiveFlg = FALSE;
    dbg.TimeCount = 0;
    dbg.USB_dataflag = USB_DATA_UNREADY;
    dbg.TbufCount    = 0;
    dbg.RowCount     = 0;
    #endif
    #endif
    
    #ifdef TPD_PROXIMITY
    bdt.FDC.Flag             = 0;       // 
    bdt.FDC.WFNum            = 0;       // 
    bdt.FDC.BigNum           = 0;       // 
    bdt.FaceDetectDelay      = 0;
    #endif
    bdt.PrevFingerDetectNum  = 0;
    bdt.PrevFingerDetectNum2 = 0;

    bdt.MaxNoise_Sum = 0;
    bdt.MaxNoise_SumCount = 0;
    bdt.Noise_Sum = 0;
    bdt.updatecount = 350;


    for (i = 0; i < FINGER_NUM_MAX;i++)
    {
        bdt.DPD[i].FingerStateFlag      = 0;    /*  No Finger */
        bdt.DPD[i].FingerStateFlagEdge  = 0;
        bdt.DPD[i].JustPassStateFlag4   = 0;
        bdt.DPD[i].AdjustDistance       = FINGER_ADJUST_DISTANCE_MAX;
        bdt.DPD[i].AdjustState          = STATE_STICK_START;
        bdt.DPD[i].AdjStickCounter      = 0;
        bdt.DPD[i].AdjustOrigin_x       = 0;
        bdt.DPD[i].AdjustOrigin_y       = 0;

        bdt.DPD[i].StayCount            = 0;
        bdt.DPD[i].Stay_XSum            = 0;
        bdt.DPD[i].Stay_YSum            = 0;

#ifdef SUPER_FILTER4EDGE
        bdt.DPD[i].EdgeShift_LR         = 0;
        bdt.DPD[i].EdgeOffset_LR        = 64;
        bdt.DPD[i].EdgeShift_TB         = 0;
        bdt.DPD[i].EdgeOffset_TB        = 64;
        bdt.DPD[i].FingerRealNum1_X     = 0;
        bdt.DPD[i].FingerRealNum2_X     = 0;
        bdt.DPD[i].FingerRealNum1_Y     = 0;
        bdt.DPD[i].FingerRealNum2_Y     = 0;
        bdt.DPD[i].FingMovingSpeed      = FINGER_FINGER_SPEED_LOW;
#endif        
        for (j = 0;j < FINGER_INFO_SAVE_NUM;j++)
        {
            bdt.DPD[i].Prev_Finger_X[j]  = 0;
            bdt.DPD[i].Prev_Finger_Y[j]  = 0;
        }
        
        bdt.DPD[i].EdgeInfo.EdgeNum = 0;
        bdt.DPD[i].EdgeInfo.PrevEdgeNum = 0;

        #ifdef FROMOUT2IN_INORDER
        for (j = 0;j < 3; j++)
        {
            bdt.DPD[i].Finger_X_Erpt[j]  = 0; 
            bdt.DPD[i].Finger_Y_Erpt[j]  = 0;
            bdt.DPD[i].Finger_X_Erpt1[j] = 0;
            bdt.DPD[i].Finger_Y_Erpt1[j] = 0;
        }
        #endif        
        
        bdt.DPD[i].Point_Hold        = 0;
        bdt.DPD[i].LifeNumber        = 0;
        bdt.DPD[i].Finger_X_Reported = 0;
        bdt.DPD[i].Finger_Y_Reported = 0;
        bdt.DPD[i].Finger_X_Raw      = 0;
        bdt.DPD[i].Finger_Y_Raw      = 0;

        #ifdef RAWPOINT4FINGMATCH
        bdt.DPD[i].Finger_XRAW[0] = 0;
        bdt.DPD[i].Finger_XRAW[1] = 0;
        bdt.DPD[i].Finger_XRAW[2] = 0;
        bdt.DPD[i].Finger_YRAW[0] = 0;
        bdt.DPD[i].Finger_YRAW[1] = 0;
        bdt.DPD[i].Finger_YRAW[2] = 0;
        #endif


    }
    
    /******************************************
    * bdt.BFD.ProtectTimeCount = PROTECT_TIME;
    ****************************************** */
    bdt.BFD.FingerLeftProtectTime = PROTECT_TIME;
    bdt.BFD.AfterBaseUpdatedTime  = 0;
    bdt.PressKeyFlag1             = NO_KEY_PRESSED;
    bdt.BFD.DeltaSum              = 0;
    bdt.BFD.DeltaSumMaxThreshold  = XMTR_NUM*RECV_NUM*10; /* 0 -0xfff; */  
    bdt.BaseChangeFlag            = 0;
    bdt.ThresholdPara             = THRESHOLD_PARA_MAX;
    bdt.FingerRealNum             = 0;
    bdt.FingerRealNum1            = 0;

    bdt.LFrame_NUM                = 0;

    for (i = 0; i < FINGER_NUM_MAX;i++)
    {
        bdt.LFrame_X_XMTR[i] = XMTR_NUM;
        bdt.LFrame_Y_RECV[i] = RECV_NUM;
    }
    
    #ifdef FREQHOP_BYSTRETCH
    bdt.NoiseDataFrFrame      = 0;
    bdt.SumNoiseDataFrFrame   = 0;
    bdt.TxScanNoiseCount      = 0;
    bdt.StretchValue          = STRETCH_STNUM;
    bdt.StretchInReg          = STRETCH_STNUM; // No StretchInReg could be STRETCH_STNUM later
    for (i = 0; i < 15;i++)
    {
        bdt.NoiseDataTable[i] = 0;
    }
    bdt.FingerDelay = 0;
    bdt.TxScanValid = 0;
    bdt.CurNoiseThrHi = 0;
    bdt.CurNoiseThrLo = 0;
    bdt.CurNoiseSum = 0;
    #endif
    
    bdt.PCBA.ProtectTime           = PROTECT_TIME;
    bdt.PCBA.FrameMaxSample        = FRAME_MAX_SAMPLE;
    bdt.PCBA.AbnormalMaxDiff       = ABNORMAL_MAX_DIFF;
    bdt.PCBA.AbnormalNumDiff       = ABNORMAL_NUM_DIFF;
    //bdt.PCBA.FixPointMaxTime       = FIX_POINT_MAX_TIME;    // useless now(QFU)
    bdt.PCBA.MaxUpdateTime         = MAX_MUST_UPDATE_PERIOD;
    
    bdt.PCBA.HighRefSet            = HIREF_SETTING;
    bdt.PCBA.LowRefSet             = LOREF_SETTING;
    bdt.PCBA.HighRefGainSet        = HIREF_GAIN_SET;
    bdt.PCBA.LowRefGainSet         = LOREF_GAIN_SET;
    bdt.PCBA.HighRefPlSet          = HIREF_PL_SET;
    bdt.PCBA.LowRefPlSet           = LOREF_PL_SET;
    
    #ifdef SCREEN_FULL_ADAPTIVE
    bdt.PCBA.HighRefGainSet        = 0;
    bdt.PCBA.LowRefGainSet         = 0;
    bdt.PCBA.RefHLSetCount         = 0;
    bdt.PCBA.RefHLSetEndFlag       = 0;
    for(i=0; i<CIRCLE_MAXCOUNT; i++)
        bdt.PCBA.RefHLSetAve[i]    = 0;
    #endif
    
    bdt.PCBA.MaxValueNoFinger      = MAX_VAL_NON_FINGER;
    bdt.PCBA.FinThrMin             = ROUGH_FINGER_THR;
    
    bdt.PCBA.RcvmRcvrFcapSet       = RCVM_RCVR_FCAP_SET;
    
    bdt.Prepare2SleepMode = 0;
    bdt.ClockCount4ANY    = 0;
    
    for (i = 0;i < XMTR_NUM;i++)
    {
        bdt.AveDeltaXmtr[i] = 0;
    }
    
    for ( i = 0;i < FINGER_NUM_MAX;i++)
    {
        bdt.DPD[i].DataInvalidFlag = 0;       /*  Default we think all data is valid */
        for (j = 0;j < FINGER_NUM_MAX;j++)
        {
            bdt.DPD[i].Extremum_Pos_X[j] = 0;
            bdt.DPD[i].Extremum_Pos_Y[j] = 0;
        }
    }
    

    for(j=0; j<RECV_NUM; j++)
    {
        bdt.RxFcapValue[j] = bdt.PCBA.RcvmRcvrFcapSet;
    }

    #ifdef SHOW_H_DATA
    bdt.Left_h   = 0;
    bdt.Right_h  = 0;
    bdt.Top_h    = 0;
    bdt.Bottom_h = 0;
    #endif
    #ifdef CHANNEL_ADAPTIVE
    bdt.AllAveValue = 0;
    bdt.AbnormalTxChNum = 0;
    bdt.AbnormalRxChNum = 0;
    bdt.AdjustRxChFlag = 0;
    for(i=0; i<SXMTR_NUM; i++)
    {
        bdt.TxAveValue[i] = 0;        
        bdt.TxAbnormalCh[i] = -1;    //不能初始化为0，因为我们需要0通道
    }
    for(j=0; j<SRECV_NUM; j++)
    {
        bdt.RxAveValue[j] = 0;
        bdt.RxAbnormalCh[j] = -1;
    }
    for(i=0; i<ABCHANDPOINT_MAXNUM; i++)
    {
        bdt.RealAbnormalTxCh[i] = -1;
        bdt.RealAbnormalRxCh[i] = -1;
    }
    bdt.FindOkFlag = 0;
    bdt.AbnormalPointNum = 0;
    bdt.AbPointTxiCoord = -1;
    bdt.AbPointRxiCoord = -1;
    bdt.AbPointTxjCoord = -1;
    bdt.AbPointRxjCoord = -1;
    bdt.TxRxiFCAP = 0;
    bdt.TxRxjFCAP = 0;
    for(i=0; i<ABNORMALPOINT_MAXNUM; i++)
    {
        bdt.AbnormalPoint[i] = 0;
    }
    #endif
    bdt.FingerReqNum = FINGER_REQUIREMENT_NUM;   //R01 -a
}

/******************************************************************************
* We always set "ONE_MORE_LINE_SCAN" in the array first
******************************************************************************/
#if (defined(PHONESCREEN_13X10) || defined(SH_FEATUREPHONE_13X9))
uint8_t  const XmtrOrder[XMTR_NUM+1] = {SCN_T0,SCN_T0,SCN_T1,SCN_T2,SCN_T3,SCN_T4,SCN_T5,SCN_T6,SCN_T7,SCN_T8,SCN_T9,SCN_T10,SCN_T11,SCN_T12};
#elif (defined(FPCSCREEN_17X10) )
uint8_t  const XmtrOrder[XMTR_NUM+1] = {SCN_T0,SCN_T0,SCN_T1,SCN_T2,SCN_T3,SCN_T4,SCN_T5,SCN_T6,SCN_T7,SCN_T8,SCN_T9,SCN_T10,SCN_T11,SCN_T12,SCN_T13,SCN_T14,SCN_T15,SCN_T16};
#else
uint8_t  const XmtrOrder[XMTR_NUM+1] = {SCN_T0,SCN_T0,SCN_T1,SCN_T2,SCN_T3,SCN_T4,SCN_T5,SCN_T6,SCN_T7,SCN_T8,SCN_T9,SCN_T10,SCN_T11,SCN_T12,SCN_T13,SCN_T14};
#endif


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void TC1126_Init_TxMappingRegisters(void)
{
    uint16_t i;
    uint16_t TX0TO15_ENABLE_FLAG = 0;
    uint16_t TX16_ENABLE_FLAG    = 0;
    uint16_t TX0TO15_PLFLAG      = 0;
    uint16_t TX16_PLFLAG         = 0;

#ifdef ONE_MORE_LINE_SCAN
    for (i = 0;i <= XMTR_NUM;i++)
    {
        if(XmtrOrder[i] < 16)
        {
            SPI_write_singleData(TXMAP_REG(i),TXMAP_TX_CH(XmtrOrder[i]));
            TX0TO15_ENABLE_FLAG |= (1<<XmtrOrder[i]);
            if(1 == TX_DRIVE_PL) TX0TO15_PLFLAG |= (1<<XmtrOrder[i]);
        }
        else if(16 == XmtrOrder[i])
        {
            SPI_write_singleData(TXMAPTOTX16ONLY_REG, TXMAPTO16O_Bit(i));
            SPI_write_singleData(TXMAP_REG(i),0);
            //R02 -a
            RegTab_t.Reg2DBitDef_t.Rmp3RegConf = 0;
            RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_TX_EN16 = 1;
            TX16_ENABLE_FLAG |= RegTab_t.Reg2DBitDef_t.Rmp3RegConf;
            if(1 == TX_DRIVE_PL) 
            {
                RegTab_t.Reg2DBitDef_t.Rmp3RegConf = 0;
                RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_TX_PL16 =1;
                TX16_PLFLAG |= RegTab_t.Reg2DBitDef_t.Rmp3RegConf;
            }
            //R02 -e
        }
    }
#else
    for (i = 0;i < XMTR_NUM;i++)
    {
        if(XmtrOrder[i+1] < 16)
        {
            SPI_write_singleData(TXMAP_REG(i),TXMAP_TX_CH(XmtrOrder[i+1]));
            TX0TO15_ENABLE_FLAG |= (1<<XmtrOrder[i+1]);
            if(1 == TX_DRIVE_PL) TX0TO15_PLFLAG |= (1<<XmtrOrder[i+1]);
        }
        else if(16 == XmtrOrder[i+1])
        {
            SPI_write_singleData(TXMAPTOTX16ONLY_REG, TXMAPTO16O_Bit(i));
            SPI_write_singleData(TXMAP_REG(i),0);
            //R02 -a
            RegTab_t.Reg2DBitDef_t.Rmp3RegConf = 0;
            RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_TX_EN16 = 1;
            TX16_ENABLE_FLAG |= RegTab_t.Reg2DBitDef_t.Rmp3RegConf;
            if(1 == TX_DRIVE_PL)
            {
                RegTab_t.Reg2DBitDef_t.Rmp3RegConf = 0;
                RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_TX_PL16 = 1;
                TX16_PLFLAG |= RegTab_t.Reg2DBitDef_t.Rmp3RegConf;
            }
            //R02 -e
        }
    }
#endif    
    /********************************************************
    * Setup the polority of the XMTR OUTPUT pin
    ******************************************************** */
    SPI_write_singleData(TXEN_REG, TX0TO15_ENABLE_FLAG);       /*  0-11 */
    SPI_write_singleData(TPL1_REG, TX0TO15_PLFLAG); /*  cfg_reg30, 12'h000 */
    //R02 -a
    RegTab_t.Reg2DBitDef_t.Rmp3RegConf = 0;
    RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_CH_MAP8 =SCN_R8;
    RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_CH_MAP9 =SCN_R9;
    SPI_write_singleData(RMP3_REG, TX16_PLFLAG|TX16_ENABLE_FLAG|RegTab_t.Reg2DBitDef_t.Rmp3RegConf);

 #ifdef DOZE_ALLOWED 
    RegTab_t.Reg24BitDef_t.DurvRegConf = 0;
    RegTab_t.Reg24BitDef_t.DurvReg_t.DURV_RESET_DUR = DUR_RESET;
    RegTab_t.Reg24BitDef_t.DurvReg_t.DURV_INTEG_DUR = DUR_INTEG;
    SPI_write_singleData(DURV_REG, RegTab_t.Reg24BitDef_t.DurvRegConf); /*  (QFU) It needs more DEBUG */
  #endif
//R02 -e
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void TC1126_Init_TxMappingRegisters4SLEEP(void)
{
    uint16_t i;
    uint16_t TX0_MAPPING         = 0;
    uint16_t TX1_MAPPING         = 0;
    uint16_t TX0To15_PLFLAG      = 0;
    uint16_t TX16_PLFLAG         = 0;
    uint16_t TX0To15_EN          = 0;
    uint16_t TX16_EN             = 0;

    #if (DOZE_TX_DRIVE_PL == TX_DRIVE_NORM)
    uint32_t Sleep_TxPolarity    = 0; /*  TEMP setting HERE(QFU) */
    #endif

    #if (DOZE_TX_DRIVE_PL == TX_DRIVE_INVS)
    uint32_t Sleep_TxPolarity    = 1; /*  TEMP setting HERE(QFU) */
    #endif

    #if (2 == DOZE_TXREADNUM)
    /***********************************************
    * 1. Enable setting is not changed;
    * 2. Mapping setting should be changed;
    * 3. Polarity should be changed (maybe);
    *********************************************** */
    for (i = 0; i < ((XMTR_NUM>>1)+0); i++)
    {
        if(XmtrOrder[i+1] < 16)
        {
            TX0_MAPPING |= (1<<XmtrOrder[i+1]);
            TX0To15_EN  |= (1<<XmtrOrder[i+1]);
            if(1 == Sleep_TxPolarity)
            {    
                TX0To15_PLFLAG |= (1<<XmtrOrder[i+1]);
            }  
        }
        else if(16 == XmtrOrder[i+1])
        {
            SPI_write_singleData(TXMAPTOTX16ONLY_REG, (1<<0)); /*  SCN16 to T0 */
            //R02 -a
            RegTab_t.Reg2DBitDef_t.Rmp3RegConf = 0;
            RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_TX_EN16 = 1;
            TX16_EN  |= RegTab_t.Reg2DBitDef_t.Rmp3RegConf;
            if(1 == Sleep_TxPolarity) 
            {
                RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_TX_EN16 = 0;
                RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_TX_PL16 = 1;
                TX16_PLFLAG |= RegTab_t.Reg2DBitDef_t.Rmp3RegConf;
                //R02 -e
            }
        }
    }
    SPI_write_singleData(TXMAP0_REG, TX0_MAPPING);           /*  T0 */
    
    for (i = ((XMTR_NUM>>1)+1); i < XMTR_NUM; i++)
    {
        if(XmtrOrder[i+1] < 16)
        {
            TX1_MAPPING |= (1<<XmtrOrder[i+1]);
            TX0To15_EN  |= (1<<XmtrOrder[i+1]);
            if(1 == Sleep_TxPolarity) TX0To15_PLFLAG |= (1<<XmtrOrder[i+1]);
        }
        else if(16 == XmtrOrder[i+1])
        {
            SPI_write_singleData(TXMAPTOTX16ONLY_REG, (1<<1)); /*  SCN16 to T1 */
            //R02 -a
            RegTab_t.Reg2DBitDef_t.Rmp3RegConf = 0;
            RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_TX_EN16 = 1;
            TX16_EN  |= RegTab_t.Reg2DBitDef_t.Rmp3RegConf;
            if(1 == Sleep_TxPolarity) 
            {  
                RegTab_t.Reg2DBitDef_t.Rmp3RegConf = 0;
                RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_TX_PL16 = 1;
                TX16_PLFLAG |= RegTab_t.Reg2DBitDef_t.Rmp3RegConf ;
            }
            //R02 -e
        }
    }
    SPI_write_singleData(TXMAP1_REG, TX1_MAPPING);           /*  T1 */
    
    SPI_write_singleData(TXEN_REG,   TX0To15_EN);            /*  EN for T0 and T15 */
    SPI_write_singleData(TPL1_REG,   TX0To15_PLFLAG);        /*  PL for T0 and T15 */
    //R02 -a
    RegTab_t.Reg2DBitDef_t.Rmp3RegConf = 0;
    RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_CH_MAP8 =SCN_R8;
    RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_CH_MAP9 =SCN_R9;
    SPI_write_singleData(RMP3_REG,   TX16_PLFLAG|TX16_EN|RegTab_t.Reg2DBitDef_t.Rmp3RegConf);
    RegTab_t.Reg24BitDef_t.DurvRegConf = 0;
    RegTab_t.Reg24BitDef_t.DurvReg_t.DURV_RESET_DUR = DUR_RESET;
    RegTab_t.Reg24BitDef_t.DurvReg_t.DURV_INTEG_DUR = 60;
    SPI_write_singleData(DURV_REG,RegTab_t.Reg24BitDef_t.DurvRegConf);/*  (QFU) It needs more DEBUG */
    #endif
    //R02 -e 
    #if (3 == DOZE_TXREADNUM)
    for (i = 0; i < 5; i++)
    {
        if(XmtrOrder[i+1] < 16)
            {
            TX0_MAPPING |= (1<<XmtrOrder[i+1]);
            TX0To15_EN  |= (1<<XmtrOrder[i+1]);
            if(1 == Sleep_TxPolarity)
            {
                TX0To15_PLFLAG |= (1<<XmtrOrder[i+1]);
            }
        }
        else if(16 == XmtrOrder[i+1])
        {
            SPI_write_singleData(TXMAPTOTX16ONLY_REG, (1<<0)); /*  SCN16 to T0 */
            //R02 -a
            RegTab_t.Reg2DBitDef_t.Rmp3RegConf = 0;
            RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_TX_EN16 = 1;
            TX16_EN  |= RegTab_t.Reg2DBitDef_t.Rmp3RegConf;
            if(1 == Sleep_TxPolarity)
            {
                RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_TX_EN16 = 0;
                RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_TX_PL16 = 1;
                TX16_PLFLAG |= RegTab_t.Reg2DBitDef_t.Rmp3RegConf;
                //R02 -e
            }
        }
    }
    SPI_write_singleData(TXMAP0_REG, TX0_MAPPING);           /*  T0 */
    
    for (i = 5; i < 10; i++)
    {
        if(XmtrOrder[i+1] < 16)
        {
            TX1_MAPPING |= (1<<XmtrOrder[i+1]);
            TX0To15_EN  |= (1<<XmtrOrder[i+1]);
            if(1 == Sleep_TxPolarity) 
            {
                TX0To15_PLFLAG |= (1<<XmtrOrder[i+1]);
            }
        }
        else if(16 == XmtrOrder[i+1])
        {
            SPI_write_singleData(TXMAPTOTX16ONLY_REG, (1<<1)); /*  SCN16 to T1 */
            //R02 -a
            RegTab_t.Reg2DBitDef_t.Rmp3RegConf = 0;
            RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_TX_EN16 = 1;
            TX16_EN  |= RegTab_t.Reg2DBitDef_t.Rmp3RegConf;
            if(1 == Sleep_TxPolarity)
            {
                RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_TX_EN16 = 0;
                RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_TX_PL16 = 1;
                TX16_PLFLAG |= RegTab_t.Reg2DBitDef_t.Rmp3RegConf;
                //R02 -e
            }
        }
    }
    SPI_write_singleData(TXMAP1_REG, TX1_MAPPING);           /*  T1 */
    
    for(i = 10; i < XMTR_NUM; i++)
    {
        if(XmtrOrder[i+1] < 16)
        {
            TX0_MAPPING |= (1<<XmtrOrder[i+1]);
            TX0To15_EN  |= (1<<XmtrOrder[i+1]);
            if(1 == Sleep_TxPolarity)
            {
                TX0To15_PLFLAG |= (1<<XmtrOrder[i+1]);
            }
        }
        else if(16 == XmtrOrder[i+1])
        {
            SPI_write_singleData(TXMAPTOTX16ONLY_REG, (1<<2)); /*  SCN16 to T0 */
            //R02 -a
            RegTab_t.Reg2DBitDef_t.Rmp3RegConf = 0;
            RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_TX_EN16 = 1;
            TX16_EN  |= RegTab_t.Reg2DBitDef_t.Rmp3RegConf;
            if(1 == Sleep_TxPolarity)
            {
                RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_TX_EN16 = 0;
                RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_TX_PL16 = 1;
                TX16_PLFLAG |= RegTab_t.Reg2DBitDef_t.Rmp3RegConf;
                //R02 -e
            }
        }
    }
    SPI_write_singleData(TXMAP2_REG, TX0_MAPPING);           /*  repeat to use VAR "TX0_MAPPING" */
    
    SPI_write_singleData(TXEN_REG,   TX0To15_EN);            /*  EN for T0 and T15 */
    SPI_write_singleData(TPL1_REG,   TX0To15_PLFLAG);        /*  PL for T0 and T15 */
    //R02 -a
    RegTab_t.Reg2DBitDef_t.Rmp3RegConf = 0;
    RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_CH_MAP8 =SCN_R8;
    RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_CH_MAP9 =SCN_R9;
    SPI_write_singleData(RMP3_REG,   TX16_PLFLAG|TX16_EN|RegTab_t.Reg2DBitDef_t.Rmp3RegConf);
    RegTab_t.Reg24BitDef_t.DurvRegConf = 0;
    RegTab_t.Reg24BitDef_t.DurvReg_t.DURV_RESET_DUR = DUR_RESET;
    RegTab_t.Reg24BitDef_t.DurvReg_t.DURV_INTEG_DUR = 48;
    SPI_write_singleData(DURV_REG, RegTab_t.Reg24BitDef_t.DurvRegConf); /*  (QFU) It needs more DEBUG */
    #endif
    //R02 -e
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void TC1126_Init_RefHLRegWRITE(void)
{
    //R02 -a
    RegTab_t.Reg28BitDef_t.RefhRegConf = 0;
    RegTab_t.Reg28BitDef_t.RcvmReg_t.REFH_REFHI_EN   = TCM_ENABLE;
    RegTab_t.Reg28BitDef_t.RcvmReg_t.REFH_REFHI_INP  = TCM_ENABLE;
    RegTab_t.Reg28BitDef_t.RcvmReg_t.REFH_REFHI_TCAP = bdt.PCBA.HighRefSet;
    RegTab_t.Reg28BitDef_t.RcvmReg_t.REFH_REFHI_FCAP = bdt.PCBA.HighRefGainSet;
    RegTab_t.Reg28BitDef_t.RcvmReg_t.REFH_REFHI_PL   = bdt.PCBA.HighRefPlSet;
    #ifdef COEF_SCALE_ENABLE
    if(iAUTOSCAN_MODE == bdt.ModeSelect)
    {
        RegTab_t.Reg28BitDef_t.RcvmReg_t.REFH_SCALE_EN = TCM_ENABLE;
        RegTab_t.Reg28BitDef_t.RcvmReg_t.REFH_SCALE_MODE= SCALE_MODE_SELECT;
    }
    #endif

    RegTab_t.Reg29BitDef_t.ReflRegConf = 0;
    RegTab_t.Reg29BitDef_t.RcvmReg_t.REFL_REFLO_EN   = TCM_ENABLE;
    RegTab_t.Reg29BitDef_t.RcvmReg_t.REFL_REFLO_INP  = TCM_ENABLE;
    RegTab_t.Reg29BitDef_t.RcvmReg_t.REFL_REFLO_TCAP = bdt.PCBA.LowRefSet;
    RegTab_t.Reg29BitDef_t.RcvmReg_t.REFL_REFLO_FCAP = bdt.PCBA.LowRefGainSet;
    RegTab_t.Reg29BitDef_t.RcvmReg_t.REFL_REFLO_PL   = bdt.PCBA.LowRefPlSet;
    
    //R02 -e
    SPI_write_singleData(REFH_REG, RegTab_t.Reg28BitDef_t.RefhRegConf);
    SPI_write_singleData(REFL_REG, RegTab_t.Reg29BitDef_t.ReflRegConf); 
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void TC1126_Init_CapHighLowSetting(void)
{
    #if 1
    //bdt.PCBA.HighRefSet     = HIREF_SETTING;
    //bdt.PCBA.HighRefGainSet = HIREF_GAIN_SET;
    //bdt.PCBA.LowRefSet      = LOREF_SETTING;
    //bdt.PCBA.LowRefGainSet  = LOREF_GAIN_SET;
    //bdt.PCBA.HighRefPlSet   = HIREF_PL_SET;
    //bdt.PCBA.LowRefPlSet    = LOREF_PL_SET;
    
    #ifdef SCREEN_FULL_ADAPTIVE
    bdt.PCBA.HighRefGainSet = 0;
    bdt.PCBA.LowRefGainSet  = 0;
    #endif
    #endif
    TC1126_Init_RefHLRegWRITE();
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void TC1126_Init_CapHighLowSetting4SLEEP(void)
{
    RegTab_t.Reg28BitDef_t.RefhRegConf = 0;
    RegTab_t.Reg28BitDef_t.RcvmReg_t.REFH_REFHI_EN   = TCM_ENABLE;
    RegTab_t.Reg28BitDef_t.RcvmReg_t.REFH_REFHI_INP  = TCM_ENABLE;
    RegTab_t.Reg28BitDef_t.RcvmReg_t.REFH_REFHI_TCAP = DOZE_HIREF_SETTING;
    RegTab_t.Reg28BitDef_t.RcvmReg_t.REFH_REFHI_FCAP = DOZE_HIREF_GAIN_SET;
    RegTab_t.Reg28BitDef_t.RcvmReg_t.REFH_REFHI_PL   = DOZE_HIREF_PL_SET;

    RegTab_t.Reg29BitDef_t.ReflRegConf = 0;
    RegTab_t.Reg29BitDef_t.RcvmReg_t.REFL_REFLO_EN   = TCM_ENABLE;
    RegTab_t.Reg29BitDef_t.RcvmReg_t.REFL_REFLO_INP  = TCM_ENABLE;
    RegTab_t.Reg29BitDef_t.RcvmReg_t.REFL_REFLO_TCAP = DOZE_LOREF_SETTING;
    RegTab_t.Reg29BitDef_t.RcvmReg_t.REFL_REFLO_FCAP = DOZE_LOREF_GAIN_SET;
    RegTab_t.Reg29BitDef_t.RcvmReg_t.REFL_REFLO_PL   = DOZE_LOREF_PL_SET;
    
    SPI_write_singleData(REFH_REG, RegTab_t.Reg28BitDef_t.RefhRegConf);
    SPI_write_singleData(REFL_REG, RegTab_t.Reg29BitDef_t.ReflRegConf); 
}




/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void TC1126_Init_AbsModeSetting(void)
{
    #ifdef COEF_SCALE_ENABLE
    #if 0
    SPI_write_singleData(TFC0_REG, TFC0_T0R0_FCAP_COEF(8) | TFC0_T0R1_FCAP_COEF(8)
                        | TFC0_T0R2_FCAP_COEF(8) | TFC0_T0R3_FCAP_COEF(8));  
    SPI_write_singleData(TFC1_REG, TFC1_T0R4_FCAP_COEF(8) | TFC1_T0R5_FCAP_COEF(8)
                        | TFC1_T0R6_FCAP_COEF(8) | TFC1_T0R7_FCAP_COEF(8));  
    SPI_write_singleData(TFC2_REG, TFC2_T0R8_FCAP_COEF(8) | TFC2_T0R9_FCAP_COEF(8)
                        | TFC2_TNR0_FCAP_COEF(8) | TFC2_SPECIAL_RXI_COORD(10));  
    SPI_write_singleData(DIAG_REG, DIAG_SEPCIAL_TXRXI_FCAP(2/*RCVM_RCVR_FCAP_SET*/) 
                        | DIAG_SEPCIAL_TXI_COORD(14)); 
    SPI_write_singleData(TXMAPTOTX16MORE_REG, TXMAPTO16M_TXJ_CORD(14)
                        | TXMAPTO16M_RXJ_CORD(10) | TXMAPTO16M_TXRXJ_FCAP(2/*RCVM_RCVR_FCAP_SET*/)); 
    //#else
    SPI_write_singleData(TFC0_REG, 0);  
    SPI_write_singleData(TFC1_REG, 0);  
    SPI_write_singleData(TFC2_REG, 0); 
    SPI_write_singleData(DIAG_REG, 0); 
    SPI_write_singleData(TXMAPTOTX16MORE_REG, 0);
    #endif
    #endif
    SPI_write_singleData(FLEN_REG, RECV_NUM*DOZE_TXREADNUM);                 /*  XMTR_NUM * RECV_NUM */
}




/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void TC1126_Init_TransModeSetting(void)
{
    #ifdef COEF_SCALE_ENABLE
    uint16_t fcapvalue;
    fcapvalue = (bdt.PCBA.RcvmRcvrFcapSet<<2);

    RegTab_t.Reg32BitDef_t.Tfc0RegConf = 0;
    RegTab_t.Reg32BitDef_t.Tfc0Reg_t.TFC0_T0R0_FCAP_COEF = fcapvalue;
    RegTab_t.Reg32BitDef_t.Tfc0Reg_t.TFC0_T0R1_FCAP_COEF = fcapvalue;
    RegTab_t.Reg32BitDef_t.Tfc0Reg_t.TFC0_T0R2_FCAP_COEF = fcapvalue;
    RegTab_t.Reg32BitDef_t.Tfc0Reg_t.TFC0_T0R3_FCAP_COEF = fcapvalue;
    SPI_write_singleData(TFC0_REG,RegTab_t.Reg32BitDef_t.Tfc0RegConf);

    RegTab_t.Reg33BitDef_t.Tfc1RegConf = 0;
    RegTab_t.Reg33BitDef_t.Tfc1Reg_t.TFC1_T0R4_FCAP_COEF = fcapvalue;
    RegTab_t.Reg33BitDef_t.Tfc1Reg_t.TFC1_T0R5_FCAP_COEF = fcapvalue;
    RegTab_t.Reg33BitDef_t.Tfc1Reg_t.TFC1_T0R6_FCAP_COEF = fcapvalue;
    RegTab_t.Reg33BitDef_t.Tfc1Reg_t.TFC1_T0R7_FCAP_COEF = fcapvalue;
    SPI_write_singleData(TFC1_REG, RegTab_t.Reg33BitDef_t.Tfc1RegConf);

    RegTab_t.Reg34BitDef_t.Tfc2RegConf = 0;
    RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_T0R8_FCAP_COEF = fcapvalue;
    RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_T0R9_FCAP_COEF = fcapvalue;
    RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_TNR0_FCAP_COEF = fcapvalue;
    RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_SPECIAL_RXI_COORD = fcapvalue;
    SPI_write_singleData(TFC2_REG, RegTab_t.Reg34BitDef_t.Tfc2RegConf);
    
    RegTab_t.Reg35BitDef_t.DiagRegConf = 0;
    RegTab_t.Reg35BitDef_t.DiagReg_t.DIAG_SEPCIAL_TXRXI_FCAP = RCVM_RCVR_FCAP_SET;
    RegTab_t.Reg35BitDef_t.DiagReg_t.DIAG_SEPCIAL_TXI_COORD  = INVALID_CHORPOINT;
    SPI_write_singleData(DIAG_REG, RegTab_t.Reg35BitDef_t.DiagRegConf);


    RegTab_t.Reg53BitDef_t.TxMapToTx16MoreRegConf = 0;
    RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_TXJ_CORD = INVALID_CHORPOINT;
    RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_RXJ_CORD = INVALID_CHORPOINT;
    RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_TXRXJ_FCAP = RCVM_RCVR_FCAP_SET;
    SPI_write_singleData(TXMAPTOTX16MORE_REG, RegTab_t.Reg53BitDef_t.TxMapToTx16MoreRegConf);
    #endif

    RegTab_t.Reg3BBitDef_t.FlenRegConf = 0;
    #ifdef ONE_MORE_LINE_SCAN
    RegTab_t.Reg3BBitDef_t.FlenReg_t.FLEN_FRAME_LEN = (XMTR_NUM*RECV_NUM + RECV_NUM);
    #else
    RegTab_t.Reg3BBitDef_t.FlenReg_t.FLEN_FRAME_LEN = (XMTR_NUM*RECV_NUM);
    #endif
    SPI_write_singleData(FLEN_REG, RegTab_t.Reg3BBitDef_t.FlenRegConf);/*  FLEN_FRAME_LEN = XMTR_NUM * RECV_NUM  */
}
/*******************************************************************************
* Function Name  : TC1126_ChAdaptive_RefHLRegWRITE
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void TC1126_ChAdaptive_RefHLRegWRITE(void)
{
    uint16_t high;
    uint16_t low;

    high = (SPI_read_singleData(REFH_REG)&(0xfffc));
    low = (SPI_read_singleData(REFL_REG)&(0xfffc));

    //R02 -a
    RegTab_t.Reg28BitDef_t.RefhRegConf= 0;
    RegTab_t.Reg28BitDef_t.RcvmReg_t.REFH_REFHI_FCAP= bdt.PCBA.HighRefGainSet;
    high |= RegTab_t.Reg28BitDef_t.RefhRegConf;

    RegTab_t.Reg29BitDef_t.ReflRegConf = 0;
    RegTab_t.Reg29BitDef_t.RcvmReg_t.REFL_REFLO_FCAP = bdt.PCBA.LowRefGainSet;
    low  |= RegTab_t.Reg29BitDef_t.ReflRegConf;
    //R02 -e

    SPI_write_singleData(REFH_REG, high);
    SPI_write_singleData(REFL_REG, low); 
}


#ifdef CHANNEL_ADAPTIVE
/*******************************************************************************
* Function Name  : TC1126_RxChAdaptive_TransModeSetting
* Description    : 往寄存器写入每个Rx通道FCAP的值
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void TC1126_RxChAdaptive_TransModeSetting(void)
{    
    // 注：此时的TFC0_T0R0_FCAP_COEF要与TFC2_TNR0_FCAP_COEF保持一致
    //R02 -a
    RegTab_t.Reg32BitDef_t.Tfc0RegConf = 0;
    RegTab_t.Reg32BitDef_t.Tfc0Reg_t.TFC0_T0R0_FCAP_COEF = (bdt.RxFcapValue[0]<<2);
    RegTab_t.Reg32BitDef_t.Tfc0Reg_t.TFC0_T0R1_FCAP_COEF = (bdt.RxFcapValue[1]<<2)
    RegTab_t.Reg32BitDef_t.Tfc0Reg_t.TFC0_T0R2_FCAP_COEF = (bdt.RxFcapValue[2]<<2);
    RegTab_t.Reg32BitDef_t.Tfc0Reg_t.TFC0_T0R3_FCAP_COEF = (bdt.RxFcapValue[3]<<2);
    SPI_write_singleData(TFC0_REG, RegTab_t.Reg32BitDef_t.Tfc0RegConf);

    RegTab_t.Reg33BitDef_t.Tfc1RegConf = 0
    RegTab_t.Reg33BitDef_t.Tfc1Reg_t.TFC1_T0R4_FCAP_COEF =(bdt.RxFcapValue[4]<<2);
    RegTab_t.Reg33BitDef_t.Tfc1Reg_t.TFC1_T0R5_FCAP_COEF =(bdt.RxFcapValue[5]<<2);
    RegTab_t.Reg33BitDef_t.Tfc1Reg_t.TFC1_T0R6_FCAP_COEF =(bdt.RxFcapValue[6]<<2);
    RegTab_t.Reg33BitDef_t.Tfc1Reg_t.TFC1_T0R7_FCAP_COEF =(bdt.RxFcapValue[7]<<2);
    SPI_write_singleData(TFC1_REG, RegTab_t.Reg33BitDef_t.Tfc1RegConf);

    RegTab_t.Reg34BitDef_t.Tfc2RegConf = 0;
    RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_T0R8_FCAP_COEF=(bdt.RxFcapValue[8]<<2);
    RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_T0R9_FCAP_COEF=(bdt.RxFcapValue[9]<<2);
    RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_TNR0_FCAP_COEF=(bdt.RxFcapValue[0]<<2);
    RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_SPECIAL_RXI_COORD=INVALID_CHORPOINT;
    SPI_write_singleData(TFC2_REG,RegTab_t.Reg34BitDef_t.Tfc2RegConf);
    //R02 -e
}

/*******************************************************************************
* Function Name  : TC1126_ChAdaptive_TransModeSetting
* Description    : 往寄存器写入需要调节的异常通道或者异常点的FCAP值
* Input          :     TxorRxFlag：标记SCALE_MODE类型；
* Output         : 
* Return         : 
*******************************************************************************/
void TC1126_ChAdaptive_TransModeSetting(uint32_t TxorRxFlag)
{
    uint16_t RegValue;
    uint16_t FcapValue;
    uint16_t FcapValue1;

    RegValue = ((SPI_read_singleData(REFH_REG))&(0xcfff));
    //R02 -a
    RegTab_t.Reg28BitDef_t.RefhRegConf = 0;
    RegTab_t.Reg28BitDef_t.RcvmReg_t.REFH_SCALE_EN =1;
    RegTab_t.Reg28BitDef_t.RcvmReg_t.REFH_SCALE_MODE = TxorRxFlag;
    RegValue |= RegTab_t.Reg28BitDef_t.RefhRegConf ;                                // 打开COEF_SCALE_ENABLE     // 转换SCALE_MODE
    //R02 -e
    SPI_write_singleData(REFH_REG, RegValue);
    //R02 -a
    RegTab_t.Reg32BitDef_t.Tfc0RegConf = 0;
    RegTab_t.Reg32BitDef_t.Tfc0Reg_t.TFC0_T0R0_FCAP_COEF = (bdt.RxFcapValue[0]<<2);
    RegTab_t.Reg32BitDef_t.Tfc0Reg_t.TFC0_T0R1_FCAP_COEF = (bdt.RxFcapValue[1]<<2)
    RegTab_t.Reg32BitDef_t.Tfc0Reg_t.TFC0_T0R2_FCAP_COEF = (bdt.RxFcapValue[2]<<2);
    RegTab_t.Reg32BitDef_t.Tfc0Reg_t.TFC0_T0R3_FCAP_COEF = (bdt.RxFcapValue[3]<<2);
    SPI_write_singleData(TFC0_REG, RegTab_t.Reg32BitDef_t.Tfc0RegConf);

    RegTab_t.Reg33BitDef_t.Tfc1RegConf = 0
    RegTab_t.Reg33BitDef_t.Tfc1Reg_t.TFC1_T0R4_FCAP_COEF =(bdt.RxFcapValue[4]<<2);
    RegTab_t.Reg33BitDef_t.Tfc1Reg_t.TFC1_T0R5_FCAP_COEF =(bdt.RxFcapValue[5]<<2);
    RegTab_t.Reg33BitDef_t.Tfc1Reg_t.TFC1_T0R6_FCAP_COEF =(bdt.RxFcapValue[6]<<2);
    RegTab_t.Reg33BitDef_t.Tfc1Reg_t.TFC1_T0R7_FCAP_COEF =(bdt.RxFcapValue[7]<<2);
    SPI_write_singleData(TFC1_REG,RegTab_t.Reg33BitDef_t.Tfc1RegConf);
         //R02 -e
    if( TxorRxFlag == SCALE_MODE_2_SPRX)        //SCALE_MODE_2_SPRX
    {
        if((bdt.AbnormalTxChNum == 0)&&(bdt.AbnormalRxChNum == 1))
        {
            FcapValue = bdt.RxAbnormalCh[bdt.RealAbnormalRxCh[0]];
            //R02 -a
            RegTab_t.Reg34BitDef_t.Tfc2RegConf = 0;
            RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_T0R8_FCAP_COEF=(bdt.RxFcapValue[8]<<2);
            RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_T0R9_FCAP_COEF=(bdt.RxFcapValue[9]<<2);
            RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_TNR0_FCAP_COEF=(bdt.RxFcapValue[0]<<2);
            RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_SPECIAL_RXI_COORD=INVALID_CHORPOINT;
            SPI_write_singleData(TFC2_REG, RegTab_t.Reg34BitDef_t.Tfc2RegConf);

            RegTab_t.Reg35BitDef_t.DiagRegConf = 0;
            RegTab_t.Reg35BitDef_t.DiagReg_t.DIAG_SEPCIAL_TXRXI_FCAP = FcapValue;
            RegTab_t.Reg35BitDef_t.DiagReg_t.DIAG_SEPCIAL_TXI_COORD = INVALID_CHORPOINT;	
            SPI_write_singleData(DIAG_REG,RegTab_t.Reg35BitDef_t.DiagRegConf);

            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreRegConf = 0;
            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_TXJ_CORD =INVALID_CHORPOINT;
            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_RXJ_CORD =(bdt.RealAbnormalRxCh[0]);
            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_TXRXJ_FCAP =FcapValue;
            SPI_write_singleData(TXMAPTOTX16MORE_REG,RegTab_t.Reg53BitDef_t.TxMapToTx16MoreRegConf);
            //R02 -e  
      }
        else if((bdt.AbnormalTxChNum == 0)&&(bdt.AbnormalRxChNum == 2))
        {
            FcapValue = bdt.RxAbnormalCh[bdt.RealAbnormalRxCh[0]];
            FcapValue1 = bdt.RxAbnormalCh[bdt.RealAbnormalRxCh[1]];
            //R02 -a
            RegTab_t.Reg34BitDef_t.Tfc2RegConf = 0;
            RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_T0R8_FCAP_COEF=(bdt.RxFcapValue[8]<<2);
            RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_T0R9_FCAP_COEF=(bdt.RxFcapValue[9]<<2);
            RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_TNR0_FCAP_COEF=(bdt.RxFcapValue[0]<<2);
            RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_SPECIAL_RXI_COORD=bdt.RealAbnormalRxCh[0];
            SPI_write_singleData(TFC2_REG, RegTab_t.Reg34BitDef_t.Tfc2RegConf);

            RegTab_t.Reg35BitDef_t.DiagRegConf = 0;
            RegTab_t.Reg35BitDef_t.DiagReg_t.DIAG_SEPCIAL_TXRXI_FCAP = FcapValue;
            RegTab_t.Reg35BitDef_t.DiagReg_t.DIAG_SEPCIAL_TXI_COORD = INVALID_CHORPOINT;	
            SPI_write_singleData(DIAG_REG,RegTab_t.Reg35BitDef_t.DiagRegConf); 

            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreRegConf = 0;
            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_TXJ_CORD =INVALID_CHORPOINT;
            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_RXJ_CORD =(bdt.RealAbnormalRxCh[1]);
            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_TXRXJ_FCAP =FcapValue1;
            SPI_write_singleData(TXMAPTOTX16MORE_REG, RegTab_t.Reg53BitDef_t.TxMapToTx16MoreRegConf);
            //R02 -e          
        }
    }
    else if(TxorRxFlag == SCALE_MODE_2_SPTX)    //SCALE_MODE_2_SPTX
    {
        if((bdt.AbnormalTxChNum == 1)&&(bdt.AbnormalRxChNum == 0))
        {
            FcapValue = bdt.TxAbnormalCh[bdt.RealAbnormalTxCh[0]];
            //R02 -a
            RegTab_t.Reg35BitDef_t.DiagRegConf = 0;
            RegTab_t.Reg35BitDef_t.DiagReg_t.DIAG_SEPCIAL_TXRXI_FCAP = FcapValue;
            RegTab_t.Reg35BitDef_t.DiagReg_t.DIAG_SEPCIAL_TXI_COORD = bdt.RealAbnormalTxCh[0];	
            SPI_write_singleData(DIAG_REG,RegTab_t.Reg35BitDef_t.DiagRegConf);


            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreRegConf = 0;
            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_TXJ_CORD =bdt.RealAbnormalTxCh[0];
            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_RXJ_CORD =INVALID_CHORPOINT;
            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_TXRXJ_FCAP =FcapValue;
            SPI_write_singleData(TXMAPTOTX16MORE_REG,RegTab_t.Reg53BitDef_t.TxMapToTx16MoreRegConf);
            //R02 -e
        }
        else if((bdt.AbnormalTxChNum == 2)&&(bdt.AbnormalRxChNum == 0))
        {
            FcapValue = bdt.TxAbnormalCh[bdt.RealAbnormalTxCh[0]];
            FcapValue1 = bdt.TxAbnormalCh[bdt.RealAbnormalTxCh[1]];
            //R02 -a
            RegTab_t.Reg35BitDef_t.DiagRegConf = 0;
            RegTab_t.Reg35BitDef_t.DiagReg_t.DIAG_SEPCIAL_TXRXI_FCAP = FcapValue;
            RegTab_t.Reg35BitDef_t.DiagReg_t.DIAG_SEPCIAL_TXI_COORD = bdt.RealAbnormalTxCh[0];	
            SPI_write_singleData(DIAG_REG,RegTab_t.Reg35BitDef_t.DiagRegConf);


            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreRegConf = 0;
            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_TXJ_CORD =bdt.RealAbnormalTxCh[1];
            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_RXJ_CORD =INVALID_CHORPOINT;
            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_TXRXJ_FCAP =FcapValue1;
            SPI_write_singleData(TXMAPTOTX16MORE_REG,RegTab_t.Reg53BitDef_t.TxMapToTx16MoreRegConf);
            //R02 -e
        }
    }
    else if(TxorRxFlag == SCALE_MODE_2_TXRX)    //SCALE_MODE_2_TXRX
    {
        FcapValue = bdt.TxAbnormalCh[bdt.RealAbnormalTxCh[0]];

        //R02 -a
        RegTab_t.Reg34BitDef_t.Tfc2RegConf = 0;
        RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_T0R8_FCAP_COEF=(bdt.RxFcapValue[8]<<2);
        RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_T0R9_FCAP_COEF=(bdt.RxFcapValue[9]<<2);
        RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_TNR0_FCAP_COEF=(bdt.RxFcapValue[0]<<2);
        RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_SPECIAL_RXI_COORD=bdt.RealAbnormalRxCh[0];
        SPI_write_singleData(TFC2_REG,RegTab_t.Reg34BitDef_t.Tfc2RegConf);

        RegTab_t.Reg35BitDef_t.DiagRegConf = 0;
        RegTab_t.Reg35BitDef_t.DiagReg_t.DIAG_SEPCIAL_TXRXI_FCAP = FcapValue;
        RegTab_t.Reg35BitDef_t.DiagReg_t.DIAG_SEPCIAL_TXI_COORD = INVALID_CHORPOINT;	
        SPI_write_singleData(DIAG_REG, RegTab_t.Reg35BitDef_t.DiagRegConf);



        RegTab_t.Reg53BitDef_t.TxMapToTx16MoreRegConf = 0;
        RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_TXJ_CORD =bdt.RealAbnormalTxCh[0];
        RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_RXJ_CORD =INVALID_CHORPOINT;
        RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_TXRXJ_FCAP =FcapValue;
        SPI_write_singleData(TXMAPTOTX16MORE_REG,RegTab_t.Reg53BitDef_t.TxMapToTx16MoreRegConf);
    }//R02 -e
    if(TxorRxFlag == SCALE_MODE_2_POINTS)    
    {
        if(bdt.AbnormalPointNum == 1)
        {//R02 -a
            RegTab_t.Reg34BitDef_t.Tfc2RegConf = 0;
            RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_T0R8_FCAP_COEF=(bdt.RxFcapValue[8]<<2);
            RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_T0R9_FCAP_COEF=(bdt.RxFcapValue[9]<<2);
            RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_TNR0_FCAP_COEF=(bdt.RxFcapValue[0]<<2);
            RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_SPECIAL_RXI_COORD=INVALID_CHORPOINT;
            SPI_write_singleData(TFC2_REG,RegTab_t.Reg34BitDef_t.Tfc2RegConf);
            

            RegTab_t.Reg35BitDef_t.DiagRegConf = 0;
            RegTab_t.Reg35BitDef_t.DiagReg_t.DIAG_SEPCIAL_TXRXI_FCAP = bdt.TxRxiFCAP;
            RegTab_t.Reg35BitDef_t.DiagReg_t.DIAG_SEPCIAL_TXI_COORD = INVALID_CHORPOINT;	
            SPI_write_singleData(DIAG_REG,RegTab_t.Reg35BitDef_t.DiagRegConf);


            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreRegConf = 0;
            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_TXJ_CORD =bdt.AbPointTxiCoord;
            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_RXJ_CORD =(bdt.AbPointRxiCoord);
            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_TXRXJ_FCAP =(bdt.TxRxiFCAP);
            SPI_write_singleData(TXMAPTOTX16MORE_REG,  RegTab_t.Reg53BitDef_t.TxMapToTx16MoreRegConf);
            //R02 -e
        }
        else if(bdt.AbnormalPointNum == 2)
        {
            //R02 -a
            RegTab_t.Reg34BitDef_t.Tfc2RegConf = 0;
            RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_T0R8_FCAP_COEF=(bdt.RxFcapValue[8]<<2);
            RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_T0R9_FCAP_COEF=(bdt.RxFcapValue[9]<<2);
            RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_TNR0_FCAP_COEF=(bdt.RxFcapValue[0]<<2);
            RegTab_t.Reg34BitDef_t.Tfc2Reg_t.TFC2_SPECIAL_RXI_COORD=bdt.AbPointRxiCoord;
            SPI_write_singleData(TFC2_REG,RegTab_t.Reg34BitDef_t.Tfc2RegConf);
            
            RegTab_t.Reg35BitDef_t.DiagRegConf = 0;
            RegTab_t.Reg35BitDef_t.DiagReg_t.DIAG_SEPCIAL_TXRXI_FCAP = bdt.TxRxiFCAP;
            RegTab_t.Reg35BitDef_t.DiagReg_t.DIAG_SEPCIAL_TXI_COORD = bdt.AbPointTxiCoord;	
            SPI_write_singleData(DIAG_REG, RegTab_t.Reg35BitDef_t.DiagRegConf);


            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreRegConf = 0;
            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_TXJ_CORD =bdt.AbPointTxjCoord;
            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_RXJ_CORD =(bdt.AbPointRxjCoord);
            RegTab_t.Reg53BitDef_t.TxMapToTx16MoreReg_t.TXMAPTO16M_TXRXJ_FCAP =(bdt.TxRxiFCAP);
            SPI_write_singleData(TXMAPTOTX16MORE_REG, RegTab_t.Reg53BitDef_t.TxMapToTx16MoreRegConf);
        }   //R02 -e
    }
}
#endif


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void TC1126_Init_ADCI_REG0X22(void)
{
    RegTab_t.Reg22BitDef_t.AdciRegConf = 0;
    RegTab_t.Reg22BitDef_t.AdciReg_t.ADCI_TEMP_ABSO = 1;
    RegTab_t.Reg22BitDef_t.AdciReg_t.ADCI_TEMP_BCNT = 1;
    RegTab_t.Reg22BitDef_t.AdciReg_t.ADCI_TEMP_REST = 1;

    #ifdef PREQY_CHIRP_ONLY 
    RegTab_t.Reg22BitDef_t.AdciReg_t.ADCI_STRETCH_END = 2;
    #endif
    SPI_write_singleData(ADCI_REG, RegTab_t.Reg22BitDef_t.AdciRegConf);
}




/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void TC1126_Init_FCTL_REG0X3F(void)
{
    RegTab_t.Reg3FBitDef_t.FctlRegConf = 0;
    switch(bdt.ModeSelect)
    {
        case iAUTOSCAN_MODE:
        {
            RegTab_t.Reg3FBitDef_t.FctlReg_t.FCTL_SCAN_MODE = 2;
            #ifdef ONE_MORE_LINE_SCAN
            RegTab_t.Reg3FBitDef_t.FctlReg_t.FCTL_TXCH_NUM = (XMTR_NUM+1);
            #else
            RegTab_t.Reg3FBitDef_t.FctlReg_t.FCTL_TXCH_NUM = (XMTR_NUM);
            #endif
            break;
        }
        case DOZE_MODE:
        case SLEEP_MODE:
        {   
            RegTab_t.Reg3FBitDef_t.FctlReg_t.FCTL_SCAN_MODE = 3;
            RegTab_t.Reg3FBitDef_t.FctlReg_t.FCTL_TXCH_NUM = (DOZE_TXREADNUM);
            break;
        }
        default:
            break;
    }
    SPI_write_singleData(FCTL_REG, RegTab_t.Reg3FBitDef_t.FctlRegConf);
}




/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void TC1126_Init_REVM_REG0X27(void)
{
    RegTab_t.Reg27BitDef_t.RcvmRegConf = 0;
    RegTab_t.Reg27BitDef_t.RcvmReg_t.RCVM_CHAN_RST_EN = 1;
    switch(bdt.ModeSelect)
    {
        case iAUTOSCAN_MODE:
        {
            RegTab_t.Reg27BitDef_t.RcvmReg_t.RCVM_RJCT_EN        = 1;
            RegTab_t.Reg27BitDef_t.RcvmReg_t.RCVM_RCVR_TURBO_EN1 = 1;
            RegTab_t.Reg27BitDef_t.RcvmReg_t.RCVM_RCVR_FCAP      = bdt.PCBA.RcvmRcvrFcapSet;
            RegTab_t.Reg27BitDef_t.RcvmReg_t.RCVM_FILT_BW        = 3;
            break;
        }
        case DOZE_MODE:
        case SLEEP_MODE:
        {
            RegTab_t.Reg27BitDef_t.RcvmReg_t.RCVM_RCVR_ABS_EN    = 1;
            RegTab_t.Reg27BitDef_t.RcvmReg_t.RCVM_RCVR_FCAP      = RCVM_RCVR_FCAP_SET;
            break;
        }
    }
    SPI_write_singleData(RCVM_REG,RegTab_t.Reg27BitDef_t.RcvmRegConf);
}




/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void TC1126_Init_RXEN_Field4Noise(void)
{//R02 -a
    RegTab_t.Reg3EBitDef_t.RxcnRegConf = 0;
    RegTab_t.Reg3EBitDef_t.RxcnReg_t.RXCN_RX_CH_SIZE = RECV_NUM;

    #ifdef PHASE_HOP_ONLY
    RegTab_t.Reg3EBitDef_t.RxcnReg_t.RXCN_PHASE_HOP = TCM_ENABLE;
    RegTab_t.Reg3EBitDef_t.RxcnReg_t.RXCN_PHASE_MODE = 2;
    RegTab_t.Reg3EBitDef_t.RxcnReg_t.RXCN_CLUSTER_SIZE =1;
    #endif

    #ifdef FREQY_HOP_ONLY
    RegTab_t.Reg3EBitDef_t.RxcnReg_t.RXCN_FREQ_HOP = 1;    // middle filter with frequency hopping
    RegTab_t.Reg3EBitDef_t.RxcnReg_t.RXCN_CLUSTER_SIZE =2;
    #endif

    #ifdef PREQY_CHIRP_ONLY
    RegTab_t.Reg3EBitDef_t.RxcnReg_t.RXCN_FREQ_CHIRP = TCM_ENABLE;
    #endif
    //R02 -e
    SPI_write_singleData(RXCN_REG, RegTab_t.Reg3EBitDef_t.RxcnRegConf); /*  REG3E */
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void TC1126_Init_iAutoScanModeSetting(void)
{
    TC1126_Init_CapHighLowSetting();
    TC1126_Init_TransModeSetting();
    TC1126_Init_TxMappingRegisters();

    RegTab_t.Reg3ABitDef_t.ProbRegConf = 0;

    /*  Set the Internal SCAN Mode Period */
    #ifdef COMMUNICATION_WITH_PC
      #ifdef SHOW_EVERY_FRAME_DATA
        SPI_write_singleData(PERD_REG, 0xFFF);
        RegTab_t.Reg3ABitDef_t.ProbReg_t.PROB_INTR_MODE = 3;
        SPI_write_singleData(PROB_REG, RegTab_t.Reg3ABitDef_t.ProbRegConf);
      #else
        if(dbg.DebugInfoLevel == DEBUG_INFO_NONE)
          SPI_write_singleData(PERD_REG, ISCANMODE_PERD_REG_VALUE);
        else if(dbg.DebugInfoLevel == DEBUG_INFO_FIGLOC)
          SPI_write_singleData(PERD_REG, ISCANMODE_PERD_REG_VALUE_FINGER_ONLY);
        else
          SPI_write_singleData(PERD_REG, ISCANMODE_PERD_REG_VALUE_NORMAL);
      #endif
    #else
      SPI_write_singleData(PERD_REG, ISCANMODE_PERD_REG_VALUE); /*  cfg_reg39 */
    #endif

    //R02 -a
    RegTab_t.Reg3ABitDef_t.ProbReg_t.PROB_INTR_MODE = 0;
    SPI_write_singleData(PROB_REG,RegTab_t.Reg3ABitDef_t.ProbRegConf); /*  Choose .... */
     //R02 -e

    TC1126_Init_ADCI_REG0X22();
    TC1126_Init_REVM_REG0X27();
    TC1126_Init_FCTL_REG0X3F();
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void TC1126_Init_SleepModeSetting(uint32_t SLEEP_TIME)
{
    TC1126_Init_AbsModeSetting();
    TC1126_Init_CapHighLowSetting4SLEEP();
    TC1126_Init_TxMappingRegisters4SLEEP();
    
    /*  Set the Sleeping Period */
    SPI_write_singleData(PERD_REG, (uint16_t)(SLEEP_TIME&0xfff) );       /*  cfg_reg39 */
    SPI_write_singleData(PROB_REG, (uint16_t)((SLEEP_TIME>>12)&0xfff) ); /*  Change the sleeping time */
    
    /*  Setup the working mode as SCAN+SLEEP mode inside of the chip; */
    TC1126_Init_ADCI_REG0X22();
    TC1126_Init_REVM_REG0X27(); /*  SET ABS MODE over here */
    TC1126_Init_FCTL_REG0X3F(); /*  TX NUMBER = ABS TX SCAN NUM */
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void TC1126_Init_AllRegisters(void)
{
    uint16_t temp_reg;
    uint32_t temp32;
    //R02 -a
    /*************************************************
    * Reset Slow Clock and Related Module
    ************************************************* */
    RegTab_t.Reg1FBitDef_t.FlagRegConf = 0;
    RegTab_t.Reg1FBitDef_t.FlagReg_t.FLAG_SCLK_RST = TCM_ENABLE;
    SPI_write_singleData(FLAG_REG, RegTab_t.Reg1FBitDef_t.FlagRegConf);
    
    /*************************************************
    * ADCM_REG is set, which is Useless Here Actually
    * Since we set it again @ Start_ADC_At_Begining
    **************************************************/
    RegTab_t.Reg21BitDef_t.AdcmRegConf = 0;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_ADC_SPEED = ADC_SPEED_SET;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_ACS = ACS_SPEED_SET;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_MB_EN = TCM_ENABLE;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_ACTV_CONF = TCM_ENABLE; 
    SPI_write_singleData(ADCM_REG, RegTab_t.Reg21BitDef_t.AdcmRegConf);
    
    /**************************************************
    * OSCC_REG is set, which is essential for working
    **************************************************/
    RegTab_t.Reg20BitDef_t.OsccReg_t.OSCC_FAST_MODE = 0;
    RegTab_t.Reg20BitDef_t.OsccReg_t.OSCC_OSD_TRIM = 0x3C;
    RegTab_t.Reg20BitDef_t.OsccReg_t.OSCC_OSD_MODE = 0;
    temp_reg  = RegTab_t.Reg20BitDef_t.OsccRegConf;
    SPI_write_singleData(OSCC_REG, temp_reg); /*  0x3C0,0x580 */

    RegTab_t.Reg24BitDef_t.DurvRegConf = 0;
    RegTab_t.Reg24BitDef_t.DurvReg_t.DURV_RESET_DUR = DUR_RESET;
    RegTab_t.Reg24BitDef_t.DurvReg_t.DURV_INTEG_DUR = DUR_INTEG;
    SPI_write_singleData(DURV_REG, RegTab_t.Reg24BitDef_t.DurvRegConf);/*  cfg_reg24, 12'h144 */
   
    RegTab_t.Reg25BitDef_t.DursRegConf = 0;
    RegTab_t.Reg25BitDef_t.DursReg_t.DURS_STRETCH_DUR = DUR_STRETCH;
    RegTab_t.Reg25BitDef_t.DursReg_t.DURS_STRETCH_INC = STRETCH_INC_REG25;
    SPI_write_singleData(DURS_REG, RegTab_t.Reg25BitDef_t.DursRegConf); /*  cfg_reg25, 12'h000 */

    
    RegTab_t.Reg26BitDef_t.BcntRegConf = 0;
    RegTab_t.Reg26BitDef_t.BcntReg_t.BCNT_BURST_CNT      = BURST_CNT&0x7ff;   //bdt.PCBA.BurstCnt;
    RegTab_t.Reg26BitDef_t.BcntReg_t.BCNT_RCVR_TURBO_EN0 = (BURST_CNT>>11)&1; //bdt.PCBA.BurstCnt;
    SPI_write_singleData(BCNT_REG,RegTab_t.Reg26BitDef_t.BcntRegConf); /*  cfg_reg26, 12'h02f */
    
    /********************************************************
    * Enable or disable the RECV Sensor INPUT pin
    *********************************************************/
    temp32  = (1 << (RECV_STR)) - 1;   /*  RECV_STR, RECV_END */
    temp32  = ~temp32;
    temp32 &= (1 << RECV_END) - 1;   /*  RECV_STR, RECV_END */
    SPI_write_singleData(REN0_REG, 0x3ff);       /*  cfg_reg2a, 12'hfff,RX0-RX11 */

    RegTab_t.Reg2BBitDef_t.Rmp1RegConf = 0;
    RegTab_t.Reg2BBitDef_t.Rmp1Reg_t.RMP1_CH_MAP0 = SCN_R0;
    RegTab_t.Reg2BBitDef_t.Rmp1Reg_t.RMP1_CH_MAP1 = SCN_R1;
    RegTab_t.Reg2BBitDef_t.Rmp1Reg_t.RMP1_CH_MAP2 = SCN_R2;
    RegTab_t.Reg2BBitDef_t.Rmp1Reg_t.RMP1_CH_MAP3 = SCN_R3;
    SPI_write_singleData(RMP1_REG,RegTab_t.Reg2BBitDef_t.Rmp1RegConf);

    RegTab_t.Reg2CBitDef_t.Rmp2RegConf = 0;
    RegTab_t.Reg2CBitDef_t.Rmp2Reg_t.RMP2_CH_MAP4 = SCN_R4;
    RegTab_t.Reg2CBitDef_t.Rmp2Reg_t.RMP2_CH_MAP5 = SCN_R5;
    RegTab_t.Reg2CBitDef_t.Rmp2Reg_t.RMP2_CH_MAP6 = SCN_R6;
    RegTab_t.Reg2CBitDef_t.Rmp2Reg_t.RMP2_CH_MAP7 = SCN_R7;
    SPI_write_singleData(RMP2_REG, RegTab_t.Reg2CBitDef_t.Rmp2RegConf);

    RegTab_t.Reg2DBitDef_t.Rmp3RegConf = 0;
    RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_CH_MAP8 =SCN_R8;
    RegTab_t.Reg2DBitDef_t.Rmp3Reg_t.RMP3_CH_MAP9 =SCN_R9;
    SPI_write_singleData(RMP3_REG, RegTab_t.Reg2DBitDef_t.Rmp3RegConf);
    //R02 -e

    //R02 -a
    RegTab_t.Reg36BitDef_t.TcrsRegConf = 0;
    RegTab_t.Reg36BitDef_t.TcrsReg_t.TCRS_RCVR_FCAP_RST = TCM_ENABLE;
    RegTab_t.Reg36BitDef_t.TcrsReg_t.TCRS_RCVR_FILT_RST = TCM_ENABLE;
    RegTab_t.Reg36BitDef_t.TcrsReg_t.TCRS_RCVR_DEMOD_RST = TCM_ENABLE;
    RegTab_t.Reg36BitDef_t.TcrsReg_t.TCRS_RCVR_SNH_RST = TCM_ENABLE;
    RegTab_t.Reg36BitDef_t.TcrsReg_t.TCRS_RCVR_INPSWT_RST = TCM_ENABLE;
    temp_reg = RegTab_t.Reg36BitDef_t.TcrsRegConf;
    //R02 -e
    SPI_write_singleData(TCRS_REG, temp_reg); /*  cfg_reg36, 12'hf80  */
   /************************************************************************
   * Delay for a while  After reset something
   ************************************************************************/

    mdelay(10);



    
    TC1126_Init_TxMappingRegisters();
    
    /********************************************************
    * Diagnostic and Test Control Register
    ******************************************************** */
    SPI_write_singleData(DIAG_REG, 0x0000); /*  Diagnostic Control; Some RGE_Fields are added  */
    SPI_write_singleData(TCRS_REG, 0x0000); /*  Test Control Reset and Enable */
    SPI_write_singleData(TCEN_REG, 0x0000); /*  Test Control Eanble and Reset */
   //R02 -a
    RegTab_t.Reg39BitDef_t.PerdRegConf = 0;
    RegTab_t.Reg39BitDef_t.PerdReg_t.PERD_PERIOD = 0x40;
    SPI_write_singleData(PERD_REG, RegTab_t.Reg39BitDef_t.PerdRegConf);                  /*  Period[11:0]; */

    RegTab_t.Reg3ABitDef_t.ProbRegConf = 0;
    RegTab_t.Reg3ABitDef_t.ProbReg_t.PROB_PERIOD = 0;
    RegTab_t.Reg3ABitDef_t.ProbReg_t.PROB_INTR_MODE = 0;
    SPI_write_singleData(PROB_REG,RegTab_t.Reg3ABitDef_t.ProbRegConf);  /*  Period[19:12],INTR_MOD[2:0],Frame_Repeat; */


    RegTab_t.Reg3CBitDef_t.TthrRegConf = 0;
    RegTab_t.Reg3CBitDef_t.TthrReg_t.TTHR_TOUCH_TH0 = 0xb0;
    SPI_write_singleData(TTHR_REG, RegTab_t.Reg3CBitDef_t.TthrRegConf); /*  REG3C */
    //R02 -e
    TC1126_Init_RXEN_Field4Noise();
    
    switch(bdt.ModeSelect)
    {
        case iAUTOSCAN_MODE:
        {
            TC1126_Init_iAutoScanModeSetting();
            break;
        }
        case DOZE_MODE:
        {
            TC1126_Init_SleepModeSetting(DOZE_MODE_PERIOD); /*  Doze Mode (Not SLEEP Mode) */
            break;
        }
        case SLEEP_MODE:
        {
            TC1126_Init_SleepModeSetting(0x0);   /*  Sleep Mode (Not DOZE Mode) */
            break;
        }
        default:
            break;
    }
    CN1100_print("D.\n");
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void TC1126_GotoDozeMode(void)
{
    #if defined(DOZE_ALLOWED)
        CN1100_print("==>DOZE_MODE\n");
    #endif

    //******************************************
    // DISABLE TIMING_EN
    //******************************************
    RegTab_t.Reg21BitDef_t.AdcmRegConf = 0;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_ADC_SPEED = ADC_SPEED_SET;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_ACS = ACS_SPEED_SET;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_MB_EN = 1;
    SPI_write_singleData(ADCM_REG,RegTab_t.Reg21BitDef_t.AdcmRegConf);

    bdt.ModeSelect            = DOZE_MODE;
    bdt.MTD.mtdc.Doze_FirstIn = 0;
    bdt.MTD.mtdc.Doze_OddNum  = 0;
    
    TC1126_Init_SleepModeSetting(DOZE_MODE_PERIOD);
    //**********************************************************************
    // Reset PERD_REG small to reduce the time of throwing useless frames
    //**********************************************************************
    SPI_write_singleData(PERD_REG, 0x100);
    SPI_write_singleData(PROB_REG, 0x0); 

    #ifdef FINGER_HWDET4DOZE
    //R02 -a
    RegTab_t.Reg3CBitDef_t.TthrRegConf = 0;
    RegTab_t.Reg3CBitDef_t.TthrReg_t.TTHR_TOUCH_TH0 = 0x30;
    RegTab_t.Reg3CBitDef_t.TthrReg_t.TTHR_TOUCH_DETECT = 1;
    SPI_write_singleData(TTHR_REG,RegTab_t.Reg3CBitDef_t.TthrRegConf);

    RegTab_t.Reg3BBitDef_t.FlenRegConf = 0;
    RegTab_t.Reg3BBitDef_t.FlenReg_t.FLEN_FRAME_LEN = (2*RECV_NUM);
    RegTab_t.Reg3BBitDef_t.FlenReg_t.FLEN_TOUCH_TH1 = 2;
    SPI_write_singleData(FLEN_REG, RegTab_t.Reg3BBitDef_t.FlenRegConf);
    #endif
    //R02 -e

#if 1
{
    SPI_write_singleData(FLAG_REG,  0x009f);
    RegTab_t.Reg21BitDef_t.AdcmRegConf = 0;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_ADC_SPEED = ADC_SPEED_SET;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_ACS = ACS_SPEED_SET;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_SHRT_CKT_EN = 1;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_TIMING_EN = 1;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_MB_EN = 1;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_ACTV_CONF = 1; 
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_XMTR_STR = XMTR_STRENGTH_SET;
    SPI_write_singleData(ADCM_REG, RegTab_t.Reg21BitDef_t.AdcmRegConf );


    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_ACTV_CONF = 0; 
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_SHRT_CKT_EN = 0;
    RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_XMTR_STR_ENB = 1;
    SPI_write_singleData(ADCM_REG, RegTab_t.Reg21BitDef_t.AdcmRegConf );
}
#else
    TC1126_Init_StartADCByReg21();
#endif
}


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void TC1126_GotoAutoScanMode(uint16_t auto_mode_sel)
{
    if(iAUTOSCAN_MODE == auto_mode_sel)
    {
        #if defined(DOZE_ALLOWED)
            CN1100_print("==>iAUTOSCAN_MODE\n");
        #endif

        bdt.ModeSelect  = iAUTOSCAN_MODE;
        TC1126_Init_iAutoScanModeSetting();
    } 

    #ifdef FINGER_HWDET4DOZE
    RegTab_t.Reg3CBitDef_t.TthrRegConf = 0;
    RegTab_t.Reg3CBitDef_t.TthrReg_t.TTHR_TOUCH_TH0 = 0x30;
    SPI_write_singleData(TTHR_REG, RegTab_t.Reg3CBitDef_t.TthrRegConf);

    #ifdef ONE_MORE_LINE_SCAN
    RegTab_t.Reg3BBitDef_t.FlenRegConf = 0;
    RegTab_t.Reg3BBitDef_t.FlenReg_t.FLEN_FRAME_LEN = (XMTR_NUM*RECV_NUM + RECV_NUM);
    RegTab_t.Reg3BBitDef_t.FlenReg_t.FLEN_TOUCH_TH1 = 2;
    SPI_write_singleData(FLEN_REG,RegTab_t.Reg3BBitDef_t.FlenRegConf);
    #else
    RegTab_t.Reg3BBitDef_t.FlenRegConf = 0;
    RegTab_t.Reg3BBitDef_t.FlenReg_t.FLEN_FRAME_LEN = (XMTR_NUM*RECV_NUM);
    RegTab_t.Reg3BBitDef_t.FlenReg_t.FLEN_TOUCH_TH1 = 2;
      SPI_write_singleData(FLEN_REG,RegTab_t.Reg3BBitDef_t.FlenRegConf);
    #endif

    #endif
    
    bdt.BSDSTS.iBuf_A_Fill   = FRAME_UNFILLED;
    bdt.BSDSTS.iBuf_B_Fill   = FRAME_UNFILLED;
    bdt.MTD.NoFingerCnt4Doze = 0;
    TC1126_Init_StartADCByReg21();
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/

void TC1126_GotoSleepMode(void)
{
    if(DOZE_MODE == bdt.ModeSelect)
    {
        /*****************************************************************************
        * Set the Sleeping Period when previous mode is DOZE mode
        *****************************************************************************/
        SPI_write_singleData(PERD_REG, 0); // cfg_reg39
        SPI_write_singleData(PROB_REG, 0); // Change the sleeping time
    }
    else
    {
        TC1126_Init_SleepModeSetting(0);
    }
    bdt.ModeSelect = SLEEP_MODE;
    
    #ifdef FINGER_HWDET4DOZE
    RegTab_t.Reg3CBitDef_t.TthrRegConf = 0;
    RegTab_t.Reg3CBitDef_t.TthrReg_t.TTHR_TOUCH_TH0 = 0x30;
    SPI_write_singleData(TTHR_REG,RegTab_t.Reg3CBitDef_t.TthrRegConf);

    RegTab_t.Reg3BBitDef_t.FlenRegConf = 0;
    RegTab_t.Reg3BBitDef_t.FlenReg_t.FLEN_FRAME_LEN = (2*RECV_NUM);
    RegTab_t.Reg3BBitDef_t.FlenReg_t.FLEN_TOUCH_TH1 = 2;
    SPI_write_singleData(FLEN_REG, RegTab_t.Reg3BBitDef_t.FlenRegConf);
    #endif

    SPI_write_singleData(FLAG_REG, 0x001f);
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
#ifdef FINGER_HWDET4DOZE
/*********************************************************
* AutoDetection funcation is enabled @ Doze Mode
*********************************************************/
uint16_t TC1126_DozeModeDataHandling(uint16_t BufferID)
{
    uint16_t i, j, rtnValue = 0;
    int16_t  tempint16 =0 , tempMax = 0;
    
    #ifdef CN1100_iSCANMODE
    #define  SkipFrameNUM 2
    #endif
    
    /*********************************************
    * When Interrupt is coming, we skip the first 
    * "SkipFrameNUM" frame, then treat others;
    *********************************************/
    SPI_read_DATAs(0x400, DOZE_TXREADNUM*RECV_NUM, (uint16_t *)(bdt.FrameDatLoadA));
    SPI_read_DATAs(0x400+DOZE_TXREADNUM*RECV_NUM, DOZE_TXREADNUM*RECV_NUM, (uint16_t *)(bdt.FrameDatLoadB));
    if(bdt.MTD.mtdc.Doze_FirstIn<SkipFrameNUM)
    {
        bdt.MTD.mtdc.Doze_FirstIn++;
    }
    else
    {
    
        for(j=0; j<DOZE_TXREADNUM; j++)
        {
            for(i=0; i<RECV_NUM; i++) 
            {
                tempint16 = bdt.FrameDatLoadB[j][i] - bdt.FrameDatLoadA[j][i];
                if(tempint16 < 0) 
                {
                    tempint16 = 0 - tempint16;
                }
                if(tempint16 > tempMax) 
                {
                    tempMax = tempint16;
                }    
            }
        }
    }
    
    if(tempMax > DOZE_MODE_FINGER_THR) 
    {
        #ifdef CN1100_iSCANMODE
            TC1126_GotoAutoScanMode(iAUTOSCAN_MODE);
        #endif
        
        rtnValue = 1;
    }
    
    return rtnValue;

}

#else

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
/*********************************************************
* AutoDetection funcation is disabled @ Doze Mode
*********************************************************/
uint16_t TC1126_DozeModeDataHandling(uint16_t BufferID)
{
    uint16_t i, j, rtnValue = 0;
    int16_t  tempint16 =0 , tempMax = 0;
    #define  SkipFrameNUM 3
    
        /*********************************************
        *Read data from Buffer A
        *********************************************/
        switch(BufferID)
        {
            case BUFFER_A:
            {
                SPI_read_DATAs(0x400, DOZE_TXREADNUM*RECV_NUM, (uint16_t *)(bdt.FrameDatLoadA));
                break;
            }
            case BUFFER_B:
            {
                SPI_read_DATAs(0x400+DOZE_TXREADNUM*RECV_NUM, DOZE_TXREADNUM*RECV_NUM, (uint16_t *)(bdt.FrameDatLoadB));
                break;
            }
        }

    #if 1
    if(bdt.MTD.mtdc.Doze_FirstIn < SkipFrameNUM)
    {
        bdt.MTD.mtdc.Doze_FirstIn++;
        if(bdt.MTD.mtdc.Doze_FirstIn >= SkipFrameNUM)
        {
            /*set PERD_REG as DOZE_MODE_PERIOD when we will compute delta value */
            SPI_write_singleData(PROB_REG, (uint16_t)((DOZE_MODE_PERIOD>>12)&0xfff) ); /*  Change the sleeping time */
            SPI_write_singleData(PERD_REG, (uint16_t)(DOZE_MODE_PERIOD&0xfff) );       /*  cfg_reg39 */
        }
        return rtnValue;
    }
    else
    {
        #ifdef DOZEMODE_DIFFSHOW
        CN1100_print("DeltaBuf:\n");
        #endif
        for(j=0; j<DOZE_TXREADNUM; j++)
        {
            for(i=0; i<RECV_NUM; i++) 
            {
                switch(BufferID)
                {
                    case BUFFER_A:
                    {
                    tempint16 = bdt.FrameDatLoadA[j][i] - bdt.FrameDatLoadB[j][i];
                    break;
                    }
                    case BUFFER_B:
                    {
                    tempint16 = bdt.FrameDatLoadB[j][i] - bdt.FrameDatLoadA[j][i];
                    break;
                    }
                }
                if(tempint16 < 0)
                {
                    tempint16 = 0 - tempint16;
                }
                if(tempint16 > tempMax)
                {
                    tempMax = tempint16;
                }

              #ifdef DOZEMODE_DEBUGSHOW
                #ifdef DOZEMODE_DIFFSHOW
                CN1100_print("%4d ", tempint16);
                #else
                if(BUFFER_A == BufferID)
                {
                    tempint16 = bdt.FrameDatLoadA[j][i];
                }
                else
                {
                    tempint16 = bdt.FrameDatLoadB[j][i];
                }
                CN1100_print("%4x ", tempint16);
                #endif
              #endif
            }
            #ifdef DOZEMODE_DEBUGSHOW
            CN1100_print("\n");
            #endif
        }
        #ifdef DOZEMODE_DEBUGSHOW
        CN1100_print("tempMax: %4d\n", tempMax);
        CN1100_print("\n");
        #endif
        
        if(tempMax > DOZE_MODE_FINGER_THR) 
        {
            #ifdef CN1100_iSCANMODE
                TC1126_GotoAutoScanMode(iAUTOSCAN_MODE);
            #endif
            
            rtnValue = 1;
        }
    }
    #endif
    
    return rtnValue;

}
#endif



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void TC1126_SubISR_iAutoMode(void)
{
    RegTab_t.Reg44BitDef_t.DoneRegConf = SPI_read_singleData(DONE_REG);
    if(1 == RegTab_t.Reg44BitDef_t.DoneReg_t.DONE_FRM0_READABLE)
    {
        /*  CN1100_print("A."); */
        /*  Buffer A is ready in CN1100 */
        bdt.BSDSTS.iBuf_A_Fill = FRAME_FILLED;
        RegTab_t.Reg1FBitDef_t.FlagRegConf = 0;
        RegTab_t.Reg1FBitDef_t.FlagReg_t.FLAG_FRM0_RDDONE =1;
        SPI_write_singleData(FLAG_REG, RegTab_t.Reg1FBitDef_t.FlagRegConf);      /*  Clear the interrupt Bit4(Buffer B Just Filled) */

    }
    if(1 == RegTab_t.Reg44BitDef_t.DoneReg_t.DONE_FRM1_READABLE)
    {
        /*  CN1100_print("B."); */
        /*  Buffer B is ready in CN1100 */
        bdt.BSDSTS.iBuf_B_Fill = FRAME_FILLED;
        RegTab_t.Reg1FBitDef_t.FlagRegConf = 0;
        RegTab_t.Reg1FBitDef_t.FlagReg_t.FLAG_FRM1_RDDONE =1;
        SPI_write_singleData(FLAG_REG, RegTab_t.Reg1FBitDef_t.FlagRegConf);      /*  Clear the interrupt Bit4(Buffer B Just Filled) */

    }
}



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void TC1126_SubISR_ScreenAdaptive(void)
{
            #ifdef SCREEN_SIMPLE_ADAPTIVE
                if(bdt.ScreenAdaptiveFlag)
                {
                    TC1126_Init_RefHLRegWRITE();
                    bdt.ScreenAdaptiveFlag=0;
                }
            #endif

            #ifdef SCREEN_FULL_ADAPTIVE
                // 轮流设置每次的参考电压
                if(bdt.PCBA.RefHLSetCount <= 16)
                {
                    TC1126_Init_RefHLRegWRITE();
                }
                // 最终设置参考电压
                if(bdt.PCBA.RefHLSetEndFlag != 0)
                {
                    TC1126_ChAdaptive_RefHLRegWRITE();
                }
            #endif
                
            #ifdef CHANNEL_ADAPTIVE
                if(bdt.AdjustRxChFlag == 1)
                {
                    TC1126_RxChAdaptive_TransModeSetting();
                }
                if((bdt.FindOkFlag == 1)&&(bdt.AbnormalPointNum != 0))
                {
                    TC1126_ChAdaptive_TransModeSetting(SCALE_MODE_2_POINTS);
                    bdt.FindOkFlag = 0;
                }
                if(bdt.FindOkFlag == 1)
                {
                    if((bdt.AbnormalTxChNum != 0)&&(bdt.AbnormalRxChNum == 0))
                    {
                        TC1126_ChAdaptive_TransModeSetting(SCALE_MODE_2_SPTX);
                    }
                    else if((bdt.AbnormalTxChNum == 0)&&(bdt.AbnormalRxChNum != 0))
                    {
                        TC1126_ChAdaptive_TransModeSetting(SCALE_MODE_2_SPRX);
                    }
                    else if((bdt.AbnormalTxChNum == 1)&&(bdt.AbnormalRxChNum == 1))
                    {
                        TC1126_ChAdaptive_TransModeSetting(SCALE_MODE_2_TXRX);
                    }
                    else
                    {
                        //do nothing
                    }
                    bdt.FindOkFlag = 0;
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

    enum hrtimer_restart CN1100_SysTick_ISR(struct hrtimer *timer)

{  


		if(bdt.ModeSelect==DOZE_MODE){
			spidev->irq_count++;
		}
        if((!(spidev->mode & CN1100_IS_DOZE))&&(bdt.ModeSelect!=DOZE_MODE)){
            if(!(spidev->mode & CN1100_IS_SUSPENDED))
                spidev->ticks++;
        }    

        #if defined(CN1100_RESET_ENABLE)
        if((spidev->ticks>5000)&&(bdt.ModeSelect!=DOZE_MODE)){
                CN1100_print("%d\n",spidev->ticks);
                spidev->ticks = 0; 
                queue_work(spidev->workqueue,&spidev->reset_work);
            }
        #endif

        hrtimer_start(&spidev->systic, ktime_set(0, 1000000), HRTIMER_MODE_REL);
        return HRTIMER_NORESTART;

}




/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
//uint16_t dozedebug = 0;

void CN1100_FrameScanDoneInt_ISR(struct work_struct *work)

{

        if(spidev->mode & CN1100_IS_SUSPENDED)
        {

            cn1100_set_irq(true);

            return;
        }
    
    switch(bdt.ModeSelect)
    {
        case DOZE_MODE:
        {
            bdt.BFD.bbdc.NoFingerCnt4Base = 0;
            bdt.BFD.FingerLeftProtectTime = 0;
            /***************************************************************************
            * set BaseUpdateCase as BASE_FRAME_DISCARD so we can discard the first frame 
            * when come back to normal mode
            ****************************************************************************/
            bdt.BFD.bbdc.BaseUpdateCase   = BASE_FRAME_DISCARD;

            RegTab_t.Reg44BitDef_t.DoneRegConf = SPI_read_singleData(DONE_REG);
            #if 0
            CN1100_print("di=%d %4x\n", dozedebug++, RegTab_t.Reg44BitDef_t.DoneRegConf);
            if(0 == (dozedebug&0x7))
            {
                #ifdef CN1100_iSCANMODE
                TC1126_GotoAutoScanMode(iAUTOSCAN_MODE);
                #endif
            }
            else
            #endif
            {
                if(1 == RegTab_t.Reg44BitDef_t.DoneReg_t.DONE_FRM0_READABLE)
                {
                    /* Buffer A is ready in CN1100*/
                    if(0 == TC1126_DozeModeDataHandling(BUFFER_A))
                    {
                        RegTab_t.Reg1FBitDef_t.FlagRegConf = 0;
                        RegTab_t.Reg1FBitDef_t.FlagReg_t.FLAG_FRM0_RDDONE =1;
                        SPI_write_singleData(FLAG_REG, RegTab_t.Reg1FBitDef_t.FlagRegConf);
                    }

                       spidev->irq_count = 0;

                }
                if(1 == RegTab_t.Reg44BitDef_t.DoneReg_t.DONE_FRM1_READABLE)
                {
                    /* Buffer B is ready in CN1100*/
                    if(0 == TC1126_DozeModeDataHandling(BUFFER_B))
                    {
                        RegTab_t.Reg1FBitDef_t.FlagRegConf = 0;
                        RegTab_t.Reg1FBitDef_t.FlagReg_t.FLAG_FRM1_RDDONE =1;
                        SPI_write_singleData(FLAG_REG, RegTab_t.Reg1FBitDef_t.FlagRegConf);
                    }

                       spidev->irq_count = 0;

                }
            }




                cn1100_set_irq(true);


            break;
        }
        
        case iAUTOSCAN_MODE:
        {

            spidev->irq_count = 20;
            spidev->ticks = 0;

            #ifdef DOZE_ALLOWED
                if(bdt.MTD.NoFingerCnt4Doze > WORK_MODE_NOFING_MAXPERD)
                {
                    bdt.MTD.NoFingerCnt4Doze = 0;
                    TC1126_GotoDozeMode();


                      cn1100_set_irq(true);


                    break;
                }
            #endif
            

                TC1126_SubISR_ScreenAdaptive();

                TC1126_SubISR_iAutoMode();


                if(FRAME_FILLED == bdt.BSDSTS.iBuf_A_Fill)
                {    
                    bd->BufferID = 0;
                    BufferAHandle();
                    /*Clear the interrupt Bit4(Buffer B Just Filled)*/
                }
                
                if(FRAME_FILLED == bdt.BSDSTS.iBuf_B_Fill)
                {   
                    bd->BufferID = 1;
                    BufferBHandle();
                    /*Clear the interrupt Bit4(Buffer B Just Filled)*/
                }


                cn1100_set_irq(true);



            break;
        }

        case SLEEP_MODE:
        {
            /***************************************
            * Just Disable TIMING_EN (bit4)
            ***************************************/
            RegTab_t.Reg21BitDef_t.AdcmRegConf = 0;
            RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_ADC_SPEED = ADC_SPEED_SET;
            RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_ACS       = ACS_SPEED_SET;
            RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_TIMING_EN = TCM_ENABLE;
            RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_ACTV_CONF = TCM_ENABLE; 
            RegTab_t.Reg21BitDef_t.AdcmReg_t.ADCM_XMTR_STR  = XMTR_STRENGTH_SET;
            SPI_write_singleData(ADCM_REG, RegTab_t.Reg21BitDef_t.AdcmRegConf);

            bdt.BFD.bbdc.NoFingerCnt4Base = 0;
            bdt.BFD.FingerLeftProtectTime = 0;

            /***************************************************************************
            * set BaseUpdateCase as BASE_FRAME_DISCARD so we can discard the first frame 
            * when come back to normal mode
            ****************************************************************************/
            bdt.BFD.bbdc.BaseUpdateCase   = BASE_FRAME_DISCARD;
            
            RegTab_t.Reg44BitDef_t.DoneRegConf = SPI_read_singleData(DONE_REG);
            if(1 == RegTab_t.Reg44BitDef_t.DoneReg_t.DONE_FRM0_READABLE)
            {
                /*Buffer A is ready in CN1100*/
                RegTab_t.Reg1FBitDef_t.FlagRegConf = 0;
                RegTab_t.Reg1FBitDef_t.FlagReg_t.FLAG_FRM0_RDDONE = 1;
                SPI_write_singleData(FLAG_REG, RegTab_t.Reg1FBitDef_t.FlagRegConf); /*Clear the interrupt Bit3(Buffer A Just Filled)*/
            }
            else if(1 == RegTab_t.Reg44BitDef_t.DoneReg_t.DONE_FRM1_READABLE)
            {
                /* Buffer B is ready in CN1100*/
                RegTab_t.Reg1FBitDef_t.FlagRegConf = 0;
                RegTab_t.Reg1FBitDef_t.FlagReg_t.FLAG_FRM1_RDDONE = 1;
                SPI_write_singleData(FLAG_REG, RegTab_t.Reg1FBitDef_t.FlagRegConf); /* Clear the interrupt Bit3(Buffer A Just Filled)*/
            }
            

               spidev->mode |= CN1100_IS_SUSPENDED;
               wake_up(&spidev->waitq);
 
            break;
        }
        default:
        {
            break;
        }
    }
 
}

#endif
