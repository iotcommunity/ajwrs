/*
 * ENS210.c
 *
 *  Created on: Dec 6, 2016
 *      Author: lycan
 */

#include "hal_data.h"
#include "ENS210.h"
#include "I2C.h"
#include "Timer.h"
#include <stdio.h>

ssp_err_t ENS210Initialize(void)
{
    return SSP_SUCCESS;

}

ssp_err_t ENS210Open(uint8_t address)
{
    ssp_err_t error;

    // Disable Low Power Mode
    uint8_t sys_ctrl[] = {0x10, 0x00};
    error = I2CWrite(address, sys_ctrl, 2, false);
    if (error != SSP_SUCCESS)
        return error;

    // Wait for sensor to be active
    uint8_t sys_stat = 0x00;
    while((sys_stat & 0x01) == 0)
    {
        error = I2CWriteByte(address, 0x11, true);
        if (error != SSP_SUCCESS)
            return error;
        error = I2CReadByte(address, &sys_stat, false);
        if (error != SSP_SUCCESS)
            return error;

        printf(".");
        TimerSleepMs(50);
    }

    // Enable Continuous Measurements
    uint8_t sens_run[] = {0x21, 0x03};
    error = I2CWrite(address, sens_run, 2, false);
    if (error != SSP_SUCCESS)
        return error;

    // Start Measurements
    uint8_t sens_start[] = {0x22, 0x03};
    error = I2CWrite(address, sens_start, 2, false);
    if (error != SSP_SUCCESS)
        return error;

    return SSP_SUCCESS;
}

ssp_err_t ENS210ChipId(uint8_t address, uint16_t * const chipId)
{
    ssp_err_t error;

    error = I2CWriteByte(address, 0x00, true);
    if (error != SSP_SUCCESS)
        return error;

    error = I2CRead(address, (uint8_t * const )chipId, 2, false);
    if (error != SSP_SUCCESS)
        return error;

    return SSP_SUCCESS;
}

ssp_err_t ENS210UpdateSensors(uint8_t address, ENS210Data * const data)
{
    ssp_err_t error;

    uint8_t buffer[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint16_t rawTemp;
    uint16_t rawHumi;

    error = I2CWriteByte(address, 0x30, true);
    if (error != SSP_SUCCESS)
        return error;
    error = I2CRead(address, buffer, 6, false);
    if (error != SSP_SUCCESS)
        return error;

    // Cast Data
    rawTemp = (uint16_t)((buffer[1] << 8) | buffer[2]);
    rawHumi = (uint16_t)((buffer[4] << 8) | buffer[5]);

    data->Temperature = (float)((rawTemp / 64.0) - 273.15);
    data->Humidity = (float)((rawHumi / 512.0));

    return SSP_SUCCESS;
}
