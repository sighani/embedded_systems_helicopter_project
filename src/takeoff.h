/*
 * takeoff.h
 *
 *  Created on: 18/05/2021
 *      Author: Author: Nathaniel Stainton, Saman Ighani, Jack Walsh
 */

#ifndef SRC_TAKEOFF_H_
#define SRC_TAKEOFF_H_

typedef enum flyingState{GROUNDED, LANDING, FLYING, CALIBRATE} flyingState;

void initFSM();
void initSwitchInt(void);
void switchIntHandler(void);


extern flyingState g_heliState;


#endif /* SRC_TAKEOFF_H_ */
