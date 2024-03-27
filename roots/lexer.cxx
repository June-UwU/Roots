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

token::token(std::string_view lexeme, token_kind kind, u32 line, u32 character)
    : lexeme(lexeme), kind(kind), line(line), character(character) {}

token::token(const token &lhs) {
    kind      = lhs.kind;
    line      = lhs.line;
    lexeme    = lhs.lexeme;
    character = lhs.character;
}

token::token(token &&lhs) {
    line      = lhs.line;
    kind      = lhs.kind;
    lexeme    = lhs.lexeme;
    character = lhs.character;
}

token &token::operator=(const token &lhs) {
    line      = lhs.line;
    kind      = lhs.kind;
    lexeme    = lhs.lexeme;
    character = lhs.character;
    return *this;
}

token &token::operator=(token &&lhs) {
    line      = lhs.line;
    kind      = lhs.kind;
    lexeme    = lhs.lexeme;
    character = lhs.character;
    return *this;
}

std::string_view token::get_lexeme() { return lexeme; }

std::string      token::get_kind_string() {
    u32 string_index = GET_TOKEN_KIND_INDEX(kind);
    return std::string(token_kind_string[string_index]);
}

u32        token::get_character_pos() { return character; }

u32        token::get_line() { return line; }

token_kind token::get_kind() { return kind; }

