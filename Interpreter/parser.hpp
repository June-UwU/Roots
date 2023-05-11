#pragma once
#include "types.hpp"
#include "token.hpp"
#include "Ast.hpp"
#include "expression.hpp"
#include <vector>


AstNode* parse(std::vector<Token>& tokenList);