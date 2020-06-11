/*
 * Gpio.c
 *
 *  Created on: Jun 10, 2020
 *      Author: owen
 */


#include "Gpio.h"

void gpioSetPinSpeed(Gpio *gpio , int pin , PinSpeed){
	gpio->OSPEEDR &= ~(3 << (pin*2 ));
	gpio->OSPEEDR
}
