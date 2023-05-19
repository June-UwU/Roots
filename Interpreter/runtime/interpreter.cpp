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
                runtimeStack.push(bExpr->getExpr(RHS));
                runtimeStack.push(bExpr->getOperator());
                runtimeStack.push(bExpr->getExpr(LHS));
            }break;
            case GROUP:
            {
                Group* group = reinterpret_cast<Group*>(expr);
                runtimeStack.push(group->getExpr());
            }break;
            case UNARY_EXPR:
            {
                UnaryExpr* uExpr = reinterpret_cast<UnaryExpr*>(expr);
                runtimeStack.push(uExpr->getOperator());
                runtimeStack.push(uExpr->getExpr());
            }break;
            case EXPR:
            {
                Expr* bExpr = reinterpret_cast<Expr*>(expr);
                runtimeStack.push(bExpr->getExpr());
            }break;
            case LITERAL:
            {
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
                }
            }break;
            case UNARY_OPER:
            case BINARY_OPER:
            {       
                oper = runtimeStack.top();
                runtimeStack.pop();
                evaluate(R1,R1State,oper,runtimeStack);
            }break;
            default:
                LOG_WARN("default case for interpret");
            break;
        }
    }
    LOG_INFO("Root Object Creation Phase");
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
    Token uOper = unaryOper->getToken();
    switch(uOper.getTokenType())
    {
        case BANG:
        {
            switch(type)
            {
                case REGISTER_INT32:
                {
                    bool value = true;
                    s32 regVal = std::get<s32>(accumulator);
                    if( 0x0 != regVal)
                    {
                        value = false;
                    }
                    type = REGISTER_BOOLEAN;
                    accumulator.emplace<bool>(value);
                }break;
                case REGISTER_UINT32:
                {
                    bool value = true;
                    u32 regVal = std::get<u32>(accumulator);
                    if( 0x0 != regVal)
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
        break;
    }
}

void evaluateBinary(Register& accumulator, RegisterState& type,const AstNode* oper, AstNode* rhs)
{
    const BinaryOper* binaryOper = reinterpret_cast<const BinaryOper*>(oper);
    Token bOper = binaryOper->oper;
    LiteralExpr* operand = reinterpret_cast<LiteralExpr*>(rhs);
    Token operandToken = operand->getToken();
    Literal operandVal = operandToken.getLiteral();
    switch(bOper.getTokenType())
    {
        case PLUS:
        {
            TokenType operandType = operandToken.getTokenType(); 
            if(operandType == STRING && type == REGISTER_STRING)
            {
                accumulator.emplace<std::string>(std::get<std::string>(operandVal) 
                + std::get<std::string>(accumulator));
            }
            else if(type == REGISTER_FLOAT && operandType != STRING)
            {
                f32 regConst = std::get<f32>(accumulator);
                switch (operandType)
                {
                    case UNSIGNED_INTEGER_NUMBER:
                    {
                        accumulator.emplace<f32>(std::get<u32>(operandVal) + regConst);
                    }break;
                    case SIGNED_INTEGER_NUMBER:
                    {
                        accumulator.emplace<f32>(std::get<s32>(operandVal) + regConst);
                    }break;
                    case FLOAT_NUMBER:
                    {
                        accumulator.emplace<f32>(std::get<f32>(operandVal) + regConst);
                    }break;
                    default:
                        LOG_WARN("unsupported types for operation attempt");
                    break;
                }
            }
            else if(operandType == FLOAT_NUMBER && type != REGISTER_STRING)
            {
                type = REGISTER_FLOAT;
                f32 operConst = std::get<f32>(operandVal);
                switch(type)
                {
                    case REGISTER_INT32:
                    {
                        accumulator.emplace<f32>(operConst + std::get<s32>(accumulator));
                    }break;
                    case REGISTER_UINT32:
                    {
                        accumulator.emplace<f32>(operConst + std::get<u32>(accumulator));
                    }break;
                    default:
                        LOG_WARN("unsupported types for operation attempt");
                    break;
                }
            }
            else
            {
                switch (type)
                {
                    case REGISTER_INT32:
                    {
                        s32 regVal = std::get<s32>(accumulator);
                        switch (operandType)
                        {
                            case UNSIGNED_INTEGER_NUMBER:
                            {
                                accumulator.emplace<s32>(regVal + std::get<u32>(operandVal));
                            }break;
                            case SIGNED_INTEGER_NUMBER:
                            {
                                accumulator.emplace<s32>(regVal + std::get<s32>(operandVal));
                            }break;
                            default:
                                LOG_WARN("unsupported types for operation attempt");
                            break;
                        }
                    }break;
                    case REGISTER_UINT32:
                    {
                        u32 regVal = std::get<u32>(accumulator);
                        switch(operandType)
                        {
                            case UNSIGNED_INTEGER_NUMBER:
                            {
                                accumulator.emplace<u32>(regVal + std::get<u32>(operandVal));
                            }break;
                            case SIGNED_INTEGER_NUMBER:
                            {
                                type = REGISTER_INT32;
                                accumulator.emplace<s32>(regVal + std::get<s32>(operandVal));
                            }break;
                            default:
                                LOG_WARN("unsupported types for operation attempt");
                            break;
                        }
                    }break;             
                    default:
                        LOG_WARN("unsupported types for operation attempt");
                    break;
                }
            }
        }break;
        case MINUS:
        {
            TokenType operandType = operandToken.getTokenType(); 
            if(operandType == STRING && type == REGISTER_STRING)
            {
                LOG_WARN("unsupported types for operation attempt");
            }
            else if(type == REGISTER_FLOAT && operandType != STRING)
            {
                f32 regConst = std::get<f32>(accumulator);
                switch (operandType)
                {
                    case UNSIGNED_INTEGER_NUMBER:
                    {
                        accumulator.emplace<f32>(std::get<u32>(operandVal) - regConst);
                    }break;
                    case SIGNED_INTEGER_NUMBER:
                    {
                        accumulator.emplace<f32>(std::get<s32>(operandVal) - regConst);
                    }break;
                    case FLOAT_NUMBER:
                    {
                        accumulator.emplace<f32>(std::get<f32>(operandVal) - regConst);
                    }break;
                    default:
                        LOG_WARN("unsupported types for operation attempt");
                    break;
                }
            }
            else if(operandType == FLOAT_NUMBER && type != REGISTER_STRING)
            {
                type = REGISTER_FLOAT;
                f32 operConst = std::get<f32>(operandVal);
                switch(type)
                {
                    case REGISTER_INT32:
                    {
                        accumulator.emplace<f32>(operConst - std::get<s32>(accumulator));
                    }break;
                    case REGISTER_UINT32:
                    {
                        accumulator.emplace<f32>(operConst - std::get<u32>(accumulator));
                    }break;
                    default:
                        LOG_WARN("unsupported types for operation attempt");
                    break;
                }
            }
            else
            {
                switch (type)
                {
                    case REGISTER_INT32:
                    {
                        s32 regVal = std::get<s32>(accumulator);
                        switch (operandType)
                        {
                            case UNSIGNED_INTEGER_NUMBER:
                            {
                                accumulator.emplace<s32>(regVal - std::get<u32>(operandVal));
                            }break;
                            case SIGNED_INTEGER_NUMBER:
                            {
                                accumulator.emplace<s32>(regVal - std::get<s32>(operandVal));
                            }break;
                            default:
                                LOG_WARN("unsupported types for operation attempt");
                            break;
                        }
                    }break;
                    case REGISTER_UINT32:
                    {
                        u32 regVal = std::get<u32>(accumulator);
                        switch(operandType)
                        {
                            case UNSIGNED_INTEGER_NUMBER:
                            {
                                accumulator.emplace<u32>(regVal - std::get<u32>(operandVal));
                            }break;
                            case SIGNED_INTEGER_NUMBER:
                            {
                                type = REGISTER_INT32;
                                accumulator.emplace<s32>(regVal - std::get<s32>(operandVal));
                            }break;
                            default:
                                LOG_WARN("unsupported types for operation attempt");
                            break;
                        }
                    }break;             
                    default:
                        LOG_WARN("unsupported types for operation attempt");
                    break;
                }
            }
        }break;
        case STAR:
        {
            TokenType operandType = operandToken.getTokenType(); 
            if(operandType == STRING && type == REGISTER_STRING)
            {
                LOG_WARN("unsupported types for operation attempt");
            }
            else if(type == REGISTER_FLOAT && operandType != STRING)
            {
                f32 regConst = std::get<f32>(accumulator);
                switch (operandType)
                {
                    case UNSIGNED_INTEGER_NUMBER:
                    {
                        accumulator.emplace<f32>(std::get<u32>(operandVal) * regConst);
                    }break;
                    case SIGNED_INTEGER_NUMBER:
                    {
                        accumulator.emplace<f32>(std::get<s32>(operandVal) * regConst);
                    }break;
                    case FLOAT_NUMBER:
                    {
                        accumulator.emplace<f32>(std::get<f32>(operandVal) * regConst);
                    }break;
                    default:
                        LOG_WARN("unsupported types for operation attempt");
                    break;
                }
            }
            else if(operandType == FLOAT_NUMBER && type != REGISTER_STRING)
            {
                type = REGISTER_FLOAT;
                f32 operConst = std::get<f32>(operandVal);
                switch(type)
                {
                    case REGISTER_INT32:
                    {
                        accumulator.emplace<f32>(operConst * std::get<s32>(accumulator));
                    }break;
                    case REGISTER_UINT32:
                    {
                        accumulator.emplace<f32>(operConst * std::get<u32>(accumulator));
                    }break;
                    default:
                        LOG_WARN("unsupported types for operation attempt");
                    break;
                }
            }
            else
            {
                switch (type)
                {
                    case REGISTER_INT32:
                    {
                        s32 regVal = std::get<s32>(accumulator);
                        switch (operandType)
                        {
                            case UNSIGNED_INTEGER_NUMBER:
                            {
                                accumulator.emplace<s32>(regVal * std::get<u32>(operandVal));
                            }break;
                            case SIGNED_INTEGER_NUMBER:
                            {
                                accumulator.emplace<s32>(regVal * std::get<s32>(operandVal));
                            }break;
                            default:
                                LOG_WARN("unsupported types for operation attempt");
                            break;
                        }
                    }break;
                    case REGISTER_UINT32:
                    {
                        u32 regVal = std::get<u32>(accumulator);
                        switch(operandType)
                        {
                            case UNSIGNED_INTEGER_NUMBER:
                            {
                                accumulator.emplace<u32>(regVal * std::get<u32>(operandVal));
                            }break;
                            case SIGNED_INTEGER_NUMBER:
                            {
                                type = REGISTER_INT32;
                                accumulator.emplace<s32>(regVal * std::get<s32>(operandVal));
                            }break;
                            default:
                                LOG_WARN("unsupported types for operation attempt");
                            break;
                        }
                    }break;             
                    default:
                        LOG_WARN("unsupported types for operation attempt");
                    break;
                }
            }
        }break;
        case SLASH:
        {
            TokenType operandType = operandToken.getTokenType(); 
            if(operandType == STRING && type == REGISTER_STRING)
            {
                LOG_WARN("unsupported types for operation attempt");
            }
            else if(type == REGISTER_FLOAT && operandType != STRING)
            {
                f32 regConst = std::get<f32>(accumulator);
                switch (operandType)
                {
                    case UNSIGNED_INTEGER_NUMBER:
                    {
                        accumulator.emplace<f32>(std::get<u32>(operandVal) / regConst);
                    }break;
                    case SIGNED_INTEGER_NUMBER:
                    {
                        accumulator.emplace<f32>(std::get<s32>(operandVal) / regConst);
                    }break;
                    case FLOAT_NUMBER:
                    {
                        accumulator.emplace<f32>(std::get<f32>(operandVal) / regConst);
                    }break;
                    default:
                        LOG_WARN("unsupported types for operation attempt");
                    break;
                }
            }
            else if(operandType == FLOAT_NUMBER && type != REGISTER_STRING)
            {
                type = REGISTER_FLOAT;
                f32 operConst = std::get<f32>(operandVal);
                switch(type)
                {
                    case REGISTER_INT32:
                    {
                        accumulator.emplace<f32>(operConst / std::get<s32>(accumulator));
                    }break;
                    case REGISTER_UINT32:
                    {
                        accumulator.emplace<f32>(operConst / std::get<u32>(accumulator));
                    }break;
                    default:
                        LOG_WARN("unsupported types for operation attempt");
                    break;
                }
            }
            else
            {
                switch (type)
                {
                    case REGISTER_INT32:
                    {
                        s32 regVal = std::get<s32>(accumulator);
                        switch (operandType)
                        {
                            case UNSIGNED_INTEGER_NUMBER:
                            {
                                accumulator.emplace<s32>(regVal / std::get<u32>(operandVal));
                            }break;
                            case SIGNED_INTEGER_NUMBER:
                            {
                                accumulator.emplace<s32>(regVal / std::get<s32>(operandVal));
                            }break;
                            default:
                                LOG_WARN("unsupported types for operation attempt");
                            break;
                        }
                    }break;
                    case REGISTER_UINT32:
                    {
                        u32 regVal = std::get<u32>(accumulator);
                        switch(operandType)
                        {
                            case UNSIGNED_INTEGER_NUMBER:
                            {
                                accumulator.emplace<u32>(regVal / std::get<u32>(operandVal));
                            }break;
                            case SIGNED_INTEGER_NUMBER:
                            {
                                type = REGISTER_INT32;
                                accumulator.emplace<s32>(regVal / std::get<s32>(operandVal));
                            }break;
                            default:
                                LOG_WARN("unsupported types for operation attempt");
                            break;
                        }
                    }break;             
                    default:
                        LOG_WARN("unsupported types for operation attempt");
                    break;
                }
            }
        }break;
        default:
            LOG_WARN("unsupported operation");
        break;
    }
}

void initializeRegister(Register& R1, const AstNode* node, RegisterState& R1State)
{
    const LiteralExpr* expr = reinterpret_cast<const LiteralExpr*>(node);
    Literal litValue = expr->getToken().getLiteral();  
    switch(expr->getToken().getTokenType())
    {
        case STRING:
            R1.emplace<std::string>(std::get<std::string>(litValue));
            R1State = REGISTER_STRING;
            break;
        case UNSIGNED_INTEGER_NUMBER:
            R1.emplace<u32>(std::get<u32>(litValue));
            R1State = REGISTER_UINT32;
            break;
        case SIGNED_INTEGER_NUMBER:
            R1.emplace<s32>(std::get<s32>(litValue));
            R1State = REGISTER_INT32;
            break;
        case FLOAT_NUMBER:
            R1.emplace<f32>(std::get<f32>(litValue));
            R1State = REGISTER_FLOAT;
            break;
        default:
            LOG_WARN("register initialization default case might be a error");
            break;
    }
}

