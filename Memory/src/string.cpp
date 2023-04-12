#include "definitions.h"

Memory::string::string(): string(0)
{
}
Memory::string::string(QWORD size): address((BYTE *) Memory::allocate(size)), length(size)
{
}
Memory::string::string(const Memory::string &copy): string(copy.length)
{
	Memory::copy(this->address, copy.address, this->length);
}
Memory::string::string(Memory::string &&move) noexcept: address(move.address), length(move.length)
{
	move.address = nullptr;
	move.length = 0;
}
Memory::string::~string()
{
	Memory::free(this->address);
	this->address = nullptr;
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
Memory::string &Memory::string::operator=(Memory::string &&move) noexcept
{
	if (&move != this)
	{
		Memory::free(this->address);
		this->address = move.address;
		this->length = move.length;
		move.address = nullptr;
		move.length = 0;
	}
	return *this;
}
BYTE &Memory::string::operator[](QWORD off) const
{
	if (off < this->length)
	{
		return this->address[off];
	}
	throw Memory::exception(Memory::ERRNO_ACCESS_VIOLATION);
}
void Memory::string::resize(QWORD size)
{
	if (this->length != size)
	{
		this->address = (BYTE *) Memory::reallocate(this->address, size);
		this->length = size;
	}
}