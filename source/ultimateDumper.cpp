#include <stdlib.h>

#include "../include/ultimateDumper.hpp"

#define IF_ARG_NULL_RETURN(arg) \
    COMMON_IF_ARG_NULL_RETURN(arg, DUMPER_ERROR_INVALID_ARGUMENT, getDumperErrorMessage)

#define IF_ERR_RETURN(error) \
    COMMON_IF_ERR_RETURN(error, getDumperErrorMessage, DUMPER_STATUS_OK)

#define IF_NOT_COND_RETURN(condition, error) \
    COMMON_IF_NOT_COND_RETURN(condition, error, getDumperErrorMessage)

const size_t FILE_NAME_BUFFER_SIZE = 100;
char* fileFullNameBuffer = NULL;
char* fileNameBuffer     = NULL;

const char* getDumperErrorMessage(DumperErrors error) {
    switch (error) {
        case DUMPER_STATUS_OK:
            return "No errors in linked list, everything is valid.\n";
        case DUMPER_ERROR_INVALID_ARGUMENT:
            return "Dumper error: invalid argument (possibly set to NULL).\n";
        case DUMPER_ERROR_MEMORY_ALLOCATION_ERROR:
            return "Dumper error: memory allocation error.\n";
        case DUMPER_ERROR_COULD_OPEN_FILE:
            return "Dumper error: couldn't open file.\n";

        default:
            return "Dumper error: unknown error.\n";
    }
}

DumperErrors dumperConstructor(Dumper* dumper,
                               const char* dirForLogsPath,
                               const char* outputFileFormat) {
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(dirForLogsPath);
    IF_ARG_NULL_RETURN(outputFileFormat);

    *dumper = {};
    dumper->dirForLogsPath     = dirForLogsPath;
    dumper->outputFileFormat   = outputFileFormat;
    dumper->numberOfLogsBefore = 0;

    fileFullNameBuffer = (char*)calloc(FILE_NAME_BUFFER_SIZE, sizeof(char));
    IF_NOT_COND_RETURN(fileFullNameBuffer != NULL,
                       DUMPER_ERROR_MEMORY_ALLOCATION_ERROR);
    fileNameBuffer = (char*)calloc(FILE_NAME_BUFFER_SIZE, sizeof(char));
    IF_NOT_COND_RETURN(fileNameBuffer != NULL,
                       DUMPER_ERROR_MEMORY_ALLOCATION_ERROR);
    LOG_DEBUG("ok");

    system("mkdir -p logs");
    system("mkdir -p logs/images");
    system("mkdir -p logs/html");

    return DUMPER_STATUS_OK;
}

DumperErrors dumperDumpLinkedListNode(Dumper* dumper, const Node* node) {
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(node);

    ++dumper->numberOfLogsBefore;
    memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    sprintf(fileNameBuffer, "%d_node.dot",
            dumper->numberOfLogsBefore, dumper->outputFileFormat);
    LOG_DEBUG("ok2");
    memset(fileFullNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    sprintf(fileFullNameBuffer, "%s/%s", dumper->dirForLogsPath, fileNameBuffer);
    LOG_DEBUG_VARS(fileNameBuffer);
    LOG_DEBUG_VARS(fileFullNameBuffer);

    FILE* outputFile = fopen(fileFullNameBuffer, "w");
    IF_NOT_COND_RETURN(outputFile != NULL,
                       DUMPER_ERROR_COULD_OPEN_FILE);

    // fprintf(outputFile, "   \n\
    //     digraph G {                             \n\
    //         main -> parse -> execute;                               \n\
    //         main -> init;                               \n\
    //         main -> cleanup;                                \n\
    //         execute -> make_string;                             \n\
    //         execute -> printf                               \n\
    //         init -> make_string;                                \n\
    //         main -> printf;                             \n\
    //         execute -> compare;                             \n\
    //     }                               \n\
    // ");

    // fprintf(outputFile, "\
    // digraph html {              \n\
    //     abc [shape=none, margin=0, label=<                   \n\
    //     <TABLE>     \n\
    //         <TR>        \n\
    //             <TD>bruh1</TD>      \n\
    //             <TD>bruh2</TD>      \n\
    //             <TD>bruh3</TD>      \n\
    //             <TD>bruh4</TD>      \n\
    //         </TR>       \n\
    //     </TABLE>>];                     \n\
    // }");

    fprintf(outputFile, "\
    digraph html {                      \n\
        iamnode [shape=none, margin=0, label=<                                         \n\
        <TABLE>                                         \n\
            <TR><TD>arrInd: %d</TD></TR>                                         \n\
            <TR><TD>dataInd: %d</TD></TR>                                         \n\
            <TR><TD>next: %d</TD></TR>           \n\
            <TR><TD>prev: %d</TD></TR>   \n\
        </TABLE>>];                                         \n\
    }", node->arrInd, node->data, node->next, node->prev);
    fclose(outputFile);



    memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    sprintf(fileNameBuffer, "%d_node.%s",
            dumper->numberOfLogsBefore, dumper->outputFileFormat);
    LOG_DEBUG_VARS(fileNameBuffer);

    memset(fileFullNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    sprintf(fileFullNameBuffer, "dot -Tpng logs/1_node.dot -o  %s/images/%s", dumper->dirForLogsPath, fileNameBuffer);
    LOG_DEBUG_VARS(fileFullNameBuffer);
    system(fileFullNameBuffer);
    return DUMPER_STATUS_OK;
}

DumperErrors dumperDumpLinkedList(Dumper* dumper) {
    IF_ARG_NULL_RETURN(dumper);

    return DUMPER_STATUS_OK;
}

DumperErrors dumperDestructor(Dumper* dumper) {
    IF_ARG_NULL_RETURN(dumper);

    FREE(fileNameBuffer);
    FREE(fileFullNameBuffer);
    *dumper = {};

    return DUMPER_STATUS_OK;
}
