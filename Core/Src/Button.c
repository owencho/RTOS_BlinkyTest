#include "Button.h"
#include "Event.h"
#include "EventType.h"
#include "Exti.h"
#include "Hardware.h"
static Event * buttonEventPtr = NULL;
static EventType expectedButtonState;


int convertEventTypeIntoButtonstate(EventType state){
    if(state== BUTTON_RELEASED_EVENT)
        return 0;
    else if(state== BUTTON_PRESSED_EVENT)
        return 1;
    else
        return 1;
}

void rawButtonEventRequest(Event * event , EventType state){
    //disable global IRQ
    extiSetInterruptMaskRegister(exti,PIN_0,MASKED);
  	expectedButtonState = state;
    if(readPhysicalButton() == convertEventTypeIntoButtonstate(state)){
    	event->type = state;
      	eventEnqueue(event);
        extiSetInterruptMaskRegister(exti,PIN_0,NOT_MASKED);
      	return;
    }
    buttonEventPtr = event;
    extiSetInterruptMaskRegister(exti,PIN_0,NOT_MASKED);
}

void buttonEventISR(){
    if(buttonEventPtr != NULL){
      	if(readPhysicalButton() == convertEventTypeIntoButtonstate(expectedButtonState)){
        		buttonEventPtr->type = expectedButtonState;
        		eventEnqueue(buttonEventPtr);
        		buttonEventPtr = NULL;
      	}
    }
}
