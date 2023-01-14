#include "definitions.h"
#include "MWORD.h"

const int EXP_BIAS = (1 << 10) - 1;
const DWORD EXP_SHIFT = 52;
const QWORD SIGNIF_BIT_MASK = (1ULL << EXP_SHIFT) - 1;
const QWORD EXP_OFFSET = 1023;

QWORD DoubleToLong(double x)
{
	return *((QWORD *) &x);
}
double LongToDouble(QWORD x)
{
	return *((double *) &x);
}
int NumberSize(QWORD x)
{
	int v = 0;
	while (x)
	{
		v++;
		x >>= 1;
	}
	return v;
}
int LeadingZeros(QWORD x)
{
	return 64 - NumberSize(x);
}
int TrailingZeros(QWORD x)
{
	if (x)
	{
		int r = 0;
		while (!(x & 1))
		{
			r++;
			x >>= 1;
		}
		return r;
	}
	return 64;
}
int estimateDecExp(QWORD fractBits, int binExp)
{
	double d2 = LongToDouble(((QWORD) EXP_BIAS << EXP_SHIFT) | (fractBits & ((1ULL << EXP_SHIFT) - 1)));
	double d = (d2 - 1.5) * 0.289529654 + 0.176091259 + (double) binExp * 0.301029995663981;
	QWORD dBits = DoubleToLong(d);  //can't be NaN here so use raw
	int exponent = (int) ((dBits >> EXP_SHIFT) & 0x7FF) - EXP_BIAS;
	bool isNegative = (dBits & (1ULL << 63)) != 0; // discover sign
	if (exponent >= 0 && exponent < 52) // hot path
	{
		QWORD mask = ((1ULL << EXP_SHIFT) - 1) >> exponent;
		int r = (int) (((dBits & SIGNIF_BIT_MASK) | (1ULL << EXP_SHIFT)) >> (EXP_SHIFT - exponent));
		return isNegative ? (((mask & dBits) == 0L) ? -r : -r - 1) : r;
	}
	else if (exponent < 0)
	{
		return (((dBits & ~(1ULL << 63)) == 0) ? 0 : ((isNegative) ? -1 : 0));
	}
	else //if (exponent >= 52)
	{
		return (int) d;
	}
}
void roundup(int firstDigitIndex, int nDigits, Memory::string &digits, int &decExponent)
{
	int i = (firstDigitIndex + nDigits - 1);
	int q = digits[i];
	if (q == '9') {
		while (q == '9' && i > firstDigitIndex) {
			digits[i] = '0';
			q = digits[--i];
		}
		if (q == '9') {
			// carryout! High-order 1, rest 0s, larger exp.
			decExponent += 1;
			digits[firstDigitIndex] = '1';
			return;
		}
		// else fall through.
	}
	digits[i] = (char) (q + 1);
}

