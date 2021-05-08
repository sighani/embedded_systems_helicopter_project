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
#include "display.h"


#define HELIRANGE ((4095 * 10)/30)
#define SYSDISPLAYDIV 150

// Maps 2^12 - 1 values to a 3.3V range. Then calculates bit range for 0.8V

//Heli Altitude Variables
static int32_t heliAltMax;
static int32_t heliAltMin;
static int32_t heliAltCurrent;
static int32_t heliAltPercentage;


//Function Declarations
void resetAltimeter(uint32_t meanVal);


int main(void) {
//  Variable Definitions
    uint16_t i;
    int32_t sum;
    int32_t meanVal;
    uint8_t initFlag = 1;
    uint8_t messcount = 0;

//  Calls for Initialisation
    initButtons();
    initClock();
    initADC();
    initYaw();
    initCircBuf (&g_inBuffer, BUF_SIZE);
    initDisplay();
    g_yaw = 0;

//  Enable ISPs
    IntMasterEnable();


    while(1) {

        // Taken from ADCDemo.c Lab3
        // Background task: calculate the (approximate) mean of the values in the
        // circular buffer and display it, together with the sample number.
        sum = 0;
        for (i = 0; i < BUF_SIZE; i++) {
            sum = sum + readCircBuf (&g_inBuffer);
        }
        meanVal = (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;



        // Run on first start up
        if (initFlag && meanVal > 0) {
            initFlag = 0;
            heliAltPercentage = 0;
            heliAltMin = meanVal;
            heliAltMax = meanVal - HELIRANGE;
        }

        // Calculate alititude as percentage with cut offs
        heliAltPercentage = ((heliAltMin - meanVal) * 100 ) / HELIRANGE;
        if (heliAltPercentage < 0) {
            heliAltPercentage = 0;
        }
        else if (heliAltPercentage > 100) {
            heliAltPercentage = 100;
        }

        if (messcount >= 12) {
            messcount = 0;   
            displayMessage(meanVal, displayMode, heliAltPercentage,g_yaw);
        }
        messcount++;


        // Button Logic
        // Switch Display
            if ((checkButton (UP) == PUSHED)) {
                OrbitOledClear();
                displayMode = (displayMode + 1) % 4;
            }
       // Re-zero altimeter
            if ((checkButton (LEFT) == PUSHED)) {
                resetAltimeter(meanVal);
            }

        updateButtons();
        SysCtlDelay (SysCtlClockGet() / SYSDISPLAYDIV);  // Update display at ~ 4 Hz


    }

}

// Utility Functions
void resetAltimeter(uint32_t meanVal) {
    heliAltPercentage = 0;
    heliAltMin = meanVal;
}


