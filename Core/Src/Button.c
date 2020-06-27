#include "Button.h"
#include "Gpio.h"
#include "Event.h"
#include "Exti.h"
static Event * buttonEventPtr = NULL;
static PressReleaseState expectedButtonState;

int readPhysicalButton(){
   return gpioReadBit(gpioA , 0 );
}

void rawButtonEventRequest(Event * event , PressReleaseState state){
    //disable global IRQ
	expectedButtonState = state;
    if(readPhysicalButton() == state){
      	if(state == PRESS)
      		buttonEventPtr->type = BUTTON_PRESSED_EVENT;
      	else
      		buttonEventPtr->type = BUTTON_RELEASED_EVENT;

      	eventEnqueue(event);
      	return;
    }
    buttonEventPtr = event;
    extiSetInterruptMaskRegister(exti,PIN_0,NOT_MASKED);
}
/*readPhysicalButton()
if it is same as the state
 then enqueue `evt` in the eventqueue
 if not the same . otherwise ,store the evt in buttonEvent
*/

void buttonEventISR(){
    if(buttonEventPtr != NULL){
      	if(readPhysicalButton() == PRESS)
      		  buttonEventPtr->type = BUTTON_PRESSED_EVENT;
      	else
      		  buttonEventPtr->type = BUTTON_RELEASED_EVENT;
      	eventEnqueue(buttonEventPtr);
        //buttonEventPtr = NULL;
    }
}
