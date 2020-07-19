#ifndef FAKEIRQ_H
#define FAKEIRQ_H
#include "unity.h"
#include "Event.h"
#include "EventQueue.h"
#include "TimerEventQueue.h"
#include "TimerEvent.h"
#include "Led.h"
typedef struct FakeInfo FakeInfo;
struct FakeInfo {
		uintptr_t  funcPtr;
    int shouldCallFunc;
		void * inArgs;
};

typedef struct FakeSequence FakeSequence;
struct FakeSequence{
		FakeInfo *fakeInfo;
		uintptr_t retVal;
};

typedef void (*Handler)(void*);
void freeFakeInfo(FakeInfo * info);
void freeExpectedArgs(void * data);
void initFakeBlinkyButtonSequence(FakeSequence sequence[]);
uintptr_t blinkyButtonSequenceStateMachine(uintptr_t funcPtr , void *data);
FakeInfo * createFakeInfo(void* funcName,int shouldCallFunc,int argCount,...);
FakeInfo * call_handleButtonStateMachine(Event * event);
FakeInfo * call_handleBlinkyStateMachine(Event * event);
FakeInfo * expect_turnLed(OnOffState state);
FakeInfo * expect_rawButtonEventRequest(Event * event , EventType state);
FakeInfo * expect_eventEnqueue(EventQueue * queue,Event * event);
FakeInfo * expect_timerEventRequest(TimerEventQueue * timerEventQueue,TimerEvent * event,int expiryPeriod);
int triggerSequence();
void fake_turnLed(OnOffState state, int callNumber);
void fake_rawButtonEventRequest(Event * event , EventType state, int callNumber);
void fake_eventEnqueue(EventQueue * queue,Event * event, int callNumber);
void fake_timerEventRequest(TimerEventQueue * timerEventQueue,TimerEvent * event,int expiryPeriod, int callNumber);
#endif // FAKE_IRQ_H
