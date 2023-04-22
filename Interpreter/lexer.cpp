#include "logger.hpp"
#include "lexer.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>


constexpr const char* EXITCMD = "exit";
Error tokenize(std::vector<Token>& tokenList,std::string source);

Error lexInteractive(std::vector<Token>& tokenList, std::string statement) 
{
    return NOERROR;
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
        return tokenList;
    }
    s32 character = getc(openFile);
    while(EOF != character)
    {
        source += static_cast<char>(character);
        character = getc(openFile);
    }
    LOG_INFO("%s" ,source.c_str());
    tokenize(tokenList,source);

    return tokenList;
}

Error tokenize(std::vector<Token>& tokenList, std::string source)
{
    return NOERROR;
}
