#include "expression.hpp"

constexpr const char* EXPRTYPE_STR[EXPRTYPE]
{
    "EXPR","GROUP","UNARY_EXPR","BINARY_EXPR",
    "LITERAL","BINARY_OPER","UNARY_OPER","EXPR_LITERAL",
    "EXPR_GROUP","EXPR_UNARY","EXPR_BINARY"
};

void ExprVisitor::visit(AbstractExpr* expr)
{
    switch(expr->getType())
        {
            case EXPR:
            case GROUP:
            case UNARY_EXPR:
            case BINARY_EXPR:
            case LITERAL:
            case BINARY_OPER:
            case UNARY_OPER:
            case EXPR_LITERAL:
            case EXPR_GROUP:
            case EXPR_UNARY:
            case EXPR_BINARY:
            default:
            break;
        }
}

ExprType AbstractExpr::getType() const
{
    return type;
}

Expr::Expr(SharedRef<LiteralExpr> literalRef)
    :literal{literalRef}
{
    type = EXPR_LITERAL;
}

Expr::Expr(SharedRef<Group> groupRef)
    :group{groupRef}
{
    type = EXPR_GROUP;
}

Expr::Expr(SharedRef<UnaryExpr> unary)
    :unaryExpr{unary}
{
    type = EXPR_UNARY;
}

Expr::Expr(SharedRef<BinaryExpr> binary)
    :binaryExpr{binary}
{
    type = EXPR_BINARY;
}

void Expr::accept(ExprVisitor& visitor)
{
    visitor.visit(this);
}

Group::Group(Token lhs, SharedRef<Expr> midExpr, Token rhs)
    :leftToken{lhs}
    ,expr{midExpr}
    ,rightToken{rhs}
{
    type = GROUP;
}

void Group::accept(ExprVisitor& visitor)
{
    visitor.visit(this);
}

UnaryExpr::UnaryExpr(SharedRef<UnaryOper> oper, SharedRef<Expr> exprRef)
    :unaryOper{oper}
    ,expr{exprRef}
{
    type = UNARY_EXPR;
}

void UnaryExpr::accept(ExprVisitor& visitor)
{
    visitor.visit(this);
}

BinaryExpr::BinaryExpr(SharedRef<Expr> lhs, SharedRef<BinaryOper> oper, SharedRef<Expr> rhs)
    :leftExpr{lhs}
    ,rightExpr{rhs}
{
    type = BINARY_EXPR;
}

void BinaryExpr::accept(ExprVisitor& visitor)
{
    visitor.visit(this);
}

LiteralExpr::LiteralExpr(Token literalRef)
    :literal{literalRef}
{
    type = LITERAL;
}

void LiteralExpr::accept(ExprVisitor& visitor)
{
    visitor.visit(this);
}

UnaryOper::UnaryOper(Token operToken)
    :oper{operToken}
{
    type = UNARY_OPER;
}

void UnaryOper::accept(ExprVisitor& visitor)
{
    visitor.visit(this);
}

BinaryOper::BinaryOper(Token operToken)
    :oper{operToken}
{
    type = BINARY_OPER;
}

void BinaryOper::accept(ExprVisitor& visitor)
{
    visitor.visit(this);
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
