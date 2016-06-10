/* HAL-only entry function */
#include "hal_data.h"

#include <stdio.h>
#include <string.h>

// Buffer Size
#define OUTPUT_BUFFER_SIZE 1024

// Tick Rate
#define COUNTS_PER_MILLISECOND  (120E6 / 1000)

#define POT_CHANNEL 0

// Buffers
char outputBuffer[OUTPUT_BUFFER_SIZE];

// Flags
volatile bool transmitComplete;

int _write(int file, char *buffer, int count);
int _write(int file, char *buffer, int count)
{
    // As far as I know, there isn't a way to retrieve how many
    // bytes were send on using the uart->write function if it does not return
    // SSP_SUCCESS (unless we want to use the tx interrupt function and a global counter
    // so, we will send each character one by one instead.
    int bytesTransmitted = 0;

    for (int i = 0; i < count; i++)
    {
        // Start Transmission
        transmitComplete = false;
        ssp_err_t err = g_uart.p_api->write (g_uart.p_ctrl, (uint8_t const *) (buffer + i), 1);
        if (err != SSP_SUCCESS)
        {
            break;
        }
        while (!transmitComplete)
        {
        }

        bytesTransmitted++;
    }

    return bytesTransmitted;
}

// Callback Function for UART interrupts
void user_uart_callback(uart_callback_args_t * p_args)
{
    // Get Event Type
    switch (p_args->event)
    {
        // Transmission Complete
        case UART_EVENT_TX_COMPLETE:
            transmitComplete = true;
        break;
        default:
        break;
    }
}

void hal_entry(void)
{
    // Variable to hold ADC Data
    uint16_t adcData;
    float adcVoltage;

    // Error Holder
    ssp_err_t err;

    // Variable to hold counts
    timer_size_t counts = 0;

    // Open UART
    g_uart.p_api->open (g_uart.p_ctrl, g_uart.p_cfg);

    // Disable Output Buffering
    setvbuf ( stdout, NULL, _IONBF, OUTPUT_BUFFER_SIZE);

    // Open the timer using the configured options from the configurator
    err = g_timer.p_api->open (g_timer.p_ctrl, g_timer.p_cfg);
    if (err != SSP_SUCCESS)
    {
        printf ("Error opening timer. Value: %d\r\n", (int) err);
        while (1)
        {
            ;
        }
    }

    // Open the ADC
    err = g_adc.p_api->open (g_adc.p_ctrl, g_adc.p_cfg);
    if (err != SSP_SUCCESS)
    {
        printf ("Error opening analog to digital converter 0. Value: %d\r\n", (int) err);
        while (1)
        {
            ;
        }
    }

    // Configure Scan
    err = g_adc.p_api->scanCfg (g_adc.p_ctrl, g_adc.p_channel_cfg);
    if (err != SSP_SUCCESS)
    {
        printf ("Error configuring ADC Scan 0. Value: %d\r\n", (int) err);
        while (1)
        {
            ;
        }
    }

    // Use TTY100 commands to clear screen and reset screen pointer
    printf ("\033[2J"); // Clear Screen
    printf ("\033[H"); // Return Home
    printf ("\033[3J"); // Clear Back Buffer

    // Print Header
    printf ("Lesson 008: ADC\r\n");
    printf ("Counts: 0\r\n");
    printf (" Value: 0\r\n");

    while (true)
    {
        // Start ADC Scan
        err = g_adc.p_api->scanStart (g_adc.p_ctrl);
        if (err != SSP_SUCCESS)
        {
            printf ("Error starting ADC Scan 0. Value: %d\r\n", (int) err);
            while (1)
            {
                ;
            }
        }
        // Wait for scan to complete
        while (true)
        {
            err = g_adc.p_api->scanStatusGet (g_adc.p_ctrl);
            if (err != SSP_SUCCESS)
            {
                continue;
            }
            break;
        }

        // Read ADC
        err = g_adc.p_api->read (g_adc.p_ctrl, 0, &adcData);

        if (err != SSP_SUCCESS)
        {
            printf ("Error starting ADC Scan. Value: %d\r\n", (int) err);
            while (1)
            {
                ;
            }
        }

        // Display Values
        adcVoltage = ((adcData * 3.3f)/4095.0f);
        printf("\033[2A"); // Move up two lines
        printf("\033[8C"); // Move right 8 characters
        printf("\033[K"); // Clear to end of line
        printf ("%d\r\n", adcData);
        printf("\033[8C"); // Move right 8 characters
        printf("\033[K"); // Clear to end of line
        printf ("%f\r\n", adcVoltage);

        // Wait 100ms before we do another read
        // Reset the timer to 0
        g_timer.p_api->reset (g_timer.p_ctrl);
        while (1)
        {
            // Get current counts
            g_timer.p_api->counterGet (g_timer.p_ctrl, &counts);

            // Check if 1000ms has elapsed => This should be a helper function at some point
            // Need to look if the PBCLK settings are stored in a define somewhere...
            if (counts > (100 * COUNTS_PER_MILLISECOND))
            {
                // Reset the timer to 0
                g_timer.p_api->reset (g_timer.p_ctrl);
                break;
            }
        }
    }
}
