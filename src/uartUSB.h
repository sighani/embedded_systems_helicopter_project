/*
 * uartUSB.h
 *
 *  Created on: 12/05/2021
 *      Author: P.J. Bones  UCECE
 *      Modified by: Nathaniel Stainton, Saman Ighani, Jack Walsh
 */

#ifndef SRC_UARTUSB_H_
#define SRC_UARTUSB_H_

#define BAUD_RATE 9600
#define UART_USB_BASE UART0_BASE
#define UART_USB_PERIPH_UART SYSCTL_PERIPH_UART0
#define UART_USB_PERIPH_GPIO SYSCTL_PERIPH_GPIOA
#define UART_USB_GPIO_BASE GPIO_PORTA_BASE
#define UART_USB_GPIO_PIN_RX GPIO_PIN_0
#define UART_USB_GPIO_PIN_TX GPIO_PIN_1
#define UART_USB_GPIO_PINS UART_USB_GPIO_PIN_RX | UART_USB_GPIO_PIN_TX

#include "takeoff.h"

extern uint16_t g_uartCount;

void initialiseUSB_UART(void);

void UARTSend(char *pucBuffer);

void UARTSendHeli(int16_t yaw, int16_t desiredYaw, uint16_t dutyTail, int16_t desiredAlt, int16_t altitude, uint16_t dutyMain, flyingState heliMode);

#endif /* SRC_UARTUSB_H_ */
