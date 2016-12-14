/*
 * AMS_iAQ_Core_C.c
 *
 *  Created on: Dec 6, 2016
 *      Author: lycan
 */

#include "hal_data.h"
#include "I2C.h"
#include "AMS_iAQ_Core_C.h"


ssp_err_t AMSiAQCoreInitialize(void)
{
    return SSP_SUCCESS;
}

ssp_err_t AMSiAQCoreUpdateSensors(uint8_t address, AMSiAQCoreData * const data)
{
    ssp_err_t error;
    uint8_t amsData[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    error = I2CRead(address, amsData, 9, false);
    if (error != SSP_SUCCESS)
        return error;

    data->Prediction = (uint16_t)((amsData[0] << 8) + amsData[1]);
    data->Status = amsData[2];
    data->Resistance = (uint32_t)((amsData[3] << 24) | (amsData[4] << 16) | (amsData[5] << 8) | (amsData[6]));
    data->TVOC = (uint16_t)((amsData[7] << 8) + amsData[8]);

    return SSP_SUCCESS;
}
