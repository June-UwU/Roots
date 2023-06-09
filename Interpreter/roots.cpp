#include "roots.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "logger.hpp"
#include "runtime/compiler.hpp"
#include "runtime/decompiler.hpp"
#include "runtime/virtualMachine.hpp"
#include <iostream>
#include <cstring>

static RootObject* NULLPTR = nullptr; 
constexpr const char* EXITCMD = "exit";

void printNewTokenInteractive(const std::vector<Token>& tokenList,u32 currentIndex);

#include "runtime/instructions.hpp"
static ByteCode code[] = {INS_RET,INS_RET,INS_RET,INS_RET,INS_RET,INS_RET,INS_RET,INS_RET,INS_RET,
INS_RET,INS_RET,INS_RET,INS_RET,INS_RET,INS_RET,INS_RET,INS_RET,INS_RET,INS_RET,INS_RET,INS_RET,INS_RET,INS_END};

Error runInteractive()
{
    NULLPTR = new RootObject();
    initializeVM();
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
        printNewTokenInteractive(tokenList,0);
        //run();
        tokenList.pop_back();
        tokenList.clear();
    }
    VM->loadCode(code);
    decompile(code,CONSOLE_DUMP);
    decompile(code,TEXT_FILE);
    destroyVM();
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

RootObject* getNullPointer()
{
    return NULLPTR;
}