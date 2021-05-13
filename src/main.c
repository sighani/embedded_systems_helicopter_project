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

// Maps 2^12 - 1 values to a 3V range. Then calculates bit range for 0.8V
#define HELIRANGE ((4095 * 8) / 30)
#define SYSDISPLAYDIV 150

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
    initMainRotor();
    initTailRotor();
    initClock();
    initADC();
    initYaw();
    initCircBuf(&g_inBuffer, BUF_SIZE);
    initDisplay();
    g_yaw = 0;

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
            g_heliAltPercentage = 0;
            heliAltMin = meanVal;
            heliAltMax = meanVal - HELIRANGE;
        }

        // Calculate alititude as percentage with cut offs
        g_heliAltPercentage = ((heliAltMin - meanVal) * 100) / HELIRANGE;
        if (g_heliAltPercentage < 0)
        {
            g_heliAltPercentage = 0;
        }
        else if (g_heliAltPercentage > 100)
        {
            g_heliAltPercentage = 100;
        }

        if (messcount >= 12)
        {
            messcount = 0;
            updateFlightData(g_heliAltPercentage, g_yaw, g_tail_duty, g_main_duty);
        }
        messcount++;

        // Button Logic
        if ((checkButton(UP) == PUSHED))
        {
            //TODO: Order helicopter up
        }
        if ((checkButton(DOWN) == PUSHED))
        {
            //TODO: Order helicopter down
        }

        if ((checkButton(LEFT) == PUSHED))
        {
            //TODO: Order helicopter 15 CCW
        }
        if ((checkButton(RIGHT) == PUSHED))
        {
            //TODO: Order helicopter 15 CW
        }

        // Re-zero altimeter
        // if ((checkButton (DOWN) == PUSHED)) {
        //     resetAltimeter(meanVal);
        // }

        updateButtons();
        SysCtlDelay(SysCtlClockGet() / SYSDISPLAYDIV); // Update display at ~ 4 Hz
    }
}

// // Utility Functions
// void resetAltimeter(uint32_t meanVal) {
//     g_heliAltPercentage = 0;
//     heliAltMin = meanVal;
// }
