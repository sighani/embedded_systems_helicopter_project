/*
 * display.c
 *
 *  Created on: 22/04/2021
 *      Author: Nathaniel Stainton, Saman Ighani, Jack Walsh
 */
#include <stdint.h>

#include "../OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"

#include "display.h"

#define TEETH_NUM 112

#define TEETHINDEG ((10 * 360) / (TEETH_NUM * 4))

void updateFlightData(uint32_t, heli_alt, int16_t yaw, uint32_t duty_tail, uint32_t duty_main);
{
    char string[17]; // 16 characters across the display
    usnprintf(string, sizeof(string), "Alti %3d%%", heliPercentage);
    OLEDStringDraw(string, 0, 0);

    usnprintf(string, sizeof(string), "Yaw: %4d Deg", ((((yaw * TEETHINDEG) / 10) % 360)));
    OLEDStringDraw(string, 0, 1);

    usnprintf(string, sizeof(string), "M:%3%% T:%3%%", duty_main, duty_tail);
    OLEDStringDraw(string, 0, 2);
}
