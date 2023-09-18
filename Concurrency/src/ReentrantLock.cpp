#include <concurrency.h>
#include <ReentrantLock.h>
#include <cthread.h>
#include <exception.h>

extern "C"
{
long _InterlockedCompareExchange(volatile long *, long, long);
}

bool Concurrency::ReentrantLock::operator+()
{
	DWORD current = WTM::thread::current().ID();
	_InterlockedCompareExchange(((volatile long *) &this->exclusive), (long) current, 0);
	if (((this->exclusive & 0xFFFFFFFF) != current))
	{
		return false;
	}
	this->exclusive += (1ULL << 32);
	return true;
}
void Concurrency::ReentrantLock::operator++(int)
{
	while (!+*this)
	{
		// A value of zero causes the thread to relinquish the remainder of its time slice to any other thread that is ready to run.
		// TODO ReentrantLock needs a list for wating threads, fair or no-fair
		WTM::thread::sleep(0);
	}
}
void Concurrency::ReentrantLock::operator--(int)
{
	DWORD current = WTM::thread::current().ID();
	if ((this->exclusive & 0xFFFFFFFF) != current)
	{
		throw Memory::exception(Concurrency::ERRNO_LOCK_BELONG, Memory::EXTERNAL);
	}

	this->exclusive -= (1ULL << 32);
	if ((this->exclusive >> 32) == 0)
	{
		this->exclusive = 0;
	}
}
bool Concurrency::ReentrantLock::operator!() const
{
	return this->exclusive == 0;
}
Concurrency::ReentrantLock::operator bool() const
{
	DWORD current = WTM::thread::current().ID();
	return (this->exclusive & 0xFFFFFFFF) == current;
}
Concurrency::ReentrantLock::operator DWORD() const
{
	if (*this)
	{
		return this->exclusive >> 32;
	}
	return 0;
}
void Concurrency::ReentrantLock::lock()
{
	(*this)++;
}
void Concurrency::ReentrantLock::unlock()
{
	(*this)--;
}
bool Concurrency::ReentrantLock::free() const
{
	return !(*this);
}