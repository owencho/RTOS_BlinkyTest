#include "unity.h"
#include "ButtonSM.h"
#include "mock_Time.h"
#include "mock_TimerEvent.h"
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
ButtonStateMachine buttonSM;
BlinkyStateMachine blinkySM;
Event evt ,timerEvent,buttonEvent;
Event evtForBlinky;
void setUp(void){}
void tearDown(void){}
void initButtonSM(ButtonStateMachine * sm, Callback callback,ButtonState state,
              PressReleaseState buttonStatus,Event timerEvent,Event buttonEvent){
    sm->callback = callback;
    sm->state = state;
    sm->buttonStatus = buttonStatus;
    sm->timerEvent = timerEvent;
    sm->buttonEvent = buttonEvent;
}

void initEvent(Event * event, ListItem * next ,EventType type,
               GenericStateMachine * stateMachine,void* data){
    event->next = next;
    event->type = type;
    event->stateMachine = stateMachine;
    event->data = data;
}
void test_ButtonInitStateMachine(void){
    rawButtonEventRequest_Expect(&buttonSM.buttonEvent,PRESS);
    buttonInitStateMachine(&buttonSM);
    TEST_ASSERT_EQUAL(buttonSM.callback,handleButtonStateMachine );
    TEST_ASSERT_EQUAL(buttonSM.state,BUTTON_RELEASED );
    TEST_ASSERT_EQUAL(buttonSM.buttonStatus,RELEASE );
    TEST_ASSERT_EQUAL(buttonSM.timerEvent.stateMachine,&buttonSM );
    TEST_ASSERT_EQUAL(buttonSM.buttonEvent.stateMachine,&buttonSM );
}
//release state
//Button pressed event on button released_state
// when button is not requested from blinky
void test_ButtonInitStateMachine_released_state_without_request(void){
    rawButtonEventRequest_Expect(&buttonSM.buttonEvent,PRESS);
    buttonInitStateMachine(&buttonSM);
    initEvent(&evt,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&buttonSM,NULL);

    handleButtonStateMachine(&evt);
    TEST_ASSERT_EQUAL(buttonSM.state,BUTTON_RELEASED);
}

void test_ButtonInitStateMachine_released_state_with_request_but_different_event(void){
    rawButtonEventRequest_Expect(&buttonSM.buttonEvent,PRESS);
    buttonInitStateMachine(&buttonSM);
    initEvent(&evt,NULL,TIMEOUT_EVENT,(GenericStateMachine *)&buttonSM,NULL);
    buttonEventRequest(&evtForBlinky,PRESS);

    handleButtonStateMachine(&evt);
    TEST_ASSERT_EQUAL(buttonSM.state,BUTTON_RELEASED);
}

