/* HAL-only entry function */
#include "hal_data.h"
void hal_entry(void)
{
    // Create a simple message
    uint8_t data[] = "This is a test";

    // Catch the function call results for debugging
    ssp_err_t openResult;
    ssp_err_t writeResult;

    // Open the UART driver
    openResult = g_uart.p_api->open(g_uart.p_ctrl, g_uart.p_cfg);

    // Send the message data
    writeResult = g_uart.p_api->write(g_uart.p_ctrl, data, 14);
}
