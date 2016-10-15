/* HAL-only entry function */
#include "hal_data.h"

#include <stdbool.h>

/* Include Project Files */
#include "Uart.h"
#include "Menu.h"
#include "Led.h"
#include "Switch.h"
#include "Adc.h"
#include "Dac.h"

void hal_entry(void)
{
    ssp_err_t error;

    // Initialize Peripherals

    error = UartInitialize ();
    if (error != SSP_SUCCESS)
    {
        while (true)
            ;
    }
    error = UartOpen ();
    if (error != SSP_SUCCESS)
    {
        while (true)
            ;
    }

    error = TimerInitialize();
    if (error != SSP_SUCCESS)
    {
        printf("Error Initializing Timer.");
        while (true)
            ;
    }

    error = LedInitialize();
    if (error != SSP_SUCCESS)
    {
        printf("Error Initializing LED.");
        while (true)
            ;
    }

    error = SwitchInitialize();
    if (error != SSP_SUCCESS)
    {
        printf("Error Initializing Switches.");
        while (true)
            ;
    }
    error = AdcInitialize();
    if (error != SSP_SUCCESS)
    {
        printf("Error Initializing ADC.");
        while (true)
            ;
    }

    error = DacInitialize();
    if (error != SSP_SUCCESS)
    {
        printf("Error Initializing DAC.");
        while (true)
            ;
    }
    while (true)
    {
        MenuPrint ();
    }
}
