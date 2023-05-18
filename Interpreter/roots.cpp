#include "roots.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "logger.hpp"
#include "astPrettyPrinter.hpp"
#include "runtime/interpreter.hpp"
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
        RootObject* retObj = interpret(ast);
        switch (retObj->getType())
        {
        case BOOLEAN_OBJECT:
        {
            BooleanObject* obj = reinterpret_cast<BooleanObject*>(retObj);
            std::cout << "Evaluation : "<<obj->getValue() << "\n";
        }break;
        case FLOAT_OBJECT:
        {
            FloatObject* obj = reinterpret_cast<FloatObject*>(retObj);
            std::cout << "Evaluation : "<<obj->getValue() << "\n";
        }break;
        case SIGNED_INTEGER_OBJECT:
        {
            IntObject* obj = reinterpret_cast<IntObject*>(retObj);
            std::cout << "Evaluation : "<<obj->getValue() << "\n";
        }break;
        case UNSIGNED_INTEGER_OBJECT:
        {
            UintObject* obj = reinterpret_cast<UintObject*>(retObj);
            std::cout << "Evaluation : "<<obj->getValue() << "\n";
        }break;
        default:
            // LOG ERROR...?
            break;
        }
        delete ast;
        ast = nullptr;
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
