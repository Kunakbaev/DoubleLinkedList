#ifndef INCLUDE_LINKED_LIST_LIB_HPP
#define INCLUDE_LINKED_LIST_LIB_HPP

#include <cstddef>

#include "../include/linkedListErrorsHandler.hpp"

typedef int node_datatype_t;

struct Node {
    node_datatype_t data;
    int             next; // "pointer" to a next element, if equal to 0, than it's last element , if equal to -1, than it's a free element
    int             prev; // "pointer" to a prev element, if equal to 0, than it's first element, if equal to -1, than it's a free element
};

struct LinkedList {
    Node* nodes;
    size_t listSize;
};

LinkedListErrors constructLinkedList(LinkedList* list);
LinkedListErrors destructLinkedList(LinkedList* list);

#endif
