/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_ioport.h"
#include "r_ioport_api.h"
#include "r_elc.h"
#include "r_elc_api.h"
#include "r_cgc.h"
#include "r_cgc_api.h"
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer;
#ifdef NULL
#define TIMER_ON_GPT_CALLBACK_USED_g_timer (0)
#else
#define TIMER_ON_GPT_CALLBACK_USED_g_timer (1)
#endif
#if TIMER_ON_GPT_CALLBACK_USED_g_timer
void NULL(timer_callback_args_t * p_args);
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
