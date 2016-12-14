/* HAL-only entry function */
#include "hal_data.h"
#include <stdio.h>

#include "Uart.h"
#include "I2C.h"
#include "Timer.h"
#include "AMS_iAQ_Core_C.h"
#include "ENS210.h"
#include "AS3935.h"
#include "TMD3782.h"

#define USE_VT100
void hal_entry(void)
{
    ssp_err_t error;
    uint8_t const iAQCoreCAddress = 0x5A;
    uint8_t const tmd3782Address = 0x39;
    uint8_t const ens210Address = 0x43;
    uint8_t const as3935Address = 0x02;

    AMSiAQCoreData aMSiAQCoreData;
    ENS210Data ens210Data;
    AS3935Data as3935Data;
    TMD3782Data tmd3782Data;
    uint16_t ens210ChipId;
    uint8_t tmd3782ChipId;
    uint8_t tmd3782Status;

    /************************************
     * Init UART
     ************************************/
    error = UartInitialize ();
    if (error != SSP_SUCCESS)
    {
        while (true)
        {
        }
    }

    error = UartOpen ();
    if (error != SSP_SUCCESS)
    {
        while (true)
        {
        }
    }

    /************************************
     * Clear Console
     ************************************/
#ifdef USE_VT100
    {
        // Use TTY100 commands to clear screen and reset screen pointer
        printf ("\033[2J"); // Clear Screen
        printf ("\033[H"); // Return Home
        printf ("\033[3J"); // Clear Back Buffer
    }
#endif
    /************************************
     * Init Timer
     ************************************/
    printf ("Initializing Timer: ");
    error = TimerInitialize ();
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("OK.\r\n");

    /************************************
     * Init I2C
     ************************************/
    printf ("Initializing I2C: ");
    error = I2CInitialize ();
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("OK.\r\n");

    printf ("Opening I2C: ");
    error = I2COpen ();
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("OK.\r\n");

    printf ("Initializing AMS iQA Core C: ");
    error = AMSiAQCoreInitialize();
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("OK.\r\n");

    printf ("Initializing ENS210: ");
    error = ENS210Initialize();
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("OK.\r\n");

    printf ("Initializing AS3935: ");
    error = AS3935Initialize();
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("OK.\r\n");

    printf ("Initializing TMD3782: ");
    error = TMD3782Initialize();
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("OK.\r\n");

    printf ("Opening ENS210: ");
    error = ENS210Open(ens210Address);
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("OK.\r\n");

    printf ("ENS210 Chip ID: ");
    error = ENS210ChipId(ens210Address, &ens210ChipId);
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("0x%02x\r\n", ens210ChipId);

    printf ("Opening TMD3782: ");
    error = TMD3782Open(tmd3782Address);
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("OK.\r\n");

    printf ("TMD3782 Chip ID: ");
    error = TMD3782ChipId(tmd3782Address, &tmd3782ChipId);
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("0x%02x\r\n", tmd3782ChipId);

    printf ("TMD3782 Status: ");
    error = TMD3782Status(tmd3782Address, &tmd3782Status);
    if (error != SSP_SUCCESS)
    {
        printf ("Failed.\r\n");
        while (true)
        {
        }
    }
    printf ("0x%02x\r\n", tmd3782Status);

    printf("\r\nSensor Values:\r\n");

#ifdef USE_VT100
    {
        // Save Cursor Position
        printf ("\0337");
    }
#endif

    while (true)
    {
        AMSiAQCoreUpdateSensors(iAQCoreCAddress, &aMSiAQCoreData);
        ENS210UpdateSensors(ens210Address, &ens210Data);
        AS3935UpdateSensors(as3935Address, &as3935Data);
        TMD3782UpdateSensors(tmd3782Address, &tmd3782Data);

        printf("AMS iAQ Core C Data:\r\n");
        printf("Prediction: %dppm\033[K\r\n", aMSiAQCoreData.Prediction);
        printf("    Status: ");
        if (aMSiAQCoreData.Status == 0x00)      printf("OK\033[K");
        else if (aMSiAQCoreData.Status == 0x10) printf("WARMING UP\033[K");
        else if (aMSiAQCoreData.Status == 0x01) printf("BUSY\033[K");
        else if (aMSiAQCoreData.Status == 0x80) printf("ERROR\033[K");
        else                                    printf("UNKNOWN\033[K");
        printf("\r\n");

        printf("Resistance: %luOhms\033[K\r\n", aMSiAQCoreData.Resistance);
        printf("      TVOC: %dppb\033[K\r\n", aMSiAQCoreData.TVOC);

        printf("\r\n");
        printf("ENS210 Data: \r\n");
        printf(" T: %fC\033[K\r\n", ens210Data.Temperature);
        printf("RH: %f%%\033[K\r\n", ens210Data.Humidity);

        printf("\r\n");
        printf("AS3935 Data: \r\n");
        printf("                             AFE Gain Boost: %d\033[K\r\n", as3935Data.AFE_GB);
        printf("                               Power Status: %s\033[K\r\n", as3935Data.PWD == 1 ? "Power Down" : "Active");
        printf("                          Noise Floor Level: %d\033[K\r\n", as3935Data.NF_LEV);
        printf("                         Watchdog Threshold: %d\033[K\r\n", as3935Data.WDTH);
        printf("                Minimum Number of Lightning: %d\033[K\r\n", as3935Data.MIN_NUM_LIGH);
        printf("                            Spike Rejection: %d\033[K\r\n", as3935Data.SREJ);
        printf("Frequency Division Ratio for Antenna Tuning: %d\033[K\r\n", as3935Data.LCO_FDIV);
        printf("                             Mask Disturber: %d\033[K\r\n", as3935Data.MASK_DIST);
        printf("                                  Interrupt: 0x%02X\033[K\r\n", as3935Data.INT);
        printf("                                       Noise Level Too High: %s\033[K\r\n", (as3935Data.INT & 0b0001) > 0 ? "Yes" : "No");
        printf("                                         Disturber Detected: %s\033[K\r\n", (as3935Data.INT & 0b0100) > 0 ? "Yes" : "No");
        printf("                                        Lightning Interrupt: %s\033[K\r\n", (as3935Data.INT & 0b1000) > 0 ? "Yes" : "No");
        printf("             Energy of the Single Lightning: %d\033[K\r\n", (as3935Data.S_LIG_M << 20) | (as3935Data.S_LIG_M << 8) | as3935Data.S_LIG_L);
        printf("                        Distance Estimation: %dkm\033[K\r\n", as3935Data.DISTANCE);
        printf("                                        LCO: %s\033[K\r\n", (as3935Data.DISP_LCO) > 0 ? "Yes" : "No");
        printf("                                       SRCO: %s\033[K\r\n", (as3935Data.DISP_SRCO) > 0 ? "Yes" : "No");
        printf("                                       TRCO: %s\033[K\r\n", (as3935Data.DISP_TRCO) > 0 ? "Yes" : "No");
        printf("                 Internal Tuning Capacitors: %dpf\033[K\r\n", as3935Data.TUN_CAP * 8);
        printf("                        Calibration of TRCO: %s\033[K\r\n", as3935Data.TRCO_CALIB_DONE == 1 ? "Calibrated" : "Not Calibrated");
        printf("                        Calibration of TRCO: %s\033[K\r\n", as3935Data.TRCO_CALIB_NOK == 1 ? "Error" : "Successful");
        printf("                        Calibration of SRCO: %s\033[K\r\n", as3935Data.SRCO_CALIB_DONE == 1 ? "Calibrated" : "Not Calibrated");
        printf("                        Calibration of SRCO: %s\033[K\r\n", as3935Data.SRCO_CALIB_NOK == 1 ? "Error" : "Successful");

        printf("\r\n");
        printf("TMD3782 Data: \r\n");
        printf("    Clear: %d\033[K\r\n", tmd3782Data.Clear);
        printf("      Red: %d\033[K\r\n", tmd3782Data.Red);
        printf("    Green: %d\033[K\r\n", tmd3782Data.Green);
        printf("     Blue: %d\033[K\r\n", tmd3782Data.Blue);
        printf("Proximity: %d\033[K\r\n", tmd3782Data.Proximity);


#ifdef USE_VT100
        {
            // Restore Cursor Position
            printf ("\0338");
        }
#endif

        TimerSleepMs (500);
    }
}
