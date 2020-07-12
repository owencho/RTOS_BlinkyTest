#include "unity.h"
#include "mock_Time.h"
#include "TimerEvent.h"
#include "TimerEventQueue.h"
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
TimerEvent * outputTimerEvent;
TimerEventQueue timerEventQueueList;


void initTimerEvent(TimerEvent * event, TimerEvent *next ,EventType type,
                    GenericStateMachine * stateMachine,int time){
    event->next = next;
    event->type = type;
    event->stateMachine = stateMachine;
    event->time = time;
}

void initEventQueue(TimerEventQueue * eventQueue,TimerEvent * head,
                    TimerEvent * tail,int count,int tick){
    eventQueue->head = head;
    eventQueue->tail = tail;
    eventQueue->count = count;
    eventQueue->previous = NULL;
    eventQueue->current = head;
    eventQueue->relativeTick = tick;
}

// it wont do anything for empty eventQueue
void test_checkAndAddTimerEvent_for_first_item(void){
    initEventQueue(&timerEventQueueList, NULL,NULL,0,0);
    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,50);
    checkAndAddTimerEvent(&timerEventQueueList,&timeEv);

    outputTimerEvent=(TimerEvent*)getCurrentListItem((List*)&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    fakeCheckIRQ(__LINE__);
}
/*
*             (relative tick is 0)
*              timerEv
*                 50
*             (add timerEv4 with 10 second)
*
*               timerEv4          timerEv
*                 10       ----->   40
*
*/
void test_checkAndAddTimerEvent_for_second_item(void){
    initEventQueue(&timerEventQueueList, &timeEv ,&timeEv,1,0);
    initTimerEvent(&timeEv4, NULL ,BUTTON_PRESSED_EVENT,NULL,10);
    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,50);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    disableIRQ_StubWithCallback(fake_disableIRQ);

    checkAndAddTimerEvent(&timerEventQueueList,&timeEv4);

    outputTimerEvent=(TimerEvent*)getCurrentListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv,BUTTON_PRESSED_EVENT,
                                  NULL,10);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL,BUTTON_PRESSED_EVENT,
                                  NULL,40);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);

    TEST_ASSERT_EQUAL(2,timerEventQueueList.count);
    fakeCheckIRQ(__LINE__);
}
/*
*             (relative tick is 5)
*               timerEv4          timerEv
*                 10       ----->   40
*             (add timerEv3 with 20 second)
*               timerEv4          timerEv3        timerEv
*                 10       ----->   15      ---->   25
*        |
*        5(relativeTick)
*/
void test_checkAndAddTimerEvent_for_third_item(void){
    initEventQueue(&timerEventQueueList, &timeEv4 ,&timeEv,2,5);
    initTimerEvent(&timeEv4, &timeEv ,BUTTON_PRESSED_EVENT,NULL,10);
    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,40);
    initTimerEvent(&timeEv3, NULL ,BUTTON_PRESSED_EVENT,
                  NULL,20);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    disableIRQ_StubWithCallback(fake_disableIRQ);

    checkAndAddTimerEvent(&timerEventQueueList,&timeEv3);

    outputTimerEvent=(TimerEvent*)getCurrentListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv3,BUTTON_PRESSED_EVENT
                                  ,NULL,10);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv,BUTTON_PRESSED_EVENT
                                  ,NULL,15);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL,BUTTON_PRESSED_EVENT
                                  ,NULL,25);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);

    TEST_ASSERT_EQUAL(3,timerEventQueueList.count);
    fakeCheckIRQ(__LINE__);
}
/*
*             (relative tick is 5)
*               timerEv4          timerEv3        timerEv
*                 10       ----->   15      ---->   25
*             (add timerEv3 with 80 second)
*               timerEv4          timerEv3        timerEv     timerEv2
*                 10       ----->   15      ---->   25   --->    37
*        |
*        7(relativeTick)
*/
void test_checkAndAddTimerEvent_for_last_item(void){
    initEventQueue(&timerEventQueueList, &timeEv4 ,&timeEv,3,7);
    initTimerEvent(&timeEv4, &timeEv3 ,BUTTON_PRESSED_EVENT,NULL,10);
    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,25);
    initTimerEvent(&timeEv3, &timeEv ,BUTTON_PRESSED_EVENT,
                  NULL,15);
    initTimerEvent(&timeEv2, NULL ,BUTTON_PRESSED_EVENT,NULL,80);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    disableIRQ_StubWithCallback(fake_disableIRQ);

    checkAndAddTimerEvent(&timerEventQueueList,&timeEv2);

    outputTimerEvent=(TimerEvent*)getCurrentListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv3,BUTTON_PRESSED_EVENT,
                                NULL,10);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv,BUTTON_PRESSED_EVENT,
                                  NULL,15);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv2,BUTTON_PRESSED_EVENT,
                                  NULL,25);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL,BUTTON_PRESSED_EVENT,
                                  NULL,37);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);

    TEST_ASSERT_EQUAL(4,timerEventQueueList.count);
    fakeCheckIRQ(__LINE__);
}
/*
*             (relative tick is 5)
*               timerEv4          timerEv3        timerEv     timerEv2
*                 10       ----->   15      ---->   25   --->    37
*             (dequeue)
*               timerEv3        timerEv     timerEv2
*                 15      ---->   25   --->    37
*        |
*        0(relativeTick)
*/
void test_timerEventDequeue(void){
    initEventQueue(&timerEventQueueList, &timeEv4 ,&timeEv,4,7);
    initTimerEvent(&timeEv4, &timeEv3 ,BUTTON_PRESSED_EVENT,NULL,10);
    initTimerEvent(&timeEv, &timeEv2 ,BUTTON_PRESSED_EVENT,NULL,25);
    initTimerEvent(&timeEv3, &timeEv ,BUTTON_PRESSED_EVENT,NULL,15);
    initTimerEvent(&timeEv2, NULL ,BUTTON_PRESSED_EVENT,NULL,37);
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    timerEventDequeue(&timerEventQueueList);
    TEST_ASSERT_EQUAL(3,timerEventQueueList.count);

    outputTimerEvent=(TimerEvent*)getCurrentListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv,BUTTON_PRESSED_EVENT,
                                NULL,15);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv2 ,BUTTON_PRESSED_EVENT,
                                  NULL,25);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL ,BUTTON_PRESSED_EVENT,
                                  NULL,37);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.relativeTick);
    fakeCheckIRQ(__LINE__);
}

