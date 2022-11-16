#include "execdef.h"

HANDLE process = GetCurrentProcess();

DWORD strlen(const void *str)
{
	const char *s = (const char *) str;
	while (*s++);
	return s - ((const char *)str) - 1;
}

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
		this->source.ensure(fnLen + 1);
		Memory::copy(this->source.address, imgLine.FileName, fnLen + 1);
		this->line = imgLine.LineNumber;
	}

	QWORD len = strlen(syminfo->Name);
	this->function.ensure(len + 1);
	Memory::copy(this->function.address, syminfo->Name, len + 1);

	this->address = (void *)syminfo->Address;
	this->module = (void *)syminfo->ModBase;

	char modname[MAX_SYM_NAME * sizeof(CHAR)]{0};
	K32GetModuleBaseNameA(process, (HMODULE) this->module, modname, MAX_SYM_NAME);
	len = strlen(modname);
	this->library.ensure(len + 1);
	Memory::copy(this->library.address, modname, len + 1);
}

Exception::exception::frame::frame(const Exception::exception::frame &copy)
{
	this->address = copy.address;
	this->offset = copy.offset;
	this->module = copy.module;
	this->function = copy.function;
	this->library = copy.library;
	this->source = copy.source;
	this->line = copy.line;
}

Exception::exception::frame::frame(Exception::exception::frame &&move):
	address(move.address),
	offset(move.offset),
	module(move.module),
	function((Memory::string &&)move.function),
	library((Memory::string &&)move.library),
	source((Memory::string &&)move.source),
	line(move.line)
{
	move.address = 0;
	move.offset = 0;
	move.module = 0;
	move.line = 0;
}

Exception::exception::frame::~frame()
{
	this->address = 0;
	this->offset = 0;
	this->module = 0;
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
		this->address = move.address;
		this->offset = move.offset;
		this->module = move.module;
		this->function = (Memory::string &&)move.function;
		this->library = (Memory::string &&)move.library;
		this->source = (Memory::string &&)move.source;
		this->line = move.line;
		move.address = 0;
		move.offset = 0;
		move.module = 0;
		move.line = 0;
	}
	return *this;
}