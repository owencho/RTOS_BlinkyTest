#include "unity.h"
#include "List.h"
#include "ListItem.h"
#include "ListItemCompare.h"
#include "LinkedListCompare.h"
#include "LinkedListProcessor.h"
#include "CustomAssert.h"
#include "EventType.h"
#include "StateMachine.h"
#include "mock_ProcessorFunction.h"
#include "NetworkNode.h"
List linkedList;
ListItem  itemA,itemB,itemC;
ListItem * listItemA;
ListItem *listItemB;
ListItem *listItemC;
Link  linkItemDataA,linkItemDataB,linkItemDataC;
ListItem * outputListItem;
List* outList;
GenericStateMachine sm;
void setUp(void){}
void tearDown(void){}


void initList(List * link, ListItem * head ,ListItem * tail ,int count ,ListItem * current){
    link->head = head;
    link->tail = tail;
    link->count = count;
    link->current = current;
    link->previous =NULL;
}

void initListItem(ListItem * listItem, ListItem * next ,void* data){
    listItem->next = next;
    listItem->data = data;
}

List* createLinkList(ListItem * head ,ListItem * tail ,int count ,ListItem * current,ListItem * previous){
    List * list = (List *)malloc(sizeof(List));
    list->head = head;
    list->tail = tail;
    list->count = count;
    list->current = current;
    list->previous =previous;
    return list;
}

ListItem *createListItemTest(ListItem * next ,void* data){
    ListItem * listItem = (ListItem *)malloc(sizeof(ListItem));
    listItem->next = next;
    listItem->data = data;
    return listItem;
}

//Test for createList
void test_List_createList(void){
    outList = createList();
    //createList will malloc for List
    TEST_ASSERT_NULL(outList->head);
    TEST_ASSERT_NULL(outList->tail);
    TEST_ASSERT_NULL(outList->current);
    TEST_ASSERT_NULL(outList->previous);
    TEST_ASSERT_EQUAL_PTR(0,outList->count);
    freeList(outList);
}
//Test for createListItem
void test_List_createListItem_without_data(void){
    outputListItem = createListItem(NULL);
    //createListItem will malloc for ListItem
    TEST_ASSERT_NULL(outputListItem->next);
    TEST_ASSERT_NULL(outputListItem->data);
    freeListItem(outputListItem);
}

void test_List_createListItem_with_data(void){
    outputListItem = createListItem((void*)&linkItemDataA);
    //createListItem will malloc for ListItem
    TEST_ASSERT_EQUAL_PTR(&linkItemDataA,outputListItem->data);
    TEST_ASSERT_NULL(outputListItem->next);
    freeListItem(outputListItem);
}

//Test for resetCurrentListItem
void test_List_resetList(void){
    initListItem(&itemA, &itemB ,(void*)&linkItemDataA);
    initList(&linkedList, &itemA ,&itemB ,0 ,&itemB);
    resetCurrentListItem(&linkedList);
    //reset will reset the current back to first item
    TEST_ASSERT_EQUAL_PTR(&itemA,linkedList.current);
}

void test_List_resetList_current_NULL(void){
    initList(&linkedList, NULL ,NULL ,0 ,NULL);
    resetCurrentListItem(&linkedList);
    //reset will reset the current back to first item
    TEST_ASSERT_NULL(linkedList.current);
}

void test_List_resetList_NULL(void){
    initList(&linkedList, NULL ,NULL ,0 ,NULL);
    resetCurrentListItem(NULL);
    // do nothing as NULL passed in
}
//Test for getCurrentListItem
/*
*
*   linkedList -> itemA -> itemB -> NULL
*
*/
void test_List_getCurrentListItem(void){
    initListItem(&itemA,&itemB,(void*)&linkItemDataA);
    initListItem(&itemB,NULL,(void*)&linkItemDataB);
    initList(&linkedList, &itemA ,&itemB ,0 ,&itemA);
    //Link list with itemA as head and itemB as tail
    // getCurrentListItem will return the first listitem(listitemA)
    outputListItem=getCurrentListItem(&linkedList);
    TEST_ASSERT_EQUAL_PTR(&itemA,outputListItem);
}

void test_List_getCurrentListItem_NULL(void){
    outputListItem=getCurrentListItem(NULL);
    TEST_ASSERT_NULL(outputListItem);
}

