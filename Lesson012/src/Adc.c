#include "hal_data.h"
#include "Defines.h"
#include "Adc.h"

float potentiometerVoltage;
float potentiometerResistance;
float potentiometerPercent;
float temperatureDegreesC;
float temperatureDegreesF;
float temperatureVoltage;
float vrefVoltage;

ssp_err_t AdcInitialize()
{
    ssp_err_t error;

    potentiometerVoltage = 0;
    potentiometerResistance = 0;
    potentiometerPercent = 0;
    temperatureDegreesC = 0;
    temperatureDegreesF = 0;
    temperatureVoltage = 0;
    vrefVoltage = 0;

    // Open the ADC
    error = g_adc.p_api->open (g_adc.p_ctrl, g_adc.p_cfg);
    if (error != SSP_SUCCESS)
         return error;

    // Configure Scan
    error = g_adc.p_api->scanCfg (g_adc.p_ctrl, g_adc.p_channel_cfg);
    if (error != SSP_SUCCESS)
         return error;

    // Start ADC Scan
    error = g_adc.p_api->scanStart (g_adc.p_ctrl);
    if (error != SSP_SUCCESS)
         return error;

    return SSP_SUCCESS;
}

float AdcGetPotentiometerVoltage()
{
    return potentiometerVoltage;
}

float AdcGetPotentiometerResistance()
{
    return potentiometerResistance;
}

float AdcGetPotentiometerPercent()
{
    return potentiometerPercent;
}

float AdcGetTemperatureVoltage()
{
    return temperatureVoltage;
}

float AdcGetTemperatureDegreesC()
{
    return temperatureDegreesC;
}

float AdcGetTemperatureDegreesF()
{
    return temperatureDegreesF;
}

float AdcGetVrefVoltage()
{
    return vrefVoltage;
}

ssp_err_t AdcRead()
{
    adc_data_size_t adcCounts;
    adc_data_size_t temperatureCounts;
    adc_data_size_t internalVRefCounts;

    ssp_err_t error;

    // Start ADC Scan
    error = g_adc.p_api->scanStart (g_adc.p_ctrl);
    if (error != SSP_SUCCESS)
         return error;

    while(g_adc.p_api->scanStatusGet (g_adc.p_ctrl) != SSP_SUCCESS)
    {
    }

    // Read ADC
    error = g_adc.p_api->read (g_adc.p_ctrl, POT_CHANNEL, &adcCounts);
    if (error != SSP_SUCCESS)
         return error;

    // Calculate Voltage
    potentiometerVoltage = ((adcCounts * 3.3f) / 4095.0f);
    potentiometerPercent = potentiometerVoltage / 3.30f * 100.0f;
    potentiometerResistance = potentiometerVoltage / 3.30f * 10000.0f;

    error = g_adc.p_api->read (g_adc.p_ctrl, TEMP_CHANNEL, &temperatureCounts);
    if (error != SSP_SUCCESS)
         return error;

    // Calculate Temperature
    temperatureVoltage = ((temperatureCounts * 3.3f) / 4095.0f);
    temperatureDegreesC = (float)(((temperatureVoltage - 1.24f) / 0.0041f) + 25.0f);
    temperatureDegreesF = (float)(((temperatureDegreesC * 9.0f) / 5.0f) + 32.0f);

    error = g_adc.p_api->read (g_adc.p_ctrl, INTERNAL_VREF_CHANNEL, &internalVRefCounts);
    if (error != SSP_SUCCESS)
         return error;

    // Calculate VREF Voltage
    vrefVoltage = ((internalVRefCounts * 3.3f) / 4095.0f);

    return SSP_SUCCESS;
}
