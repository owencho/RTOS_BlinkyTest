#ifndef CustomAssert_H
#define CustomAssert_H
#include "unity.h"
#include "ListItem.h"
#include "EventType.h"
#include "TimerEvent.h"
#include "StateMachine.h"
/*
void assertEqualListItem (const ListItem * actualListItem,
                          const ListItem * next,
                          const EventType type,
                    	    const GenericStateMachine * stateMachine,
                          const void * data,
                          const UNITY_LINE_TYPE lineNumber);
*/
void assertEqualTimerEvent (const TimerEvent * actualListItem,
                            const TimerEvent * next,
                            const EventType type,
                      	    const GenericStateMachine * stateMachine,
                            const int data,
                            const UNITY_LINE_TYPE lineNumber);
/*
#define TEST_ASSERT_EQUAL_LIST_ITEM(actualListItem,next,type,stateMachine,data)        \
                      assertEqualListItem(actualListItem,next,type,stateMachine,data,__LINE__)
*/
#define TEST_ASSERT_EQUAL_TIMER_EVENT(actualListItem,next,type,stateMachine,time)        \
                      assertEqualTimerEvent(actualListItem,next,type,stateMachine,time,__LINE__)



#endif // CustomAssert_H
