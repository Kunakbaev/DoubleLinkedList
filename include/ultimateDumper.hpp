#ifndef INCLUDE_ULTIMATE_DUMPER_HPP
#define INCLUDE_ULTIMATE_DUMPER_HPP

// ASK: circular import?

enum DumperErrors {
    DUMPER_STATUS_OK                        = 0,
    DUMPER_ERROR_INVALID_ARGUMENT           = 1,
    DUMPER_ERROR_MEMORY_ALLOCATION_ERROR    = 2,
    DUMPER_ERROR_COULD_OPEN_FILE            = 3,
    DUMPER_ERROR_LIST_SUBLIB_ERROR          = 4,
};

#include "dumperStruct.hpp"
#include "linkedListLib.hpp"

DumperErrors dumperConstructor(Dumper* dumper,
                               size_t maxNumOfNodesToDraw,
                               const char* dirForLogsPath,
                               const char* outputFileFormat);
DumperErrors dumperDumpLinkedListNode(Dumper* dumper, const Node* node);
DumperErrors dumperDumpLinkedList(Dumper* dumper, const LinkedList* list);
DumperErrors dumperDestructor(Dumper* dumper);

const char* getDumperErrorMessage(DumperErrors error);

#endif