//Test for getNextListItem
/*
*
*   linkedList -> itemA -> itemB -> NULL
*
*/
void test_List_getNextListItem(void){
    initListItem(&itemA,&itemB,(void*)&linkItemDataA);
    initListItem(&itemB,NULL,(void*)&linkItemDataB);
    initList(&linkedList, &itemA ,&itemB ,0 ,&itemA);
    //Link list with itemA as head and itemB as tail
    // getCurrentListItem will return the first listitem(listitemA)
    outputListItem=getCurrentListItem(&linkedList);
    TEST_ASSERT_EQUAL_PTR(&itemA,outputListItem);
    // getNextListItem will return the second listitem(listitemB)
    outputListItem=getNextListItem(&linkedList);
    TEST_ASSERT_EQUAL_PTR(&itemB,outputListItem);
    // getNextListItem will return NULL as nextItem is empty
    outputListItem=getNextListItem(&linkedList);
    TEST_ASSERT_NULL(outputListItem);
}

/*
*
*   linkedList -> itemA -> NULL
*
*/
void test_List_getNextListItem_second_item_is_NULL(void){
    initListItem(&itemA,NULL,(void*)&linkItemDataA);
    initList(&linkedList, &itemA ,&itemA ,0 ,&itemA);
    //Link list with itemA as head and tail
    // getCurrentListItem will return the first listitem(listitemA)
    outputListItem=getCurrentListItem(&linkedList);
    TEST_ASSERT_EQUAL_PTR(&itemA,outputListItem);
    // getNextListItem will return NULL as nextItem is empty
    outputListItem=getNextListItem(&linkedList);
    TEST_ASSERT_NULL(outputListItem);
}

void test_List_getNextListItem_input_NULL(void){
    outputListItem=getNextListItem(NULL);
    TEST_ASSERT_NULL(outputListItem);
}
//Test for listAddItemToTail
/*
*   linkedList -> NULL
*          ↓ add itemA
*   linkedList -> itemA -> NULL
*          ↓ add itemB
*    linkedList -> itemA -> itemB -> NULL
*
*/
void test_List_listAddItemToTail(void){
    initList(&linkedList, NULL ,NULL ,0 ,NULL);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);
    initListItem(&itemB,NULL,(void*)&linkItemDataB);
    //empty linkedList add itemA
    // head ,tail and current points to itemA
    // count = 1 as 1 item inside
    outList=listAddItemToTail(&linkedList,&itemA);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->head);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->tail);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->current);
    TEST_ASSERT_NULL(outList->previous);
    TEST_ASSERT_EQUAL(1,outList->count);
    outputListItem= getCurrentListItem(outList);
    TEST_ASSERT_EQUAL_PTR(outputListItem,&itemA);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_NULL(outputListItem);
    resetCurrentListItem(outList);

    //linkedList add itemB
    // head and current points to itemA
    // tail now points to itemB
    // count = 2 as 2 item inside
    outList=listAddItemToTail(&linkedList,&itemB);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->head);
    TEST_ASSERT_EQUAL_PTR(&itemB,outList->tail);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->current);
    TEST_ASSERT_NULL(outList->previous);
    TEST_ASSERT_EQUAL(2,outList->count);
    outputListItem= getCurrentListItem(outList);
    TEST_ASSERT_EQUAL_PTR(outputListItem,&itemA);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_EQUAL_PTR(outputListItem,&itemB);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_NULL(outputListItem);
    resetCurrentListItem(outList);

}

