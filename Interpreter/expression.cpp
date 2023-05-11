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

Expr::Expr(AstNode* sExpr, ExprType exprType)
{
    expr = sExpr;
    type = exprType;
}

Expr::~Expr()
{
    delete expr;
}

void Expr::accept(ExprVisitor& visitor)
{
    visitor.visit(this);
}

Group::Group(Token lhs, AstNode* midExpr, Token rhs)
    :leftToken{lhs}
    ,expr{midExpr}
    ,rightToken{rhs}
{
    type = GROUP;
}

Group::~Group()
{
    delete expr;
}

void Group::accept(ExprVisitor& visitor)
{
    visitor.visit(this);
}

UnaryExpr::UnaryExpr(AstNode* oper, AstNode* exprRef)
    :unaryOper{oper}
    ,expr{exprRef}
{
    type = UNARY_EXPR;
}

UnaryExpr::~UnaryExpr()
{
    delete expr;
}

void UnaryExpr::accept(ExprVisitor& visitor)
{
    visitor.visit(this);
}

BinaryExpr::BinaryExpr(AstNode* lhs, AstNode* oper, AstNode* rhs)
    :leftExpr{lhs}
    ,binaryOper{oper}
    ,rightExpr{rhs}
{
    type = BINARY_EXPR;
}

BinaryExpr::~BinaryExpr()
{
    delete leftExpr;
    delete rightExpr;
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

Token LiteralExpr::getToken() const
{
    return literal;
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

Token UnaryOper::getToken() const
{
    return oper;
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

