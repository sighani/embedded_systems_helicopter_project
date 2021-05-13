#include <stdint.h>

#include "controller.h"
#include "setup.h"
#include "altitudeADC.h"

uint16_t stepInputAltitude(uint16_t ref, uint16_t current)
{
    float temp;
    float error = ref - current;

    temp = ref + (error * Kp) + (Ki * g_intcounter);
    // (Ki * g_intbuff) probably needs to be divided by the frequency of the systick int handler alternatively the gain itsself could just factor it in.

    return temp;
}

void updateIntegralErrorAltitude()
{
    g_intcounter = g_intcounter + (g_heliAltPercentage - g_heliAltSetpoint);
}
