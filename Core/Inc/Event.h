#ifndef EVENT_H
#define EVENT_H
#include <stdint.h>
#include <stddef.h>
typedef enum{
    BUTTON_RELEASED_EVENT,BUTTON_PRESSED_EVENT,TIMEOUT_EVENT,NO_EVENT
} EventType;

typedef struct Event Event;
typedef Event * (*EventFnPtr)(void * dataPtr,Event * event);
struct Event {
    EventType type;
    EventFnPtr * functionPtr;
    uint32_t flag;
    void * data;
};

Event * initEventStruct();
Event * clearEventStruct(Event * event);
void initEventQueue();
void addEventIntoEventQueue(Event * event);
Event * createEvent(Event * event,EventType type,EventFnPtr * functionPtr,void * data);
void addEventIntoEventQueue(Event * event);
int  deEventQueue(Event ** event);
#endif // EVENT_H
