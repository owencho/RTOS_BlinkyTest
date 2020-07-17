#include "BlinkyButtonIntegration.h"
#include "Event.h"
#include "EventQueue.h"
#include "TimerEvent.h"
#include "TimerEventQueue.h"
#include "Led.h"


void fake_turnLed(OnOffState state, int callNumber){
    uintptr_t * args = (uintptr_t)malloc(sizeof(uintptr_t));
    args[0] = state;
    blinkyButtonSequenceStateMachine((uintptr_t)fake_turnLed,(uintptr_t)args);
}

void fake_rawButtonEventRequest(Event * event , EventType state, int callNumber){
    uintptr_t * args = (uintptr_t)malloc(sizeof(uintptr_t * 2));
    args[0] = event;
    args[1] = state;
    blinkyButtonSequenceStateMachine((uintptr_t)fake_rawButtonEventRequest,(uintptr_t)args);
}

void fake_eventEnqueue(EventQueue * queue,Event * event, int callNumber){
    uintptr_t * args = (uintptr_t)malloc(sizeof(uintptr_t * 2));
    args[0] = queue;
    args[1] = event;
    blinkyButtonSequenceStateMachine((uintptr_t)fake_eventEnqueue,(uintptr_t)args);
}

void fake_timerEventRequest(TimerEventQueue * timerEventQueue,TimerEvent * event,int expiryPeriod){
    uintptr_t * args = (uintptr_t)malloc(sizeof(uintptr_t * 3));
    args[0] = timerEventQueue;
    args[1] = event;
    args[2] = expiryPeriod;
    blinkyButtonSequenceStateMachine((uintptr_t)fake_timerEventRequest,(uintptr_t)args);
}
