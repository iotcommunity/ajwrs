/* HAL-only entry function */
#include "hal_data.h"
void hal_entry(void)
{
    uint8_t data[] = "This is a test";
    ssp_err_t openResult;
    ssp_err_t writeResult;

    openResult = g_uart.p_api->open(g_uart.p_ctrl, g_uart.p_cfg);
    writeResult = g_uart.p_api->write(g_uart.p_ctrl, data, 14);
    ;
}
