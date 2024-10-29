#include <cassert>

#include "../include/linkedListLib.hpp"

#define IF_ARG_NULL_RETURN(arg) \
    COMMON_IF_ARG_NULL_RETURN(arg, LINKED_LIST_ERROR_INVALID_ARGUMENT, getLinkedListErrorMessage)

#define IF_ERR_RETURN(error) \
    COMMON_IF_ERR_RETURN(error, getLinkedListErrorMessage, LINKED_LIST_STATUS_OK)

#define IF_NOT_COND_RETURN(condition, error) \
    COMMON_IF_NOT_COND_RETURN(condition, error)

LinkedListErrors constructLinkedList(LinkedList* list) {
    IF_ARG_NULL_RETURN(list);

    LOG_INFO("constructing list");

    *list = {};
    // TODO: add 2 fictive elems

    return LINKED_LIST_STATUS_OK;
}

LinkedListErrors destructLinkedList(LinkedList* list) {
    IF_ARG_NULL_RETURN(list);

    LOG_INFO("destructing list");
    FREE(list->nodes);
    list->listSize = 0;
    *list = {};

    return LINKED_LIST_STATUS_OK;
}
