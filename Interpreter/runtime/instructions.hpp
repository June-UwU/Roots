#pragma once
#include "../types.hpp"

typedef enum Ins
{
    INS_RET = 0x0,
    
    INS_END,
    INS_SIZE,
}Ins;

constexpr const char* INS_STR[INS_SIZE]
{
    "ret","end"
};

typedef u8 ByteCode;