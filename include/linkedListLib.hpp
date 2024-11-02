#ifndef INCLUDE_LINKED_LIST_LIB_HPP
#define INCLUDE_LINKED_LIST_LIB_HPP

#include <cstddef>

#include "../include/dumperStruct.hpp"
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
    int      fictiveNode; // created once and then not modified
    int    freeNodesHead; // "pointer" to a head of free nodes list, if equal to -1, than there are no free nodes in list
    Dumper        dumper;
};


//  -------------------------------------------      DELETION OF ELEMENTS        ----------------------------------

LinkedListErrors deleteFromRealArrIndex(LinkedList* list,
                                        size_t arrayPosition);
LinkedListErrors superSlow_deleteFromPosition(LinkedList* list,
                                              size_t deletionPosition);
LinkedListErrors superSlow_deleteNodeWithValue(LinkedList* list,
                                               node_datatype_t value);
LinkedListErrors deleteListHead(LinkedList* list);
LinkedListErrors deleteListTail(LinkedList* list);

//  -------------------------------------------      INSERTION OF ELEMENTS        ----------------------------------

LinkedListErrors insertAfterRealArrIndex(LinkedList* list,
                                         size_t arrayPosition,
                                         node_datatype_t newValue);
LinkedListErrors superSlow_insertAfterPosition(LinkedList* list,
                                               size_t insertPosition,
                                               node_datatype_t newValue);
LinkedListErrors superSlow_insertAfterNodeWithValue(LinkedList* list,
                                                    size_t partitionValue,
                                                    node_datatype_t newValue);
LinkedListErrors insertBeforeHead(LinkedList* list,
                                  node_datatype_t newValue);
LinkedListErrors insertAfterTail(LinkedList* list,
                                 node_datatype_t newValue);

LinkedListErrors checkIfLinkedListIsValid(const LinkedList* list);
LinkedListErrors constructLinkedList(LinkedList* list, Dumper dumper);
LinkedListErrors dumpLinkedListNode(const Node* node);
LinkedListErrors dumpLinkedList(const LinkedList* list);
LinkedListErrors destructLinkedList(LinkedList* list);

#endif
