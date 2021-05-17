/*
 * setup.h
 *
 *  Created on: 19/03/2021
 *      Author: Nathaniel Stainton, Saman Ighani, Jack Walsh
 */

#ifndef SRC_SETUP_H_
#define SRC_SETUP_H_

void initClock(void);
void initDisplay(void);

// Global Variables
//extern uint32_t g_ulSampCnt;    // Counter for the interrupts
extern uint8_t g_uartFlag;

#endif /* SRC_SETUP_H_ */
