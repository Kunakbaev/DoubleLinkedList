#ifndef INCLUDE_LINKED_LIST_ERRORS_HANDLER_HPP
#define INCLUDE_LINKED_LIST_ERRORS_HANDLER_HPP

#include "errorsHandlerDefines.hpp"

enum LinkedListErrors {
    LINKED_LIST_STATUS_OK                     = 0,
    LINKED_LIST_ERROR_INVALID_ARGUMENT        = 1,
    LINKED_LIST_ERROR_MEMORY_ALLOCATION_ERROR = 2,
};

const char* getLinkedListErrorMessage(LinkedListErrors error);

#endif
