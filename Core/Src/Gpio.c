/*
 * Gpio.c
 *
 *  Created on: Jun 10, 2020
 *      Author: owen
 */


#include "Gpio.h"
#include "Common.h"

void gpioSetPinSpeed(Gpio *gpio , int pin , PinSpeed speed){
	gpio->OSPEEDR &= ~(3 << (pin*2 ));
	gpio->OSPEEDR |= speed << (pin *2);
}

void gpioSetMode(Gpio *gpio , int pin , PinOutputType type){
	gpio->MODER &= ~(3 << (pin*2 ));
	gpio->MODER |= 1 << (pin *2);
	gpio->OTYPER &= ~(1 << pin);
	gpio->OTYPER |= type << pin;
}

void gpioWrite(Gpio *gpio , int pin , int value){
	gpio->ODR &= ~(value << pin);
	gpio->ODR |= value << pin;
}

