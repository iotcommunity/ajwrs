/* HAL-only entry function */
#include "hal_data.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// Buffer Sizes
#define OUTPUT_BUFFER_SIZE 1024
#define INPUT_BUFFER_SIZE 1024

// Buffers
char outputBuffer[OUTPUT_BUFFER_SIZE];
char inputBuffer[INPUT_BUFFER_SIZE];

volatile int inputBufferIndex;

volatile bool transmitComplete;
volatile bool receiveComplete;

// Custom printf prototype
int _printf (const char *format, ...);

// Custom printf Implementation
int _printf (const char *format, ...)
{
    // Temporary Buffer
   char buffer[OUTPUT_BUFFER_SIZE];

   // Variable Argument List
   va_list arg;

   int done;

   // Get Variable Arguments
   va_start (arg, format);

   // Pass format string and arguments to string formatter
   done = vsnprintf(buffer, OUTPUT_BUFFER_SIZE, format, arg);

   // Start Transmission
   transmitComplete = false;
   g_uart.p_api->write (g_uart.p_ctrl, buffer, done);

   while (!transmitComplete)
   {
   }

   // End Variable Arguments
   va_end (arg);

   return done;
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
            inputBuffer[inputBufferIndex] = p_args->data;

            // Check for end of data (new line or carriage return)
            if ( (inputBuffer[inputBufferIndex] == '\n') || (inputBuffer[inputBufferIndex] == '\r'))
            {
                // Replace new line/carriage return with NULL
                inputBuffer[inputBufferIndex] = NULL;

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
    g_uart.p_api->open(g_uart.p_ctrl, g_uart.p_cfg);

    // Use TTY100 commands to clear screen and reset screen pointer
    _printf("\033[2J"); // Clear Screen
    _printf("\033[H"); // Return Home
    _printf("\033[3J"); // Clear Back Buffer

    // Print Header
    _printf("Day Seven - UART Input\r\n");

    while (true)
    {
        _printf("\r\nInput: ");

        // Receive all characters up to new line/carriage return.
        receiveComplete = false;
        g_uart.p_api->read(g_uart.p_ctrl, NULL, NULL);
        while (!receiveComplete){}

        // Reset Input Buffer => This does not detect or handle overflow events.
        // If there is an overflow, only the first INPUT_BUFFER_SIZE characters are printed, the rest are lost
        inputBufferIndex = 0;

        // Print Results
        _printf("%s", inputBuffer);
    }
}
