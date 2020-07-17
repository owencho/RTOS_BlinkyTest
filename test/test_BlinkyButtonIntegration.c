#include "unity.h"
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
#include "ButtonSM.h"
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

#define RETURN(x) ((void*)x)
#define NOTHING (NULL)
#define NONE (NULL)

void fakeButtonBlinkySequence(FakeSequence sequence[]){
    turnLed_StubWithCallBack(fake_turnLed);
    buttonEventRequest_StubWithCallBack(fake_buttonEventRequest);
    timerEventRequest_StubWithCallBack(fake_timerEventRequest);
    rawButtonEventRequest_StubWithCallBack(fake_rawButtonEventRequest);
    eventEnqueue_StubWithCallBack(fake_eventEnqueue);
    initFakeButtonBlinkySequence(sequence);
}

void test_blinkyInitStateMachine(void){

    FakeSequence sequenceX[] =
    {expect_turnLed(ON),RETURN(NOTHING)},
    {expect_buttonEventRequest(&event->stateMachine->blinkyEvent,RELEASE),RETURN(NOTHING)},
    {expect_handleButtonStateMachine(&event),RETURN(NOTHING)},



}
