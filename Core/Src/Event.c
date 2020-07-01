#include "Event.h"
#include "List.h"
#include "Hardware.h"
List eventQueueList;

void eventEnqueue(Event * event){
	  disableIRQ();
		if(event == NULL){
				enableIRQ();
				return;
		}
    listAddItemToTail(&eventQueueList,(ListItem * )event);
    enableIRQ();
}

int eventDequeue(Event ** event){
	  disableIRQ();
    if(event == NULL || eventQueueList.count ==0){
    	  enableIRQ();
      	return 0;
    }
    resetCurrentListItem(&eventQueueList);
    *event = deleteHeadListItem(&eventQueueList);
    enableIRQ();
    return 1;
}
