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
	IoRegister CR;
	IoRegister PLLCFGR;
	IoRegister CFGR;
	IoRegister CIR;
	IoRegister AHB1RSTR;
	IoRegister AHB2RSTR;
	IoRegister AHB3RSTR;
	IoRegister RESERVE0;
	IoRegister APB1RSTR;
	IoRegister APB2RSTR;
	IoRegister RESERVE1;
	IoRegister RESERVE2;
	IoRegister AHB1ENR;
	IoRegister AHB2ENR;
	IoRegister AHB3ENR;
	IoRegister RESERVE3;

};

#define rccReg (Rcc*)(RCC_BASEADDRESS)
void enableGpioG();
void disableGpioG();
void enableGpioA();
void disableGpioA();
#endif /* INC_RCC_H_ */
