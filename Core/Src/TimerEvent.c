/*
 * TimerEvent.c
 *
 *  Created on: Jun 25, 2020
 *      Author: academic
 */
#include "TimerEvent.h"
#include "Event.h"
#include "List.h"
#include "Interrupt.h"
#include "LinkedListCompare.h"
#include "EventCompare.h"
List * eventTimerQueueList;
ListItem * eventTimerItem;
ListItem * currentEventTimerItem;
Event * timeEventPtr;
int currentTick,calculatedTime,nextCalculatedTime;
void initEventTimerQueue(){
	eventTimerQueueList = createList();
}
void timerEventStart (Event * event,uint32_t expiryPeriod){
	timeEventPtr = event;
	if(eventTimerQueueList->count == 0){
		resetTick();
		timeEventPtr->data = &expiryPeriod;
		eventTimerEnqueue(timeEventPtr);
	}
	else{
		disableIRQ();
		expiryPeriod = expiryPeriod-currentTick;
		eventTimerItem = findListItem(eventTimerQueueList,&expiryPeriod,(LinkedListCompare)eventCompareForTime);
		calculatedTime = currentTick + expiryPeriod - (int)(event->data);
		timeEventPtr->data = (void*)&calculatedTime;
		eventTimerQueueList=listAddItemToNext(eventTimerQueueList, eventTimerItem,(void*)timeEventPtr);

		*(uint32_t*)eventTimerItem->data= *(uint32_t*)(eventTimerItem->data) - calculatedTime;
	    enableIRQ();
	}
}

void incTick(){
	if(eventTimerQueueList->count != 0)
		currentTick++;
}

void resetTick(){
	disableIRQ();
	currentTick = 0;
	enableIRQ();
}
void eventTimerEnqueue(Event * event){
	disableIRQ();
    listAddItemToTail(eventTimerQueueList, (void *) event);
    enableIRQ();
}

void eventTimerDequeue(){
		disableIRQ();
	  	//disable interrupt put here to protect data from race condition
	    if(eventTimerQueueList->count ==0){
	    	enableIRQ();
	      	return ;
	    }
	    resetCurrentListItem(eventTimerQueueList);
	    deleteHeadListItem(eventTimerQueueList);
	    enableIRQ();
	    return ;
}

void timerEventISR(){
	Event * eventFromQueue;
	uint32_t * timePtr;
    if(timeEventPtr != NULL){
    	//reset linkedList
    	resetCurrentListItem(eventTimerQueueList);
    	// get the timevalue head item
    	currentEventTimerItem=getCurrentListItem(eventTimerQueueList);
    	eventFromQueue = currentEventTimerItem->data;
    	timePtr = (uint32_t*)eventFromQueue->data;
      	if(*timePtr == currentTick){
      		timeEventPtr = currentEventTimerItem->data;
      		timeEventPtr->type = TIMEOUT_EVENT;
      		eventEnqueue(timeEventPtr);
      		eventTimerDequeue();
      		resetTick();
      	}


        //buttonEventPtr = NULL;
    }
}

