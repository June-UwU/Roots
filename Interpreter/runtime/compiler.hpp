#pragma once
#include "../types.hpp"
#include "../token.hpp"
#include "instructions.hpp"
#include "rootObjects.hpp"
#include <vector>

constexpr const u32 LOCALS_MAX_RANGE = INT16_MAX;

typedef struct Local
{
    u32 depth;
    u32 count;
    RootObject* objects[LOCALS_MAX_RANGE];
}Local;

typedef struct Compiler
{
    u32 localCount;
    u32 scopeDepth;
    Local locals[LOCALS_MAX_RANGE];
}Compiler;

ByteCode* compile(std::vector<Token> tokens);