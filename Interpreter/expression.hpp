#pragma once
#include "types.hpp"
#include "token.hpp"

class AbstractExpr;
class Group;
class UnaryExpr;
class BinaryExpr;
class LiteralExpr;
class BinaryOper;
class UnaryOper;

using Ast = AbstractExpr;
using AstNode = AbstractExpr;
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
    STUDSTMT = 0x0, // TODO : change this to whatever is required
    STMTTYPE
}StmtType;

bool isValidOper(Token token, ExprType type);
const char* getExprTypeString(ExprType type);

class AbstractStmt
{
    public:
        StmtType getType() const;
    protected:
        StmtType type;
};

class VarDeclStmt : public AbstractStmt
{
    public:
    private:
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


