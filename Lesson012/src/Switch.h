/*
 * Switch.h
 *
 *  Created on: Oct 1, 2016
 *      Author: lycan
 */

#ifndef SWITCH_H_
#define SWITCH_H_


ssp_err_t SwitchInitialize();
ssp_err_t SwitchRead();
int SwitchGetState(int switchNumber);
const char * SwitchGetStateText(int switchState);

#endif /* SWITCH_H_ */
