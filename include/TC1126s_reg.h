/******************************************************************************
 * ¡ã?¨¨¡§?¨´¨®D(C) TRUECOREING
 * DEPARTMENT:
 * MANUAL_PERCENT:
 * ???t??3?: CN1000_data.c 
 * ???t¡À¨º¨º?:    
 * ?¨²¨¨Y?a¨°a: 
 * ???¨¹?¦Ì?¡Â:
 * ¦Ì¡À?¡ã¡ã?¡À?: 
 * ¡Á¡Â    ??: 
 * ¨ª¨º3¨¦¨¨??¨²:
 * ¦Ì¡À?¡ã?e¨¨?¨¨?: 
 *
 * DT??????1: ¡ä¨²??o?1?
 * DT??¨¨??¨²: 2014-09-15
 * ¡ã? ¡À? o?:
 * DT ?? ¨¨?: Wangpc
 * DT???¨²¨¨Y: 
 *
 * DT??????2: Add one feature that acquire 10 fingers then parse 5 fingers 
 * DT??¨¨??¨²: 2014-11-12
 * ¡ã? ¡À? o?:
 * DT ?? ¨¨?: Wangpc(R01)
 * DT???¨²¨¨Y: 
 *
 * DT??????3: Rebuild structure of register
 * DT??¨¨??¨²: 2014-11-19
 * ¡ã? ¡À? o?:
 * DT ?? ¨¨?: Wangpc(R02)
 * DT???¨²¨¨Y: 
 *****************************************************************************/


#ifndef TC1126S_REG_H
#define  TC1126S_REG_H


/*******************************************************/
/**** 1.1    *********  CONST  *************************/
/*******************************************************/
#define HIGH_REF_1PF        0
#define HIGH_REF_2PF        1
#define HIGH_REF_4PF        2
#define HIGH_REF_6PF        3
#define HIGH_REF_POS        1
#define HIGH_REF_NEG        0

#define LOW_REF_0PF         0
#define LOW_REF_1PF         1
#define LOW_REF_2PF         2
#define LOW_REF_3PF         3
#define LOW_REF_POS         1
#define LOW_REF_NEG         0

#define REF_GAIN_4PF        0
#define REF_GAIN_8PF        1
#define REF_GAIN_12PF       2
#define REF_GAIN_16PF       3

#define RXF_GAIN_4PF        0
#define RXF_GAIN_8PF        1
#define RXF_GAIN_12PF       2
#define RXF_GAIN_16PF       3

#define TX_DRIVE_NORM       0
#define TX_DRIVE_INVS       1

#define XMTR_STRENGTH_MIN   0
#define XMTR_STRENGTH_MID1  1
#define XMTR_STRENGTH_MID2  2
#define XMTR_STRENGTH_MAX   3

#define SCALE_MODE_2_POINTS 0
#define SCALE_MODE_2_SPRX   1
#define SCALE_MODE_2_SPTX   2
#define SCALE_MODE_2_TXRX   3

/*******************************************************/
/**** 1.2    *********  REGISTER  **********************/
/*******************************************************/

#define FLAG_REG                0x1F      // FLAG for Frame and ADC transform

#define OSCC_REG                0x20      // OSC control register

#define ADCM_REG                0x21      // ADC Management
#define ADCM_XMTR_STR_MIN       0
#define ADCM_XMTR_STR_MID1      1
#define ADCM_XMTR_STR_MID2      2
#define ADCM_XMTR_STR_MAX       3
#define ADCM_ACS_DIV01          0
#define ADCM_ACS_DIV02          1
#define ADCM_ACS_DIV03          2
#define ADCM_ACS_DIV04          3
#define ADCM_ACS_DIV06          4
#define ADCM_ACS_DIV08          5
#define ADCM_ACS_DIV012         6
#define ADCM_ACS_DIV020         7
#define ADCM_ADC_SPEED_DIV1     0
#define ADCM_ADC_SPEED_DIV2     1
#define ADCM_ADC_SPEED_DIV4     2
#define ADCM_ADC_SPEED_DIV6     3

#define ADCI_REG                0x22       // ADC Interrupt Type selection

#define DACC_REG                0x23       // DAC (Digital Value)

#define DURV_REG                0x24       // Duration for both Reset and Integration

#define DURS_REG                0x25       // Durtion of Stretch

#define BCNT_REG                0x26       // Burst Count

#define RCVM_REG                0x27       // Receiver Management

#define REFH_REG                0x28       // Refernece High

#define REFL_REG                0x29       // Refernece Low

#define REN0_REG                0x2A       // RECV Enable [11:0]
#define REN0_RCVR_EN(x)        (1<<(x)) 

