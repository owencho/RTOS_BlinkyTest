#ifndef BLINKY_H
#define BLINKY_H

#include <stdint.h>
#include "Event.h"
#include "ButtonSM.h"
#include "StateMachine.h"
#include "TimerEvent.h"
typedef enum{
    BLINKY_START,
    LED_OFF_BUTTON_RELEASED,
    LED_ON,
    BLINK_ON,
    BLINK_OFF,
    LED_OFF_BUTTON_HOLD,
} BlinkyState;

typedef struct BlinkyStateMachine BlinkyStateMachine;
struct BlinkyStateMachine {
  	Callback callback;
  	BlinkyState state;
  	//uint32_t currentTick;
  	uint32_t isButtonReleased;
    ButtonStateMachine * button;
    TimerEvent timerEvent;
    Event blinkyEvent;
};

void blinkyInitStateMachine(BlinkyStateMachine * sm,ButtonStateMachine * button);
void blinkyStartStateMachine(BlinkyStateMachine * sm);
void handleBlinkyStateMachine(Event *event);
#endif // BLINKY_H
