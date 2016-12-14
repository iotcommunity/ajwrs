/*
 * AS3935.h
 *
 *  Created on: Dec 6, 2016
 *      Author: lycan
 */

#ifndef AS3935_H_
#define AS3935_H_

typedef struct _as3935Data
{
    // 0x00
    uint8_t:2;
    uint8_t AFE_GB:5;
    uint8_t PWD:1;

    // 0x01
    uint8_t:1;
    uint8_t NF_LEV:3;
    uint8_t WDTH:4;

    //0x02
    uint8_t:1;
    uint8_t CL_STAT:1;
    uint8_t MIN_NUM_LIGH:2;
    uint8_t SREJ:4;

    //0x03
    uint8_t LCO_FDIV:2;
    uint8_t MASK_DIST:1;
    uint8_t:1;
    uint8_t INT:4;

    //0x04
    uint8_t S_LIG_L;

    // 0x05
    uint8_t S_LIG_M;

    //0x06
    uint8_t:3;
    uint8_t S_LIG_MM:5;

    //0x07
    uint8_t:2;
    uint8_t DISTANCE:6;

    //0x08
    uint8_t DISP_LCO:1;
    uint8_t DISP_SRCO:1;
    uint8_t DISP_TRCO:1;
    uint8_t:1;
    uint8_t TUN_CAP:4;

    //0x3A
    uint8_t TRCO_CALIB_DONE:1;
    uint8_t TRCO_CALIB_NOK:2;
    uint8_t:6;


    //0x3B
    uint8_t SRCO_CALIB_DONE:1;
    uint8_t SRCO_CALIB_NOK:1;
    uint8_t:6;
} AS3935Data;

ssp_err_t AS3935Initialize(void);
ssp_err_t AS3935UpdateSensors(uint8_t address, AS3935Data * const data);


#endif /* AS3935_H_ */
