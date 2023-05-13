#include "parser.hpp"
#include "logger.hpp"
#include <memory>
// TODO : clean up here and expression

// helpers
bool isEnd(std::vector<Token>& tokenList, u32& index);

// Grammar rules for the RD parser.
AstNode* expression(std::vector<Token>& tokenList,u32& index);
AstNode* equality(std::vector<Token>& tokenList, u32& index);
AstNode* comparsion(std::vector<Token>& tokenList, u32& index);
AstNode* term(std::vector<Token>& tokenList, u32& index);
AstNode* factor(std::vector<Token>& tokenList, u32& index);
AstNode* unary(std::vector<Token>& tokenList, u32& index);
AstNode* primary(std::vector<Token>& tokenList, u32& index);

AstNode* parse(std::vector<Token>& tokenList)
{
    u32 currentIndex = 0;
    return expression(tokenList, currentIndex);
}

AstNode* expression(std::vector<Token>& tokenList, u32& index)
{
    return equality(tokenList,index);
}

AstNode* equality(std::vector<Token>& tokenList, u32& index)
{
    AstNode* lhs = comparsion(tokenList,index);
    bool isOper = true;
    while(isOper && !isEnd(tokenList,index))
        {
            AstNode* oper;
            switch(tokenList[index].getTokenType())
            {
                case EQUAL_EQUAL:
                case BANG_EQUAL:
                {   
                    oper = new BinaryOper(tokenList[index]);
                    index++;
                }break;
                default:
                // TODO : might need to eat up the semi colons here; or does it have any other meaning
                    isOper = false;
                    continue;
            }
            AstNode* rhs = comparsion(tokenList,index);
            lhs = new BinaryExpr(lhs,oper,rhs);
        }
    return lhs;
}

AstNode* comparsion(std::vector<Token>& tokenList, u32& index)
{
    AstNode* lhs = term(tokenList,index);
    bool isOper = true;
    while(isOper && !isEnd(tokenList,index))
        {
            AstNode* oper;
            switch(tokenList[index].getTokenType())
                {
                    case GREATER_EQUAL:
                    case LESS_EQUAL:
                    case GREATER:
                    case LESS:
                    {
                        oper = new BinaryOper(tokenList[index]);
                        index++;
                    }break;
                    default:
                        isOper = false;
                        continue;
                }
            AstNode* rhs = term(tokenList,index);
            lhs = new BinaryExpr(lhs,oper,rhs); 
        }
    return lhs;
}

AstNode* term(std::vector<Token>& tokenList,u32& index)
{
    AstNode* lhs = factor(tokenList,index);
    bool isOper = true;
    while(isOper && !isEnd(tokenList,index))
        {
            AstNode* oper;
            switch(tokenList[index].getTokenType())
                {
                    case MINUS:
                    case PLUS:
                    {
                        oper = new BinaryOper(tokenList[index]);
                        index++;
                    }break;
                    default:
                        isOper = false;
                        continue;
                }
            AstNode* rhs = factor(tokenList,index);
            lhs = new BinaryExpr(lhs,oper,rhs);
        }
    return lhs;
}

AstNode* factor(std::vector<Token>& tokenList, u32& index)
{
    AstNode* lhs = unary(tokenList, index);
    bool isOper = true;
    while(isOper && !isEnd(tokenList,index))
        {
            AstNode* oper;
            switch(tokenList[index].getTokenType())
                {
                    case SLASH:
                    case STAR:
                    {
                        oper = new BinaryOper(tokenList[index]);
                        index++;
                    }break;
                    default:
                        isOper = false;
                        continue;
                }
            AstNode* rhs = unary(tokenList,index);
            lhs = new BinaryExpr(lhs,oper,rhs);
        }
    return lhs;
}

AstNode* unary(std::vector<Token>& tokenList, u32& index)
{
    AstNode* expr = nullptr;
    AstNode* oper = nullptr;
    switch(tokenList[index].getTokenType())
        {
            case MINUS:
            case BANG:
            {
                oper = new UnaryOper(tokenList[index]);
                index++;
                expr = unary(tokenList,index);
            }break;
            default:
                return primary(tokenList,index);
        }
    expr = new UnaryExpr(oper,expr);
    return expr;
}

AstNode* primary(std::vector<Token>& tokenList, u32& index)
{
    AstNode* expr = nullptr;

    switch(tokenList[index].getTokenType())
        {
            case TRUE:
            case FALSE:
            case STRING:
            case SIGNED_INTEGER_NUMBER:
            case UNSIGNED_INTEGER_NUMBER:
            {
                expr = new LiteralExpr(tokenList[index]);
                index++;
            }break;
            case LEFT_PAREN:
            {
                Token lParen = tokenList[index];
                index++;
                AstNode* gExpr = expression(tokenList,index);
                Token rParen = tokenList[index];
                index++;
                expr =  new Group(lParen,gExpr,rParen);
            }break;
            default:
                // TODO : error handling and other good stuff
                LOG_ERROR("primary grammar default case - possible error");
                break;
        }
    return expr;
}

bool isEnd(std::vector<Token>& tokenList, u32& index)
{
    return (tokenList[index].getTokenType() == END_OF_FILE);
}

