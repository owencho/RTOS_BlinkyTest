/*
 * Interrupt.c
 *
 *  Created on: Jun 27, 2020
 *      Author: owen
 */

#include "stm32f4xx_hal.h"
#include "Hardware.h"
#include "Gpio.h"
int readPhysicalButton(){
   return gpioReadBit(gpioA , 0 );
}

void disableIRQ(){
	__disable_irq();
}
void enableIRQ(){
	__enable_irq();
}
