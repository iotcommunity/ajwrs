#ifndef ADC_H_
#define ADC_H_

#define POT_CHANNEL 0
#define TEMP_CHANNEL 28
#define INTERNAL_VREF_CHANNEL 29

ssp_err_t AdcInitialize();
ssp_err_t AdcRead();
float AdcGetPotentiometerVoltage();
float AdcGetPotentiometerResistance();
float AdcGetPotentiometerPercent();
float AdcGetTemperatureVoltage();
float AdcGetTemperatureDegreesC();
float AdcGetTemperatureDegreesF();
float AdcGetVrefVoltage();

#endif /* ADC_H_ */
