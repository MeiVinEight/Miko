#include "definitions.h"
#include "StaticDictionary.h"

DWORD StaticDictionary::NWORDS[25];
DWORD StaticDictionary::DOFFSET[25];
DWORD StaticDictionary::NDBITS[25] = {0, 0, 0, 0, 10, 10, 11, 11, 10, 10, 10, 10, 10, 9, 9, 8, 7, 7, 8, 7, 7, 6, 6, 5, 5};
const char StaticDictionary::PREFIX_SUFFIX[] = " the s of and , in to e \".\">\n"
											   "] for a that . , with ' from by . The on as is ing \n"
											   "\t:ed ( at ly =\".com/ of the . This not er al ='ful ive less est ize \xC2\xA0ous ";
DWORD StaticDictionary::transformation[121][3] = {
	{0x0000,  0, 0x0000}, //   0
	{0x0000,  0, 0x0001}, //   1
	{0x0001,  0, 0x0001}, //   2
	{0x0000,  3, 0x0000}, //   3
	{0x0000,  1, 0x0001}, //   4
	{0x0000,  0, 0x0005}, //   5
	{0x0001,  0, 0x0000}, //   6
	{0x0502,  0, 0x0001}, //   7
	{0x0000,  0, 0x0604}, //   8
	{0x0000,  1, 0x0000}, //   9
	{0x0000,  0, 0x0905}, //  10
	{0x0000,  4, 0x0000}, //  11
	{0x0000, 12, 0x0000}, //  12
	{0x0E02,  0, 0x0000}, //  13
	{0x0000,  0, 0x0E02}, //  14
	{0x0001,  1, 0x0001}, //  15
	{0x0000,  0, 0x0F04}, //  16
	{0x0000,  0, 0x1204}, //  17
	{0x1602,  0, 0x0001}, //  18
	{0x0000,  0, 0x1801}, //  19
	{0x0000,  0, 0x1901}, //  20
	{0x0000,  0, 0x1A02}, //  21
	{0x0000,  0, 0x1C01}, //  22
	{0x0000, 14, 0x0000}, //  23
	{0x0000,  0, 0x1D01}, //  24
	{0x0000,  0, 0x1E05}, //  25
	{0x0000,  5, 0x0000}, //  26
	{0x0000, 13, 0x0000}, //  27
	{0x0000,  0, 0x2203}, //  28
	{0x0000,  0, 0x2406}, //  29
	{0x0001,  1, 0x0000}, //  30
	{0x0000,  0, 0x2A02}, //  31
	{0x1901,  0, 0x0000}, //  32
	{0x0001,  0, 0x2C02}, //  33
	{0x0000,  6, 0x0000}, //  34
	{0x0000,  0, 0x2D06}, //  35
	{0x0000,  0, 0x3301}, //  36
	{0x0000,  0, 0x3406}, //  37
	{0x0000,  0, 0x3904}, //  38
	{0x0000,  7, 0x0000}, //  39
	{0x0000,  8, 0x0000}, //  40
	{0x0005,  0, 0x0000}, //  41
	{0x0000, 15, 0x0000}, //  42
	{0x0000,  0, 0x3D06}, //  43
	{0x0000,  2, 0x0000}, //  44
	{0x0000,  0, 0x4204}, //  45
	{0x0000,  0, 0x4504}, //  46
	{0x0000,  0, 0x4804}, //  47
	{0x0000, 18, 0x0000}, //  48
	{0x0000, 12, 0x4C04}, //  49
	{0x0000,  0, 0x5002}, //  50
	{0x0000,  0, 0x5201}, //  51
	{0x0001,  0, 0x2A02}, //  52
	{0x0000,  0, 0x5303}, //  53
	{0x0000, 11, 0x0000}, //  54
	{0x0000,  9, 0x0000}, //  55
	{0x0000, 17, 0x0000}, //  56
	{0x0000,  0, 0x5601}, //  57
	{0x0000,  1, 0x2C02}, //  58
	{0x0000, 19, 0x0000}, //  59
	{0x0000,  0, 0x5706}, //  60
	{0x0000,  0, 0x5B03}, //  61
	{0x0005,  0, 0x0604}, //  62
	{0x0000, 16, 0x0000}, //  63
	{0x0000, 20, 0x0000}, //  64
	{0x0001,  1, 0x0E02}, //  65
	{0x0000,  1, 0x1801}, //  66
	{0x1901,  0, 0x5601}, //  67
	{0x0000,  2, 0x0001}, //  68
	{0x0000,  1, 0x1A02}, //  69
	{0x0000,  0, 0x5E02}, //  70
	{0x0001,  0, 0x1901}, //  71
	{0x6005,  0, 0x0000}, //  72
	{0x0005,  0, 0x6508}, //  73
	{0x0000,  1, 0x3301}, //  74
	{0x0000,  0, 0x6D07}, //  75
	{0x0000,  0, 0x2C01}, //  76
	{0x1901,  0, 0x0001}, //  77
	{0x0000,  1, 0x5601}, //  78
	{0x0000,  1, 0x1901}, //  79
	{0x0000,  0, 0x7305}, //  80
	{0x0001,  0, 0x5E02}, //  81
	{0x0000,  0, 0x7803}, //  82
	{0x0001,  2, 0x0001}, //  83
	{0x0000,  0, 0x7B03}, //  84
	{0x0001,  2, 0x0000}, //  85
	{0x0000,  0, 0x7E02}, //  86
	{0x0000,  2, 0x1801}, //  87
	{0x0000,  1, 0x3D02}, //  88
	{0x0001,  0, 0x5601}, //  89
	{0x0000,  0, 0x8004}, //  90
	{0x0001,  1, 0x2A02}, //  91
	{0x0000,  0, 0x8404}, //  92
	{0x0000,  0, 0x8805}, //  93
	{0x0000,  2, 0x3301}, //  94
	{0x0000,  0, 0x8D04}, //  95
	{0x0001,  1, 0x2A01}, //  96
	{0x0000,  2, 0x1A02}, //  97
	{0x0001,  0, 0x7E02}, //  98
	{0x0000,  1, 0x2C01}, //  99
	{0x0000,  0, 0x9104}, // 100
	{0x0000,  2, 0x2A01}, // 101
	{0x9502,  0, 0x0000}, // 102
	{0x0001,  0, 0x2C01}, // 103
	{0x0000,  1, 0x5E02}, // 104
	{0x0000,  2, 0x5E02}, // 105
	{0x0000,  0, 0x9704}, // 106
	{0x0000,  2, 0x2C02}, // 107
	{0x0000,  1, 0x7E02}, // 108
	{0x0001,  1, 0x2C01}, // 109
	{0x0001,  2, 0x5E02}, // 110
	{0x0001,  2, 0x2C02}, // 111
	{0x0000,  2, 0x2C01}, // 112
	{0x0000,  2, 0x5601}, // 113
	{0x0000,  2, 0x2A02}, // 114
	{0x0001,  2, 0x2A01}, // 115
	{0x0000,  2, 0x7E02}, // 116
	{0x0001,  2, 0x2A02}, // 117
	{0x0001,  1, 0x5E02}, // 118
	{0x0001,  2, 0x7E02}, // 119
	{0x0001,  1, 0x7E02}, // 120
};
void (*(StaticDictionary::transforms[5]))(Memory::string &, WORD) = {
	[](Memory::string &word, WORD idx) -> void
	{
	},
	[](Memory::string &word, WORD idx) -> void
	{
		StaticDictionary::ferment(word, 0);
	},
	[](Memory::string &word, WORD idx) -> void
	{
		for (WORD i = 0; i < word.length;)
			i += StaticDictionary::ferment(word, i);
	},
	[](Memory::string &word, WORD idx) -> void
	{
		idx -= 2;
		QWORD len = (word.length < idx) ? 0 : (word.length - idx);
		Memory::string fword(len);
		Memory::copy(fword.address, word.address + idx, len);
		word = (Memory::string &&)fword;
	},
	[](Memory::string &word, WORD idx) -> void
	{
		idx -= 11;
		QWORD len = (word.length < idx) ? 0 : (word.length - idx);
		Memory::string fword(len);
		Memory::copy(fword.address, word.address, len);
		word = (Memory::string &&)fword;
	}
};

