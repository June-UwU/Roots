#pragma once
#include <string>
#include "types.hpp"
#include <variant>

typedef enum TokenType
{
    // KEYWORDS
    INT8 = 0x0,
    INT16,
    INT32,
    INT64,
    UINT8,
    UINT16,
    UINT32,
    UINT64,
    FLOAT32,
    FLOAT64,
    CLASS,
    TRUE,
    FALSE,
    AND,
    OR, 
    THIS,
    SUPER,
    RETURN,
    NONE,
    FOR,
    IF,
    // LITERALS
    STRING,
    IDENTIFIER,
    UNSIGNED_INTEGER_NUMBER,
    SIGNED_INTEGER_NUMBER,
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
    // END OF FILE
    END_OF_FILE,
    TOKENTYPE
}TokenType;

using Literal = std::variant<s32,u32,f32,std::string>; 

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

