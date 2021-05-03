/*
 * setup.h
 *
 *  Created on: 19/03/2021
 *      Author: Nat
 */

#ifndef SRC_SETUP_H_
#define SRC_SETUP_H_

void initClock(void);
void initADC(void);
void initDisplay(void);


// Constants
#define BUF_SIZE 10
#define SAMPLE_RATE_HZ 8 //By Nyquist Theorem

// Global Variables
extern circBuf_t g_inBuffer;        // Buffer of size BUF_SIZE integers (sample values)
//extern uint32_t g_ulSampCnt;    // Counter for the interrupts
extern int16_t g_yaw;

#endif /* SRC_SETUP_H_ */
