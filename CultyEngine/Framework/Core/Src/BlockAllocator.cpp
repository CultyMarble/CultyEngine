#include "Precompiled.h"
#include "BlockAllocator.h"

#include "DebugUtils.h"

using namespace CultyEngine;
using namespace CultyEngine::Core;

BlockAllocator::BlockAllocator(const char* name, size_t blockSize, size_t capacity)
	: mName(name), mBlockSize(blockSize), mCapacity(capacity)
{
	ASSERT(blockSize > 0, "BlockAllocator: %s invalid block size", mName.c_str());
	ASSERT(capacity > 0, "BlockAllocator: %s invalid capacity", mName.c_str());

	mData = std::malloc(blockSize * capacity);

	for (size_t i = 0; i < capacity; ++i)
	{
		mFreeBlocks[i] = static_cast<uint8_t*>(mData) + (i * blockSize);
	}

	LOG("%s allocated %zu block, blocks size: %zu", mName.c_str(), capacity, blockSize);
}

BlockAllocator::~BlockAllocator()
{
	ASSERT(mBlockAllocatedTotal == mBlockFreed, "BlockAllocator: %s not all blocks are freed", mName.c_str());
	std::free(mData);
	mData = nullptr;

	LOG("%s destructed, Allocated: %zu, Freed: %zu, Highest: %zu",
		mName.c_str(), mBlockAllocatedCurrent, mBlockFreed, mBlockHighest);
}

void* BlockAllocator::Allocate()
{
	if (mFreeBlocks.empty())
	{
		LOG("%s no free blocks available", mName.c_str());
		return nullptr;
	}

	void* freeBlock = mFreeBlocks.back();
	mFreeBlocks.pop_back();

	++mBlockAllocatedTotal;
	++mBlockAllocatedCurrent;
	mBlockHighest = std::max(mBlockHighest, mBlockAllocatedCurrent);

	LOG("%s allocated blocks at %p, Allocated: %zu, Highest: %zu",
		mName.c_str(), freeBlock, mBlockAllocatedCurrent, mBlockHighest);

	return freeBlock;
}

void BlockAllocator::Free(void* ptr)
{
	if (ptr == nullptr)
		return;

	const uint8_t* start = static_cast<uint8_t*>(mData);
	const uint8_t* end = static_cast<uint8_t*>(mData) + (mBlockSize * mCapacity);
	const uint8_t* current = static_cast<uint8_t*>(ptr);
	const auto diff = current - start;
	ASSERT(current >= start && current < end && static_cast<size_t>(diff) % mBlockSize == 0,
		"BlockAllocator: %s invalid address being freed!", mName.c_str());

	LOG("%s free %p", mName.c_str(), ptr);
	--mBlockAllocatedCurrent;
	++mBlockFreed;
	mFreeBlocks.emplace_back(ptr);
}