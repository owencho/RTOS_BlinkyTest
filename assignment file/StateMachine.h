/*
 * StateMachine.h
 *
 *  Created on: Jun 17, 2020
 *      Author: academic
 */

#ifndef INC_STATEMACHINE_H_
#define INC_STATEMACHINE_H_

typedef struct StateMachine StateMachine;
struct StateMachine {
	StateMachine * nextStates;
	int value;
};
typedef enum{
	S0,S1,S2,
} states;

void init_stateMachine (StateMachine * states, StateMachine * nextStates, int value);
StateMachine * nextStateMachine (StateMachine * states);
#endif /* INC_STATEMACHINE_H_ */
