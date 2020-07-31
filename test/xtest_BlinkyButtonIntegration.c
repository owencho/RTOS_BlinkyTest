#include "unity.h"
#include "mock_Time.h"
#include "mock_TimerEventQueue.h"
#include "mock_EventQueue.h"
#include "Event.h"
#include "ButtonAndBlinkyQueue.h"
#include "Exception.h"
#include "CExceptionConfig.h"
#include "CException.h"
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
#include "BlinkyButtonIntegration.h"
// This is backup test for the pulling based integration test
TimerEvent timeEv;
Event evt ,evt1,evt2,evt3, buttonEv;
BlinkyStateMachine blinkySM;
ButtonStateMachine  buttonSM;
CEXCEPTION_T ex;
void setUp(void){}
void tearDown(void){}

void setUpBlinkySM(BlinkyStateMachine * sm, Callback callback,BlinkyState state,
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

#define RETURN(x) ((uintptr_t)(x))
#define NOTHING (NULL)
#define NONE (NULL)

void initFakeSequence(FakeSequence sequence[]){
    turnLed_StubWithCallback(fake_turnLed);
    extiSetInterruptMaskRegister_StubWithCallback(fake_extiSetInterruptMaskRegister);
    readPhysicalButton_StubWithCallback(fake_readPhysicalButton);
  //  timerEventRequest_StubWithCallback(fake_timerEventRequest);
    //rawButtonEventRequest_StubWithCallback(fake_rawButtonEventRequest);
  //  eventEnqueue_StubWithCallback(fake_eventEnqueue);
    fakeButtonBlinkySequence(sequence);
}

void test_handleBlinkyStateMachine_BUTTON_PRESSED_EVENT_check_blinky(void){

    initEvent(&evt1,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&buttonSM,NULL);
    initEvent(&evt2,NULL,TIMEOUT_EVENT,(GenericStateMachine *)&buttonSM,NULL);
    initEvent(&evt3,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    FakeSequence sequence[] ={
    {expect_rawButtonEventRequest(&buttonSM.buttonEvent,PRESS),RETURN(NOTHING)}, //0
    {call_handleButtonStateMachine(&evt1),RETURN(NOTHING)}, //1
    // timer for button debouncing
    {expect_timerEventRequest(&buttonBlinkyTimerEventQueue,&buttonSM.timerEvent,100),RETURN(NOTHING)}, //2
    //when timer expired and blinkyEvent get queued into eventQueue
    {call_handleButtonStateMachine(&evt2),RETURN(NOTHING)}, //3
    {expect_eventEnqueue(&buttonBlinkyEventQueue,&blinkySM.blinkyEvent),RETURN(NOTHING)}, //4
    {expect_rawButtonEventRequest(&evt2,BUTTON_RELEASED_EVENT),RETURN(NOTHING)}, //5
    // blinky was called from the event Queue
    {call_handleBlinkyStateMachine(&evt3),RETURN(NOTHING)}, //6
    {expect_turnLed(ON),RETURN(NOTHING)}, //seq7
    {NULL,RETURN(NOTHING)} //end of sequence
    };
    initFakeSequence(sequence);
    Try{
        buttonInitStateMachine(&buttonSM);
        blinkyInitStateMachine(&blinkySM,&buttonSM);

        while(triggerSequence()){

        }

    }
    Catch(ex){
        dumpException(ex);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
}

void test_handleBlinkyStateMachine_BUTTON_PRESSED_EVENT_check_blinky__(void){
  FakeSequence sequence[] ={
    //rawButtonEventRequest from buttonStartStateMachine
    {expect_extiSetInterruptMaskRegister(exti,PIN_0,MASKED),RETURN(NOTHING)},
    {expect_readPhysicalButton(),RETURN(RELEASE)}, //0
    {expect_extiSetInterruptMaskRegister(exti,PIN_0,NOT_MASKED),RETURN(NOTHING)},
    //button interrupt
    {simulate_buttonInterrupt(),NONE},
    {expect_readPhysicalButton(),RETURN(PRESS)},
    //expect the button stateMachine to be called
    {simulate_mainExecutiveLoop(),NONE},
    //BUTTON_PRESSED_EVENT
    //set timerTick (this will call TimerEventISR)
    {simulate_setTimerTick(101),NONE},
    //after event is dequeue from the timerEventQueue
    {simulate_mainExecutiveLoop(),NONE},
    //TIMEOUT_EVENT
    // called into handleButtonStateMachine
    //rawButtonEventRequest from handleButtonStateMachine
    {expect_extiSetInterruptMaskRegister(exti,PIN_0,MASKED),RETURN(NOTHING)},
    {expect_readPhysicalButton(),RETURN(PRESS)},
    {expect_extiSetInterruptMaskRegister(exti,PIN_0,NOT_MASKED),RETURN(NOTHING)},
    //after handleButtonStateMachine is called
    //button interrupt
    {simulate_buttonInterrupt(),NONE},
    {expect_readPhysicalButton(),RETURN(PRESS)},
    //
    {simulate_mainExecutiveLoop(),NONE},
    //BUTTON_PRESSED_EVENT
    //handleBlinkyStateMachine is called
    {expect_turnLed(ON),RETURN(NOTHING)},
    //end of sequence
    {NULL,RETURN(NOTHING)}
    };
    initFakeSequence(sequence);
    Try{
        buttonInitStateMachine(&buttonSM);
        buttonStartStateMachine(&buttonSM);
        blinkyInitStateMachine(&blinkySM,&buttonSM);
        blinkyStartStateMachine(&blinkySM);

        while(triggerSequence()){

        }

    }
    Catch(ex){
        dumpException(ex);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
}
