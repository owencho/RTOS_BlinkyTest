/*
 * Gpio.c
 *
 *  Created on: Jun 10, 2020
 *      Author: owen
 */


#include "Gpio.h"
#include "Common.h"

void gpioSetPinSpeed(GpioRegs *gpio , int pin , PinSpeed speed){
	gpio->OSPEEDR &= ~(3 << (pin*2 ));
	gpio->OSPEEDR |= speed << (pin *2);
}

void gpioSetMode(GpioRegs *gpio , int pin , PinMode mode){
	gpio->MODER &= ~(3 << (pin*2 ));
	gpio->MODER |= mode << (pin *2);
}

void gpioSetOutputType(GpioRegs *gpio , int pin ,PinOutputType type){
	gpio->OTYPER &= ~(1 << pin);
	gpio->OTYPER |= type << pin;
}


void gpioWrite(GpioRegs *gpio , int value){
	gpio->ODR = value;
}

void gpioWriteBit(GpioRegs *gpio , int pin , int value){
	gpio->BSRR = 1<< (pin + ((~value & 1) <<4 ));
}


int gpioReadBit(GpioRegs *gpio , int pin ){
	return (gpio->IDR >> pin )& 0x1;
}

void gpioToggleBit(GpioRegs *gpio , int pin ){
	gpio->ODR ^= (1 << pin);
}

