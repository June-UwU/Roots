#pragma once
#include "types.hpp"

typedef enum ErrorType
{
    ERROR_LEXER = 0x0,
    ERRORTYPE
}ErrorType;

void reportError(ErrorType error);

