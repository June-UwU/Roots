#pragma once
#include "types.hpp"
#include <vector>
#include <string>

#define TOKENS                  \
    X(INVALID)                  \
    X(IDENTIFIER)               \
    X(OPEN_CURLY_BRACE)         \
    X(CLOSE_CURLY_BRACE)        \
    X(OPEN_SQUARE_BRACE)        \
    X(CLOSE_SQUARE_BRACE)       \
    X(OPEN_BRACE)               \
    X(CLOSE_BRACE)              \
    X(ASTRIX)                   \
    X(DASH)                     \
    X(PLUS)                     \
    X(OPEN_ARROW)               \
    X(CLOSE_ARROW)              \
    X(BACK_SLASH)               \
    X(FORWARD_SLASH)            \
    X(QUOTES)                   \
    X(HASH)                     \
    X(SEMI_COLON)               \
    X(EQUAL)                    \
    X(NUMERIC_LITERAL)          \
    X(ARROW)                    \
    X(COMMA)                    \
    X(COLON)                    \

#define X(token) token,

typedef enum token_kind {
    TOKEN_PREFIX = -1,
    TOKENS
    TOKEN_KIND_SIZE
} token_kind;

#undef X


class token {
  public:
    token(std::string_view lexeme, token_kind kind);
    token(token &lhs);
    token(token &&rhs);

    token &operator=(token &lhs);
    token &operator=(token &&lhs);

    std::string_view get_lexeme();
    std::string get_kind_string();
    token_kind get_kind();
  private:
    std::string_view lexeme;
    token_kind kind;
};


std::vector<token> lex_source(std::string &id);
void print_token_list(std::vector<token> &token_list);
