/* HAL-only entry function */
#include "hal_data.h"
#include <stdio.h>
#include <string.h>

// Create global variable to hold transmission station
volatile bool transmitComplete;

// Redefine puts to handle printf output
int puts(const char *str)
{
    // Set Transmit Flag to false
    transmitComplete = false;

    // Write String to UART
    g_uart.p_api->write(g_uart.p_ctrl, (const uint8_t *)str, strlen(str));

    // Wait until flag is set
    while (!transmitComplete) {}

    // Reset Flag
    transmitComplete = false;

    return 0;
}

// Callback function to set Transmit Done flag when transmission is complete
void user_uart_callback(uart_callback_args_t *p_args)
{
    // Return if event type is not for completed transmission
    if (p_args->event != UART_EVENT_TX_COMPLETE)
    {
        return;
    }

    // Set Transmit Complete Flag
    transmitComplete = true;
}

void hal_entry(void)
{
    // Initialize Transmission Complete Flag
    transmitComplete = false;

    // Open the UART driver
    g_uart.p_api->open(g_uart.p_ctrl, g_uart.p_cfg);

    // Clear Transmission Complete Flag
    transmitComplete = false;

    // Send the message data
    g_uart.p_api->write(g_uart.p_ctrl, (const uint8_t *)"Test\r\n", 6);

    // Wait for Transmission to complete
    while (!transmitComplete) {}

    // Clear Transmission Complete Flag
    transmitComplete = false;

    // Try a printf
    printf("\r\nThis is a test!\r\n");

    // Try another printf to make sure we are waiting for the previous one to complete
    printf("\r\nThis is another test!\r\n");
}

