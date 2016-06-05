/***********************************************************************************************************************
 * Copyright [2015] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 * 
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * File Name    : r_input_capture_api.h
 * Description  : Input Capture API. Allows sampling of input signals for pulse width and period.
 **********************************************************************************************************************/

#ifndef DRV_INPUT_CAPTURE_API_H
#define DRV_INPUT_CAPTURE_API_H

/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup INPUT_CAPTURE_API Input Capture Interface
 *
 * @brief Interface for sampling input signals for pulse width or period.
 *
 * @section INPUT_CAPTURE_API_SUMMARY Summary
 * The input capture interface provides for sampling of input signals to determine the width of a pulse (from one edge
 * to the opposite edge) or the period of a pulse (from one edge to the next same edge). An interrupt can be triggered
 * after each measurement is captured.
 *
 * @section INPUT_CAPTURE_API_INSTANCES Known Implementations
 * @see GPT_INPUT_CAPTURE
 *
 * @section INPUT_CAPTURE_API_RELATED See also:
 * @see TIMER_API
 *
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "bsp_api.h"
#include "r_timer_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define INPUT_CAPTURE_API_VERSION_MAJOR (1)
#define INPUT_CAPTURE_API_VERSION_MINOR (0)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Input capture operational modes */
typedef enum e_input_capture_mode
{
    INPUT_CAPTURE_MODE_PULSE_WIDTH,       ///< Measure a signal pulse width.
    // TODO_INPUT_CAPTURE (MBB) Period measurements not yet implemented
    // INPUT_CAPTURE_MODE_PERIOD,       ///< Measure a signal period length.
} input_capture_mode_t;

/** Input capture signal edge trigger */
typedef enum e_input_capture_signal_edge
{
    INPUT_CAPTURE_SIGNAL_EDGE_RISING,   ///< The capture begins at the rising edge.
    INPUT_CAPTURE_SIGNAL_EDGE_FALLING,  ///< The capture begins at the falling edge.
    // TODO_INPUT_CAPTURE (MBB) Capture both edges not yet implemented
    // INPUT_CAPTURE_SIGNAL_EDGE_BOTH,     ///< The capture begins at either edge.
} input_capture_signal_edge_t;

/** Input capture signal level, primarily used for the enable signal */
typedef enum e_input_capture_signal_level
{
    INPUT_CAPTURE_SIGNAL_LEVEL_NONE,    ///< Use this if signal_level is not applicable to a particular measurement.
    INPUT_CAPTURE_SIGNAL_LEVEL_LOW,     ///< The capture is enabled at the low level.
    INPUT_CAPTURE_SIGNAL_LEVEL_HIGH,    ///< The capture is enabled at the high level.
    INPUT_CAPTURE_SIGNAL_LEVEL_MAXVAL,  ///< Last enum value + 1, used to detect illegal values.
} input_capture_signal_level_t;

/** Specifies either a one-time or continuous measurements */
typedef enum e_input_capture_repetition
{
    INPUT_CAPTURE_REPETITION_PERIODIC,  ///< Capture continuous measurements, until explicitly stopped or disabled.
    INPUT_CAPTURE_REPETITION_ONESHOT,   ///< Capture a single measurement, then interrupts are disabled.
} input_capture_repetition_t;

/** Specifies whether or not the counter is cleared to 0 after each captured measurement */
typedef enum e_input_capture_autoclear
{
    INPUT_CAPTURE_AUTOCLEAR_OFF,        ///< The counter is not cleared after a capture, allows for accumulated counts.
    INPUT_CAPTURE_AUTOCLEAR_ON,         ///< The counter is cleared after each measurement is captured.
} input_capture_autoclear_t;

/** Specifies whether or not interrupts are enabled immediately after the open */
typedef enum e_input_capture_irq_enable
{
    INPUT_CAPTURE_IRQ_DISABLE,          ///< Interrupts are disabled after open is called.
    INPUT_CAPTURE_IRQ_ENABLE,           ///< Interrupts are enabled after open is called.
} input_capture_irq_enable_t;

/** Events that can trigger a callback function */
typedef enum e_input_capture_event
{
    INPUT_CAPTURE_EVENT_MEASUREMENT,    ///< A measurement (pulse width or period) has been captured.
} input_capture_event_t;

/** Callback function parameter data */
typedef struct st_input_capture_callback_args
{
    uint8_t                channel;     ///< The channel being used.
    input_capture_event_t  event;       ///< The event that caused the interrupt and callback.
    timer_size_t           counter;     ///< The value of the timer captured at the time of interrupt.
    void const             * p_context; ///< Placeholder for user data, set in input_capture_cfg_t::p_context.
} input_capture_callback_args_t;

/** Definition of the callback function and parameters. This function is called from an interrupt environment. */
typedef void (* input_capture_callback_t)(input_capture_callback_args_t const * p_args);

/** Channel control block. DO NOT INITIALIZE.  Initialization occurs when input_capture_api_t::open is called. */
typedef struct st_input_capture_ctrl
{
    uint8_t                     channel;     ///< The channel in use.
    input_capture_mode_t        mode;        ///< The mode of measurement being performed.
    input_capture_repetition_t  repetition;  ///< One-shot or periodic measurement.
    input_capture_callback_t    p_callback;  ///< Pointer to user's callback function, or NULL if no interrupt desired.
    void const                  * p_context; ///< Pointer to user's context data, to be passed to the callback function.
} input_capture_ctrl_t;

