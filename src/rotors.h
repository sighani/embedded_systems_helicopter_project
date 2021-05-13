#ifndef __ROTORS_H__
#define __ROTORS_H__

void initMainRotor();

void initTailRotor();

void enableRotors();

void setMainPWM(uint32_t ui32Freq, uint32_t ui32Duty);

void setTailPWM(uint32_t ui32Freq, uint32_t ui32Duty);

extern uint32_t g_main_duty;
extern uint32_t g_tail_duty;
#endif // __ROTORS_H__