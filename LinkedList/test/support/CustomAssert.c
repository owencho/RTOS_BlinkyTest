#include <stdio.h>
#include "unity.h"
#include "CustomAssert.h"
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <stdarg.h>

#define _STRINGIFY(x)     #x
#define STRINGIFY(x)     _STRINGIFY(x)


void assertEqualListItem (const ListItem * actualListItem,
                          const ListItem * next,
                          const EventType type,
                    	    const GenericStateMachine * stateMachine,
                          const void * data,
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
           sprintf(msg,"the actual ListItem Event type is different with expected next ListItem");
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

    if(actualListItem->data != data){
       if(data == NULL){
           sprintf(msg,"the data in ListItem is not NULL");
       }
       else if(actualListItem->data == NULL){
           sprintf(msg,"the actual data in ListItem is NULL");
       }
       else{
           sprintf(msg,"the actual data in ListItem is different with expected data in ListItem");
       }
       UNITY_TEST_FAIL(lineNumber,msg);
    }
}
