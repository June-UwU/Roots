#pragma once
#include "types.hpp"

bool add_source_to_context(std::string &path, FILE *stream);
std::string get_source(std::string &path);