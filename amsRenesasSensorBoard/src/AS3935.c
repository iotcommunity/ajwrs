/*
 * AS3935.c
 *
 *  Created on: Dec 6, 2016
 *      Author: lycan
 */


#include "hal_data.h"
#include "I2C.h"
#include "AS3935.h"

ssp_err_t AS3935Initialize(void)
{
    return SSP_SUCCESS;
}

ssp_err_t AS3935UpdateSensors(uint8_t address, AS3935Data * const data)
{
    ssp_err_t error;
    //uint8_t rawData[11] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


    error = I2CWriteByte(address, 0x00, false);
    if (error != SSP_SUCCESS)
        return error;

    error = I2CRead(address, (uint8_t *)data, 9, false);
    if (error != SSP_SUCCESS)
        return error;

    error = I2CWriteByte(address, 0x3A, false);
    if (error != SSP_SUCCESS)
        return error;

    error = I2CRead(address, (uint8_t *)(data + 9), 2, false);
    if (error != SSP_SUCCESS)
        return error;

    //memcpy(data, rawData, 11);

    return SSP_SUCCESS;
}
