#include "Event.h"
#include "List.h"

List * eventQueueList;
ListItem * item;
Event * initEventStruct(){
    Event * event = malloc(sizeof(Event));
    event = clearEventStruct(event);
    return event;
}

Event * clearEventStruct(Event * event){
    event-> type = NO_EVENT;
    event-> flag = 0;
    event-> functionPtr = NULL;
    event-> data = NULL;
    return event;
}

void initEventQueue(){
    eventQueueList = createList();
}


void addEventIntoEventQueue(Event * event){
    listAddItemToTail(eventQueueList, (void *) event);
}

 Event * createEvent(Event * event,EventType type,EventFnPtr * functionPtr,void * data){
     event-> type = type;
     event-> flag = 0;
     event-> functionPtr = functionPtr;
     event-> data = data;
     return event;
}

int  deEventQueue(Event ** event){
    if(eventQueueList->count ==0)
        return 0;
    resetCurrentListItem(eventQueueList);
    //item = getCurrentListItem(eventQueueList);
    //*event = item->data;
    *event = deleteHeadListItem(eventQueueList);
    return 1;
}