void test_ButtonInitStateMachine_released_state_with_request_from_blinky(void){
    rawButtonEventRequest_Expect(&buttonSM.buttonEvent,PRESS);
    buttonInitStateMachine(&buttonSM);
    initEvent(&evt,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&buttonSM,NULL);
    initEvent(&evtForBlinky,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    buttonEventRequest(&evtForBlinky,PRESS);

    timerEventStart_Expect(&buttonSM.timerEvent,100);
    handleButtonStateMachine(&evt);
    TEST_ASSERT_EQUAL(buttonSM.state,BUTTON_PRESSED_DEBOUNCING);
}

void test_ButtonInitStateMachine_BUTTON_PRESSED_DEBOUNCING_state(void){
    initButtonSM(&buttonSM, (Callback)handleButtonStateMachine
                ,BUTTON_PRESSED_DEBOUNCING,PRESS,timerEvent,buttonEvent);
    initEvent(&evt,NULL,TIMEOUT_EVENT,(GenericStateMachine *)&buttonSM,NULL);
    initEvent(&evtForBlinky,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    buttonEventRequest(&evtForBlinky,PRESS);

    eventEnqueue_Expect(&evtForBlinky);
    rawButtonEventRequest_Expect(&evtForBlinky,BUTTON_RELEASED_EVENT);
    handleButtonStateMachine(&evt);
    TEST_ASSERT_EQUAL(buttonSM.state,BUTTON_PRESSED);
}

void test_ButtonInitStateMachine_BUTTON_PRESSED_DEBOUNCING_not_timeout_state(void){
    initButtonSM(&buttonSM, (Callback)handleButtonStateMachine
                ,BUTTON_PRESSED_DEBOUNCING,PRESS,timerEvent,buttonEvent);
    initEvent(&evt,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&buttonSM,NULL);
    initEvent(&evtForBlinky,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    buttonEventRequest(&evtForBlinky,PRESS);

    handleButtonStateMachine(&evt);
    TEST_ASSERT_EQUAL(buttonSM.state,BUTTON_PRESSED_DEBOUNCING);
}

void test_ButtonInitStateMachine_BUTTON_PRESSED_but_not_requested_by_blinky(void){
    initButtonSM(&buttonSM, (Callback)handleButtonStateMachine
                ,BUTTON_PRESSED,PRESS,timerEvent,buttonEvent);
    initEvent(&evt,NULL,BUTTON_RELEASED_EVENT,(GenericStateMachine *)&buttonSM,NULL);
    initEvent(&evtForBlinky,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);

    handleButtonStateMachine(&evt);
    TEST_ASSERT_EQUAL(buttonSM.state,BUTTON_PRESSED);
}

void test_ButtonInitStateMachine_BUTTON_PRESSED_requested_by_blinky_but_not_released_event(void){
    initButtonSM(&buttonSM, (Callback)handleButtonStateMachine
                ,BUTTON_PRESSED,PRESS,timerEvent,buttonEvent);
    initEvent(&evt,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&buttonSM,NULL);
    initEvent(&evtForBlinky,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    buttonEventRequest(&evtForBlinky,RELEASE);

    handleButtonStateMachine(&evt);
    TEST_ASSERT_EQUAL(buttonSM.state,BUTTON_PRESSED);
}

void test_ButtonInitStateMachine_BUTTON_PRESSED_requested_by_blinky_but_different_request_from_blinky(void){
    initButtonSM(&buttonSM, (Callback)handleButtonStateMachine
                ,BUTTON_PRESSED,PRESS,timerEvent,buttonEvent);
    initEvent(&evt,NULL,BUTTON_RELEASED_EVENT,(GenericStateMachine *)&buttonSM,NULL);
    initEvent(&evtForBlinky,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    buttonEventRequest(&evtForBlinky,PRESS);

    handleButtonStateMachine(&evt);
    TEST_ASSERT_EQUAL(buttonSM.state,BUTTON_PRESSED);
}

void test_ButtonInitStateMachine_BUTTON_PRESSED_requested_by_blinky(void){
    initButtonSM(&buttonSM, (Callback)handleButtonStateMachine
                ,BUTTON_PRESSED,PRESS,timerEvent,buttonEvent);
    initEvent(&evt,NULL,BUTTON_RELEASED_EVENT,(GenericStateMachine *)&buttonSM,NULL);
    initEvent(&evtForBlinky,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    buttonEventRequest(&evtForBlinky,RELEASE);

    timerEventStart_Expect(&buttonSM.timerEvent,100);
    handleButtonStateMachine(&evt);
    TEST_ASSERT_EQUAL(buttonSM.state,BUTTON_RELEASED_DEBOUNCING);
}

void test_ButtonInitStateMachine_BUTTON_RELEASED_DEBOUNCING_state(void){
    initButtonSM(&buttonSM, (Callback)handleButtonStateMachine
                ,BUTTON_RELEASED_DEBOUNCING,PRESS,timerEvent,buttonEvent);
    initEvent(&evt,NULL,TIMEOUT_EVENT,(GenericStateMachine *)&buttonSM,NULL);
    initEvent(&evtForBlinky,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    buttonEventRequest(&evtForBlinky,PRESS);

    eventEnqueue_Expect(&evtForBlinky);
    rawButtonEventRequest_Expect(&evtForBlinky,BUTTON_PRESSED_EVENT);
    handleButtonStateMachine(&evt);
    TEST_ASSERT_EQUAL(buttonSM.state,BUTTON_RELEASED);
}

void test_ButtonInitStateMachine_BBUTTON_RELEASED_DEBOUNCING_not_timeout_state(void){
    initButtonSM(&buttonSM, (Callback)handleButtonStateMachine
                ,BUTTON_RELEASED_DEBOUNCING,PRESS,timerEvent,buttonEvent);
    initEvent(&evt,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&buttonSM,NULL);
    initEvent(&evtForBlinky,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    buttonEventRequest(&evtForBlinky,PRESS);

    handleButtonStateMachine(&evt);
    TEST_ASSERT_EQUAL(buttonSM.state,BUTTON_RELEASED_DEBOUNCING);
}
