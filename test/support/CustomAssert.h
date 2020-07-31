#ifndef CustomAssert_H
#define CustomAssert_H
#include "unity.h"
#include "ListItem.h"
#include "EventType.h"
#include "TimerEvent.h"
#include "StateMachine.h"

void assertEqualTimerEvent (const TimerEvent * actualListItem,
                            const TimerEvent * next,
                            const EventType type,
                      	    const GenericStateMachine * stateMachine,
                            const int data,
                            const UNITY_LINE_TYPE lineNumber);

#define TEST_ASSERT_EQUAL_TIMER_EVENT(actualListItem,next,type,stateMachine,time)        \
                      assertEqualTimerEvent(actualListItem,next,type,stateMachine,time,__LINE__)

void testReportFailure (char* message ,...);
#endif // CustomAssert_H
