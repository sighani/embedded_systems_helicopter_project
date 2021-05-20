/*
 * display.h
 *
 *  Created on: 22/04/2021
 *      Author: Nathaniel Stainton, Saman Ighani, Jack Walsh
 * 
 *  Writes inputs to OLED display on BoosterPack.
 */

#ifndef SRC_DISPLAY_H_
#define SRC_DISPLAY_H_

void updateFlightData(int16_t heliAlt, int16_t yaw, uint16_t dutyTail, uint16_t dutyMain, int16_t altRef, int16_t tailRef);

#endif /* SRC_DISPLAY_H_ */
