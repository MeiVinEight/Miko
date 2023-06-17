#ifndef CMAKE_BUILD
#include "ExceptionHandler.h"

TryBlockMap::IRange::IRange(TryBlockMap::iterator f, TryBlockMap::iterator s): first((TryBlockMap::iterator &&) f), second((TryBlockMap::iterator &&) s)
{
}
#endif