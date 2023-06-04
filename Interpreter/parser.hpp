#pragma once
#include "types.hpp"
#include "token.hpp"
#include "expression.hpp"
#include "memory/ArenaAllocators.hpp"
#include <vector>


Statement* parse(ArenaAllocator& allocator,std::vector<Token>& tokenList, u32& currentIndex, Block* owner);
