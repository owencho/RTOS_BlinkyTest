#include "ButtonSM.h"
#include "Button.h"
#include"Time.h"
#include"TimerEvent.h"
static Event * buttonEventPtr;
static PressReleaseState expectedButtonState;

void buttonInitStateMachine(ButtonStateMachine * sm){
    sm->callback =(void*) handleButtonStateMachine;
    sm->state = BUTTON_RELEASED;
    sm->buttonStatus = RELEASE;
    sm->buttonEvent.stateMachine =(GenericStateMachine *) sm;
    sm->timerEvent.stateMachine = (GenericStateMachine *)sm;
    rawButtonEventRequest(&sm->buttonEvent,PRESS);
    //eventRequest for PRESS button
}

PressReleaseState getButtonState(ButtonStateMachine * sm){
    return sm->buttonStatus;
}
//pure event base void handleBlinkyStateMachine(BlinkyStateMachine * sm,Event *event)
void handleButtonStateMachine(Event *event){
    ButtonStateMachine * sm = (ButtonStateMachine*) event->stateMachine;
    switch(sm->state){
        case BUTTON_RELEASED :
            if(event->type == BUTTON_PRESSED_EVENT){
            	if(buttonEventPtr !=NULL && expectedButtonState ==PRESS){
					eventEnqueue(buttonEventPtr);
					//buttonEventPtr = NULL;
					sm->state=BUTTON_PRESSED_DEBOUNCING;
					timerEventStart(&sm->timerEvent,100);
            	}
;
            }
        break;
      	case BUTTON_PRESSED_DEBOUNCING:
            if(event->type == TIMEOUT_EVENT){
            	buttonEventPtr->type = BUTTON_PRESSED_EVENT;
            	eventEnqueue(buttonEventPtr);
            	buttonEventPtr = NULL;
                sm->state = BUTTON_PRESSED;
            }

        break;
        case BUTTON_PRESSED:
        	if(event->type == BUTTON_PRESSED_EVENT){
				if(buttonEventPtr !=NULL && expectedButtonState ==RELEASE){
					//buttonEventPtr->type = BUTTON_RELEASED_EVENT;
					eventEnqueue(buttonEventPtr);
					//buttonEventPtr = NULL;
					sm->state=BUTTON_RELEASED_DEBOUNCING;
					timerEventStart(&sm->timerEvent,100);
				}
        	}
        break;
        case BUTTON_RELEASED_DEBOUNCING:
            if(event->type == TIMEOUT_EVENT){
            	buttonEventPtr->type=BUTTON_RELEASED_EVENT;
            	eventEnqueue(buttonEventPtr);
            	buttonEventPtr = NULL;
                sm->state = BUTTON_RELEASED;
            }
        break;
    }
}

void buttonEventRequest(Event * event ,PressReleaseState state){
  	buttonEventPtr = event;
  	expectedButtonState = state;
  	rawButtonEventRequest(event ,state);
}
