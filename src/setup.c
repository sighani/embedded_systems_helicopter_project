/*
 * setup.c
 *
 *  Created on: 19/03/2021
 *      Author: Nathaniel Stainton, Saman Ighani, Jack Walsh
 *
 *  Setsup SYSTICK hander.
 *  TAKEN FROM LABCODE.
 */
#include <stdbool.h>
#include <stdint.h>
#include "labcode/circBufT.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/adc.h"
#include "../OrbitOLED/OrbitOLEDInterface.h"
#include "inc/hw_memmap.h"
#include "labcode/buttons4.h"
#include "driverlib/gpio.h"
#include "altitudeADC.h"
#include "yaw.h"
#include "controller.h"
#include "uartUSB.h"

#define PWM_DIVIDER_CODE SYSCTL_PWMDIV_4

// Interrupts
void SysTickIntHandler(void)
{
    ADCProcessorTrigger(ADC0_BASE, 3);

    //SLOW DOWN PI CONTROL
    if (g_altControllerTrigger >= 10)
    {
        g_altControllerTrigger = 0;
        controllerAltitude();
        controllerYaw();
    }
    g_altControllerTrigger++;
}

// Intialisation

void initClock(void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);

    // Set up the period for the SysTick timer.  The SysTick timer period is
    // set as a function of the system clock.
    SysTickPeriodSet(SysCtlClockGet() / SAMPLE_RATE_HZ);
    //
    // Register the interrupt handler
    SysTickIntRegister(SysTickIntHandler);

    // Set the PWM clock rate (using the prescaler)
    SysCtlPWMClockSet(PWM_DIVIDER_CODE);

    //
    // Enable interrupt and device
    SysTickIntEnable();
    SysTickEnable();
}

void initDisplay(void)
{
    // intialise the Orbit OLED display
    OLEDInitialise();
}