/*
*   linkedList -> itemA -> NULL
*          ↓ add itemB
*    linkedList -> itemA -> itemB -> NULL
*          ↓ add itemC
*    linkedList -> itemA -> itemB -> itemC -> NULL
*/
void test_List_listAddItemToTail_addC(void){
    initList(&linkedList, &itemA ,&itemA ,1 ,&itemA);
    initListItem(&itemB,NULL,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);
    initListItem(&itemC,NULL,(void*)&linkItemDataC);
    //linkedList add itemB
    // head and current points to itemA
    // tail now points to itemB
    // count = 2 as 2 item inside
    outList=listAddItemToTail(&linkedList,&itemB);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->head);
    TEST_ASSERT_EQUAL_PTR(&itemB,outList->tail);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->current);
    TEST_ASSERT_NULL(outList->previous);
    TEST_ASSERT_EQUAL(2,outList->count);
    outputListItem= getCurrentListItem(outList);
    TEST_ASSERT_EQUAL_PTR(outputListItem,&itemA);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_EQUAL_PTR(outputListItem,&itemB);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_NULL(outputListItem);
    resetCurrentListItem(outList);
    //linkedList add itemC
    // head and current points to itemA
    // tail now points to itemC
    // count = 3 as 3 item inside
    outList=listAddItemToTail(&linkedList,&itemC);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->head);
    TEST_ASSERT_EQUAL_PTR(&itemC,outList->tail);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->current);
    TEST_ASSERT_NULL(outList->previous);
    TEST_ASSERT_EQUAL(3,outList->count);
    outputListItem= getCurrentListItem(outList);
    TEST_ASSERT_EQUAL_PTR(outputListItem,&itemA);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_EQUAL_PTR(outputListItem,&itemB);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_EQUAL_PTR(outputListItem,&itemC);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_NULL(outputListItem);
    resetCurrentListItem(outList);
}

void test_List_listAddItemToTail_list_NULL(void){
    outList=listAddItemToTail(NULL,&itemC);
    TEST_ASSERT_NULL(outList);
}

void test_List_listAddItemToTail_data_NULL(void){
    initList(&linkedList, &itemA ,&itemA ,1 ,&itemA);
    initListItem(&itemB,NULL,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);
    initListItem(&itemC,NULL,(void*)&linkItemDataC);

    outList=listAddItemToTail(&linkedList,NULL);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->head);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->tail);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->current);
    TEST_ASSERT_NULL(outList->previous);
    TEST_ASSERT_EQUAL(1,outList->count);
}

void test_List_listAddItemToTail_both_input_NULL(void){
    outList=listAddItemToTail(NULL,NULL);
    TEST_ASSERT_NULL(outList);
}
//Test for listAddItemToHead
void test_List_listAddItemToHead_linkedList_is_NULL(void){
    initListItem(&itemB,NULL,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);
    initListItem(&itemC,NULL,(void*)&linkItemDataC);

    outList=listAddItemToHead(NULL,&itemB);
    TEST_ASSERT_NULL(outList);
}

