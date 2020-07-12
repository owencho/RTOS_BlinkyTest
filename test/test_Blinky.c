#include "unity.h"
#include "mock_ButtonSM.h"
#include "mock_Time.h"
#include "mock_TimerEventQueue.h"
#include "mock_EventQueue.h"
#include "Event.h"
#include "ButtonAndBlinkyQueue.h"
#include "mock_Hardware.h"
#include "mock_Exti.h"
#include "mock_Irq.h"
#include "mock_Button.h"
#include <stdint.h>
#include "StateMachine.h"
#include "CustomAssert.h"
#include "EventType.h"
#include "Blinky.h"
#include "mock_Led.h"
TimerEvent timeEv;
Event evt , buttonEv;
BlinkyStateMachine blinkySM;
ButtonStateMachine  buttonSM;
void setUp(void){}
void tearDown(void){}

void initBlinkySM(BlinkyStateMachine * sm, Callback callback,BlinkyState state,
                  uint32_t isButtonReleased,ButtonStateMachine * button,
                  TimerEvent timerEvent,Event blinkyEvent){
    sm->callback = callback;
    sm->state = state;
    sm->isButtonReleased = isButtonReleased;
    sm->button = button;
    sm->timerEvent = timerEvent;
    sm->blinkyEvent = blinkyEvent;
}

void initEvent(Event * event, Event * next ,EventType type,
               GenericStateMachine * stateMachine,void* data){
    event->next = next;
    event->type = type;
    event->stateMachine = stateMachine;
    event->data = data;
}

void test_blinkyInitStateMachine(void){
    buttonEventRequest_Expect(&blinkySM.blinkyEvent,PRESS);
    blinkyInitStateMachine(&blinkySM,&buttonSM);
    TEST_ASSERT_EQUAL(blinkySM.callback,handleBlinkyStateMachine );
    TEST_ASSERT_EQUAL(blinkySM.state,LED_OFF_BUTTON_RELEASED );
    TEST_ASSERT_EQUAL(blinkySM.isButtonReleased,0 );
    TEST_ASSERT_EQUAL(blinkySM.button,&buttonSM );
    TEST_ASSERT_EQUAL(blinkySM.timerEvent.stateMachine,&blinkySM );
    TEST_ASSERT_EQUAL(blinkySM.blinkyEvent.stateMachine,&blinkySM );
}

