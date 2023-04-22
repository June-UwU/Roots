#pragma once
#include "types.hpp"
#include "token.hpp"
#include <string>
#include <vector>

Error lexInteractive(std::vector<Token>& tokenList, std::string statement);
std::vector<Token> lexFile(const std::string filePath);

