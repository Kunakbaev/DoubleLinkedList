#ifndef INCLUDE_LINKED_LIST_ERRORS_HANDLER_HPP
#define INCLUDE_LINKED_LIST_ERRORS_HANDLER_HPP

#include "errorsHandlerDefines.hpp"

enum LinkedListErrors {
    LINKED_LIST_STATUS_OK                               = 0,
    LINKED_LIST_ERROR_INVALID_ARGUMENT                  = 1,
    LINKED_LIST_ERROR_MEMORY_ALLOCATION_ERROR           = 2,
    LINKED_LIST_LIST_IS_TOO_BIG                         = 3,
    LINKED_LIST_ERRORS_BAD_LINK_BETWEEN_ELEMS           = 4,
    LINKED_LIST_ERROR_NO_AVAILABLE_FREE_NODES           = 5,
    LINKED_LIST_ERROR_NO_NODE_WITH_SUCH_DATA_FOUND      = 6,
    LINKED_LIST_ERROR_DELETION_ON_EMPTY_LIST            = 7,
};

const char* getLinkedListErrorMessage(LinkedListErrors error);

#endif
