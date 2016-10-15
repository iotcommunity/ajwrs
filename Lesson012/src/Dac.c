#include "hal_data.h"
#include "Defines.h"
#include "Dac.h"

ssp_err_t DacInitialize()
{
    ssp_err_t error;

    // Open DAC
    error = g_dac.p_api->open (g_dac.p_ctrl, g_dac.p_cfg);
    if (error != SSP_SUCCESS)
         return error;

    error = g_dac.p_api->start (g_dac.p_ctrl);
    if (error != SSP_SUCCESS)
         return error;

    return SSP_SUCCESS;
}

ssp_err_t DacWrite(float value)
{
    ssp_err_t error;
    dac_size_t dacValue;

    // Check Bounds of Value
    if (value > 3.30)
        value = 3.30f;

    if (value < 0)
        value = 0.0f;

    dacValue = (dac_size_t)((value / 3.30f) * 4095);

    error = g_dac.p_api->write (g_dac.p_ctrl, dacValue);
    if (error != SSP_SUCCESS)
          return error;

    return SSP_SUCCESS;
}

