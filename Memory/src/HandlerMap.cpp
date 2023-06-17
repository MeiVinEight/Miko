#ifndef CMAKE_BUILD
#include "ExceptionHandler.h"

HandlerMap::HandlerMap(const TryBlockMapEntry *tryMap, unsigned long long imageBase, unsigned int functionStart):
imageBase(imageBase), functionStart(functionStart)
{
	if (tryMap->dispHandlerArray)
	{
		this->buffer = imageRelToByteBuffer(this->imageBase, tryMap->dispHandlerArray);
		this->numHandlers = ReadUnsigned(&this->buffer);
		this->bufferStart = this->buffer;
		this->DecompHandler();
	}
}
HandlerMap::iterator HandlerMap::begin()
{
	return {*this, 0};
}
HandlerMap::iterator HandlerMap::end()
{
	return {*this, this->numHandlers};
}
void HandlerMap::setBuffer(unsigned int index)
{
	this->buffer = this->bufferStart;
	this->DecompHandler();
	while (index--)
	{
		this->DecompHandler();
	}
}
void HandlerMap::resetBuffer()
{
	this->setBuffer(0);
}
void HandlerMap::DecompHandler()
{
	this->handler.reset();
	this->handler.header.value = this->buffer[0];
	++this->buffer;
	if (this->handler.header.adjectives)
	{
		this->handler.adjectives = ReadUnsigned(&this->buffer);
	}
	if (this->handler.header.dispType)
	{
		this->handler.dispType = ReadInt(&this->buffer);
	}
	if (this->handler.header.dispCatchObj)
	{
		this->handler.dispCatchObj = ReadUnsigned(&this->buffer);
	}
	this->handler.dispOfHandler = ReadInt(&this->buffer);
	if (this->handler.header.contIsRVA)
	{
		if (this->handler.header.contAddr == HandlerTypeHeader::contType::ONE)
		{
			this->handler.continuationAddress[0] = this->functionStart + ReadInt(&this->buffer);
		}
		else if (this->handler.header.contAddr == HandlerTypeHeader::contType::TWO)
		{
			this->handler.continuationAddress[0] = this->functionStart + ReadInt(&this->buffer);
			this->handler.continuationAddress[1] = this->functionStart + ReadInt(&this->buffer);
		}
	}
	else
	{
		if (this->handler.header.contAddr == HandlerTypeHeader::contType::ONE)
		{
			this->handler.continuationAddress[0] = this->functionStart + ReadUnsigned(&this->buffer);
		}
		else if (this->handler.header.contAddr == HandlerTypeHeader::contType::TWO)
		{
			this->handler.continuationAddress[0] = this->functionStart + ReadUnsigned(&this->buffer);
			this->handler.continuationAddress[1] = this->functionStart + ReadUnsigned(&this->buffer);
		}
	}
}
#endif