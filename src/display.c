/*
 * display.c
 *
 *  Created on: 22/04/2021
 *      Author: Nat
 */
#include <stdint.h>

#include "OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"

#include "display.h"

#define TEETH_NUM 112

#define TEETHINDEG ((10 * 360) / (TEETH_NUM*4))

displayMode_e displayMode = ALTIPER;


// Function for writing to OLED
// Functions are adaptions from ADCDemo.c Lab 3 code.
void displayPercentage(uint32_t heliPercentage) {
    char string[17];  // 16 characters across the display
    OLEDStringDraw ("Altimeter", 0, 0);
    usnprintf (string, sizeof(string), "%% of max  %3d%%", heliPercentage);
    OLEDStringDraw (string, 0, 1);
    OLEDStringDraw ("", 0, 3);
}
void displayYaw(int16_t yaw) {
    char string[17];  // 16 characters across the display
    OLEDStringDraw ("Relative Dir", 0, 0);
    usnprintf (string, sizeof(string), "%3d Deg", ((((yaw * TEETHINDEG) / 10) % 360) - 180) );
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
    OrbitOledClear();
}

void displayMessage(uint16_t meanVal, uint32_t displayMode, uint32_t heliPercentage, int16_t yaw) {
    if (displayMode == ALTIPER) {
        displayPercentage(heliPercentage);
    } else if (displayMode == ALTIMEAN) {
        displayMeanVal(meanVal);
    } else if (displayMode == YAWDEG) {
        displayYaw(yaw);
    } else if (displayMode == BLANK) {
        displayBlank();
    } else {
        //THIS 'SHOULD' NEVER HAPPEN
    }
}
