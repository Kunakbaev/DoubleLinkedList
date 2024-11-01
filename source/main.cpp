#include <iostream>

#include "logLib.hpp"
#include "../include/linkedListLib.hpp"
#include "../include/ultimateDumper.hpp"

#define IF_MAIN_ERR_RETURN(error)                                   \
    do {                                                            \
        if (error != LINKED_LIST_STATUS_OK) {                         \
            LOG_ERROR(getLinkedListErrorMessage(error));             \
            assert(error != LINKED_LIST_STATUS_OK);                   \
            return error;                                           \
        }                                                           \
    } while(0)

LinkedListErrors insertAndDump(LinkedList* list, int position, int value);
LinkedListErrors deleteAndDump(LinkedList* list, int position);

int main() {
    setLoggingLevel(DEBUG);
    LOG_DEBUG("Hello world");

    LinkedList list = {};
    IF_MAIN_ERR_RETURN(constructLinkedList(&list));

    LOG_DEBUG("---------------------");
    IF_MAIN_ERR_RETURN(dumpLinkedList(&list));
    LOG_DEBUG("---------------------");

    //IF_MAIN_ERR_RETURN(insertAndDump(&list, 0, 10));
    //IF_MAIN_ERR_RETURN(insertAndDump(&list, 1, 20));
    // IF_MAIN_ERR_RETURN(insertAndDump(&list, 0, 30));
    // IF_MAIN_ERR_RETURN(insertAndDump(&list, 3, 40));
    // IF_MAIN_ERR_RETURN(insertAndDump(&list, 2, 50));
    // IF_MAIN_ERR_RETURN(insertAndDump(&list, 0, 60));

    Node node = {1010, 228, 9, 10};
    Dumper dumper = {};
    // add error check
    dumperConstructor(&dumper, 5, "logs", "png");
    // dumperDumpLinkedListNode(&dumper, &list.nodes[0]);
    // dumperDumpLinkedListNode(&dumper, &list.nodes[1]);
    // dumperDumpLinkedListNode(&dumper, &list.nodes[2]);
    dumperDumpLinkedList(&dumper, &list);
//     dumper.maxNumOfNodesToDraw = 10;
//     dumperDumpLinkedList(&dumper, &list);
//
//     IF_MAIN_ERR_RETURN(deleteAndDump(&list, 1));
//     dumperDumpLinkedList(&dumper, &list);
    dumperDestructor(&dumper);

    IF_MAIN_ERR_RETURN(destructLinkedList(&list));

    return 0;
}

LinkedListErrors insertAndDump(LinkedList* list, int position, int value) {
    if (list == NULL) {
        LOG_ERROR(getLinkedListErrorMessage(LINKED_LIST_ERROR_INVALID_ARGUMENT));
        return LINKED_LIST_ERROR_INVALID_ARGUMENT;
    }

    // ASK: is there need to check errors;
    IF_MAIN_ERR_RETURN(superSlow_insertAfterPosition(list, position, value));
    IF_MAIN_ERR_RETURN(dumpLinkedList(list));
    return LINKED_LIST_STATUS_OK;
}

LinkedListErrors deleteAndDump(LinkedList* list, int position) {
    if (list == NULL) {
        LOG_ERROR(getLinkedListErrorMessage(LINKED_LIST_ERROR_INVALID_ARGUMENT));
        return LINKED_LIST_ERROR_INVALID_ARGUMENT;
    }

    // ASK: is there need to check errors;
    IF_MAIN_ERR_RETURN(superSlow_deleteFromPosition(list, position));
    IF_MAIN_ERR_RETURN(dumpLinkedList(list));

    return LINKED_LIST_STATUS_OK;
}
