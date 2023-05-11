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
                    BinaryOper* binaryOper = new BinaryOper(tokenList[index]);
                    oper = new AstNode(binaryOper);
                    index++;
                }break;
                default:
                // TODO : might need to eat up the semi colons here; or does it have any other meaning
                    isOper = false;
                    continue;
            }
            AstNode* rhs = comparsion(tokenList,index);
            BinaryExpr* expr = new BinaryExpr(lhs,oper,rhs);
            lhs = new AstNode(expr);
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
                        BinaryOper* bOper = new BinaryOper(tokenList[index]);
                        oper = new AstNode(bOper);
                        index++;
                    }break;
                    default:
                        isOper = false;
                        continue;
                }
            AstNode* rhs = term(tokenList,index);
            BinaryExpr* bExpr = new BinaryExpr(lhs,oper,rhs); 
            lhs = new AstNode(bExpr);
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
                        BinaryOper* bOper = new BinaryOper(tokenList[index]);
                        oper =new AstNode(bOper);
                        index++;
                    }break;
                    default:
                        isOper = false;
                        continue;
                }
            AstNode* rhs = factor(tokenList,index);
            BinaryExpr* expr = new BinaryExpr(lhs,oper,rhs);
            lhs = new AstNode(expr);
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
                        BinaryOper* bOper = new BinaryOper(tokenList[index]);
                        oper = new AstNode(bOper);
                        index++;
                    }break;
                    default:
                        isOper = false;
                        continue;
                }
            AstNode* rhs = unary(tokenList,index);
            BinaryExpr* expr = new BinaryExpr(lhs,oper,rhs);
            lhs = new AstNode(expr);
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
                UnaryOper* uOper = new UnaryOper(tokenList[index]);
                oper = new AstNode(uOper);
                index++;
                expr = unary(tokenList,index);
            }break;
            default:
                return primary(tokenList,index);
        }
    UnaryExpr* uExpr = new UnaryExpr(oper,expr);
    expr = new AstNode(uExpr);
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
                LiteralExpr* lExpr = new LiteralExpr(tokenList[index]);
                expr = new AstNode(lExpr);
                index++;
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

