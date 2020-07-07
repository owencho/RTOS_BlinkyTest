#include "unity.h"
#include "ButtonSM.h"
#include "mock_Time.h"
#include "TimerEvent.h"
#include "List.h"
#include "ListItemCompare.h"
#include "Event.h"
#include "mock_Hardware.h"
#include "mock_Irq.h"
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
#include "FakeIRQ.h"
TimerEvent timeEv,timeEv2 ,timeEv3,timeEv4;
TimerEvent * outputTimerEvent;
List testQueueList;
extern List timerEventQueueList;
extern List eventQueueList;
extern int relativeTick;

void setTickTest(int time){
    relativeTick = time;
}

void initTimerEvent(TimerEvent * event, TimerEvent *next ,EventType type,
                    GenericStateMachine * stateMachine,int time){
    event->next = next;
    event->type = type;
    event->stateMachine = stateMachine;
    event->time = time;
}
void resetTimerQueueTest(){
    timerEventQueueList.head = NULL;
    timerEventQueueList.tail = NULL;
    timerEventQueueList.count = 0;
    timerEventQueueList.current = NULL;
    timerEventQueueList.previous = NULL;
    setTickTest(0);
}

void resetEventQueueTest(){
    eventQueueList.head = NULL;
    eventQueueList.tail = NULL;
    eventQueueList.count = 0;
    eventQueueList.current = NULL;
    eventQueueList.previous = NULL;
}

void initTimerEventQueueWithData(int timeDelay1 , int timeDelay2,int timeDelay3,
                                 int timeDelay4){
    resetTimerQueueTest();
    initTimerEvent(&timeEv,NULL,NO_EVENT,NULL,0);
    initTimerEvent(&timeEv2,NULL,BUTTON_PRESSED_EVENT,NULL,0);
    initTimerEvent(&timeEv3,NULL,BUTTON_RELEASED_EVENT,NULL,0);
    initTimerEvent(&timeEv4,NULL,BUTTON_PRESSED_EVENT,NULL,0);

    timerEventRequest(&timeEv,timeDelay1);
    timerEventRequest(&timeEv2,timeDelay2);
    timerEventRequest(&timeEv3,timeDelay3);
    timerEventRequest(&timeEv4,timeDelay4);
}


void test_checkAndAddTimerEvent_for_first_item(void){
    resetTimerQueueTest();
    setTickTest(0);
    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,50);
    checkAndAddTimerEvent(&timeEv);

    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    fakeCheckIRQ(__LINE__);
}

void test_checkAndAddTimerEvent_for_second_item(void){
    resetTimerQueueTest();
    setTickTest(0);
    initTimerEvent(&timeEv4, NULL ,BUTTON_PRESSED_EVENT,NULL,10);
    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,50);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    disableIRQ_StubWithCallback(fake_disableIRQ);

    timerEventEnqueue(&timeEv);
    checkAndAddTimerEvent(&timeEv4);

    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv,BUTTON_PRESSED_EVENT,
                                  NULL,10);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL,BUTTON_PRESSED_EVENT,
                                  NULL,40);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);

    TEST_ASSERT_EQUAL(2,timerEventQueueList.count);
    fakeCheckIRQ(__LINE__);
}

void test_checkAndAddTimerEvent_for_third_item(void){
    setTickTest(5);
    initTimerEvent(&timeEv3, NULL ,BUTTON_PRESSED_EVENT,
                  NULL,20);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    disableIRQ_StubWithCallback(fake_disableIRQ);

    checkAndAddTimerEvent(&timeEv3);

    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv3,BUTTON_PRESSED_EVENT
                                  ,NULL,10);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv,BUTTON_PRESSED_EVENT
                                  ,NULL,15);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL,BUTTON_PRESSED_EVENT
                                  ,NULL,25);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);

    TEST_ASSERT_EQUAL(3,timerEventQueueList.count);
    fakeCheckIRQ(__LINE__);
}

void test_checkAndAddTimerEvent_for_last_item(void){
    setTickTest(7);
    initTimerEvent(&timeEv2, NULL ,BUTTON_PRESSED_EVENT,NULL,80);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    disableIRQ_StubWithCallback(fake_disableIRQ);

    checkAndAddTimerEvent(&timeEv2);

    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv3,BUTTON_PRESSED_EVENT,
                                NULL,10);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv,BUTTON_PRESSED_EVENT,
                                  NULL,15);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv2,BUTTON_PRESSED_EVENT,
                                  NULL,25);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL,BUTTON_PRESSED_EVENT,
                                  NULL,37);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);

    TEST_ASSERT_EQUAL(4,timerEventQueueList.count);
    fakeCheckIRQ(__LINE__);
}

