/*
 * TMD3782.c
 *
 *  Created on: Dec 12, 2016
 *      Author: lycan
 */
#include "hal_data.h"
#include "I2C.h"
#include "TMD3782.h"

ssp_err_t TMD3782Initialize(void)
{
    return SSP_SUCCESS;
}

ssp_err_t TMD3782ChipId(uint8_t address, uint8_t * const chipId)
{
    ssp_err_t error;
    error = I2CReadRegister(address, 0b10000000 | 0x12, chipId, 1, false);
    if (error != SSP_SUCCESS)
        return error;

    return SSP_SUCCESS;
}
ssp_err_t TMD3782Open(uint8_t address)
{
    ssp_err_t error;

    // Write to address 0x00, WEN, PEN, AEN, PON = 1
    error = I2CWriteRegister(address, 0b10000000 | 0x00, 0b00001111, false);
    if (error != SSP_SUCCESS)
        return error;

    // Write to address 0x01, 64 cycle integration time
    error = I2CWriteRegister(address, 0b10000000 | 0x01, 0xC0, false);
    if (error != SSP_SUCCESS)
        return error;

    // Write to address 0x0E, 30 pulses
    error = I2CWriteRegister(address, 0b10000000 | 0x0E, 30, false);
    if (error != SSP_SUCCESS)
        return error;

    return SSP_SUCCESS;
}

ssp_err_t TMD3782Status(uint8_t address, uint8_t * const status)
{
    ssp_err_t error;
    error = I2CReadRegister(address, 0b10000000 | 0x13, status, 1, false);

    return SSP_SUCCESS;
}

ssp_err_t TMD3782UpdateSensors(uint8_t address, TMD3782Data * const data)
{
    ssp_err_t error;
    uint8_t rawData[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    error = I2CReadRegister(address, 0b10100000 | 0x14, rawData, 10, false);
    if (error != SSP_SUCCESS)
        return error;

    data->Clear = (uint16_t)((rawData[1] << 8) | rawData[0]);
    data->Red = (uint16_t)((rawData[3] << 8) | rawData[2]);
    data->Green = (uint16_t)((rawData[5] << 8) | rawData[4]);
    data->Blue = (uint16_t)((rawData[7] << 8) | rawData[6]);
    data->Proximity = (uint16_t)((rawData[9] << 8) | rawData[8]);

    return SSP_SUCCESS;
}

ssp_err_t TMD3782ProximityInterruptClear(uint8_t address)
{
    ssp_err_t error;
    error = I2CWriteByte(address, 0b11100101, false);
    if (error != SSP_SUCCESS)
        return error;

    return SSP_SUCCESS;
}

ssp_err_t TMD3782ClearInterruptClear(uint8_t address)
{
    ssp_err_t error;
    error = I2CWriteByte(address, 0b11100110, false);
    if (error != SSP_SUCCESS)
        return error;

    return SSP_SUCCESS;
}

ssp_err_t TMD3782ProximityAndInterruptClear(uint8_t address)
{
    ssp_err_t error;
    error = I2CWriteByte(address, 0b11100111, false);
    if (error != SSP_SUCCESS)
        return error;

    return SSP_SUCCESS;
}