#define RMP1_REG                0x2B       // RECV Enable [11:0]

#define RMP2_REG                0x2C       // RECV Enable 

#define RMP3_REG                0x2D       // RECV Enable 

#define TXEN_REG                0x2E       // Tansmitter Enable [27:24]




//************************************************************
// REG_0x2F is reserved
//************************************************************

#define TPL1_REG                0x30       // Tansmitter Plority [23:12]

//************************************************************
// REG_0x31 is reserved
//************************************************************

#define TFC0_REG                0x32      // Tansmitter Input [11:0]

#define TFC1_REG                0x33      // Tansmitter Input [23:12]

#define TFC2_REG                0x34      // Tansmitter Input [27:24]

#define DIAG_REG                0x35      // Diagnostic Control

#define TCRS_REG                0x36      // Test Control ---- Reset and Enable

#define TCEN_REG                0x37      // Test Control ---- Enable and Reset

//************************************************************
// REG_0x38 is reserved
//************************************************************

#define PERD_REG                0x39      // Write Register ---- Period

#define PROB_REG                0x3A      // Write Register ---- Probe and Period

#define FLEN_REG                0x3B      // Frame Length

#define TTHR_REG                0x3C      // RX CH Begin0

//************************************************************
// REG_0x3d is reserved
//************************************************************

#define RXCN_REG                0x3E      // RX CH Number

#define FCTL_REG                0x3F      // TX CH Control


//#define TTHR_REG    0x40      // TOUCH THR, etc
//#define TTHR_TTHR0(x)           ((x)<<0)  // Bit5:0,   TOUCH THR0
//#define TTHR_TTHR1(x)           ((x)<<6)  // Bit7:6,   TOUCH THR1
//#define TTHR_TDETECT            (1<<8)    // Bit8,     TOUCH Detect Enable Bit
//#define TTHR_CLUSTERSIZE(x)     ((x)<<10) // Bit11:10, Cluster Size

//************************************************************
// REG41 is reserved
// REG42 is changed from 0x39 (TC1126)
// REG43 is changed from 0x3A (TC1126)
// REG44 is changed from 0x38 (TC1126)
//************************************************************

#define RJCT_REG                0x42      // Read is moved from 0x39 to 0x42

#define REVN_REG                0x43      // Read is moved from 0x3A to 0x43

#define DONE_REG                0x44      // Something is done in Status, Read Only

#define FM0FG0_ADDR             0x45      // Frame0_Finger0_Address (11:0)
#define FM0FG0_DELTA            0x46      // Frame0_Finger0_Delta   (11:0)
#define FM0FG1_ADDR             0x47      // Frame0_Finger1_Address (11:0)
#define FM0FG1_DELTA            0x48      // Frame0_Finger1_Delta   (11:0)
#define FM1FG0_ADDR             0x49      // Frame1_Finger0_Address (11:0)
#define FM1FG0_DELTA            0x4A      // Frame1_Finger0_Delta   (11:0)
#define FM1FG1_ADDR             0x4B      // Frame1_Finger1_Address (11:0)
#define FM1FG1_DELTA            0x4C      // Frame1_Finger1_Delta   (11:0)

//************************************************************
// TX Mapping
//************************************************************
#define TXMAP_REG(x)            (0x40+(x)) // X = 0, ,,, , 17, Total: 18
#define TXMAP0_REG              0x40     // TX_CH_MAP0[15-0], First Time TX
#define TXMAP1_REG              0x41     // TX_CH_MAP1[15-0], Second Time TX
#define TXMAP2_REG              0x42     // TX_CH_MAP2[15-0], Third Time TX
#define TXMAP3_REG              0x43     // TX_CH_MAP3[15-0], Forth Time TX
#define TXMAP4_REG              0x44     // TX_CH_MAP4[15-0], Fifth Time TX
#define TXMAP5_REG              0x45     // TX_CH_MAP5[15-0], Sixth Time TX
#define TXMAP6_REG              0x46     // TX_CH_MAP6[15-0]
#define TXMAP7_REG              0x47     // TX_CH_MAP7[15-0]
#define TXMAP8_REG              0x48     // TX_CH_MAP8[15-0]
#define TXMAP9_REG              0x49     // TX_CH_MAP9[15-0]
#define TXMAP10_REG             0x4A     // TX_CH_MAP10[15-0]
#define TXMAP11_REG             0x4B     // TX_CH_MAP11[15-0]
#define TXMAP12_REG             0x4C     // TX_CH_MAP12[15-0]
#define TXMAP13_REG             0x4D     // TX_CH_MAP13[15-0]
#define TXMAP14_REG             0x4E     // TX_CH_MAP14[15-0]
#define TXMAP15_REG             0x4F     // TX_CH_MAP15[15-0], 16th Time TX 
#define TXMAP16_REG             0x50     // TX_CH_MAP16[15-0], 17th Time TX
#define TXMAP17_REG             0x51     // TX_CH_MAP17[15-0], 18th Time TX

