#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Defines.h"
#include "Menu.h"
#include "Led.h"
#include "Switch.h"
#include "Adc.h"
#include "Dac.h"

char MenuRead();

void MenuPrint()
{
    char menuChoice;
    bool validMenuChoice;
    float potentiometerVoltage;

    if (USE_VT100)
    {
        // Use TTY100 commands to clear screen and reset screen pointer
        printf ("\033[2J"); // Clear Screen
        printf ("\033[H"); // Return Home
        printf ("\033[3J"); // Clear Back Buffer
    }

    if (AdcRead() != SSP_SUCCESS)
        printf("Error reading ADC.\r\n");

    potentiometerVoltage = AdcGetPotentiometerVoltage();

    // Print Header
    printf ("\r\n");
    printf ("|=================================================|\r\n");
    printf ("|     Lesson 012: Full Peripheral Application     |\r\n");
    printf ("|=================================================|\r\n");
    printf ("| 1) Turn LED on                                  |\r\n");
    printf ("| 2) Turn LED off                                 |\r\n");
    if (LedGetSpeed () > 0)
    {
        printf ("| 3) Walk LED forwards (Speed %1d)                  |\r\n", LedGetSpeed ());
        printf ("| 4) Walk LED backwards                           |\r\n");
    }
    else if (LedGetSpeed () == 0)
    {
        printf ("| 3) Walk LED forwards                            |\r\n");
        printf ("| 4) Walk LED backwards                           |\r\n");
    }

    else
    {
        printf ("| 3) Walk LED forwards                            |\r\n");
        printf ("| 4) Walk LED backwards (Speed %1d)                |\r\n", LedGetSpeed ());
    }
    printf ("| 5) Turn LED Walk off                            |\r\n");
    printf ("| 6) Read Switches                                |\r\n");
    printf ("|        Switch 1: %-11s                    |\r\n", SwitchGetStateText(SwitchGetState(1)));
    printf ("|        Switch 2: %-11s                    |\r\n", SwitchGetStateText(SwitchGetState(2)));
    printf ("|        Switch 3: %-11s                    |\r\n", SwitchGetStateText(SwitchGetState(3)));
    printf ("| 7) Read ADC                                     |\r\n");
    printf ("|       Potentiometer Voltage: % 15.9fV   |\r\n", potentiometerVoltage);
    printf ("|    Potentiometer Resistance: % 10.4f Ohms    |\r\n", AdcGetPotentiometerResistance());
    printf ("|       Potentiometer Percent: % 8.2f%%          |\r\n", AdcGetPotentiometerPercent());
    printf ("|         Temperature Voltage: % 15.9fV   |\r\n", AdcGetTemperatureVoltage());
    printf ("|         Temperature Degrees: % 8.2fC          |\r\n", AdcGetTemperatureDegreesC());
    printf ("|         Temperature Degrees: % 8.2fF          |\r\n", AdcGetTemperatureDegreesF());
    printf ("|                Vref Voltage: % 15.9fV   |\r\n", AdcGetVrefVoltage());
    printf ("| 8) Write DAC (% 15.9fV)                 |\r\n", potentiometerVoltage);
    printf ("|=================================================|\r\n");
    printf (" Input: ");

    menuChoice = 0;
    validMenuChoice = true;
    while (true)
    {
        menuChoice = MenuRead ();

        switch (menuChoice)
        {
            case '1':
                LedTurnOn ();
            break;
            case '2':
                LedTurnOff ();
            break;
            case '3':
                LedWalk (1);
            break;
            case '4':
                LedWalk (-1);
            break;
            case '5':
                LedWalk (0);
            break;
            case '6':
                SwitchRead();
            break;
            case '7':
                AdcRead();
            break;
            case '8':
                DacWrite(potentiometerVoltage);
            break;
            default:
                validMenuChoice = false;
                printf("%c is not a valid response.\r\n Input: ", menuChoice);

            break;
        }

        if (validMenuChoice)
            break;
    }

}

char MenuRead()
{
    char input;

// Get Single Character
    scanf ("%c", &input);

// Convert to integer
    return input;
}
