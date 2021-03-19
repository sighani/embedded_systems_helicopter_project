#include <stdint.h>
#include <stdbool.h>
#include "labcode/circBufT.h"
#include "driverlib/interrupt.h"
#include "setup.h"
#include "driverlib/sysctl.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"
#include "labcode/buttons4.h"


static uint32_t heliAltMax;
static uint32_t heliAltMin;
static uint32_t heliAltCurrent;
static uint32_t heliAltPercentage;

void displayPercentage(uint32_t heliPercentage) {
    char string[17];  // 16 characters across the display

    OLEDStringDraw ("Altimeter", 0, 0);

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
//    clear just in case
    OLEDStringDraw ("                ", 0, 1);
    usnprintf (string, sizeof(string), "%% of max  %3d%% ", heliPercentage);
    // Update line on display.
    OLEDStringDraw (string, 0, 1);

//    usnprintf (string, sizeof(string), "disp[%i]", f_displayMode);
    OLEDStringDraw ("", 0, 3);
}

void displayMeanVal(uint16_t meanVal) {
    char string[17];  // 16 characters across the display

    OLEDStringDraw ("Altimeter", 0, 0);

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf (string, sizeof(string), "Mean ADC = %4d", meanVal);
    OLEDStringDraw (string, 0, 1);

//    usnprintf (string, sizeof(string), "disp[%i]", f_displayMode);
    OLEDStringDraw ("", 0, 3);
}

void displayBlank(void) {
    OLEDStringDraw ("                ", 0, 0);
    OLEDStringDraw ("                ", 0, 1);
    OLEDStringDraw ("                ", 0, 3);
}

void displayMessage(uint16_t meanVal, uint32_t f_displayMode) {
    if (f_displayMode == 0) {
        displayPercentage(meanVal, f_displayMode);
    } else  if (f_displayMode == 1) {
        displayMeanVal(meanVal, f_displayMode);
    } else if (f_displayMode == 2 ) {
        displayBlank(f_displayMode);
    } else {
        //THIS 'SHOULD' NEVER HAPPEN
    }
}

void resetAltimeter(void) {
    heliAltMin = heliAltCurrent;
    heliAltPercentage = 0;
}

int main(void) {
    uint16_t i;
    int32_t sum;
    uint32_t meanVal;
    uint32_t f_displayMode = 0;


//  Initialisations
    initButtons();

    initClock();

    initADC();
    initDisplay();
    initCircBuf (&g_inBuffer, BUF_SIZE);

//    Enable ISPs

    IntMasterEnable();


//    TODO: SET heliAltCurrent

    resetAltimeter();

    while(1) {

        // Background task: calculate the (approximate) mean of the values in the
        // circular buffer and display it, together with the sample number.
        sum = 0;
        for (i = 0; i < BUF_SIZE; i++)
            sum = sum + readCircBuf (&g_inBuffer);
        meanVal = (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;
        // Calculate and display the rounded mean of the buffer contents


        // TODO : WRITE CODE TO CALCULATE PERCENTAGE

        displayMessage(meanVal);

//          Switch Display
            if ((checkButton (UP) == PUSHED)) {
                f_displayMode = (f_displayMode + 1) % 3;
            }
//          Re-zero altimeter
            if ((checkButton (LEFT) == PUSHED)) {
                resetAltimeter();
            }

        SysCtlDelay (SysCtlClockGet() / 150);  // Update display at ~ 50 Hz

    }

}
