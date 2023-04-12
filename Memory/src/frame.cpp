#include "definitions.h"

Memory::exception::frame::frame(void *returnAddress)
{
	this->offset = returnAddress;

	CHAR symbuf[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(CHAR)]{0};
	SYMBOL_INFO *syminfo = (SYMBOL_INFO *) symbuf;
	syminfo->SizeOfStruct = sizeof(SYMBOL_INFO);
	syminfo->MaxNameLen = MAX_SYM_NAME;
	QWORD disp = 0;
	SymFromAddr(GetCurrentProcess(), (QWORD) returnAddress, &disp, syminfo);

	this->function.resize(StringLength(syminfo->Name));
	Memory::copy(this->function.address, syminfo->Name, this->function.length);

	this->address = (void *) syminfo->Address;

	MEMORY_BASIC_INFORMATION memoryInfo = {0};
	VirtualQuery(returnAddress, &memoryInfo, sizeof(MEMORY_BASIC_INFORMATION));
	// this->address = (void *) memoryInfo.BaseAddress;
	this->module = (void *) memoryInfo.AllocationBase;

	char modname[MAX_SYM_NAME + 1]{0};
	// K32GetModuleBaseNameA(GetCurrentProcess(), (HMODULE) this->module, modname, MAX_SYM_NAME);
	DWORD len = GetModuleFileNameA((HMODULE) this->module, modname, MAX_SYM_NAME);
	DWORD idx = len;
	while (idx && modname[--idx] != '\\');
	idx++;
	this->library.resize(len - idx);
	Memory::copy(this->library.address, modname + idx, this->library.length);
}
Memory::exception::frame::frame(const Memory::exception::frame &copy) = default;
Memory::exception::frame::~frame() = default;