void test_timerEventDequeue_event_queue_is_empty(void){
    //resetTick
    initEventQueue(&timerEventQueueList, NULL ,NULL,0,0);
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);

    timerEventDequeue(&timerEventQueueList);
    TEST_ASSERT_NULL(timerEventQueueList.head);
    TEST_ASSERT_NULL(timerEventQueueList.tail);
    TEST_ASSERT_NULL(timerEventQueueList.current);
    TEST_ASSERT_NULL(timerEventQueueList.previous);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.count);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.relativeTick);
    fakeCheckIRQ(__LINE__);
}
/*
*             (relative tick is 0)
*             (add timerEv4 with 10 second)
*
*               timerEv4
*                 10
*
*/
void test_timerEventEnqueue_for_first_item(void){
    //timerEventEnqueue
    enableIRQ_StubWithCallback(fake_enableIRQ);
    disableIRQ_StubWithCallback(fake_disableIRQ);
    initEventQueue(&timerEventQueueList, NULL ,NULL,0,0);
    initTimerEvent(&timeEv4, NULL ,BUTTON_PRESSED_EVENT,NULL,10);
    timerEventEnqueue(&timerEventQueueList,&timeEv4);
    outputTimerEvent=(TimerEvent*)getCurrentListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL,
                                  BUTTON_PRESSED_EVENT,NULL,10);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    TEST_ASSERT_EQUAL(1,timerEventQueueList.count);
    fakeCheckIRQ(__LINE__);
}
/*
*             (relative tick is 0)
*              timerEv4
*                 10
*             (add timerEv with 10 second)
*
*               timerEv4          timerEv
*                 10       ----->   0
*
*/
void test_timerEventEnqueue_for_second_item_with_same_delay(void){
    //timerEventEnqueue
    enableIRQ_StubWithCallback(fake_enableIRQ);
    disableIRQ_StubWithCallback(fake_disableIRQ);
    initEventQueue(&timerEventQueueList, &timeEv4 ,&timeEv4,1,0);
    initTimerEvent(&timeEv4, NULL ,BUTTON_PRESSED_EVENT,NULL,10);
    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,10);
    timerEventEnqueue(&timerEventQueueList,&timeEv);

    outputTimerEvent=(TimerEvent*)getCurrentListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv ,BUTTON_PRESSED_EVENT,NULL,10);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL ,BUTTON_PRESSED_EVENT,NULL,0);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    TEST_ASSERT_EQUAL(2,timerEventQueueList.count);
    fakeCheckIRQ(__LINE__);
}


