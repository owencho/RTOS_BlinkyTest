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
TimerEvent timeEv;
Event evt ,evt1,evt2,evt3, buttonEv;
BlinkyStateMachine blinkySM;
ButtonStateMachine  buttonSM;
CEXCEPTION_T ex;
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

#define RETURN(x) ((uintptr_t)(x))
#define NOTHING (NULL)
#define NONE (NULL)

void fakeButtonBlinkySequence(FakeSequence sequence[]){
    turnLed_StubWithCallback(fake_turnLed);
    timerEventRequest_StubWithCallback(fake_timerEventRequest);
    rawButtonEventRequest_StubWithCallback(fake_rawButtonEventRequest);
    eventEnqueue_StubWithCallback(fake_eventEnqueue);
    initFakeBlinkyButtonSequence(sequence);
}

void test_handleBlinkyStateMachine_BUTTON_PRESSED_EVENT_check_blinky(void){

    initEvent(&evt1,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&buttonSM,NULL);
    initEvent(&evt2,NULL,TIMEOUT_EVENT,(GenericStateMachine *)&buttonSM,NULL);
    initEvent(&evt3,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine *)&blinkySM,NULL);
    FakeSequence sequenceX[] ={
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
    fakeButtonBlinkySequence(sequenceX);
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
