#define EXCEPTION_SHARED
#include <exception.h>
#include <memory.h>
#pragma comment(lib, "DbgHelp.lib")

#ifdef __cplusplus
extern "C"
{
#endif

#define DLL_PROCESS_ATTACH 1
#define DLL_PROCESS_DETACH 0
#define SYMOPT_LOAD_LINES 0x10
#define MAX_SYM_NAME 2000

typedef const char *PCSTR;
typedef void *HINSTANCE;
typedef int BOOL;
typedef char CHAR;
typedef unsigned long ULONG;
typedef unsigned __int64 ULONG64;
typedef unsigned __int64 DWORD64, *PDWORD64;
typedef void *HANDLE;
typedef CHAR *LPSTR;
typedef void *HMODULE;
typedef void *PVOID;
typedef DWORD *PDWORD;
typedef CHAR *PCHAR;
typedef struct
{
	ULONG       SizeOfStruct;
	ULONG       TypeIndex;        // Type Index of symbol
	ULONG64     Reserved[2];
	ULONG       Index;
	ULONG       Size;
	ULONG64     ModBase;          // Base Address of module comtaining this symbol
	ULONG       Flags;
	ULONG64     Value;            // Value of symbol, ValuePresent should be 1
	ULONG64     Address;          // Address of symbol including base address of module
	ULONG       Register;         // register holding value or pointer to value
	ULONG       Scope;            // scope of the symbol
	ULONG       Tag;              // pdb classification
	ULONG       NameLen;          // Actual length of name
	ULONG       MaxNameLen;
	CHAR        Name[1];          // Name of symbol
} SYMBOL_INFO, *PSYMBOL_INFO;
typedef struct
{
	DWORD    SizeOfStruct;           // set to sizeof(IMAGEHLP_LINE64)
	PVOID    Key;                    // internal
	DWORD    LineNumber;             // line number in file
	PCHAR    FileName;               // full filename
	DWORD64  Address;                // first instruction of line
} IMAGEHLP_LINE64, *PIMAGEHLP_LINE64;


HANDLE __stdcall GetCurrentProcess(void);
DWORD __declspec(dllimport) __stdcall SymSetOptions(DWORD);
BOOL __declspec(dllimport) __stdcall SymInitialize(HANDLE, PCSTR, BOOL);
BOOL __declspec(dllimport) __stdcall SymCleanup(HANDLE);
BOOL __declspec(dllimport) __stdcall SymFromAddr(HANDLE, DWORD64, PDWORD64, PSYMBOL_INFO);
DWORD K32GetModuleBaseNameA(HANDLE, HMODULE, LPSTR, DWORD);
__declspec(dllimport) WORD __stdcall RtlCaptureStackBackTrace(DWORD, DWORD, PVOID *, PDWORD);
BOOL __declspec(dllimport) __stdcall SymGetLineFromAddr64(HANDLE, DWORD64, PDWORD, PIMAGEHLP_LINE64);
DWORD __stdcall GetLastError(void);
inline void* __cdecl operator new(size_t size, void* where) noexcept
{
	(void) size;
	return where;
}


#ifdef __cplusplus
}
#endif

HANDLE process = GetCurrentProcess();

int __stdcall DllMain(HINSTANCE *instance, unsigned int reason, void *reserved)
{
	(void)&DllMain;
	(void)instance;
	(void)reserved;

	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			SymSetOptions(SYMOPT_LOAD_LINES);
			return SymInitialize(process, 0, 1);
		}
		case DLL_PROCESS_DETACH:
		{
			return SymCleanup(process);
		}
	}
	return 1;
}

QWORD strlen(char *str)
{
	QWORD len = 0;
	for (char *s = str; *s++; len++);
	return len;
}

void backtrace(Exception::exception &exec)
{
	DWORD max = 64 * sizeof(void *);
	DWORD count = 0;
	void **arr = (void **)Memory::allocate(max);
	void *retAddr;
	while (RtlCaptureStackBackTrace(count + 2, 1, &retAddr, NULL))
	{
		if (count >= max)
		{
			max *= 2;
			arr = (void **)Memory::reallocate(arr, max);
		}
		arr[count++] = retAddr;
	}

	exec.stack = (Exception::exception::frame *)Memory::allocate(count * sizeof(Exception::exception::frame));
	for (DWORD i = 0; i < count; new(exec.stack + i) Exception::exception::frame(arr[i]), i++);
	exec.count = count;

	Memory::free(arr);
}

