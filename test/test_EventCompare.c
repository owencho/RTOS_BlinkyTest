#include "unity.h"
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
#include "mock_Led.h"
#include "CustomAssert.h"
#include "EventCompare.h"
#include "FakeIRQ.h"

TimerEvent timeEv,timeEv2 ,timeEv3,timeEv4;
Event evt , evt2;
Event * outQueue;
GenericStateMachine sm;
int outValue;
void setUp(void){}

void tearDown(void){}

void initTimerEvent(TimerEvent * event, TimerEvent *next ,EventType type,
                    GenericStateMachine * stateMachine,int time){
    event->next = next;
    event->type = type;
    event->stateMachine = stateMachine;
    event->time = time;
}
//eventCompareForAddingTimeEvent(TimerEvent *currentEvent, TimerEvent * newEvent)
// if time in currentEvent->next > newEvent
// return 1
// if time in newEvent > currentEvent->next
// return 0 then newEvent time is deducted with oldEvent

// if currentEvent-> next > newEvent
void test_eventCompareForAddingTimeEvent(void){
    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,50);
    initTimerEvent(&timeEv2, &timeEv3 ,BUTTON_PRESSED_EVENT,NULL,30);
    initTimerEvent(&timeEv3, NULL ,BUTTON_PRESSED_EVENT,NULL,120);
    outValue=eventCompareForAddingTimeEvent (&timeEv2, &timeEv);
    TEST_ASSERT_EQUAL(1,outValue);
    TEST_ASSERT_EQUAL(50-30,timeEv.time);
}

// if currentEvent-> next is NULL
void test_eventCompareForAddingTimeEvent_current_next_NULL(void){
    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,50);
    initTimerEvent(&timeEv2, NULL,BUTTON_PRESSED_EVENT,NULL,30);
    outValue=eventCompareForAddingTimeEvent (&timeEv2, &timeEv);
    TEST_ASSERT_EQUAL(1,outValue);
    TEST_ASSERT_EQUAL(20,timeEv.time);
}

// if currentEvent-> next is smaller than new newEvent time
void test_eventCompareForAddingTimeEvent_current_next_smaller(void){
    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,50);
    initTimerEvent(&timeEv2, &timeEv3,BUTTON_PRESSED_EVENT,NULL,30);
    initTimerEvent(&timeEv3, NULL ,BUTTON_PRESSED_EVENT,NULL,10);
    outValue=eventCompareForAddingTimeEvent (&timeEv2, &timeEv);
    TEST_ASSERT_EQUAL(0,outValue);
    TEST_ASSERT_EQUAL(20,timeEv.time);
}

void test_eventCompareForAddingTimeEvent_NULL_current(void){
    outValue=eventCompareForAddingTimeEvent (NULL, &timeEv);
    TEST_ASSERT_EQUAL(0,outValue);
}

void test_eventCompareForAddingTimeEvent_NULL_new(void){
    outValue=eventCompareForAddingTimeEvent (&timeEv,NULL);
    TEST_ASSERT_EQUAL(0,outValue);
}

void test_eventCompareSameTimeEvent_different(void){
    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,50);
    initTimerEvent(&timeEv2, NULL,BUTTON_PRESSED_EVENT,NULL,30);
    outValue=eventCompareSameTimeEvent (&timeEv2, &timeEv);
    TEST_ASSERT_EQUAL(0,outValue);
}

void test_eventCompareSameTimeEvent_same(void){
    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,50);
    initTimerEvent(&timeEv2, NULL,BUTTON_PRESSED_EVENT,NULL,30);
    outValue=eventCompareSameTimeEvent(&timeEv, &timeEv);
    TEST_ASSERT_EQUAL(1,outValue);
}

void test_eventCompareSameTimeEvent_NULL_current(void){
    outValue=eventCompareSameTimeEvent(NULL, &timeEv);
    TEST_ASSERT_EQUAL(0,outValue);
}

void test_eventCompareSameTimeEvent_NULL_new(void){
    outValue=eventCompareSameTimeEvent(&timeEv,NULL);
    TEST_ASSERT_EQUAL(0,outValue);
}