String::string String::stringify(double d)
{
	Memory::string digits(20);
	Memory::fill(digits, 0, digits.length);
	int decExponent = 0;
	int firstDigitIndex;
	int nDigits;
	int (*max)(int, int) = [](int a, int b) -> int
	{
		return (a > b) ? a : b;
	};
	QWORD dBits = DoubleToLong(d);
	bool isNegative = dBits & (1ULL << 63);
	QWORD fractBits = dBits & ((1ULL << EXP_SHIFT) - 1);
	int binExp = (int) ((dBits >> EXP_SHIFT) & 0x7FF);
	if (binExp == 0x7FF) // NaN or Infinity
	{
		if (fractBits)
			return "NaN";
		else
			return isNegative ? "-Infinity" : "Infinity";
	}
	int nSignificantBits;
	if (binExp == 0)
	{
		if (fractBits == 0)
			return isNegative ? "-0.0" : "0.0";
		int leadingZeros = LeadingZeros(fractBits);
		DWORD shift = leadingZeros - (63 - EXP_SHIFT);
		fractBits <<= shift;
		binExp = 1 - (int) shift;
		nSignificantBits = 64 - leadingZeros;
	}
	else
	{
		fractBits |= (1ULL << EXP_SHIFT);
		nSignificantBits = EXP_SHIFT + 1;
	}
	binExp -= EXP_BIAS;

	// dtoa
	int tailZeros = TrailingZeros(fractBits);
	int nFractBits = (int) ((EXP_SHIFT + 1) - tailZeros);
	int nTinyBits = max(0, nFractBits - binExp - 1);
	// split
	{
		int decExp = estimateDecExp(fractBits, binExp);
		int B2, B5;
		int S2, S5;
		int M2, M5;
		B5 = max(0, -decExp);
		B2 = B5 + nTinyBits + binExp;
		S5 = max(0, decExp);
		S2 = S5 + nTinyBits;
		M5 = B5;
		M2 = B2 - nSignificantBits;
		fractBits >>= tailZeros;
		B2 -= nFractBits - 1;
		int common2factor = (B2 < S2) ? B2 : S2;
		B2 -= common2factor;
		S2 -= common2factor;
		M2 -= common2factor;
		M2 -= (nFractBits == 1);
		if (M2 < 0)
		{
			B2 -= M2;
			S2 -= M2;
			M2 = 0;
		}
		int ndigit = 0;
		bool low, high;
		long long lowDigitDifference;
		int q;

		MWORD b = (POWER(5, B5) * fractBits) << B2;
		MWORD s = POWER(5, S5) << S2;
		MWORD m = POWER(5, M5) << M2;
		MWORD tens = s * 10;
		q = (int) (b / s);
		b = (b % s) * 10;
		m *= 10;
		low = b < m;
		high = (b + m) > tens;
		if ((q == 0) && !high)
		{
			decExp--;
		}
		else
		{
			digits[ndigit++] = '0' + q;
		}
		if (decExp < -3 || decExp >= 8)
			high = low = false;
		while (!low && !high)
		{
			q = (int) (b / s);
			b = (b % s) * 10;
			m *= 10;
			if (m > (MWORD) 0)
			{
				low = (b < m);
				high = ((b + m) > tens);
			}
			else
			{
				low = high = true;
			}
			digits[ndigit++] = '0' + q;
		}
		lowDigitDifference = (b << 1).compare(tens); // (long long) ((long long) (b << 1) - tens)

		decExponent = decExp + 1;
		firstDigitIndex = 0;
		nDigits = ndigit;
		if (high)
		{
			if (low)
			{
				if (lowDigitDifference == 0)
				{
					if ((digits[firstDigitIndex + nDigits - 1] & 1) != 0)
					{
						roundup(firstDigitIndex, nDigits, digits, decExponent);
					}
				}
				else if (lowDigitDifference > 0)
				{
					roundup(firstDigitIndex, nDigits, digits, decExponent);
				}
			}
			else
			{
				roundup(firstDigitIndex, nDigits, digits, decExponent);
			}
		}
	}

	BYTE result[26]{0};
	DWORD i = 0;
	if (isNegative)
		result[i++] = '-';

	if (decExponent > 0 && decExponent < 8)
	{
		DWORD charLength = (nDigits < decExponent) ? nDigits : decExponent;
		Memory::copy(result + i, digits + firstDigitIndex, charLength);
		i += charLength;
		if (charLength < decExponent)
		{
			charLength = decExponent - charLength;
			Memory::fill(result + i, '0', charLength);
			i += charLength;
			result[i++] = '.';
			result[i++] = '0';
		}
		else
		{
			result[i++] = '.';
			if (charLength < nDigits)
			{
				DWORD t = nDigits - charLength;
				Memory::copy(result + i, digits + firstDigitIndex + charLength, t);
				i += t;
			}
			else
			{
				result[i++] = '0';
			}
		}
	}
	else if (decExponent <= 0 && decExponent > -3)
	{
		result[i++] = '0';
		result[i++] = '.';
		if (decExponent != 0)
		{
			Memory::fill(result + i, '0', -decExponent);
			i -= decExponent;
		}
		Memory::copy(result + i, digits + firstDigitIndex, nDigits);
		i += nDigits;
	}
	else
	{
		result[i++] = digits[firstDigitIndex];
		result[i++] = '.';
		if (nDigits > 1)
		{
			Memory::copy(result + i, digits + firstDigitIndex + 1, nDigits - 1);
			i += nDigits - 1;
		}
		else
		{
			result[i++] = '0';
		}
		result[i++] = 'E';
		int e;
		if (decExponent <= 0)
		{
			result[i++] = '-';
			e = -decExponent + 1;
		}
		else
		{
			e = decExponent - 1;
		}

		if (e < 10)
		{
			result[i++] = e + '0';
		}
		else if (e < 100)
		{
			result[i++] = (e / 10) + '0';
			result[i++] = (e % 10) + '0';
		}
		else
		{
			result[i++] = (e / 100) + '0';
			e %= 100;
			result[i++] = (e / 10) + '0';
			result[i++] = (e % 10) + '0';
		}
	}
	return String::string(result, i);
}
String::string String::stringify(QWORD val, bool sign)
{
	char buf[25]{0};
	int i = 0;
	if (sign)
	{
		bool neg = val >> 63;
		val &= ((1ULL << 63) - 1);
		if (neg)
		{
			buf[i++] = '-';
			val -= 1;
			val = ~val;
			val &= ((1ULL << 63) - 1);
		}
	}
	QWORD dec = 10000000000000000000ULL;
	bool add = false;
	while (dec)
	{
		BYTE dig = val / dec;
		val %= dec;
		dec /= 10;
		add |= !!dig;
		buf[i] = (char)(dig + '0');
		i += add;
	}
	return String::string(buf);
}
String::string String::stringify(QWORD val)
{
	return String::stringify(val, false);
}
String::string String::stringify(int val)
{
	return String::stringify(val, true);
}
double String::floating(const String::string &str)
{
	bool negative = false;
	QWORD i = 0;
	if (str[i] == '+' || str[i] == '-')
		negative = str[i++] == '-';

	if (String::string(str.address + i, str.length - i) == "NaN")
	{
		return LongToDouble((1ULL << 63) - 1);
	}
	else if (String::string(str.address + i, str.length - i) == "Infinity")
	{
		QWORD bits = 0x7FFULL << 52;
		if (negative)
		{
			bits |= (1ULL << 63);
		}
		return LongToDouble(bits);
	}
	else
	{
		QWORD bits = (((QWORD) negative) << 63);
		BYTE off = EXP_SHIFT;
		QWORD ipart = 0;
		QWORD exp = 0x7FF;
		for (; i < str.length; i++)
		{
			if (str[i] >= '0' && str[i] <= '9')
			{
				ipart *= 10;
				ipart += str[i] - '0';
				continue;
			}
			break;
		}
		if (ipart)
		{
			int width = NumberSize(ipart);
			exp = width - 1 + EXP_OFFSET;
			if (width > EXP_SHIFT)
			{
				ipart >>= (width - (EXP_SHIFT + 1));
				off = 0;
			}
			else
			{
				ipart <<= ((EXP_SHIFT + 1) - width);
				off -= (width - 1);
			}
			ipart &= SIGNIF_BIT_MASK;
			bits |= ipart;
		}
		if (i < str.length && str[i] == '.')
		{
			i++;
			QWORD fpart = 0;
			QWORD dec = 1;
			for (; i < str.length; i++)
			{
				if (str[i] >= '0' && str[i] <= '9')
				{
					fpart *= 10;
					fpart += str[i] - '0';
					dec *= 10;
					continue;
				}
				break;
			}
			int bitcount = 0;
			while (off && fpart)
			{
				bitcount++;
				fpart <<= 1;
				QWORD b = fpart >= dec;
				if (b)
					fpart -= dec;

				if (exp != 0x7FF)
				{
					bits |= (b << (--off));
				}
				else if (b)
				{
					exp = EXP_OFFSET - bitcount;
				}
			}
		}
		bits |= exp << EXP_SHIFT;
		double val = LongToDouble(bits);
		if (i < str.length && (str[i] == 'E' || str[i] == 'e'))
		{
			i++;
			bool positiveE = true;
			if (str[i] == '+' || str[i] == '-')
				positiveE = str[i++] == '+';

			QWORD decExp = 0;
			for (; i < str.length; i++)
			{
				if (str[i] >= '0' && str[i] <= '9')
				{
					decExp *= 10;
					decExp += str[i] - '0';
					continue;
				}
				break;
			}
			double a = 1E1;
			while (decExp)
			{
				if (decExp & 1)
				{
					positiveE ? (val *= a) : (val /= a);
				}
				decExp >>= 1;
				a *= a;
			}
		}
		if (i < str.length)
			return *((double *)1);
		return val;
	}
}
QWORD String::integer(const String::string &str)
{
	QWORD i = 0;
	bool neg = false;
	if (i < str.length && str[i] == '-')
	{
		neg = true;
		i++;
	}
	QWORD val = 0;
	for (; i < str.length; i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			val *= 10;
			val += str[i] - '0';
			continue;
		}
		break;
	}
	if (i < str.length)
		return *((QWORD *)1);

	return neg ? (val * -1) : (val);
}