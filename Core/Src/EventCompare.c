/*
 * EventCompare.c
 *
 *  Created on: Jun 28, 2020
 *      Author: owen
 */

#include "EventCompare.h"
uint32_t accumulativeTime = 0;
uint32_t * eventTime;
int eventCompareForTime (Event *event, uint32_t * time){
	eventTime = event->data;
    if((accumulativeTime + *eventTime) >= *time){
    	return 0;
    }
    else if ((accumulativeTime +*eventTime) < *time){
    	accumulativeTime = accumulativeTime+ *eventTime;
    	return -1;
    }
    return 1;
}