#define TXMAP_TX_CH(x)         (1<<(x))  //Bitx, tx_ch_mapX, Mapping to TX_x signal
#define TXMAP_TX_CH_0          (1<<0)   //Bit0, tx_ch_map0, Mapping to TX0 signal
#define TXMAP_TX_CH_1          (1<<1)   //Bit1, tx_ch_map1, Mapping to TX1 signal
#define TXMAP_TX_CH_2          (1<<2)   //Bit2, tx_ch_map2, Mapping to TX2 signal
#define TXMAP_TX_CH_3          (1<<3)   //Bit3, tx_ch_map3, Mapping to TX3 signal
#define TXMAP_TX_CH_4          (1<<4)   //Bit4, tx_ch_map4, Mapping to TX4 signal
#define TXMAP_TX_CH_5          (1<<5)   //Bit5, tx_ch_map5, Mapping to TX5 signal
#define TXMAP_TX_CH_6          (1<<6)   //Bit6, tx_ch_map6, Mapping to TX6 signal
#define TXMAP_TX_CH_7          (1<<7)   //Bit7, tx_ch_map7, Mapping to TX7 signal
#define TXMAP_TX_CH_8          (1<<8)   //Bit8, tx_ch_map8, Mapping to TX8 signal
#define TXMAP_TX_CH_9          (1<<9)   //Bit9, tx_ch_map9, Mapping to TX9 signal
#define TXMAP_TX_CH_10         (1<<10)  //Bit10, tx_ch_map10, Mapping to TX10 signal
#define TXMAP_TX_CH_11         (1<<11)  //Bit11, tx_ch_map11, Mapping to TX11 signal
#define TXMAP_TX_CH_12         (1<<12)  //Bit12, tx_ch_map12, Mapping to TX12 signal
#define TXMAP_TX_CH_13         (1<<13)  //Bit13, tx_ch_map13, Mapping to TX13 signal
#define TXMAP_TX_CH_14         (1<<14)  //Bit14, tx_ch_map14, Mapping to TX14 signal
#define TXMAP_TX_CH_15         (1<<15)  //Bit15, tx_ch_map15, Mapping to TX15 signal

#define TXMAPTOTX16ONLY_REG     0x52      // TX_CH_MAP to TX16 for "sometime" (First Time to 16Th Time)
#define TXMAPTO16O_Bit(x)      (1<<(x))   // BitX: "Time Slut X"

#define TXMAPTOTX16MORE_REG     0x53


//R02 -a
typedef union
{
    unsigned short FlagRegConf;
    struct {
     unsigned short  FLAG_BRST_FLAG_CLR     :1;    // Clear Flag of Burst
     unsigned short  FLAG_ADC_FLAG_CLR      :1;    // Clear Flag of ADC    
     unsigned short  FLAG_CVI_FLAG_CLR      :1;    // Clear Flag of CVI
     unsigned short  FLAG_FRM0_RDDONE       :1;    // Frame0 Read Done
     unsigned short  FLAG_FRM1_RDDONE       :1;    // Frame1 Read Done
     unsigned short  FLAG_CHIP_RST          :1;    // Reset Chip
     unsigned short  FLAG_MCLK_RST          :1;    // Reset Master Clock
     unsigned short  FLAG_SCLK_RST          :1;
    } FlagReg_t;
} Reg1FBitDef;


typedef union
{
   unsigned short OsccRegConf;
   struct {
        unsigned short OSCC_OSD_MODE  : 4;  //(x)        ((x)<<0)  // osc_mode[3:0]
        unsigned short OSCC_OSD_TRIM  : 6;  //(x)        ((x)<<4)  // osc_trim[5:0]
        unsigned short OSCC_FAST_MODE : 2;  //(x)        ((x)<<10) // fast_mode[1:0]
    } OsccReg_t;
} Reg20BitDef;


