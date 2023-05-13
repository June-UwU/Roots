#include "astPrettyPrinter.hpp"
#include <iostream>
#include <deque>

void levelPrint(s32 level);
void printNode(AstNode* node);
void prettyPrint(AstNode* expr);
bool isEndNode(AstNode* node);

void astPrettyPrint(Ast* root)
{
    prettyPrint(root);
}

void levelPrint(s32 level)
{
    for(int i = level; i > 0; i--)
    {
        std::cout << "\t";
        if((i - 1) == 0)
        {
            std::cout << "|";
        }
    }
    std::cout << "-";
}

void printNode(AstNode* node, s32 level)
{
    levelPrint(level);
    std::cout << "[" << getExprTypeString(node->getType());
    switch(node->getType())
    {
        case LITERAL:
        {
            LiteralExpr* literal = reinterpret_cast<LiteralExpr*>(node);
            std::cout << " " << literal->getToken().toString();
        }break;
        case BINARY_OPER:
        {
            BinaryOper* bOper = reinterpret_cast<BinaryOper*>(node);
            std::cout << " " << bOper->getToken().toString();
        }break;
        case UNARY_OPER:
        {
            UnaryOper* uOper = reinterpret_cast<UnaryOper*>(node);
            std::cout << " " << uOper->getToken().toString();
        }break;
        default:
            break;
    }
    std::cout << "]\n";
}


void prettyPrint(AstNode* expr)
{
    std::deque<AstNode*> queue;
    queue.push_back(expr);
    s32 level = 0;
    while(!queue.empty())
    {
        AstNode* aExpr = reinterpret_cast<AstNode*>(queue.front());
        queue.pop_front();
        printNode(aExpr,level);
        switch(aExpr->getType())
        {
            case EXPR:
            {
                Expr* rExpr = reinterpret_cast<Expr*>(aExpr);
                queue.push_back(rExpr->expr);
                level++;
            }break;
            case BINARY_EXPR:
            {
                BinaryExpr* bExpr = reinterpret_cast<BinaryExpr*>(aExpr);
                queue.push_back(bExpr->leftExpr);
                queue.push_back(bExpr->binaryOper);
                queue.push_back(bExpr->rightExpr);
                level++;
            }break;
            case UNARY_EXPR:
            {
                UnaryExpr* uExpr = reinterpret_cast<UnaryExpr*>(aExpr);
                queue.push_back(uExpr->unaryOper);
                queue.push_back(uExpr->expr);
                level++;
            }break;
            case GROUP:
            {
                Group* group = reinterpret_cast<Group*>(aExpr);
                queue.push_back(group->expr);
                level++;
            }break;
            default:
                break;
        }
    }
}

bool isEndNode(AstNode* node)
{
    switch(node->getType())
    {
        case LITERAL:
        case BINARY_OPER:
        case UNARY_OPER:
            return true;
        default:
            return false;
    }
}
