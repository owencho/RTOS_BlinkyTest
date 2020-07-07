#include "unity.h"
#include "TimerEvent.h"
#include "CustomAssert.h"
#include "EventType.h"
#include "StateMachine.h"
void setUp(void){}
void tearDown(void){}
TimerEvent  itemA,itemB,itemC;
TimerEvent *listItemB;
int  linkItemDataA,linkItemDataB,linkItemDataC;
GenericStateMachine sm , sm2;

void initTimerEvent(TimerEvent * event, TimerEvent *next ,EventType type,
                    GenericStateMachine * stateMachine,int time){
    event->next = next;
    event->type = type;
    event->stateMachine = stateMachine;
    event->time = time;
}

void test_CustomAssert_pass_all_same(void){
    initTimerEvent(&itemB,NULL,NO_EVENT,NULL,50);
    TEST_ASSERT_EQUAL_TIMER_EVENT(&itemB,NULL,NO_EVENT,NULL,50);
}
void test_actualListItem_NULL(void){
    TEST_ASSERT_EQUAL_TIMER_EVENT(NULL,NULL,NO_EVENT,NULL,50);
}

void test_CustomAssert_fail_actual_next_null(void){
    initTimerEvent(&itemB,NULL,NO_EVENT,NULL,50);
    TEST_ASSERT_EQUAL_TIMER_EVENT(&itemB,&itemA,NO_EVENT,NULL,50);
}

void test_CustomAssert_fail_expected_next_null(void){
    initTimerEvent(&itemB,&itemA,NO_EVENT,NULL,50);
    TEST_ASSERT_EQUAL_TIMER_EVENT(&itemB,NULL,NO_EVENT,NULL,50);
}

void test_CustomAssert_fail_expected_actual_next_different(void){
    initTimerEvent(&itemB,&itemA,NO_EVENT,NULL,50);
    TEST_ASSERT_EQUAL_TIMER_EVENT(&itemB,&itemC,NO_EVENT,NULL,50);
}

void test_CustomAssert_fail_expected_actual_eventType_different(void){
    initTimerEvent(&itemB,NULL,NO_EVENT,NULL,50);
    TEST_ASSERT_EQUAL_TIMER_EVENT(&itemB,NULL,BUTTON_PRESSED_EVENT,NULL,50);
}

void test_CustomAssert_fail_expected_actual_stateMachine_different(void){
    initTimerEvent(&itemB,NULL,NO_EVENT,&sm,50);
    TEST_ASSERT_EQUAL_TIMER_EVENT(&itemB,NULL,NO_EVENT,&sm2,50);
}

void test_CustomAssert_fail_expected_stateMachine_NULL(void){
    initTimerEvent(&itemB,NULL,NO_EVENT,&sm,50);
    TEST_ASSERT_EQUAL_TIMER_EVENT(&itemB,NULL,NO_EVENT,NULL,50);
}

void test_CustomAssert_fail_actual_stateMachine_NULL(void){
    initTimerEvent(&itemB,NULL,NO_EVENT,NULL,50);
    TEST_ASSERT_EQUAL_TIMER_EVENT(&itemB,NULL,NO_EVENT,&sm,50);
}

void test_CustomAssert_fail_actual_data_NULL(void){
    initTimerEvent(&itemB,NULL,NO_EVENT,NULL,NULL);
    TEST_ASSERT_EQUAL_TIMER_EVENT(&itemB,NULL,NO_EVENT,NULL,50);
}

void test_CustomAssert_fail_expected_data_NULL(void){
    initTimerEvent(&itemB,NULL,NO_EVENT,NULL,50);
    TEST_ASSERT_EQUAL_TIMER_EVENT(&itemB,NULL,NO_EVENT,NULL,NULL);
}

void test_CustomAssert_fail_expected_actual_data_different(void){
    initTimerEvent(&itemB,NULL,NO_EVENT,NULL,50);
    TEST_ASSERT_EQUAL_TIMER_EVENT(&itemB,NULL,NO_EVENT,NULL,40);
}