void test_List_listAddItemToHead_linkedListData_is_NULL(void){
    initListItem(&itemB,NULL,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);
    initListItem(&itemC,NULL,(void*)&linkItemDataC);

    outList=listAddItemToHead(&linkedList,NULL);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->head);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->tail);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->current);
    TEST_ASSERT_NULL(outList->previous);
    TEST_ASSERT_EQUAL(1,outList->count);
}
/*
*   linkedList -> NULL
*          ↓ add itemB
*    linkedList -> itemB -> NULL
*          ↓ add itemC
*    linkedList -> itemC -> itemB -> itemA -> NULL
*/
void test_List_listAddItemToHead_head_null(void){
    initList(&linkedList, NULL ,NULL ,0 ,NULL);
    initListItem(&itemB,NULL,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);
    initListItem(&itemC,NULL,(void*)&linkItemDataC);
    //linkedList add itemB
    // head points to itemB
    // count = 2 as 2 item inside
    outList=listAddItemToHead(&linkedList,&itemB);
    TEST_ASSERT_EQUAL_PTR(&itemB,outList->head);
    TEST_ASSERT_EQUAL_PTR(&itemB,outList->tail);
    TEST_ASSERT_EQUAL_PTR(&itemB,outList->current);
    TEST_ASSERT_NULL(outList->previous);
    TEST_ASSERT_EQUAL(1,outList->count);
    outputListItem= getCurrentListItem(outList);
    TEST_ASSERT_EQUAL_PTR(outputListItem,&itemB);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_NULL(outputListItem);
}
/*
*   linkedList -> itemA -> NULL
*          ↓ add itemB
*    linkedList -> itemB -> itemA -> NULL
*          ↓ add itemC
*    linkedList -> itemC -> itemB -> itemA -> NULL
*/
void test_List_listAddItemToHead(void){
    initList(&linkedList, &itemA ,&itemA ,1 ,&itemA);
    initListItem(&itemB,NULL,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);
    initListItem(&itemC,NULL,(void*)&linkItemDataC);
    //linkedList add itemB
    // head points to itemB
    // count = 2 as 2 item inside
    outList=listAddItemToHead(&linkedList,&itemB);
    TEST_ASSERT_EQUAL_PTR(&itemB,outList->head);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->tail);
    TEST_ASSERT_EQUAL_PTR(&itemB,outList->current);
    TEST_ASSERT_NULL(outList->previous);
    TEST_ASSERT_EQUAL(2,outList->count);
    outputListItem= getCurrentListItem(outList);
    TEST_ASSERT_EQUAL_PTR(outputListItem,&itemB);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_EQUAL_PTR(outputListItem,&itemA);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_NULL(outputListItem);
    resetCurrentListItem(outList);

    //linkedList add itemC
    // head points to itemC
    // count = 3 as 3 item inside

    outList=listAddItemToHead(&linkedList,&itemC);
    TEST_ASSERT_EQUAL_PTR(&itemC,outList->head);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->tail);
    TEST_ASSERT_EQUAL_PTR(&itemC,outList->current);
    TEST_ASSERT_NULL(outList->previous);
    TEST_ASSERT_EQUAL(3,outList->count);
    outputListItem= getCurrentListItem(outList);
    TEST_ASSERT_EQUAL_PTR(outputListItem,&itemC);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_EQUAL_PTR(outputListItem,&itemB);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_EQUAL_PTR(outputListItem,&itemA);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_NULL(outputListItem);
    resetCurrentListItem(outList);


}
/*
*   linkedList -> itemA -> NULL
*          ↓ add itemB after A
*    linkedList -> itemA -> itemB -> NULL
*          ↓ add itemC
*    linkedList -> itemA  -> itemC -> itemB -> NULL
*/
void test_List_listAddItemToNext_addC(void){
    initList(&linkedList, &itemA ,&itemA ,1 ,&itemA);
    initListItem(&itemB,NULL,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);
    initListItem(&itemC,NULL,(void*)&linkItemDataC);
    //linkedList add itemB
    // head and current points to itemA
    // tail now points to itemB
    // count = 2 as 2 item inside
    outList=listAddItemToNext(&linkedList,&itemA,&itemB);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->head);
    TEST_ASSERT_EQUAL_PTR(&itemB,outList->tail);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->current);
    TEST_ASSERT_NULL(outList->previous);
    TEST_ASSERT_EQUAL(2,outList->count);

    outputListItem= getCurrentListItem(outList);
    TEST_ASSERT_EQUAL_PTR(&itemA,outputListItem);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_EQUAL_PTR(&itemB,outputListItem);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_NULL(outputListItem);
    resetCurrentListItem(outList);

    outList=listAddItemToNext(&linkedList,&itemA,&itemC);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->head);
    TEST_ASSERT_EQUAL_PTR(&itemB,outList->tail);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->current);
    TEST_ASSERT_NULL(outList->previous);
    TEST_ASSERT_EQUAL(3,outList->count);
    outputListItem= getCurrentListItem(outList);
    TEST_ASSERT_EQUAL_PTR(&itemA,outputListItem);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_EQUAL_PTR(&itemC,outputListItem);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_EQUAL_PTR(&itemB,outputListItem);
    outputListItem= getNextListItem(outList);
    TEST_ASSERT_NULL(outputListItem);
    resetCurrentListItem(outList);
}

void test_List_listAddItemToNext_currentList_are_not_in_the_linkedList(void){
    initList(&linkedList, &itemA ,&itemA ,1 ,&itemA);
    initListItem(&itemB,NULL,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);
    initListItem(&itemC,NULL,(void*)&linkItemDataC);
    //linkedList add itemB
    // head and current points to itemA
    // tail now points to itemB
    // count = 2 as 2 item inside
    outList=listAddItemToNext(&linkedList,&itemB,&itemA);
    TEST_ASSERT_EQUAL_PTR(outList->head,&itemA);
    TEST_ASSERT_EQUAL_PTR(outList->tail,&itemA);
    TEST_ASSERT_EQUAL_PTR(outList->current,&itemA);
    TEST_ASSERT_NULL(outList->previous);
    TEST_ASSERT_EQUAL(1,outList->count);
}

