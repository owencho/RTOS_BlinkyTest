#ifndef EVENT_H
#define EVENT_H
#include <stdint.h>
typedef enum{
    BUTTON_PRESSED_EVENT,TIMEOUT_EVENT
} EventType;

typedef struct Event Event;
typedef Event * (*EventFnPtr)(void * dataPtr);
struct Event {
    EventType type;
    EventFnPtr * functionPtr;
    uint32_t flag;
    void * data;
};

void initEventStruct(Event * event);
void createEvent(Event * event,EventFnPtr * functionPtr,void * data);
void addEventIntoEventQueue(Event * event);
#endif // EVENT_H
