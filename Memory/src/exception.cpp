#include "definitions.h"

Memory::exception::exception(DWORD code, BYTE type): code(code), type(type)
{
	this->message = Memory::message(code, type);
	backtrace(*this);
}
Memory::exception::exception(DWORD code): code(code), type(Memory::EXTERNAL)
{
	this->message = Memory::message(code, this->type);
	backtrace(*this);
}
Memory::exception::~exception()
{
	for (DWORD i = 0; i < this->count; this->stack[i++].Memory::exception::frame::~frame());
	Memory::free(this->stack);
}