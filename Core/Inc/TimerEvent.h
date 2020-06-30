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

struct EventTimer {
	ListItem * next;
    EventType type;
    GenericStateMachine * stateMachine;
    int data;
};

void timerEventStart(Event*timerEvent,uint32_t expiryPeriod);
void initEventTimerQueue();
void resetTick();
void incTick();
void eventTimerEnqueue(Event * event);
void eventTimerDequeue();
void timerEventISR();
#endif /* INC_TIMEREVENT_H_ */
