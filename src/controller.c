#include <stdint.h>

#include "controller.h"
#include "rotors.h"
#include "setup.h"
#include "altitudeADC.h"

#define MKp 1.05
#define MKi 0.5

#define TKp 0.1
#define TKi 0.05

int16_t g_yaw_current;
int16_t g_yaw_ref;

int16_t g_alt_current;
int16_t g_alt_ref;

int8_t g_altControllerTrigger;
int8_t g_yawControllerTrigger;

float g_intcounterAlt;
float g_intcounterYaw;


void controllerAltitude()
{
    //Update Integral Gain
    // g_intcounterAlt = g_intcounterAlt + (g_alt_current - g_alt_ref);

    float plantInput;
    float error = g_alt_ref - g_alt_current;

    plantInput =  (error * MKp); //+ (MKi * g_intcounterAlt);
    // (Ki * g_intbuff) probably needs to be divided by the frequency of the systick int handler alternatively the gain itsself could just factor it in.

    //Clamp output
    if (plantInput > 98) {
        plantInput = 98;
    } else if (plantInput < 2) {
        plantInput = 2;
    }
    setMainPWM(plantInput);

}

void controllerYaw()
{
    //Update Integral Gain
    // g_intcounterYaw = g_intcounterYaw + (g_yaw_current - g_yaw_ref);


    float plantInput;
    float error = g_yaw_ref - g_alt_current;

    plantInput = (error * TKp);// + (TKi * g_intcounterYaw);
    // (Ki * g_intbuff) probably needs to be divided by the frequency of the systick int handler alternatively the gain itsself could just factor it in.

    //Clamp output
    if (plantInput > 98) {
        plantInput = 98;
    } else if (plantInput < 2) {
        plantInput = 2;
    }
    setTailPWM(plantInput);

}