typedef union
{
    unsigned short AdcmRegConf;
    struct {
         unsigned short ADCM_ACTV_CONF    : 1;     // Bit0,     Used for working with SHRT_CKT_EN
         unsigned short ADCM_MB_EN        : 1;    // Bit1,     Turn on Analog Biases 
         unsigned short ADCM_XMTR_STR     : 2;     // Bit3:2,   Strength of TX (15ns to 125ns, 5pf to 100pf)
         unsigned short ADCM_TIMING_EN    : 1;      // Bit4:     Timing Enable(=1) OR Disable(=0) for ADC
         unsigned short ADCM_XMTR_STR_ENB : 1;     // Bit5,     Transmitters NOT tri-state the output
         unsigned short ADCM_SHRT_CKT_EN  : 1;      // Bit6,     shrt_ckt_en
         unsigned short ADCM_ACS          : 3;    // Bit9:7,   Prescaler for Transeiver Clock
         unsigned short ADCM_ADC_SPEED    : 2;     // Bit11:10, Prescaler for ADC and DAC
         unsigned short ADCM_TEMP_SPMB    : 1;      // Bit12,temp_spmb
    } AdcmReg_t;
} Reg21BitDef;


typedef union{
    unsigned short AdciRegConf;
    struct {
         unsigned short DUMMY            :1;
         unsigned short ADCI_TEMP_ABSO   :1;  // Bit1,temp_abso
         unsigned short ADCI_TEMP_BWAT   :1;    // Bit2,temp_bwat
         unsigned short ADCI_TEMP_BCNT   :1;    // Bit3,temp_bcnt
         unsigned short ADCI_STRETCH_END :6;  // Bit9:4,stretch_end
         unsigned short ADCI_TEMP_REST   :1;    // Bit10,temp_rest
         unsigned short ADCI_SHUTDN_ENAB :1;   // Bit11,temp_enab
    } AdciReg_t;
} Reg22BitDef;


typedef union{
    unsigned short DaccRegConf;
    struct {
        unsigned short DACC_DDAC_DATA  : 12;   // Bit11:0,  DAC input data
    } DaccReg_t;
} Reg23BitDef;


typedef union{
    unsigned short DurvRegConf;
    struct {
        unsigned short DURV_RESET_DUR :6;  // Bit5:0, Duration of the reset phase of the sensing cycle 
        unsigned short DURV_INTEG_DUR :6;  // Bit11:6,Duration of charge integration phase of the sensing cycle
    } DurvReg_t;
} Reg24BitDef;


typedef union{
    unsigned short DursRegConf;
    struct {
        unsigned short DURS_STRETCH_DUR  :6; // Bit5:0, Duration of the stretch phase of the sensing cycle
        unsigned short DURS_STRETCH_INC  :6;  // Bit11:6,stretch_inc
    } DursReg_t;
} Reg25BitDef;


typedef union{
    unsigned short BcntRegConf;
    struct {
         unsigned short BCNT_BURST_CNT      :11;      // Bit10:0,Duration of a burst in terms of sensing cycles.
         unsigned short BCNT_RCVR_TURBO_EN0 :1;       // Bit11,  rcvr_turbo_en0
    } BcntReg_t;
} Reg26BitDef;


typedef union{
    unsigned short RcvmRegConf;
    struct {
      unsigned short RCVM_RCVR_FCAP       :2;        // Bit1:0, Feedback cap of the receivers(4PF, 8PF, 12PF, and 16PF)
      unsigned short RCVM_FILT_BW         :2;           // Bit3:2, Val of the field defines the bw of LPF of the RECV ch.
      unsigned short RCVM_CHAN_RST_EN     :1;       // Bit4,   RST the filter of RECV-CH before starting of a sensing cluster
      unsigned short RCVM_RJCT_CNT_RST_EN :1;    // Bit5,   The reject counter resets at the start of a burst
      unsigned short RCVM_RCVR_STR_ENB    :1;   // Bit6,   Input of receiver does not float
      unsigned short RCVM_RCVR_TURBO_EN1  :1;   // Bit7,   rcvr_turbo_en1 / spare_out[1]
      unsigned short RCVM_RJCT_EN         :1;   // Bit8,   Enables the cycle skipping feature in which ....
      unsigned short RCVM_RCVR_ALL_SHORT  :1;    // Bit9,   Short all-receiver to a common node
      unsigned short RCVM_ABS_PL          :1;    // Bit10,  It changes the order of the pull up and pull down signals
      unsigned short RCVM_RCVR_ABS_EN     :1;   // Bit11,  Absolute mode of Receiver on the receiver pins in ABS mode
    } RcvmReg_t;
} Reg27BitDef;


