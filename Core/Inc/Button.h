#ifndef BUTTON_H
#define BUTTON_H

#include "Event.h"
typedef enum{
    RELEASE,PRESS
} PressReleaseState;

int readPhysicalButton();
void rawButtonEventRequest(Event * event,PressReleaseState state);
void buttonEventISR();
#endif // BUTTON_H
