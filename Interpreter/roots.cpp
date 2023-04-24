#include "roots.hpp"
#include "lexer.hpp"
#include <iostream>

constexpr const char* EXITCMD = "exit";

void printNewTokenInteractive(const std::vector<Token>& tokenList,u32 currentIndex);

Error runInteractive()
{
    std::cout << "Roots Interactive Shell\n";
    std::string statement = "";
    std::vector<Token> tokenList;
    u32 tokenListIndex = 0;
    while(true)
    {
        std::cout << ">>";
        std::cin >> statement;
        if(EXITCMD == statement)
        {
            break;
        }
        lexInteractive(tokenList,statement);
        run();
        printNewTokenInteractive(tokenList,tokenListIndex);
        tokenListIndex = tokenList.size() - 1;
        std::cout << tokenListIndex;
    }
    return NOERROR;
}
Error runFiles(const std::string filePath)
{
    std::vector<Token> tokenList = lexFile(filePath);
    
    run();

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
        std::cout << tokenList[i].toString().c_str() << "\n";
    }
}
