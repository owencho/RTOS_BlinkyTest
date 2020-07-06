#include "unity.h"
#include "Event.h"
#include "EventType.h"
#include "StateMachine.h"
#include "CustomAssert.h"
#include "List.h"
#include "ListItemCompare.h"
#include "mock_Hardware.h"

extern List eventQueueList;

Event evt , evt2;
Event * outQueue;
GenericStateMachine sm;

void setUp(void){}

void tearDown(void){}

void initEvent(Event * event, ListItem * next ,EventType type,
               GenericStateMachine * stateMachine,void* data){
    event->next = next;
    event->type = type;
    event->stateMachine = stateMachine;
    event->data = data;
}

void test_Event_eventEnqueue(void){
    disableIRQ_Expect();
    enableIRQ_Expect();
    initEvent(&evt, NULL,BUTTON_PRESSED_EVENT,&sm,NULL);
    eventEnqueue(&evt);
    TEST_ASSERT_EQUAL(&evt,eventQueueList.head);
    TEST_ASSERT_EQUAL(&evt,eventQueueList.tail);
    TEST_ASSERT_EQUAL(&evt,eventQueueList.current);
    TEST_ASSERT_NULL(eventQueueList.previous);
    TEST_ASSERT_EQUAL(1,eventQueueList.count);
}

void test_Event_eventEnqueue_with_NULL_input(void){
    disableIRQ_Expect();
    enableIRQ_Expect();
    initEvent(&evt, NULL,BUTTON_PRESSED_EVENT,&sm,NULL);
    eventEnqueue(NULL);
    TEST_ASSERT_EQUAL(&evt,eventQueueList.head);
    TEST_ASSERT_EQUAL(&evt,eventQueueList.tail);
    TEST_ASSERT_EQUAL(&evt,eventQueueList.current);
    TEST_ASSERT_NULL(eventQueueList.previous);
    TEST_ASSERT_EQUAL(1,eventQueueList.count);
}


void test_Event_eventEnqueue_withqueue_inside(void){
    disableIRQ_Expect();
    enableIRQ_Expect();
    initEvent(&evt2, NULL,BUTTON_RELEASED_EVENT,&sm,NULL);
    eventEnqueue(&evt2);
    TEST_ASSERT_EQUAL(&evt,eventQueueList.head);
    TEST_ASSERT_EQUAL(&evt2,eventQueueList.tail);
    TEST_ASSERT_EQUAL(&evt,eventQueueList.current);
    TEST_ASSERT_NULL(eventQueueList.previous);
    TEST_ASSERT_EQUAL(2,eventQueueList.count);
}

void test_Event_eventdequeue_with2queue_inside(void){
    disableIRQ_Expect();
    enableIRQ_Expect();
    TEST_ASSERT_EQUAL(1,eventDequeue(&outQueue));
    TEST_ASSERT_EQUAL(&evt,outQueue);
    TEST_ASSERT_EQUAL(&evt2,eventQueueList.head);
    TEST_ASSERT_EQUAL(&evt2,eventQueueList.tail);
    TEST_ASSERT_EQUAL(&evt2,eventQueueList.current);
    TEST_ASSERT_NULL(eventQueueList.previous);
    TEST_ASSERT_EQUAL(1,eventQueueList.count);
}

void test_Event_eventdequeue_with1queue_inside(void){
    disableIRQ_Expect();
    enableIRQ_Expect();
    TEST_ASSERT_EQUAL(1,eventDequeue(&outQueue));
    TEST_ASSERT_EQUAL(&evt2,outQueue);
    TEST_ASSERT_NULL(eventQueueList.head);
    TEST_ASSERT_NULL(eventQueueList.tail);
    TEST_ASSERT_NULL(eventQueueList.current);
    TEST_ASSERT_NULL(eventQueueList.previous);
    TEST_ASSERT_EQUAL(0,eventQueueList.count);
}

void test_Event_eventdequeue_withNoqueue_inside(void){
    disableIRQ_Expect();
    enableIRQ_Expect();
    TEST_ASSERT_EQUAL(0,eventDequeue(&outQueue));
}

void test_Event_eventdequeue_withNULL_inside(void){
    disableIRQ_Expect();
    enableIRQ_Expect();
    TEST_ASSERT_EQUAL(0,eventDequeue(NULL));
}
