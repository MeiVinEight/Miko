#ifndef CMAKE_BUILD

#include <typeinfo.h>


extern "C"
{


int __cdecl strcmp(const char *, const char *);
#pragma intrinsic(strcmp)


}


__declspec(dllexport) unsigned long long type_info::hash_code() const noexcept
{
	unsigned long long fnv_offset_basis = 14695981039346656037ULL;
	unsigned long long fnv_prime        = 1099511628211ULL;

	unsigned long long value = fnv_offset_basis;
	for (const unsigned char *it = (const unsigned char *) (data.DecoratedName); *it; it++)
	{
		value ^= *it;
		value *= fnv_prime;
	}

	value ^= value >> 32;
	return value;
}
__declspec(dllexport) bool type_info::operator==(const type_info &other) const noexcept
{
	if (this != &other)
	{
		return strcmp(this->data.DecoratedName, other.data.DecoratedName) == 0;
	}
	return true;
}
__declspec(dllexport) bool type_info::operator!=(const type_info &other) const noexcept
{
	return !(*this == other);
}
__declspec(dllexport) bool type_info::before(const type_info &other) const noexcept
{
	return strcmp(this->data.DecoratedName, other.data.DecoratedName) < 0;
}
__declspec(dllexport) const char *type_info::name() const noexcept
{
	return this->data.DecoratedName;
}
__declspec(dllexport) const char *type_info::raw_name() const noexcept
{
	return this->data.DecoratedName;
}

#endif//CMAKE_BUILD