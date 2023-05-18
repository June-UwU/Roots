#pragma once
#include "../types.hpp"
#include "memoryUnits.hpp"

class ArenaAllocator
{
    public:
        ArenaAllocator(u32 size = 256 KiB);
        ArenaAllocator(const ArenaAllocator& ) = delete;
        ArenaAllocator& operator=(const ArenaAllocator&) = delete;
        ArenaAllocator(ArenaAllocator&& rhs);
        ArenaAllocator& operator=(ArenaAllocator&& rhs);
        ~ArenaAllocator();
        void* allocate(u32 size);
    private:
        u8* arena;
        u32 offset;
        u32 size;
};

