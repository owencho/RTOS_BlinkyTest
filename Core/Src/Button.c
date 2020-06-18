#include "Button.h"
#include "Gpio.h"
int readPhysicalButton(){
   return gpioReadBit(gpioA , 0 );
}
