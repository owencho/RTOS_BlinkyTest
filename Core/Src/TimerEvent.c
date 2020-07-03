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

List  timerEventQueueList;
TimerEvent * timerEventItem;
TimerEvent * currentTimerEventItem;
int totalTimeDelay,currentTick;

void incTick(){
		if(timerEventQueueList.count != 0)
				currentTick++;
}

void resetTick(){
		currentTick = 0;
}

void checkAndAddTimerEvent(TimerEvent * event){
		if(event== NULL)
				return;
		event->time = event->time + currentTick;
		resetCurrentListItem(&timerEventQueueList);
		currentTimerEventItem =(TimerEvent*) getCurrentListItem(&timerEventQueueList);
		if(currentTimerEventItem == NULL){
				return;
		}
		if(currentTimerEventItem->time > event->time){
				currentTimerEventItem->time = currentTimerEventItem->time - event->time;
				listAddItemToHead(&timerEventQueueList,(ListItem*)event);
		}
		else{
				timerEventItem = (TimerEvent*)findListItem(&timerEventQueueList,event,(LinkedListCompare)eventCompareForTime);
				// new time delay for next Time Event is old time delay - previous delay time
				if(timerEventItem->next != NULL)
						timerEventItem->next->time = timerEventItem->next->time - event->time ;

				listAddItemToNext(&timerEventQueueList,(ListItem*)timerEventItem,(ListItem*)event);
		}
}
void timerEventDequeue(){
		disableIRQ();
  	//disable interrupt put here to protect data from race condition
 		if(timerEventQueueList.count ==0){
    		enableIRQ();
      	return ;
    }
    resetCurrentListItem(&timerEventQueueList);
    deleteHeadListItem(&timerEventQueueList);
    enableIRQ();
    return ;
}

void timerEventRequest (TimerEvent * event,int expiryPeriod){
		if(event == NULL)
				return ;
		event->time =expiryPeriod ;
		timerEventEnqueue(event);
}

void timerEventEnqueue(TimerEvent * event){
		disableIRQ();
		if(event == NULL){
				enableIRQ();
				return ;
		}
		if(timerEventQueueList.count == 0){
				resetTick();
				listAddItemToTail(&timerEventQueueList,(ListItem*)event);
		}
		else{
				checkAndAddTimerEvent(event);
		}
		enableIRQ();
}

void checkNextEventTimerIsZero(List * timerEventQueue){
		TimerEvent * nextTimerEventItem;
		nextTimerEventItem=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
		while(nextTimerEventItem != NULL){
				if(currentTimerEventItem->time == 0){
						nextTimerEventItem->type = TIMEOUT_EVENT;
						eventEnqueue((Event*)nextTimerEventItem);
						timerEventDequeue();
						nextTimerEventItem=(TimerEvent*)getNextListItem(&timerEventQueueList);
				}
		}
}

void timerEventISR(){
		TimerEvent * currentTimerEventItem;
		disableIRQ();
 		if(timerEventQueueList.count !=0){
				//timeEventPtr != NULL &&
				//reset linkedList
	    	resetCurrentListItem(&timerEventQueueList);
	    	// get the timevalue head item
	    	currentTimerEventItem=(TimerEvent*)getCurrentListItem(&timerEventQueueList);
      	if(currentTimerEventItem->time == currentTick){
	      		currentTimerEventItem->type = TIMEOUT_EVENT;
	      		eventEnqueue((Event*)currentTimerEventItem);
	      		timerEventDequeue();
						resetTick();
						checkNextEventTimerIsZero(&timerEventQueueList);
     		}
    }
	 enableIRQ();
}
