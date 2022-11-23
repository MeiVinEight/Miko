#include "definitions.h"

Memory::string::string(QWORD size): address((char *)Memory::allocate(size)), length(size)
{
}

Memory::string::string(const Memory::string &copy): address((char *)Memory::allocate(copy.length)), length(copy.length)
{
	Memory::copy(this->address, copy, this->length);
}

Memory::string::string(Memory::string &&move): address(move), length(move.length)
{
	move.address = 0;
	move.length = 0;
}

Memory::string::~string()
{
	Memory::free(this->address);
	this->address = NULL;
	this->length = 0;
}

Memory::string &Memory::string::operator=(const Memory::string &copy)
{
	if (&copy != this)
	{
		(*this) = Memory::string(copy);
	}
	return *this;
}

Memory::string &Memory::string::operator=(Memory::string &&move)
{
	if (&move != this)
	{
		Memory::free(this->address);
		this->address = move;
		this->length = move.length;
		move.address = 0;
		move.length = 0;
	}
	return *this;
}

char &Memory::string::operator[](QWORD off) const
{
	if (off < this->length)
	{
		return this->address[off];
	}
	return *((char *)1);
}

Memory::string::operator char *() const
{
	return this->address;
}

void Memory::string::resize(QWORD size)
{
	if (this->length != size)
	{
		this->address = (char *)Memory::reallocate(this->address, size);
		this->length = size;
	}
}