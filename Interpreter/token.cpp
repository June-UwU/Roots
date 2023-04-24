#include "token.hpp"
#include <sstream>
#include <string.h>

constexpr const u32 KEYWORD_COUNT = STRING;
constexpr const char* KEYWORD_LIST[KEYWORD_COUNT]
{
    "i8","i16","i32","i64","u8","u16","u32","u64","f32",
    "f64","class","true","false","&","|","this","super","return",
    "null","for","if"
};

constexpr const char* TOKENTYPE_STRING[TOKENTYPE]
{
    "INT8","INT16","INT32","INT64","UINT8","UINT16","UINT32","UINT64",
    "FLOAT32","FLOAT64","CLASS","TRUE","FALSE","AND","OR","THIS","SUPER",
    "RETURN","NONE","FOR","IF","STRING","IDENTIFIER","UNSIGNED_INTEGER_NUMBER",
    "SIGNED_INTEGER_NUMBER","FLOAT_NUMBER","BANG","BANG_EQUAL","EQUAL","EQUAL_EQUAL",
    "LESS_EQUAL","GREATER_EQUAL","LESS","GREATER","DOT","MINUS","PLUS","SLASH",
    "STAR","SEMI_COLON","COLON","COMMA","LEFT_BRACE","RIGHT_BRACE","LEFT_PAREN",
    "RIGHT_PAREN","QUOTES","END_OF_FILE"
};

Token::Token(TokenType type, std::string lexeme, Literal literal, u32 line,
        std::string file)
{
    this->type =type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
    this->file = file;
}


TokenType Token::getTokenType() const
{
    return type;
}
std::string Token::getLexeme() const
{
    return lexeme;
}

Literal Token::getLiteral() const
{
    return literal;
}

u32 Token::getLine() const
{
    return line;
}

std::string Token::getFile() const
{
    return file;
}

std::string Token::toString() const
{
    std::ostringstream tokenString;
    tokenString << TOKENTYPE_STRING[type] << "\t" << lexeme << "\t";
    switch(type)
    {
        case STRING:
            tokenString << std::get<std::string>(literal);
            break;
        case SIGNED_INTEGER_NUMBER:
            tokenString << std::get<s32>(literal);
            break;
        case UNSIGNED_INTEGER_NUMBER:
            tokenString << std::get<u32>(literal);
            break;
        case FLOAT_NUMBER:
            tokenString << std::get<f32>(literal);
    }
    return tokenString.str();
}

TokenType findKeywordOrIdentifier(const std::string string)
{
    for(u32 i = 0; i < KEYWORD_COUNT; i++)
    {
        if(0 == strcmp(KEYWORD_LIST[i],string.c_str()))
        {
            return static_cast<TokenType>(i);
        }
    }
    return IDENTIFIER;
}
