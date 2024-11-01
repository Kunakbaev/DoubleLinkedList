#include <stdlib.h>

#include "../include/ultimateDumper.hpp"

#define IF_ARG_NULL_RETURN(arg) \
    COMMON_IF_ARG_NULL_RETURN(arg, DUMPER_ERROR_INVALID_ARGUMENT, getDumperErrorMessage)

#define IF_ERR_RETURN(error) \
    COMMON_IF_ERR_RETURN(error, getDumperErrorMessage, DUMPER_STATUS_OK)

#define IF_NOT_COND_RETURN(condition, error) \
    COMMON_IF_NOT_COND_RETURN(condition, error, getDumperErrorMessage)

#define LIST_ERR_CHECK(error) \
    COMMON_IF_SUBMODULE_ERR_RETURN(error, getLinkedListErrorMessage, LINKED_LIST_STATUS_OK, DUMPER_ERROR_LIST_SUBLIB_ERROR);

const size_t FILE_NAME_BUFFER_SIZE      = 100;
const size_t FULL_FILE_NAME_BUFFER_SIZE = 200;
const size_t BUFFER_SIZE                = 1 << 13;
const size_t TMP_BUFFER_SIZE            = 1 << 10;

char* fileFullNameBuffer                = NULL;
char* fileNameBuffer                    = NULL;
char* tmpBuffer                         = NULL; // ASK: how to get rid of it? Like, make strcat and sprintf at the same time
char* buffer                            = NULL;

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
        case DUMPER_ERROR_LIST_SUBLIB_ERROR:
            return "Dumper error: error occured in linked list sublib, possibly list is invalid.\n";

        default:
            return "Dumper error: unknown error.\n";
    }
}

DumperErrors dumperConstructor(Dumper* dumper,
                               size_t maxNumOfNodesToDraw,
                               const char* dirForLogsPath,
                               const char* outputFileFormat) {
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(dirForLogsPath);
    IF_ARG_NULL_RETURN(outputFileFormat);

    *dumper = {};
    dumper->maxNumOfNodesToDraw = maxNumOfNodesToDraw;
    dumper->dirForLogsPath      = dirForLogsPath;
    dumper->outputFileFormat    = outputFileFormat;
    dumper->numberOfLogsBefore  = 0;

    fileFullNameBuffer = (char*)calloc(FULL_FILE_NAME_BUFFER_SIZE, sizeof(char));
    IF_NOT_COND_RETURN(fileFullNameBuffer != NULL,
                       DUMPER_ERROR_MEMORY_ALLOCATION_ERROR);
    fileNameBuffer = (char*)calloc(FILE_NAME_BUFFER_SIZE, sizeof(char));
    IF_NOT_COND_RETURN(fileNameBuffer != NULL,
                       DUMPER_ERROR_MEMORY_ALLOCATION_ERROR);
    tmpBuffer = (char*)calloc(TMP_BUFFER_SIZE, sizeof(char));
    IF_NOT_COND_RETURN(tmpBuffer != NULL,
                       DUMPER_ERROR_MEMORY_ALLOCATION_ERROR);
    buffer = (char*)calloc(BUFFER_SIZE, sizeof(char));
    IF_NOT_COND_RETURN(buffer != NULL,
                       DUMPER_ERROR_MEMORY_ALLOCATION_ERROR);

    system("rm -rf logs");
    system("mkdir -p logs");
    system("mkdir -p logs/images");
    system("mkdir -p logs/html");
    system("mkdir -p logs/dots");

    return DUMPER_STATUS_OK;
}

static DumperErrors addNodeDumpStructToBuffer(Dumper* dumper, const Node* node) {
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(node);
    IF_ARG_NULL_RETURN(buffer);

    memset(tmpBuffer, 0, TMP_BUFFER_SIZE);
    snprintf(tmpBuffer, TMP_BUFFER_SIZE, "\
    %d [shape=none, margin=0, fontcolor=white, color=white, label=< \n\
        <TABLE cellspacing=\"0\"> \n\
            <TR><TD>arrInd: %d</TD></TR> \n\
            <TR><TD>dataInd: %d</TD></TR>\n\
            <TR><TD>next: %d</TD></TR>\n\
            <TR><TD>prev: %d</TD></TR>\n\
        </TABLE> \n\
    >];\n", node->arrInd, node->arrInd, node->data, node->next, node->prev);

    strncat(buffer, tmpBuffer, BUFFER_SIZE);

    return DUMPER_STATUS_OK;
}