void test_timerEventEnqueue_for_NULL_input(void){
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);

    initEventQueue(&timerEventQueueList, NULL ,NULL,0,0);
    initTimerEvent(&timeEv4, NULL ,BUTTON_PRESSED_EVENT,NULL,10);
    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,10);
    timerEventEnqueue(NULL,&timeEv);
    TEST_ASSERT_NULL(timerEventQueueList.head);
    TEST_ASSERT_NULL(timerEventQueueList.tail);
    TEST_ASSERT_NULL(timerEventQueueList.current);
    TEST_ASSERT_NULL(timerEventQueueList.previous);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.count);
    fakeCheckIRQ(__LINE__);
}

void test_timerEventEnqueue_for_NULL_input_event(void){
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);

    initEventQueue(&timerEventQueueList, NULL ,NULL,0,0);
    initTimerEvent(&timeEv4, NULL ,BUTTON_PRESSED_EVENT,NULL,10);
    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,10);
    timerEventEnqueue(&timerEventQueueList,NULL);
    TEST_ASSERT_NULL(timerEventQueueList.head);
    TEST_ASSERT_NULL(timerEventQueueList.tail);
    TEST_ASSERT_NULL(timerEventQueueList.current);
    TEST_ASSERT_NULL(timerEventQueueList.previous);
    TEST_ASSERT_EQUAL(0,timerEventQueueList.count);
    fakeCheckIRQ(__LINE__);
}

/*
*             (relative tick is 0)
*             (add timeEv with 100 second)
*
*               timeEv
*                 100
*
*/
void test_timerEventRequest_for_first_item(void){
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initEventQueue(&timerEventQueueList, NULL ,NULL,0,0);
    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,0);
    timerEventRequest(&timerEventQueueList,&timeEv,100);
    TEST_ASSERT_EQUAL_TIMER_EVENT(timerEventQueueList.head,NULL ,
                                  BUTTON_PRESSED_EVENT,NULL,100);
    TEST_ASSERT_EQUAL_TIMER_EVENT(timerEventQueueList.tail,NULL
                                  ,BUTTON_PRESSED_EVENT,NULL,100);
    TEST_ASSERT_EQUAL_TIMER_EVENT(timerEventQueueList.current,NULL
                                  ,BUTTON_PRESSED_EVENT,NULL,100);
    TEST_ASSERT_NULL(timerEventQueueList.previous);
    TEST_ASSERT_EQUAL(1,timerEventQueueList.count);
    fakeCheckIRQ(__LINE__);
}

