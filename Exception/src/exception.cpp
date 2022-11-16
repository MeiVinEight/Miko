#include "execdef.h"

Exception::exception::exception(const void *message): message(strlen(message) + 1)
{
	Memory::copy(this->message.address, message, this->message.length);
	backtrace(*this);
}

Exception::exception::exception(const Memory::string &str): message(str)
{
	backtrace(*this);
}

Exception::exception::exception(const Exception::exception &copy)
{
	this->count = copy.count;
	this->stack = (Exception::exception::frame *)Memory::allocate(this->count * sizeof(Exception::exception::frame));
	for (DWORD i = 0; i < this->count; i++)
	{
		new (this->stack + i) Exception::exception::frame(copy.stack[i]);
	}
}

Exception::exception::exception(Exception::exception &&move):
	stack(move.stack), count(move.count)
{
	move.stack = 0;
	move.count = 0;
}

Exception::exception::~exception()
{
	Memory::free(this->stack);
	this->stack = 0;
	this->count = 0;
}

Exception::exception &Exception::exception::operator=(const Exception::exception &copy)
{
	if (&copy != this)
	{
		(*this) = Exception::exception(copy);
	}
	return *this;
}

Exception::exception &Exception::exception::operator=(Exception::exception &&move)
{
	if (&move != this)
	{
		Memory::free(this->stack);
		this->stack = move.stack;
		this->count = move.count;
		move.stack = 0;
		move.count = 0;
	}
	return *this;
}