#include "unity.h"
#include "ButtonAndBlinkyQueue.h"
#include "mock_ButtonSM.h"
#include "mock_Time.h"
#include "mock_TimerEvent.h"
#include "mock_EventQueue.h"
#include "mock_Hardware.h"
#include "mock_Irq.h"
#include "mock_Exti.h"
#include "Button.h"
#include "Event.h"
#include <stdint.h>
#include "StateMachine.h"
#include "CustomAssert.h"
#include "EventType.h"
#include "mock_Led.h"
Event evt;
extern Event * buttonEventPtr;
extern EventType expectedButtonState;
void setUp(void){}
void tearDown(void){}

void initEvent(Event * event, Event * next ,EventType type,
               GenericStateMachine * stateMachine,void* data){
    event->next = next;
    event->type = type;
    event->stateMachine = stateMachine;
    event->data = data;
}

void test_convertEventTypeIntoButtonstate_BUTTON_RELEASED_EVENT(void){
    TEST_ASSERT_EQUAL(0,convertEventTypeIntoButtonstate(BUTTON_RELEASED_EVENT));
}

void test_convertEventTypeIntoButtonstate_BUTTON_PRESSED_EVENT(void){
    TEST_ASSERT_EQUAL(1,convertEventTypeIntoButtonstate(BUTTON_PRESSED_EVENT));
}

void test_convertEventTypeIntoButtonstate_TIMEOUT_EVENT(void){
    TEST_ASSERT_EQUAL(2,convertEventTypeIntoButtonstate(TIMEOUT_EVENT));
}

void test_convertEventTypeIntoButtonstate_NO_EVENT(void){
    TEST_ASSERT_EQUAL(2,convertEventTypeIntoButtonstate(NO_EVENT));
}

void test_rawButtonEventRequest_same_event(void){
    extiSetInterruptMaskRegister_Expect(exti,PIN_0,MASKED);
    readPhysicalButton_ExpectAndReturn(0);
    eventEnqueue_Expect(&buttonBlinkyEventQueue,&evt);
    extiSetInterruptMaskRegister_Expect(exti,PIN_0,NOT_MASKED);
    rawButtonEventRequest(&evt , BUTTON_RELEASED_EVENT);
    TEST_ASSERT_NULL(buttonEventPtr);
}

void test_rawButtonEventRequest_different_event(void){
    extiSetInterruptMaskRegister_Expect(exti,PIN_0,MASKED);
    readPhysicalButton_ExpectAndReturn(1);
    extiSetInterruptMaskRegister_Expect(exti,PIN_0,NOT_MASKED);
    rawButtonEventRequest(&evt , BUTTON_RELEASED_EVENT);
    TEST_ASSERT_EQUAL(&evt,buttonEventPtr);
}

void test_rawButtonEventRequest_NULL_event(void){
    rawButtonEventRequest(NULL , BUTTON_RELEASED_EVENT);
    TEST_ASSERT_EQUAL(&evt,buttonEventPtr);
}

void test_buttonEventISR_buttonEventPtr_is_there(void){
    readPhysicalButton_ExpectAndReturn(0);
    eventEnqueue_Expect(&buttonBlinkyEventQueue,&evt);
    buttonEventISR();
    TEST_ASSERT_NULL(buttonEventPtr);
}

void test_buttonEventISR_buttonEventPtr_is_not_there(void){
    buttonEventISR();
    TEST_ASSERT_NULL(buttonEventPtr);
}
