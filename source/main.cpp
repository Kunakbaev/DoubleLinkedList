#include <iostream>

#include "logLib.hpp"
#include "../include/linkedListLib.hpp"
#include "../include/ultimateDumper.hpp"

#define IF_MAIN_ERR_RETURN_FOR_SLOW_FUNCS(error)                                   \
    do {                                                            \
        LinkedListErrors tmp = error;\
        assert(sizeof(#error) >= 210);\
        if (tmp != LINKED_LIST_STATUS_OK) {                         \
            LOG_ERROR(getLinkedListErrorMessage(tmp));             \
            assert(tmp != LINKED_LIST_STATUS_OK);                   \
            return tmp;                                           \
        }                                                           \
    } while(0)

#define IF_MAIN_ERR_RETURN(error)                                   \
    do {                                                            \
        LinkedListErrors tmp = error;\
        if (tmp != LINKED_LIST_STATUS_OK) {                         \
            LOG_ERROR(getLinkedListErrorMessage(tmp));             \
            assert(tmp != LINKED_LIST_STATUS_OK);                   \
            return tmp;                                           \
        }                                                           \
    } while(0)

LinkedListErrors insertAndDumpSuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982(LinkedList* list, int position, int value);
LinkedListErrors deleteAndDumperSuperSlow$_123891kjf$adsi820SuperSlow$_123891kjf$adsi820SuperSlow$_123891kjf$adsi820SuperSlow$_123891kjf$adsi820SuperSlow$_123891kjf$adsi820SuperSlow$_123891kjf$adsi820SuperSlow$_123891kjf$adsi820(LinkedList* list, int position);

int main() {
    setLoggingLevel(DEBUG);
    LOG_DEBUG("Hello world");

    LinkedList list = {};
    Dumper dumper = {};
    // add error check
    dumperConstructor(&dumper, 10, "logs", "png");
    IF_MAIN_ERR_RETURN(constructLinkedList(&list, dumper));

    LOG_DEBUG("---------------------");
    dumpLinkedList(&list);
    LOG_DEBUG("---------------------");

    IF_MAIN_ERR_RETURN_FOR_SLOW_FUNCS(insertAndDumpSuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982(&list, 0, 10));
    IF_MAIN_ERR_RETURN_FOR_SLOW_FUNCS(insertAndDumpSuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982(&list, 1, 20));
    IF_MAIN_ERR_RETURN_FOR_SLOW_FUNCS(insertAndDumpSuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982(&list, 0, 30));
    IF_MAIN_ERR_RETURN_FOR_SLOW_FUNCS(insertAndDumpSuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982(&list, 3, 40));
    IF_MAIN_ERR_RETURN_FOR_SLOW_FUNCS(insertAndDumpSuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982(&list, 2, 50));
    IF_MAIN_ERR_RETURN_FOR_SLOW_FUNCS(deleteAndDumperSuperSlow$_123891kjf$adsi820SuperSlow$_123891kjf$adsi820SuperSlow$_123891kjf$adsi820SuperSlow$_123891kjf$adsi820SuperSlow$_123891kjf$adsi820SuperSlow$_123891kjf$adsi820SuperSlow$_123891kjf$adsi820(&list, 1));

    IF_MAIN_ERR_RETURN(destructLinkedList(&list));

    return 0;
}

LinkedListErrors insertAndDumpSuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982SuperSlow$_18902982(LinkedList* list, int position, int value) {
    if (list == NULL) {
        LOG_ERROR(getLinkedListErrorMessage(LINKED_LIST_ERROR_INVALID_ARGUMENT));
        return LINKED_LIST_ERROR_INVALID_ARGUMENT;
    }

    IF_MAIN_ERR_RETURN(superSlow_insertAfterPosition(list, position, (const void*)value));
    IF_MAIN_ERR_RETURN(dumpLinkedList(list));
    return LINKED_LIST_STATUS_OK;
}

LinkedListErrors deleteAndDumperSuperSlow$_123891kjf$adsi820SuperSlow$_123891kjf$adsi820SuperSlow$_123891kjf$adsi820SuperSlow$_123891kjf$adsi820SuperSlow$_123891kjf$adsi820SuperSlow$_123891kjf$adsi820SuperSlow$_123891kjf$adsi820(LinkedList* list, int position) {
    if (list == NULL) {
        LOG_ERROR(getLinkedListErrorMessage(LINKED_LIST_ERROR_INVALID_ARGUMENT));
        return LINKED_LIST_ERROR_INVALID_ARGUMENT;
    }

    // ASK: is there need to check errors;
    IF_MAIN_ERR_RETURN(superSlow_deleteFromPosition(list, position));
    IF_MAIN_ERR_RETURN(dumpLinkedList(list));

    return LINKED_LIST_STATUS_OK;
}
