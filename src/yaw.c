/*
 * yaw.c
 *
 *  Created on: 12/05/2021
 *      Authors: Nathaniel Stainton, Saman Ighani, Jack Walsh
 */

#include <stdbool.h>
#include <stdint.h>
#include "labcode/circBufT.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "../OrbitOLED/OrbitOLEDInterface.h"
#include "inc/hw_memmap.h"
//#include "driverlib/interrupt.h"
//#include "inc/tm4c123gh6pm.h"
#include "labcode/buttons4.h"
#include "driverlib/gpio.h"
#include "controller.h"
#include "takeoff.h"


// Constants
#define YAWI_1 GPIO_INT_PIN_0
#define YAWI_2 GPIO_INT_PIN_1

#define YAWC_1 GPIO_PIN_0
#define YAWC_2 GPIO_PIN_1

#define YAW_REF GPIO_PIN_4

#define TEETH_NUM 112

#define TEETHINDEG ((10 * 360) / (TEETH_NUM * 4))

// Global Variables
//uint32_t g_ulSampCnt;    // Counter for the interrupts

static int16_t yaw_ref;
static int16_t absyawref;
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

    g_yaw_current = g_yaw_current + increment[bufferState];

    // Remove drift on Test Rig.

    if (g_yaw_current < 0)
    {
        g_yaw_current = ((TEETH_NUM * 4) - 1);
    }
    else if (g_yaw_current > ((TEETH_NUM * 4) - 1))
    {
        g_yaw_current = 0;
    }
}

//ONLY WORKS ON HELICOPTER
void yawRefIntHandler(void)
{
    GPIOIntClear (GPIO_PORTC_BASE, YAW_REF);
    if(g_heliState == CALIBRATE) {
        if (GPIOPinRead(GPIO_PORTC_BASE, YAW_REF) == YAW_REF) {
            absyawref = g_yaw_current;
            g_yaw_ref = absyawref;
            g_yaw_current = 0;
            setHeliState(FLYING);
        }

    } else if (g_heliState == LANDING) {
        if (GPIOPinRead(GPIO_PORTC_BASE, YAW_REF) == YAW_REF) {
            absyawref = g_yaw_current;
            g_yaw_ref = absyawref;
            g_yaw_current = 0;
            setHeliState(GROUNDED);
        }
    }
// set a absyawref variable to g yaw current. absyawref is NOT g_yaw_ref
//    set g_yaw_ref to absyawref

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

    // ONLY WORKS ON HELICOPTER. TEST RIGS DO NOT HAVE A REF PIN.
    //reference pin init
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
//    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
//    {
//        continue;
//    }
//    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, YAW_REF);
//    GPIOPadConfigSet(GPIO_PORTC_BASE, YAW_REF,
//                     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
//
//    //Int Priorities
//    //    IntPrioritySet(INT_GPIOC, 0x20);
//    //    IntPrioritySet(INT_GPIOB, 0x60);
//
//    //reference pin interrupt
//    GPIOIntRegister(GPIO_PORTC_BASE, yawRefIntHandler);
//    GPIOIntTypeSet(GPIO_PORTC_BASE, YAW_REF, GPIO_FALLING_EDGE);
//    GPIOIntEnable(GPIO_PORTC_BASE, YAW_REF);
}