void test_List_listAddItemToNext_newList_are_ady_in_the_linkedList(void){
    initList(&linkedList, &itemA ,&itemA ,1 ,&itemA);
    initListItem(&itemB,NULL,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);
    initListItem(&itemC,NULL,(void*)&linkItemDataC);
    //linkedList add itemB
    // head and current points to itemA
    // tail now points to itemB
    // count = 2 as 2 item inside
    outList=listAddItemToNext(&linkedList,&itemA,&itemA);
    TEST_ASSERT_EQUAL_PTR(outList->head,&itemA);
    TEST_ASSERT_EQUAL_PTR(outList->tail,&itemA);
    TEST_ASSERT_EQUAL_PTR(outList->current,&itemA);
    TEST_ASSERT_NULL(outList->previous);
    TEST_ASSERT_EQUAL(1,outList->count);
}

void test_List_listAddItemToNext_NULL(void){
  initList(&linkedList, &itemA ,&itemA ,1 ,&itemA);
  initListItem(&itemB,NULL,(void*)&linkItemDataB);
  initListItem(&itemA,NULL,(void*)&linkItemDataA);
  initListItem(&itemC,NULL,(void*)&linkItemDataC);

  outList=listAddItemToNext(NULL,&itemA,&itemC);
  TEST_ASSERT_NULL(outList);
}

void test_List_listAddItemToNext_inputitem_NULL(void){
  initList(&linkedList, &itemA ,&itemA ,1 ,&itemA);
  initListItem(&itemB,NULL,(void*)&linkItemDataB);
  initListItem(&itemA,NULL,(void*)&linkItemDataA);
  initListItem(&itemC,NULL,(void*)&linkItemDataC);

  outList=listAddItemToNext(&linkedList,NULL,&itemC);
  TEST_ASSERT_EQUAL(&linkedList,outList);
}

void test_List_listAddItemToNext_new_inputitem_NULL(void){
  initList(&linkedList, &itemA ,&itemA ,1 ,&itemA);
  initListItem(&itemB,NULL,(void*)&linkItemDataB);
  initListItem(&itemA,NULL,(void*)&linkItemDataA);
  initListItem(&itemC,NULL,(void*)&linkItemDataC);

  outList=listAddItemToNext(&linkedList,&itemA,NULL);
  TEST_ASSERT_EQUAL(&linkedList,outList);
}
// test for deleteHeadListItem
/*
*    linkedList -> itemA -> itemB -> NULL
*          ↓ delete
*   linkedList -> itemB -> NULL
*          ↓ delete
*   linkedList -> NULL
*/
void test_List_deleteHeadListItem(void){
    initList(&linkedList, &itemA ,&itemB ,2 ,&itemA);
    initListItem(&itemB,NULL,(void*)&linkItemDataB);
    initListItem(&itemA,&itemB,(void*)&linkItemDataA);
    initListItem(&itemC,NULL,(void*)&linkItemDataC);
    //linkedList with itemA and itemB
    //delete itemA
    // head and current points to itemB
    // tail points to itemB
    // count = 1 as 1 item inside
    outputListItem=deleteHeadListItem(&linkedList);
    TEST_ASSERT_EQUAL_PTR(&itemA,outputListItem);
    TEST_ASSERT_EQUAL_PTR(&itemB,linkedList.head);
    TEST_ASSERT_EQUAL_PTR(&itemB,linkedList.tail);
    TEST_ASSERT_EQUAL_PTR(&itemB,linkedList.current);
    TEST_ASSERT_NULL(outList->previous);
    TEST_ASSERT_EQUAL(1,outList->count);
    //linkedList delete itemB
    // head and current points to NULL
    // tail now points to NULL
    // count = 0 as 0 item inside
    outputListItem=deleteHeadListItem(outList);
    TEST_ASSERT_EQUAL_PTR(&itemB,outputListItem);
    TEST_ASSERT_NULL(outList->head);
    TEST_ASSERT_NULL(outList->tail);
    TEST_ASSERT_NULL(outList->current);
    TEST_ASSERT_NULL(outList->previous);
    TEST_ASSERT_EQUAL(0,outList->count);
}

void test_List_deleteHeadListItemlinkedList_NULL(void){

    outputListItem=deleteHeadListItem(NULL);
    TEST_ASSERT_NULL(outputListItem);
}

/*
*   linkedList -> NULL
*/
void test_List_deleteHeadListItem_NULL(void){
    initList(&linkedList, NULL ,NULL ,0 ,NULL );
    //linkedList delete but item has nothing
    // count = 0 as 0 item inside
    outputListItem=deleteHeadListItem(&linkedList);
    TEST_ASSERT_NULL(outputListItem);
}
////checkAndDeleteListItem//////////////////////////
//This function is to check the location of the deleted node
// when this function called the deleted linkItem is already found
// it is used to determine where is the location of the linkItem and delete
// either is head , tail or middle of linkedList

