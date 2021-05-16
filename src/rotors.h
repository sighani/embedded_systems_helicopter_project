#ifndef __ROTORS_H__
#define __ROTORS_H__

#define PWM_MAIN_MAX_DUTY 95
#define PWM_MAIN_MIN_DUTY 5
#define PWM_TAIL_MAX_DUTY 95
#define PWM_TAIL_MIN_DUTY 5

void initMainRotor();

void initTailRotor();

void enableRotors();

void setMainPWM(uint32_t ui32Freq, uint32_t ui32Duty);

void setTailPWM(uint32_t ui32Freq, uint32_t ui32Duty);

extern uint32_t g_main_duty;
extern uint32_t g_tail_duty;
#endif // __ROTORS_H__
