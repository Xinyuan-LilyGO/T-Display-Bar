// #include    "bq27220.h"
#include <Arduino.h>
#include <Wire.h>
#define BQ_SHUNT_RESISTOR 10 // 0.010 ohms * 1000

// Set data into "addr"
#define BQ27220_ADDR 0x55

// Standard Commands
#define BQ_CNTL 0x00
#define BQ_AR 0x02
#define BQ_ARTTE 0x04
#define BQ_TEMP 0x06
#define BQ_VOLT 0x08
#define BQ_FLAGS 0x0A
#define BQ_CURRENT 0x0C

#define BQ_RM 0x10
#define BQ_FCC 0x12
#define BQ_AI 0x14
#define BQ_TTE 0x16
#define BQ_TTF 0x18
#define BQ_SI 0x1A
#define BQ_STTE 0x1C
#define BQ_MLI 0x1E

#define BQ_MLTTE 0x20
#define BQ_RCC 0x22
#define BQ_AP 0x24

#define BQ_INTTEMP 0x28
#define BQ_CYC 0x2A
#define BQ_SOC 0x2C
#define BQ_SOH 0x2E

#define BQ_CV 0x30
#define BQ_CC 0x32
#define BQ_BTPD 0x34
#define BQ_BTPC 0x36

#define BQ_OS 0x3A
#define BQ_DC 0x3C
#define BQ_SUB 0x3E

#define BQ_MACDATA 0x40

#define BQ_MACDATASUM 0x60
#define BQ_MACDATALEN 0x61

#define BQ_ANACNT 0x79
#define BQ_RAWC 0x7A
#define BQ_RAWV 0x7C
#define BQ_RAWT 0x7E

// Sub Commands
#define BQ_CNTL_STAT 0x0000
#define BQ_DEVICE_NUMBER 0x0001
#define BQ_FW_VERSION 0x0002
#define BQ_HW_VERSION 0x0003

#define BQ_BOARD_OFFSET 0x0009
#define BQ_CC_OFFSET 0x000A
#define BQ_CC_OFFSET_SAVE 0x000B
#define BQ_OCV_CMD 0x000C
#define BQ_BAT_INSERT 0x000D
#define BQ_BAT_REMOVE 0x000E

#define BQ_SET_SNOOZE 0x0013
#define BQ_CLEAR_SNOOZE 0x0014
#define BQ_SET_PROFILE_1 0x0015
#define BQ_SET_PROFILE_2 0x0016
#define BQ_SET_PROFILE_3 0x0017
#define BQ_SET_PROFILE_4 0x0018
#define BQ_SET_PROFILE_5 0x0019
#define BQ_SET_PROFILE_6 0x001A

#define BQ_CAL_TOGGLE 0x002D

#define BQ_SET_SEALED 0x0030

#define BQ_RESET 0x0041

#define BQ_OP_STATUS 0x0054
#define BQ_GAUGE_STATUS 0x0056

#define BQ_EXIT_CAL 0x0080
#define BQ_ENTER_CAL 0x0081

#define BQ_ENTER_CFG_UPDATE 0x0090
#define BQ_EXIT_CFG_UPDATE_REINIT 0x0091
#define BQ_EXIT_CFG_UPDATE 0x0092

#define BQ_RETURN_TO_ROM 0x0F00

// Configuration parameters
#define BQ_CONFIG_CC_GAIN 0x9184 // float

#define BQ_CONFIG_CHG_INH_LO 0x91f5   // int16_t
#define BQ_CONFIG_CHG_INH_HI 0x91f7   // int16_t
#define BQ_CONFIG_CHG_INH_HYST 0x91f9 // int16_t
#define BQ_CONFIG_CHG_CURR 0x91fb     // int16_t
#define BQ_CONFIG_CHG_VOLT 0x91fd     // int16_t

#define BQ_CONFIG_TAPER_CURR 0x9201   // int16_t
#define BQ_CONFIG_OP_CONFIG_A 0x9206  // uint16_t
#define BQ_CONFIG_OP_CONFIG_B 0x9208  // uint16_t
#define BQ_CONFIG_SOC_DELTA 0x920b    // uint8_t
#define BQ_CONFIG_CLK_CTRL 0x920c     // uint8_t
#define BQ_CONFIG_IO_CONFIG 0x920d    // uint8_t
#define BQ_CONFIG_INIT_DIS_SET 0x920e // int16_t
#define BQ_CONFIG_INIT_CHG_SET 0x9210 // int16_t
#define BQ_CONFIG_DEVICE_TYPE 0x9212  // uint16_t

#define BQ_CONFIG_SLEEP_CURR 0x9217   // int16_t
#define BQ_CONFIG_BUS_LO_TIME 0x9219  // uint8_t
#define BQ_CONFIG_OC_INH_T_LO 0x921a  // int16_t
#define BQ_CONFIG_OC_INH_T_HI 0x921c  // int16_t
#define BQ_CONFIG_SLEEP_V_TIME 0x921e // uint8_t
#define BQ_CONFIG_SLEEP_C_TIME 0x921f // uint8_t

#define BQ_CONFIG_DIS_DET_THRES 0x9228 // int16_t
#define BQ_CONFIG_CHG_DET_THRES 0x922a // int16_t
#define BQ_CONFIG_QUIT_CURR 0x922c     // int16_t
#define BQ_CONFIG_DIS_RELAX_TI 0x922e  // uint16_t

#define BQ_CONFIG_CHG_RELAX_TI 0x9230  // uint8_t
#define BQ_CONFIG_QUIT_RELAX_TI 0x9231 // uint8_t

#define BQ_CONFIG_OT_CHG 0x9232       // int16_t
#define BQ_CONFIG_OT_CHG_TIME 0x9234  // uint8_t
#define BQ_CONFIG_OT_CGH_RECOV 0x9235 // int16_t
#define BQ_CONFIG_OT_DSG 0x9237       // int16_t
#define BQ_CONFIG_OT_DSG_TIME 0x9239  // uint8_t
#define BQ_CONFIG_OT_DSG_RECOV 0x923a // int16_t
#define BQ_CONFIG_INIT_STBY 0x923c    // int8_t

