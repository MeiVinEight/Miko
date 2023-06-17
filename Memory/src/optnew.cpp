#include <optnew.h>


#ifndef CMAKE_BUILD


#include <memory.h>


#pragma comment(linker, "/EXPORT:??2@YAPEAX_K@Z")   // void *__cdecl operator new(unsigned long long);
#pragma comment(linker, "/EXPORT:??_U@YAPEAX_K@Z")  // void *__cdecl operator new[](unsigned long long);
#pragma comment(linker, "/EXPORT:??3@YAXPEAX@Z")    // void __cdecl operator delete(void *);
#pragma comment(linker, "/EXPORT:??3@YAXPEAX_K@Z")  // void __cdecl operator delete(void *, unsigned long long);
#pragma comment(linker, "/EXPORT:??_V@YAXPEAX@Z")   // void __cdecl operator delete[](void *)
#pragma comment(linker, "/EXPORT:??_V@YAXPEAX_K@Z") // void __cdecl operator delete[](void *, unsigned long long);
#pragma comment(linker, "/EXPORT:??_M@YAXPEAX_K1P6AX0@Z@Z=eh_vector_destructor_iterator")
void *__cdecl operator new(unsigned long long size)
{
	return Memory::allocate(size);
}
void *__cdecl operator new[](unsigned long long size)
{
	return operator new(size);
}
void __cdecl operator delete(void *block) noexcept
{
	Memory::free(block);
}
void __cdecl operator delete(void *block, unsigned long long) noexcept
{
	operator delete(block);
}
void __cdecl operator delete[](void *block) noexcept
{
	operator delete(block);
}
void __cdecl operator delete[](void *block, unsigned long long) noexcept
{
	operator delete(block);
}
extern "C" void eh_vector_destructor_iterator(const unsigned char *ptr, QWORD size, QWORD count, void (__thiscall *destructor)(const void *))
{
	for (ptr += (size * count); count--; destructor(ptr -= size));
}


#endif


#pragma comment(linker, "/EXPORT:??2@YAPEAX_KPEAX@Z") // void *__cdecl operator new(unsigned long long, void *);
#pragma comment(linker, "/EXPORT:??3@YAXPEAX0@Z") // void __cdecl operator delete(void *, void *);
void *__cdecl operator new(unsigned long long, void *where) noexcept
{
	return where;
}
void __cdecl operator delete(void *, void *) noexcept
{
}