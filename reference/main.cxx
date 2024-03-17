#include "repl.hpp"
#include "source.hpp"
#include <iostream>

#include "../roots/logger.hpp"

int main(int argc, char** argv) {
    constexpr const char *main_example = GET_EXAMPLE("main.rts");
    add_source(main_example);
    LOG_TRACE("%s\n", main_example);
    LOG_INFO("%s\n", main_example);
    LOG_WARN("%s\n", main_example);
    LOG_FATAL("%s\n", main_example);
    return EXIT_SUCCESS;
}
