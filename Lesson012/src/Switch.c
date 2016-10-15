/*
 * Switch.c
 *
 *  Created on: Oct 1, 2016
 *      Author: lycan
 */


#include "hal_data.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "Defines.h"
#include "Switch.h"


int switchS1State;
int switchS2State;
int switchS3State;

ssp_err_t SwitchInitialize()
{
    switchS1State = -1;
    switchS2State = -1;
    switchS3State = -1;

    return SSP_SUCCESS;
}

int SwitchGetState(int switchNumber)
{
    switch (switchNumber)
    {
        case 1:
            return switchS1State;
        case 2:
            return switchS2State;
        case 3:
            return switchS3State;
        default:
            return -1;
    }
}

const char * SwitchGetStateText(int switchState)
{
    switch (switchState)
    {
        case -1: return "Unknown";
        case 0: return "Not Pressed";
        case 1: return "Pressed";
        default: return "Unknown";
    }

    return "Unknown";
}

ssp_err_t SwitchRead()
{
    ssp_err_t error;
    ioport_level_t pinValue;

    error = g_ioport.p_api->pinRead (IOPORT_PORT_00_PIN_06, &pinValue);
    if (error != SSP_SUCCESS)
    {
        switchS1State = -1;
        return error;
    }
    switchS1State = pinValue == IOPORT_LEVEL_LOW ? 1 : 0;

    error = g_ioport.p_api->pinRead (IOPORT_PORT_00_PIN_10, &pinValue);
    if (error != SSP_SUCCESS)
    {
        switchS2State = -1;
        return error;
    }
    switchS2State = pinValue == IOPORT_LEVEL_LOW ? 1 : 0;

    error = g_ioport.p_api->pinRead (IOPORT_PORT_00_PIN_11, &pinValue);
    if (error != SSP_SUCCESS)
    {
        switchS3State = -1;
        return error;
    }
    switchS3State = pinValue == IOPORT_LEVEL_LOW ? 1 : 0;

    return SSP_SUCCESS;

}

