/* HAL-only entry function */
#include "hal_data.h"
#include <math.h>

#define PI 3.14159265358979323846

#define FREQUENCY 12000
#define SAMPLE_POINTS 32
#define DT ((1.0) / (FREQUENCY * SAMPLE_POINTS))
#define DT_NS ((DT)*(1E9))

dac_size_t dataPoints[SAMPLE_POINTS];

uint16_t dataIndex;

void g_timer_callback(timer_callback_args_t * p_args)
{
    SSP_PARAMETER_NOT_USED(p_args);

    g_dac.p_api->write (g_dac.p_ctrl, dataPoints[dataIndex]);

    // Increment or reset data index
    dataIndex = (dac_size_t)(dataIndex + 1 >= SAMPLE_POINTS ? 0 : dataIndex + 1);
}

void hal_entry(void)
{
    double t = 0;
    timer_size_t dt_ns = (timer_size_t)(DT_NS < 1 ? 1 : DT_NS);

    // Pre Generate Data Points and use as a lookup table
    for (uint16_t i = 0; i < SAMPLE_POINTS; i++)
    {
        dataPoints[i] = (dac_size_t) (((sin (2 * PI * FREQUENCY * t) + 1) / 2.0) * 4095);
        t += (dt_ns/1E9);
    }

    // Open DAC
    g_dac.p_api->open (g_dac.p_ctrl, g_dac.p_cfg);
    g_dac.p_api->start (g_dac.p_ctrl);

    // Open Timer
    g_timer.p_api->open (g_timer.p_ctrl, g_timer.p_cfg);

    // Set Period
    g_timer.p_api->periodSet(g_timer.p_ctrl, dt_ns, TIMER_UNIT_PERIOD_NSEC);

    // Start the timer
    g_timer.p_api->start(g_timer.p_ctrl);

    while (true)
    {
        // Do nothing here. Let the timer interrupts do the work.
    }
}

