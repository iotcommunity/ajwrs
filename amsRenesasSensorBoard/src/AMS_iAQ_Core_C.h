/*
 * AMS_iAQ_Core_C.h
 *
 *  Created on: Dec 6, 2016
 *      Author: lycan
 */

#ifndef AMS_IAQ_CORE_C_H_
#define AMS_IAQ_CORE_C_H_

typedef struct _AMSiAQCoreData
{
    uint16_t Prediction;
    uint8_t Status;
    uint32_t Resistance;
    uint16_t TVOC;
} AMSiAQCoreData;
ssp_err_t AMSiAQCoreInitialize(void);
ssp_err_t AMSiAQCoreUpdateSensors(uint8_t address, AMSiAQCoreData * const data);

#endif /* AMS_IAQ_CORE_C_H_ */
