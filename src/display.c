/*
 * display.c
 *
 *  Created on: 22/04/2021
 *      Author: Nathaniel Stainton, Saman Ighani, Jack Walsh
 */
#include <stdint.h>
#include <stdbool.h>

#include "../OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"

#include "display.h"
#include "takeoff.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"


#define TEETH_NUM 112

#define TEETHINDEG ((10 * 360) / (TEETH_NUM * 4))

void updateFlightData(uint32_t heli_alt, int16_t yaw, uint16_t duty_tail, uint16_t duty_main, int16_t alt_ref, int16_t tail_ref)
{
    char string[17]; // 16 characters across the display
    usnprintf(string, sizeof(string), "Alti %3d%% %1d", heli_alt, g_heliState);
    OLEDStringDraw(string, 0, 0);

    usnprintf(string, sizeof(string), "Yaw: %4d Deg", ((((yaw * TEETHINDEG) / 10) % 360)));
    OLEDStringDraw(string, 0, 1);

    usnprintf(string, sizeof(string), "M:%3d%% T:%3d%%", duty_main, duty_tail);
    OLEDStringDraw(string, 0, 2);

    usnprintf(string, sizeof(string), "M:%3d%% T:%3d", alt_ref, tail_ref);
    OLEDStringDraw(string, 0, 3);


}
