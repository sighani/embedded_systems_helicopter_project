/*
 * altitudeADC.c
 *
 *  Created on: 22/04/2021
 *      Author: Nathaniel Stainton, Saman Ighani, Jack Walsh
 *  
 *  Samples ADC values for conversion to percentage height. Taken from Labcode.
 */

#ifndef SRC_ALTITUDEADC_H_
#define SRC_ALTITUDEADC_H_

#include "labcode/circBufT.h"

// Constants
#define BUF_SIZE 50
#define SAMPLE_RATE_HZ 500
//By Nyquist Theorem it must be greater than 8
//however this is being done at 500hz to create a more responsive system

// Global Variables
extern circBuf_t g_inBuffer; // Buffer of size BUF_SIZE integers (sample values)
//uint32_t g_ulSampCnt;    // Counter for the interrupts

void ADCIntHandler(void);
void initADC(void);

#endif /* SRC_ALTITUDEADC_H_ */
