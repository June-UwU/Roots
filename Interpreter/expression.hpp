#pragma once
#include "types.hpp"
#include "token.hpp"
#include "Ast.hpp"

class ExprVisitor;
class Expr;
class Group;
class UnaryExpr;
class BinaryExpr;
class LiteralExpr;
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
const char* getExprTypeString(ExprType type);

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
        Expr(AstNode* sExpr, ExprType type);
        ~Expr();
        void accept(ExprVisitor& visitorRef);
    public:
        AstNode* expr;
};

class Group : public AbstractExpr
{
    public:
        Group(Token leftRef, AstNode* exprRef, Token rightRef);
        ~Group();
        void accept(ExprVisitor& visitor);
    public:
        Token leftToken;
        AstNode* expr;
        Token rightToken;
};

class UnaryExpr : public AbstractExpr
{
    public:
        UnaryExpr(AstNode* pOper, AstNode* pExpr);
        ~UnaryExpr();
        void accept(ExprVisitor& visitor);
    public:
        AstNode* unaryOper;
        AstNode* expr;
};

class BinaryExpr : public AbstractExpr
{
    public:
        BinaryExpr(AstNode* lhsExpr, AstNode* pOper, AstNode* rhsExpr);
        ~BinaryExpr();
        void accept(ExprVisitor& visitor);
    public:
        AstNode* leftExpr;
        AstNode* binaryOper;
        AstNode* rightExpr;
};

class LiteralExpr : public AbstractExpr 
{
    public:
        LiteralExpr(Token literal); 
        void accept(ExprVisitor& visitor);
        Token getToken() const;
    public:
        Token literal;
};

class UnaryOper : public AbstractExpr
{
    public:
        UnaryOper(Token token);
        void accept(ExprVisitor& visitor);
        Token getToken() const;
    public:
        Token oper;
};

class BinaryOper : public AbstractExpr
{
    public:
        BinaryOper(Token oper);
        void accept(ExprVisitor& visitor);
        Token getToken() const;
    public:
        Token oper;
};