void test_blinkyInitStateMachine_LED_OFF_BUTTON_RELEASED_state_remain_as_not_button_press_event(void){
    initEvent(&evt,NULL,BUTTON_RELEASED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    handleBlinkyStateMachine(&evt);
    TEST_ASSERT_EQUAL(blinkySM.state,LED_OFF_BUTTON_RELEASED );
}

void test_blinkyInitStateMachine_LED_OFF_BUTTON_RELEASED_state_change_as_button_press_event(void){
    initEvent(&evt,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    turnLed_Expect(ON);
    buttonEventRequest_Expect(&blinkySM.blinkyEvent,RELEASE);
    handleBlinkyStateMachine(&evt);
    TEST_ASSERT_EQUAL(blinkySM.state,LED_ON );
}

void test_blinkyInitStateMachine_LED_ON_state_remain_as_not_button_press_release_event(void){
    initEvent(&evt,NULL,TIMEOUT_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    initBlinkySM(&blinkySM, (Callback)handleBlinkyStateMachine,LED_ON,0,&buttonSM,timeEv,buttonEv);
    handleBlinkyStateMachine(&evt);
    TEST_ASSERT_EQUAL(blinkySM.state,LED_ON );
}

void test_blinkyInitStateMachine_LED_ON_state_remain_as_isButtonReleased_still0(void){
    initEvent(&evt,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    initBlinkySM(&blinkySM, (Callback)handleBlinkyStateMachine,LED_ON,0,&buttonSM,timeEv,buttonEv);
    handleBlinkyStateMachine(&evt);
    TEST_ASSERT_EQUAL(blinkySM.state,LED_ON );
}

void test_blinkyInitStateMachine_LED_ON_state_remain_as_isButtonReleased_is1(void){
    initEvent(&evt,NULL,BUTTON_RELEASED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    initBlinkySM(&blinkySM, (Callback)handleBlinkyStateMachine,LED_ON,0,&buttonSM,timeEv,buttonEv);
    buttonEventRequest_Expect(&blinkySM.blinkyEvent ,PRESS);
    handleBlinkyStateMachine(&evt);
    TEST_ASSERT_EQUAL(blinkySM.state,LED_ON );
    TEST_ASSERT_EQUAL(blinkySM.isButtonReleased,1 );
}

void test_blinkyInitStateMachine_LED_ON_state_change_to_BLINK_ON(void){
    initEvent(&evt,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    initBlinkySM(&blinkySM, (Callback)handleBlinkyStateMachine,LED_ON,1,&buttonSM,timeEv,buttonEv);
    timerEventRequest_Expect(&buttonBlinkyTimerEventQueue,&blinkySM.timerEvent,100);
    buttonEventRequest_Expect(&blinkySM.blinkyEvent,RELEASE);
    handleBlinkyStateMachine(&evt);
    TEST_ASSERT_EQUAL(blinkySM.state,BLINK_ON );
}

void test_blinkyInitStateMachine_BLINK_ON_state_changes_BLINK_OFF_as_TIMEOUT(void){
    initEvent(&evt,NULL,TIMEOUT_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    initBlinkySM(&blinkySM, (Callback)handleBlinkyStateMachine,BLINK_ON,0,&buttonSM,timeEv,buttonEv);
    timerEventRequest_Expect(&buttonBlinkyTimerEventQueue,&blinkySM.timerEvent,100);
    turnLed_Expect(OFF);
    handleBlinkyStateMachine(&evt);
    TEST_ASSERT_EQUAL(blinkySM.state,BLINK_OFF );
}

void test_blinkyInitStateMachine_BLINK_ON_state_remain_as_isButtonReleased_still0(void){
    initEvent(&evt,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    initBlinkySM(&blinkySM, (Callback)handleBlinkyStateMachine,BLINK_ON,0,&buttonSM,timeEv,buttonEv);
    handleBlinkyStateMachine(&evt);
    TEST_ASSERT_EQUAL(blinkySM.state,BLINK_ON );
}

void test_blinkyInitStateMachine_BLINK_ON_state_remain_as_isButtonReleased_is1(void){
    initEvent(&evt,NULL,BUTTON_RELEASED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    initBlinkySM(&blinkySM, (Callback)handleBlinkyStateMachine,BLINK_ON,0,&buttonSM,timeEv,buttonEv);
    buttonEventRequest_Expect(&blinkySM.blinkyEvent ,PRESS);
    handleBlinkyStateMachine(&evt);
    TEST_ASSERT_EQUAL(blinkySM.state,BLINK_ON );
    TEST_ASSERT_EQUAL(blinkySM.isButtonReleased,1 );
}

void test_blinkyInitStateMachine_BLINK_ON_state_change_to_LED_OFF_BUTTON_HOLD(void){
    initEvent(&evt,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    initBlinkySM(&blinkySM, (Callback)handleBlinkyStateMachine,BLINK_ON,1,&buttonSM,timeEv,buttonEv);
    turnLed_Expect(OFF);
    buttonEventRequest_Expect(&blinkySM.blinkyEvent,RELEASE);
    handleBlinkyStateMachine(&evt);
    TEST_ASSERT_EQUAL(blinkySM.state,LED_OFF_BUTTON_HOLD );
}

void test_blinkyInitStateMachine_BLINK_OFF_state_changes_BLINK_ON_as_TIMEOUT(void){
    initEvent(&evt,NULL,TIMEOUT_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    initBlinkySM(&blinkySM, (Callback)handleBlinkyStateMachine,BLINK_OFF,0,&buttonSM,timeEv,buttonEv);
    timerEventRequest_Expect(&buttonBlinkyTimerEventQueue,&blinkySM.timerEvent,100);
    turnLed_Expect(ON);
    handleBlinkyStateMachine(&evt);
    TEST_ASSERT_EQUAL(blinkySM.state,BLINK_ON );
}

void test_blinkyInitStateMachine_BLINK_OFF_state_remain_as_isButtonReleased_still0(void){
    initEvent(&evt,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    initBlinkySM(&blinkySM, (Callback)handleBlinkyStateMachine,BLINK_OFF,0,&buttonSM,timeEv,buttonEv);
    handleBlinkyStateMachine(&evt);
    TEST_ASSERT_EQUAL(blinkySM.state,BLINK_OFF );
}

void test_blinkyInitStateMachine_BLINK_OFF_state_remain_as_isButtonReleased_is1(void){
    initEvent(&evt,NULL,BUTTON_RELEASED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    initBlinkySM(&blinkySM, (Callback)handleBlinkyStateMachine,BLINK_OFF,0,&buttonSM,timeEv,buttonEv);
    buttonEventRequest_Expect(&blinkySM.blinkyEvent ,PRESS);
    handleBlinkyStateMachine(&evt);
    TEST_ASSERT_EQUAL(blinkySM.state,BLINK_OFF );
    TEST_ASSERT_EQUAL(blinkySM.isButtonReleased,1 );
}

void test_blinkyInitStateMachine_BLINK_OFF_state_change_to_LED_OFF_BUTTON_HOLD(void){
    initEvent(&evt,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    initBlinkySM(&blinkySM, (Callback)handleBlinkyStateMachine,BLINK_OFF,1,&buttonSM,timeEv,buttonEv);
    buttonEventRequest_Expect(&blinkySM.blinkyEvent,RELEASE);
    handleBlinkyStateMachine(&evt);
    TEST_ASSERT_EQUAL(blinkySM.state,LED_OFF_BUTTON_HOLD );
}

void test_blinkyInitStateMachine_LED_OFF_BUTTON_HOLD_no_change(void){
    initEvent(&evt,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    initBlinkySM(&blinkySM, (Callback)handleBlinkyStateMachine,LED_OFF_BUTTON_HOLD,1,&buttonSM,timeEv,buttonEv);
    TEST_ASSERT_EQUAL(blinkySM.state,LED_OFF_BUTTON_HOLD );
}

void test_blinkyInitStateMachine_LED_OFF_BUTTON_HOLD_change_to_LED_OFF_BUTTON_RELEASED(void){
    initEvent(&evt,NULL,BUTTON_RELEASED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    initBlinkySM(&blinkySM, (Callback)handleBlinkyStateMachine,LED_OFF_BUTTON_HOLD,1,&buttonSM,timeEv,buttonEv);
    buttonEventRequest_Expect(&blinkySM.blinkyEvent,PRESS);
    handleBlinkyStateMachine(&evt);
    TEST_ASSERT_EQUAL(blinkySM.state,LED_OFF_BUTTON_RELEASED );
}
