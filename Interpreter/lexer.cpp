#include "logger.hpp"
#include "lexer.hpp"
#include "errorHandler.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>

// STRING OPERATION
bool isNumber(const char c);
bool isAlpha(const char c);
bool isAlphaNumeric(const char c);
bool isIdentifierSymbol(const char c);
bool isEnd(std::string& source, u32& index);

constexpr const char* EXITCMD = "exit";
Error tokenize(std::vector<Token>& tokenList,std::string source, std::string file);

Error lexInteractive(std::vector<Token>& tokenList, std::string statement) 
{
    if(0 != tokenList.size())
    {
        tokenList.pop_back();
    }
    Error error = tokenize(tokenList, statement, "");

    return error;
}

std::vector<Token> lexFile(const std::string filePath)
{   
    std::string source;
    std::vector<Token> tokenList;
    FILE* openFile = fopen(filePath.c_str(),"r");
    if(nullptr == openFile)
    {
        std::ostringstream errorStream; 
        errorStream << "failed to open file\n File : " << filePath;
        LOG_ERROR("%s",errorStream.str().c_str());
        return tokenList;
    }
    s32 character = fgetc(openFile);
    while(EOF != character)
    {
        source += static_cast<char>(character);
        character = getc(openFile);
    }
    LOG_INFO("%s" ,source.c_str());
    tokenize(tokenList,source,filePath);
    
    return tokenList;
}