#define BQ_CONFIG_SYSD_S_VTHRS 0x9240 // int16_t
#define BQ_CONFIG_SYSD_S_VTIME 0x9242 // uint8_t
#define BQ_CONFIG_SYSD_C_VTHRS 0x9243 // int16_t

#define BQ_GG_SMOOTHING_CONFIG 0x9271  // uint8_t
#define BQ_CONFIG_FLAG_CONFIG_A 0x927f // uint16_t
#define BQ_CONFIG_FLAG_CONFIG_B 0x9281 // uint8_t

#define BQ_CONFIG_BATTERY_ID 0x929a // uint8_t

// Gas Gauge parameters
#define BQ_GG_CEDVp1_GAUGE_CONF 0x929b // uint16_t
#define BQ_GG_CEDVp1_FULLC_CAP 0x929d  // int16_t 15 bits
#define BQ_GG_CEDVp1_DESIGN_CAP 0x929f // int16_t 15 bits
#define BQ_GG_CEDVp1_DESIGN_V 0x92a3   // int16_t 15 bits
#define BQ_GG_CEDVp1_CHG_TERM_V 0x92a5 // int16_t
#define BQ_GG_CEDVp1_EMF 0x92a7        // uint16_t
#define BQ_GG_CEDVp1_C0 0x92a9         // uint16_t
#define BQ_GG_CEDVp1_R0 0x92ab         // uint16_t
#define BQ_GG_CEDVp1_T0 0x92ad         // uint16_t
#define BQ_GG_CEDVp1_R1 0x92af         // uint16_t
#define BQ_GG_CEDVp1_TC 0x92b1         // uint8_t
#define BQ_GG_CEDVp1_C1 0x92b2         // uint8_t
#define BQ_GG_CEDVp1_AGE_FACTOR 0x92b3 // uint8_t
#define BQ_GG_CEDVp1_FIXED_EDV0 0x92b4 // int16_t
#define BQ_GG_CEDVp1_HOLDT_EDV0 0x92b6 // uint8_t
#define BQ_GG_CEDVp1_FIXED_EDV1 0x92b7 // int16_t
#define BQ_GG_CEDVp1_HOLDT_EDV1 0x92b9 // uint8_t
#define BQ_GG_CEDVp1_FIXED_EDV2 0x92ba // int16_t
#define BQ_GG_CEDVp1_HOLDT_EDV2 0x92bc // uint8_t

// CNTL_STAT register bit equates
#define BQ_BIT_CS_CCA 0x0020
#define BQ_BIT_CS_BCA 0x0010
#define BQ_BIT_CS_SNOOZE 0x0008
#define BQ_BIT_CS_BAT_ID2 0x0004
#define BQ_BIT_CS_BAT_ID1 0x0002
#define BQ_BIT_CS_BAT_ID0 0x0001
#define BQ_BIT_CS_BAT_ID (BQ_BIT_CS_BAT_ID2 | BQ_BIT_CS_BAT_ID1 | BQ_BIT_CS_BAT_ID0)

// Gauging Status register bit equates
#define BQ_BIT_GS_VDQ 0x8000
#define BQ_BIT_GS_EDV2 0x4000
#define BQ_BIT_GS_EDV1 0x2000
#define BQ_BIT_GS_RSVD1 0x1000
#define BQ_BIT_GS_RSVD2 0x0800
#define BQ_BIT_GS_FCCX 0x0400
#define BQ_BIT_GS_RSVD3 0x0200
#define BQ_BIT_GS_RSVD4 0x0100
#define BQ_BIT_GS_CF 0x0080
#define BQ_BIT_GS_DSG 0x0040
#define BQ_BIT_GS_EDV 0x0020
#define BQ_BIT_GS_RSVD5 0x0010
#define BQ_BIT_GS_TC 0x0008
#define BQ_BIT_GS_TD 0x0004
#define BQ_BIT_GS_FC 0x0002
#define BQ_BIT_GS_FD 0x0001

// CEDV Gauging Configuration register bit equates
#define BQ_BIT_GC_SME0 0x1000
#define BQ_BIT_GC_IGNORE_SD 0x0800
#define BQ_BIT_GC_FC_FOR_VDQ 0x0400
#define BQ_BIT_GC_FCC_LIMIT 0x0100
#define BQ_BIT_GC_FIXED_EDV0 0x0020
#define BQ_BIT_GC_SC 0x0010
#define BQ_BIT_GC_EDV_CMP 0x0008
#define BQ_BIT_GC_CSYNC 0x0002
#define BQ_BIT_GC_CCT 0x0001

// FLAGS register bit equates
#define BQ_BIT_F_FD 0x8000
#define BQ_BIT_F_OCV_COMP 0x4000
#define BQ_BIT_F_OCV_FAIL 0x2000
#define BQ_BIT_F_SLEEP 0x1000
#define BQ_BIT_F_OTC 0x0800
#define BQ_BIT_F_OTD 0x0400
#define BQ_BIT_F_FC 0x0200
#define BQ_BIT_F_CHGINH 0x0100
#define BQ_BIT_F_TCA 0x0040
#define BQ_BIT_F_OCVGD 0x0020
#define BQ_BIT_F_AUTH_GD 0x0010
#define BQ_BIT_F_BATTPRES 0x0008
#define BQ_BIT_F_TDA 0x0004
#define BQ_BIT_F_SYSDWN 0x0002
#define BQ_BIT_F_DSG 0x0001

// OS register bit equates
#define BQ_BIT_OS_CFGUPDATE 0x0400
#define BQ_BIT_OS_BTPINT 0x0080
#define BQ_BIT_OS_SMTH 0x0040
#define BQ_BIT_OS_INITCOMP 0x0020
#define BQ_BIT_OS_VDQ 0x0010
#define BQ_BIT_OS_EDV2 0x0008
#define BQ_BIT_OS_SEC1 0x0004
#define BQ_BIT_OS_SEC0 0x0002
#define BQ_BIT_OS_CALMD 0x0001

