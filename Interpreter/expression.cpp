#include "expression.hpp"

constexpr const char* EXPRTYPE_STR[EXPRTYPE]
{
    "EXPR","GROUP","UNARY_EXPR","BINARY_EXPR",
    "LITERAL","BINARY_OPER","UNARY_OPER","EXPR_LITERAL",
    "EXPR_GROUP","EXPR_UNARY","EXPR_BINARY"
};

ExprType AbstractExpr::getType() const
{
    return type;
}

Expr::Expr(AstNode* sExpr, ExprType exprType)
{
    expr = sExpr;
    type = exprType;
}

Group::Group(Token lhs, AstNode* midExpr, Token rhs)
    :leftToken{lhs}
    ,expr{midExpr}
    ,rightToken{rhs}
{
    type = GROUP;
}

UnaryExpr::UnaryExpr(AstNode* oper, AstNode* exprRef)
    :unaryOper{oper}
    ,expr{exprRef}
{
    type = UNARY_EXPR;
}

BinaryExpr::BinaryExpr(AstNode* lhs, AstNode* oper, AstNode* rhs)
    :leftExpr{lhs}
    ,binaryOper{oper}
    ,rightExpr{rhs}
{
    type = BINARY_EXPR;
}

LiteralExpr::LiteralExpr(Token literalRef)
    :literal{literalRef}
{
    type = LITERAL;
}

Token LiteralExpr::getToken() const
{
    return literal;
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

