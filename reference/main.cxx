#include "repl.hpp"
#include "source.hpp"
#include <iostream>

#include "../roots/lexer.hpp"

int main(int argc, char** argv) {
    constexpr const char *main_example = GET_EXAMPLE("main.rts");
    add_source(main_example);

    std::string main(main_example);
    std::vector<token> token_list = lex_source(main);
    print_token_list(token_list);

    return EXIT_SUCCESS;
}
