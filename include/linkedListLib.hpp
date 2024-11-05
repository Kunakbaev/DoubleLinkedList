#ifndef INCLUDE_LINKED_LIST_LIB_HPP
#define INCLUDE_LINKED_LIST_LIB_HPP

#include <cstddef>

#include "../include/dumperStruct.hpp"
#include "../include/linkedListErrorsHandler.hpp"

struct Node {
    void*           data;
    int           arrInd; // ASK: should arrInd be const, position (index) at which node is stored in LinkedList nodes array
    int             next; // "pointer" to a next element, if equal to 0, than it's last element , if equal to -1, than it's a free element
    int             prev; // "pointer" to a prev element, if equal to 0, than it's first element, if equal to -1, than it's a free element
};

struct LinkedList {
    Node*          nodes;
    size_t  nodesArrSize;
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
                                               const void* value);
LinkedListErrors deleteListHead(LinkedList* list);
LinkedListErrors deleteListTail(LinkedList* list);

//  -------------------------------------------      GETTERS                      ----------------------------------

LinkedListErrors superSlow_getListNodeByIndex(const LinkedList* list, size_t searchPosition, Node* result);
LinkedListErrors superSlow_getListNodeByValue(const LinkedList* list, const void* searchValue, Node* result);

//  -------------------------------------------      INSERTION OF ELEMENTS        ----------------------------------

LinkedListErrors insertAfterRealArrIndex(LinkedList* list,
                                         size_t arrayPosition,
                                         const void* newValue);
LinkedListErrors superSlow_insertAfterPosition(LinkedList* list,
                                               size_t insertPosition,
                                               const void* newValue);
LinkedListErrors superSlow_insertAfterNodeWithValue(LinkedList* list,
                                                    const void* partitionValue,
                                                    const void* newValue);
LinkedListErrors insertBeforeHead(LinkedList* list,
                                  const void* newValue);
LinkedListErrors insertAfterTail(LinkedList* list,
                                 const void* newValue);

LinkedListErrors checkIfLinkedListIsValid(const LinkedList* list);
LinkedListErrors constructLinkedList(LinkedList* list, Dumper dumper);
LinkedListErrors dumpLinkedListNode(const Dumper* dumper, const Node* node);
LinkedListErrors dumpLinkedList(const LinkedList* list);
LinkedListErrors destructLinkedList(LinkedList* list);

#endif
