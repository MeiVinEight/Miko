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
Memory::exception::exception(const Memory::exception &exce) noexcept: code(exce.code), type(exce.type), message(exce.message)
{
	this->count = exce.count;
	this->stack = (Memory::exception::frame *) Memory::allocate(this->count * sizeof(Memory::exception::frame));
	for (DWORD i = 0; i < this->count; i++)
	{
		new (this->stack + i) Memory::exception::frame(exce.stack[i]);
	}
}
Memory::exception::~exception()
{
	for (DWORD i = 0; i < this->count; this->stack[i++].Memory::exception::frame::~frame());
	Memory::free(this->stack);
}