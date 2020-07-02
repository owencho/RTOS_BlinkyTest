#include "unity.h"
#include "ButtonSM.h"
#include "mock_Time.h"
#include "TimerEvent.h"
#include "List.h"
#include "ListItemCompare.h"
#include "mock_Event.h"
#include "mock_Hardware.h"
#include "mock_Exti.h"
#include "mock_Button.h"
#include <stdint.h>
#include "StateMachine.h"
#include "CustomAssert.h"
#include "EventType.h"
#include "Blinky.h"
#include "mock_Led.h"
#include "CustomAssert.h"
#include "EventCompare.h"
TimerEvent timeEv,timeEv2 ,timeEv3,timeEv4;
TimerEvent * outputTimerEvent;
BlinkyStateMachine blinkySM;
extern List timerEventQueueList;
extern int currentTick;
void initTimerEvent(TimerEvent * event, TimerEvent *next ,EventType type,
                    GenericStateMachine * stateMachine,int data,int accumulativeTime){
    event->next = next;
    event->type = type;
    event->stateMachine = stateMachine;
    event->data = data;
    event->accumulativeTime = accumulativeTime;
}
void resetTimerQueueTest(){
    timerEventQueueList.head = NULL;
    timerEventQueueList.tail = NULL;
    timerEventQueueList.count = 0;
    timerEventQueueList.current = NULL;
    timerEventQueueList.previous = NULL;
}

void setTickTest(int time){
    currentTick = time;
}
void test_TimerEventEnqueue_for_first_item(void){
    resetTimerQueueTest();
    //TimerEventEnqueue
    disableIRQ_Expect();
    enableIRQ_Expect();

    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,50,50);
    TimerEventEnqueue(&timeEv);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.head,NULL ,
                                  BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,50,50);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.tail,NULL
                                  ,BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,50,50);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.current,NULL
                                  ,BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,50,50);
    TEST_ASSERT_NULL(timerEventQueueList.previous);
    TEST_ASSERT_EQUAL(1,timerEventQueueList.count);
}

void test_TimerEventEnqueue_for_NULL_input(void){
    resetTimerQueueTest();
    //TimerEventEnqueue
    disableIRQ_Expect();
    enableIRQ_Expect();

    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,50,50);
    TimerEventEnqueue(NULL);
    TEST_ASSERT_NULL(timerEventQueueList.head);
    TEST_ASSERT_NULL(timerEventQueueList.tail);
    TEST_ASSERT_NULL(timerEventQueueList.current);
    TEST_ASSERT_NULL(timerEventQueueList.previous);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.count);
}


void test_timerEventRequest_for_first_item(void){
    resetTimerQueueTest();
    //resetTick
    disableIRQ_Expect();
    enableIRQ_Expect();
    //TimerEventEnqueue
    disableIRQ_Expect();
    enableIRQ_Expect();

    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,0,0);
    timerEventRequest(&timeEv,100);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.head,NULL ,
                                  BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,100,100);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.tail,NULL
                                  ,BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,100,100);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.current,NULL
                                  ,BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,100,100);
    TEST_ASSERT_NULL(timerEventQueueList.previous);
    TEST_ASSERT_EQUAL(1,timerEventQueueList.count);
}


void test_timerEventRequest_for_second_item_set_120ms_after_tick_ady_20ms_back(void){
    setTickTest(20);
    disableIRQ_Expect();
    enableIRQ_Expect();
    initTimerEvent(&timeEv2, NULL ,NO_EVENT,(GenericStateMachine*)&blinkySM,0,0);
    timerEventRequest(&timeEv2,120);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.head,&timeEv2 ,
                                  BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,100,100);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.tail,NULL
                                  ,NO_EVENT,(GenericStateMachine*)&blinkySM,40,140);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.current,&timeEv2
                                  ,BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,100,100);
    TEST_ASSERT_NULL(timerEventQueueList.previous);
    TEST_ASSERT_EQUAL(2,timerEventQueueList.count);

    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv2 ,
                                  BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,100,100);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL ,
                                  NO_EVENT,(GenericStateMachine*)&blinkySM,40,140);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
}

