#ifndef BUTTONSM_H
#define BUTTONSM_H
#include "Event.h"
#include <stdint.h>
typedef enum{
    RELEASE,PRESS
} PressReleaseState;

typedef enum{
	 BUTTON_RELEASED,
   BUTTON_PRESSED_DEBOUNCING,
   BUTTON_PRESSED,
   BUTTON_RELEASED_DEBOUNCING,
} ButtonState;

typedef struct ButtonStateMachine ButtonStateMachine;
struct ButtonStateMachine {
    ButtonState state;
    uint32_t currentTick;
    PressReleaseState buttonStatus;
};

PressReleaseState getButtonState(ButtonStateMachine *sm);
void buttonInitStateMachine(ButtonStateMachine * sm);
Event * handleButtonStateMachine(ButtonStateMachine * sm,Event * event);

#endif // BUTTONSM_H
