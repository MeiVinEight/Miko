#include "MWORD.h"
#include "BinaryStorage.h"

constexpr inline QWORD max(QWORD a, QWORD b)
{
	return (a > b) ? a : b;
}

MWORD::MWORD() = default;

MWORD::MWORD(QWORD value): MWORD()
{
	QWORD x = value;
	QWORD size = 0;
	while (x)
	{
		size++;
		x >>= 8;
	}
	this->address.resize(size);
	QWORD i = 0;
	while (value)
	{
		this->address[i++] = (value & 0xFF);
		value >>= 8;
	}
	this->length = address.length;
}

MWORD::MWORD(const MWORD &) = default;

MWORD::MWORD(MWORD &&move) noexcept: address((Memory::string &&) move.address), length(move.length)
{
	move.length = 0;
}

MWORD &MWORD::operator=(const MWORD &) & = default;

MWORD &MWORD::operator=(MWORD &&move) & noexcept
{
	if (&move != this)
	{
		this->address = (Memory::string &&) move.address;
		this->length = move.length;
		move.length = 0;
	}
	return *this;
}

MWORD &MWORD::operator+=(const MWORD &val) &
{
	DWORD carry = 0;
	QWORD size = max(this->length, val.length);
	Memory::fill(this->address.address + this->length, 0, this->address.length - this->length);
	this->address.resize(size + 1);
	for (QWORD i = 0; i < size; i++)
	{
		carry += (i < this->length) ? this->address[i] : 0;
		carry += (i < val.length) ? val.address[i] : 0;
		this->address[i] = carry & 0xFF;
		carry >>= 8;
	}
	this->address[size] = carry;
	this->clear();
	return *this;
}

MWORD &MWORD::operator-=(const MWORD &val) &
{
	if (val.length)
	{
		MWORD adder = val;
		for (QWORD i = 0; i < adder.length; i++)
		{
			adder.address[i] = ~adder.address[i];
		}
		adder += 1;
		// (*this) += (~val + 1)
		(*this) += adder;
		this->address[this->length - 1] = 0;
		this->clear();
	}
	return *this;
}

MWORD &MWORD::operator*=(const MWORD &val2) &
{
	MWORD val1 = *this;
	const Memory::string &addr1 = val1.address;
	const Memory::string &addr2 = val2.address;
	Memory::string addr3(val1.length + val2.length);
	Memory::fill(addr3.address, 0, addr3.length);

	for (QWORD i = 0; i < val1.length; i++)
	{
		DWORD carry = 0;
		for (QWORD j = 0; (j < val2.length) || carry; j++)
		{
			carry += (j < addr2.length) ? (addr1[i] * addr2[j]) : 0;
			carry += addr3[i + j];
			addr3[i + j] = carry & 0xFF;
			carry >>= 8;
		}
	}
	this->address = addr3;
	this->clear();
	return *this;
}

MWORD &MWORD::operator/=(const MWORD &val) &
{
	MWORD mod;
	MWORD::division(*this, val, *this, mod);
	return *this;
}

MWORD &MWORD::operator%=(const MWORD &val) &
{
	MWORD div;
	MWORD::division(*this, val, div, *this);
	return *this;
}

MWORD &MWORD::operator<<=(QWORD val) &
{
	QWORD byt = val >> 3;
	QWORD bit = val & 0x7;
	this->address.resize(this->length + byt + 1);
	Memory::fill(this->address.address + this->length, 0, byt + 1);
	for (QWORD i = this->length; i--;)
	{
		DWORD x = this->address[i] << bit;
		this->address[i + byt] = x & 0xFF;
		x >>= 8;
		this->address[i + byt + 1] |= x;
	}
	Memory::fill(this->address.address, 0, byt);
	this->clear();
	return *this;
}

MWORD &MWORD::operator>>=(QWORD val) &
{
	QWORD byt = val >> 3;
	QWORD bit = val & 0x7;
	for (QWORD i = 0; i < this->length - byt; i++)
	{
		(i) ? (this->address[i - 1] |= (this->address[i + byt] << (8 - bit)) & 0xFF) : 0;
		this->address[i] = this->address[i + byt] >> bit;
	}
	this->clear();
	return *this;
}

