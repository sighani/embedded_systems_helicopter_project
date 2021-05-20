/*
 * yaw.h
 *
 *  Created on: 12/05/2021
 *      Authors: Nathaniel Stainton, Saman Ighani, Jack Walsh
 * 
 *  Controls Yaw intialisation and interupts
 */

#ifndef SRC_YAW_H_
#define SRC_YAW_H_

// Constants
#define YAWI_1 GPIO_INT_PIN_0
#define YAWI_2 GPIO_INT_PIN_1

#define YAWC_1 GPIO_PIN_0
#define YAWC_2 GPIO_PIN_1

#define YAW_REF GPIO_PIN_4

#define TEETH_NUM 112

static uint8_t currentState = 0;
static uint8_t previousState = 0;

//Yaw Registration
void YawIntHandler(void);
void initYaw(void);

#endif /* SRC_YAW_H_ */
