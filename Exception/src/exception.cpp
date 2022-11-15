#include "execdef.h"

void backtrace(Exception::exception &);

Exception::exception::exception(BYTE type, WORD value): type(type), value(value)
{
	backtrace(*this);
}

Exception::exception::exception(): type(0), value(0)
{
	backtrace(*this);
}

Exception::exception::exception(const Exception::exception &copy)
{
	this->type = copy.type;
	this->value = copy.value;
	this->count = copy.count;
	this->stack = (Exception::exception::frame *)Memory::allocate(this->count * sizeof(Exception::exception::frame));
	for (DWORD i = 0; i < this->count; i++)
	{
		new (this->stack + i) Exception::exception::frame(copy.stack[i]);
	}
}

Exception::exception::exception(Exception::exception &&move):
	stack(move.stack), count(move.count), type(move.type), value(move.value)
{
	move.stack = 0;
	move.count = 0;
	move.type = 0;
	move.value = 0;
}

Exception::exception::~exception()
{
	Memory::free(this->stack);
	this->stack = 0;
	this->count = this->type = this->value = 0;
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
		this->type = move.type;
		this->value = move.value;
		move.stack = 0;
		move.count = 0;
		move.type = 0;
		move.value = 0;
	}
	return *this;
}