// Operation Config Reg A bit equates
#define BQ_BIT_OCA_TEMPS 0x8000
#define BQ_BIT_OCA_RSVD1 0x4000
#define BQ_BIT_OCA_BATG_POL 0x2000
#define BQ_BIT_OCA_BATG_EN 0x1000
#define BQ_BIT_OCA_RSVD2 0x0800
#define BQ_BIT_OCA_SLEEP 0x0400
#define BQ_BIT_OCA_SLPWAKECHG 0x0200
#define BQ_BIT_OCA_WRTEMP 0x0100
#define BQ_BIT_OCA_BIE 0x0080
#define BQ_BIT_OCA_RSVD3 0x0040
#define BQ_BIT_OCA_BI_PUP_EN 0x0020
#define BQ_BIT_OCA_PFC_CFG1 0x0010
#define BQ_BIT_OCA_PFC_CFG0 0x0008
#define BQ_BIT_OCA_WAKE_EN 0x0004
#define BQ_BIT_OCA_WK_TH1 0x0002
#define BQ_BIT_OCA_WK_TH0 0x0001

// Operation Config Reg B bit equates
#define BQ_BIT_OCB_RSVD1 0x8000
#define BQ_BIT_OCB_RSVD2 0x4000
#define BQ_BIT_OCB_RSVD3 0x2000
#define BQ_BIT_OCB_RSVD4 0x1000
#define BQ_BIT_OCB_DEF_SEAL 0x0800
#define BQ_BIT_OCB_NR 0x0400
#define BQ_BIT_OCB_RSVD5 0x0200
#define BQ_BIT_OCB_RSVD6 0x0100
#define BQ_BIT_OCB_INT_BREM 0x0080
#define BQ_BIT_OCB_INT_BATL 0x0040
#define BQ_BIT_OCB_INT_STATE 0x0020
#define BQ_BIT_OCB_INT_OCV 0x0010
#define BQ_BIT_OCB_RSVD7 0x0008
#define BQ_BIT_OCB_INT_OT 0x0004
#define BQ_BIT_OCB_INT_POL 0x0002
#define BQ_BIT_OCB_INT_FOCV 0x0001

// SOC Flags Reg A bit equates
#define BQ_BIT_SOCFA_TCSETVCT 0x0800
#define BQ_BIT_SOCFA_FCSETVCT 0x0400
#define BQ_BIT_SOCFA_TCCLEARRSOC 0x0080
#define BQ_BIT_SOCFA_TCSETRSOC 0x0040
#define BQ_BIT_SOCFA_TCCLEARV 0x0020
#define BQ_BIT_SOCFA_TCSETV 0x0010
#define BQ_BIT_SOCFA_TDCLEARRSOC 0x0008
#define BQ_BIT_SOCFA_TDSETRSOC 0x0004
#define BQ_BIT_SOCFA_TDCLEARV 0x0002
#define BQ_BIT_SOCFA_TDSETV 0x0001

// SOC Flags Reg B bit equates
#define BQ_BIT_SOCFB_FCCLEARRSOC 0x0080
#define BQ_BIT_SOCFB_FCSETRSOC 0x0040
#define BQ_BIT_SOCFB_FCCLEARV 0x0020
#define BQ_BIT_SOCFB_FCSETV 0x0010
#define BQ_BIT_SOCFB_FDCLEARRSOC 0x0008
#define BQ_BIT_SOCFB_FDSETRSOC 0x0004
#define BQ_BIT_SOCFB_FDCCLEARV 0x0002
#define BQ_BIT_SOCFB_FDSETV 0x0001

// IO Config bit equates
#define BQ_BIT_IOCFG_BtpIntPol 0x0002
#define BQ_BIT_SOCFB_BTpIntEn 0x0001

// Smoothing Config bit equates
#define BQ_BIT_SMOC_SMOOTH_EOC_EN 0x0008
#define BQ_BIT_SMOC_CMEXT 0x0004
#define BQ_BIT_SMOC_VAVG 0x0002
#define BQ_BIT_SMOC_SMEN 0x0001

class BQ27220
{
public:
    typedef struct
    {
        uint16_t cntlReg;  /*!< CNTL register */
        int16_t arReg;     /*!< AR register */
        uint16_t artteReg; /*!< ARTTE register */
        uint16_t tempReg;  /*!< TEMP register */
        uint16_t voltReg;  /*!< VOLT register */

        uint16_t flagsReg;  /*!< FLAGS register */
        int16_t currentReg; /*!< CURRENT register */
        uint16_t rmReg;     /*!< RM register */
        uint16_t fccReg;    /*!< FCC register */
        uint16_t aiReg;     /*!< AI register */

        uint16_t tteReg;  /*!< TTE register */
        uint16_t ttfReg;  /*!< TTF register */
        int16_t siReg;    /*!< SI register */
        uint16_t stteReg; /*!< STTE register */
        int16_t mliReg;   /*!< MLI register */

        uint16_t mltteReg;   /*!< MLTTE register */
        uint16_t rawccReg;   /*!< RCC register */
        int16_t apReg;       /*!< AP register */
        uint16_t intTempReg; /*!< INTTEMP register */
        uint16_t cycReg;     /*!< CYC register */

        uint16_t socReg;  /*!< SOC register */
        uint16_t sohReg;  /*!< SOH register */
        uint16_t cvReg;   /*!< CV register */
        uint16_t ccReg;   /*!< CC register */
        uint16_t btpdReg; /*!< BTPD register */

        uint16_t btpcReg;  /*!< BTPC register */
        uint16_t osReg;    /*!< OS register */
        uint16_t dcReg;    /*!< DC register */
        uint16_t subReg;   /*!< SUB command register */
        char macData[32];  /*!< MAC Data array */
        uint8_t macSumReg; /*!< MAC Data Sum register */

        uint8_t macLenReg; /*!< MAC Data Len register */
        uint8_t anacReg;   /*!< Analog Count register */
        uint16_t rawcReg;  /*!< RAWC register */
        uint16_t rawvReg;  /*!< RAWV register */
        uint16_t rawtReg;  /*!< RAWT register */

        uint8_t checksum; /*!< calculated checksum result */

        int16_t shunt_res; /*!< Shunt Resistor value / 1000 */
        char i2c_Bufx[48]; /*!< i2c buffer */
    } BQ27220_TypeDef;

