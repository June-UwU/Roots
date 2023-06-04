#include "virtualMachine.hpp"

void runByteCode(RootVM* byteCode);

RootVM::RootVM()
    :byteCode{nullptr}
{}

void RootVM::loadCode(u8* code)
{
    byteCode = code;
}

void RootVM::execute()
{
    runByteCode(this);
}

void RootVM::push(Value value)
{
    stack.push(value);
}

Value RootVM::pop()
{
    Value ret = stack.top();
    stack.pop();
    return ret;
}

void runByteCode(RootVM* byteCode)
{
    // TODO : do our logic here
}
