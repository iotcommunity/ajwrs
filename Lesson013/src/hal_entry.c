/* HAL-only entry function */
#include "hal_data.h"
#include <stdbool.h>

#define COUNTS_PER_MILLISECOND  (120E6 / 1000)

void hal_entry(void)
{
    timer_size_t timerValue;

    // Check if reset was caused by the watchdog timer.
    // If it was, turn on an LED
    if (R_SYSTEM->RSTSR1_b.WDTRF != 0)
    {
        g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_10, IOPORT_LEVEL_HIGH);
    }


    // Initialize Timer
    g_timer.p_api->open (g_timer.p_ctrl, g_timer.p_cfg);

    // Open Watchdog Timer
    g_wdt.p_api->open (g_wdt.p_ctrl, g_wdt.p_cfg);

    for (int i = 0; i < 5; i++)
    {
        // Turn Red LED on
        g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_08, IOPORT_LEVEL_HIGH);

        // Reset Timer
        g_timer.p_api->reset(g_timer.p_ctrl);
        while(true)
        {
            // Sleep for 500ms
            g_timer.p_api->counterGet(g_timer.p_ctrl, &timerValue);
            if (timerValue > (500*COUNTS_PER_MILLISECOND))
            {
                break;
            }
        }

        // Turn Red LED off
        g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_08, IOPORT_LEVEL_LOW);

        // Reset Timer
        g_timer.p_api->reset(g_timer.p_ctrl);
        while(true)
        {
            g_timer.p_api->counterGet(g_timer.p_ctrl, &timerValue);
            if (timerValue > (500*COUNTS_PER_MILLISECOND))
            {
                break;
            }
        }
    }

    // Turn Red LED off
    g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_08, IOPORT_LEVEL_LOW);

    // Turn Green LED on
    g_ioport.p_api->pinWrite (IOPORT_PORT_08_PIN_07, IOPORT_LEVEL_HIGH);

    // Start Watchdog
    g_wdt.p_api->refresh(g_wdt.p_ctrl);

    // Sit in a loop and wait for watchdog timer to fire.
    // This will not happen while in the debugger since the watchdog timer doesn't
    // run with the debugger attached.
    // Debug the code (so the device programs), disconnect the USB to the debugger, and
    // reset the development board. The program will run and 2.23 seconds after the green
    // led turns on, the program will start over.
    while(true)
    {
        ;
    }

}
