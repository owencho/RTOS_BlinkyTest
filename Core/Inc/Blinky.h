#ifndef BLINKY_H
#define BLINKY_H

#include <stdint.h>
#include "ButtonSM.h"

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
	BlinkyState state;
	uint32_t currentTick;
  uint32_t isButtonReleased;
  ButtonStateMachine * button;
};

void blinkyInitStateMachine(BlinkyStateMachine * sm,ButtonStateMachine * button);
BlinkyStateMachine * blinkyCreateStateMachine(ButtonStateMachine * button);
Event * handleBlinkyStateMachine(BlinkyStateMachine * sm,Event *event);
#endif // BLINKY_H
