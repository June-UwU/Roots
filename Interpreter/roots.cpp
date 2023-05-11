#include "roots.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "logger.hpp"
#include "astPrettyPrinter.hpp"
#include <iostream>
#include <cstring>

constexpr const char* EXITCMD = "exit";

void printNewTokenInteractive(const std::vector<Token>& tokenList,u32 currentIndex);

Error runInteractive()
{
    std::cout << "Roots Interactive Shell\n";
    std::vector<Token> tokenList;
    u32 tokenListIndex = 0;
    AstNode* ast = nullptr;
    while(true)
    {
        std::string statement;
        std::cout << ">>";
        std::getline(std::cin,statement);
        if(EXITCMD == statement)
        {
            break;
        }
        delete ast;
        lexInteractive(tokenList,statement);
        run();
        tokenList.pop_back();
        ast = parse(tokenList);
        LOG_INFO("parsing done");
        astPrettyPrint(ast);    
    }
    delete ast;
    return NOERROR;
}
Error runFiles(const std::string filePath)
{
    std::vector<Token> tokenList = lexFile(filePath);
    
    run();

    for(s32 i = 0; i < tokenList.size();i++)
    {
        LOG_INFO("%s",tokenList[i].toString().c_str());
    }

    return NOERROR;
}
Error run()
{
    return NOERROR;
}

void printNewTokenInteractive(const std::vector<Token>& tokenList, u32 currentIndex)
{
    for(u32 i = currentIndex; i < tokenList.size() - 1; i++)
    {
        LOG_INFO("%s",tokenList[i].toString().c_str());
    }
}
