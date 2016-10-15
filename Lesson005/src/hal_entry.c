/* HAL-only entry function */
#include "hal_data.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// Buffer Sizes
#define UART_BUFFER_SIZE 1024

// Output Buffer
char outputBuffer[UART_BUFFER_SIZE];

volatile bool transmitComplete;

// Custom printf prototype
int _printf (const char *format, ...);

// Custom printf Implementation
int _printf (const char *format, ...)
{
    // Temporary Buffer
   char buffer[UART_BUFFER_SIZE];

   // Variable Argument List
   va_list arg;

   int done;

   // Get Variable Arguments
   va_start (arg, format);

   // Pass format string and arguments to string formatter
   done = vsnprintf(buffer, UART_BUFFER_SIZE, format, arg);

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

        defaut: break;
    }
}

void hal_entry(void)
{
    // Open UART
    g_uart.p_api->open (g_uart.p_ctrl, g_uart.p_cfg);

    // Use TTY100 commands to clear screen and reset screen pointer
    _printf("\033[2J"); // Clear Screen
    _printf("\033[H"); // Return Home
    _printf("\033[3J"); // Clear Back Buffer

    // Print Header
    _printf("Lesson 005 - printf Redirection, Part 2\r\n");

    // Verify printf functions with format specifiers
    _printf("          String: %s\r\n", "This is a string");
    _printf("     Integer (5): %d\r\n", 5);
    _printf("Float (3.141592): %f\r\n", 3.141592);

    while (true)
    {
    }
}
