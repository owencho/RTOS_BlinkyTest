/*
 * Rcc.h
 *
 *  Created on: Jun 11, 2020
 *      Author: Owen
 */

#ifndef INC_RCC_H_
#define INC_RCC_H_

#include <stdint.h>
#include "BaseAddress.h"
#include "Common.h"


typedef struct Rcc Rcc;
struct Rcc {
	_IO_ CR;
	_IO_ PLLCFGR;
	_IO_ CFGR;
	_IO_ CIR;
	_IO_ AHB1RSTR;
	_IO_ AHB2RSTR;
	_IO_ AHB3RSTR;
	_IO_ RESERVE0;
	_IO_ APB1RSTR;
	_IO_ APB2RSTR;
	_IO_ RESERVE1;
	_IO_ RESERVE2;
	_IO_ AHB1ENR;
	_IO_ AHB2ENR;
	_IO_ AHB3ENR;
	_IO_ RESERVE3;

};

#define rccReg (Rcc*)(RCC_V)
void enableGpioG();
void disableGpioG();
#endif /* INC_RCC_H_ */
