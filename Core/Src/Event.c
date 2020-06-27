#include "Event.h"
#include "List.h"
#include "Interrupt.h"
List * eventQueueList;
ListItem * item;
Event * initEventStruct(){
    Event * event = malloc(sizeof(Event));
    //event = clearEventStruct(event);
    return event;
}

void initEventQueue(){
    eventQueueList = createList();
}


void eventEnqueue(Event * event){
	disableIRQ();
    listAddItemToTail(eventQueueList, (void *) event);
    enableIRQ();
}
/*
 void initEvent(Event * event,EventType type,EventFnPtr * functionPtr,void * data){
     event-> type = type;
     event-> functionPtr = functionPtr;
     event-> data = data;
}
*/
int  eventDequeue(Event ** event){
	disableIRQ();
  	//disable interrupt put here to protect data from race condition
    if(eventQueueList->count ==0){
    	enableIRQ();
      	return 0;
    }
    resetCurrentListItem(eventQueueList);
    *event = deleteHeadListItem(eventQueueList);
    enableIRQ();
    return 1;
}
