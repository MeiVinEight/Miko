#include "definitions.h"
#include "crt.h"

#include <memory.h>
#include <exception.h>


#define MAX_SYM_NAME 2000

extern "C"
{


typedef struct
{
	DWORD SizeOfStruct;
	DWORD TypeIndex;        // Type Index of symbol
	ULONG64     Reserved[2];
	DWORD Index;
	DWORD Size;
	ULONG64     ModBase;          // Base Address of module comtaining this symbol
	DWORD Flags;
	ULONG64     Value;            // Value of symbol, ValuePresent should be 1
	ULONG64     Address;          // Address of symbol including base address of module
	DWORD Register;         // register holding value or pointer to value
	DWORD Scope;            // scope of the symbol
	DWORD Tag;              // pdb classification
	DWORD NameLen;          // Actual length of name
	DWORD MaxNameLen;
	char Name[1];          // Name of symbol
} SYMBOL_INFO, *PSYMBOL_INFO;

BOOL __declspec(dllimport) __stdcall SymFromAddr(void *, QWORD, void *, void *);
DWORD __stdcall GetModuleFileNameA(void *, void *, DWORD);
void *__stdcall GetCurrentProcess(void);
BOOL __stdcall GetModuleHandleExA(DWORD, const void *, void **);
QWORD strlen(const char *);
#pragma intrinsic(strlen)


}

Memory::exception::frame::frame(void *returnAddress)
{
	this->offset = returnAddress;


	unsigned long long syminfoSize = sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(char);
	// CHAR symbuf[syminfoSize]{0};

	SYMBOL_INFO *syminfo = (SYMBOL_INFO *) Memory::allocate(syminfoSize);
	memset(syminfo, 0, syminfoSize);
	syminfo->SizeOfStruct = sizeof(SYMBOL_INFO);
	syminfo->MaxNameLen = MAX_SYM_NAME;
	QWORD disp = 0;
	SymFromAddr(GetCurrentProcess(), (QWORD) returnAddress, &disp, syminfo);

	this->function.resize(strlen(syminfo->Name));
	Memory::copy(this->function.address, syminfo->Name, this->function.length);

	this->address = (void *) syminfo->Address;

	Memory::free(syminfo);


	// this->address = (void *) memoryInfo.BaseAddress;
	GetModuleHandleExA(6, returnAddress, &this->module);

	char *modname = (char *) Memory::allocate(MAX_SYM_NAME + 1)/*[MAX_SYM_NAME + 1]{0}*/;
	memset(modname, 0, MAX_SYM_NAME + 1);
	// K32GetModuleBaseNameA(GetCurrentProcess(), (HMODULE) this->module, modname, MAX_SYM_NAME);
	DWORD len = GetModuleFileNameA((HMODULE) this->module, modname, MAX_SYM_NAME);
	DWORD idx = len;
	while (idx && modname[--idx] != '\\');
	idx++;
	this->library.resize(len - idx);
	Memory::copy(this->library.address, modname + idx, this->library.length);
	Memory::free(modname);
}
Memory::exception::frame::frame(const Memory::exception::frame &copy) = default;
Memory::exception::frame::~frame() = default;