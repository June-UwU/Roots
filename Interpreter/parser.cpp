#include "parser.hpp"
#include "logger.hpp"
#include <memory>
#include "roots.hpp"

// helpers
bool isEnd(std::vector<Token>& tokenList, u32& index);
Statement* parseWhile(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& currentIndex, Block* owner);
Statement* parseFor(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& currentIndex, Block* owner);
Statement* parseIf(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& currentIndex, Block* owner);
Statement* parseBlock(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& currentIndex, Block* owner);
Statement* parseVariable(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& currentIndex, Block* owner);
Statement* parseExpr(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& currentIndex, Block* owner);
// Grammar rules for the RD parser.
AstNode* expression(ArenaAllocator& allocator, std::vector<Token>& tokenList,u32& index);
AstNode* equality(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& index);
AstNode* comparsion(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& index);
AstNode* term(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& index);
AstNode* factor(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& index);
AstNode* unary(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& index);
AstNode* primary(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& index);

Statement* parse(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& currentIndex, Block* owner)
{
    Statement* stmt = nullptr;
    allocator.reset();
    switch (tokenList[currentIndex].getTokenType())
    {
    case WHILE:
        currentIndex++;
        stmt = parseWhile(allocator,tokenList,currentIndex,owner);
        break;
    case FOR:
        currentIndex++;
        stmt = parseFor(allocator,tokenList,currentIndex,owner);
        break;
    case IF:
        currentIndex++;
        stmt = parseIf(allocator,tokenList,currentIndex,owner);
        break;
    case LEFT_BRACE:
        currentIndex++;
        stmt = parseBlock(allocator,tokenList,currentIndex,owner);
        break;
    case VAR:
        currentIndex++;
        stmt = parseVariable(allocator,tokenList,currentIndex,owner);
        break; 
    default:
        stmt = parseExpr(allocator,tokenList,currentIndex,owner);
        break;
    }
    return stmt;
}

Statement* parseWhile(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& currentIndex)
{
    if(tokenList[currentIndex].getTokenType() != LEFT_BRACE)
    {
        LOG_ERROR("expected a ( ");
        // TODO : panic..!!
    }
    else
    {
        currentIndex++;
    }
    // TODO : parse if

}

Statement* parseFor(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& currentIndex)
{

}

Statement* parseIf(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& currentIndex)
{

}

Statement* parseBlock(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& currentIndex, Block* owner)
{
    Block* block = (Block*)allocator.allocate(sizeof(Block));
    block = new(block) Block(nullptr,owner);
    Statement* stmt = nullptr;
    // case for empty block
    if(tokenList[currentIndex].getTokenType() != RIGHT_BRACE)
    {
        stmt = parse(allocator,tokenList,currentIndex,block);
    }
    else
    {
        return block;
    }
    while(tokenList[currentIndex].getTokenType() != RIGHT_BRACE)
    {
        Statement* tmpStmt = parse(allocator,tokenList,currentIndex,block);
        stmt->addNextStmt(tmpStmt);
        stmt = stmt->getNextStmt();
    }
    block->addNextStmt(stmt);
    return block;
}

Statement* parseVariable(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& currentIndex, Block* owner)
{
    AstNode* id = expression(allocator,tokenList,currentIndex);
    AstNode* value  = nullptr;
    if(tokenList[currentIndex].getTokenType() == EQUAL)
    {
        currentIndex++;
        value = expression(allocator,tokenList,currentIndex);
    }
    if(id->getType() == LITERAL)
    {
        LiteralExpr* literal = (LiteralExpr*) id;
        Token token = literal->getToken();
        if(token.getTokenType() != IDENTIFIER)
        {
            LOG_ERROR("invalid identifier");
        }
        if(value != nullptr)
        {
            if(value->getType() != LITERAL)
            {
                LOG_WARN("invalid assignment, value is not a literal");
                // TODO : AHH PANIC...!!!
            }
            else
            {
                Token valueToken = ((LiteralExpr*)value)->getToken();
                RootObject* obj = nullptr;
// TODO : this switch case is mem leak bonanza..good luck fixing it.
                switch (valueToken.getTokenType())
                {
                case FLOAT_NUMBER:
                    obj = new FloatObject(std::get<f64>(valueToken.getLiteral()));
                    owner->addVariable(token.getLexeme(),obj);
                    break;
                case STRING:
                    obj = new StringObject(std::get<std::string>(valueToken.getLiteral()));
                    owner->addVariable(token.getLexeme(),obj);
                    break;
                case TRUE:
                    obj = new BooleanObject(true);
                    owner->addVariable(token.getLexeme(),obj);
                    break;
                case FALSE:
                    obj = new BooleanObject(false);
                    owner->addVariable(token.getLexeme(),obj);
                    break;
                default:
                    owner->addVariable(token.getLexeme(),getNullPointer());
                    break;
                }
            }
        }
    }
    else
    {
        LOG_ERROR("invalid identifier");
    }
    parse(allocator,tokenList,currentIndex,owner);
}

Statement* parseExpr(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& currentIndex, Block* owner)
{
    AstNode* expr = expression(allocator,tokenList,currentIndex);
    Statement* stmt = (Statement*)allocator.allocate(sizeof(Expression));
    stmt = new(stmt) Expression(expr,owner);
    return stmt;
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