typedef union{
    unsigned short RefhRegConf;
    struct {
        unsigned short REFH_REFHI_FCAP   :2;    // Bit1:0    ref_hi_fb_cap[1:0],     ---- {Could be 4, 8, 12, 16 PF}
        unsigned short REFH_REFHI_TCAP   :2;   // Bit3:2,   ref_hi_trans_cap[1:0],  ---- {Could be 1, 2, 4,  6  PF}
        unsigned short REFH_REFHI_INP    :1;       // Bit4,     ref_hi_inp,  ----Input to the driver of transmitter that drives the "hi" ref
        unsigned short REFH_REFHI_PL     :1;     // Bit5,     ref_hi_pl,   ----Polarity of the carrier that drives "hi" ref
        unsigned short REFH_REFHI_EN     :1;      // Bit6,     ref_hi_en,   ----Enables the "hi" reference channel
        unsigned short REFH_XVREF_EN     :1;      // Bit7,     xvref_en     ----Test Propose with external REF
        unsigned short REFH_CRTC_INP     :1;      // Bit8,     crtc_inp     ----Input to the driver of the transmitter...
        unsigned short REFH_CRTC_PL      :1;     // Bit9,     crtc_pl      ----Polarity of the carrier that drives...
        unsigned short REFH_VREF_RESET_EN:1;      // Bit10,    vref_reset_en / spare_out[2]
        unsigned short REFH_VREF_TRN_EN  :1;     // Bit11,    vref_trn__en / spare_out[3]
        unsigned short REFH_SCALE_MODE   :2;   // Bit13:12, scale_mode
        unsigned short REFH_SCALE_EN     :1;      // Bit14,   scale_en
    } RcvmReg_t;
} Reg28BitDef;


typedef union{
    unsigned short ReflRegConf;
    struct {
        unsigned short REFL_REFLO_FCAP :2;  // Bit1:0,   ref_low_fb_cap[1:0],    ---- {Could be 4, 8, 12, 16 PF}
        unsigned short REFL_REFLO_TCAP :2 ;  // Bit3:2,   ref_low_trans_cap[1:0], ---- {Could be 0, 1, 2,  3  PF}
        unsigned short REFL_REFLO_INP  :1;    // Bit4,     ref_low_inp,       ----Same as hi Ref
        unsigned short REFL_REFLO_PL   :1 ;    // Bit5,     ref_low_pl,        ----Same as hi Ref
        unsigned short REFL_REFLO_EN   :1 ;     // Bit6,     ref_low_en,        ----Same as hi Ref
        unsigned short REFL_CRTC_SEL   :5 ;  // Bit11:7,  reg_crtc_sel[4:0]  ----Select bits for the channel...
    } RcvmReg_t;
}Reg29BitDef;


typedef union{
    unsigned short Rmp1RegConf;
    struct {
         unsigned short RMP1_CH_MAP0 :4;       // Bit3:0,  ch_map0
         unsigned short RMP1_CH_MAP1 :4;      // Bit7:4,  ch_map1
         unsigned short RMP1_CH_MAP2 :4;       // Bit11:8,  ch_map2
         unsigned short RMP1_CH_MAP3 :4;     // Bit15:12,  ch_map3
    } Rmp1Reg_t;
} Reg2BBitDef;


typedef union{
    unsigned short Rmp2RegConf;
    struct {
        unsigned short RMP2_CH_MAP4 :4;    // Bit3:0,  ch_map4
        unsigned short RMP2_CH_MAP5 :4;   // Bit7:4,  ch_map5
        unsigned short RMP2_CH_MAP6 :4;    // Bit11:8,  ch_map6
        unsigned short RMP2_CH_MAP7 :4;  // Bit15:12,  ch_map7
    } Rmp2Reg_t;
} Reg2CBitDef;


typedef union{
    unsigned short Rmp3RegConf;
    struct {
       unsigned short RMP3_CH_MAP8 :4;   // Bit3:0,  ch_map8
       unsigned short RMP3_CH_MAP9 :4;   // Bit7:4,  ch_map9
       unsigned short RMP3_TX_EN16 :1;      // Bit8,  tx_en16
       unsigned short RMP3_TX_PL16 :1;     // Bit9,  tx_pl16
    } Rmp3Reg_t;
} Reg2DBitDef;


typedef union{
    unsigned short TxenRegConf;
    struct {
        unsigned short  TXEN_TX_EN :16;   // Bit15:0, tx_en[15:0]            
    } TxenReg_t;
} Reg2EBitDef;


typedef union
{
    unsigned short Tpl1RegConf;
    struct {
        unsigned short TPL1_TX_PL :16;      // Bit15:0. tx_pl[15:0]
    } Tpl1Reg_t;
} Reg30BitDef;


