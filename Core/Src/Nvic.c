/*
 * Nvic.c
 *
 *  Created on: Jun 12, 2020
 *      Author: academic
 */

void nvicEnableInterrupt(int interruptNum){
	int a = (interruptNum >> 5) -1 ;
	int b = interruptNum - (a << 5) ;

	NVIC->ISER[a] = 1 << b;
}

