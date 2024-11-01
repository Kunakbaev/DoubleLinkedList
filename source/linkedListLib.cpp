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
    // I want fictive node to have arrInd equal to 0, also it's easier to debug with arrIndexes
    for (int nodeInd = (int)MAX_LIST_SIZE - 1; nodeInd >= 0; --nodeInd) {
        Node* node = &list->nodes[nodeInd];
        node->arrInd = nodeInd;
        IF_ERR_RETURN(makeLinkedListNodeFree(list, &node));
        LOG_DEBUG_VARS(nodeInd, list->nodes[nodeInd].arrInd);
    }

    return LINKED_LIST_STATUS_OK;
}

static LinkedListErrors getLinkedListFreeNode(LinkedList* list, Node** newNode) {
    // RETURN_IF_INVALID(list);
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

    // ASK: what to do with this??
    // RETURN_IF_INVALID(list);
    return LINKED_LIST_STATUS_OK;
}

static bool isListNodeFree(const Node* node) {
    assert(node != NULL);
    return node->next == -1 || node->prev == -1;
}

static int getListHead(const LinkedList* list) {
    assert(list != NULL);
    int front = list->fictiveNode;
    assert(front != -1);
    int res = list->nodes[front].next;

    assert(res != -1);
    return res;
}

static int getListTail(const LinkedList* list) {
    assert(list != NULL);
    int back = list->fictiveNode;
    assert(back != -1);
    int res = list->nodes[back].prev;

    assert(res != -1);
    return res;
}

LinkedListErrors constructLinkedList(LinkedList* list) {
    IF_ARG_NULL_RETURN(list);

    LOG_INFO("constructing list");

    *list = {};
    list->listSize    = 0;
    list->fictiveNode = -1;
    list->nodes       = (Node*)calloc(MAX_LIST_SIZE, sizeof(Node));
    IF_NOT_COND_RETURN(list->nodes != NULL,
                       LINKED_LIST_ERROR_MEMORY_ALLOCATION_ERROR);
    // TODO: add 2 fictive elems

    IF_ERR_RETURN(initFreeNodesList(list));

    Node* element = NULL;
    IF_ERR_RETURN(getLinkedListFreeNode(list, &element));
    assert(element != NULL); // TODO: error check
    list->fictiveNode = element->arrInd;
    assert(list->fictiveNode == 0); // it's convenient to think than element at index 0 is special

    LOG_DEBUG_VARS(list->fictiveNode);
    IF_ERR_RETURN(dumpLinkedListNode(element));
    element->next = element->prev = element->arrInd; // loop

    LOG_DEBUG_VARS(list->fictiveNode);

    RETURN_IF_INVALID(list);
    return LINKED_LIST_STATUS_OK;
}

