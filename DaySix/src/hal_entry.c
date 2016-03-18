/* HAL-only entry function */
#include "hal_data.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_LENGTH   255
// Create global variable to hold transmission station
volatile bool transmitComplete;
volatile bool receiveComplete;
volatile int bufferIndex;
uint8_t receiveData[BUFFER_LENGTH];

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

/*
// Redefine gets to handle scanf input
char* gets (char *str)
{
    receiveComplete = false;
    char lastChar = 0;

    while(lastChar != '\n')
    {
        g_uart.p_api->read(g_uart.p_ctrl, (uint8_t const *)&receiveData[bufferIndex], 1);

        while(!receiveComplete){}
        lastChar = receiveData[bufferIndex];
        bufferIndex++;
    }

    return str;
}
*/

// Callback function to set Transmit Done flag when transmission is complete
void user_uart_callback(uart_callback_args_t *p_args)
{
    switch (p_args->event)
    {
        case UART_EVENT_TX_COMPLETE:
            // Set Transmit Complete Flag
            transmitComplete = true;
            break;

        case UART_EVENT_RX_COMPLETE:
            // Set Receive Complete Flag
            receiveComplete = true;
            break;
        default:
            break;
    }
}

void hal_entry(void)
{
    // Initialize UART Variables
    transmitComplete = false;
    receiveComplete = false;
    bufferIndex = 0;

    //memset(receiveData, 0, BUFFER_LENGTH);

    // Open the UART driver
    g_uart.p_api->open(g_uart.p_ctrl, g_uart.p_cfg);

//    while(true)
//    {
        transmitComplete = false;

        printf("Command: ");

//        bufferIndex = 0;
//        receiveComplete = false;
//        //scanf("%s", receiveData);
//
//        transmitComplete = false;
//        printf("%s", receiveData);
//    }
}