void test_timerEventDequeue(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initTimerEventQueueWithData(10,30,60,80);
    timerEventDequeue();
    TEST_ASSERT_EQUAL(3,timerEventQueueList.count);

    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv3,BUTTON_PRESSED_EVENT,
                                NULL,20);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv4 ,BUTTON_RELEASED_EVENT,
                                  NULL,30);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL ,BUTTON_PRESSED_EVENT,
                                  NULL,20);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    TEST_ASSERT_EQUAL(0,relativeTick);
    fakeCheckIRQ(__LINE__);
}

void test_timerEventDequeue_event_queue_is_empty(void){
    //resetTick
    resetTimerQueueTest();
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);

    timerEventDequeue();
    TEST_ASSERT_NULL(timerEventQueueList.head);
    TEST_ASSERT_NULL(timerEventQueueList.tail);
    TEST_ASSERT_NULL(timerEventQueueList.current);
    TEST_ASSERT_NULL(timerEventQueueList.previous);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.count);
    TEST_ASSERT_EQUAL(0,relativeTick);
    fakeCheckIRQ(__LINE__);
}

void test_timerEventEnqueue_for_first_item(void){
    resetTimerQueueTest();
    //timerEventEnqueue
    enableIRQ_StubWithCallback(fake_enableIRQ);
    disableIRQ_StubWithCallback(fake_disableIRQ);
    initTimerEvent(&timeEv2, NULL ,NO_EVENT,NULL,30);
    timerEventEnqueue(&timeEv2);
    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL ,
                                  NO_EVENT,NULL,30);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);

    TEST_ASSERT_EQUAL(1,timerEventQueueList.count);
    fakeCheckIRQ(__LINE__);
}

void test_timerEventEnqueue_for_second_item_with_same_delay(void){
    //timerEventEnqueue
    setTickTest(0);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    disableIRQ_StubWithCallback(fake_disableIRQ);
    initTimerEvent(&timeEv3, NULL ,NO_EVENT,NULL,30);

    timerEventEnqueue(&timeEv3);
    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv3 ,NO_EVENT,NULL,30);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL ,NO_EVENT,NULL,0);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    TEST_ASSERT_EQUAL(2,timerEventQueueList.count);
    fakeCheckIRQ(__LINE__);
}


void test_timerEventEnqueue_for_NULL_input(void){
    resetTimerQueueTest();
    //timerEventEnqueue
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);

    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,50);
    timerEventEnqueue(NULL);
    TEST_ASSERT_NULL(timerEventQueueList.head);
    TEST_ASSERT_NULL(timerEventQueueList.tail);
    TEST_ASSERT_NULL(timerEventQueueList.current);
    TEST_ASSERT_NULL(timerEventQueueList.previous);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.count);
    fakeCheckIRQ(__LINE__);
}


void test_timerEventRequest_for_first_item(void){
    resetTimerQueueTest();
    //timerEventEnqueue
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);

    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,0);
    timerEventRequest(&timeEv,100);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.head,NULL ,
                                  BUTTON_PRESSED_EVENT,NULL,100);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.tail,NULL
                                  ,BUTTON_PRESSED_EVENT,NULL,100);
    TEST_ASSERT_EQUAL_TIMER_EVENT((TimerEvent *)timerEventQueueList.current,NULL
                                  ,BUTTON_PRESSED_EVENT,NULL,100);
    TEST_ASSERT_NULL(timerEventQueueList.previous);
    TEST_ASSERT_EQUAL(1,timerEventQueueList.count);
    fakeCheckIRQ(__LINE__);
}


void test_timerEventRequest_for_second_item_set_120ms_after_tick_ady_20ms_back(void){
    setTickTest(20);
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);

    initTimerEvent(&timeEv2, NULL ,NO_EVENT,NULL,0);
    timerEventRequest(&timeEv2,120);
    TEST_ASSERT_EQUAL(2,timerEventQueueList.count);

    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv2 ,
                                  BUTTON_PRESSED_EVENT,NULL,100);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL ,
                                  NO_EVENT,NULL,40);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    fakeCheckIRQ(__LINE__);
}

void test_timerEventRequest_for_second_item_set_110ms_after_tick_ady_30ms_front(void){
    //resetTick
    setTickTest(30);
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initTimerEvent(&timeEv3, NULL ,BUTTON_PRESSED_EVENT,NULL,0);
    timerEventRequest(&timeEv3,50);
    TEST_ASSERT_EQUAL(3,timerEventQueueList.count);

    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv ,
                                  BUTTON_PRESSED_EVENT,NULL,80);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv2 ,
                                  BUTTON_PRESSED_EVENT,NULL,20);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL ,
                                  NO_EVENT,NULL,40);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    fakeCheckIRQ(__LINE__);
}


