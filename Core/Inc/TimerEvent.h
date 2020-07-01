/*
 * TimerEvent.h
 *
 *  Created on: Jun 25, 2020
 *      Author: academic
 */

#ifndef INC_TIMEREVENT_H_
#define INC_TIMEREVENT_H_

#include "Event.h"
#include <stdint.h>

typedef struct TimerEvent TimerEvent;
struct TimerEvent {
		TimerEvent * next;
    EventType type;
    GenericStateMachine * stateMachine;
    int data;
		int accumulativeTime;
};

void timerEventRequest (TimerEvent * event,int expiryPeriod);
void initTimerEventQueue();
void resetTick();
void incTick();
void TimerEventEnqueue(TimerEvent * event);
void TimerEventDequeue();
void timerEventISR();
#endif /* INC_TIMEREVENT_H_ */
