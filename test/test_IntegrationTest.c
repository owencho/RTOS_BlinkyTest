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
#include "ExceptionTestSupport.h"

TimerEvent timeEv;
Event evt ,evt1,evt2,evt3, buttonEv;
BlinkyStateMachine blinkySM;
ButtonStateMachine  buttonSM;
extern EventQueue buttonBlinkyEventQueue;
extern TimerEventQueue buttonBlinkyTimerEventQueue;
CEXCEPTION_T ex;
void clearEventQueue(EventQueue* queue , TimerEventQueue *timerQueue);
void setUp(void){}
void tearDown(void){
    clearEventQueue(&buttonBlinkyEventQueue,&buttonBlinkyTimerEventQueue);
}

void setUpBlinkySM(BlinkyStateMachine * sm, Callback callback,BlinkyState state,
                  uint32_t isButtonReleased,ButtonStateMachine * button){
    sm->callback = callback;
    sm->state = state;
    sm->isButtonReleased = isButtonReleased;
    sm->button = button;
    sm->blinkyEvent.stateMachine= (GenericStateMachine *)sm;
    sm->timerEvent.stateMachine = (GenericStateMachine *)sm;
}

void setUpButtonSM(ButtonStateMachine * sm, Callback callback,ButtonState state,
              PressReleaseState buttonStatus){
    sm->callback = callback;
    sm->state = state;
    sm->buttonStatus = buttonStatus;
    sm->buttonEvent.stateMachine =(GenericStateMachine *)sm;
    sm->timerEvent.stateMachine = (GenericStateMachine *)sm;
}
void setUpEvent(Event * event, Event * next ,EventType type,
                GenericStateMachine * stateMachine,void* data){
    event->next = next;
    event->type = type;
    event->stateMachine = stateMachine;
    event->data = data;
}

