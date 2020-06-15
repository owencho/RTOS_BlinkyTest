/*
 * Nvic.h
 *
 *  Created on: Jun 14, 2020
 *      Author: owen
 */

#ifndef INC_NVIC_H_
#define INC_NVIC_H_

#include <stdint.h>
#include "BaseAddress.h"
#include "Common.h"


typedef struct NvicRegs NvicRegs;
struct NvicRegs {
	IoRegister ISER[8];
	IoRegister Reserved0[3];
	IoRegister ICER[8];
	IoRegister Reserved1[3];
	IoRegister ISPR[8];
	IoRegister Reserved2[3];
	IoRegister ICPR[8];
	IoRegister Reserved3[3];
	IoRegister IABR[8];
	IoRegister Reserved4[7];
	IoRegister IPR[60];
	IoRegister STIR;

};

#define nvic (NvicRegs*)(NVIC_BASEADDRESS)

#endif /* INC_NVIC_H_ */