    BQ27220(uint8_t sda_pin, uint8_t scl_pin, uint32_t freq = 100000);
    int read_registers(BQ27220_TypeDef& dataSTR);
    uint32_t get_dev_id(BQ27220_TypeDef& dataSTR);
    void unseal(BQ27220_TypeDef& dataSTR);

private:
    uint8_t _addr;
    uint8_t _sda;
    uint8_t _scl;
    uint32_t _freq;

    uint8_t readByte(uint8_t reg);
    uint16_t readWord(uint8_t reg);
    void writeByte(uint8_t reg, uint8_t data);
    void writeWord(uint8_t reg, uint16_t data);
};

// BQ27220::BQ27220(PinName p_sda, PinName p_scl) :
//     _i2c(p_sda, p_scl)
// {
//     _i2c.frequency(100000);
// }

// BQ27220::BQ27220(PinName p_sda, PinName p_scl, int freq) :
//     _i2c(p_sda, p_scl)
// {
//     _i2c.frequency(freq);
// }

// void BQ27220::default_init(BQ27220_TypeDef& dataSTR)
// {
//     dataSTR.shunt_res = BQ_SHUNT_RESISTOR;
// }

// int BQ27220::new_battery_init(BQ27220_TypeDef& dataSTR)
// {
//     return(0);
// }

// uint16_t BQ27220::get_sub_cmmd(BQ27220_TypeDef& dataSTR, uint16_t cmmd)
// {
//     i2c_Buf[0] = BQ_CNTL;
//     i2c_Buf[2] = cmmd >> 8;
//     i2c_Buf[1] = cmmd & 255;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 3, false);
//     Thread::wait(5); // needs large delay here
//     //pc.printf("sub-a: %02x %02x %02x %02x \r\n", i2c_Buf[0], i2c_Buf[1], i2c_Buf[2], i2c_Buf[3]);
//     int i = 0;
//     for(i = 0; i < 100; i++) {
//         wait_us(66);
//         _i2c.write(BQ27220_ADDR, i2c_Buf, 1, true);
//         _i2c.read(BQ27220_ADDR + 1, i2c_Buf, 4, false);
//         //pc.printf("sub-b: %02x %02x %02x %02x \r\n", i2c_Buf[0], i2c_Buf[1], i2c_Buf[2], i2c_Buf[3]);
//         if((i2c_Buf[0]== 0xa5) && (i2c_Buf[1] == 0xff)) break;
//     }
//     wait_us(66);
//     if(i > 98) pc.printf("sub-b: ERROR \r\n");
//     return (i2c_Buf[0] << 8) | i2c_Buf[1];
// }

// uint16_t BQ27220::get_sub_cmmd_s(BQ27220_TypeDef& dataSTR, uint16_t cmmd)
// {
//     i2c_Buf[0] = BQ_CNTL;
//     i2c_Buf[2] = cmmd >> 8;
//     i2c_Buf[1] = cmmd & 255;
//     //pc.printf("sub-a: %02x %02x %02x \r\n", i2c_Buf[0], i2c_Buf[1], i2c_Buf[2]);
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 3, false);
//     wait_us(1066);
//     i2c_Buf[0] = BQ_MACDATA;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 1, true);
//     _i2c.read(BQ27220_ADDR + 1, i2c_Buf, 2, false);
//     wait_us(66);
//     //pc.printf("sub-b: %04x \r\n", (i2c_Buf[0] << 8) | i2c_Buf[1]);
//     return (i2c_Buf[0] << 8) | i2c_Buf[1];
// }

// uint16_t BQ27220::get_reg_2B(BQ27220_TypeDef& dataSTR, uint8_t reg)
// {
//     i2c_Buf[0] = reg;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 1, true);
//     _i2c.read(BQ27220_ADDR + 1, i2c_Buf, 2, false);
//     //pc.printf("sub-b: %02x %02x %02x %02x \r\n", i2c_Buf[0], i2c_Buf[1], i2c_Buf[2], i2c_Buf[3]);
//     wait_us(66);
//     //Thread::wait(2);
//     return (i2c_Buf[1] << 8) | i2c_Buf[0];
// }

// void BQ27220::enter_cfg_update(BQ27220_TypeDef& dataSTR)
// {
//     i2c_Buf[0] = BQ_CNTL;
//     i2c_Buf[1] = 0x90;
//     i2c_Buf[2] = 0x00;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 3, false);
//     wait_us(66);
//     Thread::wait(1500);
// }

// void BQ27220::exitCfgUpdateExit(BQ27220_TypeDef& dataSTR)
// {
//     i2c_Buf[0] = BQ_CNTL;
//     i2c_Buf[1] = BQ_EXIT_CFG_UPDATE & 255;
//     i2c_Buf[2] = BQ_EXIT_CFG_UPDATE >> 8;
//     pc.printf("exitCfg_cmmd: ->  ");
//     for(int i = 0; i < 3; i++) pc.printf("%02x ",i2c_Buf[i]);
//     pc.printf("\r\n");
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 3, false);
//     wait_us(66);
//     Thread::wait(5);
// }

// void BQ27220::exitCfgUpdateReInit(BQ27220_TypeDef& dataSTR)
// {
//     i2c_Buf[0] = BQ_CNTL;
//     i2c_Buf[1] = BQ_EXIT_CFG_UPDATE_REINIT & 255;
//     i2c_Buf[2] = BQ_EXIT_CFG_UPDATE_REINIT >> 8;
//     pc.printf("exitInit_cmmd: ->  ");
//     for(int i = 0; i < 3; i++) pc.printf("%02x ",i2c_Buf[i]);
//     pc.printf("\r\n");
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 3, false);
//     wait_us(66);
//     Thread::wait(5);
// }

