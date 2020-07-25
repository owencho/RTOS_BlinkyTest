#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include "unity.h"
#include "Exception.h"
#include "CExceptionConfig.h"
#include "CException.h"
#include "mock_Irq.h"
#include "mock_Led.h"
#include "mock_Time.h"
#include "mock_Hardware.h"
#include "mock_Exti.h"
#include "FakeIRQ.h"
#include "EventQueue.h"
#include "EventType.h"
#include "Event.h"
#include "EventCompare.h"
#include "TimerEventQueue.h"
#include "TimerEvent.h"
#include "TimerEventISR.h"
#include "ButtonAndBlinkyQueue.h"
#include "List.h"
#include "ListItemCompare.h"
#include "Button.h"
#include "StateMachine.h"
#include "CustomAssert.h"
#include "Blinky.h"
#include "ButtonSM.h"
#include "BlinkyButtonIntegration.h"

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
#define NOTHING ((uintptr_t)(NULL))
#define NONE ((uintptr_t)(NULL))

void initFakeSequence(FakeSequence sequence[]){
    turnLed_StubWithCallback(fake_turnLed);
    extiSetInterruptMaskRegister_StubWithCallback(fake_extiSetInterruptMaskRegister);
    readPhysicalButton_StubWithCallback(fake_readPhysicalButton);
    extiSetInterruptMaskRegister_StubWithCallback(fake_extiSetInterruptMaskRegister);
    enableIRQ_StubWithCallback(fake_enableIRQ);
    disableIRQ_StubWithCallback(fake_disableIRQ);
    fakeButtonBlinkySequence(sequence);
}


void test_handleBlinkyStateMachine_BUTTON_PRESSED_EVENT_check_blinky__(void){
  FakeSequence sequence[] ={
    //rawButtonEventRequest from buttonStartStateMachine
    {expect_extiSetInterruptMaskRegister(exti,PIN_0,MASKED),RETURN(NOTHING)},
    {expect_readPhysicalButton(),RETURN(RELEASE)}, //0
    {expect_extiSetInterruptMaskRegister(exti,PIN_0,NOT_MASKED),RETURN(NOTHING)},
    //expect to turnLed off when blinkyStartStateMachine
    {expect_turnLed(OFF),RETURN(NOTHING)},
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
        fakeCheckIRQ(__LINE__);
    }
    Catch(ex){
        dumpException(ex);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
}
