#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#define Kp 1
#define Ki 0.5

extern float g_intcounterAlt;
extern float g_intcounterYaw;

extern int16_t g_yaw_current;
extern int16_t g_yaw_ref;

extern int16_t g_alt_current;
extern int16_t g_alt_ref;

void controllerAltitude();
void controllerYaw();

#endif // __CONTROLLER_H__
