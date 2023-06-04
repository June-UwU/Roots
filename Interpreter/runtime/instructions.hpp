#pragma once
#include "../types.hpp"

typedef enum Ins
{
    INS_NOP = 0x0,
    INS_RET,

    INT_SIZE
}Ins;

typedef u8 ByteCode;