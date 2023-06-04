#include "expression.hpp"
#include "logger.hpp"

constexpr const char* EXPRTYPE_STR[EXPRTYPE]
{
    "EXPR","GROUP","UNARY_EXPR","BINARY_EXPR",
    "LITERAL","BINARY_OPER","UNARY_OPER","EXPR_LITERAL",
    "EXPR_GROUP","EXPR_UNARY","EXPR_BINARY"
};

StmtType AbstractStmt::getType() const
{
    return type;
}

Block* AbstractStmt::getOwner() const
{
    return ownerBlock;
}

void AbstractStmt::addNextStmt(Statement* stmt)
{
    next = stmt;
}

Block::Block(Statement* stmt,Block* owner)
    :list{stmt}
{
    type = BLOCK_STMT;
    ownerBlock = owner;
}

void Block::addVariable(std::string id, RootObject* obj)
{
    auto iter = env.emplace(id,obj);
    if(!iter.second)
    {
        LOG_WARN("multiple definition of variable");
        // TODO : error
    }
}

RootObject* Block::getObject(std::string id)
{
    Block* blk = this;
    while(blk)
    {
        auto iter = blk->env.find(id);
        if(iter == blk->env.end())
        {
            blk = blk->owner;
        }
        else
        {
            return iter->second;
        }
    }
    // TODO :error..
    return nullptr;
}

IfSmt::IfSmt(AstNode* pred, Block* block, Block* owner)
    :predicate{pred}
    ,stmtBlock{block}
{
    type = IF_STMT;
    ownerBlock = owner;
}

AstNode* IfSmt::getPredicate() const
{
    return predicate;
}

Block* IfSmt::getStmt() const
{
    return stmtBlock;
}

WhileStmt::WhileStmt(AstNode* pred, Block* block, Block* owner)
    :predicate{pred}
    ,stmtBlock{block}
{
    type = WHILE_STMT;
    ownerBlock = owner;
}
AstNode* WhileStmt::getPredicate() const
{
    return predicate;
}

Block* WhileStmt::getBlock() const
{
    return stmtBlock;
}

Expression::Expression(AstNode* expr, Block* owner)
{
    ownerBlock = owner;
}

AstNode* Expression::getExpr() const
{
    return expr;
}

ForStmt::ForStmt(AstNode* ini, AstNode* pred, AstNode* up, Block* block, Block* owner)
    :init{ini}
    ,predicate{pred}
    ,update{up}
{
    type = FOR_STMT;
    ownerBlock = owner;
}

AstNode* ForStmt::getInit() const
{
    return init;
}

AstNode* ForStmt::getPredicate() const
{
    return predicate;
}

AstNode* ForStmt::getUpdate() const
{
    return update;
}

Block* ForStmt::getBlock() const
{
    return ownerBlock;
}

ExprType AbstractExpr::getType() const
{
    return type;
}

Group::Group(Token lhs, AstNode* midExpr, Token rhs)
    :expr{midExpr}
{
    oper[RHS] = rhs;
    oper[LHS] = lhs;
    type = GROUP;
}

Token Group::getOperator(Placement place)
{
    if(LHS == place)
    {
        return oper[LHS];
    }
    else
    {
        return oper[RHS];
    }
}

AstNode *Group::getExpr()
{
    return expr;
}

UnaryExpr::UnaryExpr(AstNode* pOper, AstNode* exprRef)
    :oper{pOper}
    ,expr{exprRef}
{
    type = UNARY_EXPR;
}

AstNode *UnaryExpr::getOperator()
{
    return oper;
}

AstNode *UnaryExpr::getExpr()
{
    return expr;
}

BinaryExpr::BinaryExpr(AstNode* lhs, AstNode* pOper, AstNode* rhs)
    :oper{pOper}
{
    expr[LHS] = lhs;
    expr[RHS] = rhs;
    type = BINARY_EXPR;
}

AstNode *BinaryExpr::getExpr(Placement place)
{
    if(LHS == place)
    {
        return expr[LHS];
    }
    else
    {
        return expr[RHS];
    }
}

AstNode *BinaryExpr::getOperator()
{
    return oper;
}

LiteralExpr::LiteralExpr(Token literalRef)
    :expr{literalRef}
{
    type = LITERAL;
}

Token LiteralExpr::getToken() const
{
    return expr;
}

UnaryOper::UnaryOper(Token operToken)
    :oper{operToken}
{
    type = UNARY_OPER;
}

Token UnaryOper::getToken() const
{
    return oper;
}

BinaryOper::BinaryOper(Token operToken)
    :oper{operToken}
{
    type = BINARY_OPER;
}

Token BinaryOper::getToken() const
{
    return oper;
}

bool isValidExpr(const Token token,const ExprType type)
{
    switch(type)
        {
            case BINARY_OPER:
             switch(token.getTokenType())
                    {
                        case BANG_EQUAL:
                        case EQUAL:
                        case EQUAL_EQUAL:
                        case LESS_EQUAL:
                        case GREATER_EQUAL:
                        case GREATER:
                        case LESS:
                        case MINUS:
                        case PLUS:
                        case SLASH:
                        case STAR:
                            return true;
                        default:
                            return false;
                    }
                break;
            case UNARY_OPER:
                switch(token.getTokenType())
                    {
                        case MINUS:
                        case BANG:
                            return true;
                        default:
                            return false;
                    }
                break;
            default:
                return false;

        }
}


const char* getExprTypeString(ExprType type)
{
    return EXPRTYPE_STR[type];
}

