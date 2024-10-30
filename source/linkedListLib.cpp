#include <cassert>

#include "../include/linkedListLib.hpp"

#define IF_ARG_NULL_RETURN(arg) \
    COMMON_IF_ARG_NULL_RETURN(arg, LINKED_LIST_ERROR_INVALID_ARGUMENT, getLinkedListErrorMessage)

#define IF_ERR_RETURN(error) \
    COMMON_IF_ERR_RETURN(error, getLinkedListErrorMessage, LINKED_LIST_STATUS_OK)

#define IF_NOT_COND_RETURN(condition, error) \
    COMMON_IF_NOT_COND_RETURN(condition, error, getLinkedListErrorMessage)

#define RETURN_IF_INVALID(list) IF_ERR_RETURN(checkIfLinkedListIsValid(list))

// #define NEXT(node) list->nodes[node->next]

const int MAX_LIST_SIZE = 1 << 10;

static LinkedListErrors makeLinkedListNodeFree(LinkedList* list, Node** node) {
    IF_ARG_NULL_RETURN(list);
    IF_ARG_NULL_RETURN(node);
    IF_ARG_NULL_RETURN(*node);

    (*node)->data   =  0;
    (*node)->next   = -1;
    (*node)->prev   = -1;
    if (list->freeNodesHead != -1) // we don't need prev, cause it's a simple oneway list
        (*node)->next = list->freeNodesHead;
    list->freeNodesHead = (*node)->arrInd;

    return LINKED_LIST_STATUS_OK;
}

LinkedListErrors initFreeNodesList(LinkedList* list) {
    IF_ARG_NULL_RETURN(list);

    list->freeNodesHead = -1;
    for (size_t nodeInd = 0; nodeInd < MAX_LIST_SIZE; ++nodeInd) {
        Node* node = &list->nodes[nodeInd];
        node->arrInd = nodeInd;
        IF_ERR_RETURN(makeLinkedListNodeFree(list, &node));
        // LOG_DEBUG_VARS(nodeInd, list->nodes[nodeInd].arrInd);
    }

    return LINKED_LIST_STATUS_OK;
}

LinkedListErrors constructLinkedList(LinkedList* list) {
    IF_ARG_NULL_RETURN(list);

    LOG_INFO("constructing list");

    *list = {};
    list->listSize = 0;
    list->head = list->tail = -1; // ASK: is it ok?
    list->nodes = (Node*)calloc(MAX_LIST_SIZE, sizeof(Node));
    IF_NOT_COND_RETURN(list->nodes != NULL,
                       LINKED_LIST_ERROR_MEMORY_ALLOCATION_ERROR);
    // TODO: add 2 fictive elems

    IF_ERR_RETURN(initFreeNodesList(list));

    IF_ERR_RETURN(checkIfLinkedListIsValid(list));
    //RETURN_IF_INVALID((const LinkedList*)list); // checks that we constructed a valid list
    return LINKED_LIST_STATUS_OK;
}

