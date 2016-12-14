/*
 * ENS210.h
 *
 *  Created on: Dec 6, 2016
 *      Author: lycan
 */

#ifndef ENS210_H_
#define ENS210_H_

typedef struct _ENS210Data
{
    float Temperature;
    float Humidity;
} ENS210Data;

ssp_err_t ENS210Initialize(void);
ssp_err_t ENS210Open(uint8_t address);
ssp_err_t ENS210ChipId(uint8_t address, uint16_t * const chipId);
ssp_err_t ENS210UpdateSensors(uint8_t address, ENS210Data * const data);


#endif /* ENS210_H_ */
