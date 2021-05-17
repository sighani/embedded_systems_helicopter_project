#include <stdint.h>

#include "controller.h"
#include "rotors.h"
#include "setup.h"
#include "altitudeADC.h"


int16_t g_yaw_current;
int16_t g_yaw_ref;

int16_t g_alt_current;
int16_t g_alt_ref;

uint16_t g_main_duty;
uint16_t g_tail_duty;

float g_intcounter;

#define PWM_FREQ 200

void controllerAltitude()
{
    //Update Integral Gain
    g_intcounter = g_intcounter + (g_alt_current - g_alt_ref);


    float plantInput;
    float error = g_alt_ref - g_alt_current;

    plantInput =  (error * Kp) + (Ki * g_intcounter);
    // (Ki * g_intbuff) probably needs to be divided by the frequency of the systick int handler alternatively the gain itsself could just factor it in.

    //Clamp output
    if (plantInput > 100) {
        plantInput = 100;
    } else if (plantInput < 0) {
        plantInput = 0;
    }
    setMainPWM(PWM_FREQ,plantInput);
    g_main_duty = plantInput;
    g_tail_duty = plantInput;

}

