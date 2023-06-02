#include "BulkAllocator.hpp"

BulkAllocator::BulkAllocator(u64 size)
    :allocatorSize{size}
{
    allocatorList.emplace_back(size);
}

BulkAllocator::BulkAllocator(BulkAllocator&& rhs)
{
    allocatorList = std::move(rhs.allocatorList);
    allocatorSize = std::move(rhs.allocatorSize);
}

BulkAllocator& BulkAllocator::operator=(BulkAllocator&& rhs)
{
    allocatorList = std::move(rhs.allocatorList);
    allocatorSize = std::move(rhs.allocatorSize);
    return *this;
}

void* BulkAllocator:: allocate(u64 size)
{
    void* retPtr = allocatorList[allocatorList.size() - 1].allocate(size);
    if(nullptr == retPtr)
    {
        allocatorList.emplace_back(allocatorSize);
        retPtr = allocatorList[allocatorList.size() - 1].allocate(size);        
    }
    return retPtr;
}