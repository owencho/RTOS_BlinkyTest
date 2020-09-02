/*
 * Hardware.c
 *
 *  Created on: Jun 27, 2020
 *      Author: owen
 */

#include "Hardware.h"
#include "Gpio.h"


int readPhysicalButton(){
	return gpioReadBit(gpioA , 0 );
}
