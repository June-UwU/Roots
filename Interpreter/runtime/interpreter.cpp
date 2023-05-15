#include "interpreter.hpp"
#include "../logger.hpp"
#include <stack>
#include <variant>

constexpr const u32 UNINITIALIZED_REGISTER = 0x0BAD;
typedef enum RegisterState
{
    REGISTER_UNINITIALIZED = 0x0,
    REGISTER_INT32,
    REGISTER_UINT32,
    REGISTER_FLOAT,
    REGISTER_STRING,
    REGISTER_BOOLEAN
}RegisterState;

using Register = std::variant<bool,s32,u32,f32,std::string>;
using Stack = std::stack<AstNode*>;

void initializeRegister(Register& R1, const AstNode* literalNode, RegisterState& R1State);
void evaluate(Register& accumulator, RegisterState& type, const AstNode* node, Stack& stack);
void evaluateUnary(Register& accumulator, RegisterState& type, const AstNode* oper);
void evaluateBinary(Register& accumulator, RegisterState& type,const AstNode* oper, AstNode* rhs);

RootObject* interpret(const Ast* ast)
{
    std::stack<AstNode*> runtimeStack = Stack();
    Register R1;
    RegisterState R1State = REGISTER_UNINITIALIZED;
    runtimeStack.push((AstNode*)ast);
    AstNode* expr = nullptr;
    AstNode* oper = nullptr;
    while(!runtimeStack.empty())
    {
        expr = runtimeStack.top();
        runtimeStack.pop(); 
        switch(expr->getType())
        {
            case BINARY_EXPR:
            {
                BinaryExpr* bExpr = reinterpret_cast<BinaryExpr*>(expr);
                runtimeStack.push(bExpr->rightExpr);
                runtimeStack.push(bExpr->binaryOper);
                runtimeStack.push(bExpr->leftExpr);
            }break;
            case GROUP:
            {
                Group* group = reinterpret_cast<Group*>(expr);
                runtimeStack.push(group->expr);
            }break;
            case UNARY_EXPR:
            {
                UnaryExpr* uExpr = reinterpret_cast<UnaryExpr*>(expr);
                runtimeStack.push(uExpr->unaryOper);
                runtimeStack.push(uExpr->expr);
            }break;
            case EXPR:
            {
                Expr* bExpr = reinterpret_cast<Expr*>(expr);
                runtimeStack.push(bExpr->expr);
            }break;
            case LITERAL:
                if(REGISTER_UNINITIALIZED == R1State)
                {
                    initializeRegister(R1,expr,R1State);
                    oper = runtimeStack.top();
                    runtimeStack.pop();
                    evaluate(R1,R1State,oper,runtimeStack);
                }
                else
                {
                    LOG_WARN("Literal with an initialized register, check what case that generated this automata");
                    break;
                }
            case UNARY_OPER:
            case BINARY_OPER:
                    oper = runtimeStack.top();
                    runtimeStack.pop();
                    evaluate(R1,R1State,oper,runtimeStack);
                    break;
            default:
                LOG_WARN("default case for interpret");
        }
    }
    RootObject* retObj = nullptr;
    switch(R1State)
    {
        case REGISTER_FLOAT:
            retObj = new FloatObject(std::get<f32>(R1));
            break;
        case REGISTER_INT32:
            retObj = new IntObject(std::get<s32>(R1));
            break;
        case REGISTER_UINT32:
            retObj = new UintObject(std::get<u32>(R1));
            break;
        case REGISTER_STRING:
            retObj = new StringObject(std::get<std::string>(R1));
            break;
    }
    return retObj;
}

void evaluateUnary(Register& accumulator, RegisterState& type,const AstNode* oper)
{
    const UnaryOper* unaryOper = reinterpret_cast<const UnaryOper*>(oper);
    Token uOper = unaryOper->oper;
    switch(uOper.getTokenType())
    {
        case BANG:
        {
            switch(type)
            {
                case REGISTER_INT32:
                {
                    bool value = true;
                    s32 accuValue = std::get<s32>(accumulator);
                    if( 0x0 != accuValue)
                    {
                        value = false;
                    }
                    type = REGISTER_BOOLEAN;
                    accumulator.emplace<bool>(value);
                }break;
                case REGISTER_UINT32:
                {
                    bool value = true;
                    u32 accuValue = std::get<u32>(accumulator);
                    if( 0x0 != accuValue)
                    {
                        value = false;
                    }
                    type = REGISTER_BOOLEAN;
                    accumulator.emplace<bool>(value);
                }break;
                default:
                {
                    LOG_WARN("default case for unary expression");
                }break;
            }
        }break;
        case MINUS:
        {
            switch(type)
            {
                case REGISTER_INT32:
                {
                    accumulator.emplace<s32>((-1 * std::get<s32>(accumulator)));
                }break;
                case REGISTER_UINT32:
                {
                    accumulator.emplace<s32>((-1 * std::get<u32>(accumulator)));
                }break;
                case REGISTER_FLOAT:
                {
                    accumulator.emplace<s32>((-1 * std::get<f32>(accumulator)));
                }break;
                default:
                {
                    LOG_WARN("default case for binary expression");
                }break;
            }
            type = REGISTER_INT32;
        }break;
    }
}

