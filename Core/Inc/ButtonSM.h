#ifndef BUTTONSM_H
#define BUTTONSM_H

#include "Event.h"
#include "Button.h"
#include <stdint.h>
#include "StateMachine.h"

typedef enum{
    BUTTON_RELEASED,
    BUTTON_PRESSED_DEBOUNCING,
    BUTTON_PRESSED,
    BUTTON_RELEASED_DEBOUNCING,
} ButtonState;

typedef struct ButtonStateMachine ButtonStateMachine;
struct ButtonStateMachine {
    Callback callback;
    ButtonState state;
    PressReleaseState buttonStatus;
    Event timerEvent;
    Event buttonEvent;
};

PressReleaseState getButtonState(ButtonStateMachine *sm);
void buttonInitStateMachine(ButtonStateMachine * sm);
void handleButtonStateMachine(Event * event);
void buttonEventRequest(Event * event ,PressReleaseState state);
//button event goes isr
//timer event isr
#endif // BUTTONSM_H
