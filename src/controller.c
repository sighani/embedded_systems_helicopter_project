#include <stdint.h>

#include "controller.h"
#include "setup.h"
#include "altitudeADC.h"


int16_t g_yaw_current;
int16_t g_yaw_ref;

int16_t g_alt_current;
int16_t g_alt_ref;

uint16_t controllerAltitude(uint16_t ref, uint16_t current)
{
    //Update Integral Gain
    g_intcounter = g_intcounter + (g_heliAltPercentage - g_heliAltSetpoint);


    float temp;
    float error = ref - current;

    temp = ref + (error * Kp) + (Ki * g_intcounter);
    // (Ki * g_intbuff) probably needs to be divided by the frequency of the systick int handler alternatively the gain itsself could just factor it in.

    return temp;
}

void updateIntegralErrorAltitude()
{
}
