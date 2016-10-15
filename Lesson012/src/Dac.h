#ifndef DAC_H_
#define DAC_H_

ssp_err_t DacInitialize();
ssp_err_t DacWrite(float value);

#endif /* DAC_H_ */
