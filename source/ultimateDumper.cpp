#include <stdlib.h>

#include "../include/ultimateDumper.hpp"

#define IF_ARG_NULL_RETURN(arg) \
    COMMON_IF_ARG_NULL_RETURN(arg, DUMPER_ERROR_INVALID_ARGUMENT, getDumperErrorMessage)

#define IF_ERR_RETURN(error) \
    COMMON_IF_ERR_RETURN(error, getDumperErrorMessage, DUMPER_STATUS_OK)

#define IF_NOT_COND_RETURN(condition, error) \
    COMMON_IF_NOT_COND_RETURN(condition, error, getDumperErrorMessage)

const size_t FILE_NAME_BUFFER_SIZE = 1 << 14;
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

static DumperErrors addNodeDumpStructToBuffer(Dumper* dumper, const Node* node, char** buffer) {
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(node);
    IF_ARG_NULL_RETURN(buffer);

    snprintf(*buffer, FILE_NAME_BUFFER_SIZE, "\
    %d [shape=none, margin=0, fontcolor=white, color=white, label=< \n\
        <TABLE cellspacing=\"0\"> \n\
            <TR><TD>arrInd: %d</TD></TR> \n\
            <TR><TD>dataInd: %d</TD></TR>\n\
            <TR><TD>next: %d</TD></TR>\n\
            <TR><TD>prev: %d</TD></TR>\n\
        </TABLE> \n\
    >];\n", node->arrInd, node->arrInd, node->data, node->next, node->prev);

    return DUMPER_STATUS_OK;
}

