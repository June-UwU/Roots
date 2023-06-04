#include "token.hpp"
#include <sstream>
#include <string.h>
#include <iostream>

constexpr const u32 KEYWORD_COUNT = STRING;
constexpr const char* KEYWORD_LIST[KEYWORD_COUNT]
{
    "var","class","true","false","&","|","this","super","return",
    "null","for","if"
};

constexpr const char* TOKENTYPE_STRING[TOKENTYPE]
{
    "VAR","CLASS","TRUE","FALSE","AND","OR","THIS","SUPER",
    "RETURN","NONE","WHILE","FOR","IF","STRING","IDENTIFIER","FLOAT_NUMBER","BANG","BANG_EQUAL","EQUAL","EQUAL_EQUAL",
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
    try {
    switch(type)
    {
        case STRING:
            tokenString << std::get<std::string>(literal);
            break;
        case FLOAT_NUMBER:
            tokenString << std::get<f32>(literal);
    }
    }catch(std::bad_variant_access access)
                           {
        std::cout <<"\n" << access.what();
        return "";
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
