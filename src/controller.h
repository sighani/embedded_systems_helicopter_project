#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#define Kp 0.1
#define Ki 0.01

extern float g_intcounter;

extern int16_t g_yaw_current;
extern int16_t g_yaw_ref;

extern int16_t g_alt_current;
extern int16_t g_alt_ref;

void controllerAltitude();

#endif // __CONTROLLER_H__