// void BQ27220::reset(BQ27220_TypeDef& dataSTR)
// {
//     i2c_Buf[0] = BQ_CNTL;
//     i2c_Buf[1] = BQ_RESET & 255;
//     i2c_Buf[2] = BQ_RESET >> 8;
//     pc.printf("reset_cmmd: ->  ");
//     for(int i = 0; i < 3; i++) pc.printf("%02x ",i2c_Buf[i]);
//     pc.printf("\r\n");
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 3, false);
//     wait_us(66);
//     Thread::wait(5);
// }

// void BQ27220::useProfile_1(BQ27220_TypeDef& dataSTR)
// {
//     i2c_Buf[0] = BQ_CNTL;
//     i2c_Buf[1] = BQ_SET_PROFILE_1 >> 8;
//     i2c_Buf[2] = BQ_SET_PROFILE_1 & 255;;
//     pc.printf("Profile_1_cmmd: ->  ");
//     for(int i = 0; i < 3; i++) pc.printf("%02x ",i2c_Buf[i]);
//     pc.printf("\r\n");
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 3, false);
//     wait_us(66);
//     Thread::wait(200);
// }

// uint16_t BQ27220::get_cs_len(BQ27220_TypeDef& dataSTR, bool pf)
// {
//     i2c_Buf[0] = BQ_MACDATASUM;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 1, true);
//     _i2c.read(BQ27220_ADDR + 1, i2c_Buf, 1, false);
//     wait_us(66);
//     //Thread::wait(5);
//     uint16_t csl = i2c_Buf[0];

//     i2c_Buf[0] = BQ_MACDATALEN;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 1, true);
//     _i2c.read(BQ27220_ADDR + 1, i2c_Buf, 1, false);
//     wait_us(66);
//     //Thread::wait(5);
//     csl = (csl << 8) | i2c_Buf[0];
//     if(pf) pc.printf("get_cs: %02x\r\n", csl >> 8);
//     if(pf) pc.printf("get_ln: %02x\r\n", csl & 255);
//     return(csl);
// }

// uint8_t BQ27220::calc_checksum_rx(BQ27220_TypeDef& dataSTR, int length)
// {
//     uint8_t cs = 0;
//     //pc.printf("c_csum_rx_len: %02x -> ", length);
//     i2c_Buf[0] = BQ_SUB;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 1, true);
//     _i2c.read(BQ27220_ADDR + 1, i2c_Buf, 34, false);
//     wait_us(66);
//     Thread::wait(5);
//     for(int i = 0; i < length + 2; i++) {
//         cs += i2c_Buf[i];
//         //pc.printf("b: %02x cs: %02x  ", i2c_Buf[i], cs);
//     }
//     cs = 255 - cs;
//     //pc.printf("cs_rx:%02x \r\n", cs);
//     return (cs);
// }

// uint8_t BQ27220::calc_checksum_tx(BQ27220_TypeDef& dataSTR, int length)
// {
//     uint8_t cs = 0;
//     pc.printf("cs_tx_len: %02x ->    ", length);
//     for(int i = 0; i < length + 2; i++) {
//         cs += i2c_Buf[i + 1];
//         //pc.printf("i2c: %02x cs: %02x   ", i2c_Buf[i + 1], cs);
//         pc.printf("%02x ", i2c_Buf[i + 1]);
//     }
//     cs = 255 - cs;
//     pc.printf("\r\ncs_tx: %02x\r\n", cs);
//     return (cs);
// }

// uint32_t BQ27220::get_data_32(BQ27220_TypeDef& dataSTR, uint16_t sub_cmmd, int length)
// {
//     i2c_Buf[0] = BQ_SUB;
//     i2c_Buf[2] = sub_cmmd >> 8;
//     i2c_Buf[1] = sub_cmmd & 255;
//     dataSTR.subReg = sub_cmmd;
//     //pc.printf("dat-32a: %02x %02x %02x \r\n", i2c_Buf[0], i2c_Buf[1], i2c_Buf[2]);
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 3, false);
//     wait_us(3066);
//     //Thread::wait(3); //needs to be at least 2

//     dataSTR.checksum = calc_checksum_rx(dataSTR, length);

//     uint16_t cslen = get_cs_len(dataSTR, false);
//     dataSTR.macSumReg = cslen >> 8;
//     dataSTR.macLenReg = cslen & 255;

//     i2c_Buf[0] = BQ_MACDATA;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 1, true);
//     _i2c.read(BQ27220_ADDR + 1, i2c_Buf, length, false);
//     wait_us(5066);
//     //Thread::wait(5); //seems to work down to 1
//     for(int i = 0; i < length; i++) {
//         dataSTR.macData[i] = dataSTR.i2c_Bufx[i];
//         pc.printf("%02x,", dataSTR.macData[i]);
//     }
//     pc.printf("\r\n");
//     //pc.printf(" mdl: %02x,  mdcs: %02x,  ccs: %02x\r\n", dataSTR.macLenReg, dataSTR.macSumReg, dataSTR.checksum);
//     return ((uint32_t)dataSTR.subReg);
// }

// //#define BQ_SHORT          1

// void BQ27220::change_ram_1_2_4(BQ27220_TypeDef& dataSTR, uint16_t sub_cmmd, uint32_t value, int qty, bool pre)
// {
//     if(pre) {
// #ifndef BQ_SHORT
//         pc.printf("ram124_a: %04x ->         ", sub_cmmd);
//         get_data_32(dataSTR, sub_cmmd, 32);
// #endif

//         if(qty == 1) {
//             dataSTR.macData[0] = value & 255;
// #ifdef BQ_SHORT
//             dataSTR.macData[1] = 0;
//             dataSTR.macData[2] = 0;
//             dataSTR.macData[3] = 0;
// #endif
//         } else
//         if(qty == 2) {
//             dataSTR.macData[0] = (value >> 8) & 255;
//             dataSTR.macData[1] = value & 255;
// #ifdef BQ_SHORT
//             dataSTR.macData[2] = 0;
//             dataSTR.macData[3] = 0;
// #endif
//         } else
//         if(qty == 4) {
//             dataSTR.macData[0] = (value >> 24) & 255;
//             dataSTR.macData[1] = (value >> 16) & 255;
//             dataSTR.macData[2] = (value >> 8) & 255;
//             dataSTR.macData[3] = value & 255;
//         } else {
//             pc.printf("ram124_q_error\r\n");
//             return;
//         }
//     }

