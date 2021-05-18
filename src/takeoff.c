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

flyingState g_heliState;
bool pinpreviousState = 0;
bool pinState = 0;
bool g_inputDisabled;


void initFSM() {
    g_heliState = GROUNDED;
}


void setHeliState(flyingState newState)
{
    g_heliState = newState;
}


void switchIntHandler(void)
{
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_7);
    pinState = (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7) >> 7);
    flyingState local = g_heliState;
    if (g_heliState == GROUNDED && (!pinpreviousState)) {
        g_heliState = CALIBRATE;
    }
    pinpreviousState = g_heliState;

//    if (heliState != landing)
//    {
//        if (heliState == grounded && GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7) == GPIO_PIN_7)
//        {
//            heliState = hovering;
//        }
//        else if (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7) != GPIO_PIN_7)
//        {
//            heliState = flying;
//        }
//    }
}

void initSwitchInt(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)){}
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_7);
    GPIOIntRegister(GPIO_PORTA_BASE, switchIntHandler);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPD);
    GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_BOTH_EDGES);
    GPIOIntEnable(GPIO_PORTA_BASE, GPIO_PIN_7);
}
