#include "ArenaAllocators.hpp"


ArenaAllocator::ArenaAllocator(u32 capacity)
    :offset{0u}
    ,size{capacity}
{
    arena = new u8[capacity];
}
        
ArenaAllocator::ArenaAllocator(ArenaAllocator&& rhs)
{
    arena = rhs.arena;
    offset = rhs.offset;
    size = rhs.size;
}

ArenaAllocator& ArenaAllocator::operator=(ArenaAllocator&& rhs)
{
    arena = rhs.arena;
    offset = rhs.offset;
    size = rhs.size;
}

ArenaAllocator::~ArenaAllocator()
{
    delete[] arena;
}

void* ArenaAllocator::allocate(u32 allocSize)
{
    void* retPtr = nullptr;
    if((offset + allocSize) < size)
    {
        retPtr = arena + offset;
        offset += allocSize;               
    }
    return retPtr;
}