MWORD MWORD::operator+(const MWORD &val) const
{
	MWORD ths = *this;
	return ths += val;
}

MWORD MWORD::operator*(const MWORD &val) const
{
	MWORD ths = *this;
	return ths *= val;
}

MWORD MWORD::operator*(QWORD val) const
{
	return (*this) * MWORD(val);
}

MWORD MWORD::operator*(int val) const
{
	return (*this) * MWORD(val);
}

MWORD MWORD::operator/(const MWORD &val) const
{
	MWORD ths = *this;
	return ths /= val;
}

MWORD MWORD::operator%(const MWORD &val) const
{
	MWORD ths = *this;
	return ths %= val;
}

MWORD MWORD::operator<<(QWORD val) const
{
	MWORD ths = *this;
	return ths <<= val;
}

MWORD MWORD::operator<<(int val) const
{
	return (*this) << (QWORD) val;
}

bool MWORD::operator<(const MWORD &val) const
{
	if (this->length == val.length)
	{
		for (QWORD i = this->length; i--;)
		{
			if (this->address[i] != val.address[i])
			{
				return this->address[i] < val.address[i];
			}
		}
		return false;
	}
	return this->length < val.length;
}

bool MWORD::operator>(const MWORD &val) const
{
	if (this->length == val.length)
	{
		for (QWORD i = this->length; i--;)
		{
			if (this->address[i] != val.address[i])
			{
				return this->address[i] > val.address[i];
			}
		}
		return false;
	}
	return this->length > val.length;
}

bool MWORD::operator>=(const MWORD &val) const
{
	if (this->length == val.length)
	{
		for (QWORD i = this->length; i--;)
		{
			if (this->address[i] != val.address[i])
			{
				return this->address[i] > val.address[i];
			}
		}
		return true;
	}
	return this->length > val.length;
}

MWORD::operator bool() const
{
	return this->length;
}

MWORD::operator QWORD() const
{
	QWORD ret = 0;
	for (QWORD i = this->length; i--;)
	{
		ret <<= 8;
		ret |= this->address[i];
	}
	return ret;
}

MWORD::operator int() const
{
	return (int) (QWORD) (*this);
}

void MWORD::clear()
{
	this->length = this->address.length;
	while (this->length && !this->address[this->length - 1])
	{
		this->length--;
	}
}

int MWORD::compare(const MWORD &val) const
{
	if ((*this) > val)
	{
		return 1;
	}
	else if ((*this) < val)
	{
		return -1;
	}
	return 0;
}

QWORD MWORD::width() const
{
	if (this->length)
	{
		return (8 * this->length) - (LeadingZeros(this->address[this->length - 1]) - 56);
	}
	return 0;
}

String::string MWORD::string() const
{
	// log10(2)
	double log2 = 0.30102999566398119521373889472449;
	Memory::string buf((QWORD) ((double) this->width() * log2) + 2);
	Memory::fill(buf.address, 0, buf.length);
	QWORD idx = buf.length - 2;
	QWORD off = 0;
	buf[idx]  = '0';
	MWORD a = *this;
	while (a)
	{
		MWORD v;
		MWORD::division(a, 10, a, v);
		int x = v;
		idx -= off;
		off = 1;
		buf[idx] = x + '0';
	}
	return String::string(buf.address + idx, buf.length - idx);
}

/*
 * val1 / val2 = val3 ...... val4
 */
void MWORD::division(const MWORD &val1, MWORD val2, MWORD &val3, MWORD &val4)
{
	val4 = val1;
	val3 = 0;
	MWORD res = 1;
	QWORD bitcount1 = val4.width();
	QWORD bitcount2 = val2.width();
	if (bitcount1 >= bitcount2)
	{
		QWORD shift = bitcount1 - bitcount2;
		val2 <<= shift;
		res <<= shift;
		while (res && val4)
		{
			if (val4 >= val2)
			{
				val4 -= val2;
				val3 += res;
			}
			val2 >>= 1;
			res >>= 1;
		}
	}
}

MWORD POWER(MWORD a, QWORD n)
{
	MWORD r = 1;
	while (n)
	{
		if (n & 1)
		{
			r *= a;
		}
		a *= a;
		n >>= 1;
	}
	return r;
}