void test_List_checkAndDeleteListItem_linkedList_NULL(void){
    initList(&linkedList, &itemA ,&itemB ,2 ,&itemA );
    initListItem(&itemB,NULL,(void*)&linkItemDataB);
    initListItem(&itemA,&itemB,(void*)&linkItemDataA);
    initListItem(&itemC,NULL,(void*)&linkItemDataC);

    outputListItem=checkAndDeleteListItem(NULL,&itemA);
    TEST_ASSERT_NULL(outputListItem);
}

void test_List_checkAndDeleteListItem_deleteListItem_NULL(void){
    initList(&linkedList, &itemA ,&itemB ,2 ,&itemA );
    initListItem(&itemB,NULL,(void*)&linkItemDataB);
    initListItem(&itemA,&itemB,(void*)&linkItemDataA);
    initListItem(&itemC,NULL,(void*)&linkItemDataC);

    outputListItem=checkAndDeleteListItem(&linkedList,NULL);
    TEST_ASSERT_EQUAL_PTR(&itemA,linkedList.head);
    TEST_ASSERT_EQUAL_PTR(&itemB,linkedList.tail);
    TEST_ASSERT_EQUAL_PTR(&itemA,linkedList.current);
    TEST_ASSERT_EQUAL(2,linkedList.count);
    TEST_ASSERT_NULL(outputListItem);
}
/*
*    linkedList -> itemA -> itemB -> NULL
*          ↓ deleteA
*   linkedList -> itemB -> NULL
*/
void test_List_checkAndDeleteListItem_delete_firstNode(void){
    initList(&linkedList, &itemA ,&itemB ,2 ,&itemA );
    initListItem(&itemB,NULL,(void*)&linkItemDataB);
    initListItem(&itemA,&itemB,(void*)&linkItemDataA);
    initListItem(&itemC,NULL,(void*)&linkItemDataC);
    //current must points to itemA first before check the location of node and delete
    outputListItem=checkAndDeleteListItem(&linkedList,&itemA);
    TEST_ASSERT_EQUAL_PTR(&itemB,linkedList.head);
    TEST_ASSERT_EQUAL_PTR(&itemB,linkedList.tail);
    TEST_ASSERT_EQUAL_PTR(&itemB,linkedList.current);
    TEST_ASSERT_EQUAL_PTR(&itemA,outputListItem);
    TEST_ASSERT_EQUAL(1,linkedList.count);
}

/*
*    linkedList -> itemA -> itemB -> NULL
*          ↓ deleteB
*   linkedList -> itemA -> NULL
*/
void test_List_checkAndDeleteListItem_delete_tail(void){
    initList(&linkedList, &itemA ,&itemB ,2 ,&itemA );
    initListItem(&itemB,NULL,(void*)&linkItemDataB);
    initListItem(&itemA,&itemB,(void*)&linkItemDataA);
    //current must points to itemB first before check the location of node and delete
    outputListItem=getNextListItem(&linkedList);
    outputListItem=checkAndDeleteListItem(&linkedList,&itemB);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->head);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->tail);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->current);
    TEST_ASSERT_EQUAL(1,linkedList.count);

}

/*
*    linkedList -> itemC -> itemB -> itemA -> NULL
*          ↓ delete itemB
*    linkedList -> itemC -> itemA -> NULL
*
*/
void test_List_checkAndDeleteListItem_delete_middle(void){
    initList(&linkedList, &itemC,&itemA ,3 ,&itemC);
    initListItem(&itemB,&itemA,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);
    initListItem(&itemC,&itemB,(void*)&linkItemDataC);
    //current must points to itemB first before check the location of node and delete
    outputListItem=getNextListItem(&linkedList);
    outputListItem=checkAndDeleteListItem(&linkedList,&itemB);
    TEST_ASSERT_EQUAL_PTR(&itemC,outList->head);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->tail);
    TEST_ASSERT_EQUAL_PTR(&itemC,outList->current);
    TEST_ASSERT_EQUAL_PTR(&itemB,outputListItem);
    TEST_ASSERT_EQUAL(2,outList->count);

    outputListItem=getCurrentListItem(outList);
    TEST_ASSERT_EQUAL_PTR(&itemC,outputListItem);
    outputListItem=getNextListItem(outList);
    TEST_ASSERT_EQUAL_PTR(&itemA,outputListItem);
    outputListItem=getNextListItem(outList);
    TEST_ASSERT_NULL(outputListItem);
}


