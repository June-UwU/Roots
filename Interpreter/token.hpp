#pragma once
#include <string>
#include "types.hpp"
#include <variant>

typedef enum TokenType
{
    // KEYWORDS
    VAR = 0x0,
    CLASS,
    TRUE,
    FALSE,
    AND,
    OR, 
    THIS,
    SUPER,
    RETURN,
    NONE,
    WHILE,
    FOR,
    IF,
    // LITERALS
    STRING,
    IDENTIFIER,
    FLOAT_NUMBER,
    // CHARACTER TOKENS
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    LESS_EQUAL,
    GREATER_EQUAL,
    LESS,
    GREATER,
    // 
    DOT,
    MINUS,
    PLUS,
    SLASH,
    STAR,
    SEMI_COLON,
    COLON,
    COMMA,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_PAREN,
    RIGHT_PAREN,
    QUOTES,
    // END OF FILE
    END_OF_FILE,
    TOKENTYPE
}TokenType;

using Literal = std::variant<f64,std::string>; 

class Token
{
    public:
        Token() = default;
        Token(const TokenType type, const std::string lexeme, 
                Literal literal ,u32 line, const std::string file);
        TokenType getTokenType() const;
        std::string getLexeme() const;
        Literal getLiteral() const;
        u32 getLine() const;
        std::string getFile() const;
        std::string toString() const;
    private:
        TokenType type;
        std::string lexeme;
        Literal literal;
        u32 line;
        std::string file;
};

TokenType findKeywordOrIdentifier(const std::string string);
