#ifndef EVENT_H
#define EVENT_H
#include <stdint.h>
#include <stddef.h>
#include "StateMachine.h"
typedef enum{
    BUTTON_RELEASED_EVENT,BUTTON_PRESSED_EVENT,TIMEOUT_EVENT,NO_EVENT
} EventType;

typedef struct Event Event;

struct Event {
    EventType type;
    GenericStateMachine * stateMachine;
    void * data;
};

Event * createEventStruct();
void initEventQueue();
Event * initEventStruct();
void addEventIntoEventQueue(Event * event);
//void initEvent(Event * event,EventType type,StateMachine * sm,void * data);
void eventEnqueue(Event * event);
int  eventDequeue(Event ** event);
#endif // EVENT_H