////Delete Selected item//////////////////////////////////////////////////////////////

/*
*    linkedList -> itemC -> itemB -> itemA -> NULL
*          ↓ delete itemB
*    linkedList -> itemC -> itemA -> NULL
*
*/
void test_List_deleteSelectedListItem_delete_middle(void){
    initList(&linkedList,&itemC , &itemA ,3 ,&itemC );
    initListItem(&itemB,&itemA,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);
    initListItem(&itemC,&itemB,(void*)&linkItemDataC);

    outputListItem=deleteSelectedListItem(&linkedList,&itemB,(LinkedListCompare)ListItemCompare);
    TEST_ASSERT_EQUAL_PTR(&itemC,outList->head);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->tail);
    TEST_ASSERT_EQUAL_PTR(&itemC,outList->current);
    TEST_ASSERT_EQUAL_PTR(&itemB,outputListItem);
    TEST_ASSERT_EQUAL(2,outList->count);

    outputListItem=getCurrentListItem(outList);
    TEST_ASSERT_EQUAL_PTR(&itemC,outputListItem);
    outputListItem=getNextListItem(outList);
    TEST_ASSERT_EQUAL_PTR(&itemA,outputListItem);
    outputListItem=getNextListItem(outList);
    TEST_ASSERT_NULL(outputListItem);

}

/*
*    linkedList -> itemC -> itemB -> itemA -> NULL
*          ↓ delete itemC
*    linkedList -> itemB -> itemA -> NULL
*
*/
void test_List_deleteSelectedListItem_delete_front(void){
    initList(&linkedList,&itemC , &itemA ,3 ,&itemC );
    initListItem(&itemB,&itemA,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);
    initListItem(&itemC,&itemB,(void*)&linkItemDataC);

    outputListItem=deleteSelectedListItem(&linkedList,&itemC,(LinkedListCompare)ListItemCompare);
    TEST_ASSERT_EQUAL_PTR(&itemB,outList->head);
    TEST_ASSERT_EQUAL_PTR(&itemA,outList->tail);
    TEST_ASSERT_EQUAL_PTR(&itemB,outList->current);
    TEST_ASSERT_EQUAL_PTR(&itemC,outputListItem);
    TEST_ASSERT_EQUAL(2,outList->count);

    outputListItem=getCurrentListItem(outList);
    TEST_ASSERT_EQUAL_PTR(&itemB,outputListItem);
    outputListItem=getNextListItem(outList);
    TEST_ASSERT_EQUAL_PTR(&itemA,outputListItem);
    outputListItem=getNextListItem(outList);
    TEST_ASSERT_NULL(outputListItem);
}

/*
*    linkedList -> itemC -> itemB -> itemA -> NULL
*          ↓ delete itemA
*    linkedList -> itemC -> itemB -> NULL
*
*/
void test_List_deleteSelectedListItem_delete_last(void){
    initList(&linkedList,&itemC , &itemA ,3 ,&itemC );
    initListItem(&itemB,&itemA,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);
    initListItem(&itemC,&itemB,(void*)&linkItemDataC);

    outputListItem=deleteSelectedListItem(&linkedList,&itemA,(LinkedListCompare)ListItemCompare);
    TEST_ASSERT_EQUAL_PTR(&itemC,outList->head);
    TEST_ASSERT_EQUAL_PTR(&itemB,outList->tail);
    TEST_ASSERT_EQUAL_PTR(&itemC,outList->current);
    TEST_ASSERT_EQUAL_PTR(&itemA,outputListItem);
    TEST_ASSERT_EQUAL(2,outList->count);
    outputListItem=getCurrentListItem(outList);
    TEST_ASSERT_EQUAL_PTR(outputListItem,&itemC);
    outputListItem=getNextListItem(outList);
    TEST_ASSERT_EQUAL_PTR(outputListItem,&itemB);
    outputListItem=getNextListItem(outList);
    TEST_ASSERT_NULL(outputListItem);

}

