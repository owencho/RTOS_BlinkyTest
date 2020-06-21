#ifndef EVENT_H
#define EVENT_H

typedef enum{
    BUTTON_PRESSED_EVENT,TIMEOUT_EVENT
} EventType;
typedef Event * (*EventFnPtr)(void * dataPtr);
typedef struct Event Event;
struct Event {
    EventType type;
    EventFnPtr * functionPtr;
    uint32_t flag;
    void * data;
};
void initEventStruct(Event * event);
void createEvent(Event * event,EventFnPtr * functionPtr,void * data);
void addEventIntoEventQueue(Event * event);
void EventQueue();
#endif // EVENT_H
