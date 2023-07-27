#pragma once
#include "../types.hpp"

constexpr const u8 OPER_FLAG_CONST = 0x1;
constexpr const u8 OPER_FLAG_OBJ = 0x2;

typedef enum Ins
{
    INS_RET = 0x0,
    INS_PUSH,
    INS_POP,
    INS_NEGATE,
    INS_ADD,
    INS_SUB,
    INS_DIV,
    INS_MUL,
    INS_MOV,

    INS_END,
    INS_SIZE,
}Ins;

constexpr const char* INS_STR[INS_SIZE]
{
    "ret","push","pop","neg","add","sub","div","mul","mov",
    "end"
};

typedef u8 ByteCode;