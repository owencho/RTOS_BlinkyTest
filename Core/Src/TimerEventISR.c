/*
 * TimerEvent.c
 *
 *  Created on: Jun 25, 2020
 *      Author: academic
 */
#include "List.h"
#include "Hardware.h"
#include "LinkedListCompare.h"
#include "EventCompare.h"
#include "EventQueue.h"
#include "Irq.h"
#include "TimerEventISR.h"
List  timerEventQueueList;
TimerEvent * timerEventItem;
TimerEvent * currentTimerEventItem;
int totalTimeDelay,relativeTick;

void timerEventISR(EventQueue * eventQueue,TimerEventQueue *timerEventQueue){
	TimerEvent * currentTimerEventItem;
	disableIRQ();
	if(timerEventQueueGetCount(timerEventQueue) !=0){
		resetCurrentListItem((List*)timerEventQueue);
		currentTimerEventItem=(TimerEvent*)getCurrentListItem((List*)timerEventQueue);
		if(currentTimerEventItem->time == timerEventQueueGetRelativeTick(timerEventQueue)){
			currentTimerEventItem = timerEventDequeue(timerEventQueue);
			currentTimerEventItem->type = TIMEOUT_EVENT;
			eventEnqueue(eventQueue,(Event*)currentTimerEventItem);
			resetTick(timerEventQueue);
			checkAndDequeueIfNextEventTimerIsZero(eventQueue,timerEventQueue);
		}
	}
	enableIRQ();
}

void checkAndDequeueIfNextEventTimerIsZero(EventQueue * eventQueue,TimerEventQueue *timerEventQueue){
	TimerEvent * nextTimerEventItem;
	resetCurrentTimerEventQueue(timerEventQueue);
	nextTimerEventItem=timerEventQueueGetCurrentEvent(timerEventQueue);
	while(nextTimerEventItem != NULL){
		if(nextTimerEventItem->time == 0){
			nextTimerEventItem =timerEventDequeue(timerEventQueue);
			nextTimerEventItem->type = TIMEOUT_EVENT;
			eventEnqueue(eventQueue,(Event*)nextTimerEventItem);
		}
		nextTimerEventItem=timerEventQueueGetCurrentEvent(timerEventQueue);
		if(nextTimerEventItem == NULL)
			break;
		else if (nextTimerEventItem-> time != 0)
			break;
	}
	resetCurrentListItem(&timerEventQueueList);
}
