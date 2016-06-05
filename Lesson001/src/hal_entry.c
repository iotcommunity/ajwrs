/* HAL-only entry function */
#include "hal_data.h"
void hal_entry(void)
{
    bool isOn = true;
    while(1)
    {
        g_ioport.p_api->pinWrite(IOPORT_PORT_08_PIN_07, isOn);
        isOn = !isOn;
        for (int i = 0; i < 1E6; i++)
        {

        }
    }
}
