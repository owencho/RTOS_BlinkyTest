#include "unity.h"
#include "mock_Interrupt.h"
#include "mock_Event.h"
#include "mock_List.h"
#include "TimerEvent.h"
#include "StateMachine.h"
#include <stdint.h>
#include <stddef.h>

Event  evt;
void setUp(void){}

void tearDown(void){}

void initEvent(Event * event,EventType type,GenericStateMachine * stateMachine,
                void * data){
    event->type = type;
    event->stateMachine = stateMachine;
    event->data = data;

}

void test_initEventTimerQueue(void){
    createList_Expect();
    initEventTimerQueue();
}
void test_TimerEvent_timerEventStart(void){
     initEventTimerQueue();
     initEvent(&evt,NO_EVENT,NULL,NULL);
     //expect function to execute
     //expect on reset tick
     disableIRQ_Expect();
     enableIRQ_Expect();

     disableIRQ_Expect();
     listAddItemToTail_Expect(eventTimerQueueList,&evt);
     enableIRQ_Expect();
     //test function
     timerEventStart (&evt,200);
}