typedef union
{
    unsigned short Tfc0RegConf;
    struct {
        unsigned short TFC0_T0R0_FCAP_COEF :4;// Bit3:0, t0r0_fcap_coef
        unsigned short TFC0_T0R1_FCAP_COEF :4;  // Bit7:4, t0r1_fcap_coef
        unsigned short TFC0_T0R2_FCAP_COEF :4;  // Bit11:8, t0r2_fcap_coef
        unsigned short TFC0_T0R3_FCAP_COEF :4; // Bit15:12, t0r3_fcap_coef
    } Tfc0Reg_t;
} Reg32BitDef;


typedef union
{
    unsigned short Tfc1RegConf;
    struct{
        unsigned short TFC1_T0R4_FCAP_COEF :4;  // Bit3:0, t0r4_fcap_coef
        unsigned short TFC1_T0R5_FCAP_COEF :4; // Bit7:4, t0r5_fcap_coef
        unsigned short TFC1_T0R6_FCAP_COEF :4;  // Bit11:8, t0r6_fcap_coef
        unsigned short TFC1_T0R7_FCAP_COEF :4;// Bit15:12, t0r7_fcap_coef
    } Tfc1Reg_t;
} Reg33BitDef;


typedef union
{
    unsigned short Tfc2RegConf;
    struct {
        unsigned short TFC2_T0R8_FCAP_COEF :4;  // Bit3:0, t0r8_fcap_coef
        unsigned short TFC2_T0R9_FCAP_COEF :4;  // Bit7:4, t0r9_fcap_coef
        unsigned short TFC2_TNR0_FCAP_COEF :4; // Bit11:8, tnr0_fcap_coef
        unsigned short TFC2_SPECIAL_RXI_COORD :4; // Bit15:12, special_rxi_coord
    } Tfc2Reg_t;
} Reg34BitDef;


typedef union
{
    unsigned short DiagRegConf;
    struct {
        unsigned short DIAG_PROBE_SEL          :5;  // Bit4:0,   prb_sel[4:0]    ----Test propose
        unsigned short DIAG_PROBE_EN           :1;  // Bit5,     prb_en          ----Test propose, Call as prb_str in old doc
        unsigned short DIAG_SEPCIAL_TXRXI_FCAP :2;// Bit7:6,   special_txrxi_fcap
        unsigned short DIAG_DDRV               :1; // Bit8,     ddrv            ----Diagnostic Propose
        unsigned short DIAG_DHOLD              :1;  // Bit9,     dhold           ----Diagnostic Propose
        unsigned short DIAG_DVREF_EN           :1;  // Bit10,    dvref_en        ----Used VDD and GND for the diag DAC REF
        unsigned short DIAG_SPA_OUT6           :1;   // Bit11,    Spare Out 6
        unsigned short DIAG_SEPCIAL_TXI_COORD  :5; // Bit15:11,   special_txi_coord
    } DiagReg_t;
} Reg35BitDef;


typedef union
{
    unsigned short TcrsRegConf;
    struct {
        unsigned short TCRS_RCVR_FCAP_RST   :1;   // Bit11,    rcvr_fb_cap_rst,   ----Force FB_CAP to short
        unsigned short TCRS_RCVR_FILT_RST   :1;  // Bit10,    filt_rst,          ----Removes all filters from sensing channels
        unsigned short TCRS_RCVR_DEMOD_RST  :1;   // Bit9,     demod_rst,         ----Forces demodulator to sample always
        unsigned short TCRS_RCVR_SNH_RST    :1;    // Bit8,     snh_rst,           ----Forces sample and hold to sample always
        unsigned short TCRS_RCVR_INPSWT_RST :1;   // Bit7,     rcvr_inp_swt_rst,  ----Forces switch in front end to stay shorted
        unsigned short TCRS_RCVR_PULLDN_RST :1;   // Bit6,     pull_dn_rst,       ----Forces all receiver blocks to pull down
        unsigned short TCRS_RCVR_ADC_ENB    :1;    // Bit5,     adc_en_b,          ----Forces all ADCs to be turned off ???????[QFU]
        unsigned short TCRS_RCVR_FILT_ENB   :1;   // Bit4,     filt_en_b,         ----Disables the timing pulses to the filters
        unsigned short TCRS_RCVR_DEMOD_ENB  :1;   // Bit3,     demod_en_b,        ----Disables the timing pulses to the demodulator
        unsigned short TCRS_RCVR_SNH_ENB    :1;  // Bit2,     snh_en_b,          ----Disables the timing pulses to the sample and hold
        unsigned short TCRS_RCVR_INPSWT_ENB :1;   // Bit1,     rcvr_inp_swt_en_b, ----Forces switch in the front end to stay open always
        unsigned short TCRS_RCVR_PULLDN_ENB :1;   // Bit0,     pull_dn_en_b,      ----Disables the timing pulses to pull down switch(@RCVR)
    } TcrsReg_t;
} Reg36BitDef;


