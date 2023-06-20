#include <exception.h>
#include <cthread.h>

#include "definitions.h"

extern "C" QWORD __cdecl wcslen(const wchar_t *);
#pragma intrinsic(wcslen)

WTM::thread::thread(void *obj): object(obj)
{
}
WTM::thread::thread(const WTM::thread &copy)
{
	if (!DuplicateHandle(GetCurrentProcess(), copy.object, GetCurrentProcess(), &this->object, 0, 1, DUPLICATE_SAME_ACCESS))
	{
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	}
}
WTM::thread::thread(WTM::thread &&move) noexcept: object(move.object)
{
	move.object = nullptr;
}
WTM::thread::~thread() noexcept
{
	if (this->object)
	{
		CloseHandle(this->object);
	}
}
WTM::thread &WTM::thread::operator=(const WTM::thread &copy)
{
	if (this != &copy)
	{
		(*this) = WTM::thread(copy);
	}
	return *this;
}
WTM::thread &WTM::thread::operator=(WTM::thread &&move) noexcept
{
	if (this != &move)
	{
		this->~thread();
		this->object = move.object;
		move.object = nullptr;
	}
	return *this;
}
DWORD WTM::thread::ID() const
{
	DWORD id = GetThreadId(this->object);
	if (!id)
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	return id;
}
Memory::string WTM::thread::name() const
{
	wchar_t *desc = nullptr;
	long result = GetThreadDescription(this->object, &desc);
	if (result < 0)
		throw Memory::exception(result, Memory::NTSTATUS);

	QWORD lengthW = wcslen(desc);
	QWORD nBytes = WideCharToMultiByte(65001, 0, desc, (int) lengthW, nullptr, 0, nullptr, nullptr) & ((1ULL << 32) - 1);
	if (!nBytes)
		throw Memory::exception(Memory::error(), Memory::DOSERROR);

	Memory::string ret(nBytes);
	if (!WideCharToMultiByte(65001, 0, desc, (int) lengthW, (char *) ret.address, (int) ret.length, nullptr, nullptr))
		throw Memory::exception(Memory::error(), Memory::DOSERROR);

	if (LocalFree(desc))
		throw Memory::exception(Memory::error(), Memory::DOSERROR);

	return ret;
}
void WTM::thread::name(const Memory::string &str) const
{
	QWORD nBytes = MultiByteToWideChar(65001, 0, (const char *) str.address, (int) str.length, nullptr, 0);
	wchar_t *wstr = new wchar_t[nBytes + 1];
	wstr[nBytes] = 0;
	if (!MultiByteToWideChar(65001, 0, (const char *) str.address, (int) str.length, wstr, (int) nBytes))
		throw Memory::exception(Memory::error(), Memory::DOSERROR);

	long result = SetThreadDescription(this->object, wstr);
	if (result < 0)
		throw Memory::exception(result, Memory::NTSTATUS);

	delete[] wstr;
}
bool WTM::thread::alive() const
{
	DWORD ret = WaitForSingleObject(this->object, 0);
	if (ret == WAIT_FAILED)
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	return ret == WAIT_TIMEOUT;
}
bool WTM::thread::wait(DWORD time) const
{
	DWORD ret = WaitForSingleObject(this->object, time);
	if (ret == WAIT_FAILED)
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	return ret == WAIT_OBJECT_0;
}
void WTM::thread::wait() const
{
	this->wait(-1);
}
DWORD WTM::thread::suspend() const
{
	DWORD cnt = SuspendThread(this->object);
	if (cnt == (DWORD) -1)
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	return cnt + 1;
}
DWORD WTM::thread::resume() const
{
	DWORD cnt = ResumeThread(this->object);
	if (cnt == (DWORD) -1)
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	return cnt - !!cnt;
}
DWORD WTM::thread::code() const
{
	DWORD ret = 0;
	if (!GetExitCodeThread(this->object, &ret))
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	return ret;
}
WTM::thread WTM::thread::create(DWORD (*routine)(void *), void *param)
{
	DWORD TID;
	void *handle = CreateThread(nullptr, 0, routine, param, 0, &TID);
	if (!handle)
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	return WTM::thread(handle);
}
WTM::thread WTM::thread::current()
{
	WTM::thread cur(GetCurrentThread());
	WTM::thread ret(cur);
	cur.object = nullptr;
	return ret;
}
void WTM::thread::sleep(DWORD time)
{
	Sleep(time);
}