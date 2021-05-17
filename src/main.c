/*
 * Milestone1.c
 *
 *  Created on: 19/03/2021
 *      Author: Nathaniel Stainton, Saman Ighani, Jack Walsh
 */

#include <stdint.h>
#include <stdbool.h>
#include "labcode/circBufT.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "labcode/buttons4.h"
#include "../OrbitOLED/OrbitOLEDInterface.h"


#include "setup.h"
#include "altitudeADC.h"
#include "display.h"
#include "yaw.h"
#include "rotors.h"
#include "controller.h"
#include "uartUSB.h"

// Maps 2^12 - 1 values to a 3V range. Then calculates bit range for 0.8V
#define HELIRANGE ((4095 * 8) / 30)
#define SYSDISPLAYDIV 150
#define ALT_STEP 10
#define MAX_ALT 0
#define MIN_ALT 100


uint16_t heliAltMax;
uint16_t heliAltMin;

//Function Declarations
void resetAltimeter(uint32_t meanVal);

int main(void)
{
    //  Variable Definitions
    uint16_t i;
    int32_t sum;
    int32_t meanVal;
    uint8_t initFlag = 1;
    uint8_t messcount = 0;

    //  Calls for Initialisation
    initButtons();
    initClock();
    initMainRotor();
    initTailRotor();
    initADC();
    initYaw();
    initCircBuf(&g_inBuffer, BUF_SIZE);
    initDisplay();


    g_yaw_current = 0;
    g_intcounter = 0.00;
    g_main_duty = 0;
    g_tail_duty = 0;

    enableRotors();

    //  Enable ISPs
    IntMasterEnable();

    while (1)
    {

        // Taken from ADCDemo.c Lab3
        // Background task: calculate the (approximate) mean of the values in the
        // circular buffer and display it, together with the sample number.
        sum = 0;
        for (i = 0; i < BUF_SIZE; i++)
        {
            sum = sum + readCircBuf(&g_inBuffer);
        }
        meanVal = (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;

        // Run on first start up
        if (initFlag && meanVal > 0)
        {
            initFlag = 0;
            g_alt_current = 0;
            heliAltMin = meanVal;
            heliAltMax = meanVal - HELIRANGE;
        }

        // Calculate alititude as percentage with cut offs
        g_alt_current = ((heliAltMin - meanVal) * 100) / HELIRANGE;
        if (g_alt_current < 0)
        {
            g_alt_current = 0;
        }
        else if (g_alt_current > 100)
        {
            g_alt_current = 100;
        }

        if (messcount >= 12)
        {
            messcount = 0;
            updateFlightData(g_alt_current, g_yaw_current, g_tail_duty, g_main_duty);
        }
        messcount++;

        // Send UART Data at 4Hz. UARTFlag is set every 25 SysTick interrupts (250ms).
        if (g_uartFlag) {
            g_uartFlag = 0;
//            UARTSendHeli(g_alt_current);
        }

        //TODO: Add logic for moving altitude up and down, need to use controller.c function and setpwm through pwm.c, these are then called in button up and down, have #defines to set altitude steps
        // Button Logic
        if ((checkButton(UP) == PUSHED))
        {
            if (g_alt_ref <= 90) {
                g_alt_ref += ALT_STEP;
            } else {
                g_alt_ref = 100;
            }
        }
        if ((checkButton(DOWN) == PUSHED))
        {
            if (g_alt_ref >= 10) {
                g_alt_ref -= ALT_STEP;
            } else {
                g_alt_ref = 0;
            }
        }

        if ((checkButton(LEFT) == PUSHED))
        {
            //TODO: Order helicopter 15 CCW
        }
        if ((checkButton(RIGHT) == PUSHED))
        {
            //TODO: Order helicopter 15 CW
        }

        updateButtons();
        SysCtlDelay(SysCtlClockGet() / SYSDISPLAYDIV); // Update display at ~ 4 Hz
    }
}
