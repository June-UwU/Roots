#include "repl.hpp"
#include "source.hpp"
#include <iostream>

int main(int argc, char** argv) {
    constexpr const char *main_example = GET_EXAMPLE("main.rts");
    add_source(main_example);

    std::string main(main_example);
    print_source(main);

    return EXIT_SUCCESS;
}