Exception::exception::frame::frame(void *returnAddress)
{
	this->offset = returnAddress;

	CHAR symbuf[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(CHAR)]{0};
	SYMBOL_INFO *syminfo = (SYMBOL_INFO *)symbuf;
	syminfo->SizeOfStruct = sizeof(SYMBOL_INFO);
	syminfo->MaxNameLen = MAX_SYM_NAME;
	QWORD disp = 0;
	SymFromAddr(process, (QWORD) returnAddress, &disp, syminfo);

	DWORD dispLine = 0;
	IMAGEHLP_LINE64 imgLine{0};
	imgLine.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
	SymGetLineFromAddr64(process, (QWORD) returnAddress, &dispLine, &imgLine);
	if (imgLine.LineNumber)
	{
		DWORD fnLen = strlen(imgLine.FileName);
		this->source = (char *) Memory::allocate(fnLen + 1);
		Memory::copy(this->source, imgLine.FileName, fnLen + 1);
		this->line = imgLine.LineNumber;
	}

	QWORD len = strlen(syminfo->Name);
	this->function = (char *) Memory::allocate(len + 1);
	Memory::copy(this->function, syminfo->Name, len + 1);

	this->address = (void *)syminfo->Address;
	this->module = (void *)syminfo->ModBase;

	char modname[MAX_SYM_NAME * sizeof(CHAR)]{0};
	K32GetModuleBaseNameA(process, (HMODULE) this->module, modname, MAX_SYM_NAME);
	len = strlen(modname);
	this->library = (char *) Memory::allocate(len + 1);
	Memory::copy(this->library, modname, len + 1);
}

Exception::exception::frame::frame(Exception::exception::frame const &copy)
{
	this->address = copy.address;
	this->offset = copy.offset;
	this->module = copy.module;
	DWORD len = strlen(copy.function);
	this->function = (char *) Memory::allocate(len + 1);
	Memory::copy(this->function, copy.function, len + 1);

	len = strlen(copy.library);
	this->library = (char *) Memory::allocate(len + 1);
	Memory::copy(this->library, copy.library, len + 1);

	len = strlen(copy.source);
	this->source = (char *) Memory::allocate(len + 1);
	Memory::copy(this->source, copy.source, len + 1);

	this->line = copy.line;
}

Exception::exception::frame::frame(Exception::exception::frame &&move):
	address(move.address),
	offset(move.offset),
	module(move.module),
	function(move.function),
	library(move.library),
	source(move.source),
	line(move.line)
{
	move.address = 0;
	move.offset = 0;
	move.module = 0;
	move.function = 0;
	move.library = 0;
	move.source = 0;
	move.line = 0;
}

Exception::exception::frame::~frame()
{
	Memory::free(this->function);
	Memory::free(this->library);
	Memory::free(this->source);
	this->address = 0;
	this->offset = 0;
	this->module = 0;
	this->function = 0;
	this->library = 0;
	this->source = 0;
	this->line = 0;
}

Exception::exception::frame &Exception::exception::frame::operator=(Exception::exception::frame const &copy)
{
	if (&copy != this)
	{
		(*this) = Exception::exception::frame(copy);
	}
	return *this;
}

Exception::exception::frame& Exception::exception::frame::operator=(Exception::exception::frame &&move)
{
	if (&move != this)
	{
		Memory::free(this->function);
		Memory::free(this->library);
		Memory::free(this->source);
		this->address = move.address;
		this->offset = move.offset;
		this->module = move.module;
		this->function = move.function;
		this->library = move.library;
		this->source = move.source;
		this->line = move.line;
		move.address = 0;
		move.offset = 0;
		move.module = 0;
		move.function = 0;
		move.library = 0;
		move.source = 0;
		move.line = 0;
	}
	return *this;
}

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