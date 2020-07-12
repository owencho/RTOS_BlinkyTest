#include "unity.h"
#include "mock_Time.h"
#include "TimerEvent.h"
#include "TimerEventISR.h"
#include "TimerEventQueue.h"
#include "List.h"
#include "ListItemCompare.h"
#include "Event.h"
#include "EventQueue.h"
#include "mock_Hardware.h"
#include "mock_Irq.h"
#include "mock_Exti.h"
#include "mock_Button.h"
#include <stdint.h>
#include "StateMachine.h"
#include "CustomAssert.h"
#include "EventType.h"
#include "mock_Led.h"
#include "CustomAssert.h"
#include "EventCompare.h"
#include "FakeIRQ.h"
TimerEvent timeEv,timeEv2 ,timeEv3,timeEv4;
EventQueue eventQueue;
TimerEvent * outputTimerEvent;
TimerEventQueue timerEventQueueList;

void setUp(void){}
void tearDown(void){}

void initTimerEvent(TimerEvent * event, TimerEvent *next ,EventType type,
                    GenericStateMachine * stateMachine,int time){
    event->next = next;
    event->type = type;
    event->stateMachine = stateMachine;
    event->time = time;
}

void initTimerEventQueue(TimerEventQueue * eventQueue,TimerEvent * head,
                    TimerEvent * tail,int count,int tick){
    eventQueue->head = head;
    eventQueue->tail = tail;
    eventQueue->count = count;
    eventQueue->previous = NULL;
    eventQueue->current = head;
    eventQueue->relativeTick = tick;
}

void resetEventQueue(EventQueue * eventQueue){
    eventQueue->head = NULL;
    eventQueue->tail = NULL;
    eventQueue->count = 0;
    eventQueue->previous = NULL;
    eventQueue->current = NULL;
}
void test_timerEventISR_event_queue_is_empty(void){
    //resetTick
    initTimerEventQueue(&timerEventQueueList, NULL,NULL,0,0);
    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,50);
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);

    timerEventISR(&eventQueue,&timerEventQueueList);
    TEST_ASSERT_NULL(timerEventQueueList.head);
    TEST_ASSERT_NULL(timerEventQueueList.tail);
    TEST_ASSERT_NULL(timerEventQueueList.current);
    TEST_ASSERT_NULL(timerEventQueueList.previous);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.count);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.relativeTick);
    fakeCheckIRQ(__LINE__);
}

void test_timerEventISR_event_queue_is_one_time_up(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);

    initTimerEvent(&timeEv4, NULL ,TIMEOUT_EVENT,NULL,80);
    initTimerEventQueue(&timerEventQueueList, &timeEv4,&timeEv4,1,80);
    timerEventISR(&eventQueue,&timerEventQueueList);
    outputTimerEvent= (TimerEvent*)getCurrentListItem((List*)&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.relativeTick);
    fakeCheckIRQ(__LINE__);
}

void test_timerEventISR_event_queue_is_one_but_time_havent_reach(void){
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);

    initTimerEvent(&timeEv4, NULL ,TIMEOUT_EVENT,NULL,90);
    initTimerEventQueue(&timerEventQueueList, &timeEv4,&timeEv4,1,80);
    timerEventISR(&eventQueue,&timerEventQueueList);
    outputTimerEvent=(TimerEvent*)getCurrentListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL ,TIMEOUT_EVENT,NULL,90);
    TEST_ASSERT_EQUAL(80,timerEventQueueList.relativeTick);
    fakeCheckIRQ(__LINE__);
}

void test_checkAndDequeueIfNextEventTimerIsZero_no_zero_value(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);

    initTimerEventQueue(&timerEventQueueList, &timeEv4 ,&timeEv2,4,0);
    initTimerEvent(&timeEv4, &timeEv ,BUTTON_PRESSED_EVENT,NULL,10);
    initTimerEvent(&timeEv, &timeEv3 ,BUTTON_PRESSED_EVENT,NULL,30);
    initTimerEvent(&timeEv3, &timeEv2 ,BUTTON_PRESSED_EVENT,NULL,60);
    initTimerEvent(&timeEv2, NULL ,BUTTON_PRESSED_EVENT,NULL,80);
    resetEventQueue(&eventQueue);
    checkAndDequeueIfNextEventTimerIsZero(&eventQueue,&timerEventQueueList);

    outputTimerEvent=(TimerEvent*)getCurrentListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv,BUTTON_PRESSED_EVENT,
                                  NULL,10);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv3,BUTTON_PRESSED_EVENT
                                  ,NULL,30);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv2 ,BUTTON_PRESSED_EVENT,
                                  NULL,60);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL,BUTTON_PRESSED_EVENT,
                                  NULL,80);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.relativeTick);
    fakeCheckIRQ(__LINE__);
}

