#ifndef _LISTITEM_H
#define _LISTITEM_H

typedef struct ListItem ListItem ;
#include "EventType.h"
#include "StateMachine.h"

struct ListItem{
    ListItem * next ;
    void * data ;
};

#define freeListItem(listItem)                                      \
                          do{if(listItem) free(listItem);}while(0)
#endif // _LISTITEM_H
