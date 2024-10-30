#include <iostream>

#include "logLib.hpp"
#include "../include/linkedListLib.hpp"

#define IF_MAIN_ERR_RETURN(error)                                   \
    do {                                                            \
        if (error != LINKED_LIST_STATUS_OK) {                         \
            LOG_ERROR(getLinkedListErrorMessage(error));             \
            assert(error != LINKED_LIST_STATUS_OK);                   \
            return error;                                           \
        }                                                           \
    } while(0)

int main() {
    setLoggingLevel(DEBUG);
    LOG_DEBUG("Hello world");

    LinkedList list = {};
    IF_MAIN_ERR_RETURN(constructLinkedList(&list));

    IF_MAIN_ERR_RETURN(dumpLinkedList(&list));
    IF_MAIN_ERR_RETURN(insertAfterPosition(&list, 0, 10));
    IF_MAIN_ERR_RETURN(insertAfterPosition(&list, 1, 20));
    IF_MAIN_ERR_RETURN(insertAfterPosition(&list, 0, 30));
    IF_MAIN_ERR_RETURN(insertAfterPosition(&list, 3, 40));
    IF_MAIN_ERR_RETURN(insertAfterPosition(&list, 2, 50));
    IF_MAIN_ERR_RETURN(insertAfterPosition(&list, 0, 60));
    IF_MAIN_ERR_RETURN(dumpLinkedList(&list));

    IF_MAIN_ERR_RETURN(destructLinkedList(&list));

    return 0;
}
