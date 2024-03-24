#include "repl.hpp"
#include "roots.hpp"

#include <iostream>

void print_source(std::string &file) { std::cout << get_source(file) << "\n"; }