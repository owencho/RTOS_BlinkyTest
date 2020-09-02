#include "Led.h"
#include "Gpio.h"
void turnLed(OnOffState state){
	if(state==ON)
		gpioWriteBit(gpioG , 13 , 1);
	else
	    gpioWriteBit(gpioG , 13 , 0);
}
