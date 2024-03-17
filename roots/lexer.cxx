#include "lexer.hpp"
#include "roots.hpp"
#include "assert.hpp"
#include <string_view>

#define X(token) #token,

const char *token_string[]{
    TOKENS
};

#undef X

typedef struct lexer_context {
    u32 iter;
    std::string& source;
    std::string file;
} lexer_context;

std::ostream &operator<<(std::ostream &os, token &token) {
    os << token.get_kind_string() << " : " << token.get_lexeme() << "\n";
    return os;
}

token::token(std::string_view lexeme, token_kind kind) 
    : lexeme(lexeme)
    , kind(kind)
{
}

token::token(token &lhs) {
    lexeme = lhs.lexeme;
    kind = lhs.kind;
}

token::token(token &&lhs) {
    lexeme = lhs.lexeme;
    kind = lhs.kind;
}

token &token::operator=(token &lhs) {
    lexeme = lhs.lexeme;
    kind = lhs.kind;
    return *this;
}

token &token::operator=(token &&lhs) {
    lexeme = lhs.lexeme;
    kind = lhs.kind;
    return *this;
}

std::string_view token::get_lexeme() {
    return lexeme;
}

std::string token::get_kind_string() {
    return std::string(token_string[kind]);
}

token_kind token::get_kind(){
    return kind;
}

bool source_eof(lexer_context &context) {
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

char lexer_peek(lexer_context &context, u32 offset)
{
    u32 iter = context.iter;
    u32 size = context.source.size();
    u32 peek_offset = iter + offset;

    ASSERT(peek_offset < size, "out of bound lexer peek");

    return context.source[peek_offset];
}

void lexer_advance(lexer_context &context, u32 count)
{
    u32 size = context.source.size() + 1;
    u32 increment = context.iter + count;

    ASSERT(size > increment, "out of bound source lexer advance");

    context.iter = increment;
}

bool is_newline(lexer_context &context) {
    u32 iter = context.iter;
    if ('\n' == context.source[iter]) {
        return true;
    }

    return false;
}

bool is_numeric(lexer_context &context) {
    u32 iter = context.iter;
    char current = context.source[iter];

    return (('0' <= current) && ('9' >= current));
}

bool is_dot(lexer_context &context) {
    u32 iter = context.iter;

    return (context.source[iter] == '.');
}

bool is_alphabet(lexer_context &context){
    bool is_small = false;
    bool is_large = false;

    u32 iter = context.iter;
    char alpha = context.source[iter];

    is_small = (('a' <= alpha) && ('z' >= alpha));
    is_large = (('A' <= alpha) && ('Z' >= alpha));

    return (is_small || is_large);
}

bool is_special_character(lexer_context &context) {
    u32 iter = context.iter;
    char alpha = context.source[iter];
    
    bool is_special = false;

    switch (alpha) {
    case '#':
    case '{':
    case '}':
    case '[':
    case ']':
    case '-':
    case '+':
    case '*':
    case '>':
    case '<':
    case '"':
    case '/':
    case '\\':
    case '(':
    case ')':
    case ';':
    case '=':
    case ',':
    case ':':
        is_special = true;
    }

    return is_special;
}

token_kind special_token_kind(lexer_context &context) {
    u32 iter = context.iter;
    char alpha = context.source[iter];

    token_kind kind = INVALID;
    switch (alpha) {
    case '=':
        kind = EQUAL;
        break;
    case '{':
        kind = OPEN_CURLY_BRACE;
        break;
    case '}':
        kind = CLOSE_CURLY_BRACE;
        break;
    case '[':
        kind = OPEN_SQUARE_BRACE;
        break;
    case ']':
        kind = CLOSE_SQUARE_BRACE;
        break;
    case '-':
        kind = DASH;
        break;
    case '+':
        kind = PLUS;
        break;
    case '*':
        kind = ASTRIX;
        break;
    case '>':
        kind = CLOSE_ARROW;
        break;
    case '<':
        kind = OPEN_ARROW;
        break;
    case '"':
        kind = QUOTES;
        break;
    case '/':
        kind = BACK_SLASH;
        break;
    case '\\':
        kind = FORWARD_SLASH;
        break;
    case '(':
        kind = OPEN_BRACE;
        break;
    case ')':
        kind = CLOSE_BRACE;
        break;
    case '#':
        kind = HASH;
        break;
    case ';':
        kind = SEMI_COLON;
        break;
    case ',':
        kind = COMMA;
        break;
    case ':':
        kind = COLON;
        break;
    }

    return kind;
}

token_kind special_token_multicharacter(lexer_context &context, token_kind prev) {
    token_kind current_token = special_token_kind(context);

    token_kind parsed_token = prev;
    switch (prev) {
    case DASH:
        if (CLOSE_ARROW == current_token) {
            parsed_token = ARROW;
            lexer_advance(context, 1);
        }
        break;
    }

    return parsed_token;
}

std::vector<token> lex_source(std::string &id) {
    std::string &source = get_source(id);

    lexer_context ctx{0,source,id};
    std::vector<token> token_list;

    while (false == source_eof(ctx)) {
        u32 offset_begin = ctx.iter;
        
        std::string_view lexeme;
        auto begin = ctx.source.begin();

        if ((true == is_space(ctx) || true == is_newline(ctx))) {
            lexer_advance(ctx, 1);
        }
        else if (true == is_alphabet(ctx)) {

            while ((true == is_alphabet(ctx) || true == is_numeric(ctx))) {
                lexer_advance(ctx,1);
            }

            u32 offset_end = ctx.iter;
            
            auto begin_iter = begin + offset_begin; 
            auto end_iter = begin + offset_end;

            lexeme = std::string_view(begin_iter, end_iter);
            
            token_list.push_back(token(lexeme, IDENTIFIER));
        }
        else if (true == is_special_character(ctx)) {
            token_kind kind = special_token_kind(ctx);
            lexer_advance(ctx, 1);
            kind = special_token_multicharacter(ctx, kind);

            u32 offset_end = ctx.iter;
            auto begin_iter = begin + offset_begin;
            auto end_iter = begin + offset_end;

            lexeme = std::string_view(begin_iter, end_iter);
            token_list.push_back(token(lexeme, kind));
        }
        else if (true == is_numeric(ctx)) {
            while (true == is_numeric(ctx)) {
                lexer_advance(ctx,1);
            }

            u32 offset_end = ctx.iter;
            auto begin_iter = begin + offset_begin;
            auto end_iter  = begin + offset_end;

            std::string_view lexeme(begin_iter, end_iter);
            token_list.push_back(token(lexeme, NUMERIC_LITERAL));
        }
        else {
            lexer_advance(ctx, 1);
            token_list.push_back(token(lexeme, INVALID));
        }

        ASSERT(offset_begin != ctx.iter, "lexer should not advancing on %s",ctx.file)
    }

    return token_list;
}


void print_token_list(std::vector<token> &token_list) {
    for (auto &token : token_list) {
        std::cout << token;
    }
}