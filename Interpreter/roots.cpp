#include "roots.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "logger.hpp"
#include <iostream>
#include <cstring>

static RootObject* NULLPTR = nullptr; 
constexpr const char* EXITCMD = "exit";

void printNewTokenInteractive(const std::vector<Token>& tokenList,u32 currentIndex);

Error runInteractive()
{
    NULLPTR = new RootObject();
    std::cout << "Roots Interactive Shell\n";
    std::vector<Token> tokenList;
    u32 tokenListIndex = 0;
    while(true)
    {
        std::string statement;
        std::cout << ">>";
        std::getline(std::cin,statement);
        if(EXITCMD == statement)
        {
            break;
        }
        lexInteractive(tokenList,statement);
        run();
        tokenList.pop_back();
        tokenList.clear();
    }
    delete NULLPTR;
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
