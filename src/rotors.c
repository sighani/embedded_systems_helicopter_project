/*
 * rotors.c
 *
 *  Created on: 14/05/2021
 *      Authors: Nat
 * 
 *  Abstracts away PWM generation to give straight forward control.
 *  TAKEN FROM LABCODE
 */

#include <stdint.h>
#include <stdbool.h>

#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/pwm.h"

#include "rotors.h"

// HW CONFIG
#define PWM_MAIN_BASE PWM0_BASE
#define PWM_MAIN_GEN PWM_GEN_3
#define PWM_MAIN_OUTNUM PWM_OUT_7
#define PWM_MAIN_OUTBIT PWM_OUT_7_BIT
#define PWM_MAIN_PERIPH_PWM SYSCTL_PERIPH_PWM0
#define PWM_MAIN_PERIPH_GPIO SYSCTL_PERIPH_GPIOC
#define PWM_MAIN_GPIO_BASE GPIO_PORTC_BASE
#define PWM_MAIN_GPIO_CONFIG GPIO_PC5_M0PWM7
#define PWM_MAIN_GPIO_PIN GPIO_PIN_5

#define PWM_TAIL_BASE PWM1_BASE
#define PWM_TAIL_GEN PWM_GEN_2
#define PWM_TAIL_OUTNUM PWM_OUT_5
#define PWM_TAIL_OUTBIT PWM_OUT_5_BIT
#define PWM_TAIL_PERIPH_PWM SYSCTL_PERIPH_PWM1
#define PWM_TAIL_PERIPH_GPIO SYSCTL_PERIPH_GPIOF
#define PWM_TAIL_GPIO_BASE GPIO_PORTF_BASE
#define PWM_TAIL_GPIO_CONFIG GPIO_PF1_M1PWM5
#define PWM_TAIL_GPIO_PIN GPIO_PIN_1

// PWM CONFIG
#define PWM_FREQ 200
#define PWM_INIT_DUTY 0
#define PWM_DIVIDER 4

// GLOBAL VARIABLE FOR DUTY CYCLES
uint16_t g_mainDuty;
uint16_t g_tailDuty;

void initMainRotor(void)
{
    SysCtlPeripheralReset(PWM_MAIN_PERIPH_GPIO);
    SysCtlPeripheralReset(PWM_MAIN_PERIPH_PWM);

    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_GPIO);

    GPIOPinConfigure(PWM_MAIN_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_MAIN_GPIO_BASE, PWM_MAIN_GPIO_PIN);

    PWMGenConfigure(PWM_MAIN_BASE, PWM_MAIN_GEN,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    setMainPWM(PWM_INIT_DUTY);

    PWMGenEnable(PWM_MAIN_BASE, PWM_MAIN_GEN);

    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);
}

void initTailRotor(void)
{
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_GPIO);

    GPIOPinConfigure(PWM_TAIL_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_TAIL_GPIO_BASE, PWM_TAIL_GPIO_PIN);

    PWMGenConfigure(PWM_TAIL_BASE, PWM_TAIL_GEN,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    // Set the initial PWM parameters
    setTailPWM(PWM_INIT_DUTY);

    PWMGenEnable(PWM_TAIL_BASE, PWM_TAIL_GEN);

    // Disable the output.  Repeat this call with 'true' to turn O/P on.
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, false);
}

void enableRotors(void)
{
    // Initialisation is complete, so turn on the output.
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true);
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, true);
}

void setMainPWM(uint32_t ui32Duty)
{
    g_mainDuty = ui32Duty;
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period =
        SysCtlClockGet() / PWM_DIVIDER / PWM_FREQ;

    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, ui32Period);
    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM,
                     ui32Period * ui32Duty / 100);
}

void setTailPWM(uint32_t ui32Duty)
{
    g_tailDuty = ui32Duty;
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period =
        SysCtlClockGet() / PWM_DIVIDER / PWM_FREQ;

    PWMGenPeriodSet(PWM_TAIL_BASE, PWM_TAIL_GEN, ui32Period);
    PWMPulseWidthSet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM,
                     ui32Period * ui32Duty / 100);
}
