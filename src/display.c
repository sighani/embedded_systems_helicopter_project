/*
 * display.c
 *
 *  Created on: 22/04/2021
 *      Author: Nathaniel Stainton, Saman Ighani, Jack Walsh
 * 
 *  Writes inputs to OLED display on BoosterPack.
 */
#include <stdint.h>
#include <stdbool.h>

#include "../OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"

#include "display.h"
#include "takeoff.h"

//TEETH -> DEGREES CONVERSION
#define TEETH_NUM 112
#define TEETH_IN_DEG ((10 * 360) / (TEETH_NUM * 4))

/*
 * OUTPUT IS READ AS FOLLOWS.
 * ALTI : [Altitude as %] 
 * YAW  : [YAW IN DEG]
 * H    : [DUTY MAIN ROTOR] T   : [DUTY MAIN ROTOR]
 * H    : [TARGET MAIN ROTOR] T   : [TARGET MAIN ROTOR]
 */
void updateFlightData(int16_t heliAlt, int16_t yaw, uint16_t dutyTail, uint16_t dutyMain, int16_t altRef, int16_t tailRef)
{
    char string[17];
    usnprintf(string, sizeof(string), "Alti %3d%%", heliAlt);
    OLEDStringDraw(string, 0, 0);

    usnprintf(string, sizeof(string), "Yaw: %4d Deg", (((yaw * TEETH_IN_DEG) / 10) % 360));
    OLEDStringDraw(string, 0, 1);

    usnprintf(string, sizeof(string), "M:%3d%% T:%3d%%", dutyMain, dutyTail);
    OLEDStringDraw(string, 0, 2);

    usnprintf(string, sizeof(string), "M:%3d T:%3d", altRef, tailRef);
    OLEDStringDraw(string, 0, 3);
}