void test_List_deleteSelectedListItem_LinkList_NULL(void){
    outputListItem=deleteSelectedListItem(NULL,&itemA,(LinkedListCompare)ListItemCompare);
    TEST_ASSERT_NULL(outputListItem);
}

void test_List_deleteSelectedListItem_listItem_NULL(void){
    outputListItem=deleteSelectedListItem(&linkedList,NULL,(LinkedListCompare)ListItemCompare);
    TEST_ASSERT_NULL(outputListItem);
}

void test_List_deleteSelectedListItem_function_pointer_NULL(void){
    outputListItem=deleteSelectedListItem(&linkedList,&itemA,NULL);
    TEST_ASSERT_NULL(outputListItem);
}

void test_List_deleteSelectedListItem_LinkList_empty(void){
    initList(&linkedList, NULL ,NULL ,0 ,NULL );
    outputListItem=deleteSelectedListItem(&linkedList,&itemA,(LinkedListCompare)ListItemCompare);
    TEST_ASSERT_NULL(outputListItem);
}
// test for findListItem
/*
*    linkedList -> itemA -> itemB -> NULL
*
*/
void test_List_findListItem(void){
    initList(&linkedList, &itemA ,&itemB ,2 ,&itemA );
    initListItem(&itemB,NULL,(void*)&linkItemDataB);
    initListItem(&itemA,&itemB,(void*)&linkItemDataA);

    outputListItem=findListItem(&linkedList,&itemB,(LinkedListCompare)ListItemCompare);
    TEST_ASSERT_EQUAL_PTR(&itemB,outputListItem);
}

/*
*    linkedList -> itemA -> itemB -> NULL
*
*/
void test_List_findListItem_linkedList_couldnt_found_the_node(void){
    initList(&linkedList, &itemA ,&itemB ,2 ,&itemA );
    initListItem(&itemB,&itemA,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);

    outputListItem=findListItem(&linkedList,&itemC,(LinkedListCompare)ListItemCompare);
    TEST_ASSERT_NULL(outputListItem);
}

void test_List_findListItem_linkedList_NULL(void){
    initList(&linkedList, &itemA ,&itemB ,2 ,&itemA );
    initListItem(&itemB,&itemA,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);

    outputListItem=findListItem(NULL,&itemB,(LinkedListCompare)ListItemCompare);
    TEST_ASSERT_NULL(outputListItem);
}

void test_List_findListItem_data_NULL(void){
    initList(&linkedList, &itemA ,&itemB ,2 ,&itemA );
    initListItem(&itemB,&itemA,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);

    outputListItem=findListItem(&linkedList,NULL,(LinkedListCompare)ListItemCompare);
    TEST_ASSERT_NULL(outputListItem);
}

void test_List_findListItem_function_pointer_NULL(void){
    initList(&linkedList, &itemA ,&itemB ,2 ,&itemA );
    initListItem(&itemB,&itemA,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);

    outputListItem=findListItem(&linkedList,&itemB,NULL);
    TEST_ASSERT_NULL(outputListItem);
}

// test for listForEach
/*
*    linkedList -> itemC -> itemB -> itemA -> NULL
*
*/
void test_List_listForEach(void){
    initList(&linkedList, &itemC ,&itemA ,3 ,&itemC);
    initListItem(&itemB,&itemA,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);
    initListItem(&itemC,&itemB,(void*)&linkItemDataC);
    printLinkSourceProcessor_Expect(&itemC);
    printLinkSourceProcessor_Expect(&itemB);
    printLinkSourceProcessor_Expect(&itemA);
    listForEach(&linkedList,(LinkedListProcessor)printLinkSourceProcessor);
}

void test_List_listForEach_linked_is_NULL(void){
    initList(&linkedList, &itemC ,&itemA ,3 ,&itemC);
    initListItem(&itemB,&itemA,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);
    initListItem(&itemC,&itemB,(void*)&linkItemDataC);
    listForEach(NULL,(LinkedListProcessor)printLinkSourceProcessor);
}

void test_List_listForEach_processor_NULL(void){
    initList(&linkedList, &itemC ,&itemA ,3 ,&itemC);
    initListItem(&itemB,&itemA,(void*)&linkItemDataB);
    initListItem(&itemA,NULL,(void*)&linkItemDataA);
    initListItem(&itemC,&itemB,(void*)&linkItemDataC);
    listForEach(&linkedList,NULL);
}
