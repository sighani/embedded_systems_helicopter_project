#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/pwm.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "utils/ustdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "driverlib/adc.h"


void SwitchIntHandler(void)
{
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_7);

    //if not in process of landing
        //if flying
            //start landing sequence
        //else
            //start takeoff sequence

    //we are only interested in switch reads when it is not landing
        //the two cases for this is when taking off and to start landing

    //Issues will arise if the switch is left in the high position during
    //landing, may just be trivial, could be fixed with monitoring the state
    //however this will make the interupt inefficient

}


void SwitchIntInit(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOIntRegister(GPIO_PORTA_BASE, SwitchIntHandler);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD_WPU);
    GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_BOTH_EDGES);
    GPIOIntEnable(GPIO_PORTA_BASE, GPIO_PIN_7);
    switchState = 0;

}
