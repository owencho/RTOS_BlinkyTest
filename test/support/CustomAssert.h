#ifndef CustomAssert_H
#define CustomAssert_H
#include "unity.h"
#include "ListItem.h"
#include "../../../Core/Inc/EventType.h"
#include "../../../Core/Inc/StateMachine.h"

void assertEqualListItem (const ListItem * actualListItem,
                          const ListItem * next,
                          const EventType type,
                    	    const GenericStateMachine * stateMachine,
                          const void * data,
                          const UNITY_LINE_TYPE lineNumber);


#define TEST_ASSERT_EQUAL_LIST_ITEM(actualListItem,next,type,stateMachine,data)        \
                      assertEqualListItem(actualListItem,next,type,stateMachine,data,__LINE__)



#endif // CustomAssert_H
