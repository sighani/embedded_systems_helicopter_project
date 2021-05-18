#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

extern float g_intcounterAlt;
extern float g_intcounterYaw;

extern int16_t g_yaw_current;
extern int16_t g_yaw_ref;

extern int16_t g_alt_current;
extern int16_t g_alt_ref;

extern int8_t g_altControllerTrigger;
extern int8_t g_yawControllerTrigger;

void controllerAltitude();
void controllerYaw();

#endif // __CONTROLLER_H__
