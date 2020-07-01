/*
 * TimerEvent.c
 *
 *  Created on: Jun 25, 2020
 *      Author: academic
 */
#include "TimerEvent.h"
#include "Event.h"
#include "List.h"
#include "Hardware.h"
#include "LinkedListCompare.h"
#include "EventCompare.h"

uint32_t  expiryTime;
List * eventTimerQueueList;
ListItem * eventTimerItem;
ListItem * currentEventTimerItem;
Event * timeEventPtr;
int currentTick,calculatedTime,nextCalculatedTime;

void initEventTimerQueue(){
		eventTimerQueueList = createList();
}

void timerEventRequest (TimerEvent * event,uint32_t expiryPeriod){
		timeEventPtr = event;
		event->expiryPeriod = expiryPeriod;
		if(eventTimerQueueList== NULL || event == NULL)
				return ;
		if(eventTimerQueueList->count == 0){
				resetTick();
				expiryTime = expiryPeriod;
				timeEventPtr->data = &expiryTime;
				eventTimerEnqueue(timeEventPtr);
		}
		else{
			/*
			disableIRQ();
			expiryPeriod = expiryPeriod-currentTick;
			eventTimerItem = findListItem(eventTimerQueueList,&expiryPeriod,(LinkedListCompare)eventCompareForTime);
			calculatedTime = currentTick + expiryPeriod - (int)(event->data);
			timeEventPtr->data = (void*)&calculatedTime;
			eventTimerQueueList=listAddItemToNext(eventTimerQueueList, eventTimerItem,(void*)timeEventPtr);

			*(uint32_t*)eventTimerItem->data= *(uint32_t*)(eventTimerItem->data) - calculatedTime;
		    enableIRQ();
		    */
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
		if(eventTimerQueueList== NULL || event == NULL){
				enableIRQ();
				return ;
		}
    listAddItemToTail(eventTimerQueueList, (void *) event);
		enableIRQ();
}

void eventTimerDequeue(){
		disableIRQ();
  	//disable interrupt put here to protect data from race condition
		if(eventTimerQueueList== NULL){
				enableIRQ();
				return ;
		}
    else if(eventTimerQueueList->count ==0){
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
		disableIRQ();
		if(eventTimerQueueList== NULL){
				enableIRQ();
				return ;
		}
    else if(eventTimerQueueList->count !=0){
				//timeEventPtr != NULL &&
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
			 enableIRQ();
    }
}
