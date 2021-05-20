/*
 * controller.c
 *
 *  Created on: 14/05/2021
 *      Authors: Nat
 * 
 *  Reads global variables describing current and reference position to calculate appropriate PWM signal.  
 */

#include <stdint.h>

#include "controller.h"
#include "rotors.h"
#include "setup.h"
#include "altitudeADC.h"
#include "takeoff.h"

// MAIN ROTOR GAINS
#define MKp 1.8
#define MKi 0.05

// TAIL ROTOR GAINS
#define TKp 1.3
#define TKi 0.7

//CAP YAW INTEGRAL
#define YAWINTMAX 80

//TEETH -> DEGREES CONVERSION
#define TEETH_NUM 112
#define TEETH_IN_DEG ((10 * 360) / (TEETH_NUM * 4))

// GLOBAL VARIABLES TO STORE HELI ORI & HEIGHT
int16_t g_yawCurrent; // [teeth count]
int16_t g_yawRef;     // [deg]

int16_t g_altCurrent; // [%]
int16_t g_altRef;     // [%]

float altIntegralSum;
float yawIntegralSum;

//GLOBAL VARIABLE TO SLOW RESPONSE OF CONTROLLER SO IT DOESN'T HOG CYCLES.
int8_t g_altControllerTrigger;
int8_t g_yawControllerTrigger;

// Calculate Main Rotor PWM from error.
void controllerAltitude()
{
    //Update Integral Gain
    float plantInput;
    float error = g_altRef - g_altCurrent;
    altIntegralSum = altIntegralSum + error;

    plantInput = (error * MKp) + (MKi * altIntegralSum);

    // Clamp output unless grounded
    if (g_heliState != GROUNDED)
    {
        if (plantInput > 98)
        {
            plantInput = 98;
        }
        else if (plantInput < 2 && g_heliState != GROUNDED)
        {
            plantInput = 2;
        }
        setMainPWM(plantInput);
    }
    else
    {
        setMainPWM(0);
    }
}

// Calculate Tail Rotor PWM from error.
void controllerYaw()
{
    int16_t yawCurrent = ((((g_yawCurrent * TEETH_IN_DEG) / 10) % 360));
    float plantInput;

    // -------------------------------------------------------
    // Mostly figured out on a whiteboard but rescued by StackOverflow
    // Thread : https://stackoverflow.com/questions/28036652/finding-the-shortest-distance-between-two-angles/28037434
    float error = (((g_yawRef - yawCurrent + 180) % 360) - 180);
    if (error < -180)
    {
        error = error + 360;
    }
    // -------------------------------------------------------

    // scale error to make it play nice
    yawIntegralSum = yawIntegralSum + error / 50;

    // Clamp yaw int
    if (yawIntegralSum >= YAWINTMAX)
    {
        yawIntegralSum = YAWINTMAX;
    }
    else if (yawIntegralSum <= -1 * YAWINTMAX)
    {
        yawIntegralSum = -1 * YAWINTMAX;
    }

    plantInput = (error * TKp) + (TKi * yawIntegralSum);

    // Clamp output unless grounded
    if (g_heliState != GROUNDED)
    {
        //Clamp output
        if (plantInput > 98)
        {
            plantInput = 98;
        }
        else if (plantInput < 2)
        {
            plantInput = 2;
        }
        setTailPWM(plantInput);
    }
    else
    {
        setTailPWM(0);
    }
}
