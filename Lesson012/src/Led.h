#ifndef LED_H_
#define LED_H_

#include "hal_data.h"

ssp_err_t LedInitialize();
void LedTurnOn();
void LedTurnOff();
void LedWalk(int direction);
int LedGetSpeed();
void LedWalk();

#endif /* LED_H_ */
