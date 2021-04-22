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
#include "setup.h"
#include "driverlib/sysctl.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"
#include "labcode/buttons4.h"

#define HELIRANGE ((4095 * 10)/33)
// Maps 2^12 - 1 values to a 3.3V range. Then calculates bit range for 0.8V
#define TEETHINDEG ((10 * 365) / (TEETH_NUM*2))

//Heli Altitude Variables
static int32_t heliAltMax;
static int32_t heliAltMin;
static int32_t heliAltCurrent;
static int32_t heliAltPercentage;

// Function Definitions
void resetAltimeter(uint32_t meanVal);
void displayPercentage(uint32_t heliPercentage);
void displayMeanVal(uint16_t meanVal);
void displayBlank(void);
void displayMessage(uint16_t meanVal, uint32_t f_displayMode, uint32_t heliPercentage);



int main(void) {
//  Variable Definitions
    uint16_t i;
    int32_t sum;
    int32_t meanVal;
    uint32_t f_displayMode = 0;
    uint8_t initflag = 1;
    uint8_t messcount = 0;

//  Calls for Initialisation
    initButtons();
    initClock();
    initADC();
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
        if (initflag && meanVal > 0) {
            initflag = 0;
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
            displayMessage(meanVal, f_displayMode, heliAltPercentage);
        }
        messcount++;


        // Button Logic
        // Switch Display
            if ((checkButton (UP) == PUSHED)) {
                f_displayMode = (f_displayMode + 1) % 4;
            }
       // Re-zero altimeter
            if ((checkButton (LEFT) == PUSHED)) {
                resetAltimeter(meanVal);
            }

        updateButtons();
        SysCtlDelay (SysCtlClockGet() / 150);  // Update display at ~ 4 Hz


    }

}


// Utility Functions
void resetAltimeter(uint32_t meanVal) {
    heliAltPercentage = 0;
    heliAltMin = meanVal;
}

// Function for writing to OLED
// Functions are adaptions from ADCDemo.c Lab 3 code.
void displayPercentage(uint32_t heliPercentage) {
    char string[17];  // 16 characters across the display
    OLEDStringDraw ("Altimeter", 0, 0);
    usnprintf (string, sizeof(string), "%% of max  %3d%%", heliPercentage);
    OLEDStringDraw (string, 0, 1);
    OLEDStringDraw ("", 0, 3);
}
void displayYaw(uint32_t yaw) {
    char string[17];  // 16 characters across the display
    OLEDStringDraw ("Relative Direction", 0, 0);
    usnprintf (string, sizeof(string), "%3d Deg", ( (yaw * TEETHINDEG) / 10) );
    OLEDStringDraw (string, 0, 1);
    OLEDStringDraw ("", 0, 3);
}

void displayMeanVal(uint16_t meanVal) {
    char string[17];  // 16 characters across the display

    OLEDStringDraw ("Altimeter", 0, 0);
    usnprintf (string, sizeof(string), "Mean ADC = %4d", meanVal);
    OLEDStringDraw (string, 0, 1);
    OLEDStringDraw ("", 0, 3);
}

void displayBlank(void) {
    OLEDStringDraw ("                ", 0, 0);
    OLEDStringDraw ("                ", 0, 1);
    OLEDStringDraw ("                ", 0, 3);
}

void displayMessage(uint16_t meanVal, uint32_t f_displayMode, uint32_t heliPercentage) {
    if (f_displayMode == 0) {
        displayPercentage(heliPercentage);
    } else  if (f_displayMode == 1) {
        displayMeanVal(meanVal);
    } else if (f_displayMode == 2 ) {
        displayBlank();
    } else if (f_displayMode == 3 ) {
        displayYaw(g_yaw);
    } else {
        //THIS 'SHOULD' NEVER HAPPEN
    }
}
