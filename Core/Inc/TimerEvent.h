/*
 * TimerEvent.h
 *
 *  Created on: Jun 25, 2020
 *      Author: academic
 */

#ifndef INC_TIMEREVENT_H_
#define INC_TIMEREVENT_H_

#include <stdint.h>
#include <stddef.h>
#include "StateMachine.h"
#include "EventType.h"

typedef struct TimerEvent TimerEvent;
struct TimerEvent {
		TimerEvent * next;
		EventType type;
		GenericStateMachine * stateMachine;
		int time;
};

#endif /* INC_TIMEREVENT_H_ */
