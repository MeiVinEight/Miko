#ifndef MWORD_H
#define MWORD_H
#include "definitions.h"

class MWORD
{
	public:
	Memory::string address = Memory::string(0);
	QWORD length = 0;

	MWORD();
	MWORD(QWORD);
	MWORD(const MWORD &);
	MWORD(MWORD &&) noexcept;
	MWORD &operator=(const MWORD &) &;
	MWORD &operator=(MWORD &&) & noexcept;
	MWORD &operator+=(const MWORD &) &;
	MWORD &operator-=(const MWORD &) &;
	MWORD &operator*=(const MWORD &) &;
	MWORD &operator/=(const MWORD &) &;
	MWORD &operator%=(const MWORD &) &;
	MWORD &operator<<=(QWORD) &;
	MWORD &operator>>=(QWORD) &;
	MWORD operator+(const MWORD &) const;
	MWORD operator*(const MWORD &) const;
	MWORD operator*(QWORD) const;
	MWORD operator*(int) const;
	MWORD operator/(const MWORD &) const;
	MWORD operator%(const MWORD &) const;
	MWORD operator<<(QWORD) const;
	MWORD operator<<(int) const;
	bool operator<(const MWORD &) const;
	bool operator>(const MWORD &) const;
	bool operator>=(const MWORD &) const;
	operator bool() const;
	operator QWORD() const;
	operator int() const;
	void clear();
	int compare(const MWORD &) const;
	QWORD width() const;
	String::string string() const;
	static void division(const MWORD &, MWORD, MWORD &, MWORD &);
};

MWORD POWER(MWORD, QWORD);

#endif //MWORD_H
