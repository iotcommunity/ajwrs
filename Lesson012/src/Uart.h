#ifndef UART_H_
#define UART_H_

#include <stdio.h>
#include <string.h>

// Buffer Size
#define UART_BUFFER_SIZE 1024

ssp_err_t UartInitialize();
ssp_err_t UartOpen();
ssp_err_t UartClose();

#endif /* UART_H_ */
