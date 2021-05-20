/*
 * rotors.h
 *
 *  Created on: 14/05/2021
 *      Authors: Nat
 * 
 *  Abstracts away PWM generation to give straight forward control.
 *  TAKEN FROM LABCODE
 */

#ifndef __ROTORS_H__
#define __ROTORS_H__

#define PWM_MAIN_MAX_DUTY 95
#define PWM_MAIN_MIN_DUTY 5
#define PWM_TAIL_MAX_DUTY 95
#define PWM_TAIL_MIN_DUTY 5

void initMainRotor();

void initTailRotor();

void enableRotors();

void setMainPWM(uint32_t ui32Duty);

void setTailPWM(uint32_t ui32Duty);

extern uint16_t g_mainDuty;
extern uint16_t g_tailDuty;
#endif // __ROTORS_H__
