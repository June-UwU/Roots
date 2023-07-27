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
void parseVariable(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& currentIndex, Block* owner);
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
        parseVariable(allocator,tokenList,currentIndex,owner);
        parse(allocator,tokenList,currentIndex,owner);
        break; 
    default:
        stmt = parseExpr(allocator,tokenList,currentIndex,owner);
        break;
    }
    return stmt;
}

Statement* parseWhile(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& currentIndex, Block* owner)
{
    WhileStmt* whileStmt = (WhileStmt*)allocator.allocate(sizeof(WhileStmt));
    AstNode* predicate = expression(allocator,tokenList,currentIndex);;
    Block* whileBlock = (Block*)parseBlock(allocator,tokenList,currentIndex,owner);
    whileStmt = new(whileStmt) WhileStmt(predicate,whileBlock,owner);
    return whileStmt;
}

Statement* parseFor(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& currentIndex, Block* owner)
{
    ForStmt* forStmt = (ForStmt*)allocator.allocate(sizeof(ForStmt));
    AstNode* forInit = nullptr;
    AstNode* predicate = nullptr;
    AstNode* forUpdate = nullptr;
    Block* forBlock = nullptr;
    if(tokenList[currentIndex].getTokenType() != LEFT_PAREN )
    {
        LOG_ERROR("expected an initializer list for the for loop");
    }
    else
    {
        currentIndex++;
        forInit = expression(allocator,tokenList,currentIndex);
    }
    
    if(tokenList[currentIndex].getTokenType() != SEMI_COLON)
    {
        LOG_ERROR("expected a semi colon after the initializing statement");
    }
    else
    {
        currentIndex++;
        predicate = expression(allocator,tokenList,currentIndex);
    }
    
    if(tokenList[currentIndex].getTokenType() != SEMI_COLON)
    {
        LOG_ERROR("expected a semi colon after the initializing statement");
    }
    else
    {
        currentIndex++;
        forUpdate = expression(allocator,tokenList,currentIndex);
    }

    if(tokenList[currentIndex].getTokenType() != RIGHT_PAREN)
    {
        LOG_ERROR("for loop initializer not properly terminated");
    }
    else
    {
        currentIndex++;
    }

    if(tokenList[currentIndex].getTokenType() == LEFT_BRACE)
    {
        currentIndex++;
        forBlock = (Block*) parseBlock(allocator,tokenList,currentIndex,owner);
    }
    else
    {
        LOG_ERROR("unknown token for initializing a for loop block");
    }
    forStmt = new(forStmt) ForStmt(forInit,predicate,forUpdate,forBlock,owner);
    return forStmt;
}

Statement* parseIf(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& currentIndex, Block* owner)
{
    IfStmt* ifStmt = (IfStmt*)allocator.allocate(sizeof(IfStmt));
    AstNode* prediate = expression(allocator,tokenList,currentIndex);
    Block* ifBlock = (Block*)parseBlock(allocator,tokenList,currentIndex,owner);
    ifStmt = new(ifStmt) IfStmt(prediate,ifBlock,owner);
    return ifStmt;
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

void parseVariable(ArenaAllocator& allocator, std::vector<Token>& tokenList, u32& currentIndex, Block* owner)
{
    // TODO : get a cleaner implementations for this function;
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

