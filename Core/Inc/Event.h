#ifndef EVENT_H
#define EVENT_H
#include <stdint.h>
#include <stddef.h>
#include "StateMachine.h"
#include "ListItem.h"

typedef struct Event Event;

struct Event {
  	ListItem * next;
  	EventType type;
  	GenericStateMachine * stateMachine;
    void * data;
};

//void initEvent(Event * event,EventType type,StateMachine * sm,void * data);
void eventEnqueue(Event * event);
int  eventDequeue(Event ** event);
#endif // EVENT_H
