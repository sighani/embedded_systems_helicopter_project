#include <stdint.h>

#include "controller.h"
#include "rotors.h"
#include "setup.h"
#include "altitudeADC.h"

#define MKp 0.5
#define MKi 0.01

#define TKp 1.0
#define TKi 0.06

#define TEETH_NUM 112

#define TEETHINDEG ((10 * 360) / (TEETH_NUM * 4))

int16_t g_yaw_current;
int16_t g_yaw_ref;

int16_t g_alt_current;
int16_t g_alt_ref;

int8_t g_altControllerTrigger;
int8_t g_yawControllerTrigger;

int8_t g_setpoint_change;

double g_intcounterAlt;
double g_intcounterYaw;


void controllerAltitude()
{
    //Update Integral Gain

    float plantInput;
    float error = g_alt_ref - g_alt_current;

    g_intcounterAlt = g_intcounterAlt + error;

    plantInput =  (error * MKp) + (MKi * g_intcounterAlt);
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
    int16_t yaw_current = ((((g_yaw_current * TEETHINDEG) / 10) % 360));

    float plantInput;

    // -------------------------------------------------------
    // Mostly figured out on a whiteboard but rescued by StackOverflow
    // Thread : [ADD THREAD URL HERE]

    float error = (((g_yaw_ref - yaw_current + 180 ) % 360) - 180);

    if (error < -180) {
        error = error + 360;

    }
    // -------------------------------------------------------
//    error = error * -1;
//    g_tail_duty = error;

    g_intcounterYaw = g_intcounterYaw + error / 100;

    if (g_intcounterYaw >= 50) {
        g_intcounterYaw = 50;
    } else if (g_intcounterYaw <= -50) {
        g_intcounterYaw = -50;
    }

    plantInput = (error * TKp) + (TKi * g_intcounterYaw);
    // (Ki * g_intbuff) probably needs to be divided by the frequency of the systick int handler alternatively the gain itsself could just factor it in.

    //Clamp output
    if (plantInput > 98) {
        plantInput = 98;
    } else if (plantInput < 2) {
        plantInput = 2;
    }
    setTailPWM(plantInput);

}

