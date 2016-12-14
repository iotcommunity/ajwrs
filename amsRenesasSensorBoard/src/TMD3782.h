/*
 * TMD3782.h
 *
 *  Created on: Dec 12, 2016
 *      Author: lycan
 */

#ifndef TMD3782_H_
#define TMD3782_H_

typedef struct _TMD3782Data
{
    uint16_t Clear;
    uint16_t Red;
    uint16_t Green;
    uint16_t Blue;
    uint16_t Proximity;
} TMD3782Data;

ssp_err_t TMD3782Initialize(void);
ssp_err_t TMD3782Open(uint8_t address);
ssp_err_t TMD3782ChipId(uint8_t address, uint8_t * const chipId);
ssp_err_t TMD3782Status(uint8_t address, uint8_t * const status);
ssp_err_t TMD3782UpdateSensors(uint8_t address, TMD3782Data * const data);
ssp_err_t TMD3782ProximityInterruptClear(uint8_t address);
ssp_err_t TMD3782ClearInterruptClear(uint8_t address);
ssp_err_t TMD3782ProximityAndInterruptClear(uint8_t address);


#endif /* TMD3782_H_ */
