#ifndef CMAKE_BUILD
#include "ExceptionHandler.h"

TryBlockMap::TryBlockMap(const FuncInfo *pFuncInfo, unsigned long long imageBase)
{
	if (pFuncInfo->dispTryBlockMap)
	{
		this->buffer = imageRelToByteBuffer(imageBase, pFuncInfo->dispTryBlockMap);
		this->numTryBlocks = ReadUnsigned(&this->buffer);
		this->bufferStart = this->buffer;
		this->DecompTryBlock();
	}
}
TryBlockMap::iterator TryBlockMap::begin()
{
	return {*this, 0};
}
TryBlockMap::iterator TryBlockMap::end()
{
	return {*this, this->numTryBlocks};
}
unsigned int TryBlockMap::getNumTryBlocks() const
{
	return this->numTryBlocks;
}
void TryBlockMap::DecompTryBlock()
{
	this->tryBlock.tryLow = ReadUnsigned(&this->buffer);
	this->tryBlock.tryHigh = ReadUnsigned(&this->buffer);
	this->tryBlock.catchHigh = ReadUnsigned(&this->buffer);
	this->tryBlock.dispHandlerArray = ReadInt(&this->buffer);
}
#endif