/** User configuration structure, passed to input_capture_api_t::open function */
typedef struct st_input_capture_cfg
{
    uint8_t                      channel;     ///< The channel in use.
    input_capture_mode_t         mode;        ///< The mode of measurement to be performed.
    input_capture_signal_edge_t  edge;        ///< The triggering edge to start a measurement (rise, fall, both).
    input_capture_repetition_t   repetition;  ///< One-shot or periodic measurement.
    input_capture_autoclear_t    autoclear;   ///< Auto clear the counter or not.
    input_capture_irq_enable_t   irq_enable;  ///< Specifies whether interrupts are enabled or not after open.
    void const                   * p_extend;  /**< REQUIRED. Pointer to peripheral-specific extension parameters.
                                               *   See gpt_input_capture_extend_t or agt_input_capture_extend_t. */
    input_capture_callback_t     p_callback;  ///< Pointer to user's callback function, or NULL if no interrupt desired.
    void const                   * p_context; ///< Pointer to user's context data, to be passed to the callback
                                              // function.
} input_capture_cfg_t;

/** Input capture API structure. Functions implemented at the HAL layer will implement this API. */
typedef struct st_input_capture_api
{
    /** Initial configuration.
     * @par Implemented as
     * R_GPT_InputCaptureOpen()

     * @pre Peripheral clocks and any required input pins should be configured prior to calling this function.
     * @note To reconfigure after calling this function, call input_capture_api_t::close first.
     * @param[in]   p_ctrl   Pointer to control block: memory allocated by caller, contents filled in by open.
     * @param[in]   p_cfg    Pointer to configuration structure. All elements of this structure must be set by user.
     */
    ssp_err_t (* open)(input_capture_ctrl_t      * const p_ctrl,
                       input_capture_cfg_t const * const p_cfg);

    /** Close the input capture operation. Allows driver to be reconfigured, and may reduce power consumption.
     * @par Implemented as
     * R_GPT_InputCaptureClose()

     *
     * @param[in]  p_ctrl    Pointer to control block initialized by input_capture_api_t::open call.
     */
    ssp_err_t (* close)(input_capture_ctrl_t     * const p_ctrl);

    /** Gets the version of this API and stores it in structure pointed to by p_version.
     * @par Implemented as
     * R_GPT_InputCaptureVersionGet()

     *
     * @param[out]  p_version  Code and API version used.
     */
    ssp_err_t (* versionGet)(ssp_version_t   * const p_version);

    /** Disables input capture measurement interrupts at NVIC.
     * @par Implemented as
     * R_GPT_InputCaptureIrqDisable()

     *
     *
     * @pre Call input_capture_api_t::open to configure the timer before using this function.
     * @param[in]  p_ctrl    Pointer to control block initialized by input_capture_api_t::open call.
     */
    ssp_err_t (* irqDisable)(input_capture_ctrl_t const * const p_ctrl);

    /** Enables input capture measurement interrupts at NVIC.
     * @par Implemented as
     * R_GPT_InputCaptureIrqEnable()
 *
     *
     * @pre Call input_capture_api_t::open to configure the timer before using this function.
     * @param[in]  p_ctrl    Pointer to control block initialized by input_capture_api_t::open call.
     * @note Interrupts may already be enabled if specified by input_capture_cfg_t::irq_enable.
     */
    ssp_err_t (* irqEnable)(input_capture_ctrl_t const * const p_ctrl);

    /** Gets the status (running or not) of the measurement counter.
     * @par Implemented as
     * R_GPT_InputCaptureStatusGet()
     *
     * @pre Call input_capture_api_t::open to configure the timer before using this function.
     * @param[in]   p_ctrl    Pointer to control block initialized by input_capture_api_t::open call.
     * @param[out]  p_status  Pointer to returned status. Result will be one of timer_api_t::timer_status_t.
     */
    ssp_err_t (* statusGet)(input_capture_ctrl_t const * const p_ctrl,
                            timer_status_t             * const p_status);

    /** Gets the last captured timer/counter value
     * @par Implemented as
     * R_GPT_InputCaptureLastCaptureGet()
     *
     * @pre Call input_capture_api_t::open to configure the timer before using this function.
     * @param[in]  p_ctrl     Pointer to control block initialized by input_capture_api_t::open call.
     * @param[out] p_counter  Pointer to location to store last captured counter value.
     */
    ssp_err_t (* lastCaptureGet)(input_capture_ctrl_t const * const p_ctrl,
                                 timer_size_t               * const p_counter);
} input_capture_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_input_capture_instance
{
    input_capture_ctrl_t      * p_ctrl;    ///< Pointer to the control structure for this instance
    input_capture_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    input_capture_api_t const * p_api;     ///< Pointer to the API structure for this instance
} input_capture_instance_t;

/*******************************************************************************************************************//**
 * @} (end defgroup INPUT_CAPTURE_API)
 **********************************************************************************************************************/
#endif /* DRV_INPUT_CAPTURE_API_H */