void evaluate(Register& accumulator,RegisterState& type,const AstNode* oper, Stack& stack)
{
    switch(oper->getType())
    {
        case UNARY_OPER:
        {
            evaluateUnary(accumulator,type,oper);
        }break;
        case BINARY_OPER:
        {
            AstNode* rhs = stack.top();
            stack.pop();
            evaluateBinary(accumulator,type,oper,rhs);
        }break;
        default:
            LOG_WARN("evaluation error : oper is not an operator");
    }
}

void evaluateBinary(Register& accumulator, RegisterState& type,const AstNode* oper, AstNode* rhs)
{
    const BinaryOper* binaryOper = reinterpret_cast<const BinaryOper*>(oper);
    Token bOper = binaryOper->oper;
    LiteralExpr* literalExpr = reinterpret_cast<LiteralExpr*>(literalExpr);
    Literal literal = literalExpr->literal.getLiteral();
    switch(bOper.getTokenType())
    {
        case STAR:
        {
            if((type == REGISTER_STRING) || (literalExpr->literal.getTokenType() == STRING))
            {
                LOG_WARN("undefined operation called for operants");
                return;
            }
            else
            {
                if((type == REGISTER_FLOAT) || (literalExpr->literal.getTokenType() == FLOAT_NUMBER))
                {
                    if(type == REGISTER_FLOAT && literalExpr->literal.getTokenType() == FLOAT_NUMBER)
                    {
                        accumulator.emplace<f32>((std::get<f32>(accumulator) * std::get<f32>(literal))); 
                    }
                    else if(type == REGISTER_FLOAT)
                    {
                        f32 accValue = std::get<f32>(accumulator);
                        switch(literalExpr->literal.getTokenType())
                        {
                            case UNSIGNED_INTEGER_NUMBER:
                                accumulator.emplace<f32>((accValue * std::get<u32>(literal)));
                                break;
                            case SIGNED_INTEGER_NUMBER:
                                accumulator.emplace<f32>((accValue * std::get<s32>(literal)));
                                break;
                        }
                    }
                    else if(FLOAT_NUMBER == literalExpr->literal.getTokenType())
                    {
                        f32 value = std::get<f32>(literal);
                        type = REGISTER_FLOAT;
                        switch(type)
                        {
                            case REGISTER_INT32:
                                accumulator.emplace<f32>((value * std::get<s32>(accumulator)));
                                break;
                            case REGISTER_UINT32:
                                accumulator.emplace<f32>((value * std::get<u32>(accumulator)));
                                break;
                        }
                        type = REGISTER_FLOAT;
                    }
                    else
                    {
                        switch(literalExpr->literal.getTokenType())
                        {
                            case UNSIGNED_INTEGER_NUMBER:
                            {
                                u32 value = std::get<u32>(literal);
                                switch(type)
                                {
                                    case REGISTER_INT32:
                                    {
                                        accumulator.emplace<s32>((value * std::get<s32>(accumulator)));
                                    }break;
                                    case REGISTER_UINT32:
                                    {
                                        accumulator.emplace<u32>((value * std::get<u32>(accumulator)));
                                    }break;
                                }
                            }break;
                            case SIGNED_INTEGER_NUMBER:
                            {
                                s32 value = std::get<s32>(literal);
                                switch(type)
                                {
                                    case REGISTER_INT32:
                                    {
                                        accumulator.emplace<s32>((value * std::get<s32>(accumulator)));
                                    }break;
                                    case REGISTER_UINT32:
                                    {
                                        accumulator.emplace<s32>((value * std::get<u32>(accumulator)));
                                    }break;
                                }
                                type = REGISTER_INT32;
                            }break;
                        }
                    }
                }
            }
        }break;
        case SLASH:
        {
            if((type == REGISTER_STRING) || (literalExpr->literal.getTokenType() == STRING))
            {
                LOG_WARN("undefined operation called for operants");
                return;
            }
            else
            {
                if((type == REGISTER_FLOAT) || (literalExpr->literal.getTokenType() == FLOAT_NUMBER))
                {
                    if(type == REGISTER_FLOAT && literalExpr->literal.getTokenType() == FLOAT_NUMBER)
                    {
                        accumulator.emplace<f32>((std::get<f32>(accumulator) / std::get<f32>(literal))); 
                    }
                    else if(type == REGISTER_FLOAT)
                    {
                        f32 accValue = std::get<f32>(accumulator);
                        switch(literalExpr->literal.getTokenType())
                        {
                            case UNSIGNED_INTEGER_NUMBER:
                                accumulator.emplace<f32>((accValue / std::get<u32>(literal)));
                                break;
                            case SIGNED_INTEGER_NUMBER:
                                accumulator.emplace<f32>((accValue / std::get<s32>(literal)));
                                break;
                        }
                    }
                    else if(FLOAT_NUMBER == literalExpr->literal.getTokenType())
                    {
                        f32 value = std::get<f32>(literal);
                        switch(type)
                        {
                            case REGISTER_INT32:
                                accumulator.emplace<f32>((value / std::get<s32>(accumulator)));
                                break;
                            case REGISTER_UINT32:
                                accumulator.emplace<f32>((value / std::get<u32>(accumulator)));
                                break;
                        }
                        type = REGISTER_FLOAT;
                    }
                    else
                    {
                        switch(literalExpr->literal.getTokenType())
                        {
                            case UNSIGNED_INTEGER_NUMBER:
                            {
                                u32 value = std::get<u32>(literal);
                                switch(type)
                                {
                                    case REGISTER_INT32:
                                    {
                                        type = REGISTER_INT32;
                                        accumulator.emplace<s32>((s32)(value / std::get<s32>(accumulator)));
                                    }break;
                                    case REGISTER_UINT32:
                                    {
                                        accumulator.emplace<u32>((u32)(value / std::get<u32>(accumulator)));
                                    }break;
                                }
                            }break;
                            case SIGNED_INTEGER_NUMBER:
                            {
                                s32 value = std::get<s32>(literal);
                                type = REGISTER_INT32;
                                switch(type)
                                {
                                    case REGISTER_INT32:
                                    {
                                        accumulator.emplace<s32>((s32)(value / std::get<s32>(accumulator)));
                                    }break;
                                    case REGISTER_UINT32:
                                    {
                                        accumulator.emplace<s32>((s32)(value / std::get<u32>(accumulator)));
                                    }break;
                                }
                            }break;
                        }
                    }
                }
            }
        }break;
        case PLUS:
        {
            if((type == REGISTER_STRING) || (literalExpr->literal.getTokenType() == STRING))
            {
                type = REGISTER_STRING;
                accumulator.emplace<std::string>(std::get<std::string>(accumulator) +  std::get<std::string>(literal));
            }
            else
            {
                if((type == REGISTER_FLOAT) || (literalExpr->literal.getTokenType() == FLOAT_NUMBER))
                {
                    if(type == REGISTER_FLOAT && literalExpr->literal.getTokenType() == FLOAT_NUMBER)
                    {
                        accumulator.emplace<f32>((std::get<f32>(accumulator) + std::get<f32>(literal))); 
                    }
                    else if(type == REGISTER_FLOAT)
                    {
                        f32 accValue = std::get<f32>(accumulator);
                        switch(literalExpr->literal.getTokenType())
                        {
                            case UNSIGNED_INTEGER_NUMBER:
                                accumulator.emplace<f32>((accValue + std::get<u32>(literal)));
                                break;
                            case SIGNED_INTEGER_NUMBER:
                                accumulator.emplace<f32>((accValue + std::get<s32>(literal)));
                                break;
                        }
                    }
                    else if(FLOAT_NUMBER == literalExpr->literal.getTokenType())
                    {
                        f32 value = std::get<f32>(literal);
                        type = REGISTER_FLOAT;
                        switch(type)
                        {
                            case REGISTER_INT32:
                                accumulator.emplace<f32>((value + std::get<s32>(accumulator)));
                                break;
                            case REGISTER_UINT32:
                                accumulator.emplace<f32>((value + std::get<u32>(accumulator)));
                                break;
                        }
                    }
                    else
                    {
                        switch(literalExpr->literal.getTokenType())
                        {
                            case UNSIGNED_INTEGER_NUMBER:
                            {
                                u32 value = std::get<u32>(literal);
                                switch(type)
                                {
                                    case REGISTER_INT32:
                                    {
                                        accumulator.emplace<s32>((value + std::get<s32>(accumulator)));
                                    }break;
                                    case REGISTER_UINT32:
                                    {
                                        accumulator.emplace<u32>((value + std::get<u32>(accumulator)));
                                    }break;
                                }
                            }break;
                            case SIGNED_INTEGER_NUMBER:
                            {
                                s32 value = std::get<s32>(literal);
                                type = REGISTER_INT32;
                                switch(type)
                                {
                                    case REGISTER_INT32:
                                    {
                                        accumulator.emplace<s32>((value + std::get<s32>(accumulator)));
                                    }break;
                                    case REGISTER_UINT32:
                                    {
                                        accumulator.emplace<s32>((value + std::get<u32>(accumulator)));
                                    }break;
                                }
                            }break;
                        }
                    }
                }
            }
        }break;
        case MINUS:
        {
            if((type == REGISTER_STRING) || (literalExpr->literal.getTokenType() == STRING))
            {
                LOG_WARN("undefined operation called for operants");
                return;
            }
            else
            {
                if((type == REGISTER_FLOAT) || (literalExpr->literal.getTokenType() == FLOAT_NUMBER))
                {
                    if(type == REGISTER_FLOAT && literalExpr->literal.getTokenType() == FLOAT_NUMBER)
                    {
                        accumulator.emplace<f32>((std::get<f32>(accumulator) - std::get<f32>(literal))); 
                    }
                    else if(type == REGISTER_FLOAT)
                    {
                        f32 accValue = std::get<f32>(accumulator);
                        switch(literalExpr->literal.getTokenType())
                        {
                            case UNSIGNED_INTEGER_NUMBER:
                                accumulator.emplace<f32>((accValue - std::get<u32>(literal)));
                                break;
                            case SIGNED_INTEGER_NUMBER:
                                accumulator.emplace<f32>((accValue - std::get<s32>(literal)));
                                break;
                        }
                    }
                    else if(FLOAT_NUMBER == literalExpr->literal.getTokenType())
                    {
                        f32 value = std::get<f32>(literal);
                        type = REGISTER_FLOAT;
                        switch(type)
                        {
                            case REGISTER_INT32:
                                accumulator.emplace<f32>((value - std::get<s32>(accumulator)));
                                break;
                            case REGISTER_UINT32:
                                accumulator.emplace<f32>((value - std::get<u32>(accumulator)));
                                break;
                        }
                    }
                    else
                    {
                        switch(literalExpr->literal.getTokenType())
                        {
                            case UNSIGNED_INTEGER_NUMBER:
                            {
                                u32 value = std::get<u32>(literal);
                                switch(type)
                                {
                                    case REGISTER_INT32:
                                    {
                                        accumulator.emplace<s32>((value - std::get<s32>(accumulator)));
                                    }break;
                                    case REGISTER_UINT32:
                                    {
                                        accumulator.emplace<u32>((value - std::get<u32>(accumulator)));
                                    }break;
                                }
                            }break;
                            case SIGNED_INTEGER_NUMBER:
                            {
                                s32 value = std::get<s32>(literal);
                                type = REGISTER_INT32;
                                switch(type)
                                {
                                    case REGISTER_INT32:
                                    {
                                        accumulator.emplace<s32>((value - std::get<s32>(accumulator)));
                                    }break;
                                    case REGISTER_UINT32:
                                    {
                                        accumulator.emplace<s32>((value - std::get<u32>(accumulator)));
                                    }break;
                                }
                            }break;
                        }
                    }
                }
            }
        }break;
    }
}

void initializeRegister(Register& R1, const AstNode* node, RegisterState& R1State)
{
    const LiteralExpr* expr = reinterpret_cast<const LiteralExpr*>(node); 
    switch(expr->getToken().getTokenType())
    {
        case STRING:
            R1.emplace<std::string>(std::get<std::string>(expr->literal.getLiteral()));
            R1State = REGISTER_STRING;
            break;
        case UNSIGNED_INTEGER_NUMBER:
            R1.emplace<u32>(std::get<u32>(expr->literal.getLiteral()));
            R1State = REGISTER_UINT32;
            break;
        case SIGNED_INTEGER_NUMBER:
            R1.emplace<s32>(std::get<s32>(expr->literal.getLiteral()));
            R1State = REGISTER_INT32;
            break;
        case FLOAT_NUMBER:
            R1.emplace<f32>(std::get<f32>(expr->literal.getLiteral()));
            R1State = REGISTER_FLOAT;
            break;
        default:
            LOG_WARN("register initialization default case might be a error");
            break;
    }
}

