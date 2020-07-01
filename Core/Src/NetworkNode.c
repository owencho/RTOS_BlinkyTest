#include "NetworkNode.h"

char* getNodeName(NetworkNode * node){
    if(node == NULL)
        return NULL;

    return node->name;
}

List * getIteratorOfLinks(NetworkNode * node){
    if(node == NULL)
        return NULL;

    return node->links;
}
