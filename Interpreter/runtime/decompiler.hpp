#pragma once
#include "../types.hpp"
#include "instructions.hpp"

typedef enum OutputOption
{
    TEXT_FILE = 0x0,
    CONSOLE_DUMP
}OutputOption;

void decompile(ByteCode* code,OutputOption option);