/*
*               timeEv
*                 100
*             (add timeEv2 with 120 second)
*
*               timeEv  --->   timeEv2
*                 100           40
*       |
*      20ms
*/
void test_timerEventRequest_for_second_item_set_120ms_after_tick_ady_20ms_back(void){
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initEventQueue(&timerEventQueueList, &timeEv ,&timeEv,1,20);
    initTimerEvent(&timeEv, NULL ,BUTTON_PRESSED_EVENT,NULL,100);
    initTimerEvent(&timeEv2, NULL ,NO_EVENT,NULL,0);
    timerEventRequest(&timerEventQueueList,&timeEv2,120);
    TEST_ASSERT_EQUAL(2,timerEventQueueList.count);

    outputTimerEvent=(TimerEvent*)getCurrentListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv2 ,
                                  BUTTON_PRESSED_EVENT,NULL,100);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL ,
                                  NO_EVENT,NULL,40);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    fakeCheckIRQ(__LINE__);
}
/*
*               timeEv  --->   timeEv2
*                 100           40
*             (add timeEv3 with 50 second)
*
*              timeEv3 ---> timeEv  --->   timeEv2
*                80          20             40
*       |
*      30ms
*/
void test_timerEventRequest_for_second_item_set_50ms_after_tick_ady_30ms_front(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initEventQueue(&timerEventQueueList, &timeEv ,&timeEv2,2,30);
    initTimerEvent(&timeEv, &timeEv2 ,BUTTON_PRESSED_EVENT,NULL,100);
    initTimerEvent(&timeEv2, NULL ,NO_EVENT,NULL,40);
    initTimerEvent(&timeEv3, NULL ,BUTTON_PRESSED_EVENT,NULL,0);
    timerEventRequest(&timerEventQueueList,&timeEv3,50);
    TEST_ASSERT_EQUAL(3,timerEventQueueList.count);

    outputTimerEvent=(TimerEvent*)getCurrentListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv ,
                                  BUTTON_PRESSED_EVENT,NULL,80);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv2 ,
                                  BUTTON_PRESSED_EVENT,NULL,20);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL ,
                                  NO_EVENT,NULL,40);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    fakeCheckIRQ(__LINE__);
}

/*
*              timeEv3 ---> timeEv  --->   timeEv2
*                80          20             40
*             (add timeEv4 with 80 second)
*
*              timeEv3 ---> timeEv  ---> timeEv4--->  timeEv2
*                80          20             20          40
*       |
*      40ms
*/
void test_timerEventRequest_for_second_item_set_80ms_after_tick_ady_40ms_front(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initEventQueue(&timerEventQueueList, &timeEv3 ,&timeEv2,3,40);
    initTimerEvent(&timeEv, &timeEv2 ,BUTTON_PRESSED_EVENT,NULL,20);
    initTimerEvent(&timeEv2, NULL ,NO_EVENT,NULL,40);
    initTimerEvent(&timeEv3, &timeEv ,BUTTON_PRESSED_EVENT,NULL,80);
    initTimerEvent(&timeEv4, NULL ,BUTTON_PRESSED_EVENT,NULL,0);
    timerEventRequest(&timerEventQueueList,&timeEv4,80);
    TEST_ASSERT_EQUAL(4,timerEventQueueList.count);

    outputTimerEvent=(TimerEvent*)getCurrentListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv ,
                                  BUTTON_PRESSED_EVENT,NULL,80);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv4 ,
                                  BUTTON_PRESSED_EVENT,NULL,20);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,&timeEv2 ,
                                  BUTTON_PRESSED_EVENT,NULL,20);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_EQUAL_TIMER_EVENT(outputTimerEvent,NULL ,
                                  NO_EVENT,NULL,20);
    outputTimerEvent= (TimerEvent*)getNextListItem((List*)&timerEventQueueList);
    TEST_ASSERT_NULL(outputTimerEvent);
    fakeCheckIRQ(__LINE__);
}

void test_timerEventQueueGetRelativeTick(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initEventQueue(&timerEventQueueList, NULL ,NULL,0,50);
    TEST_ASSERT_EQUAL(50,timerEventQueueGetRelativeTick(&timerEventQueueList));
    fakeCheckIRQ(__LINE__);
}