bool       is_special_token_kind(token_kind &kind) {
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

class lexer_context {
  private:
    u32                                              line;
    u32                                              iter;
    std::string                                      file;
    std::string                                     &source;
    std::unordered_map<std::string_view, token_kind> lexeme_map;

  public:
    lexer_context(u32 line, u32 iter, std::string file, std::string &source)
        : line(line), iter(iter), file(file), source(source) {
        lexeme_map = generate_keyword_map();
    }

    char peek(u32 offset) {
        u32 size        = source.size();
        u32 peek_offset = iter + offset;

        ASSERT(peek_offset < size, "out of bound lexer peek : %d",peek_offset);

        return source[peek_offset];
    }

    void advance(u32 count) {
        u32 size      = source.size() + 1;
        u32 increment = iter + count;

        ASSERT(size > increment, "out of bound source lexer advance : %d", increment);
        iter = increment;
    }

    void retreat(u32 count) {
        u32 increment = iter - count;

        ASSERT(increment >= 0, "out of bound source lexer retreat : %d",increment);

        iter = increment;
    }

    bool eof() { return iter >= source.size(); }

    bool is_space() {
        if (' ' == source[iter]) {
            return true;
        }

        return false;
    }

    bool is_newline() {
        if ('\n' == source[iter]) {
            return true;
        }

        return false;
    }

    bool is_numeric() {
        char current = source[iter];

        return (('0' <= current) && ('9' >= current));
    }

    bool is_dot() { return (source[iter] == '.'); }

    bool is_alphabet() {
        bool is_small = false;
        bool is_large = false;

        char alpha    = source[iter];

        is_small      = (('a' <= alpha) && ('z' >= alpha));
        is_large      = (('A' <= alpha) && ('Z' >= alpha));

        return (is_small || is_large);
    }

    token special_operator() {

        u32 offset_begin = iter;
        u32 character    = iter;

        advance(1);
        if (false == eof()) {
            advance(1);
        }

        auto begin             = source.begin();
        u32  offset_end        = iter;
        auto begin_iter        = begin + offset_begin;
        auto end_iter          = begin + offset_end;

        auto lexeme            = std::string_view(begin_iter, end_iter);

        bool is_multichararter = is_special_token_multicharacter(lexeme);
        if (false == is_multichararter) {
            lexeme = std::string_view(begin_iter, end_iter - 1);
            retreat(1);
        }

        token_kind kind = get_token(lexeme);

        return token(lexeme, kind, line, character);
    }

    bool is_special_character() {
        std::string      may_be_symbol{source[iter]};

        std::string_view symbol_view(may_be_symbol);
        token_kind       kind = get_token(symbol_view);

        return is_special_token_kind(kind);
    }

    token identifier_or_keyword() {

        u32 offset_begin = iter;
        u32 character    = iter;

        while ((true == is_alphabet() || true == is_numeric())) {
            advance(1);
        }

        u32        offset_end = iter;

        auto       begin      = source.begin();
        auto       begin_iter = begin + offset_begin;
        auto       end_iter   = begin + offset_end;

        auto       lexeme     = std::string_view(begin_iter, end_iter);

        token_kind kind       = get_token(lexeme);
        if (INVALID == kind) {
            kind = IDENTIFIER;
        }

        return token(lexeme, kind, line, character);
    }

    bool is_special_token_multicharacter(std::string_view &lexeme) {
        token_kind kind = get_token(lexeme);

        if (INVALID != kind) {
            return true;
        }

        return false;
    }

    token numeric_value() {

        u32 offset_begin = iter;
        u32 character    = iter;

        while (true == is_numeric()) {
            advance(1);
        }

        auto             begin      = source.begin();
        u32              offset_end = iter;
        auto             begin_iter = begin + offset_begin;
        auto             end_iter   = begin + offset_end;

        std::string_view lexeme(begin_iter, end_iter);

        return token(lexeme, NUMERIC_LITERAL, line, character);
    }

    token_kind get_token(std::string_view &lexeme) {
        if (lexeme_map.end() == lexeme_map.find(lexeme)) {
            return INVALID;
        }

        return lexeme_map[lexeme];
    }

    u32          get_line() { return line; }
    u32          get_iter() { return iter; }
    void         next_line() { line = line + 1; }
    std::string  get_file() { return file; }
    std::string &get_source() { return source; }

  private:
    std::unordered_map<std::string_view, token_kind> generate_keyword_map() {
        std::unordered_map<std::string_view, token_kind> token_map;

        for (u32 i = TOKEN_KIND_BEGIN + 1; i < TOKEN_KIND_END; i++) {
            u32         string_index = GET_TOKEN_KIND_INDEX(i);
            const char *string_ptr   = token_string[string_index];
            token_map[string_ptr]    = static_cast<token_kind>(i);
        }
        ASSERT(token_map.size() < TOKEN_KIND_END,
               "token type mapping doesn't match the token specs, size increment of %d",(token_map.size() - TOKEN_KIND_END));

        return token_map;
    }
};

std::vector<token> lex_source(std::string &id) {
    std::string       &source = get_source(id);
    lexer_context      ctx(0, 0, id, source);

    std::vector<token> token_list;

    while (false == ctx.eof()) {
        u32              offset_begin = ctx.get_iter();
        std::string_view lexeme;

        if (true == ctx.is_space()) {
            ctx.advance(1);
        } else if (true == ctx.is_newline()) {
            ctx.advance(1);
            ctx.next_line();
        } else if (true == ctx.is_alphabet()) {

            token alpha_token = ctx.identifier_or_keyword();
            token_list.push_back(std::move(alpha_token));
        } else if (true == ctx.is_numeric()) {

            token number_token = ctx.numeric_value();
            token_list.push_back(std::move(number_token));
        } else if (true == ctx.is_special_character()) {

            token special_token = ctx.special_operator();

            token_list.push_back(std::move(special_token));
        } else {
            auto begin    = ctx.get_source().begin();
            lexeme        = std::string_view(begin, begin + 1);
            u32 character = ctx.get_iter();

            ctx.advance(1);
            token_list.push_back(
                token(lexeme, INVALID, ctx.get_line(), character));
        }

        ASSERT(offset_begin != ctx.get_iter(), "lexer not advancing on %s",
               ctx.get_file())
    }

    std::string_view id_view;
    token_list.push_back(
        token(id_view, SOURCE_EOF, ctx.get_line(), ctx.get_iter()));
    return token_list;
}

void print_token_list(std::vector<token> &token_list) {
    for (auto &token : token_list) {
        std::string lexeme = static_cast<std::string>(token.get_lexeme());
        log_message("%s[%d,%d] %s%s : %s%s\n", ANSI_COLOR_MAGENTA,
                    token.get_line(), token.get_character_pos(),
                    ANSI_COLOR_GREEN, token.get_kind_string().c_str(),
                    ANSI_COLOR_RESET, lexeme.c_str());
    }
}