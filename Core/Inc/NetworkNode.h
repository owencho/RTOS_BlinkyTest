#ifndef _NETWORKNODE_H
#define _NETWORKNODE_H
#include "List.h"
#include "ListItem.h"
typedef struct NetworkNode NetworkNode ;

struct NetworkNode{
    char* name;
    List * links ;
    int marked; // used for shortestPath
};

typedef struct Link Link ;

struct Link{
    int cost;
    NetworkNode * head ;
    NetworkNode * tail ;
};

char* getNodeName(NetworkNode * node);
List * getIteratorOfLinks(NetworkNode * node);
#endif // _NETWORKNODE_H
