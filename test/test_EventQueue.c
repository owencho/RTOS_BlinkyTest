#include "unity.h"
#include "EventQueue.h"
#include "EventType.h"
#include "StateMachine.h"
#include "CustomAssert.h"
#include "List.h"
#include "ListItemCompare.h"
#include "mock_Hardware.h"
#include "mock_Irq.h"
EventQueue eventQueueList;

Event evt , evt2;
Event * outQueue;
GenericStateMachine sm;

void setUp(void){}

void tearDown(void){}

void initEvent(Event * event, Event * next ,EventType type,
               GenericStateMachine * stateMachine,void* data){
    event->next = next;
    event->type = type;
    event->stateMachine = stateMachine;
    event->data = data;
}

void initEventQueue(EventQueue * eventQueue,Event * head,
                    Event * tail,int count){
    eventQueue->head = head;
    eventQueue->tail = tail;
    eventQueue->count = count;
    eventQueue->previous = NULL;
    eventQueue->current = head;
}

void test_Event_eventEnqueue(void){
    disableIRQ_Expect();
    enableIRQ_Expect();
    initEvent(&evt, NULL,BUTTON_PRESSED_EVENT,&sm,NULL);
    initEventQueue(&eventQueueList,NULL,NULL,0);
    eventEnqueue(&eventQueueList,&evt);
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
    initEventQueue(&eventQueueList,&evt,&evt,1);
    eventEnqueue(&eventQueueList,NULL);
    TEST_ASSERT_EQUAL(&evt,eventQueueList.head);
    TEST_ASSERT_EQUAL(&evt,eventQueueList.tail);
    TEST_ASSERT_EQUAL(&evt,eventQueueList.current);
    TEST_ASSERT_NULL(eventQueueList.previous);
    TEST_ASSERT_EQUAL(1,eventQueueList.count);
}

void test_Event_eventEnqueue_NULL_queue(void){
    disableIRQ_Expect();
    enableIRQ_Expect();
    initEvent(&evt, NULL,BUTTON_PRESSED_EVENT,&sm,NULL);
    initEventQueue(&eventQueueList,NULL,NULL,0);
    eventEnqueue(NULL,&evt);
}



void test_Event_eventEnqueue_withqueue_inside(void){
    disableIRQ_Expect();
    enableIRQ_Expect();
    initEvent(&evt, NULL,BUTTON_PRESSED_EVENT,&sm,NULL);
    initEvent(&evt2, NULL,BUTTON_RELEASED_EVENT,&sm,NULL);
    initEventQueue(&eventQueueList,&evt,&evt,1);
    eventEnqueue(&eventQueueList,&evt2);
    TEST_ASSERT_EQUAL(&evt,eventQueueList.head);
    TEST_ASSERT_EQUAL(&evt2,eventQueueList.tail);
    TEST_ASSERT_EQUAL(&evt,eventQueueList.current);
    TEST_ASSERT_NULL(eventQueueList.previous);
    TEST_ASSERT_EQUAL(2,eventQueueList.count);
}

void test_Event_eventdequeue_with2queue_inside(void){
    disableIRQ_Expect();
    enableIRQ_Expect();
    initEvent(&evt, &evt2,BUTTON_PRESSED_EVENT,&sm,NULL);
    initEvent(&evt2, NULL,BUTTON_RELEASED_EVENT,&sm,NULL);
    initEventQueue(&eventQueueList,&evt,&evt2,2);
    TEST_ASSERT_EQUAL(1,eventDequeue(&eventQueueList,&outQueue));
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
    initEvent(&evt2, NULL,BUTTON_RELEASED_EVENT,&sm,NULL);
    initEventQueue(&eventQueueList,&evt2,&evt2,1);
    TEST_ASSERT_EQUAL(1,eventDequeue(&eventQueueList,&outQueue));
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
    initEventQueue(&eventQueueList,NULL,NULL,0);
    TEST_ASSERT_EQUAL(0,eventDequeue(&eventQueueList,&outQueue));
}

void test_Event_eventdequeue_withNULL_inside(void){
    disableIRQ_Expect();
    enableIRQ_Expect();
    TEST_ASSERT_EQUAL(0,eventDequeue(&eventQueueList,NULL));
}

void test_Event_eventdequeue_withNULL_queue_inside(void){
    disableIRQ_Expect();
    enableIRQ_Expect();
    TEST_ASSERT_EQUAL(0,eventDequeue(NULL,&outQueue));
}
