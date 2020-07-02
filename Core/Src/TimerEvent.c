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
/*
void initTimerEventQueue(){
		timerEventQueueList = createList();
}
*/
void timerEventRequest (TimerEvent * event,int expiryPeriod){
		if(event == NULL)
				return ;
		if(timerEventQueueList.count == 0){
				resetTick();
				event->data =expiryPeriod ;
				event->accumulativeTime = expiryPeriod;
				TimerEventEnqueue(event);
		}
		else{
			disableIRQ();
			totalTimeDelay = expiryPeriod + currentTick;
			event->data =totalTimeDelay;
			event->accumulativeTime = totalTimeDelay;
			resetCurrentListItem(&timerEventQueueList);
			currentTimerEventItem =(TimerEvent*) getCurrentListItem(&timerEventQueueList);
			if(currentTimerEventItem->data >= event->data){
					currentTimerEventItem->data = currentTimerEventItem->data - event->data;
					listAddItemToHead(&timerEventQueueList,(ListItem*)event);
			}
			else{
					timerEventItem = (TimerEvent*)findListItem(&timerEventQueueList,event,(LinkedListCompare)eventCompareForTime);

					// new time delay is totalTimeDelay - previous accumlated time
					event->data = totalTimeDelay - timerEventItem->accumulativeTime;
					// new time delay for next Time Event is old time delay - previous delay time
					if(timerEventItem->next != NULL)
							timerEventItem->next->data = timerEventItem->next->data - event->data ;

					listAddItemToNext(&timerEventQueueList,(ListItem*)timerEventItem,(ListItem*)event);
			}
		  enableIRQ();
		}
}

void incTick(){
		if(timerEventQueueList.count != 0)
				currentTick++;
}

void resetTick(){
		disableIRQ();
		currentTick = 0;
		enableIRQ();
}

void TimerEventEnqueue(TimerEvent * event){
		disableIRQ();
		if(event == NULL){
				enableIRQ();
				return ;
		}
    listAddItemToTail(&timerEventQueueList,(ListItem*)event);
		enableIRQ();
}

void TimerEventDequeue(){
		//disableIRQ();
  	//disable interrupt put here to protect data from race condition
 		if(timerEventQueueList.count ==0){
    		//enableIRQ();
      	return ;
    }
    resetCurrentListItem(&timerEventQueueList);
    deleteHeadListItem(&timerEventQueueList);
    //enableIRQ();
    return ;
}
void checkNextEventTimerIsZero(List * timerEventQueue){
		TimerEvent * nextTimerEventItem;
		nextTimerEventItem=(TimerEvent*)getNextListItem(&timerEventQueueList);
		while(nextTimerEventItem != NULL){
				if(currentTimerEventItem->data == 0){
						nextTimerEventItem->type = TIMEOUT_EVENT;
						eventEnqueue((Event*)nextTimerEventItem);
						TimerEventDequeue();
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
      	if(currentTimerEventItem->accumulativeTime >= currentTick){
	      		currentTimerEventItem->type = TIMEOUT_EVENT;
	      		eventEnqueue((Event*)currentTimerEventItem);
	      		TimerEventDequeue();
						checkNextEventTimerIsZero(&timerEventQueueList);
						if(timerEventQueueList.count ==0)
								resetTick;
     		}
    }
	 enableIRQ();
}
