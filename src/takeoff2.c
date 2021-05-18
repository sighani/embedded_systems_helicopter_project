/*
 * takeoff2.c
 *
 *  Created on: 18/05/2021
 *      Author: jwa227
 */

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/pin_map.h"
#include "utils/ustdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "takeoff.h"


flyingState heliState;


flyingState getHeliState(void)
{
    return heliState;
}




