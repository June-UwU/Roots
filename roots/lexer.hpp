#pragma once
#include "types.hpp"
#include <string>
#include <vector>

#define TOKENS                                                                                                         \
    X(INVALID, "___invalid___")                                                                                        \
    X(IDENTIFIER, "identifier")                                                                                        \
    X(SOURCE_EOF, "eof")                                                                                               \
    X(NUMERIC_LITERAL, "numeric literal")        \

#define TOKEN_SYMBOLS                                                                                                  \
    X(OPEN_CURLY_BRACE, "{")                                                                                           \
    X(CLOSE_CURLY_BRACE, "}")                                                                                          \
    X(OPEN_SQUARE_BRACE, "[")                                                                                          \
    X(CLOSE_SQUARE_BRACE, "]")                                                                                         \
    X(OPEN_BRACE, "(")                                                                                                 \
    X(CLOSE_BRACE, ")")                                                                                                \
    X(ASTRIX, "*")                                                                                                     \
    X(DASH, "-")                                                                                                       \
    X(PLUS, "+")                                                                                                       \
    X(OPEN_ARROW, "<")                                                                                                 \
    X(CLOSE_ARROW, ">")                                                                                                \
    X(BACK_SLASH, "\\")                                                                                                \
    X(FORWARD_SLASH, "/")                                                                                              \
    X(QUOTES, "\"")                                                                                                    \
    X(HASH, "#")                                                                                                       \
    X(SEMI_COLON, ";")                                                                                                 \
    X(EQUAL, "=")                                                                                                      \
    X(COMMA, ",")                                                                                                      \
    X(COLON, ":")

#define TOKEN_LONG_SYMBOLS X(ARROW, "->")                                                                                                     \


#define TOKEN_KEYWORDS                                                                                                 \
    X(FUNCTION, "fn")                                                                                                  \
    X(INT_8, "s8")                                                                                                     \
    X(INT_16, "s16")                                                                                                   \
    X(INT_32, "s32")                                                                                                   \
    X(INT_64, "s64")                                                                                                   \
    X(UINT_8, "u8")                                                                                                    \
    X(UINT_16, "u16")                                                                                                  \
    X(UINT_32, "u32")                                                                                                  \
    X(UINT_64, "u64")                                                                                                  \
    X(RETURN, "return")                                                                                                \
    X(INCLUDE, "include")                  \


#define GET_TOKEN_KIND_INDEX(token_kind) token_kind - 1 

#define X(token, lexeme) token,

typedef enum token_kind {
    TOKEN_KIND_BEGIN = 0x0,
    TOKENS
    TOKEN_SYMBOLS
    TOKEN_KEYWORDS 
    TOKEN_LONG_SYMBOLS
    TOKEN_KIND_END,
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
