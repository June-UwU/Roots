#include "interpreter.hpp"
#include "instructions.hpp"
#include "../logger.hpp"
#include "virtualMachine.hpp"

void interpret()
{
}

void interpretMachineCode(ByteCode* code) 
{
    while(*code != INS_END) 
    {
        RootObject* obj = nullptr;
        RootObject* lhs = nullptr;
        RootObject* rhs = nullptr;
        RootObject* result = nullptr;
        // TODO : create a operators
        switch(*code) 
        {
            case INS_RET:
                break;
            case INS_PUSH:
                obj = reinterpret_cast<RootObject*>(++code);
                code += PTR_SIZE;
                push(obj);
                break;
            case INS_POP:
                pop();
                break;
            case INS_ADD:
                lhs = pop();
                rhs = pop();
                result = rootOperation(lhs,rhs,RT_OP_ADD);                
                push(result);
                break;
            case INS_SUB:
                lhs = pop();
                rhs = pop();
                result = rootOperation(lhs,rhs,RT_OP_SUB);                
                push(result);
                break;
            case INS_DIV:
                lhs = pop();
                rhs = pop();
                result = rootOperation(lhs,rhs,RT_OP_DIV);
                push(result);
                break;
            case INS_MUL:
                lhs = pop();
                rhs = pop();
                result = rootOperation(lhs,rhs,RT_OP_MUL);
                push(result);
                break;
            default:
                LOG_ERROR("error : unknown ins : %d",code);
                break;
        }
        code++;
    }

}