//     i2c_Buf[0] = BQ_SUB;
//     i2c_Buf[1] = sub_cmmd >> 8;
//     i2c_Buf[2] = sub_cmmd & 255;
//     if(pre) {
//         i2c_Buf[3] = dataSTR.macData[0];
//         i2c_Buf[4] = dataSTR.macData[1];
//         i2c_Buf[5] = dataSTR.macData[2];
//         i2c_Buf[6] = dataSTR.macData[3];
//     }
//     pc.printf("ram124_cmmd: ->  ");
//     int i = 0;
// #ifdef BQ_SHORT
//     for(i = 0; i < qty + 3; i++) pc.printf("%02x ",i2c_Buf[i]);
//     pc.printf("\r\n");
//     uint8_t x = calc_checksum_tx(dataSTR, qty);
//     _i2c.write(BQ27220_ADDR, i2c_Buf, qty + 3, false);
// #else
//     for(i = 0; i < 32; i++) i2c_Buf[i + 3] = dataSTR.macData[i];
//     for(i = 0; i < 35; i++) pc.printf("%02x ",i2c_Buf[i]);
//     pc.printf("\r\n");
//     uint8_t x = calc_checksum_tx(dataSTR, 32);
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 35, false);
// #endif
//     wait_us(66);
//     //Thread::wait(5);

//     i2c_Buf[0] = BQ_MACDATASUM;
//     i2c_Buf[1] = x;
// #ifndef BQ_SHORT
//     i2c_Buf[1] -= 0x20; //why is this???? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//     if((sub_cmmd >= 0x91e0) && (sub_cmmd < BQ_CONFIG_TAPER_CURR)) i2c_Buf[1]--;
// #endif
//     pc.printf("ram124_cs:   ->  ");
//     for(i = 0; i < 2; i++) pc.printf("%02x ",i2c_Buf[i]);
//     pc.printf("\r\n");
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 2, false);
//     wait_us(66);
//     //Thread::wait(5);

//     i2c_Buf[0] = BQ_MACDATALEN;
// #ifdef BQ_SHORT
//     i2c_Buf[1] = qty + 4;
// #else
//     i2c_Buf[1] = 36;
// #endif
//     pc.printf("ram124_len:  ->  ");
//     for(i = 0; i < 2; i++) pc.printf("%02x ",i2c_Buf[i]);
//     pc.printf("\r\n");
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 2, false);
//     wait_us(5066);
//     //Thread::wait(200);

//     get_cs_len(dataSTR, true);
//     //pc.printf("\r\n");

// #ifndef BQ_SHORT
//     pc.printf("ram124_x: %04x ->         ", sub_cmmd);
//     get_data_32(dataSTR, sub_cmmd, 32);
//     pc.printf("\r\n");
// #endif
// }

// uint16_t BQ27220::get_16(BQ27220_TypeDef& dataSTR, uint16_t cmmd)
// {
//     get_sub_cmmd_s(dataSTR, cmmd);
//     i2c_Buf[0] = BQ_MACDATA;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 1, true);
//     _i2c.read(BQ27220_ADDR + 1, i2c_Buf, 2, false);
//     wait_us(66);
//     return (i2c_Buf[0] << 8) | i2c_Buf[1];
// }

// uint8_t BQ27220::get_8(BQ27220_TypeDef& dataSTR, uint16_t cmmd)
// {
//     //pc.printf("get_8: %04x\r\n", cmmd);
//     get_sub_cmmd_s(dataSTR, cmmd);
//     i2c_Buf[0] = BQ_MACDATA;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 1, true);
//     _i2c.read(BQ27220_ADDR + 1, i2c_Buf, 1, false);
//     wait_us(66);
//     return i2c_Buf[0];
// }

// void BQ27220::seal(BQ27220_TypeDef& dataSTR)
// {
//     i2c_Buf[0] = BQ_CNTL;
//     i2c_Buf[1] = 0x30;
//     i2c_Buf[2] = 0x00;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 3, false);
//     wait_us(66);
//     wait_ms(5);
//     //Thread::wait(5);
// }

// void BQ27220::unseal(BQ27220_TypeDef& dataSTR)
// {
//     i2c_Buf[0] = BQ_CNTL;
//     i2c_Buf[1] = 0x14;
//     i2c_Buf[2] = 0x04;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 3, false);
//     wait_us(66);
//     wait_ms(5);
//     //Thread::wait(5);

//     i2c_Buf[0] = BQ_CNTL;
//     i2c_Buf[1] = 0x72;
//     i2c_Buf[2] = 0x36;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 3, false);
//     wait_ms(5);
//     //Thread::wait(5);
// }

// void BQ27220::full_access(BQ27220_TypeDef& dataSTR)
// {
//     i2c_Buf[0] = BQ_CNTL;
//     i2c_Buf[1] = 0xff;
//     i2c_Buf[2] = 0xff;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 3, false);
//     wait_us(66);
//     wait_ms(5);
//     //Thread::wait(5);

//     i2c_Buf[0] = BQ_CNTL;
//     i2c_Buf[1] = 0xff;
//     i2c_Buf[2] = 0xff;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 3, false);
//     wait_us(66);
//     wait_ms(5);
//     //Thread::wait(5);
// }

// uint32_t BQ27220::get_dev_id(BQ27220_TypeDef& dataSTR)
// {
//     uint16_t dat = get_sub_cmmd(dataSTR, BQ_DEVICE_NUMBER);
//     //pc.printf("dat-idq: %04x \r\n", dat);
//     if(dat != 0xa5ff) return(dat);
//     i2c_Buf[0] = BQ_SUB;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 1, true);
//     _i2c.read(BQ27220_ADDR + 1, i2c_Buf, 4, false);
//     uint32_t id = (i2c_Buf[0] << 24) | (i2c_Buf[1] << 16) | (i2c_Buf[2] << 8) | i2c_Buf[3];
//     //pc.printf("dat-idq: %08x \r\n", id);
//     wait_us(66);
//     return(id) ;
// }

