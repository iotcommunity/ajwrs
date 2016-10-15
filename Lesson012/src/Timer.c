#include "hal_data.h"
#include "Defines.h"
#include "Timer.h"

ssp_err_t TimerInitialize()
{
    // Open the timer using the configured options from the configurator
    return ledTimer.p_api->open (ledTimer.p_ctrl, ledTimer.p_cfg);
}
