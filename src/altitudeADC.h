/*
 * altitudeADC.h
 *
 *  Created on: 12/05/2021
 *      Authors: Nathaniel Stainton, Saman Ighani, Jack Walsh
 */

#ifndef SRC_ALTITUDEADC_H_
#define SRC_ALTITUDEADC_H_

// Constants
#define BUF_SIZE 50
#define SAMPLE_RATE_HZ 500 //By Nyquist Theorem it must be greater than 8, however for polling buttons, this is being done at 500hz

// Global Variables
extern circBuf_t g_inBuffer; // Buffer of size BUF_SIZE integers (sample values)
//uint32_t g_ulSampCnt;    // Counter for the interrupts

void ADCIntHandler(void);
void initADC(void);

#endif /* SRC_ALTITUDEADC_H_ */
