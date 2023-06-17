#ifndef CMAKE_BUILD
#include "ExceptionHandler.h"

TryBlockMap::iterator::iterator(TryBlockMap &map, unsigned int block): tryBlockMap(map), currBlock(block)
{
}
TryBlockMap::iterator &TryBlockMap::iterator::operator++()
{
	this->tryBlockMap.DecompTryBlock();
	this->currBlock++;
	return *this;
}
TryBlockMapEntry TryBlockMap::iterator::operator*() const
{
	return this->tryBlockMap.tryBlock;
}
bool TryBlockMap::iterator::operator!=(const TryBlockMap::iterator &other) const
{
	return this->currBlock != other.currBlock;
}
bool TryBlockMap::iterator::operator<(const TryBlockMap::iterator &other) const
{
	return this->currBlock < other.currBlock;
}
TryBlockMap::iterator &TryBlockMap::iterator::operator=(const TryBlockMap::iterator &other)
{
	this->currBlock = other.currBlock;
	return *this;
}

#endif