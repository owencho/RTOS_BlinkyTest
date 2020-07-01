#include "unity.h"
#include "NetworkNode.h"
#include "ListItemCompare.h"
#include "List.h"
#include "ListItem.h"
NetworkNode node;
List linkList;
char* name;
List*outputLink;

void setUp(void){}
void tearDown(void){}

void initNetworkNode(NetworkNode * node, char*name,List * links){
    node->name = name;
    node-> links = links;
}

void test_NetworkNode_getNodeName(void){
    initNetworkNode(&node,"ABC" ,&linkList);
    name = getNodeName(&node);
    TEST_ASSERT_EQUAL("ABC",name);
}

void test_NetworkNode_getNodeName_null_input(void){
    initNetworkNode(&node,"ABC" ,&linkList);
    name = getNodeName(NULL);
    TEST_ASSERT_NULL(name);
}
void test_NetworkNode_getNodeName_scan(void){
    initNetworkNode(&node,"Scan" ,&linkList);
    name = getNodeName(&node);
    TEST_ASSERT_EQUAL("Scan",name);
}

void test_List_getIteratorOfLinks(void){
    initNetworkNode(&node,"ABC",&linkList);
    outputLink=getIteratorOfLinks(&node);
    TEST_ASSERT_EQUAL_PTR(&linkList,outputLink);
}

void test_List_getIteratorOfLinks_null_input(void){
    initNetworkNode(&node,"ABC",&linkList);
    outputLink=getIteratorOfLinks(NULL);
    TEST_ASSERT_NULL(outputLink);
}