LinkedListErrors checkIfLinkedListIsValid(const LinkedList* list) {
    IF_ARG_NULL_RETURN(list);
    IF_NOT_COND_RETURN(list->listSize < MAX_LIST_SIZE,
                        LINKED_LIST_LIST_IS_TOO_BIG);

    if (list->listSize == 0) {
        IF_NOT_COND_RETURN(list->head == -1 && list->tail == -1,
                           LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
        return LINKED_LIST_STATUS_OK;
    }

    if (list->listSize == 1) {
        IF_NOT_COND_RETURN(list->head == list->tail,
                           LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
        return LINKED_LIST_STATUS_OK;
    }

    IF_NOT_COND_RETURN(list->head != -1, LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
    IF_NOT_COND_RETURN(list->tail != -1, LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);

    LOG_DEBUG_VARS(list->head, list->tail, list->nodes[list->head].prev);
    // FIXME: add errors
    IF_NOT_COND_RETURN(list->head < MAX_LIST_SIZE, LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
    IF_NOT_COND_RETURN(list->tail < MAX_LIST_SIZE, LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
    IF_NOT_COND_RETURN(list->nodes[list->head].prev == -1,
                       LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
    IF_NOT_COND_RETURN(list->nodes[list->tail].next == -1,
                       LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);

    Node curNode = list->nodes[list->head];
    for (size_t nodeInd = 0; nodeInd + 1 < list->listSize; ++nodeInd) {
        int arrInd = curNode.arrInd;
        int nxt    = curNode.next;
        int prev   = curNode.prev;

        IF_NOT_COND_RETURN(nxt != -1 && arrInd == list->nodes[nxt].prev,
                           LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);

        IF_NOT_COND_RETURN(curNode.next < MAX_LIST_SIZE, LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
        curNode = list->nodes[curNode.next];
    }

    return LINKED_LIST_STATUS_OK;
}

static LinkedListErrors getLinkedListFreeNode(LinkedList* list, Node** newNode) {
    RETURN_IF_INVALID(list);
    IF_ARG_NULL_RETURN(list);
    IF_ARG_NULL_RETURN(newNode);
    IF_NOT_COND_RETURN(list->freeNodesHead != -1,
                       LINKED_LIST_ERROR_NO_AVAILABLE_FREE_NODES);

    // storing head into newNode
    *newNode = &list->nodes[list->freeNodesHead];
    // popping head
    list->freeNodesHead = list->nodes[list->freeNodesHead].next;

    // clearing node
    // WARNING: do not clear arrInd
    (*newNode)->prev = (*newNode)->next = -1;
    (*newNode)->data = 0;

    RETURN_IF_INVALID(list);
    return LINKED_LIST_STATUS_OK;
}

LinkedListErrors getListNodeByValue(const LinkedList* list, node_datatype_t searchValue, Node* result) {
    IF_ARG_NULL_RETURN(result);
    IF_ARG_NULL_RETURN(list);
    RETURN_IF_INVALID(list);

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

// stores node located in list at searchPosition in result var (it's a pointer to a pointer to a Node)
// if searchPosition is 0, stores NULL in result
LinkedListErrors getListNodeByIndex(const LinkedList* list, size_t searchPosition, Node** result) {
    IF_ARG_NULL_RETURN(result);
    IF_ARG_NULL_RETURN(list);
    IF_ARG_NULL_RETURN(result);
    IF_NOT_COND_RETURN(searchPosition <= list->listSize, LINKED_LIST_ERROR_INVALID_ARGUMENT); // FIXME: add error
    RETURN_IF_INVALID((LinkedList*)list);

    if (!searchPosition) {
        *result = NULL;
        return LINKED_LIST_STATUS_OK;
    }

    // TODO: add errors check
    int currentNode = list->head;
    for (size_t nodeInd = 0; nodeInd < searchPosition - 1; ++nodeInd) {
        currentNode = list->nodes[currentNode].next;
    }
    *result = &list->nodes[currentNode];

    // no node found
    RETURN_IF_INVALID(list);
    //return LINKED_LIST_ERROR_NO_NODE_WITH_SUCH_DATA_FOUND;
    return LINKED_LIST_STATUS_OK;
}

static void linkTwoNodes(Node* first, Node* second) {
    assert(first  != NULL);
    assert(second != NULL);

    first->next  = second->arrInd;
    second->prev = first->arrInd;
}

LinkedListErrors insertAfterPosition(LinkedList* list,
                                     size_t insertPosition,
                                     node_datatype_t newValue) {
    LOG_INFO("insert after position");
    IF_ARG_NULL_RETURN(list);
    IF_NOT_COND_RETURN(insertPosition <= list->listSize,
                       LINKED_LIST_ERROR_INVALID_ARGUMENT); // TODO: add error

    Node* newNode = NULL;
    IF_ERR_RETURN(getLinkedListFreeNode(list, &newNode));
    assert(newNode != NULL);
    newNode->data = newValue;

    LOG_DEBUG_VARS(newNode->arrInd, newNode->next, newNode->prev);

    if (list->listSize == 0) {
        ++list->listSize;
        list->head = list->tail = newNode->arrInd;
        LOG_DEBUG_VARS(list->freeNodesHead);
        return LINKED_LIST_STATUS_OK;
    }

    Node* element = NULL;
    IF_ERR_RETURN(getListNodeByIndex(list, insertPosition, &element));
    LOG_DEBUG_VARS(element);
    if (element != NULL)
        LOG_DEBUG_VARS(element->data, element->arrInd, element->next);

    ++list->listSize;
    if (element == NULL) {
        linkTwoNodes(newNode, &list->nodes[list->head]);
        list->head = newNode->arrInd;
        LOG_DEBUG_VARS(newNode->arrInd);
        return LINKED_LIST_STATUS_OK;
    }

    if (element->next != -1)
        linkTwoNodes(newNode, &list->nodes[element->next]);
    else
        list->tail = newNode->arrInd;
    linkTwoNodes(element, newNode);

    return LINKED_LIST_STATUS_OK;
}

LinkedListErrors deleteFromPosition(LinkedList* list,
                                    size_t deletionPosition) {
    LOG_INFO("deletion from position");
    IF_ARG_NULL_RETURN(list);
    IF_NOT_COND_RETURN(deletionPosition <= list->listSize,
                       LINKED_LIST_ERROR_INVALID_ARGUMENT); // TODO: add error
    IF_NOT_COND_RETURN(list->listSize != 0,
                       LINKED_LIST_ERROR_DELETION_ON_EMPTY_LIST);

    Node* element = NULL;
    IF_ERR_RETURN(getListNodeByIndex(list, deletionPosition, &element));
    LOG_DEBUG_VARS(element);
    if (element != NULL)
        LOG_DEBUG_VARS(element->data, element->arrInd, element->next);

    --list->listSize;
    if (list->listSize == 0) {
        IF_ERR_RETURN(makeLinkedListNodeFree(list, &element));
        list->head = list->tail = -1;
        return LINKED_LIST_STATUS_OK;
    }

    int prev = element->prev;
    int next = element->next;
    IF_ERR_RETURN(makeLinkedListNodeFree(list, &element));

    if (next == -1) {
        list->nodes[prev].next = -1;
        list->tail = prev;
        return LINKED_LIST_STATUS_OK;
    }
    if (prev == -1) {
        list->nodes[next].prev = -1;
        list->head = next;
        return LINKED_LIST_STATUS_OK;
    }
    linkTwoNodes(&list->nodes[prev], &list->nodes[next]);

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

    int curNode = list->head;
    for (size_t nodeInd = 0; nodeInd < list->listSize; ++nodeInd) {
        assert(curNode < MAX_LIST_SIZE);
        int nxt = list->nodes[curNode].next;
        IF_ERR_RETURN(dumpLinkedListNode(&list->nodes[curNode]));

        // we validated list, so no errors should occur
        // IF_NOT_COND_RETURN(nxt != -1 || nodeInd == list->listSize - 1 ||
        //                    (nodeInd == ,
        //                    LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
        curNode = nxt;
    }
    LOG_DEBUG("-----------------------------------");

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
