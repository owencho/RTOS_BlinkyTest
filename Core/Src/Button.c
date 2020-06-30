#include "Button.h"
#include "Gpio.h"
#include "Event.h"
#include "Exti.h"
static Event * buttonEventPtr = NULL;
static EventType expectedButtonState;

int readPhysicalButton(){
   return gpioReadBit(gpioA , 0 );
}

EventType convertEventTypeintoButtonState()
//if else statement

void rawButtonEventRequest(Event * event , EventType state){
    //disable global IRQ
	expectedButtonState = state;
    if(readPhysicalButton() == convertEventTypeIntoButtonstate(state)){
      	buttonEventPtr->type = state;
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
      	if(readPhysicalButton() == convertEventTypeIntoButtonstate(expectedButtonState)){
      		buttonEventPtr->type = expectedButtonState;
      		eventEnqueue(buttonEventPtr);
      	}
        buttonEventPtr = NULL;
    }
}
