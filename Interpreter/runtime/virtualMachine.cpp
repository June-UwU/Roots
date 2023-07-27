#include "virtualMachine.hpp"
#include "instructions.hpp"
#include "..\logger.hpp"

RootVM* VM = nullptr;
u8 checkOperFlag(u8 code);


void initializeVM()
{
    VM = new RootVM();
}

void destroyVM()
{
    delete VM;
}

void push(RootObject* value)
{
    VM->stack.push(value);
}

RootObject* pop()
{
    if(VM->stack.empty()) 
    {
        LOG_ERROR("POP called on empty VM stack");
        return nullptr;
    }
    RootObject* ret = VM->stack.top();
    VM->stack.pop();
    return ret;
}

void dumpVMState() 
{
    LOG_INFO("\n\n\nRegisters State \n IP : %ld",VM->ip);    
    //trap for debug purpose
    __builtin_trap();
}