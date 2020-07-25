#ifndef BUTTON_H
#define BUTTON_H

#include "Event.h"

typedef enum{
    RELEASE,PRESS
} PressReleaseState;


void rawButtonEventRequest(Event * event , EventType state);
void buttonEventISR();
int convertEventTypeIntoButtonstate(EventType state);
#endif // BUTTON_H
