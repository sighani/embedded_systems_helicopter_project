/*
 * yaw.c
 *
 *  Created on: 12/05/2021
 *      Authors: Nathaniel Stainton, Saman Ighani, Jack Walsh
 * 
 *  Controls Yaw intialisation and interupts
 */

#include <stdbool.h>
#include <stdint.h>

#include "labcode/circBufT.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "../OrbitOLED/OrbitOLEDInterface.h"
#include "inc/hw_memmap.h"
#include "labcode/buttons4.h"
#include "driverlib/gpio.h"

#include "controller.h"
#include "takeoff.h"

// HW CONFIG
#define YAWI_1 GPIO_INT_PIN_0
#define YAWI_2 GPIO_INT_PIN_1

#define YAWC_1 GPIO_PIN_0
#define YAWC_2 GPIO_PIN_1
#define YAW_REF GPIO_PIN_4

//TEETH -> DEGREES CONVERSION
#define TEETH_NUM 112
#define TEETHINDEG ((10 * 360) / (TEETH_NUM * 4))

// Variables
static uint8_t currentState = 0;
static uint8_t previousState = 0;

const int8_t increment[16] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};

//Yaw Registration
void YawIntHandler(void)
{
    int32_t rawBuffer;
    uint8_t bufferState;

    GPIOIntClear(GPIO_PORTB_BASE, YAWI_1 | YAWI_2); // First function IAW line 885 driverlib/gpio.c

    rawBuffer = GPIOPinRead(GPIO_PORTB_BASE, (YAWC_1 | YAWC_2));
    // returning a byte with status of each pin (31 - 8 should be ignored) e.g. .... .... 0000 0010, bits 2+ will remain 0.

    previousState = currentState;

    currentState = rawBuffer & 0x03;

    bufferState = currentState | (previousState << 2); // 0000 YYXX Y = Previous State, X = Current State
    //these are done so that ascending order is clockwise assuming sensor moves relative to gears as said in lecture notes

    g_yawCurrent = g_yawCurrent + increment[bufferState];

    // Remove drift on Test Rig.

    if (g_yawCurrent < 0)
    {
        g_yawCurrent = ((TEETH_NUM * 4) - 1);
    }
    else if (g_yawCurrent > ((TEETH_NUM * 4) - 1))
    {
        g_yawCurrent = 0;
    }
}

void initYaw(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //    Wait for port to initialise
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {
        continue;
    }
    GPIOIntRegister(GPIO_PORTB_BASE, YawIntHandler);

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, YAWC_1 | YAWC_2);
    GPIOIntTypeSet(GPIO_PORTB_BASE, YAWC_1 | YAWC_2, GPIO_BOTH_EDGES);
    GPIOIntEnable(GPIO_PORTB_BASE, YAWI_1 | YAWI_2);

    // CODE BELOW LEFT FOR... POSTERITY.

    //reference pin init
    //    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    //    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
    //    {
    //        continue;
    //    }
    // GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, YAW_REF);
    // GPIOPadConfigSet(GPIO_PORTC_BASE, YAW_REF,
    //                  GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    //
    //    //Int Priorities
    //    //    IntPrioritySet(INT_GPIOC, 0x20);
    //    //    IntPrioritySet(INT_GPIOB, 0x60);
    //
    //reference pin interrupt
    // GPIOIntRegister(GPIO_PORTC_BASE, yawRefIntHandler);
    // GPIOIntTypeSet(GPIO_PORTC_BASE, YAW_REF, GPIO_FALLING_EDGE);
    // GPIOIntEnable(GPIO_PORTC_BASE, YAW_REF);
}
