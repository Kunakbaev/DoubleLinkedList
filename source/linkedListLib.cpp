#include <cassert>

#include "../include/linkedListLib.hpp"

#define IF_ARG_NULL_RETURN(arg) \
    COMMON_IF_ARG_NULL_RETURN(arg, LINKED_LIST_ERROR_INVALID_ARGUMENT, getLinkedListErrorMessage)

#define IF_ERR_RETURN(error) \
    COMMON_IF_ERR_RETURN(error, getLinkedListErrorMessage, LINKED_LIST_STATUS_OK)

#define IF_NOT_COND_RETURN(condition, error) \
    COMMON_IF_NOT_COND_RETURN(condition, error, getLinkedListErrorMessage)

#define RETURN_IF_INVALID(list) IF_ERR_RETURN(checkIfLinkedListIsValid(list))

const int MAX_LIST_SIZE = 1 << 10;

LinkedListErrors initFreeNodesList(LinkedList* list) {
    IF_ARG_NULL_RETURN(list);



    return LINKED_LIST_STATUS_OK;
}

LinkedListErrors constructLinkedList(LinkedList* list) {
    IF_ARG_NULL_RETURN(list);

    LOG_INFO("constructing list");

    *list = {};
    list->listSize = 0;
    list->head = list->tail = NULL; // ASK: is it ok?
    list->nodes = (Node*)calloc(MAX_LIST_SIZE, sizeof(Node));
    IF_NOT_COND_RETURN(list->nodes != NULL,
                       LINKED_LIST_ERROR_MEMORY_ALLOCATION_ERROR);
    // TODO: add 2 fictive elems

    IF_ERR_RETURN(initFreeNodesList(list));

    RETURN_IF_INVALID(list); // checks that we constructed a valid list
    return LINKED_LIST_STATUS_OK;
}

static LinkedListErrors checkIfLinkedListIsValid(const LinkedList* list) {
    IF_ARG_NULL_RETURN(list);
    IF_NOT_COND_RETURN(list->listSize < MAX_LIST_SIZE,
                        LINKED_LIST_LIST_IS_TOO_BIG);

    for (size_t nodeInd = 0; nodeInd + 1 < list->listSize; ++nodeInd) {
        int arrInd = list->nodes[nodeInd].arrInd;
        int nxt    = list->nodes[nodeInd].next;
        int prev   = list->nodes[nodeInd].prev;

        IF_NOT_COND_RETURN(nxt != -1 && arrInd == list->nodes[nxt].prev,
                           LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
    }

    return LINKED_LIST_STATUS_OK;
}

static LinkedListErrors getLinkedListFreeNode(LinkedList* list, int* newNode) {
    RETURN_IF_INVALID(list);
    IF_ARG_NULL_RETURN(list);
    IF_ARG_NULL_RETURN(newNode);
    IF_NOT_COND_RETURN(list->freeNodesHead != -1,
                       LINKED_LIST_ERROR_NO_AVAILABLE_FREE_NODES);

    *newNode = list->freeNodesHead;
    list->freeNodesHead = list->nodes[list->freeNodesHead].next;

    RETURN_IF_INVALID(list);
    return LINKED_LIST_STATUS_OK;
}

LinkedListErrors getListNodeByValue(const LinkedList* list, node_datatype_t searchValue, Node* result) {
    IF_ARG_NULL_RETURN(result);
    IF_ARG_NULL_RETURN(list);
    RETURN_IF_INVALID((LinkedList*)list);

    for (size_t nodeInd = 0; nodeInd < list->listSize; ++nodeInd) {
        if (list->nodes[nodeInd].data == searchValue) {
            *result = list->nodes[nodeInd];
            return LINKED_LIST_STATUS_OK;
        }
    }

    // no node found
    RETURN_IF_INVALID(list);
    return LINKED_LIST_ERROR_NO_NODE_WITH_SUCH_DATA_FOUND;
}

LinkedListErrors insertAfterPosition(LinkedList* list,
                                     int node_datatype_t newValue) {
    IF_ARG_NULL_RETURN(list);

    int newNode = -1;
    IF_ERR_RETURN(getLinkedListFreeNode(list, &newNode));

    if (list->listSize == 0) {
        ++list->listSize;
        list->head = list->tail = newNode;
        return LINKED_LIST_STATUS_OK;
    }

    Node element = {};
    IF_ERR_RETURN(getListNodeByValue(list, partitionValue, &element));


    return LINKED_LIST_STATUS_OK;
}

LinkedListErrors dumpLinkedListNode(const Node* node) {
    IF_ARG_NULL_RETURN(node);

    LOG_DEBUG_VARS(node->arrInd, node->data, node->next, node->prev);

    return LINKED_LIST_STATUS_OK;
}

LinkedListErrors dumpLinkedList(const LinkedList* list) {
    IF_ARG_NULL_RETURN(list);
    RETURN_IF_INVALID(list);

    LOG_DEBUG("-----------------------------------\nLinked list:");
    LOG_DEBUG_VARS(list, list->listSize, list->nodes);
    LOG_DEBUG("-----------------------------------");

    int curNode = list->head;
    for (size_t nodeInd = 0; nodeInd < list->listSize; ++nodeInd) {
        assert(curNode < list->listSize);
        int nxt = list->nodes[curNode].next;
        IF_ERR_RETURN(dumpLinkedListNode(&list->nodes[curNode]));

        // we validated list, so no errors should occur
        // IF_NOT_COND_RETURN(nxt != -1 || nodeInd == list->listSize - 1 ||
        //                    (nodeInd == ,
        //                    LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
        curNode = nxt;
    }

    RETURN_IF_INVALID(list);
    return LINKED_LIST_STATUS_OK;
}

LinkedListErrors destructLinkedList(LinkedList* list) {
    IF_ARG_NULL_RETURN(list);
    RETURN_IF_INVALID(list);

    LOG_INFO("destructing list");
    FREE(list->nodes);
    list->listSize = 0;
    *list = {};

    return LINKED_LIST_STATUS_OK;
}
