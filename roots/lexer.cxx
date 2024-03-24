#include "lexer.hpp"
#include "assert.hpp"
#include "roots.hpp"
#include <string_view>
#include <unordered_map>

#define X(token, lexeme) lexeme,

const char *token_string[]{
    TOKENS TOKEN_SYMBOLS TOKEN_KEYWORDS TOKEN_LONG_SYMBOLS};

#undef X

#define X(token, lexeme) #token,

const char *token_kind_string[]{
    TOKENS TOKEN_SYMBOLS TOKEN_KEYWORDS TOKEN_LONG_SYMBOLS};

#undef X

typedef struct lexer_context {
    u32          iter;
    std::string &source;
    std::string  file;
} lexer_context;

std::ostream &operator<<(std::ostream &os, token &token) {
    os << token.get_kind_string() << " : " << token.get_lexeme() << "\n";
    return os;
}

token::token(std::string_view lexeme, token_kind kind)
    : lexeme(lexeme), kind(kind) {}

token::token(token &lhs) {
    lexeme = lhs.lexeme;
    kind   = lhs.kind;
}

token::token(token &&lhs) {
    lexeme = lhs.lexeme;
    kind   = lhs.kind;
}

token &token::operator=(token &lhs) {
    lexeme = lhs.lexeme;
    kind   = lhs.kind;
    return *this;
}

token &token::operator=(token &&lhs) {
    lexeme = lhs.lexeme;
    kind   = lhs.kind;
    return *this;
}

std::string_view token::get_lexeme() { return lexeme; }

std::string      token::get_kind_string() {
    u32 string_index = GET_TOKEN_KIND_INDEX(kind);
    return std::string(token_kind_string[string_index]);
}

token_kind token::get_kind() { return kind; }

bool       source_eof(lexer_context &context) {
    u32 size = context.source.size();
    return context.iter >= size;
}

bool is_space(lexer_context &context) {
    u32 iter = context.iter;
    if (' ' == context.source[iter]) {
        return true;
    }

    return false;
}

token_kind
get_token(std::string_view                                 &lexeme,
          std::unordered_map<std::string_view, token_kind> &lexeme_map) {
    if (lexeme_map.end() == lexeme_map.find(lexeme)) {
        return INVALID;
    }

    return lexeme_map[lexeme];
}

char lexer_peek(lexer_context &context, u32 offset) {
    u32 iter        = context.iter;
    u32 size        = context.source.size();
    u32 peek_offset = iter + offset;

    ASSERT(peek_offset < size, "out of bound lexer peek");

    return context.source[peek_offset];
}

void lexer_advance(lexer_context &context, u32 count) {
    u32 size      = context.source.size() + 1;
    u32 increment = context.iter + count;

    ASSERT(size > increment, "out of bound source lexer advance");

    context.iter = increment;
}

void lexer_retreat(lexer_context &context, u32 count) {
    u32 increment = context.iter - count;

    ASSERT(context.iter >= 0, "out of bound source lexer retreat");

    context.iter = increment;
}

std::unordered_map<std::string_view, token_kind> generate_keyword_map() {
    std::unordered_map<std::string_view, token_kind> token_map;

    for (u32 i = TOKEN_KIND_BEGIN + 1; i < TOKEN_KIND_END; i++) {
        u32         string_index = GET_TOKEN_KIND_INDEX(i);
        const char *string_ptr   = token_string[string_index];
        token_map[string_ptr]    = static_cast<token_kind>(i);
    }
    ASSERT(token_map.size() < TOKEN_KIND_END,
           "token type mapping doesn't match the token specs");

    return token_map;
}

bool is_newline(lexer_context &context) {
    u32 iter = context.iter;
    if ('\n' == context.source[iter]) {
        return true;
    }

    return false;
}

bool is_numeric(lexer_context &context) {
    u32  iter    = context.iter;
    char current = context.source[iter];

    return (('0' <= current) && ('9' >= current));
}

bool is_dot(lexer_context &context) {
    u32 iter = context.iter;

    return (context.source[iter] == '.');
}

bool is_alphabet(lexer_context &context) {
    bool is_small = false;
    bool is_large = false;

    u32  iter     = context.iter;
    char alpha    = context.source[iter];

    is_small      = (('a' <= alpha) && ('z' >= alpha));
    is_large      = (('A' <= alpha) && ('Z' >= alpha));

    return (is_small || is_large);
}

