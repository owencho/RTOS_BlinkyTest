#include "Blinky.h"
#include <malloc.h>
#include "Led.h"
#include "Button.h"
#include "Time.h"

//#define getButtonState(sm) (sm->button->buttonStatus)

void blinkyInitStateMachine(BlinkyStateMachine * sm,ButtonStateMachine * button){
    sm->state = LED_OFF_BUTTON_RELEASED;
    sm->currentTick = 0;
    sm->isButtonReleased = 0;
    sm->button = button;
}

BlinkyStateMachine * blinkyCreateStateMachine(ButtonStateMachine * button){
    BlinkyStateMachine * sm = malloc(sizeof(BlinkyStateMachine));
    blinkyInitStateMachine(sm,button);
    return sm;
}

//pure event base void handleBlinkyStateMachine(BlinkyStateMachine * sm,Event *event)

Event * handleBlinkyStateMachine(BlinkyStateMachine * sm,Event *event){
    switch(sm->state){
          case BLINKY_START :
              turnLed(OFF);
              sm->state=LED_OFF_BUTTON_RELEASED;
              break;
        	case LED_OFF_BUTTON_RELEASED:
              if(event->type == BUTTON_PRESSED_EVENT){
                  turnLed(ON);
                  sm->state=LED_ON;
                  sm->isButtonReleased = 0;
              }
              break;
          case LED_ON:
              if(event->type == BUTTON_RELEASED_EVENT)
                  sm->isButtonReleased = 1;
              if(sm->isButtonReleased==1 && event->type == BUTTON_PRESSED_EVENT){
                  sm->currentTick = getCurrentTime();
                  sm->state=BLINK_ON;
                  sm->isButtonReleased = 0;
              }
              break;
          case BLINK_ON:
              if(event->type == BUTTON_RELEASED_EVENT)
                  sm->isButtonReleased = 1;

              if(sm->isButtonReleased==1 && event->type == BUTTON_PRESSED_EVENT ){
                  sm->state=LED_OFF_BUTTON_HOLD;
                  turnLed(OFF);
              }
              else if(getCurrentTime()- sm->currentTick >= 100){
                  sm->currentTick = getCurrentTime();
                  sm->state=BLINK_OFF;
                  turnLed(OFF);
              }
              break;
          case BLINK_OFF:
              if(event->type == BUTTON_RELEASED_EVENT)
                  sm->isButtonReleased = 1;

              if(sm->isButtonReleased==1 && event->type == BUTTON_PRESSED_EVENT )
                  sm->state=LED_OFF_BUTTON_HOLD;
              else if(getCurrentTime()- sm->currentTick >= 100){
                  sm->currentTick = getCurrentTime();
                  sm->state=BLINK_ON;
                  turnLed(ON);
              }
              break;
          case LED_OFF_BUTTON_HOLD:
              if(event->type == BUTTON_RELEASED_EVENT)
                  sm->state=LED_OFF_BUTTON_RELEASED;
              break;
    }
    return event;
}

// pull
/*
void handleBlinkyStateMachine(BlinkyStateMachine * sm){
    switch(sm->state){
          case BLINKY_START :
              turnLed(OFF);
              sm->state=LED_OFF_BUTTON_RELEASED;
              break;
        	case LED_OFF_BUTTON_RELEASED:
              if(getButtonState(sm->button) == PRESS){
                  turnLed(ON);
                  sm->state=LED_ON;
                  sm->isButtonReleased = 0;
              }
              break;
          case LED_ON:
              if(getButtonState(sm->button) == RELEASE)
                  sm->isButtonReleased = 1;
              if(sm->isButtonReleased==1 && getButtonState(sm->button) == PRESS ){
                  sm->currentTick = getCurrentTime();
                  sm->state=BLINK_ON;
                  sm->isButtonReleased = 0;
              }
              break;
          case BLINK_ON:
              if(getButtonState(sm->button) == RELEASE)
                  sm->isButtonReleased = 1;

              if(sm->isButtonReleased==1 && getButtonState(sm->button) == PRESS ){
                  sm->state=LED_OFF_BUTTON_HOLD;
                  turnLed(OFF);
              }
              else if(getCurrentTime()- sm->currentTick >= 100){
                  sm->currentTick = getCurrentTime();
                  sm->state=BLINK_OFF;
                  turnLed(OFF);
              }
              break;
          case BLINK_OFF:
              if(getCurrentTime() == RELEASE)
                  sm->isButtonReleased = 1;

              if(sm->isButtonReleased==1 && getButtonState(sm->button) == PRESS )
                  sm->state=LED_OFF_BUTTON_HOLD;
              else if(getCurrentTime()- sm->currentTick >= 100){
                  sm->currentTick = getCurrentTime();
                  sm->state=BLINK_ON;
                  turnLed(ON);
              }
              break;
          case LED_OFF_BUTTON_HOLD:
              if(getButtonState(sm->button) == RELEASE)
                  sm->state=LED_OFF_BUTTON_RELEASED;
              break;
    }
}
*/