void test_timerEventQueueGetRelativeTick_NULL(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initEventQueue(&timerEventQueueList, NULL ,NULL,0,50);
    TEST_ASSERT_EQUAL(0,timerEventQueueGetRelativeTick(NULL));
    fakeCheckIRQ(__LINE__);
}

void test_timerEventQueueGetCount(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initEventQueue(&timerEventQueueList, NULL ,NULL,1,50);
    TEST_ASSERT_EQUAL(1,timerEventQueueGetCount(&timerEventQueueList));
    fakeCheckIRQ(__LINE__);
}

void test_timerEventQueueGetCount_NULL(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initEventQueue(&timerEventQueueList, NULL ,NULL,0,50);
    TEST_ASSERT_EQUAL(0,timerEventQueueGetCount(NULL));
    fakeCheckIRQ(__LINE__);
}

void test_timerEventQueueGetCurrentEvent(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initEventQueue(&timerEventQueueList, NULL ,NULL,1,50);
    TEST_ASSERT_NULL(timerEventQueueGetCurrentEvent(&timerEventQueueList));
    fakeCheckIRQ(__LINE__);
}

void test_timerEventQueueGetCurrentEvent_with_event(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initEventQueue(&timerEventQueueList, &timeEv ,&timeEv ,1,50);
    TEST_ASSERT_EQUAL(&timeEv,timerEventQueueGetCurrentEvent(&timerEventQueueList));
    fakeCheckIRQ(__LINE__);
}

void test_timerEventQueueGetCurrentEvent_NULL(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initEventQueue(&timerEventQueueList, NULL ,NULL,0,50);
    TEST_ASSERT_NULL(timerEventQueueGetCurrentEvent(NULL));
    fakeCheckIRQ(__LINE__);
}

void test_timerEventQueueGetNextEvent_with_event(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initEventQueue(&timerEventQueueList, &timeEv ,&timeEv ,1,50);
    initTimerEvent(&timeEv, &timeEv2 ,BUTTON_PRESSED_EVENT,NULL,20);
    TEST_ASSERT_EQUAL(&timeEv2,timerEventQueueGetNextEvent(&timerEventQueueList));
    fakeCheckIRQ(__LINE__);
}

void test_timerEventQueueGetNextEvent_NULL(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initEventQueue(&timerEventQueueList, NULL ,NULL,0,50);
    TEST_ASSERT_NULL(timerEventQueueGetNextEvent(NULL));
    fakeCheckIRQ(__LINE__);
}

void test_resetCurrentTimerEventQueue_with_event(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initEventQueue(&timerEventQueueList, &timeEv ,&timeEv2 ,2,50);
    initTimerEvent(&timeEv, &timeEv2 ,BUTTON_PRESSED_EVENT,NULL,20);
    timerEventQueueGetNextEvent(&timerEventQueueList);
    TEST_ASSERT_EQUAL(&timeEv2,timerEventQueueList.current);
    resetCurrentTimerEventQueue(&timerEventQueueList);
    TEST_ASSERT_EQUAL(&timeEv,timerEventQueueList.current);
    fakeCheckIRQ(__LINE__);
}

void test_resetCurrentTimerEventQueue_NULL(void){
    //resetTick
    disableIRQ_StubWithCallback(fake_disableIRQ);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    initEventQueue(&timerEventQueueList, &timeEv ,&timeEv2 ,2,50);
    initTimerEvent(&timeEv, &timeEv2 ,BUTTON_PRESSED_EVENT,NULL,20);
    timerEventQueueGetNextEvent(&timerEventQueueList);
    TEST_ASSERT_EQUAL(&timeEv2,timerEventQueueList.current);
    resetCurrentTimerEventQueue(NULL);
    TEST_ASSERT_EQUAL(&timeEv2,timerEventQueueList.current);
    fakeCheckIRQ(__LINE__);
}
