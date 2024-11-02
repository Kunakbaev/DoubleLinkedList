#include "../include/linkedListErrorsHandler.hpp"

const char* getLinkedListErrorMessage(LinkedListErrors error) {
    switch (error) {
        case LINKED_LIST_STATUS_OK:
            return "No errors in linked list, everything is valid.\n";
        case LINKED_LIST_ERROR_INVALID_ARGUMENT:
            return "Linked list error: invalid argument (possibly set to NULL).\n";
        case LINKED_LIST_ERROR_MEMORY_ALLOCATION_ERROR:
            return "Linked list error: memory allocation error.\n";
        case LINKED_LIST_LIST_IS_TOO_BIG:
            return "Linked list error: number of elements in list is too big.\n";
        case LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS:
            return "Linked list error: links between some nodes are incorrect.\n";
        case LINKED_LIST_ERROR_NO_AVAILABLE_FREE_NODES:
            return "Linked list error: free node is required, however there are no available free nodes.\n";
        case LINKED_LIST_ERROR_NO_NODE_WITH_SUCH_DATA_FOUND:
            return "Linked list error: node with such data does not exist.\n";
        case LINKED_LIST_ERROR_DELETION_ON_EMPTY_LIST:
            return "Linked list error: list is empty, but deletion of element happens.\n";
        case LINKED_LIST_DUMPER_ERROR:
            return "Linked list error: error occured in dumper sub module.\n";

        default:
            return "Linked list error: unknown error.\n";
    }
}

