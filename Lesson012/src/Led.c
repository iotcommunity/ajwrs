#include "hal_data.h"
#include <stdbool.h>
#include "Defines.h"
#include "Led.h"

int LedSpeed;
int LedDirection;
int LedState;

ssp_err_t LedInitialize()
{
    LedTurnOff ();
    LedDirection = 0;
    LedState = 0;
    return SSP_SUCCESS;

}
void LedTurnOn()
{
    LedSpeed = 0;
    g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_07, true);
}

void LedTurnOff()
{
    LedWalk(0);
}

int LedGetSpeed()
{
    return LedSpeed;
}

void LedWalk(int direction)
{
    if (direction > 0)
    {
        LedSpeed = LedSpeed < 9 ? LedSpeed + 1 : 9;
    }
    else if (direction == 0)
    {
        LedSpeed = 0;
    }
    else
    {
        LedSpeed = LedSpeed > -9 ? LedSpeed - 1 : -9;
    }

    // Set Timer Interrupt Value

    // Positive
    if (LedSpeed > 0)
    {
        if (LedState == 0)
        {
            LedState = 0b00000010;
        }
        ledTimer.p_api->periodSet (ledTimer.p_ctrl, (timer_size_t) (1112.5 - LedSpeed * 112.5), TIMER_UNIT_PERIOD_MSEC);
        ledTimer.p_api->start(ledTimer.p_ctrl);
        LedDirection = 1;
    }
    // Negative
    else if (LedSpeed < 0)
    {
        if (LedState == 0)
        {
            LedState = 0b00010000;
        }
        ledTimer.p_api->periodSet (ledTimer.p_ctrl, (timer_size_t) (1112.5 + LedSpeed * 112.5), TIMER_UNIT_PERIOD_MSEC);
        ledTimer.p_api->start(ledTimer.p_ctrl);
        LedDirection = -1;
    }
    else
    {
        ledTimer.p_api->stop(ledTimer.p_ctrl);
        LedDirection = 0;
        LedState = 0;
        g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_07, false);
        g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_08, false);
        g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_09, false);
        g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_10, false);
    }
}

void ledTimerInterrupt(timer_callback_args_t * p_args)
{
    SSP_PARAMETER_NOT_USED(p_args);
    g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_07, (LedState & 0b00000010)>> 1);
    g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_08, (LedState & 0b00000100)>> 2);
    g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_09, (LedState & 0b00001000)>> 3);
    g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_10, (LedState & 0b00010000)>> 4);

    LedState += 2*LedDirection;

    if (LedState == 0b00000001)
    {
        LedState = LedDirection > 0 ? 0b00000010 : 0b00010000;
    }
    else if (LedState == 0b00010001)
    {
        LedState = LedDirection > 0 ? 0b00000010 : 0b00010000;
    }

}
