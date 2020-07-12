#ifndef EVENT_H
#define EVENT_H
#include <stdint.h>
#include <stddef.h>
#include "StateMachine.h"
#include "EventType.h"

typedef struct Event Event;

struct Event {
  	Event * next;
    void * data;
  	EventType type;
  	GenericStateMachine * stateMachine;
};

#endif // EVENT_H