// uint32_t BQ27220::get_fw_rev(BQ27220_TypeDef& dataSTR)
// {
//     uint16_t dat = get_sub_cmmd(dataSTR, BQ_FW_VERSION);
//     //pc.printf("dat-fwq: %04x \r\n", dat);
//     if(dat != 0xa5ff) return(dat);
//     i2c_Buf[0] = BQ_SUB;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 1, true);
//     _i2c.read(BQ27220_ADDR + 1, i2c_Buf, 34, false);
//     wait_us(66);
//     return (i2c_Buf[0] << 24) | (i2c_Buf[1] << 16) | (i2c_Buf[2] << 8) | i2c_Buf[3];
// }

// uint32_t BQ27220::get_hw_rev(BQ27220_TypeDef& dataSTR)
// {
//     uint16_t dat = get_sub_cmmd(dataSTR, BQ_HW_VERSION);
//     //pc.printf("dat-fwq: %04x \r\n", dat);
//     if(dat != 0xa5ff) return(dat);
//     i2c_Buf[0] = BQ_SUB;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 1, true);
//     _i2c.read(BQ27220_ADDR + 1, i2c_Buf, 34, false);
//     wait_us(66);
//     return (i2c_Buf[0] << 24) | (i2c_Buf[1] << 16) | (i2c_Buf[2] << 8) | i2c_Buf[3];
// }

// void BQ27220::set_ntc_as_sensor(BQ27220_TypeDef& dataSTR, bool ntc)
// {
//     uint16_t res = get_16(dataSTR, BQ_CONFIG_OP_CONFIG_A);
//         pc.printf("s_ntc: %04x ", res);

//     if(!(ntc)) {
//         pc.printf(" N ");
//         res &= ~BQ_BIT_OCA_TEMPS;
//         res |=  BQ_BIT_OCA_BIE;

//     } else {
//         pc.printf(" L ");
//         res &= ~BQ_BIT_OCA_BIE;
//         res |=  BQ_BIT_OCA_TEMPS;
//     }

//     pc.printf("new: %04x\r\n", res);
//     change_ram_1_2_4(dataSTR, BQ_CONFIG_OP_CONFIG_A - 0x20, (uint32_t)res, 2, true);
// }
// /*
// void BQ27220::set_reg(BQ27220_TypeDef& dataSTR, uint16_t reg, uint16_t da, int byt)
// {
//     uint16_t res = get_16(dataSTR, reg);
//     change_ram_1_2_4(dataSTR, reg, (uint32_t)da, byt, true);
// }
// */
// /*
// uint16_t BQ27220::get_OS_reg(BQ27220_TypeDef& dataSTR)
// {
//     i2c_Buf[0] = BQ_OS;
//     _i2c.write(BQ27220_ADDR, i2c_Buf, 1, true);
//     _i2c.read(BQ27220_ADDR + 1, i2c_Buf, 2, false);
//     wait_us(66);
//     dataSTR.osReg = (i2c_Buf[1] << 8) | i2c_Buf[0];
//     return(dataSTR.osReg);
// }
// */

// int BQ27220::read_registers(BQ27220_TypeDef& dataSTR)
// {
//     i2c_Buf[0] = BQ_CNTL;
//     int result = _i2c.write(BQ27220_ADDR, i2c_Buf, 1, true);
//     if(result) return(result + 0x10);
//     result = _i2c.read(BQ27220_ADDR + 1, i2c_Buf, 32, false);
//     wait_us(66);
//     if(result) return(result + 0x18);
//     //Thread::wait(1);

//     dataSTR.cntlReg =    (i2c_Buf[BQ_CNTL    - BQ_CNTL + 1] << 8) | i2c_Buf[BQ_CNTL    - BQ_CNTL];
//     dataSTR.arReg =      (i2c_Buf[BQ_AR      - BQ_CNTL + 1] << 8) | i2c_Buf[BQ_AR      - BQ_CNTL];
//     dataSTR.artteReg =   (i2c_Buf[BQ_ARTTE   - BQ_CNTL + 1] << 8) | i2c_Buf[BQ_ARTTE   - BQ_CNTL];
//     dataSTR.tempReg =    (i2c_Buf[BQ_TEMP    - BQ_CNTL + 1] << 8) | i2c_Buf[BQ_TEMP    - BQ_CNTL];
//     dataSTR.voltReg =    (i2c_Buf[BQ_VOLT    - BQ_CNTL + 1] << 8) | i2c_Buf[BQ_VOLT    - BQ_CNTL];
//     dataSTR.flagsReg =   (i2c_Buf[BQ_FLAGS   - BQ_CNTL + 1] << 8) | i2c_Buf[BQ_FLAGS   - BQ_CNTL];
//     dataSTR.currentReg = (i2c_Buf[BQ_CURRENT - BQ_CNTL + 1] << 8) | i2c_Buf[BQ_CURRENT - BQ_CNTL];

//     dataSTR.rmReg =      (i2c_Buf[BQ_RM      - BQ_CNTL + 1] << 8) | i2c_Buf[BQ_RM      - BQ_CNTL];
//     dataSTR.fccReg =     (i2c_Buf[BQ_FCC     - BQ_CNTL + 1] << 8) | i2c_Buf[BQ_FCC     - BQ_CNTL];
//     dataSTR.aiReg =      (i2c_Buf[BQ_AI      - BQ_CNTL + 1] << 8) | i2c_Buf[BQ_AI      - BQ_CNTL];
//     dataSTR.tteReg =     (i2c_Buf[BQ_TTE     - BQ_CNTL + 1] << 8) | i2c_Buf[BQ_TTE     - BQ_CNTL];
//     dataSTR.ttfReg =     (i2c_Buf[BQ_TTF     - BQ_CNTL + 1] << 8) | i2c_Buf[BQ_TTF     - BQ_CNTL];
//     dataSTR.siReg =      (i2c_Buf[BQ_SI      - BQ_CNTL + 1] << 8) | i2c_Buf[BQ_SI      - BQ_CNTL];
//     dataSTR.stteReg =    (i2c_Buf[BQ_STTE    - BQ_CNTL + 1] << 8) | i2c_Buf[BQ_STTE    - BQ_CNTL];
//     dataSTR.mliReg =     (i2c_Buf[BQ_MLI     - BQ_CNTL + 1] << 8) | i2c_Buf[BQ_MLI     - BQ_CNTL];