Error tokenize(std::vector<Token>& tokenList, std::string source, std::string file)
{
    u32 currentIndex = 0;
    u32 line = 0;
    Literal literal;
    std::string string;
    u32 startIndex;
    while(!isEnd(source,currentIndex))
    {
        switch(source[currentIndex])
        {
            case '*': 
                tokenList.emplace_back(STAR,"*",Literal(),line,file);
                currentIndex++;
                break;
            case '.':
                tokenList.emplace_back(DOT,".",Literal(),line,file);
                currentIndex++;
                break;
            case '-':
                tokenList.emplace_back(MINUS,"-",Literal(),line,file);
                currentIndex++;
                break;
            case '+':
                tokenList.emplace_back(PLUS,"+",Literal(),line,file);
                currentIndex++;
                break;
            case ';':
                tokenList.emplace_back(SEMI_COLON,";",Literal(),line,file);
                currentIndex++;
                break;
            case ':':
                tokenList.emplace_back(COLON,":",Literal(),line,file);
                currentIndex++;
                break;
            case ',':
                tokenList.emplace_back(COMMA,",",Literal(),line,file);
                currentIndex++;
                break;
            case '{':
                tokenList.emplace_back(LEFT_BRACE,"{",Literal(),line,file);
                currentIndex++;
                break;
            case '}':
                tokenList.emplace_back(RIGHT_BRACE,"}",Literal(),line,file);
                currentIndex++;
                break;
            case '(':
                tokenList.emplace_back(LEFT_PAREN,"(",Literal(),line,file);
                currentIndex++;
                break;
            case ')':
                tokenList.emplace_back(RIGHT_PAREN,")",Literal(),line,file);
                currentIndex++;
                break;
            case '&':
                tokenList.emplace_back(AND,"&",Literal(),line,file);
                currentIndex++;
                break;
            case '|':
                tokenList.emplace_back(OR,"|",Literal(),line,file);
                currentIndex++;
                break;
            case '"':
                startIndex = currentIndex;
                currentIndex++;
                while('"' != source[currentIndex])
                {
                    currentIndex++;
                    if(isEnd(source,currentIndex))
                    {
                        LOG_ERROR("unterminated string at line : %d",line);
                        reportError(ERROR_LEXER);
                    }
                    if('\n' == source[currentIndex])
                    {
                        line++;
                    }
                }
                currentIndex++;
                string = source.substr(startIndex,currentIndex);
                literal = Literal{string};
                tokenList.emplace_back(STRING,string,literal,line,file);
                break;
            case '/':
                if('/' == source[currentIndex + 1])
                {
                    while('\n' != source[currentIndex] || '\0' == source[currentIndex])
                    {
                        currentIndex++;
                    }
                    currentIndex++;
                }
                else
                {
                    tokenList.emplace_back(SLASH,"/",Literal(),line,file);
                    currentIndex++;
                }
                break;
            case '!':
                if('=' == source[currentIndex + 1])
                {
                    tokenList.emplace_back(BANG_EQUAL,"!=",Literal(),line,file);
                    currentIndex += 2;
                }
                else
                {
                    tokenList.emplace_back(BANG,"!",Literal(),line,file);
                    currentIndex++;
                }
                break;
            case '<':
                if('=' == source[currentIndex + 1])
                {
                    tokenList.emplace_back(LESS_EQUAL,"<=",Literal(),line,file);
                    currentIndex += 2;
                }
                else
                {
                    tokenList.emplace_back(LESS,"<",Literal(),line,file);
                    currentIndex++;
                }
                break;
            case '>':
                if('=' == source[currentIndex + 1])
                {
                    tokenList.emplace_back(GREATER_EQUAL, ">=",Literal(),line,file);
                    currentIndex += 2;
                }
                else
                {
                    tokenList.emplace_back(GREATER,">",Literal(),line,file);
                    currentIndex++;
                }
                break;
            case '=':
                switch(source[currentIndex + 1])
                {
                    case '=':
                        tokenList.emplace_back(EQUAL_EQUAL,"==",Literal(),line,file);
                        currentIndex += 2;
                        break;
                    case '<':
                        tokenList.emplace_back(LESS_EQUAL,"=<",Literal(),line,file);
                        currentIndex += 2;
                        break;
                    case '>':
                        tokenList.emplace_back(GREATER_EQUAL,"=>",Literal(),line,file);
                        currentIndex += 2;
                        break;
                    default:
                        tokenList.emplace_back(EQUAL,"=",Literal(),line,file);
                        currentIndex++;
                }
                break;
            case ' ':
            case '\r':
            case '\t':
                currentIndex++;
                break;
            case '\n':
                currentIndex++;
                line++;
                break;
            default:
                if(isAlpha(source[currentIndex]) || ('_' == source[currentIndex]))
                {
                    startIndex = currentIndex;
                    while(isIdentifierSymbol(source[currentIndex]))
                    {
                        currentIndex++;
                    }
                    std::string idOrKeyword = source.substr(startIndex,currentIndex);
                    TokenType token = findKeywordOrIdentifier(idOrKeyword);
                    if(tokenList[tokenList.size() - 1].getTokenType() == VAR)
                    {
                        token = IDENTIFIER;
                    }
                    tokenList.emplace_back(token,idOrKeyword,Literal(),line,file);
                }
                else if(isNumber(source[currentIndex]))
                {
                    startIndex = currentIndex;
                    TokenType token = FLOAT_NUMBER;
                    string = source.substr(startIndex,currentIndex);
                    f64 floatValue = std::stof(string);
                    literal = Literal(floatValue);
                    tokenList.emplace_back(token,string,literal,line,file);
                }
                else
                {
                    LOG_ERROR("Error :  Unknown character %c while parsing.", 
                        source[currentIndex]);
                    reportError(ERROR_LEXER);
                }
                break;
        }
    }
    tokenList.emplace_back(END_OF_FILE," ",Literal(),line,file);
    return NOERROR;
}

bool isEnd(std::string& string, u32& index)
{
    return ('\0' == string[index]);
}

bool isAlpha(const char c)
{
    if(((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')))
    {
        return true;
    }
    else 
    {
        return false;
    }
}

bool isNumber(const char c)
{
    if((c >= '0') && (c <= '9'))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isAlphaNumeric(const char c)
{
    if(isNumber(c) || isAlpha(c))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isIdentifierSymbol(const char c)
{
    if((isAlphaNumeric(c)) || ('_' == c))
    {
        return true;
    }
    else
    {
        return false;
    }
}
