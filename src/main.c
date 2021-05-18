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

// Maps 2^12 - 1 values to a 3V range. Then calculates bit range for 0.8V
#define HELIRANGE ((4095 * 10) / 30)
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


    enableRotors();

    //  Enable ISPs
    IntMasterEnable();

    while (1)
    {

        if (circBufferCounter >= 0) {
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

        if (altInitCounter >= 0) {
            altInitCounter = 0;
            // Run on first start up
            if (initFlag && meanVal > 500)
            {
                initFlag = 0;
                g_alt_current = 0;
                heliAltMin = meanVal;
            }
        }
        altInitCounter++;

        if (currentAltitudeCounter >= 0) {
            currentAltitudeCounter = 0;
            // Calculate alititude as percentage with cut offs
            g_alt_current = ((heliAltMin - meanVal) * 100) / HELIRANGE;
    //        if (g_alt_current < 0)
    //        {
    //            g_alt_current = 0;
    //        }
    //        else if (g_alt_current > 100)
    //        {
    //            g_alt_current = 100;
    //        }
        }
        currentAltitudeCounter++;

        if (displayCounter >= 100)
        {
            displayCounter = 0;
            updateFlightData(g_alt_current, g_yaw_current, g_tail_duty, g_main_duty, g_alt_ref, g_yaw_ref);
        }
        displayCounter++;


        if (buttonCounter >= 0) {
            buttonCounter = 0;
            // Button Logic
            if ((checkButton(UP) == PUSHED) && (g_alt_ref < MAX_ALT)) {
                if (g_alt_ref <= (MAX_ALT-ALT_STEP)) {
                   g_alt_ref += ALT_STEP;
               } else {
                   g_alt_ref = 100;
               }
            }
            if ((checkButton(DOWN) == PUSHED) && (g_alt_ref > MIN_ALT))
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
        if (checkButton(LEFT) == PUSHED)
        {
//            g_setpoint_change = 1;
            g_yaw_ref = (( g_yaw_ref + YAW_STEP) % 360);

        }


        if (checkButton(RIGHT) == PUSHED)
        {
//            g_setpoint_change = 1;
            g_yaw_ref = (360 + ( g_yaw_ref - YAW_STEP)) % 360;
        buttonCounter++;

        if (g_uartCount >= 100) {
            g_uartCount = 0;
            UARTSendHeli(g_yaw_current, g_yaw_ref, g_tail_duty, g_alt_ref, g_alt_current, g_main_duty);
        }
        g_uartCount++;
    }
}