typedef union
{
    unsigned short TcenRegConf;
    struct {
        unsigned short TCEN_XSTOP_SENSING_EN:1;   // Bit11,    xstop_sensing_en,  ----The external stop sensing feature is enabled
        unsigned short TCEN_MRCVR_ENB       :1;   // Bit10,    mrcvr_en_b,        ----Forces all receivers to be disabled at the same time
        unsigned short TCEN_DBLBUFRD_EN     :1;   // Bit9,     dbl_buf_rd_en,     ----A read from the double buffered registers read from 
        unsigned short TCEN_SHARE_RST       :1;   // Bit8,     share_rst,         ----Forces sample sharing to be on all the time
        unsigned short TCEN_RCVR_FCAP_ENB   :1;   // Bit7,     rcvr_fb_cap_en_b,  ----Disable timing pulses to the FB CAP reset switch
        unsigned short TCEN_PULLUP_RST      :1;   // Bit6,     pull_up_rst,       ----Forces all receiver blocks to pull up all the time
        unsigned short TCEN_XMTR_PROBE_EN   :1;   // Bit5,     xmtr_probe_en,     ----Enables the probe on one of the transmitter pins
        unsigned short TCEN_XMTR_CLKOUT_EN  :1;   // Bit4,     xmtr_clkout_en,    ----Enables Digital CLK out on one of the transmitter pins
        unsigned short TCEN_UFO_MODE        :1;   // Bit3,     ufo_mode,          ----Main oscillator frequency to run at 20MHz
        unsigned short TCEN_SHARE_ENB       :1;   // Bit2,     share_en_b,        ----Disabled sample sharing to be on all the time.
        unsigned short TCEN_SLEW_DETECT_ENB :1;   // Bit1,     slew_detect_en_b,  ----Slew detection mechanism (Enable/Disable) for the sensing
        unsigned short TCEN_PULL_UP_ENB     :1;   // Bit0,     pull_up_en_b       ----Disables the timing pulses to pull up switch(@RCVR)
    } TcenReg_t;
} Reg37BitDef;


typedef union
{
    unsigned short PerdRegConf;
    struct {
        unsigned short PERD_PERIOD  :12;    // Bit11:0,  Period[11:0]
    } PerdReg_t;
} Reg39BitDef;


typedef union
{
    unsigned short ProbRegConf;
    struct {
        unsigned short PROB_PERIOD           :8;   // Bit7:0,   time_period[19:12]
        unsigned short PROB_INTR_MODE        :3; // Bit10:8,  intr_mode[2:0]
        unsigned short PROB_FRAME_REPEAT_ENB :1;  //bit11,frame_repeat
    } ProbReg_t;
} Reg3ABitDef;


typedef union
{
    unsigned short FlenRegConf;
    struct {
        unsigned short FLEN_FRAME_LEN :10;   // Bit9:0,   Frame_Length[9:0]
        unsigned short FLEN_TOUCH_TH1 :2;  //Bit11:10,touch_th1
    } FlenReg_t;
} Reg3BBitDef;


typedef union
{
    unsigned short TthrRegConf;
    struct {
        unsigned short TTHR_TOUCH_TH0     : 8;    //Bit7:0,touch_th0
        unsigned short TTHR_TOUCH_DETECT  : 1;       //Bit8,  touch_detect
        unsigned short TTHR_HOST_ENDIAN   : 2;   //Bit11:10,host_endian
    } TthrReg_t;
} Reg3CBitDef;


typedef union
{
    unsigned short RxcnRegConf;
    struct {
        unsigned short RXCN_RX_CH_SIZE   :5; //Bit4:0, rx_ch_size
        unsigned short RXCN_CLUSTER_SIZE :2; //Bit6:5, cluster_size
        unsigned short RXCN_SCAN_POST    :1;   //Bit7, scan_post
        unsigned short RXCN_FREQ_HOP     :2;   //Bit9:8, freq_hop
        unsigned short RXCN_FREQ_CHIRP   :1;  //Bit10, freq_chip
        unsigned short RXCN_DENOISE_EN   :1;   //Bit11, Denoise_en
        unsigned short RXCN_PHASE_MODE   :2; //Bit13:12, phase_mode
        unsigned short RXCN_PHASE_HOP    :1;  //Bit14, phase_hop
        unsigned short RXCN_FILTER_HALF  :1;   //Bit15, filter_half
    } RxcnReg_t;
} Reg3EBitDef;


