/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "r_sci_uart.h"
#include "r_uart_api.h"
#include "r_ioport.h"
#include "r_ioport_api.h"
#include "r_elc.h"
#include "r_elc_api.h"
#include "r_cgc.h"
#include "r_cgc_api.h"
/** UART on SCI Instance. */
extern const uart_instance_t g_uart;
#ifdef NULL
#else
extern void NULL(uint32_t channel, uint32_t level);
#endif
#ifdef user_uart_callback
#define UART_ON_SCI_UART_CALLBACK_USED_g_uart (0)
#else
#define UART_ON_SCI_UART_CALLBACK_USED_g_uart (1)
#endif
#if UART_ON_SCI_UART_CALLBACK_USED_g_uart
void user_uart_callback(uart_callback_args_t * p_args);
#endif
/** IOPORT Instance */
extern const ioport_instance_t g_ioport;
/** ELC Instance */
extern const elc_instance_t g_elc;
/** CGC Instance */
extern const cgc_instance_t g_cgc;
/** Prototypes for generated HAL functions. */
void hal_entry(void);
void g_hal_init(void);
#endif /* HAL_DATA_H_ */
