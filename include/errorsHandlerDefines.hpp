#ifndef COMMON_ERRORS_HANDLER_DEFINES_HPP
#define COMMON_ERRORS_HANDLER_DEFINES_HPP

#include <assert.h>

#include "logLib.hpp"
#include "utils.hpp"

// ASK: how to check if error == OK if all enums are different type
// log and return happens only if error realy occured (error != STATUS_OK)

// FIXME: bruh, auto is C++ feature, can pass error type to define, no big deal
#define COMMON_IF_ERR_RETURN(errorTmp, getErrorMessageFunc, OK_STATUS)                                        \
    do {                                                            \
        auto bruh = errorTmp;                                           \
        if (bruh != OK_STATUS) {                                   \
            LOG_ERROR(getErrorMessageFunc(bruh));                  \
            assert(bruh != OK_STATUS);                             \
            return bruh;                                           \
        }                                                           \
    } while(0)

#define COMMON_IF_NOT_COND_RETURN(condition, error, getErrorMessageFunc)                        \
    do {                                                            \
        bool tmpCondition = (condition);                            \
        if (!tmpCondition) {                                        \
            LOG_ERROR(getErrorMessageFunc(error));                      \
            LOG_DEBUG(#condition);\
            assert(#condition && tmpCondition);                                   \
            return error;                                           \
        }                                                           \
    } while(0)

// ASK: should I create tmp variable for this case too?
#define COMMON_IF_ARG_NULL_RETURN(arg, error, getErrorMessageFunc)                       \
    do {                                                            \
        if (arg == NULL) {                                          \
            LOG_ERROR(getErrorMessageFunc(error));                      \
            assert(arg != NULL);                                    \
            return error;                                           \
        }                                                           \
    } while (0)






#define _my_fe_0(_call, error, getErrorMessage, arg, ...)
#define _my_fe_1(_call, error, getErrorMessage, arg, ...) _call(arg, error, getErrorMessage);
#define _my_fe_2(_call, error, getErrorMessage, arg, ...) _call(arg, error, getErrorMessage); _my_fe_1(_call, error, getErrorMessage, __VA_ARGS__)
#define _my_fe_3(_call, error, getErrorMessage, arg, ...) _call(arg, error, getErrorMessage); _my_fe_2(_call, error, getErrorMessage, __VA_ARGS__)
#define _my_fe_3(_call, error, getErrorMessage, arg, ...) _call(arg, error, getErrorMessage); _my_fe_2(_call, error, getErrorMessage, __VA_ARGS__)
#define _my_fe_4(_call, error, getErrorMessage, arg, ...) _call(arg, error, getErrorMessage); _my_fe_3(_call, error, getErrorMessage, __VA_ARGS__)
#define _my_fe_5(_call, error, getErrorMessage, arg, ...) _call(arg, error, getErrorMessage); _my_fe_4(_call, error, getErrorMessage, __VA_ARGS__)

#define _MY_GET_NTH_ARG(_1, _2, _3, _4, _5, _6, N, ...) N

// ASK: full cringe or can be used in some cases?
#define COMMON_IF_NOT_COND_MULTIPLE_RETURN(error, getErrorMessageFunc, ...)     \
    do {                                                                        \
        _MY_GET_NTH_ARG("ignored", ##__VA_ARGS__, _my_fe_5, _my_fe_4,                    \
            _my_fe_3, _my_fe_2, _my_fe_1, _my_fe_0)                                         \
        (COMMON_IF_NOT_COND_RETURN, error, getErrorMessageFunc, ##__VA_ARGS__);                     \
    } while (0)

#define COMMON_IF_SUBMODULE_ERR_RETURN(errorTmp, getErrorMessageFunc, OK_STATUS, returnError)                                        \
    do {                                                            \
        auto bruh = errorTmp;                                           \
        if (bruh != OK_STATUS) {                                   \
            LOG_ERROR(getErrorMessageFunc(bruh));                  \
            assert(bruh != OK_STATUS);                             \
            return returnError;                                           \
        }                                                           \
    } while(0)


#endif
