#include <exception.h>
#include <cprocess.h>

#include "definitions.h"

#define DELETE                           (0x00010000L)
#define READ_CONTROL                     (0x00020000L)
#define WRITE_DAC                        (0x00040000L)
#define WRITE_OWNER                      (0x00080000L)
#define SYNCHRONIZE                      (0x00100000L)
#define STANDARD_RIGHTS_REQUIRED         (0x000F0000L)

#define TOKEN_ASSIGN_PRIMARY    (0x0001)
#define TOKEN_DUPLICATE         (0x0002)
#define TOKEN_IMPERSONATE       (0x0004)
#define TOKEN_QUERY             (0x0008)
#define TOKEN_QUERY_SOURCE      (0x0010)
#define TOKEN_ADJUST_PRIVILEGES (0x0020)
#define TOKEN_ADJUST_GROUPS     (0x0040)
#define TOKEN_ADJUST_DEFAULT    (0x0080)
#define TOKEN_ADJUST_SESSIONID  (0x0100)

#define PROCESS_ALL_ACCESS        (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0xFFFF)

#define TOKEN_ALL_ACCESS_P (STANDARD_RIGHTS_REQUIRED |\
                            TOKEN_ASSIGN_PRIMARY     |\
                            TOKEN_DUPLICATE          |\
                            TOKEN_IMPERSONATE        |\
                            TOKEN_QUERY              |\
                            TOKEN_QUERY_SOURCE       |\
                            TOKEN_ADJUST_PRIVILEGES  |\
                            TOKEN_ADJUST_GROUPS      |\
                            TOKEN_ADJUST_DEFAULT)
#define TOKEN_ALL_ACCESS (TOKEN_ALL_ACCESS_P | TOKEN_ADJUST_SESSIONID)

#pragma region STRUCT

typedef struct
{
	DWORD LowPart;
	long HighPart;
} LUID;
typedef struct
{
	LUID Luid;
	DWORD Attributes;
} LUID_AND_ATTRIBUTES;
typedef struct
{
	DWORD PrivilegeCount;
	LUID_AND_ATTRIBUTES Privileges[1];
} TOKEN_PRIVILEGES;
typedef struct
{
	long ExitStatus;
	void *PebBaseAddress;
	QWORD AffinityMask;
	long BasePriority;
	QWORD UniqueProcessId;
	QWORD InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION;

#pragma endregion

WTM::process::process(void *handle): object(handle)
{
}
WTM::process::process(const WTM::process &copy)
{
	if (!DuplicateHandle(GetCurrentProcess(), copy.object, GetCurrentProcess(), &this->object, 0, 1, DUPLICATE_SAME_ACCESS))
	{
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	}
}
WTM::process::process(WTM::process &&move) noexcept: object(move.object)
{
	move.object = nullptr;
}
WTM::process::~process()
{
	if (this->object)
	{
		CloseHandle(this->object);
	}
}
WTM::process &WTM::process::operator=(const WTM::process &copy)
{
	if (this != &copy)
	{
		(*this) = WTM::process(copy);
	}
	return *this;
}
WTM::process &WTM::process::operator=(WTM::process &&move) noexcept
{
	if (this != &move)
	{
		this->~process();
		this->object = move.object;
		move.object = nullptr;
	}
	return *this;
}
void WTM::process::suspend() const
{
	long result = ZwSuspendProcess(this->object);
	if (result < 0)
		throw Memory::exception(result, Memory::NTSTATUS);
}
void WTM::process::resume() const
{
	long result = ZwResumeProcess(this->object);
	if (result < 0)
		throw Memory::exception(result, Memory::NTSTATUS);
}
DWORD WTM::process::ID() const
{
	return GetProcessId(this->object);
}
bool WTM::process::alive() const
{
	DWORD ret = WaitForSingleObject(this->object, 0);
	if (ret == WAIT_FAILED)
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	return ret == WAIT_TIMEOUT;
}
bool WTM::process::wait(DWORD time) const
{
	DWORD ret = WaitForSingleObject(this->object, time);
	if (ret == WAIT_FAILED)
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	return ret == WAIT_OBJECT_0;
}
void WTM::process::wait() const
{
	this->wait(-1);
}
void WTM::process::privilege(const char *name) const
{
	void *token = nullptr;
	if (!OpenProcessToken(this->object, TOKEN_ALL_ACCESS, &token))
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	LUID_AND_ATTRIBUTES privilege = {};
	privilege.Attributes = 2;
	if (!LookupPrivilegeValueA(nullptr, name, &privilege.Luid))
	{
		CloseHandle(token);
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	}
	TOKEN_PRIVILEGES privileges = {};
	privileges.PrivilegeCount = 1;
	privileges.Privileges[0] = privilege;
	if (!AdjustTokenPrivileges(token, false, &privileges, sizeof(privileges), nullptr, nullptr))
	{
		CloseHandle(token);
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	}
	CloseHandle(token);
}
WTM::process WTM::process::parent() const
{
	WTM::process::current().privilege("SeDebugPrivilege");
	PROCESS_BASIC_INFORMATION information = {};
	long status = NtQueryInformationProcess(this->object, 0, &information, sizeof(information), nullptr);
	if (status)
		throw Memory::exception(status, Memory::NTSTATUS);
	return WTM::process::open((DWORD) information.InheritedFromUniqueProcessId);
}
Memory::string WTM::process::name() const
{
	char modname[2001] = {};
	DWORD len = 2000;
	if (!QueryFullProcessImageNameA(this->object, 0, modname, &len))
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	Memory::string ret(len);
	Memory::copy(ret.address, modname, len);
	return ret;
}
void WTM::process::terminate(DWORD code) const
{
	if (!TerminateProcess(this->object, code))
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
}
WTM::process WTM::process::current()
{
	WTM::process pro(GetCurrentProcess());
	WTM::process ret(pro);
	pro.object = nullptr;
	return ret;
}
WTM::process WTM::process::open(DWORD pid)
{
	void *proc = OpenProcess(PROCESS_ALL_ACCESS, true, pid);
	/*
	DWORD ret = WaitForSingleObject(proc, 0);
	if (ret != WAIT_TIMEOUT)
		proc = nullptr;
	*/
	return WTM::process(proc);
}