#pragma once
#include "../types.hpp"
#include "rootObjects.hpp"
#include <variant>
#include <stack>
#include <map>

typedef struct RootVM
{
    std::stack<RootObject*> stack;
    std::map<std::string,RootObject*> globals;
    
    u8* ip;
}RootVM;

void initializeVM();
void destroyVM(); 
void push(RootObject* obj);
RootObject* pop();
void dumpVMState();

extern RootVM* VM;