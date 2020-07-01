#include "LinkCompare.h"

int linkCompare (Link *link, Link *compareNode){
    if(link == compareNode)
        return 1;
    return 0;
}
