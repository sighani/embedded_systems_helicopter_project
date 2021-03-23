#include <stdint.h>
#include <stdbool.h>
#include "labcode/circBufT.h"
#include "driverlib/interrupt.h"
#include "setup.h"
#include "driverlib/sysctl.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"
#include "labcode/buttons4.h"


void displayPercentage(uint32_t heliPercentage) {
    char string[17];  // 16 characters across the display

    OLEDStringDraw ("Altimeter", 0, 0);

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
//    clear just in case
    usnprintf (string, sizeof(string), "%% of max  %3d%%", heliPercentage);
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

void displayMessage(uint16_t meanVal, uint32_t f_displayMode, uint32_t heliPercentage) {
    if (f_displayMode == 0) {
        displayPercentage(heliPercentage);
    } else  if (f_displayMode == 1) {
        displayMeanVal(meanVal);
    } else if (f_displayMode == 2 ) {
        displayBlank();
    } else {
        //THIS 'SHOULD' NEVER HAPPEN
    }
}

static int32_t heliAltMax;
static int32_t heliAltMin;
static int32_t heliAltCurrent;
static int32_t heliAltPercentage;
static int32_t range = (4095 * 8)/30;


void resetAltimeter(uint32_t meanVal) {
    heliAltPercentage = 0;
    heliAltMin = meanVal;
}

int main(void) {
    uint16_t i;
    int32_t sum;
    int32_t meanVal;
    uint32_t f_displayMode = 0;
    uint8_t initflag = 1;



//  Initialisations
    initButtons();

    initClock();

    initADC();
    initCircBuf (&g_inBuffer, BUF_SIZE);

    initDisplay();

//    Enable ISPs

    IntMasterEnable();



    while(1) {

        // Background task: calculate the (approximate) mean of the values in the
        // circular buffer and display it, together with the sample number.
        sum = 0;
        for (i = 0; i < BUF_SIZE; i++) {
            sum = sum + readCircBuf (&g_inBuffer);
        }
        meanVal = (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;

        if (initflag && meanVal > 0) {
            initflag = 0;
            heliAltPercentage = 0;
            heliAltMin = meanVal;
            heliAltMax = meanVal - range;
        }


        heliAltPercentage = ((heliAltMin - meanVal) * 100 ) / range;


        if (heliAltPercentage < 0) {
            heliAltPercentage = 0;
        }
        else if (heliAltPercentage > 100) {
            heliAltPercentage = 100;
        }


        displayMessage(meanVal, f_displayMode, heliAltPercentage);

//          Switch Display
            if ((checkButton (UP) == PUSHED)) {
                f_displayMode = (f_displayMode + 1) % 3;
            }
//          Re-zero altimeter
            if ((checkButton (LEFT) == PUSHED)) {
                resetAltimeter(meanVal);
            }

        SysCtlDelay (SysCtlClockGet() / 12);  // Update display at ~ 50 Hz

    }

}
