#include <stdint.h>

#include "controller.h"
#include "rotors.h"
#include "setup.h"
#include "altitudeADC.h"


int16_t g_yaw_current;
int16_t g_yaw_ref;

int16_t g_alt_current;
int16_t g_alt_ref;


float g_intcounterAlt;
float g_intcounterYaw;


void controllerAltitude()
{
    //Update Integral Gain
    g_intcounterAlt = g_intcounterAlt + (g_alt_current - g_alt_ref);


    float plantInput;
    float error = g_alt_ref - g_intcounterAlt;

    plantInput =  (error * Kp) + (Ki * g_intcounterAlt);
    // (Ki * g_intbuff) probably needs to be divided by the frequency of the systick int handler alternatively the gain itsself could just factor it in.

    //Clamp output
    if (plantInput > 100) {
        plantInput = 100;
    } else if (plantInput < 0) {
        plantInput = 0;
    }
    setMainPWM(plantInput);
    g_main_duty = plantInput;

}

void controllerYaw()
{
    //Update Integral Gain
    g_intcounterYaw = g_intcounterYaw + (g_yaw_current - g_yaw_ref);


    float plantInput;
    float error = g_yaw_ref - g_intcounterYaw;

    plantInput = (error * Kp) + (Ki * g_intcounterYaw);
    // (Ki * g_intbuff) probably needs to be divided by the frequency of the systick int handler alternatively the gain itsself could just factor it in.

    //Clamp output
    if (plantInput > 100) {
        plantInput = 100;
    } else if (plantInput < 0) {
        plantInput = 0;
    }
    setTailPWM(plantInput);
    g_tail_duty = plantInput;

}

