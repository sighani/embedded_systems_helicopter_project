/*
 * display.h
 *
 *  Created on: 22/04/2021
 *      Author: Nat
 */

#ifndef SRC_DISPLAY_H_
#define SRC_DISPLAY_H_

void displayPercentage(uint32_t heliPercentage);
void displayYaw(uint32_t yaw);
void displayMeanVal(uint16_t meanVal);
void displayBlank(void);
void displayMessage(uint16_t meanVal, uint32_t displayMode, uint32_t heliPercentage, int32_t yaw);

//Display state enum
typedef enum {ALTIPER, ALTIMEAN, YAWDEG, BLANK} displayMode_e;

extern displayMode_e displayMode;


#endif /* SRC_DISPLAY_H_ */
