#include "Ast.hpp"
#include "../expression.hpp"

#define REF_INC() ((*ref) = (*ref) + 1)  
#define REF_DEC() ((*ref) = (*ref) - 1)
#define REF_ZERO() ((*ref) = 0)

void deleteNode(void* ptr);

AstNode::AstNode(void* pData)
{
    data = pData;
    if(nullptr == ref)
    {
        ref = new u32;
    }
    if(((*ref) > 0))
    {
        REF_ZERO();
    }
    REF_INC();
}

AstNode::AstNode(const AstNode& rhs)
{
    data = rhs.data;
    ref = rhs.ref;
    REF_INC();
}

AstNode& AstNode::operator=(const AstNode& rhs)
{
    data = rhs.data;
    ref = rhs.ref;
    REF_INC();
    return *this;
}

AstNode::AstNode(AstNode&& rhs)
{
    data = std::move(rhs.data);
    ref = std::move(rhs.ref);
}
        
AstNode& AstNode::operator=(AstNode&& rhs)
{
    data = std::move(rhs.data);
    ref = std::move(rhs.ref);
    return *this;
}

AstNode::~AstNode()
{
    if(nullptr != ref)
    {
        REF_DEC();
        if((0 == (*ref)))
        {
            deleteNode(data);
            delete ref;
        }
    }
}

void* AstNode::get()
{
    return data;
}

void AstNode::reset()
{
    data = nullptr;
    REF_ZERO();
}

void deleteNode(void* ptr)
{
    AbstractExpr* expr = reinterpret_cast<AbstractExpr*>(ptr);
    switch(expr->getType())
    {
        case BINARY_EXPR:
        {
            delete reinterpret_cast<BinaryExpr*>(expr);
        }break;
        case UNARY_EXPR:
        {
            delete reinterpret_cast<UnaryExpr*>(expr);
        }break;
        case GROUP:
        {
            delete reinterpret_cast<Group*>(expr);
        }break;
        default:
            return;
    }
}
