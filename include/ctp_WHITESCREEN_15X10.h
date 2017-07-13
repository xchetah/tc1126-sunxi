#ifndef BOARD_WHITESCREEN_15X10_H
#define  BOARD_WHITESCREEN_15X10_H

//****************************************************
// This TYPE of the screen is the first TP we used
//                            (From 2013-5-1)
//****************************************************
#define HIREF_SETTING        HIGH_REF_4PF
#define LOREF_SETTING        LOW_REF_1PF
#define HIREF_PL_SET         HIGH_REF_POS
#define LOREF_PL_SET         LOW_REF_POS
#define HIREF_GAIN_SET       REF_GAIN_16PF
#define LOREF_GAIN_SET       REF_GAIN_12PF
#define TX_DRIVE_PL          TX_DRIVE_INVS

#define RCVM_RCVR_FCAP_SET   RXF_GAIN_12PF

#define DUR_RESET            8

#ifdef WINNER_TWD                    //for 全志通文达
#define DUR_INTEG             63
#else
#define DUR_INTEG             36
#endif

#define DUR_STRETCH          1      // TX Freq: 150K

#ifdef WINNER_TWD                   //for 全志通文达
#define BURST_CNT            0x81C
#else
#define BURST_CNT            0x82f  //0x81C
#endif



#define DOZE_HIREF_SETTING   HIGH_REF_2PF
#define DOZE_LOREF_SETTING   LOW_REF_2PF
#define DOZE_HIREF_PL_SET    HIGH_REF_POS
#define DOZE_LOREF_PL_SET    LOW_REF_NEG
#define DOZE_HIREF_GAIN_SET  REF_GAIN_12PF
#define DOZE_LOREF_GAIN_SET  REF_GAIN_12PF
#define DOZE_TX_DRIVE_PL     TX_DRIVE_NORM

#define DOZE_TXREADNUM       2


#define XMTR_STR             0
#define SXMTR_NUM            15 // Screen XMTR
#define KXMTR_NUM            0  // Key XMTR
#define XMTR_NUM             (SXMTR_NUM + KXMTR_NUM)
#define RECV_STR             0
#define SRECV_NUM            10
#define KRECV_NUM            0
#define RECV_NUM            (SRECV_NUM + KRECV_NUM)

#define MIN_VALUE_POINT      0
#define ROUGH_FINGER_THR     50     // Minimum Value for Valid Finger
#define MAX_VAL_NON_FINGER   200
#define BE_PERCENT_RATIO     BE25PERCENT
#define XMTR_STRENGTH_SET    XMTR_STRENGTH_MAX

#endif
