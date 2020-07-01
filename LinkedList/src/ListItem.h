#ifndef _LISTITEM_H
#define _LISTITEM_H

typedef struct ListItem ListItem ;
#include "../../Core/Inc/EventType.h"
#include "../../Core/Inc/StateMachine.h"

struct ListItem{
    ListItem * next ;
    EventType type ;
    GenericStateMachine * stateMachine;
    void * data ;
};

#define freeListItem(listItem)                                      \
                          do{if(listItem) free(listItem);}while(0)
#endif // _LISTITEM_H