DumperErrors dumperDumpLinkedListNode(Dumper* dumper, const Node* node) {
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(node);

    LOG_DEBUG("node dumping ---------------------");
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

    const size_t BIG_NODE_BUFFER_SIZE = 1 << 10;
    char* buffer = (char*)calloc(BIG_NODE_BUFFER_SIZE, sizeof(char));
    IF_NOT_COND_RETURN(buffer != NULL,
                       DUMPER_ERROR_MEMORY_ALLOCATION_ERROR);
    LOG_DEBUG_VARS(buffer);

    strcat(buffer, "digraph html {\n\
        overlap=false\n\
        splines=ortho\n\
        bgcolor=\"black\"\n\
        rankdir=LR\n\
        pad=0.2\n\
    ");

    memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    IF_ERR_RETURN(addNodeDumpStructToBuffer(dumper, node, &fileNameBuffer));
    strcat(buffer, fileNameBuffer);
    LOG_DEBUG_VARS(buffer);
    strcat(buffer, "}\n");
    fprintf(outputFile, buffer);
    fclose(outputFile);

    memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    sprintf(fileNameBuffer, "%d_node.%s",
            dumper->numberOfLogsBefore, dumper->outputFileFormat);
    LOG_DEBUG_VARS(fileNameBuffer);

    memset(fileFullNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    sprintf(fileFullNameBuffer, "dot -Tpng logs/%d_node.dot -o  %s/images/%s",
            dumper->numberOfLogsBefore, dumper->dirForLogsPath, fileNameBuffer);
    LOG_DEBUG_VARS(fileFullNameBuffer);
    system(fileFullNameBuffer);
    return DUMPER_STATUS_OK;
}

static void drawLabelToGraphvizVert(char* buffer, int nodeToPoint, const char* labelName) {
    assert(buffer != NULL);
    assert(labelName != NULL);

    memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    snprintf(fileNameBuffer, FILE_NAME_BUFFER_SIZE,
        "%sPointer [shape=rect, fontcolor=white, color=white, label=\"%s\"]\n",
        labelName, labelName);
    strcat(buffer, fileNameBuffer);

    memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    snprintf(fileNameBuffer, FILE_NAME_BUFFER_SIZE,
        "%sPointer -> %d [color=orange]\n",
        labelName, nodeToPoint);
    strcat(buffer, fileNameBuffer);
}

static DumperErrors drawMainLinkedList(Dumper* dumper, char* buffer, LinkedList* list) {
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(buffer);

    int curNode = list->fictiveNode;
    for (size_t nodeInd = 0; nodeInd < list->listSize + 1 && nodeInd < dumper->maxNumOfNodesToDraw; ++nodeInd) {
        memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
        // FIXME: rename or add additional buffer for node struct dump string
        //LOG_DEBUG_VARS(buffer);
        IF_ERR_RETURN(addNodeDumpStructToBuffer(dumper, &list->nodes[curNode], &fileNameBuffer));
        //LOG_DEBUG_VARS(curNode, buffer, fileNameBuffer);
        LOG_DEBUG_VARS(strlen(buffer));
        strcat(buffer, fileNameBuffer);

        int nxt = list->nodes[curNode].next;

        int weight = 1;
        if (nodeInd == list->listSize)
            weight = 0;
        memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
        snprintf(fileNameBuffer, FILE_NAME_BUFFER_SIZE,
            "%d -> %d [weight=%d, color=lightblue]\n", curNode, nxt, weight);
        strcat(buffer, fileNameBuffer);

        memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
        snprintf(fileNameBuffer, FILE_NAME_BUFFER_SIZE,
            "%d -> %d [weight=%d, color=pink]\n", nxt, curNode, weight);
        strcat(buffer, fileNameBuffer);

        curNode = nxt;
    }

    drawLabelToGraphvizVert(buffer, list->fictiveNode, "fictive node");

    return DUMPER_STATUS_OK;
}

static DumperErrors drawListOfFreeNodes(Dumper* dumper, char* buffer, LinkedList* list) {
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(buffer);

    LinkedListErrors error = checkIfLinkedListIsValid(list);
    if (error != LINKED_LIST_STATUS_OK) {
        LOG_ERROR(getLinkedListErrorMessage(error));
        return DUMPER_ERROR_INVALID_ARGUMENT; // make normal error
    }

    LOG_DEBUG("drawing free nodes list");
    int curNode = list->freeNodesHead;
    for (size_t nodesDrawn = 0; nodesDrawn < dumper->maxNumOfNodesToDraw && curNode != -1; ++nodesDrawn) {
        LOG_DEBUG_VARS(curNode);
        memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
        IF_ERR_RETURN(addNodeDumpStructToBuffer(dumper, &list->nodes[curNode], &fileNameBuffer));
        strcat(buffer, fileNameBuffer);

        int nxt = list->nodes[curNode].next;

        memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
        snprintf(fileNameBuffer, FILE_NAME_BUFFER_SIZE,
            "%d -> %d [weight=1, color=lightblue]\n", curNode, nxt);
        strcat(buffer, fileNameBuffer);

        curNode = nxt;
    }

    drawLabelToGraphvizVert(buffer, list->freeNodesHead, "free nodes");

    return DUMPER_STATUS_OK;
}

DumperErrors dumperDumpLinkedList(Dumper* dumper, LinkedList* list) {
    IF_ARG_NULL_RETURN(dumper);
    IF_ARG_NULL_RETURN(list);

    // FIXME: FIXME: FIXME: super cringe functions
    // FIXME:
    // FIXME:
    // FIXME:
    // FIXME:



    LOG_DEBUG("list dumping ---------------------");
    ++dumper->numberOfLogsBefore;
    memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    // TODO: rewrite with snprintf
    sprintf(fileNameBuffer, "%d_list.dot",
            dumper->numberOfLogsBefore, dumper->outputFileFormat);
    LOG_DEBUG("ok2");
    memset(fileFullNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    sprintf(fileFullNameBuffer, "%s/%s", dumper->dirForLogsPath, fileNameBuffer);
    LOG_DEBUG_VARS(fileNameBuffer);
    LOG_DEBUG_VARS(fileFullNameBuffer);

    FILE* outputFile = fopen(fileFullNameBuffer, "w");
    IF_NOT_COND_RETURN(outputFile != NULL,
                       DUMPER_ERROR_COULD_OPEN_FILE);

    const size_t BIG_LIST_BUFFER_SIZE = 1 << 15;
    char* buffer = (char*)calloc(BIG_LIST_BUFFER_SIZE, sizeof(char));
    IF_NOT_COND_RETURN(buffer != NULL,
                       DUMPER_ERROR_MEMORY_ALLOCATION_ERROR);

    // FIXME: add errors check
    LOG_DEBUG_VARS("ok");
    int curNode = list->fictiveNode;
    LOG_DEBUG_VARS(curNode, list->fictiveNode);
    strcat(buffer, "digraph html {\n\
        overlap=false\n\
        splines=ortho\n\
        bgcolor=\"black\"\n\
        rankdir=LR\n\
        pad=0.2\n\
    ");

    IF_ERR_RETURN(drawMainLinkedList(dumper, buffer, list));
    IF_ERR_RETURN(drawListOfFreeNodes(dumper, buffer, list));

    strcat(buffer, "}\n");
    LOG_DEBUG_VARS(buffer);


    fprintf(outputFile, buffer);
    fclose(outputFile);

    memset(fileNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    sprintf(fileNameBuffer, "%d_list.%s",
            dumper->numberOfLogsBefore, dumper->outputFileFormat);
    LOG_DEBUG_VARS(fileNameBuffer);

    memset(fileFullNameBuffer, 0, FILE_NAME_BUFFER_SIZE);
    sprintf(fileFullNameBuffer, "dot -Tpng logs/1_list.dot -o %s/images/%s", dumper->dirForLogsPath, fileNameBuffer);
    LOG_DEBUG_VARS(fileFullNameBuffer);
    system(fileFullNameBuffer);

    // FIXME: free buffer on error
    FREE(buffer);

    return DUMPER_STATUS_OK;
}

DumperErrors dumperDestructor(Dumper* dumper) {
    IF_ARG_NULL_RETURN(dumper);

    FREE(fileNameBuffer);
    FREE(fileFullNameBuffer);
    *dumper = {};

    return DUMPER_STATUS_OK;
}
