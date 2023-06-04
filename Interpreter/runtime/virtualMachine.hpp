#pragma once
#include "../types.hpp"
#include <variant>
#include <stack>

using Data = std::variant<s8,s16,s32,s64,u8,u16,u32,u64,std::string>; 

typedef enum ValueType
{
    INT8_T = 0x0,
    INT16_T,
    INT32_T,
    INT64_T,
    UINT8_T,
    UINT16_T,
    UINT32_T,
    UINT64_T,
    FLOAT32_T,
    FLOAT64_T,
    STRING
}ValueType;

typedef struct Value
{
    ValueType type;
    Data data;
}Value;

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