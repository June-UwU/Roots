#pragma once
#include "../types.hpp"
#include <variant>
#include <stack>

using Value = f64;

class RootVM
{
    public:
        RootVM();
        RootVM(const RootVM&) = delete;
        RootVM& operator=(const RootVM&) = delete;
        RootVM(RootVM&&) = delete;
        RootVM& operator=(RootVM&&) = delete;
        void loadCode(u8* code);
        void execute();
        void push(Value value);
        Value pop();
    private:
        std::stack<Value> stack;
        u8* byteCode;
};

extern RootVM* VM;
void initializeVM();
void destroyVM(); 