/*
 * TimerEvent.h
 *
 *  Created on: Jun 25, 2020
 *      Author: academic
 */

#ifndef INC_TIMEREVENT_H_
#define INC_TIMEREVENT_H_

#include "Event.h"
#include "List.h"
#include <stdint.h>

typedef struct TimerEvent TimerEvent;
struct TimerEvent {
		TimerEvent * next;
    int time;
		EventType type;
		GenericStateMachine * stateMachine;
};

void timerEventRequest (TimerEvent * event,int expiryPeriod);
void initTimerEventQueue();
void resetTick();
void incTick();
void * timerEventDequeue();
void timerEventEnqueue(TimerEvent * event);
void timerEventISR();
void checkAndDequeueIfNextEventTimerIsZero();
void checkAndAddTimerEvent(TimerEvent * event);
#endif /* INC_TIMEREVENT_H_ */
