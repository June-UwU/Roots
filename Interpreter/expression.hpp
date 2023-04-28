#pragma once
#include "types.hpp"
#include "token.hpp"

class ExprVisitor;
class Expr;
class Group;
class UnaryExpr;
class BinaryExpr;
class LiteralExpir;
class BinaryOper;
class UnaryOper;

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

class AbstractExpr
{
    public:
        ExprType getType() const;
        virtual void accept(ExprVisitor& visitor) = 0;
    protected:
        ExprType type;
};

class ExprVisitor
{
    public:
        ExprVisitor() = default;
        void visit(AbstractExpr* expr);
    private:
};

class Expr : public AbstractExpr
{
    public:
        Expr(SharedRef<Literal> literalRef);
        Expr(SharedRef<Group> groupRef);
        Expr(SharedRef<UnaryExpr> unaryExprRef);
        Expr(SharedRef<BinaryExpr> binaryExprRef);
        void accept(ExprVisitor& visitorRef);
    private:
        SharedRef<Literal> literal;
        SharedRef<Group> group;
        SharedRef<UnaryExpr> unaryExpr;
        SharedRef<BinaryExpr> binaryExpr;
};

class Group : public AbstractExpr
{
    public:
        Group(Token leftRef, SharedRef<Expr> exprRef, Token rightRef);
        void accept(ExprVisitor& visitor);
    private:
        Token leftToken;
        SharedRef<Expr> expr;
        Token rightToken;
};

class UnaryExpr : public AbstractExpr
{
    public:
        UnaryExpr(SharedRef<UnaryOper> opRef, SharedRef<Expr> exprRef);
        void accept(ExprVisitor& visitor);
    private:
        SharedRef<UnaryOper> unaryOper;
        SharedRef<Expr> expr;
};

class BinaryExpr : public AbstractExpr
{
    public:
        BinaryExpr(SharedRef<Expr> lhsExprRef, SharedRef<BinaryOper> operRef, SharedRef<Expr> rhsExprRef);
        void accept(ExprVisitor& visitor);
    private:
        SharedRef<Expr> leftExpr;
        SharedRef<BinaryOper> binaryOper;
        SharedRef<Expr> rightExpr;
};

class LiteralExpr : public AbstractExpr 
{
    public:
        LiteralExpr(Token literalRef);
        void accept(ExprVisitor& visitor);
    private:
        Token literal;
};

class UnaryOper : public AbstractExpr
{
    public:
        UnaryOper(Token token);
        void accept(ExprVisitor& visitor);
    private:
        Token oper;
};

class BinaryOper : public AbstractExpr
{
    public:
        BinaryOper(Token oper);
        void accept(ExprVisitor& visitor);
    private:
        Token oper;
};
