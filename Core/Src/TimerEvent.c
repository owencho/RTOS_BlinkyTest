/*
 * TimerEvent.c
 *
 *  Created on: Jun 25, 2020
 *      Author: academic
 */

#include "Event.h"
#include "List.h"
#include "Interrupt.h"
List * eventTimerQueueList;
ListItem * eventTimerItem;
Event * timeEvent;
int currentTick,calculatedTime;
void initEventTimerQueue(){
    eventQueueList = createList();
}
void timerEventRequest (Event * event,uint32_t time){
	timeEvent = event;
	if(eventQueueList->count == 0){
		resetTick();
		timeEvent->data = time;
		listAddItemToHead(eventTimerQueueList,(void)timeEvent);
	}
	else{
		eventTimerItem = findListItem(eventTimerQueueList,time,timeQueueCompare );
		calculatedTime = currentTick + time - (int)(event->data);
		eventTimerQueueList=listAddItemToNext(eventTimerQueueList, eventTimerItem,calculatedTime );
	}


}

void incTick(){
	if(eventQueueList->count != 0)
		currentTick++;
}

void resetTick(){
	disableIRQ();
	currentTick = 0;
	enableIRQ();
}
void eventTimerEnqueue(Event * event){
	disableIRQ();
    listAddItemToTail(eventQueueList, (void *) event);
    enableIRQ();
}
