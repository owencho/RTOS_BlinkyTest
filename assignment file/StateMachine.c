/*
 * StateMachine.c
 *
 *  Created on: Jun 17, 2020
 *      Author: academic
 */

#include "Rcc.h"
#include "BaseAddress.h"
#include "StateMachine.h"
#include "Common.h"

void init_stateMachine (StateMachine * states, StateMachine * nextStates, int value){
		states->nextStates = nextStates;
		states->value = value;

}
StateMachine * nextStateMachine (StateMachine * states){
		return states->nextStates;
}
