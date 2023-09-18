#ifndef REENTRANTLOCK_H
#define REENTRANTLOCK_H

#include <WinType.h>
#include <concurrencyspec.h>

namespace Concurrency
{
	class ReentrantLock
	{
		private:
		QWORD exclusive = 0;
		public:
		CONCURRENCYAPI bool operator+();
		CONCURRENCYAPI void operator++(int);
		CONCURRENCYAPI void operator--(int);
		CONCURRENCYAPI bool operator!() const;
		CONCURRENCYAPI operator bool() const;
		CONCURRENCYAPI operator DWORD() const;
		CONCURRENCYAPI void lock();
		CONCURRENCYAPI void unlock();
		CONCURRENCYAPI bool free() const;
	};
}

#endif //REENTRANTLOCK_H
