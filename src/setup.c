/*
 * setup.h
 *
 *  Created on: 19/03/2021
 *      Author: Nat
 *
 *  Extraction of initialisation code from ADCdemo1.c and button polling from lab 2
 */
#include <stdbool.h>
#include <stdint.h>
#include "labcode/circBufT.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/adc.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "inc/hw_memmap.h"
#include "labcode/buttons4.h"
#include "driverlib/gpio.h"


// Constants
#define BUF_SIZE 50
#define SAMPLE_RATE_HZ 500 //By Nyquist Theorem it must be greater than 8, however for polling buttons, this is being done at 500hz

#define YAWI_1 GPIO_INT_PIN_0
#define YAWI_2 GPIO_INT_PIN_1

#define YAWC_1 GPIO_PIN_0
#define YAWC_2 GPIO_PIN_1


// Global Variables
circBuf_t g_inBuffer;        // Buffer of size BUF_SIZE integers (sample values)
uint32_t g_ulSampCnt;    // Counter for the interrupts

// Interupts
void SysTickIntHandler(void) {
    ADCProcessorTrigger(ADC0_BASE, 3);
    g_ulSampCnt++;
}

void ADCIntHandler(void) {
    uint32_t ulValue;
    // Clean up, clearing the interrupt
    ADCIntClear(ADC0_BASE, 3);
    // ADCIntClear to the first function IAW with reccomendation line 350 of driverlib.c/adc.c

    // Get the single sample from ADC0.  ADC_BASE is defined in
    // inc/hw_memmap.h
    ADCSequenceDataGet(ADC0_BASE, 3, &ulValue);
    //
    // Place it in the circular buffer (advancing write index)
    writeCircBuf(&g_inBuffer, ulValue);


}

// Intialisation

void initClock(void) {
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
    //
    // Set up the period for the SysTick timer.  The SysTick timer period is
    // set as a function of the system clock.
    SysTickPeriodSet(SysCtlClockGet() / SAMPLE_RATE_HZ);
    //
    // Register the interrupt handler
    SysTickIntRegister(SysTickIntHandler);
    //
    // Enable interrupt and device
    SysTickIntEnable();
    SysTickEnable();
}

void initADC(void) {
    //
    // The ADC0 peripheral must be enabled for configuration and use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion.
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    //
    // Configure step 0 on sequence 3.  Sample channel 0 (ADC_CTL_CH0) in
    // single-ended mode (default) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
    // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
    // sequence 0 has 8 programmable steps.  Since we are only doing a single
    // conversion using sequence 3 we will only configure step 0.  For more
    // on the ADC sequences and steps, refer to the LM3S1968 datasheet.
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE |
                             ADC_CTL_END);

    //
    // Since sample sequence 3 is now configured, it must be enabled.
    ADCSequenceEnable(ADC0_BASE, 3);

    //
    // Register the interrupt handler
    ADCIntRegister (ADC0_BASE, 3, ADCIntHandler);

    //
    // Enable interrupts for ADC0 sequence 3 (clears any outstanding interrupts)
    ADCIntEnable(ADC0_BASE, 3);
}

//Yaw Registration

void YawIntHandler(void) {
    int32_t rawBuffer;
    uint8_t bufferState;

    GPIOIntClear(GPIO_PORTB_BASE, YAWI_1 | YAWI_2); // First function IAW line 885 driverlib/gpio.c

    rawBuffer = GPIOPinRead(GPIO_PORTB_BASE, (YAWC_1 | YAWC_1));
    // returning a byte with status of each pin (31 - 8 should be ignored) e.g. .... .... 0000 0010, bits 2+ will remain 0.

    bufferState = rawBuffer & 0x03 // 0000 00XX

}

void initYaw(void) {

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

//    Wait for port to intialise
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {
        continue;
    }
    GPIOIntRegister(GPIO_PORTB_BASE, YawIntHandler);

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, YAWC_1 | YAWC_2);
    GPIOIntTypeSet(GPIO_PORTB_BASE, YAWC_1 | YAWC_2, GPIO_BOTH_EDGES);

    GPIOIntEnable(GPIO_PORTB_BASE, YAWI_1 | YAWI_2);
}


void initDisplay(void) {
    // intialise the Orbit OLED display
    OLEDInitialise ();
}