void test_checkAndDequeueIfNextEventTimerIsZero_with_one_zero_value(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initTimerEventQueue(&timerEventQueueList, &timeEv4 ,&timeEv2,4,0);
    initTimerEvent(&timeEv4, &timeEv ,BUTTON_PRESSED_EVENT,NULL,0);
    initTimerEvent(&timeEv, &timeEv3 ,BUTTON_PRESSED_EVENT,NULL,30);
    initTimerEvent(&timeEv3, &timeEv2 ,BUTTON_PRESSED_EVENT,NULL,60);
    initTimerEvent(&timeEv2, NULL ,BUTTON_PRESSED_EVENT,NULL,80);
    resetEventQueue(&eventQueue);
    checkAndDequeueIfNextEventTimerIsZero(&eventQueue,&timerEventQueueList);
    outputTimerEvent=(TimerEvent*)getCurrentListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv3,BUTTON_PRESSED_EVENT,
                                  NULL,30);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv2 ,BUTTON_PRESSED_EVENT,
                                  NULL,60);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent, NULL ,BUTTON_PRESSED_EVENT,
                                  NULL,80);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.relativeTick);
    fakeCheckIRQ(__LINE__);
}

void test_checkAndDequeueIfNextEventTimerIsZero_with_two_zero_value(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initTimerEventQueue(&timerEventQueueList, &timeEv4 ,&timeEv2,4,0);
    initTimerEvent(&timeEv4, &timeEv ,BUTTON_PRESSED_EVENT,NULL,0);
    initTimerEvent(&timeEv, &timeEv3 ,BUTTON_PRESSED_EVENT,NULL,0);
    initTimerEvent(&timeEv3, &timeEv2 ,BUTTON_PRESSED_EVENT,NULL,60);
    initTimerEvent(&timeEv2, NULL ,BUTTON_PRESSED_EVENT,NULL,80);
    resetEventQueue(&eventQueue);
    checkAndDequeueIfNextEventTimerIsZero(&eventQueue,&timerEventQueueList);
    outputTimerEvent=(TimerEvent*)getCurrentListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv2 ,BUTTON_PRESSED_EVENT,
                                  NULL,60);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent, NULL ,BUTTON_PRESSED_EVENT,
                                  NULL,80);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.relativeTick);
    fakeCheckIRQ(__LINE__);
}

void test_checkAndDequeueIfNextEventTimerIsZero_with_three_zero_value(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initTimerEventQueue(&timerEventQueueList, &timeEv4 ,&timeEv2,4,0);
    initTimerEvent(&timeEv4, &timeEv ,BUTTON_PRESSED_EVENT,NULL,0);
    initTimerEvent(&timeEv, &timeEv3 ,BUTTON_PRESSED_EVENT,NULL,0);
    initTimerEvent(&timeEv3, &timeEv2 ,BUTTON_PRESSED_EVENT,NULL,0);
    initTimerEvent(&timeEv2, NULL ,BUTTON_PRESSED_EVENT,NULL,80);
    resetEventQueue(&eventQueue);
    checkAndDequeueIfNextEventTimerIsZero(&eventQueue,&timerEventQueueList);
    outputTimerEvent=(TimerEvent*)getCurrentListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL,BUTTON_PRESSED_EVENT
                                  ,NULL,80);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.relativeTick);
    fakeCheckIRQ(__LINE__);
}

void test_checkAndDequeueIfNextEventTimerIsZero_with_all_zero_value(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initTimerEventQueue(&timerEventQueueList, &timeEv4 ,&timeEv2,4,0);
    initTimerEvent(&timeEv4, &timeEv ,BUTTON_PRESSED_EVENT,NULL,0);
    initTimerEvent(&timeEv, &timeEv3 ,BUTTON_PRESSED_EVENT,NULL,0);
    initTimerEvent(&timeEv3, &timeEv2 ,BUTTON_PRESSED_EVENT,NULL,0);
    initTimerEvent(&timeEv2, NULL ,BUTTON_PRESSED_EVENT,NULL,0);
    resetEventQueue(&eventQueue);
    checkAndDequeueIfNextEventTimerIsZero(&eventQueue,&timerEventQueueList);;
    outputTimerEvent=(TimerEvent*)getCurrentListItem((List*)&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.relativeTick);
    fakeCheckIRQ(__LINE__);
}
