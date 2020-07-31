#include <stdio.h>
#include "unity.h"
#include "CustomAssert.h"
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <stdarg.h>

#define _STRINGIFY(x)     #x
#define STRINGIFY(x)     _STRINGIFY(x)

void assertEqualTimerEvent (const TimerEvent * actualListItem,
                            const TimerEvent * next,
                            const EventType type,
                      	    const GenericStateMachine * stateMachine,
                            const int time,
                            const UNITY_LINE_TYPE lineNumber){

    char msg[1024];
    int length;
    int i;

    if(actualListItem == NULL){
         UNITY_TEST_FAIL(lineNumber,"The actual listItem is NULL");
     }
    if(actualListItem->next != next){
       if(next == NULL){
           sprintf(msg,"the next ListItem is not NULL");
       }
       else if(actualListItem->next == NULL){
           sprintf(msg,"the actual next ListItem is NULL ");
       }
       else{
           sprintf(msg,"the next ListItem is different with expected next ListItem");
       }
       UNITY_TEST_FAIL(lineNumber,msg);
    }

    if(actualListItem->type != type){
           sprintf(msg,"the actual ListItem Event type is different with expected listItem Type");
       UNITY_TEST_FAIL(lineNumber,msg);
    }

    if(actualListItem->stateMachine != stateMachine){
       if(stateMachine == NULL){
           sprintf(msg,"the expected stateMachine in ListItem is not NULL");
       }
       else if(actualListItem->stateMachine == NULL){
           sprintf(msg,"the actual stateMachine in ListItem is NULL");
       }
       else{
           sprintf(msg,"the actual stateMachine in ListItem is different with expected stateMachine in ListItem");
       }
       UNITY_TEST_FAIL(lineNumber,msg);
    }

    if(actualListItem->time != time){
        sprintf(msg,"the actual time (%d) is different with expected time (%d)",actualListItem->time ,time);
        UNITY_TEST_FAIL(lineNumber,msg);
    }

}

void testReportFailure (char* message ,...){
    int actualLength;
    char* buffer;
    va_list arg;
    va_start(arg, message);
    actualLength = vsnprintf(NULL,0, message, arg);   //trick system to take actualLength
    buffer =malloc(actualLength + 1);               // allocate value to buffer
    vsnprintf(buffer,actualLength + 1, message, arg);
    va_end(arg);
    UNITY_TEST_FAIL(__LINE__, buffer);
}
