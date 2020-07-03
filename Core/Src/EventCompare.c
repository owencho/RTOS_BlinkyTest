/*
 * EventCompare.c
 *
 *  Created on: Jun 28, 2020
 *      Author: owen
 */

#include "EventCompare.h"
int eventCompareForTime (TimerEvent *currentEvent, TimerEvent * newEvent){
		if(currentEvent == NULL || newEvent == NULL)
				return 0;

		newEvent->time = newEvent->time - currentEvent->time;
		if(currentEvent->next == NULL){
				return 1 ;
		}

		else if(currentEvent->next->time > newEvent->time ){
				return 1;
		}
    else{
				return 0;
		}
}
