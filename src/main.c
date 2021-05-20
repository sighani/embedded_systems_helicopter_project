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
#include "driverlib/uart.h"

#include "setup.h"
#include "altitudeADC.h"
#include "display.h"
#include "yaw.h"
#include "rotors.h"
#include "controller.h"
#include "uartUSB.h"
#include "takeoff.h"

// Maps 2^12 - 1 values to a 3V range. Then calculates bit range for 1V
#define HELI_RANGE ((4095 * 10) / 30)
#define ALT_STEP 10
#define MAX_ALT 100
#define MIN_ALT 0

#define YAW_STEP 15
#define MAX_YAW 360

int main(void)
{
    //  Variable Definitions
    uint16_t i;
    int32_t sum;
    int32_t meanVal;
    uint16_t heliAltMin;
    uint8_t initFlag = 1;
    uint8_t displayCounter = 0;
    uint8_t circBufferCounter = 0;
    uint8_t altInitCounter = 0;
    uint8_t currentAltitudeCounter = 0;
    uint8_t buttonCounter = 0;

    //  Calls for Initialisation
    initMainRotor();
    initTailRotor();
    initButtons();
    initClock();
    initADC();
    initYaw();
    initCircBuf(&g_inBuffer, BUF_SIZE);
    initDisplay();
    initialiseUSB_UART();
    initSwitchInt();

    // Enable Rotors, set to off.
    enableRotors();

    //  Enable ISPs
    IntMasterEnable();

    // Paced Loop
    while (1)
    {

        // CALCULATE ADC
        if (circBufferCounter >= 15)
        {
            circBufferCounter = 0;
            // Taken from ADCDemo.c Lab3
            // Background task: calculate the (approximate) mean of the values in the
            // circular buffer and display it, together with the sample number.
            sum = 0;
            for (i = 0; i < BUF_SIZE; i++)
            {
                sum = sum + readCircBuf(&g_inBuffer);
            }
            meanVal = (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;
        }
        circBufferCounter++;

        // START UP ADC
        if (altInitCounter >= 15 && initFlag)
        {
            altInitCounter = 0;
            // Run on first start up
            if (initFlag && meanVal > 500)
            {
                initFlag = 0;
                g_altCurrent = 0;
                heliAltMin = meanVal;
            }
        }
        altInitCounter++;

        // COVERT ADC TO PERCENTAGE
        if (currentAltitudeCounter >= 15)
        {
            currentAltitudeCounter = 0;
            // Calculate alititude as percentage with cut offs
            g_altCurrent = ((heliAltMin - meanVal) * 100) / HELI_RANGE;
            if (g_altCurrent < 0)
            {
                g_altCurrent = 0;
            }
            else if (g_altCurrent > 100)
            {
                g_altCurrent = 100;
            }

            // LANDING SHUTOFF
            if (g_altCurrent == 0 && g_altRef == 0)
            {
                g_heliState = GROUNDED;
            }
            else
            {
                g_heliState = FLYING;
            }
        }
        currentAltitudeCounter++;

        // UPDATE FLIGHT SCREEN
        if (displayCounter >= 100)
        {
            displayCounter = 0;
            updateFlightData(g_altCurrent, g_yawCurrent, g_tailDuty, g_mainDuty, g_altRef, g_yawRef);
        }
        displayCounter++;

        // POLL BUTTONS
        if (buttonCounter >= 20 && !g_inputDisabled)
        {
            buttonCounter = 0;
            //ALTITUDE SET
            if ((checkButton(UP) == PUSHED) && (g_altRef < MAX_ALT))
            {
                if (g_altRef <= (MAX_ALT - ALT_STEP))
                {
                    g_altRef += ALT_STEP;
                }
                else
                {
                    g_altRef = 100;
                }
            }
            if ((checkButton(DOWN) == PUSHED) && (g_altRef > MIN_ALT))
            {
                if (g_altRef >= (MIN_ALT + ALT_STEP))
                {
                    g_altRef -= ALT_STEP;
                }
                else
                {
                    g_altRef = 0;
                }
            }

            // YAW SET (L/R SWAPPED SINCE CODE WAS TESTED AS IT WAS INVERTED)
            if (checkButton(RIGHT) == PUSHED)
            {
                g_yawRef = ((g_yawRef + YAW_STEP) % 360);
            }
            if (checkButton(LEFT) == PUSHED)
            {
                g_yawRef = (360 + (g_yawRef - YAW_STEP)) % 360;
            }

            updateButtons();
        }
        buttonCounter++;

        // TRANSMIT UART. 500 GIVES ABOUT 6HZ. (DETERMINED EXPERIMENTALLY)
        if (g_uartCount >= 500)
        {
            g_uartCount = 0;
            UARTSendHeli(g_yawCurrent, g_yawRef, g_tailDuty, g_altRef, g_altCurrent, g_mainDuty, g_heliState);
        }
        g_uartCount++;
    }
}
