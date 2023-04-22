#pragma once
#include "types.hpp"
#include <string>

Error runInteractive();
Error runFiles(const std::string filePath);
Error run();
