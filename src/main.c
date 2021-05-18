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

// Maps 2^12 - 1 values to a 3V range. Then calculates bit range for 0.8V
#define HELIRANGE ((4095 * 8) / 30)
#define SYSDISPLAYDIV 150
#define ALT_STEP 10
#define MAX_ALT 100
#define MIN_ALT 0

#define YAW_STEP 15
#define MAX_YAW 360



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
    initialiseUSB_UART();
    initSwitchInt();
    initFSM();


    g_yaw_current = 0;

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
            updateFlightData(g_alt_current, g_yaw_current, g_tail_duty, g_main_duty, g_alt_ref, g_yaw_ref);
        }
        messcount++;

//        if (g_uartFlag) {
//            g_uartFlag = 0;
////            UARTSendHeli(g_yaw_current, g_yaw_ref, g_tail_duty, g_alt_ref, g_alt_current, g_main_duty);
//        }

        //TODO: Add logic for moving altitude up and down, need to use controller.c function and setpwm through pwm.c, these are then called in button up and down, have #defines to set altitude steps
        // Button Logic
        if ((g_alt_ref < MAX_ALT) && (checkButton(UP) == PUSHED))
        {
            if (g_alt_ref <= (MAX_ALT-ALT_STEP)) {
                g_alt_ref += ALT_STEP;
            } else {
                g_alt_ref = 100;
            }
        }
        if ((g_alt_ref < MIN_ALT) && (checkButton(DOWN) == PUSHED))
        {
            if (g_alt_ref >= (MIN_ALT+ALT_STEP)) {
                g_alt_ref -= ALT_STEP;
            } else {
                g_alt_ref = 0;
            }
        }

        if (checkButton(LEFT) == PUSHED)
        {
            g_yaw_ref = (360 + ( g_yaw_ref - YAW_STEP)) % 360;
        }
        if (checkButton(RIGHT) == PUSHED)
        {
            g_yaw_ref = (( g_yaw_ref + YAW_STEP) % 360);

        }
        updateButtons();
    }
}