LinkedListErrors checkIfLinkedListIsValid(const LinkedList* list) {
    IF_ARG_NULL_RETURN(list);
    IF_NOT_COND_RETURN(list->listSize < MAX_LIST_SIZE,
                        LINKED_LIST_LIST_IS_TOO_BIG);

    IF_NOT_COND_RETURN(list->fictiveNode != -1,
                       LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
    if (list->listSize == 0) {
        Node node = list->nodes[list->fictiveNode];
        IF_NOT_COND_RETURN(node.next == node.arrInd &&
                           node.prev == node.arrInd,
                           LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
        return LINKED_LIST_STATUS_OK;
    }

    int head = getListHead(list);
    int tail = getListTail(list);
    // LOG_DEBUG_VARS(head, tail, list->nodes[head].prev);
    // FIXME: add errors
    IF_NOT_COND_RETURN(head != -1,           LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
    IF_NOT_COND_RETURN(tail != -1,           LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
    IF_NOT_COND_RETURN(head < MAX_LIST_SIZE, LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
    IF_NOT_COND_RETURN(tail < MAX_LIST_SIZE, LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
    IF_NOT_COND_RETURN(list->nodes[head].prev == list->fictiveNode,
                       LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
    IF_NOT_COND_RETURN(list->nodes[tail].next == list->fictiveNode,
                       LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);

    Node curNode = list->nodes[head];
    for (size_t nodeInd = 0; nodeInd + 1 < list->listSize; ++nodeInd) {
        int arrInd = curNode.arrInd;
        int next   = curNode.next;
        int prev   = curNode.prev;

        IF_NOT_COND_RETURN(prev != -1 && next != -1,
                           LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
        IF_NOT_COND_RETURN(arrInd == list->nodes[next].prev,
                           LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);

        IF_NOT_COND_RETURN(curNode.next < MAX_LIST_SIZE, LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS);
        curNode = list->nodes[curNode.next];
    }

    return LINKED_LIST_STATUS_OK;
}

//  -------------------------------------------      GET LIST'S NODE BY SOME CONDITION        ----------------------------------

// function works in O(listSize), stores pointer to a node with data = searchValue, if not found, then result = NULL
// if multiple nodes with such data exist, some random node will be chosen
LinkedListErrors superSlow_getListNodeByValue(const LinkedList* list, node_datatype_t searchValue, Node* result) {
    IF_ARG_NULL_RETURN(result);
    IF_ARG_NULL_RETURN(list);
    RETURN_IF_INVALID(list);

    for (size_t nodeInd = 0; nodeInd < list->listSize; ++nodeInd) {
        if (nodeInd == list->fictiveNode)
            continue;
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
LinkedListErrors superSlow_getListNodeByIndex(const LinkedList* list, size_t searchPosition, Node** result) {
    IF_ARG_NULL_RETURN(result);
    IF_ARG_NULL_RETURN(list);
    IF_ARG_NULL_RETURN(result);
    IF_NOT_COND_RETURN(searchPosition <= list->listSize,
                       LINKED_LIST_ERROR_INVALID_ARGUMENT); // FIXME: add error
    RETURN_IF_INVALID(list);

    // TODO: add errors check
    int currentNode = list->fictiveNode;
    for (size_t nodeInd = 0; nodeInd < searchPosition; ++nodeInd) {
        currentNode = list->nodes[currentNode].next;
    }
    *result = &list->nodes[currentNode];

    // ASK: is it ok, that we can return fictive vertex, that should not be visible for user
    RETURN_IF_INVALID(list);
    return LINKED_LIST_STATUS_OK;
}

// helper function, merges two nodes together
static void linkTwoNodes(Node* first, Node* second) {
    assert(first  != NULL);
    assert(second != NULL);

    first->next  = second->arrInd;
    second->prev = first->arrInd;
}

//  -------------------------------------------      INSERTION OF ELEMENTS        ----------------------------------

LinkedListErrors insertAfterRealArrIndex(LinkedList* list,
                                         size_t arrayPosition,
                                         node_datatype_t newValue) {
    LOG_INFO("insert after real arr index");
    IF_ARG_NULL_RETURN(list);
    IF_NOT_COND_RETURN(arrayPosition < MAX_LIST_SIZE,
                       LINKED_LIST_ERROR_INVALID_ARGUMENT); // TODO: add error
    RETURN_IF_INVALID(list);

    Node* newNode = NULL;
    IF_ERR_RETURN(getLinkedListFreeNode(list, &newNode));
    assert(newNode != NULL);
    newNode->data = newValue;
    LOG_DEBUG_VARS(newNode->arrInd, newNode->next, newNode->prev);

    Node* element = &list->nodes[arrayPosition];
    LOG_DEBUG_VARS(element);
    LOG_DEBUG_VARS(element->data, element->arrInd, element->next, element->prev);
    IF_NOT_COND_RETURN(!isListNodeFree(element),
                       LINKED_LIST_ERROR_INVALID_ARGUMENT);

    assert(element->next != -1);
    ++list->listSize;
    linkTwoNodes(newNode, &list->nodes[element->next]);
    linkTwoNodes(element, newNode);

    RETURN_IF_INVALID(list);
    return LINKED_LIST_STATUS_OK;
}

LinkedListErrors superSlow_insertAfterPosition(LinkedList* list,
                                     size_t insertPosition,
                                     node_datatype_t newValue) {
    LOG_INFO("insert after position");
    IF_ARG_NULL_RETURN(list);
    IF_NOT_COND_RETURN(insertPosition <= list->listSize,
                       LINKED_LIST_ERROR_INVALID_ARGUMENT); // TODO: add error
    RETURN_IF_INVALID(list);

    Node* element = NULL;
    IF_ERR_RETURN(superSlow_getListNodeByIndex(list, insertPosition, &element));
    LOG_DEBUG_VARS(element);
    assert(element != NULL); // FIXME: error check
    LOG_DEBUG_VARS(element->data, element->arrInd, element->next);
    LOG_DEBUG_VARS(list->fictiveNode);

    IF_ERR_RETURN(insertAfterRealArrIndex(list, element->arrInd, newValue));

    RETURN_IF_INVALID(list);
    return LINKED_LIST_STATUS_OK;
}

//  -------------------------------------------      DELETION OF ELEMENTS        ----------------------------------

LinkedListErrors deleteFromRealArrIndex(LinkedList* list,
                                        size_t arrayPosition) {
    LOG_INFO("deletion from real array position");
    IF_ARG_NULL_RETURN(list);
    IF_NOT_COND_RETURN(arrayPosition != list->fictiveNode,
                       LINKED_LIST_ERROR_INVALID_ARGUMENT);
    IF_NOT_COND_RETURN(arrayPosition < MAX_LIST_SIZE,
                       LINKED_LIST_ERROR_INVALID_ARGUMENT); // TODO: add error
    IF_NOT_COND_RETURN(list->listSize != 0,
                       LINKED_LIST_ERROR_DELETION_ON_EMPTY_LIST);
    RETURN_IF_INVALID(list);

    Node* element = &list->nodes[arrayPosition];
    LOG_DEBUG_VARS(element);
    LOG_DEBUG_VARS(element->data, element->arrInd, element->next);
    IF_NOT_COND_RETURN(!isListNodeFree(element),
                       LINKED_LIST_ERROR_INVALID_ARGUMENT);

    --list->listSize;
    int prev = element->prev;
    int next = element->next;
    assert(prev != -1);
    assert(next != -1);
    linkTwoNodes(&list->nodes[prev], &list->nodes[next]);
    IF_ERR_RETURN(makeLinkedListNodeFree(list, &element));

    RETURN_IF_INVALID(list);
    return LINKED_LIST_STATUS_OK;
}

LinkedListErrors superSlow_deleteFromPosition(LinkedList* list,
                                    size_t deletionPosition) {
    LOG_INFO("deletion from position");
    IF_ARG_NULL_RETURN(list);
    IF_NOT_COND_RETURN(deletionPosition <= list->listSize,
                       LINKED_LIST_ERROR_INVALID_ARGUMENT); // TODO: add error
    IF_NOT_COND_RETURN(list->listSize != 0,
                       LINKED_LIST_ERROR_DELETION_ON_EMPTY_LIST);
    RETURN_IF_INVALID(list);

    Node* element = NULL;
    IF_ERR_RETURN(superSlow_getListNodeByIndex(list, deletionPosition, &element));
    LOG_DEBUG_VARS(element);
    if (element != NULL)
        LOG_DEBUG_VARS(element->data, element->arrInd, element->next);

    IF_ERR_RETURN(deleteFromRealArrIndex(list, element->arrInd));

    RETURN_IF_INVALID(list);
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

    int curNode = getListHead(list);
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

    LOG_INFO("destructing list");
    RETURN_IF_INVALID(list);

    FREE(list->nodes);
    list->listSize = 0;
    *list = {};

    return LINKED_LIST_STATUS_OK;
}
