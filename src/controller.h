#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#define MKp 10
#define MKi 0.5

#define TKp 0.1
#define TKi 0.05

extern float g_intcounterAlt;
extern float g_intcounterYaw;

extern int16_t g_yaw_current;
extern int16_t g_yaw_ref;

extern int16_t g_alt_current;
extern int16_t g_alt_ref;

void controllerAltitude();
void controllerYaw();

#endif // __CONTROLLER_H__