void test_timerEventRequest_for_second_item_set_110ms_after_tick_ady_30ms_front(void){
    //resetTick
    setTickTest(30);
    disableIRQ_Expect();
    enableIRQ_Expect();
    initTimerEvent(&timeEv3, NULL ,BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,0,0);
    timerEventRequest(&timeEv3,50);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.head,&timeEv ,
                                  BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,80,80);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.tail,NULL
                                  ,NO_EVENT,(GenericStateMachine*)&blinkySM,40,140);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.current,&timeEv ,
                                  BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,80,80);
    TEST_ASSERT_NULL(timerEventQueueList.previous);
    TEST_ASSERT_EQUAL(3,timerEventQueueList.count);
    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv ,
                                  BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,80,80);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv2 ,
                                  BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,20,100);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL ,
                                  NO_EVENT,(GenericStateMachine*)&blinkySM,40,140);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
}


void test_timerEventRequest_for_second_item_set_80ms_after_tick_ady_40ms_front(void){
    //resetTick
    setTickTest(40);
    disableIRQ_Expect();
    enableIRQ_Expect();
    initTimerEvent(&timeEv4, NULL ,BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,0,0);
    timerEventRequest(&timeEv4,80);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.head,&timeEv ,
                                  BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,80,80);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.tail,NULL
                                  ,NO_EVENT,(GenericStateMachine*)&blinkySM,20,140);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.current,&timeEv ,
                                  BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,80,80);
    TEST_ASSERT_NULL(timerEventQueueList.previous);
    TEST_ASSERT_EQUAL(4,timerEventQueueList.count);

    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv ,
                                  BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,80,80);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv4 ,
                                  BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,20,100);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv2 ,
                                  BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,20,120);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL ,
                                  NO_EVENT,(GenericStateMachine*)&blinkySM,20,140);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
}


void test_TimerEventDequeue(void){
    //resetTick
    //disableIRQ_Expect();
    //enableIRQ_Expect();

    TimerEventDequeue();
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.head,&timeEv4 ,
                                  BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,20,100);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.tail,NULL
                                  ,NO_EVENT,(GenericStateMachine*)&blinkySM,20,140);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.current,&timeEv4 ,
                                  BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,20,100);
    TEST_ASSERT_NULL(timerEventQueueList.previous);
    TEST_ASSERT_EQUAL(3,timerEventQueueList.count);

    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv4 ,
                                  BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,20,100);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv2 ,
                                  BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,20,120);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL ,
                                  NO_EVENT,(GenericStateMachine*)&blinkySM,20,140);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
}

void test_TimerEventDequeue_event_queue_is_empty(void){
    //resetTick
    resetTimerQueueTest();

    TimerEventDequeue();
    TEST_ASSERT_NULL(timerEventQueueList.head);
    TEST_ASSERT_NULL(timerEventQueueList.tail);
    TEST_ASSERT_NULL(timerEventQueueList.current);
    TEST_ASSERT_NULL(timerEventQueueList.previous);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.count);
}

void test_timerEventISR_event_queue_is_empty(void){
    //resetTick
    resetTimerQueueTest();
    disableIRQ_Expect();
    enableIRQ_Expect();

    timerEventISR();
    TEST_ASSERT_NULL(timerEventQueueList.head);
    TEST_ASSERT_NULL(timerEventQueueList.tail);
    TEST_ASSERT_NULL(timerEventQueueList.current);
    TEST_ASSERT_NULL(timerEventQueueList.previous);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.count);
}

void test_timerEventISR_event_queue_is_one(void){
    //resetTick
    resetTimerQueueTest();
    disableIRQ_Expect();
    enableIRQ_Expect();
    disableIRQ_Expect();
    enableIRQ_Expect();
    disableIRQ_Expect();
    enableIRQ_Expect();

    setTickTest(80);
    initTimerEvent(&timeEv4, NULL ,TIMEOUT_EVENT,(GenericStateMachine*)&blinkySM,0,0);
    timerEventRequest(&timeEv4,80);
    eventEnqueue_Expect((Event*)&timeEv4);
    timerEventISR();


}
