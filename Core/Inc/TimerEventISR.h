#ifndef TIMEREVENTISR_H
#define TIMEREVENTISR_H
#include "EventType.h"
#include "EventQueue.h"
#include "TimerEventQueue.h"
#include "TimerEvent.h"
void timerEventISR(EventQueue * eventQueue,TimerEventQueue *timerEventQueue);
void checkAndDequeueIfNextEventTimerIsZero(EventQueue * eventQueue,TimerEventQueue *timerEventQueue);
#endif // TIMEREVENTISR_H
