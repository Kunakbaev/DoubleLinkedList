#ifndef INCLUDE_LINKED_LIST_LIB_HPP
#define INCLUDE_LINKED_LIST_LIB_HPP

#include <cstddef>

#include "../include/linkedListErrorsHandler.hpp"

typedef int node_datatype_t;

struct Node {
    node_datatype_t data;
    int           arrInd; // ASK: should arrInd be const, position (index) at which node is stored in LinkedList nodes array
    int             next; // "pointer" to a next element, if equal to 0, than it's last element , if equal to -1, than it's a free element
    int             prev; // "pointer" to a prev element, if equal to 0, than it's first element, if equal to -1, than it's a free element
};

struct LinkedList {
    Node*          nodes;
    size_t      listSize;
    int             head; // "pointer" to a head element, if equal to -1, than list is empty
    int             tail; // "pointer" to a tail element, if equal to -1, than list is empty
    int    freeNodesHead; // "pointer" to a head of free nodes list, if equal to -1, than there are no free nodes in list
};

LinkedListErrors insertAfterPosition(LinkedList* list,
                                     size_t insertPosition,
                                     node_datatype_t newValue);
LinkedListErrors checkIfLinkedListIsValid(const LinkedList* list);
LinkedListErrors constructLinkedList(LinkedList* list);
LinkedListErrors dumpLinkedListNode(const Node* node);
LinkedListErrors dumpLinkedList(const LinkedList* list);
LinkedListErrors destructLinkedList(LinkedList* list);

#endif
