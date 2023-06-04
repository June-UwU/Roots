#pragma once
#include "../types.hpp"
#include "../token.hpp"
#include "instructions.hpp"
#include <vector>

ByteCode* compile(std::vector<Token> tokens);