#include "unity.h"
#include "ListItem.h"
#include "CustomAssert.h"
#include "EventType.h"
#include "StateMachine.h"
void setUp(void){}
void tearDown(void){}
ListItem  itemA,itemB,itemC;
ListItem *listItemB;
int  linkItemDataA,linkItemDataB,linkItemDataC;
GenericStateMachine sm , sm2;

void initListItem(ListItem * listItem, ListItem * next ,EventType type,GenericStateMachine * stateMachine,void* data){
    listItem->next = next;
    listItem->type = NO_EVENT;
    listItem->stateMachine = stateMachine;
    listItem->data = data;
}

void test_CustomAssert_pass_all_same(void){
    initListItem(&itemB,NULL,NO_EVENT,NULL,(void*)&linkItemDataB);
    TEST_ASSERT_EQUAL_LIST_ITEM(&itemB,NULL,NO_EVENT,NULL,(void*)&linkItemDataB);
}
void test_actualListItem_NULL(void){
    TEST_ASSERT_EQUAL_LIST_ITEM(NULL,NULL,NO_EVENT,NULL,(void*)&linkItemDataB);
}

void test_CustomAssert_fail_actual_next_null(void){
    initListItem(&itemB,NULL,NO_EVENT,NULL,(void*)&linkItemDataB);
    TEST_ASSERT_EQUAL_LIST_ITEM(&itemB,&itemA,NO_EVENT,NULL,(void*)&linkItemDataB);
}

void test_CustomAssert_fail_expected_next_null(void){
    initListItem(&itemB,&itemA,NO_EVENT,NULL,(void*)&linkItemDataB);
    TEST_ASSERT_EQUAL_LIST_ITEM(&itemB,NULL,NO_EVENT,NULL,(void*)&linkItemDataB);
}

void test_CustomAssert_fail_expected_actual_next_different(void){
    initListItem(&itemB,&itemA,NO_EVENT,NULL,(void*)&linkItemDataB);
    TEST_ASSERT_EQUAL_LIST_ITEM(&itemB,&itemC,NO_EVENT,NULL,(void*)&linkItemDataB);
}

void test_CustomAssert_fail_expected_actual_eventType_different(void){
    initListItem(&itemB,NULL,NO_EVENT,NULL,(void*)&linkItemDataB);
    TEST_ASSERT_EQUAL_LIST_ITEM(&itemB,NULL,BUTTON_PRESSED_EVENT,NULL,(void*)&linkItemDataB);
}

void test_CustomAssert_fail_expected_actual_stateMachine_different(void){
    initListItem(&itemB,NULL,NO_EVENT,&sm,(void*)&linkItemDataB);
    TEST_ASSERT_EQUAL_LIST_ITEM(&itemB,NULL,NO_EVENT,&sm2,(void*)&linkItemDataB);
}

void test_CustomAssert_fail_expected_stateMachine_NULL(void){
    initListItem(&itemB,NULL,NO_EVENT,&sm,(void*)&linkItemDataB);
    TEST_ASSERT_EQUAL_LIST_ITEM(&itemB,NULL,NO_EVENT,NULL,(void*)&linkItemDataB);
}

void test_CustomAssert_fail_actual_stateMachine_NULL(void){
    initListItem(&itemB,NULL,NO_EVENT,NULL,(void*)&linkItemDataB);
    TEST_ASSERT_EQUAL_LIST_ITEM(&itemB,NULL,NO_EVENT,&sm,(void*)&linkItemDataB);
}

void test_CustomAssert_fail_actual_data_NULL(void){
    initListItem(&itemB,NULL,NO_EVENT,NULL,NULL);
    TEST_ASSERT_EQUAL_LIST_ITEM(&itemB,NULL,NO_EVENT,NULL,(void*)&linkItemDataB);
}

void test_CustomAssert_fail_expected_data_NULL(void){
    initListItem(&itemB,NULL,NO_EVENT,NULL,(void*)&linkItemDataB);
    TEST_ASSERT_EQUAL_LIST_ITEM(&itemB,NULL,NO_EVENT,NULL,NULL);
}

void test_CustomAssert_fail_expected_actual_data_different(void){
    initListItem(&itemB,NULL,NO_EVENT,NULL,(void*)&linkItemDataB);
    TEST_ASSERT_EQUAL_LIST_ITEM(&itemB,NULL,NO_EVENT,NULL,(void*)&linkItemDataC);
}
