#pragma once
#include "types.hpp"
#include "token.hpp"
#include "expression.hpp"
#include "memory/ArenaAllocators.hpp"
#include <vector>


AstNode* parse(ArenaAllocator& allocator,std::vector<Token>& tokenList);