DWORD StaticDictionary::ferment(Memory::string &word, QWORD pos)
{
	if (word[pos] < 192)
	{
		if (word[pos] >= 97 && word[pos] <= 122)
			word[pos] = word[pos] ^ 32;

		return 1;
	}
	else if (word[pos] < 224)
	{
		if (pos + 1 < word.length)
			word[pos + 1] = word[pos + 1] ^ 32;

		return 2;
	}
	else
	{
		if (pos + 2 < word.length)
			word[pos + 2] = word[pos + 2] ^ 5;

		return 3;
	}
}

void StaticDictionary::dictionary(Memory::string &output, QWORD &length, QWORD allowed, QWORD distance, QWORD copyLength)
{
	QWORD wordID = distance - (allowed + 1);
	QWORD index = wordID % StaticDictionary::NWORDS[copyLength];
	QWORD transformID = wordID >> StaticDictionary::NDBITS[copyLength];

	Memory::string word(copyLength);
	Memory::copy(word.address, StaticDictionary::DICT + (StaticDictionary::DOFFSET[copyLength] + index * copyLength), copyLength);

	if (transformID > 120 || copyLength > 24 || copyLength < 4)
		Memory::violation();

	DWORD tt = StaticDictionary::transformation[transformID][1];
	if (tt < 3)
		StaticDictionary::transforms[tt](word, tt);
	else if (tt < 12)
		StaticDictionary::transforms[3](word, tt);
	else
		StaticDictionary::transforms[4](word, tt);

	Memory::string prefix(StaticDictionary::transformation[transformID][0] & 0xFF);
	Memory::string suffix(StaticDictionary::transformation[transformID][2] & 0xFF);
	Memory::copy(prefix.address, StaticDictionary::PREFIX_SUFFIX + (StaticDictionary::transformation[transformID][0] >> 8), prefix.length);
	Memory::copy(suffix.address, StaticDictionary::PREFIX_SUFFIX + (StaticDictionary::transformation[transformID][2] >> 8), suffix.length);
	Memory::string tword(prefix.length + word.length + suffix.length);
	Memory::copy(tword.address, prefix.address, prefix.length);
	Memory::copy(tword.address + prefix.length, word.address, word.length);
	Memory::copy(tword.address + prefix.length + word.length, suffix.address, suffix.length);
	Memory::copy(output.address + length, tword.address, tword.length);
	length += tword.length;
}

void DllMainStaticDictionary(unsigned int reason)
{
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			StaticDictionary::DOFFSET[0] = 0;
			StaticDictionary::NWORDS[0] = 0;
			for (WORD i = 0; i++ < 24;)
			{
				StaticDictionary::NWORDS[i] = (i < 4) ? 0 : (1 << StaticDictionary::NDBITS[i]);
				StaticDictionary::DOFFSET[i] = StaticDictionary::DOFFSET[i - 1] + (i - 1) * StaticDictionary::NWORDS[i - 1];
			}
			QWORD DICTSIZE = StaticDictionary::DOFFSET[24] + 24 * StaticDictionary::NWORDS[24];
			(void) DICTSIZE;
			break;
		}
	}
}