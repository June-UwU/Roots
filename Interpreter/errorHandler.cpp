#include "errorHandler.hpp"
#include <stdlib.h>
#include "logger.hpp"

constexpr const char* ERRORTYPE_STRING[ERRORTYPE]
{
    "ERROR_LEXER"
};

void reportError(ErrorType error)
{
    switch(error)
    {
        case ERROR_LEXER:
            LOG_ERROR("error %d :failed to lexer tokens",error); 
            exit(-1);
        default:
            LOG_ERROR("unknown error %d",error);
    }
}

