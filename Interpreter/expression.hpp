#pragma once
#include "types.hpp"
#include "token.hpp"
#include "runtime/rootObjects.hpp"
#include <map>

//Statement
class AbstractStmt;
class VariableDecl;
class IfStmt;
class WhileStmt;
class ForStmt;
class Block;

// AstNode
class AbstractExpr;
class Group;
class UnaryExpr;
class BinaryExpr;
class LiteralExpr;
class BinaryOper;
class UnaryOper;

using Ast = AbstractExpr;
using AstNode = AbstractExpr;
using Statement = AbstractStmt;
constexpr const u32 PAIR = 2;
typedef enum ExprType
{
    EXPR = 0x0,
    GROUP,
    UNARY_EXPR,
    BINARY_EXPR,
    LITERAL,
    BINARY_OPER,
    UNARY_OPER,
    EXPR_LITERAL,
    EXPR_GROUP,
    EXPR_UNARY,
    EXPR_BINARY,
    EXPRTYPE
}ExprType;

typedef enum Placement
{
    RHS = 0x0,
    LHS,
    PLACEMENT
}Placement;

typedef enum StmtType
{
    BLOCK_STMT = 0x0,
    IF_STMT,
    EXPRESSION_STMT,
    WHILE_STMT,
    FOR_STMT,
    STMTTYPE
}StmtType;

bool isValidOper(Token token, ExprType type);
const char* getExprTypeString(ExprType type);

class AbstractStmt
{
    public:
        StmtType getType() const;
        Block* getOwner() const;
        void addNextStmt(Statement* stmt);
        AbstractStmt* getNextStmt() const;
    protected:
        Statement* next = nullptr;
        Block* ownerBlock = nullptr;
        StmtType type;
};

class Block : public  AbstractStmt
{
    public:
        Block(Statement* stmts, Block* owner);
        void addVariable(std::string id, RootObject* obj);
        RootObject* getObject(std::string id);
    private:
        Block* owner;
        std::map<std::string, RootObject*> env;
        Statement* list;

};

class IfStmt : public AbstractStmt
{
    public:
        IfStmt(AstNode* predicate, Block* block, Block* owner);
        AstNode* getPredicate() const;
        Block* getStmt() const;
    private:
        AstNode* predicate;
        Block* stmtBlock;
};

class Expression : public AbstractStmt
{
    public:
        Expression(AstNode* expr, Block* owner);
        AstNode* getExpr() const;
    private:
        AstNode* expr;
};

class WhileStmt : public AbstractStmt
{
    public:
        WhileStmt(AstNode* predicate, Block* block, Block* owner);
        AstNode* getPredicate() const;
        Block* getBlock() const;
    private:
        AstNode* predicate;
        Block* stmtBlock;
};

class ForStmt : public AbstractStmt
{
    public:
        ForStmt(AstNode* init, AstNode* pred, AstNode* update, Block* block, Block* owner);
        AstNode* getInit() const;
        AstNode* getPredicate() const;
        AstNode* getUpdate() const;
        Block* getBlock() const;
    private:
        AstNode* init;
        AstNode* predicate;
        AstNode* update;
        Block* stmtBlock;
};

// basic expression primitives
class AbstractExpr
{
    public:
        ExprType getType() const;
    protected:
        ExprType type;
};

class Group : public AbstractExpr
{
    public:
        Group(Token leftRef, AstNode* exprRef, Token rightRef);
        Token getOperator(Placement place);
        AstNode* getExpr();
    private:
        Token oper[PAIR];
        AstNode* expr;
};

class UnaryExpr : public AbstractExpr
{
    public:
        UnaryExpr(AstNode* pOper, AstNode* pExpr);
        AstNode* getOperator();
        AstNode* getExpr();
    private:
        AstNode* oper;
        AstNode* expr;
};

class BinaryExpr : public AbstractExpr
{
    public:
        BinaryExpr(AstNode* lhs, AstNode* pOper, AstNode* rhs);
        AstNode* getExpr(Placement place);
        AstNode* getOperator();
    private:
        AstNode* expr[PAIR];
        AstNode* oper;
};

class LiteralExpr : public AbstractExpr 
{
    public:
        LiteralExpr(Token literal); 
        Token getToken() const;
    public:
        Token expr;
};

class UnaryOper : public AbstractExpr
{
    public:
        UnaryOper(Token token);
        Token getToken() const;
    public:
        Token oper;
};

class BinaryOper : public AbstractExpr
{
    public:
        BinaryOper(Token oper);
        Token getToken() const;
    public:
        Token oper;
};