//     i2c_Buf[0] = BQ_MLTTE;
//     result = _i2c.write(BQ27220_ADDR, i2c_Buf, 1, true);
//     //if(result) return(result + 0x20);
//     result =_i2c.read(BQ27220_ADDR + 1, i2c_Buf, 32, false);
//     wait_us(66);
//     //Thread::wait(1);
//     //if(result) return(result + 0x28);

//     dataSTR.mltteReg =   (i2c_Buf[BQ_MLTTE   -  BQ_MLTTE + 1] << 8) | i2c_Buf[BQ_MLTTE   -  BQ_MLTTE];
//     dataSTR.rawccReg =   (i2c_Buf[BQ_RCC     -  BQ_MLTTE + 1] << 8) | i2c_Buf[BQ_RCC     -  BQ_MLTTE];
//     dataSTR.apReg =      (i2c_Buf[BQ_AP      -  BQ_MLTTE + 1] << 8) | i2c_Buf[BQ_AP      -  BQ_MLTTE];
//     dataSTR.intTempReg = (i2c_Buf[BQ_INTTEMP -  BQ_MLTTE + 1] << 8) | i2c_Buf[BQ_INTTEMP -  BQ_MLTTE];
//     dataSTR.cycReg =     (i2c_Buf[BQ_CYC     -  BQ_MLTTE + 1] << 8) | i2c_Buf[BQ_CYC     -  BQ_MLTTE];
//     dataSTR.socReg =     (i2c_Buf[BQ_SOC     -  BQ_MLTTE + 1] << 8) | i2c_Buf[BQ_SOC     -  BQ_MLTTE];
//     dataSTR.sohReg =     (i2c_Buf[BQ_SOH     -  BQ_MLTTE + 1] << 8) | i2c_Buf[BQ_SOH     -  BQ_MLTTE];

//     dataSTR.cvReg =      (i2c_Buf[BQ_CV      -  BQ_MLTTE + 1] << 8) | i2c_Buf[BQ_CV      -  BQ_MLTTE];
//     dataSTR.ccReg =      (i2c_Buf[BQ_CC      -  BQ_MLTTE + 1] << 8) | i2c_Buf[BQ_CC      -  BQ_MLTTE];
//     dataSTR.btpdReg =    (i2c_Buf[BQ_BTPD    -  BQ_MLTTE + 1] << 8) | i2c_Buf[BQ_BTPD    -  BQ_MLTTE];
//     dataSTR.btpcReg =    (i2c_Buf[BQ_BTPC    -  BQ_MLTTE + 1] << 8) | i2c_Buf[BQ_BTPC    -  BQ_MLTTE];
//     dataSTR.osReg =      (i2c_Buf[BQ_OS      -  BQ_MLTTE + 1] << 8) | i2c_Buf[BQ_OS      -  BQ_MLTTE];
//     dataSTR.dcReg =      (i2c_Buf[BQ_DC      -  BQ_MLTTE + 1] << 8) | i2c_Buf[BQ_DC      -  BQ_MLTTE];
//     dataSTR.subReg =     (i2c_Buf[BQ_SUB     -  BQ_MLTTE + 1] << 8) | i2c_Buf[BQ_SUB     -  BQ_MLTTE];

//     i2c_Buf[0] = BQ_MACDATA;
//     result = _i2c.write(BQ27220_ADDR, i2c_Buf, 1, true);
//     //if(result) return(result + 0x30);
//     result =_i2c.read(BQ27220_ADDR + 1, i2c_Buf, 32, false);
//     wait_us(66);
//     //Thread::wait(1);
//     //if(result) return(result + 0x38);

//     for(int i = 0; i < 32; i++) {
//         dataSTR.macData[i] = i2c_Buf[i];
//     }

//     i2c_Buf[0] = BQ_MACDATASUM;
//     result = _i2c.write(BQ27220_ADDR, i2c_Buf, 1, true);
//     //if(result) return(result + 0x40);
//     result =_i2c.read(BQ27220_ADDR + 1, i2c_Buf, 32, false);
//     wait_us(66);
//     //Thread::wait(1);
//     //if(result) return(result + 0x48);

//     dataSTR.macSumReg = (i2c_Buf[BQ_MACDATASUM  -  BQ_MACDATASUM + 1] << 8) | i2c_Buf[BQ_MACDATASUM  -  BQ_MACDATASUM];
//     dataSTR.macLenReg = (i2c_Buf[BQ_MACDATALEN  -  BQ_MACDATASUM + 1] << 8) | i2c_Buf[BQ_MACDATALEN  -  BQ_MACDATASUM];
//     dataSTR.anacReg =    i2c_Buf[BQ_ANACNT      -  BQ_MACDATASUM];
//     dataSTR.rawcReg =   (i2c_Buf[BQ_RAWC        -  BQ_MACDATASUM + 1] << 8) | i2c_Buf[BQ_RAWC        -  BQ_MACDATASUM];
//     dataSTR.rawvReg =   (i2c_Buf[BQ_RAWV        -  BQ_MACDATASUM + 1] << 8) | i2c_Buf[BQ_RAWV        -  BQ_MACDATASUM];
//     dataSTR.rawtReg =   (i2c_Buf[BQ_RAWT        -  BQ_MACDATASUM + 1] << 8) | i2c_Buf[BQ_RAWT        -  BQ_MACDATASUM];

//     return(0);
// }