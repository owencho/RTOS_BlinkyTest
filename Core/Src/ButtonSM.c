#include "ButtonSM.h"
#include "Button.h"
#include"Time.h"

void buttonInitStateMachine(ButtonStateMachine * sm){
    sm->state = BUTTON_RELEASED;
    sm->currentTick = 0;
    sm->currentTick = RELEASE;
}

PressReleaseState getButtonState(ButtonStateMachine * sm){
    return sm->buttonStatus;
}
//pure event base void handleBlinkyStateMachine(BlinkyStateMachine * sm,Event *event)
// pull
Event * handleButtonStateMachine(ButtonStateMachine * sm,Event *event){
    switch(sm->state){
          case BUTTON_RELEASED :
              if(event->type == BUTTON_PRESSED_EVENT){
                  sm->state=BUTTON_PRESSED_DEBOUNCING;
                  sm->buttonStatus = PRESS;
                  sm->currentTick = getCurrentTime();
              }
          break;
        	case BUTTON_PRESSED_DEBOUNCING:
              if(getCurrentTime()- sm->currentTick >= 100)
                  sm->state = BUTTON_PRESSED;
          break;
          case BUTTON_PRESSED:
              if(event->type == BUTTON_RELEASED_EVENT){
                  sm->state=BUTTON_RELEASED_DEBOUNCING;
                  sm->buttonStatus = RELEASE;
                  sm->currentTick = getCurrentTime();
              }
          break;
          case BUTTON_RELEASED_DEBOUNCING:
              if(getCurrentTime()- sm->currentTick >= 100)
                  sm->state = BUTTON_RELEASED;
          break;
    }
    event->flag =1;
    return event;
}