void test_timerEventRequest_for_second_item_set_80ms_after_tick_ady_40ms_front(void){
    //resetTick
    setTickTest(40);
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initTimerEvent(&timeEv4, NULL ,BUTTON_PRESSED_EVENT,NULL,0);
    timerEventRequest(&timeEv4,80);
    TEST_ASSERT_EQUAL(4,timerEventQueueList.count);

    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv ,
                                  BUTTON_PRESSED_EVENT,NULL,80);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv4 ,
                                  BUTTON_PRESSED_EVENT,NULL,20);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv2 ,
                                  BUTTON_PRESSED_EVENT,NULL,20);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL ,
                                  NO_EVENT,NULL,20);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    fakeCheckIRQ(__LINE__);
}


void test_timerEventISR_event_queue_is_empty(void){
    //resetTick
    resetTimerQueueTest();
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);

    timerEventISR();
    TEST_ASSERT_NULL(timerEventQueueList.head);
    TEST_ASSERT_NULL(timerEventQueueList.tail);
    TEST_ASSERT_NULL(timerEventQueueList.current);
    TEST_ASSERT_NULL(timerEventQueueList.previous);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.count);
    TEST_ASSERT_EQUAL(0,relativeTick);
    fakeCheckIRQ(__LINE__);
}

void test_timerEventISR_event_queue_is_one_time_up(void){
    //resetTick
    resetTimerQueueTest();
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);

    initTimerEvent(&timeEv4, NULL ,TIMEOUT_EVENT,NULL,0);
    timerEventRequest(&timeEv4,80);
    setTickTest(80);
    timerEventISR();
    outputTimerEvent= (TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    TEST_ASSERT_EQUAL(0,relativeTick);
    fakeCheckIRQ(__LINE__);
}

void test_timerEventISR_event_queue_is_one_but_time_havent_reach(void){
    //resetTick
    resetTimerQueueTest();
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);

    initTimerEvent(&timeEv4, NULL ,TIMEOUT_EVENT,NULL,0);
    timerEventRequest(&timeEv4,80);
    setTickTest(55);
    timerEventISR();
    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL ,TIMEOUT_EVENT,NULL,80);
    TEST_ASSERT_EQUAL(55,relativeTick);
    fakeCheckIRQ(__LINE__);
}

void test_checkAndDequeueIfNextEventTimerIsZero_no_zero_value(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initTimerEventQueueWithData(10,30,60,80);

    checkAndDequeueIfNextEventTimerIsZero();

    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv2,NO_EVENT,
                                  NULL,10);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv3,BUTTON_PRESSED_EVENT
                                  ,NULL,20);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv4 ,BUTTON_RELEASED_EVENT,
                                  NULL,30);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL,BUTTON_PRESSED_EVENT,
                                  NULL,20);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    fakeCheckIRQ(__LINE__);
}

void test_checkAndDequeueIfNextEventTimerIsZero_with_one_zero_value(void){
    //resetTick
    resetEventQueueTest();
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initTimerEventQueueWithData(0,30,60,80);
    checkAndDequeueIfNextEventTimerIsZero();
    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv3,BUTTON_PRESSED_EVENT,
                                  NULL,30);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv4 ,BUTTON_RELEASED_EVENT,
                                  NULL,30);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent, NULL ,BUTTON_PRESSED_EVENT,
                                  NULL,20);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    TEST_ASSERT_EQUAL(0,relativeTick);
    fakeCheckIRQ(__LINE__);
}

void test_checkAndDequeueIfNextEventTimerIsZero_with_two_zero_value(void){
    //resetTick
    resetEventQueueTest();
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initTimerEventQueueWithData(0,0,60,80);
    checkAndDequeueIfNextEventTimerIsZero();
    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv4,BUTTON_RELEASED_EVENT
                                  ,NULL,60);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent, NULL ,BUTTON_PRESSED_EVENT
                                  ,NULL,20);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    TEST_ASSERT_EQUAL(0,relativeTick);
    fakeCheckIRQ(__LINE__);
}

void test_checkAndDequeueIfNextEventTimerIsZero_with_three_zero_value(void){
    //resetTick
    resetEventQueueTest();
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initTimerEventQueueWithData(0,0,0,80);
    checkAndDequeueIfNextEventTimerIsZero();
    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL,BUTTON_PRESSED_EVENT
                                  ,NULL,80);
    outputTimerEvent= (TimerEvent*)getNextListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    TEST_ASSERT_EQUAL(0,relativeTick);
    fakeCheckIRQ(__LINE__);
}

void test_checkAndDequeueIfNextEventTimerIsZero_with_all_zero_value(void){
    //resetTick
    resetEventQueueTest();
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initTimerEventQueueWithData(0,0,0,0);
    checkAndDequeueIfNextEventTimerIsZero();
    outputTimerEvent=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    TEST_ASSERT_EQUAL(0,relativeTick);
    fakeCheckIRQ(__LINE__);
}
