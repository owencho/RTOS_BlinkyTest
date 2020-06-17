/*
 * Gpio.h
 *
 *  Created on: Jun 10, 2020
 *      Author: owen
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include <stdint.h>
#include "BaseAddress.h"
#include "Common.h"

typedef enum{
	LOW_SPEED,MEDIUM_SPEED,HIGH_SPEED,VERY_HI_SPEED,
} PinSpeed;

typedef enum{
	OPEN_DRAIN,PUSH_PULL,
} PinOutputType;

typedef enum{
	GPIO_IN,GPIO_OUT,GPIO_ALT,GPIO_ANALOG,
} PinMode;

typedef enum{
	NONE,PULL_UP,PULL_DOWN,
} PinPullUpDown;


typedef struct GpioRegs GpioRegs;
struct GpioRegs {
	IoRegister MODER;
	IoRegister OTYPER;
	IoRegister OSPEEDR;
	IoRegister PUPDR;
	IoRegister IDR;
	IoRegister ODR;
	IoRegister BSRR;
	IoRegister LCKR;
	IoRegister AFRL;
	IoRegister AFRH;
};

#define gpioG (GpioRegs*)(GPIO_G)
#define gpioA (GpioRegs*)(GPIO_A)
void gpioSetMode(GpioRegs *gpio , int pin , PinMode mode);
void gpioSetOutputType(GpioRegs *gpio , int pin ,PinOutputType type);
void gpioSetPinSpeed(GpioRegs *gpio , int pin , PinSpeed speed);
void gpioWriteBit(GpioRegs *gpio , int pin , int value);
void gpioWrite(GpioRegs *gpio , int value);
int gpioReadBit(GpioRegs *gpio , int pin );
void gpioToggleBit(GpioRegs *gpio , int pin );

#endif /* INC_GPIO_H_ */
