#pragma once
#include "../types.hpp"
#include "memoryUnits.hpp"
#include "ArenaAllocators.hpp"
#include <vector>


class BulkAllocator
{
    public:
        BulkAllocator(u64 allocatorSize);
        BulkAllocator(const BulkAllocator& ) = delete;
        BulkAllocator& operator=(const BulkAllocator&) = delete;
        BulkAllocator(BulkAllocator&& rhs);
        BulkAllocator& operator=(BulkAllocator&& rhs);
        void* allocate(u64 size);
    private:
        u64 allocatorSize;
        std::vector<ArenaAllocator> allocatorList;
};