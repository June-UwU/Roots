#include "parser.hpp"
#include "logger.hpp"
#include <memory>

// helpers
bool isEnd(std::vector<Token>& tokenList, u32& index);

// Grammar rules for the RD parser.
SharedRef<Expr> expression(std::vector<Token>& tokenList,u32& index);
SharedRef<Expr> equality(std::vector<Token>& tokenList, u32& index);
SharedRef<Expr> comparsion(std::vector<Token>& tokenList, u32& index);
SharedRef<Expr> term(std::vector<Token>& tokenList, u32& index);
SharedRef<Expr> factor(std::vector<Token>& tokenList, u32& index);
SharedRef<Expr> unary(std::vector<Token>& tokenList, u32& index);
SharedRef<Expr> primary(std::vector<Token>& tokenList, u32& index);

SharedRef<Expr> parse(std::vector<Token>& tokenList)
{
    u32 currentIndex = 0;
    return expression(tokenList, currentIndex);
}

SharedRef<Expr> expression(std::vector<Token>& tokenList, u32& index)
{
    return equality(tokenList,index);
}

SharedRef<Expr> equality(std::vector<Token>& tokenList, u32& index)
{
    SharedRef<Expr> lhs = comparsion(tokenList,index);
    bool isOper = true;
    while(isOper || isEnd(tokenList,index))
        {
            SharedRef<BinaryOper> oper;
            switch(tokenList[index].getTokenType())
            {
                case EQUAL_EQUAL:
                case BANG_EQUAL:
                    oper = std::make_shared<BinaryOper>(tokenList[index]);
                    index++;
                    break;
                default:
                // TODO : might need to eat up the semi colons here; or does it have any other meaning
                    isOper = false;
                    continue;
            }
            SharedRef<Expr> rhs = comparsion(tokenList,index);
            SharedRef<BinaryExpr> expr = std::make_shared<BinaryExpr>(lhs,oper,rhs);
            lhs = std::make_shared<Expr>(expr);
        }
    return lhs;
}

SharedRef<Expr> comparsion(std::vector<Token>& tokenList, u32& index)
{
    SharedRef<Expr> lhs = term(tokenList,index);
    bool isOper = true;
    while(isOper || !isEnd(tokenList,index))
        {
            SharedRef<BinaryOper> oper;
            switch(tokenList[index].getTokenType())
                {
                    case GREATER_EQUAL:
                    case LESS_EQUAL:
                    case GREATER:
                    case LESS:
                        oper = std::make_shared<BinaryOper>(tokenList[index]);
                        index++;
                        break;
                    default:
                        isOper = false;
                        continue;
                }
            SharedRef<Expr> rhs = term(tokenList,index);
            SharedRef<BinaryExpr> expr = std::make_shared<BinaryExpr>(lhs,oper,rhs);
            lhs = std::make_shared<Expr>(expr);
        }
    return lhs;
}

SharedRef<Expr> term(std::vector<Token>& tokenList,u32& index)
{
    SharedRef<Expr> lhs = factor(tokenList,index);
    bool isOper = true;
    while(isOper || !isEnd(tokenList,index))
        {
            SharedRef<BinaryOper> oper;
            switch(tokenList[index].getTokenType())
                {
                    case MINUS:
                    case PLUS:
                        oper = std::make_shared<BinaryOper>(tokenList[index]);
                        index++;
                        break;
                    default:
                        isOper = false;
                        continue;
                }
            SharedRef<Expr> rhs = factor(tokenList,index);
            SharedRef<BinaryExpr> expr = std::make_shared<BinaryExpr>(lhs,oper,rhs);
            lhs = std::make_shared<Expr>(expr);
        }
    return lhs;
}

SharedRef<Expr> factor(std::vector<Token>& tokenList, u32& index)
{
    SharedRef<Expr> lhs = unary(tokenList, index);
    bool isOper = true;
    while(isOper || !isEnd(tokenList,index))
        {
            SharedRef<BinaryOper> oper;
            switch(tokenList[index].getTokenType())
                {
                    case SLASH:
                    case STAR:
                        oper = std::make_shared<BinaryOper>(tokenList[index]);
                        index++;
                        break;
                    default:
                        isOper = false;
                        continue;
                }
            SharedRef<Expr> rhs = unary(tokenList,index);
            SharedRef<BinaryExpr> expr = std::make_shared<BinaryExpr>(lhs,oper,rhs);
            lhs = std::make_shared<Expr>(expr);
        }
    return lhs;
}

SharedRef<Expr> unary(std::vector<Token>& tokenList, u32& index)
{
    SharedRef<Expr> expr;
    SharedRef<UnaryOper> oper;
    switch(tokenList[index].getTokenType())
        {
            case MINUS:
            case BANG:
                oper = std::make_shared<UnaryOper>(tokenList[index]);
                index++;
                expr = unary(tokenList,index);
            default:
                return primary(tokenList,index);
        }
}

SharedRef<Expr> primary(std::vector<Token>& tokenList, u32& index)
{
    SharedRef<Expr> expr;

    switch(tokenList[index].getTokenType())
        {
            case TRUE:
            case FALSE:
            case STRING:
            case SIGNED_INTEGER_NUMBER:
            case UNSIGNED_INTEGER_NUMBER:
                {
                    SharedRef<LiteralExpr> literal = std::make_shared<LiteralExpr>(tokenList[index]);
                    expr = std::make_shared<Expr>(literal);
                    index++;
                }
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

