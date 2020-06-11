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

typedef struct Gpio Gpio;
struct Gpio {
	_IO_ MODER;
	_IO_ OTYPER;
	_IO_ OSPEEDR;
	_IO_ PUPDR;
	_IO_ IDR;
	_IO_ ODR;
	_IO_ BSRR;
	_IO_ LCKR;
	_IO_ AFRL;
	_IO_ AFRH;
};

#define gpioG (Gpio*)(GPIO_G)

void gpioSetMode(Gpio *gpio , int pin , PinOutputType type);
void gpioSetPinSpeed(Gpio *gpio , int pin , PinSpeed speed);
void gpioWrite(Gpio *gpio , int pin , int value);
#endif /* INC_GPIO_H_ */
