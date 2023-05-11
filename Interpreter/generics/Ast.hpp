#pragma once
#include "../types.hpp"

class AstNode
{
    public:
        AstNode() = delete;
        AstNode(void* pData);
        AstNode(const AstNode& rhs);
        AstNode& operator=(const AstNode& rhs);
        AstNode(AstNode&& rhs);
        AstNode& operator=(AstNode&& rhs);
        ~AstNode();
        void* get();
        void reset();
    private:
        void* data = nullptr;
        u32* ref = nullptr;
};
