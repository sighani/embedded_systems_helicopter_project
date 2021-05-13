#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#define Kp 0.1
#define Ki 0.01

uint16_t stepInputAltitude(uint16_t ref, uint16_t current);
void updateIntegralErrorAltitude();

extern float g_intcounter;

#endif // __CONTROLLER_H__