#include "parser.hpp"
#include "logger.hpp"
#include <memory>
// TODO : clean up here and expression

// helpers
bool isEnd(std::vector<Token>& tokenList, u32& index);

// Grammar rules for the RD parser.
AstNode* expression(ArenaAllocator& allocator, std::vector<Token>& tokenList,u32& index);
AstNode* equality(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& index);
AstNode* comparsion(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& index);
AstNode* term(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& index);
AstNode* factor(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& index);
AstNode* unary(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& index);
AstNode* primary(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& index);

AstNode* parse(ArenaAllocator& allocator, std::vector<Token>& tokenList)
{
    u32 currentIndex = 0;
    return expression(allocator, tokenList, currentIndex);
}

AstNode* expression(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& index)
{
    return equality(allocator, tokenList,index);
}

AstNode* equality(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& index)
{
    AstNode* expr = nullptr;
    AstNode* lhs = comparsion(allocator, tokenList,index);
    bool isOper = true;
    while(isOper && !isEnd(tokenList,index))
        {
            AstNode* oper;
            switch(tokenList[index].getTokenType())
            {
                case EQUAL_EQUAL:
                case BANG_EQUAL:
                {   
                    oper = (AstNode*)allocator.allocate(sizeof(BinaryOper)); 
                    oper = new (oper) BinaryOper(tokenList[index]);
                    index++;
                }break;
                default:
                // TODO : might need to eat up the semi colons here; or does it have any other meaning
                    isOper = false;
                    continue;
            }
            AstNode* rhs = comparsion(allocator, tokenList,index);
            expr = (AstNode*)allocator.allocate(sizeof(BinaryExpr));
            lhs = new (expr) BinaryExpr(lhs,oper,rhs);
        }
    return lhs;
}

AstNode* comparsion(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& index)
{
    AstNode* expr = nullptr;
    AstNode* lhs = term(allocator,tokenList,index);
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
                        oper = (BinaryOper*)allocator.allocate(sizeof(BinaryOper));
                        oper = new (oper) BinaryOper(tokenList[index]);
                        index++;
                    }break;
                    default:
                        isOper = false;
                        continue;
                }
            AstNode* rhs = term(allocator,tokenList,index);
            expr = (BinaryExpr*)allocator.allocate(sizeof(BinaryExpr));
            lhs = new (expr) BinaryExpr(lhs,oper,rhs); 
        }
    return lhs;
}

AstNode* term(ArenaAllocator& allocator, std::vector<Token>& tokenList,u32& index)
{
    AstNode* expr = nullptr;
    AstNode* lhs = factor(allocator,tokenList,index);
    bool isOper = true;
    while(isOper && !isEnd(tokenList,index))
        {
            AstNode* oper;
            switch(tokenList[index].getTokenType())
                {
                    case MINUS:
                    case PLUS:
                    {
                        oper = (AstNode*)allocator.allocate(sizeof(BinaryOper));
                        oper = new (oper) BinaryOper(tokenList[index]);
                        index++;
                    }break;
                    default:
                        isOper = false;
                        continue;
                }
            AstNode* rhs = factor(allocator,tokenList,index);
            expr = (AstNode*) allocator.allocate(sizeof(BinaryExpr));
            lhs = new (expr) BinaryExpr(lhs,oper,rhs);
        }
    return lhs;
}

AstNode* factor(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& index)
{
    AstNode* expr = nullptr;
    AstNode* lhs = unary(allocator,tokenList, index);
    bool isOper = true;
    while(isOper && !isEnd(tokenList,index))
        {
            AstNode* oper;
            switch(tokenList[index].getTokenType())
                {
                    case SLASH:
                    case STAR:
                    {
                        oper = (AstNode*) allocator.allocate(sizeof(BinaryOper));
                        oper = new (oper) BinaryOper(tokenList[index]);
                        index++;
                    }break;
                    default:
                        isOper = false;
                        continue;
                }
            AstNode* rhs = unary(allocator,tokenList,index);
            expr = (AstNode*) allocator.allocate(sizeof(BinaryExpr));
            lhs = new (expr) BinaryExpr(lhs,oper,rhs);
        }
    return lhs;
}

AstNode* unary(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& index)
{
    AstNode* tempExpr = nullptr;
    AstNode* expr = nullptr;
    AstNode* oper = nullptr;
    switch(tokenList[index].getTokenType())
        {
            case MINUS:
            case BANG:
            {
                oper = (AstNode*) allocator.allocate(sizeof(UnaryOper));
                oper = new (oper) UnaryOper(tokenList[index]);
                index++;
                expr = unary(allocator,tokenList,index);
            }break;
            default:
                return primary(allocator,tokenList,index);
        }
    tempExpr = (AstNode*) allocator.allocate(sizeof(UnaryExpr));
    expr = new (tempExpr) UnaryExpr(oper,expr);
    return expr;
}

AstNode* primary(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& index)
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
                expr = (AstNode*) allocator.allocate(sizeof(LiteralExpr));
                expr = new (expr) LiteralExpr(tokenList[index]);
                index++;
            }break;
            case LEFT_PAREN:
            {
                Token lParen = tokenList[index];
                index++;
                AstNode* gExpr = expression(allocator,tokenList,index);
                Token rParen = tokenList[index];
                index++;
                expr = (AstNode*) allocator.allocate(sizeof(Group));
                expr =  new (expr) Group(lParen,gExpr,rParen);
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

