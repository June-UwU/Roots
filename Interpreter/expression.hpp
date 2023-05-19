#pragma once
#include "types.hpp"
#include "token.hpp"

class AbstractExpr;
class ExprVisitor;
class Expr;
class Group;
class UnaryExpr;
class BinaryExpr;
class LiteralExpr;
class BinaryOper;
class UnaryOper;

using Ast = AbstractExpr;
using AstNode = AbstractExpr;

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

bool isValidOper(Token token, ExprType type);
const char* getExprTypeString(ExprType type);

class AbstractExpr
{
    public:
        ExprType getType() const;
    protected:
        ExprType type;
};

class Expr : public AbstractExpr
{
    public:
        Expr(AstNode* sExpr, ExprType type);
        ~Expr();
    public:
        AstNode* expr;
};

class Group : public AbstractExpr
{
    public:
        Group(Token leftRef, AstNode* exprRef, Token rightRef);
    public:
        Token leftToken;
        AstNode* expr;
        Token rightToken;
};

class UnaryExpr : public AbstractExpr
{
    public:
        UnaryExpr(AstNode* pOper, AstNode* pExpr);
    public:
        AstNode* unaryOper;
        AstNode* expr;
};

class BinaryExpr : public AbstractExpr
{
    public:
        BinaryExpr(AstNode* lhsExpr, AstNode* pOper, AstNode* rhsExpr);
    public:
        AstNode* leftExpr;
        AstNode* binaryOper;
        AstNode* rightExpr;
};

class LiteralExpr : public AbstractExpr 
{
    public:
        LiteralExpr(Token literal); 
        Token getToken() const;
    public:
        Token literal;
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


