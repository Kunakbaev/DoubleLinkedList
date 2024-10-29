#include "../include/linkedListErrorsHandler.hpp"

const char* getLinkedListErrorMessage(LinkedListErrors error) {
    switch (error) {
        case LINKED_LIST_STATUS_OK:
            return "No errors in linked list, everything is valid.\n";
        case LINKED_LIST_ERROR_INVALID_ARGUMENT:
            return "Linked list error: invalid argument (possibly set to NULL).\n";
        case LINKED_LIST_ERROR_MEMORY_ALLOCATION_ERROR:
            return "Linked list error: memory allocation error.\n";

        default:
            return "Linked list error: unknown error.\n";
    }
}

