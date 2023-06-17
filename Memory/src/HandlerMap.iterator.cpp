#ifndef CMAKE_BUILD
#include "ExceptionHandler.h"

HandlerMap::iterator::iterator(HandlerMap &map, unsigned int block): handlerMap(map), currBlock(block)
{
}
HandlerMap::iterator &HandlerMap::iterator::operator++()
{
	this->handlerMap.DecompHandler();
	++this->currBlock;
	return *this;
}
HandlerType HandlerMap::iterator::operator*() const
{
	return this->handlerMap.handler;
}
bool HandlerMap::iterator::operator==(const HandlerMap::iterator &other) const
{
	return this->currBlock == other.currBlock;
}
bool HandlerMap::iterator::operator!=(const HandlerMap::iterator &other) const
{
	return !(*this == other);
}
#endif