DumperErrors dumperDumpLinkedListNode(Dumper* dumper, const Node* node) {
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(node);

    LOG_DEBUG("node dumping ---------------------");
    ++dumper->numberOfLogsBefore;
    memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    snprintf(fileNameBuffer, FILE_NAME_BUFFER_SIZE,
             "dots/%d_node.dot", dumper->numberOfLogsBefore);

    memset(fileFullNameBuffer, 0, FULL_FILE_NAME_BUFFER_SIZE);
    snprintf(fileFullNameBuffer, FULL_FILE_NAME_BUFFER_SIZE,
             "%s/%s", dumper->dirForLogsPath, fileNameBuffer);

    FILE* outputFile = fopen(fileFullNameBuffer, "w");
    IF_NOT_COND_RETURN(outputFile != NULL,
                       DUMPER_ERROR_COULD_OPEN_FILE);

    memset(buffer, 0, BUFFER_SIZE);
    strncat(buffer, "digraph html {\n\
        overlap=false\n\
        splines=ortho\n\
        bgcolor=\"black\"\n\
        rankdir=LR\n\
        pad=0.2\n\
    ", BUFFER_SIZE);

    IF_ERR_RETURN(addNodeDumpStructToBuffer(dumper, node));
    strcat(buffer, "}\n");
    fprintf(outputFile, buffer);
    fclose(outputFile);

    memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    snprintf(fileNameBuffer, FILE_NAME_BUFFER_SIZE, "%d_node.%s",
            dumper->numberOfLogsBefore, dumper->outputFileFormat);

    memset(fileFullNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    snprintf(fileFullNameBuffer, FULL_FILE_NAME_BUFFER_SIZE,
            "dot -Tpng logs/dots/%d_node.dot -o  %s/images/%s",
            dumper->numberOfLogsBefore, dumper->dirForLogsPath, fileNameBuffer);
    system(fileFullNameBuffer);
    return DUMPER_STATUS_OK;
}

static void drawLabelToGraphvizVert(int nodeToPoint, const char* labelName) {
    assert(buffer != NULL);
    assert(labelName != NULL);

    memset(tmpBuffer, 0, TMP_BUFFER_SIZE);
    snprintf(tmpBuffer, TMP_BUFFER_SIZE,
        "%sPointer [shape=rect, fontcolor=white, color=white, label=\"%s\"]\n",
        labelName, labelName);
    strncat(buffer, tmpBuffer, BUFFER_SIZE);

    memset(tmpBuffer, 0, TMP_BUFFER_SIZE);
    snprintf(tmpBuffer, TMP_BUFFER_SIZE,
        "%sPointer -> %d [color=orange]\n",
        labelName, nodeToPoint);
    strncat(buffer, tmpBuffer, BUFFER_SIZE);
}

static DumperErrors drawMainLinkedList(Dumper* dumper, LinkedList* list) {
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(buffer);

    int curNode = list->fictiveNode;
    for (size_t nodeInd = 0; nodeInd < list->listSize + 1 && nodeInd < dumper->maxNumOfNodesToDraw; ++nodeInd) {
        // FIXME: rename or add additional buffer for node struct dump string
        IF_ERR_RETURN(addNodeDumpStructToBuffer(dumper, &list->nodes[curNode]));

        int weight = 1;
        int nxt = list->nodes[curNode].next;
        if (nodeInd == list->listSize)
            weight = 0;
        memset(tmpBuffer, 0, TMP_BUFFER_SIZE);
        snprintf(tmpBuffer, TMP_BUFFER_SIZE,
            "%d -> %d [weight=%d, color=lightblue]\n", curNode, nxt, weight);
        strncat(buffer, tmpBuffer, BUFFER_SIZE);

        memset(tmpBuffer, 0, TMP_BUFFER_SIZE);
        snprintf(tmpBuffer, TMP_BUFFER_SIZE,
            "%d -> %d [weight=%d, color=pink]\n", nxt, curNode, weight);
        strncat(buffer, tmpBuffer, BUFFER_SIZE);

        curNode = nxt;
    }

    drawLabelToGraphvizVert(list->fictiveNode, "fictive node");

    return DUMPER_STATUS_OK;
}

static DumperErrors drawListOfFreeNodes(Dumper* dumper, LinkedList* list) {
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(buffer);
    LIST_ERR_CHECK(checkIfLinkedListIsValid(list));

    LOG_DEBUG("drawing free nodes list");
    int curNode = list->freeNodesHead;
    for (size_t nodesDrawn = 0; nodesDrawn < dumper->maxNumOfNodesToDraw && curNode != -1; ++nodesDrawn) {
        IF_ERR_RETURN(addNodeDumpStructToBuffer(dumper, &list->nodes[curNode]));

        int nxt = list->nodes[curNode].next;
        memset(tmpBuffer, 0, TMP_BUFFER_SIZE);
        snprintf(tmpBuffer, TMP_BUFFER_SIZE,
            "%d -> %d [weight=1, color=lightblue]\n", curNode, nxt);
        strncat(buffer, tmpBuffer, BUFFER_SIZE);

        curNode = nxt;
    }

    drawLabelToGraphvizVert(list->freeNodesHead, "free nodes");

    return DUMPER_STATUS_OK;
}

DumperErrors dumperDumpLinkedList(Dumper* dumper, LinkedList* list) {
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(list);
    LIST_ERR_CHECK(checkIfLinkedListIsValid(list));

    // FIXME: FIXME: FIXME: super cringe functions
    // FIXME:
    // FIXME:
    // FIXME:
    // FIXME:



    LOG_DEBUG("list dumping ---------------------");
    ++dumper->numberOfLogsBefore;
    memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    // TODO: rewrite with snprintf
    snprintf(fileNameBuffer, FILE_NAME_BUFFER_SIZE, "dots/%d_list.dot",
             dumper->numberOfLogsBefore, dumper->outputFileFormat);

    memset(fileFullNameBuffer, 0, FULL_FILE_NAME_BUFFER_SIZE);
    snprintf(fileFullNameBuffer, FULL_FILE_NAME_BUFFER_SIZE,
             "%s/%s", dumper->dirForLogsPath, fileNameBuffer);

    LOG_DEBUG_VARS(fileFullNameBuffer, fileNameBuffer);
    FILE* outputFile = fopen(fileFullNameBuffer, "w");
    IF_NOT_COND_RETURN(outputFile != NULL,
                       DUMPER_ERROR_COULD_OPEN_FILE);

    memset(buffer, 0, BUFFER_SIZE);
    // FIXME: add errors check
    int curNode = list->fictiveNode;
    LOG_DEBUG_VARS(curNode, list->fictiveNode);
    strncat(buffer, "digraph html {\n\
        overlap=false\n\
        splines=ortho\n\
        bgcolor=\"black\"\n\
        rankdir=LR\n\
        pad=0.2\n\
    ", BUFFER_SIZE);

    IF_ERR_RETURN(drawMainLinkedList(dumper, list));
    IF_ERR_RETURN(drawListOfFreeNodes(dumper, list));

    strncat(buffer, "}\n", BUFFER_SIZE);
    LOG_DEBUG_VARS(buffer);
    fprintf(outputFile, buffer);
    fclose(outputFile);

    memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    snprintf(fileNameBuffer, FILE_NAME_BUFFER_SIZE,
            "%d_list.%s", dumper->numberOfLogsBefore, dumper->outputFileFormat);
    LOG_DEBUG_VARS(fileNameBuffer);

    memset(fileFullNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    snprintf(fileFullNameBuffer, FULL_FILE_NAME_BUFFER_SIZE,
             "dot -Tpng logs/dots/%d_list.dot -o %s/images/%s",
            dumper->numberOfLogsBefore, dumper->dirForLogsPath, fileNameBuffer);
    LOG_DEBUG_VARS(fileFullNameBuffer);
    system(fileFullNameBuffer);

    // FIXME: free buffer on error

    return DUMPER_STATUS_OK;
}

DumperErrors dumperDestructor(Dumper* dumper) {
    IF_ARG_NULL_RETURN(dumper);

    FREE(fileNameBuffer);
    FREE(tmpBuffer);
    FREE(buffer);
    FREE(fileFullNameBuffer);
    *dumper = {};

    return DUMPER_STATUS_OK;
}
