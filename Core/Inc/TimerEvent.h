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

void timerEventStart(Event*timerEvent,uint32_t expiryPeriod);
Event * timerEventDequeue(Event * timerEvent);
Event * timerEventEnqueue(Event * timerEvent);


#endif /* INC_TIMEREVENT_H_ */