bool is_special_token_kind(token_kind &kind) {
    bool is_special = false;
    switch (kind) {
#define X(token, lexeme)                                                       \
    case token: {                                                              \
        is_special = true;                                                     \
    } break;

        TOKEN_SYMBOLS

#undef X
    }

    return kind;
}

bool is_special_character(
    lexer_context                                    &context,
    std::unordered_map<std::string_view, token_kind> &lexeme_map) {
    u32              iter = context.iter;
    std::string      may_be_symbol{context.source[iter]};

    std::string_view symbol_view(may_be_symbol);
    token_kind       kind = get_token(symbol_view, lexeme_map);

    return is_special_token_kind(kind);
}

bool is_special_token_multicharacter(
    std::string_view                                 &lexeme,
    std::unordered_map<std::string_view, token_kind> &lexeme_map) {
    token_kind kind = get_token(lexeme, lexeme_map);

    if (INVALID != kind) {
        return true;
    }

    return false;
}

token identifier_or_keyword(
    lexer_context                                    &context,
    std::unordered_map<std::string_view, token_kind> &lexeme_map) {

    u32 offset_begin = context.iter;

    while ((true == is_alphabet(context) || true == is_numeric(context))) {
        lexer_advance(context, 1);
    }

    u32        offset_end = context.iter;

    auto       begin      = context.source.begin();
    auto       begin_iter = begin + offset_begin;
    auto       end_iter   = begin + offset_end;

    auto       lexeme     = std::string_view(begin_iter, end_iter);

    token_kind kind       = get_token(lexeme, lexeme_map);
    if (INVALID == kind) {
        kind = IDENTIFIER;
    }

    return token(lexeme, kind);
}

token numeric_value(
    lexer_context                                    &context,
    std::unordered_map<std::string_view, token_kind> &lexeme_map) {

    u32 offset_begin = context.iter;

    while (true == is_numeric(context)) {
        lexer_advance(context, 1);
    }

    auto             begin      = context.source.begin();
    u32              offset_end = context.iter;
    auto             begin_iter = begin + offset_begin;
    auto             end_iter   = begin + offset_end;

    std::string_view lexeme(begin_iter, end_iter);

    return token(lexeme, NUMERIC_LITERAL);
}

token special_operator(
    lexer_context                                    &context,
    std::unordered_map<std::string_view, token_kind> &lexeme_map) {

    u32 offset_begin = context.iter;

    lexer_advance(context, 1);
    if (false == source_eof(context)) {
        lexer_advance(context, 1);
    }

    auto begin      = context.source.begin();
    u32  offset_end = context.iter;
    auto begin_iter = begin + offset_begin;
    auto end_iter   = begin + offset_end;

    auto lexeme     = std::string_view(begin_iter, end_iter);

    bool is_multichararter =
        is_special_token_multicharacter(lexeme, lexeme_map);
    if (false == is_multichararter) {
        lexeme = std::string_view(begin_iter, end_iter - 1);
        lexer_retreat(context, 1);
    }

    token_kind kind = get_token(lexeme, lexeme_map);

    return token(lexeme, kind);
}

std::vector<token> lex_source(std::string &id) {
    std::string                                     &source = get_source(id);

    lexer_context                                    ctx{0, source, id};
    std::unordered_map<std::string_view, token_kind> lexeme_map =
        generate_keyword_map();
    std::vector<token> token_list;

    while (false == source_eof(ctx)) {
        u32              offset_begin = ctx.iter;
        std::string_view lexeme;

        if ((true == is_space(ctx) || true == is_newline(ctx))) {
            lexer_advance(ctx, 1);
        } else if (true == is_alphabet(ctx)) {

            token alpha_token = identifier_or_keyword(ctx, lexeme_map);

            token_list.push_back(std::move(alpha_token));
        } else if (true == is_numeric(ctx)) {

            token number_token = numeric_value(ctx, lexeme_map);

            token_list.push_back(std::move(number_token));
        } else if (true == is_special_character(ctx, lexeme_map)) {

            token special_token = special_operator(ctx, lexeme_map);

            token_list.push_back(std::move(special_token));
        } else {
            auto begin = ctx.source.begin();
            lexeme     = std::string_view(begin, begin + 1);
            lexer_advance(ctx, 1);
            token_list.push_back(token(lexeme, INVALID));
        }

        ASSERT(offset_begin != ctx.iter, "lexer should not advancing on %s",
               ctx.file)
    }

    std::string_view id_view;
    token_list.push_back(token(id_view, SOURCE_EOF));
    return token_list;
}

void print_token_list(std::vector<token> &token_list) {
    for (auto &token : token_list) {
        std::cout << token;
    }
}