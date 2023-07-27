#include "compiler.hpp"
#include <cstring>

static Compiler* COMPILER = nullptr;

void initializeCompiler()
{
    COMPILER = new Compiler;
    COMPILER->localCount = 0;
    COMPILER->scopeDepth = 0;
    std::memset(COMPILER->locals,0,sizeof(Local) * LOCALS_MAX_RANGE);
}
void destroyCompiler();

ByteCode* compile(std::vector<Token> tokens)
{
    return nullptr;
}