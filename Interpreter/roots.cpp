#include "roots.hpp"
#include "lexer.hpp"
#include <iostream>

constexpr const char* EXITCMD = "exit";

Error runInteractive()
{
    std::cout << "Roots Interactive Shell\n";
    std::string statement;
    std::vector<Token> tokenList;
    while(statement != EXITCMD)
    {
        std::cout << ">>";
        lexInteractive(tokenList,statement);
        run();
        std::cin >> statement;
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