void clearEventQueue(EventQueue* queue , TimerEventQueue *timerQueue){
    queue->head = NULL;
    queue->tail = NULL;
    queue->count = 0;
    queue->previous = NULL;
    queue->current = NULL;
    timerQueue->head = NULL;
    timerQueue->tail = NULL;
    timerQueue->count = 0;
    timerQueue->previous = NULL;
    timerQueue->current = NULL;
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
// this test is to test BUTTON RELEASE EVENT FOR LED_ON to BLINK_ON
// when the button was pressed
void test_handleBlinkyStateMachine_from_LED_ON_to_BLINK_ON(void){
    setUpBlinkySM(&blinkySM, (Callback)handleBlinkyStateMachine,LED_ON,0,&buttonSM);
    setUpButtonSM(&buttonSM, (Callback)handleButtonStateMachine,BUTTON_RELEASED,RELEASE);
    setUpEvent(&evt,NULL,BUTTON_RELEASED_EVENT,(GenericStateMachine*)&blinkySM,NULL);
    setUpEvent(&evt2,NULL,TIMEOUT_EVENT,(GenericStateMachine*)&buttonSM,NULL);
    FakeSequence sequence[] ={
      //simulate the rawButtonEventRequest to press
      {call_handleBlinkyStateMachine(&evt)},
      {simulate_rawButtonEventRequest(&evt2,PRESS)},
      //rawButtonEventRequest is requested on handleButtonStateMachine
      {expect_readPhysicalButton(),RETURN(RELEASE)},
      // mockup where the event just
      // now trying to simulate the buttonInterrupt
      {simulate_buttonInterrupt(),NONE},
      {expect_readPhysicalButton(),RETURN(PRESS)},
      // where at this point the event was queued into the normal queue
      // to call the button stateMachine
      {simulate_mainExecutiveLoop(),NONE},
      // at this point the timerEventRequest was called to queue into the timerEventQueue
      //set timerTick (this will call TimerEventISR)
      {simulate_setTimerTick(101),NONE},
      //after event is dequeue from the timerEventQueue
      //expected button event is called again for BUTTON_PRESSED_DEBOUNCING with TIMEOUT_EVENT
      {simulate_mainExecutiveLoop(),NONE},
      //button Event is enqueued into the eventQueue and rawButtonEventRequest is called
      {expect_readPhysicalButton(),RETURN(PRESS)},
      // and now the current state of Button stateMachine is BUTTON_PRESSED
      // main executive loop is run again
      {simulate_mainExecutiveLoop(),NONE},
      // blinkyEvent was dequeued and handleBlinkyStateMachine was called
      // the led still turn ON as it goes from BLINK_ON to LED_ON
      {NULL,RETURN(NOTHING)}
      };
      initFakeSequence(sequence);
      extiSetInterruptMaskRegister_StubWithCallback(fake_doNothing);
      Try{
          while(triggerSequence()){

          }
          fakeCheckIRQ(__LINE__);
      }
      Catch(ex){
          dumpException(ex);
          TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
      }
}

// this test is to test BUTTON PRESSED EVENT FOR BLINK_ON to LED_OFF
// when the button was pressed
void test_handleBlinkyStateMachine_from_BLINK_ON_to_LED_OFF(void){
  setUpBlinkySM(&blinkySM, (Callback)handleBlinkyStateMachine,BLINK_ON,0,&buttonSM);
  setUpButtonSM(&buttonSM, (Callback)handleButtonStateMachine,BUTTON_RELEASED,RELEASE);
  setUpEvent(&evt,NULL,BUTTON_RELEASED_EVENT,(GenericStateMachine*)&blinkySM,NULL);
  setUpEvent(&evt2,NULL,TIMEOUT_EVENT,(GenericStateMachine*)&buttonSM,NULL);
  FakeSequence sequence[] ={
    //simulate the rawButtonEventRequest to press
    {call_handleBlinkyStateMachine(&evt)},
    {simulate_rawButtonEventRequest(&evt2,PRESS)},
    //rawButtonEventRequest is requested on handleButtonStateMachine
    {expect_readPhysicalButton(),RETURN(RELEASE)},
    // mockup where the event just
    // now trying to simulate the buttonInterrupt
    {simulate_buttonInterrupt(),NONE},
    {expect_readPhysicalButton(),RETURN(PRESS)},
    // where at this point the event was queued into the normal queue
    // to call the button stateMachine
    {simulate_mainExecutiveLoop(),NONE},
    // at this point the timerEventRequest was called to queue into the timerEventQueue
    //set timerTick (this will call TimerEventISR)
    {simulate_setTimerTick(101),NONE},
    //after event is dequeue from the timerEventQueue
    //expected button event is called again for BUTTON_PRESSED_DEBOUNCING with TIMEOUT_EVENT
    {simulate_mainExecutiveLoop(),NONE},
    //button Event is enqueued into the eventQueue and rawButtonEventRequest is called
    {expect_readPhysicalButton(),RETURN(PRESS)},
    // and now the current state of Button stateMachine is BUTTON_PRESSED
    // main executive loop is run again
    {simulate_mainExecutiveLoop(),NONE},
    // blinkyEvent was dequeued and handleBlinkyStateMachine was called
    // the led will turn OFF as it goes from BLINK_ON to LED_OFF
    {expect_turnLed(OFF),RETURN(NOTHING)},
    {NULL,RETURN(NOTHING)}
    };
    initFakeSequence(sequence);
    extiSetInterruptMaskRegister_StubWithCallback(fake_doNothing);
    Try{
        while(triggerSequence()){

        }
        fakeCheckIRQ(__LINE__);
    }
    Catch(ex){
        dumpException(ex);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
}

// this test is to test TIMEOUT_EVENT FOR BLINK_ON to BLINK_OFF
void test_handleBlinkyStateMachine_from_BLINK_ON_to_BLINK_OFF(void){
  setUpBlinkySM(&blinkySM, (Callback)handleBlinkyStateMachine,LED_ON,1,&buttonSM);
  setUpButtonSM(&buttonSM, (Callback)handleButtonStateMachine,BUTTON_RELEASED,RELEASE);
  setUpEvent(&evt,NULL,BUTTON_PRESSED_EVENT,(GenericStateMachine*)&blinkySM,NULL);
  setUpEvent(&evt2,NULL,TIMEOUT_EVENT,(GenericStateMachine*)&buttonSM,NULL);
  FakeSequence sequence[] ={
    //simulate the rawButtonEventRequest to press
    {call_handleBlinkyStateMachine(&evt)},
    // simulate the main executive loop see is there any event to be called
    {simulate_mainExecutiveLoop(),NONE},
    // expected to have no event as timer is not expired yet
    {simulate_setTimerTick(30),NONE},
    // event in timerEventQueue are still remain inside as it is not expired yet
    {simulate_mainExecutiveLoop(),NONE},
    // we set timerTick again to expire the timerEvent
    {simulate_setTimerTick(100),NONE},
    // event in timerEventQueue are dequeued and it is queued into main EventQueue
    {simulate_mainExecutiveLoop(),NONE},
    // handleBlinkyStateMachine will be called in this Sequence
    // timerEventRequest is called again in the stateMachine as the stateMachine is transitioning
    // into blink_OFF where it also required timer to calculate the clock.
    // and there we expect the Led to turn off as it is BlinkOff
    {expect_turnLed(OFF),RETURN(NOTHING)},
    {NULL,RETURN(NOTHING)}
    };
    initFakeSequence(sequence);
    extiSetInterruptMaskRegister_StubWithCallback(fake_doNothing);
    Try{
        while(triggerSequence()){

        }
        fakeCheckIRQ(__LINE__);
    }
    Catch(ex){
        dumpException(ex);
        TEST_FAIL_MESSAGE("Do not expect any exception to be thrown");
    }
}
