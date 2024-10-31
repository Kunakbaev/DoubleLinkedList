#ifndef INCLUDE_ULTIMATE_DUMPER_HPP
#define INCLUDE_ULTIMATE_DUMPER_HPP

// ASK: circular import?
#include "linkedListLib.hpp"

enum DumperErrors {
    DUMPER_STATUS_OK                        = 0,
    DUMPER_ERROR_INVALID_ARGUMENT           = 1,
    DUMPER_ERROR_MEMORY_ALLOCATION_ERROR    = 2,
    DUMPER_ERROR_COULD_OPEN_FILE            = 3,
};

struct Dumper {
    const char* dirForLogsPath;
    const char* outputFileFormat;
    size_t      numberOfLogsBefore; // ASK: we want every file to have unique name, file with bigger number was outputed later in program, maybe it will be automaticly sorted by time of creation
};

DumperErrors dumperConstructor(Dumper* dumper,
                               const char* dirForLogsPath,
                               const char* outputFileFormat);
DumperErrors dumperDumpLinkedListNode(Dumper* dumper, const Node* node);
DumperErrors dumperDumpLinkedList(Dumper* dumper);
DumperErrors dumperDestructor(Dumper* dumper);

const char* getDumperErrorMessage(DumperErrors error);

#endif