typedef union
{
    unsigned short FctlRegConf;
    struct {
        unsigned short DUMMY          : 4;
        unsigned short FCTL_TXCH_NUM  : 5;   // Bit8:4,   TX_CH_NUMBER[4:0]
        unsigned short FCTL_SCAN_MODE : 2; // Bit10:9:  SCAN MODE:(0:LineMode,1:ManualMode,2:AutoMode,3:SleepMode)
        unsigned short FCTL_S_SEL     : 1;   // Bit11,    X Sel
    } FctlReg_t;
} Reg3FBitDef;


typedef union
{
    unsigned short RjctRegConf;
    struct {
        unsigned short RJCT_RJCT_CNT   : 11;  // Bit10:0,  Rjct_cnt[10:0]
        unsigned short RJCT_TIMING_ENB : 1;  // Bit11,    Timing_EN_B
    } RjctReg_t;
} Reg42BitDef;


typedef union
{
    unsigned short RevnRegConf;
    struct {
        unsigned short REVN_DCMP    :   1;  // Bit0,     DCMP
        unsigned short DUMMY        :   4;
        unsigned short REVN_REVISION:   4;   // Bit8:5,   Revision[3:0]
    } RevnReg_t;
} Reg43BitDef;


typedef union
{
    unsigned short DoneRegConf;
    struct {
        unsigned short DONE_BRST_DONE     :1; // Bit0,      Burst Done flag,        Write 1 to clear it
        unsigned short DONE_ADC_DONE      :1;   // Bit1,      ADC converting is done, Write 1 to clear it
        unsigned short DONE_CVI_DONE      :1;    // Bit2,      CVI done; Read Only
        unsigned short DONE_FRM0_READABLE :1;   // Bit3,      Frame0 Readable
        unsigned short DONE_FRM1_READABLE :1;   // Bit4,      Frame1 Readable
        unsigned short DONE_TP_ID         :3;  // Bit7:5,    Touch_Panel ID
    } DoneReg_t;
} Reg44BitDef;


typedef union
{
    unsigned short TxMapToTx16MoreRegConf;
    struct {
        unsigned short TXMAPTO16M_TS16       : 1;        //Bit0, The 17Th Time, Out to TX16
        unsigned short TXMAPTO16M_TS17       : 1;        //Bit1, The 18Th Time, Out to TX16
        unsigned short TXMAPTO16M_TXRXJ_FCAP : 2;   //Bit3:2,  Special_TXRXJ_FCAP
        unsigned short TXMAPTO16M_RXJ_CORD   : 4;   //Bit7:4,  4Bits, Special_RXJ_COORD
        unsigned short TXMAPTO16M_TXJ_CORD   : 5;    //Bit12:8, 4Bits, Special_TXJ_COORD
    } TxMapToTx16MoreReg_t;
} Reg53BitDef;


typedef struct
{
   Reg1FBitDef Reg1FBitDef_t;
   Reg20BitDef Reg20BitDef_t;
   Reg21BitDef Reg21BitDef_t;
   Reg22BitDef Reg22BitDef_t;
   Reg23BitDef Reg23BitDef_t;
   Reg24BitDef Reg24BitDef_t;
   Reg25BitDef Reg25BitDef_t;
   Reg26BitDef Reg26BitDef_t;
   Reg27BitDef Reg27BitDef_t;
   Reg28BitDef Reg28BitDef_t;
   Reg29BitDef Reg29BitDef_t;
   Reg2BBitDef Reg2BBitDef_t;
   Reg2CBitDef Reg2CBitDef_t;
   Reg2DBitDef Reg2DBitDef_t;
   Reg2EBitDef Reg2EBitDef_t;
   Reg30BitDef Reg30BitDef_t;
   Reg32BitDef Reg32BitDef_t;
   Reg33BitDef Reg33BitDef_t;
   Reg34BitDef Reg34BitDef_t;
   Reg35BitDef Reg35BitDef_t;
   Reg36BitDef Reg36BitDef_t;
   Reg37BitDef Reg37BitDef_t;
   Reg39BitDef Reg39BitDef_t;
   Reg3ABitDef Reg3ABitDef_t;
   Reg3BBitDef Reg3BBitDef_t;
   Reg3CBitDef Reg3CBitDef_t;
   Reg3EBitDef Reg3EBitDef_t;
   Reg3FBitDef Reg3FBitDef_t;
   Reg42BitDef Reg42BitDef_t;
   Reg43BitDef Reg43BitDef_t;   
   Reg44BitDef Reg44BitDef_t;
   Reg53BitDef Reg53BitDef_t;
   
}RegTab;



//#ifdef CN1100_MAIN_C
 // RegTab RegTab_t;

//#else
//  extern RegTab RegTab_t;
  
//#endif
//R02 -e

#endif
