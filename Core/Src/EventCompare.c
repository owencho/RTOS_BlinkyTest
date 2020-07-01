/*
 * EventCompare.c
 *
 *  Created on: Jun 28, 2020
 *      Author: owen
 */

#include "EventCompare.h"
int eventCompareForTime (TimerEvent *currentEvent, TimerEvent * newEvent){
		if(currentEvent->next == NULL)
				return 1 ;
		else if(currentEvent->next->accumulativeTime >= newEvent->data )
    		return 1;
    else
    	return 0;
}
