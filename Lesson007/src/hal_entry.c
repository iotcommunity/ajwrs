/* HAL-only entry function */
#include "hal_data.h"

#include <stdio.h>
#include <string.h>

// Buffer Sizes
#define UART_BUFFER_SIZE 1024
#define INPUT_BUFFER_SIZE 1024

// Buffers
char outputBuffer[UART_BUFFER_SIZE];
char inputBuffer[INPUT_BUFFER_SIZE];

volatile int inputBufferIndex;

volatile bool transmitComplete;
volatile bool receiveComplete;

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
        ssp_err_t err = g_uart.p_api->write (g_uart.p_ctrl, (uint8_t const *)(buffer + i), 1);
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

            // Received Character
        case UART_EVENT_RX_CHAR:
            // Add data to input Buffer
            inputBuffer[inputBufferIndex] = (char) p_args->data;

            // Check for end of data (new line or carriage return)
            if ((inputBuffer[inputBufferIndex] == '\n') || (inputBuffer[inputBufferIndex] == '\r'))
            {
                // Replace new line/carriage return with null character
                inputBuffer[inputBufferIndex] = 0;

                // Set Flag
                receiveComplete = true;
            }

            // Increment Buffer Index
            inputBufferIndex++;

            // Check for overflow
            if (inputBufferIndex >= INPUT_BUFFER_SIZE)
            {
                // Overflow occurred, set flag and reset buffer index.
                receiveComplete = true;
                inputBufferIndex = 0;
            }

        break;
            defaut: break;
    }
}

void hal_entry(void)
{
    inputBufferIndex = 0;

    // Open UART
    g_uart.p_api->open (g_uart.p_ctrl, g_uart.p_cfg);

    // Disable Output Buffering
    setvbuf ( stdout, NULL, _IONBF, UART_BUFFER_SIZE);

    // Use TTY100 commands to clear screen and reset screen pointer
    printf ("\033[2J"); // Clear Screen
    printf ("\033[H"); // Return Home
    printf ("\033[3J"); // Clear Back Buffer

    // Print Header
    printf ("Lesson 007: Redirecting printf to UART Part 3\r\n");

    while (true)
    {
        printf ("\r\nInput: ");

        // Receive all characters up to new line/carriage return.
        receiveComplete = false;
        g_uart.p_api->read (g_uart.p_ctrl, NULL, (uint32_t)NULL);
        while (!receiveComplete)
        {
        }

        // Reset Input Buffer => This does not detect or handle overflow events.
        // If there is an overflow, only the first INPUT_BUFFER_SIZE characters are printed, the rest are lost
        inputBufferIndex = 0;

        // Print Results
        printf ("%s", inputBuffer);
    }
}
