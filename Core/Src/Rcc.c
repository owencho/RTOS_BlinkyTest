/*
 * Rcc.c
 *
 *  Created on: Jun 11, 2020
 *      Author: owen
 */
#include "Rcc.h"
#include "BaseAddress.h"
#include "Common.h"
Rcc * rcc = rccReg;

void enableGpioG(){
	rcc->AHB1RSTR &= ~(1 << 6);
	rcc->AHB1ENR |= 1 << 6;
}

void disableGpioG(){
	rcc-> AHB1RSTR |= 1 << 6;
	rcc-> AHB1ENR &= ~(1 << 6);
}

void enableGpioA(){
	rcc->AHB1RSTR &= ~(1 << 0);
	rcc->AHB1ENR |= 1 << 0;
}

void disableGpioA(){
	rcc-> AHB1RSTR |= 1 << 0;
	rcc-> AHB1ENR &= ~(1 << 0);
}
