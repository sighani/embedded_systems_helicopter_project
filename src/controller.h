/*
 * controller.h
 *
 *  Created on: 14/05/2021
 *      Authors: Nat
 * 
 *  Reads global variables describing current and reference position to calculate appropriate PWM signal.  
 */

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

extern int16_t g_yawCurrent;
extern int16_t g_yawRef;

extern int16_t g_altCurrent;
extern int16_t g_altRef;

extern int8_t g_altControllerTrigger;
extern int8_t g_yawControllerTrigger;

void controllerAltitude();
void controllerYaw();

#endif // __CONTROLLER_H__
