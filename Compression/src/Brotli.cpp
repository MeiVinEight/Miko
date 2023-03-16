#include "definitions.h"
#include "StaticDictionary.h"

/* References: RFC7932 - Section 4.  Encoding of Distances */
BYTE DISTANCE_INDEX[] = { 0, 1, 2, 3, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1};
int DISTANCE_OFFSET[] = { 0, 0, 0, 0, -1, 1, -2, 2, -3, 3, -1, 1, -2, 2, -3, 3 };
/* References: RFC7932 - Section 5. Encoding of Literal Insertion Lengths and Copy Lengths */
WORD INSERT_AND_COPY_CODE[] = { 0x0000, 0x0008, 0x0000, 0x0008, 0x0800, 0x0808, 0x0010, 0x1000, 0x0810, 0x1008, 0x1010 };
DWORD INSERT_LENGTH[] = { 0, 1, 2, 3, 4, 5, 6, 8, 10, 14, 18, 26, 34, 50, 66, 98, 130, 194, 322, 578, 1090, 2114, 6210, 22594 };
BYTE INSERT_EXTRA[] = { 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7, 8, 9, 10, 12, 14, 24 };
DWORD COPY_LENGTH[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 18, 22, 30, 38, 54, 70, 102, 134, 198, 326, 582, 1094, 2118 };
BYTE COPY_EXTRA[] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7, 8, 9, 10, 24};
BYTE COMPLEX_SYMBOL[] = { 1, 2, 3, 4, 0, 5, 17, 6, 16, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

namespace ContextMap
{
	// 3 * 256
	WORD LOOKUP[3 << 8] = {
		// Lut0:
		0,  0,  0,  0,  0,  0,  0,  0,  0,  4,  4,  0,  0,  4,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		8, 12, 16, 12, 12, 20, 12, 16, 24, 28, 12, 12, 32, 12, 36, 12,
		44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 32, 32, 24, 40, 28, 12,
		12, 48, 52, 52, 52, 48, 52, 52, 52, 48, 52, 52, 52, 52, 52, 48,
		52, 52, 52, 52, 52, 48, 52, 52, 52, 52, 52, 24, 12, 28, 12, 12,
		12, 56, 60, 60, 60, 56, 60, 60, 60, 56, 60, 60, 60, 60, 60, 56,
		60, 60, 60, 60, 60, 56, 60, 60, 60, 60, 60, 24, 12, 28, 12,  0,
		0,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1,
		0,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1,
		0,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1,
		0,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1,
		2,  3,  2,  3,  2,  3,  2,  3,  2,  3,  2,  3,  2,  3,  2,  3,
		2,  3,  2,  3,  2,  3,  2,  3,  2,  3,  2,  3,  2,  3,  2,  3,
		2,  3,  2,  3,  2,  3,  2,  3,  2,  3,  2,  3,  2,  3,  2,  3,
		2,  3,  2,  3,  2,  3,  2,  3,  2,  3,  2,  3,  2,  3,  2,  3,
		// Lut1:
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1,
		1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1,
		1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		// Lut2:
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7
	};

	/*
	 * CMODE:
	 * 0: LSB6
	 * 1: MSB6
	 * 2: UTF8
	 * 3: Signed
	 */
	DWORD compute(BYTE P1, BYTE P2, BYTE CMODE)
	{
		switch (CMODE)
		{
			case 0:
				return P1 & 0x3F;
			case 1:
				return P1 >> 2;
			case 2:
				return LOOKUP[(0 << 8) + P1] | LOOKUP[(1 << 8) + P2];
			case 3:
				return (LOOKUP[(2 << 8) + P1] << 3) | LOOKUP[(2 << 8) + P2];
			default:
				Memory::violation();
		}
		return 0;
	}
}
class State
{
	public:
	WORD WBITS = 0;
	DWORD BTYPEL = 0;
	DWORD BTYPEI = 0;
	DWORD BTYPED = 0;
	DWORD BLENL = 1 << 24;
	DWORD BLENI = 1 << 24;
	DWORD BLEND = 1<< 24;
	DWORD distance[4] = {4, 11, 15, 16};
	Memory::string CMAPL = Memory::string(0);
	Memory::string CMAPD = Memory::string(0);
	void updateLastDistance(DWORD dis)
	{
		if (dis)
		{
			for (int i = 3; i--; this->distance[i + 1] = this->distance[i]);
			this->distance[0] = dis;
		}
	}
};
class Huffman
{
	public:
	Huffman *children[2]{nullptr, nullptr};
	QWORD value = 0;
	~Huffman()
	{
		delete this->children[0];
		delete this->children[1];
	}
	Huffman *&operator[](WORD i)
	{
		return this->children[i];
	}
	QWORD operator()(Compression::NibbleReader &in) const
	{
		if (this->children[0] || this->children[1])
			return (*(this->children[in.read(1)]))(in);

		return this->value;
	}
};

void IMTF(Memory::string &data)
{
	BYTE que[256];
	for (WORD i = 0; i < 256; i++)
		que[i] = i;

	for (QWORD i = 0; i < data.length; i++)
	{
		BYTE idx = data[i];
		BYTE val = que[idx];
		data[i] = val;
		if (idx)
		{
			for (BYTE j = idx; j; j--)
				que[j] = que[j - 1];
			que[0] = val;
		}
	}
}

BYTE ReadWBITS(Compression::NibbleReader &br)
{
	BYTE WBITS = br.read(1);
	if (!WBITS)
		return 16;

	WBITS = br.read(3);
	if (WBITS)
		return WBITS + 17;

	WBITS = br.read(3);
	if (WBITS)
		return WBITS + 8;

	return 17;
}

WORD ReadVarByte(Compression::NibbleReader &br)
{
	WORD bits = br.read(1);
	if (!bits)
		return 1;

	bits = br.read(3);
	return br.read(bits) + (1 << bits) + 1;
}

Huffman *CreateHuffman(const BYTE *clens, QWORD alphabetSize)
{
	QWORD count[16]{0};
	QWORD numsym = 0;
	QWORD last = 0;
	for (QWORD i = 0; i < alphabetSize; i++)
	{
		if (clens[i])
		{
			last = i;
			numsym++;
			count[clens[i]]++;
		}
	}
	if (numsym == 1)
	{
		return new Huffman{{nullptr, nullptr}, last};
	}
	else
	{
		WORD offset[16]{0};
		for (WORD i = 0; i < 15; i++)
		{
			offset[i + 1] = (offset[i] + count[i]) << 1;
		}
		Huffman *huffman = new Huffman;
		for (QWORD sym = 0; sym < alphabetSize; sym++)
		{
			if (clens[sym])
			{
				BYTE symlen = clens[sym];
				WORD symcod = offset[symlen]++;
				Huffman *current = huffman;
				for (WORD k = symlen; k--;)
				{
					BYTE bit = (symcod >> k) & 1;
					if (!(*current)[bit])
					{
						(*current)[bit] = new Huffman;
					}
					current = (*current)[bit];
				}
				current->value = sym;
			}
		}
		return huffman;
	}
}

/* References: RFC7932 - Section 3. Compressed Representation of Prefix Codes
 *
 * +--------------------------------------------------------+
 * |                   Alphabet size table                  |
 * +-----------------+-------------------------+------------+
 * | Prefix Code     | Alphabet Size           | Definition |
 * +-----------------+-------------------------+------------+
 * | literal         | 256                     |            |
 * +-----------------+-------------------------+------------+
 * | distance        | 16 + NDIRECT +          | Section 4  |
 * |                 | (48 << NPOSTFIX)        |            |
 * +-----------------+-------------------------+------------+
 * | insert-and-copy | 704                     | Section 5  |
 * | length          |                         |            |
 * +-----------------+-------------------------+------------+
 * | block count     | 26                      | Section 6  |
 * +-----------------+-------------------------+------------+
 * | block type      | NBLTYPESx + 2,          | Section 6  |
 * |                 | (where x is I, L, or D) |            |
 * +-----------------+-------------------------+------------+
 * | context map     | NTREESx + RLEMAXx       | Section 7  |
 * |                 | (where x is L or D)     |            |
 * +-----------------+-------------------------+------------+
 *
 * The bit count of symbol is depends on alphabet size, it should
 * be the minimum number ALPHABET_BITS, which satisfied that
 * 1 << ALPHABET_BITS >= Alphabet size
 */
Huffman *DecodeHuffman(Compression::NibbleReader &br, QWORD alphabetSize)
{
	WORD skip = br.read(2);
	if (skip == 1)
	{
		// alphabet bit
		WORD abb = 0;
		QWORD x = alphabetSize - 1;
		while (x)
		{
			abb++;
			x >>= 1;
		}

		Huffman *node = new Huffman;
		WORD symbolCount = br.read(2) + 1;
		QWORD buff[4]{0};
		for (WORD i = 0; i < symbolCount; buff[i++] = br.read(abb));
		switch (symbolCount)
		{
			// Only one symbol, not need huffman code
			case 1:
			{
				node->value = buff[0];
				break;
			}
				// Huffman code is [0, 1]
			case 2:
			{
				node->children[0] = new Huffman;
				node->children[1] = new Huffman;
				node->children[0]->value = buff[0];
				node->children[1]->value = buff[1];
				break;
			}
				// Huffman code is [0, 10, 11]
			case 3:
			{
				node->children[0] = new Huffman;
				node->children[0]->value = buff[0];

				node->children[1] = new Huffman;
				node->children[1]->children[0] = new Huffman;
				node->children[1]->children[1] = new Huffman;
				node->children[1]->children[0]->value = buff[1];
				node->children[1]->children[1]->value = buff[2];
				break;
			}
				// tree-select = 0: Huffman code is [00, 01, 10, 11]
				// tree-select = 1: Huffman code is [0, 10, 110, 111]
			case 4:
			{
				WORD treeSelect = br.read(1);
				if (treeSelect)
				{
					// [0, 10, 110, 111]
					node->children[0] = new Huffman;
					node->children[0]->value = buff[0];

					node->children[1] = new Huffman;
					node->children[1]->children[0] = new Huffman;
					node->children[1]->children[0]->value = buff[1];

					node->children[1]->children[1] = new Huffman;
					node->children[1]->children[1]->children[0] = new Huffman;
					node->children[1]->children[1]->children[1] = new Huffman;
					node->children[1]->children[1]->children[0]->value = buff[2];
					node->children[1]->children[1]->children[1]->value = buff[3];
				}
				else
				{
					// [00, 01, 10, 11]
					node->children[0] = new Huffman;
					node->children[1] = new Huffman;

					node->children[0]->children[0] = new Huffman;
					node->children[0]->children[1] = new Huffman;
					node->children[1]->children[0] = new Huffman;
					node->children[1]->children[1] = new Huffman;

					node->children[0]->children[0]->value = buff[0];
					node->children[0]->children[1]->value = buff[1];
					node->children[1]->children[0]->value = buff[2];
					node->children[1]->children[1]->value = buff[3];
				}
				break;
			}
		}
		return node;
	}
	else
	{
		/*
		 * We use brotli huffman format:
		 * References RFC7932 - 3.2. Use of Prefix Coding in the Brotli Format
		 *
		 * Code length encoded as follows:
		 * 0..15: Represent code lengths of 0..15
		 *    16: Copy the previous non-zero code length 3..6 times.
		 *        The next 2 bits indicate repeat length
		 *              (0 = 3, ... , 3 = 6)
		 *        If this is the first code length, or all previous
		 *        code lengths are zero, a code length of 8 is
		 *        repeated 3..6 times.
		 *        A repeated code length code of 16 modifies the
		 *        repeat count of the previous one as follows:
		 *           repeat count = (4 * (repeat count - 2)) +
		 *                          (3..6 on the next 2 bits)
		 *        Example:  Codes 7, 16 (+2 bits 11), 16 (+2 bits 10)
		 *                  will expand to 22 code lengths of 7
		 *                  (1 + 4 * (6 - 2) + 5)
		 *    17: Repeat a code length of 0 for 3..10 times.
		 *        The next 3 bits indicate repeat length
		 *              (0 = 3, ... , 7 = 10)
		 *        A repeated code length code of 17 modifies the
		 *        repeat count of the previous one as follows:
		 *           repeat count = (8 * (repeat count - 2)) +
		 *                          (3..10 on the next 3 bits)
		 *
		 * We refer a code length as a csymbol,
		 * and the csymbol bit length as follows:
		 * Length    Code
		 * ------    ----
		 * 0           00
		 * 1         0111
		 * 2          011
		 * 3           10
		 * 4           01
		 * 5         1111
		 * And this is also a huffman tree:
		 *       0/\1
		 *       /  \
		 *      /    \
		 *     /      \
		 *   0/\1    0/\1
		 *   /  \    /  \
		 *  0    3  4  0/\1
		 *             /  \
		 *            2  0/\1
		 *               /  \
		 *              1    5
		 */
		static Huffman *_00 = new Huffman{{nullptr, nullptr}, 0};
		static Huffman *_01 = new Huffman{{nullptr, nullptr}, 3};
		static Huffman *_0 = new Huffman{{_00, _01}, 0};
		static Huffman *_10 = new Huffman{{nullptr, nullptr}, 4};
		static Huffman *_110 = new Huffman{{nullptr, nullptr}, 2};
		static Huffman *_1110 = new Huffman{{nullptr, nullptr}, 1};
		static Huffman *_1111 = new Huffman{{nullptr, nullptr}, 5};
		static Huffman *_111 = new Huffman{{_1110, _1111}, 0};
		static Huffman *_11 = new Huffman{{_110, _111}, 0};
		static Huffman *_1 = new Huffman{{_10, _11}, 0};
		static Huffman symbols = {{_0, _1}, 0};
		BYTE symbolLength[18] = {0};
		// BYTE codeLength[alphabetSize] = {0};
		DWORD space = 1 << 5;
		DWORD nsym = 0;
		for (WORD i = skip; i < 18 && space; i++)
		{
			BYTE symidx = COMPLEX_SYMBOL[i];
			BYTE symlen = symbols(br);
			symbolLength[symidx] = symlen;
			if (symlen)
			{
				space -= (32 >> symlen);
				nsym++;
			}
		}
		if (space && (nsym != 1))
		{
			Memory::violation();
		}

		Huffman *codes = CreateHuffman(symbolLength, 18);
		Memory::string codeLength(alphabetSize);
		Memory::fill(codeLength.address, 0, codeLength.length);
		space = 1 << 15;
		QWORD repeat = 0;
		WORD prevclen = 8;
		WORD prevrepl = 0;
		for (QWORD i = 0; i < alphabetSize && space;)
		{
			WORD clen = (*codes)(br);
			if (clen < 16)
			{
				codeLength[i++] = clen;
				space -= clen ? (1 << (15 - clen)) : 0;
				prevclen = clen ? clen : prevclen;
				repeat = 0;
			}
			else
			{
				WORD repclen = (clen - 16) ? 0 : prevclen;
				WORD extra = 3 + br.read(clen - 14);
				repeat = (prevrepl == repclen) ? repeat : 0;
				QWORD delta = repeat;
				repeat = (repeat ? (((repeat - 2) << (clen - 14))) : repeat) + extra;
				delta = repeat - delta;
				prevrepl = repclen;
				if (i + delta > alphabetSize)
				{
					Memory::violation();
				}
				Memory::fill(codeLength.address + i, repclen, delta);
				if (repclen)
				{
					space -= delta << (15 - repclen);
				}
				i += delta;
			}
		}
		delete codes;
		return CreateHuffman((BYTE *) codeLength.address, codeLength.length);
	}
}

Memory::string DecodeContextMap(Compression::NibbleReader &br, QWORD contextMapSize, WORD trees)
{
	Memory::string context(contextMapSize);
	if (trees >= 2)
	{
		DWORD RELMAX = br.read(1);
		if (RELMAX)
			RELMAX = br.read(4) + 1;

		Huffman *huffman = DecodeHuffman(br, RELMAX + trees);
		for (QWORD i = 0; i < contextMapSize;)
		{
			DWORD code = (*huffman)(br);
			if (code > RELMAX)
			{
				context[i++] = code - RELMAX;
			}
			else
			{
				DWORD rep = (1 << code) + br.read(code);
				if (i + rep > contextMapSize)
				{
					Memory::violation();
				}
				Memory::fill(context.address + i, 0, rep);
				i += rep;
			}
		}
		delete huffman;

		if (br.read(1))
			IMTF(context);
	}
	else
	{
		Memory::fill(context.address, 0, context.length);
	}
	return context;
}

void DecodeCompressedMetaBlock(State &s, Compression::NibbleReader &br, Memory::string &output, QWORD &length)
{
	WORD NBLTYPESL = ReadVarByte(br);
	s.BTYPEL = 0;
	s.BLENL = 1 << 24;
	// TODO NBLTYPESL >= 2
	if (NBLTYPESL >= 2)
		Memory::violation();
	WORD NBLTYPESI = ReadVarByte(br);
	s.BTYPEI = 0;
	s.BLENI = 1 << 24;
	// TODO NBLTYPESI >= 2
	if (NBLTYPESI >= 2)
		Memory::violation();
	WORD NBLTYPESD = ReadVarByte(br);
	s.BTYPED = 0;
	s.BLEND = 1 << 24;
	// TODO NBLTYPESD >= 2
	if (NBLTYPESD >= 2)
		Memory::violation();

	BYTE NPOSTFIX = br.read(2);
	BYTE NDIRECT = br.read(4) << NPOSTFIX;

	BYTE *contextMode = (BYTE *)Memory::allocate(NBLTYPESL);
	for (WORD i = 0; i < NBLTYPESL; contextMode[i++] = br.read(2));

	WORD NTREESL = ReadVarByte(br);
	s.CMAPL = DecodeContextMap(br, NBLTYPESL << 6, NTREESL);
	WORD NTREESD = ReadVarByte(br);
	s.CMAPD = DecodeContextMap(br, NBLTYPESD << 2, NTREESD);

	Huffman **TREESL = new Huffman*[NTREESL];
	Huffman **TREESI = new Huffman*[NBLTYPESI];
	Huffman **TREESD = new Huffman*[NTREESD];
	for (WORD i = 0; i < NTREESL; TREESL[i++] = DecodeHuffman(br, 256));
	for (WORD i = 0; i < NBLTYPESI; TREESI[i++] = DecodeHuffman(br, 704));
	for (WORD i = 0; i < NTREESD; TREESD[i++] = DecodeHuffman(br, 16ULL + NDIRECT + (48ULL << NPOSTFIX)));

	while (length < output.length)
	{
		// TODO NBLTYPESI >= 2
		if (!s.BLENI)
		{
			Memory::violation();
		}
		s.BLENI--;

		/* References: RFC7932 - Section 5. Encoding of Literal Insertion Lengths and Copy Lengths
		 *
		 * Insert length and copy length compressed in data is:
		 * <insert-and-copy code, insert extra bits, copy extra bits>
		 *
		 * IACC (insert-and-copy code) is a 10-bit number range 0..703
		 * +------------+
		 * | IACC = 266 |
		 * +----+---+---+
		 * |0100|001|010|
		 * +----+---+---+
		 *   ^    ^   ^
		 *   |    |   |
		 *   |    |   +---- Copy code offset
		 *   |    +-------- Insert code offset
		 *   +------------- The cell in insert-and-copy code table
		 *
		 * The insert-and-copy code table is:
		 * Insert              Copy code
		 * code       0..7       8..15     16..23
		 *        +----------+----------+
		 *  0..7  |   0..63  |  64..127 | <--- distance symbol 0
		 *        +----------+----------+----------+
		 *  0..7  | 128..191 | 192..255 | 384..447 |
		 *        +----------+----------+----------+
		 *  8..15 | 256..319 | 320..383 | 512..575 |
		 *        +----------+----------+----------+
		 * 16..23 | 448..511 | 576..639 | 640..703 |
		 *        +----------+----------+----------+
		 *
		 * IACC = 266, is in 256..319, that is,
		 * the insert code is 8 + Insert code offset = 9,
		 * and the copy code is 0 + Copy code offset = 2.
		 *
		 * Insert length table:
		 *      Extra              Extra               Extra
		 * Code Bits Lengths  Code Bits Lengths   Code Bits Lengths
		 * ---- ---- -------  ---- ---- -------   ---- ---- -------
		 *  0    0     0       8    2   10..13    16    6   130..193
		 *  1    0     1       9    2   14..17    17    7   194..321
		 *  2    0     2      10    3   18..25    18    8   322..577
		 *  3    0     3      11    3   26..33    19    9   578..1089
		 *  4    0     4      12    4   34..49    20   10   1090..2113
		 *  5    0     5      13    4   50..65    21   12   2114..6209
		 *  6    1    6,7     14    5   66..97    22   14   6210..22593
		 *  7    1    8,9     15    5   98..129   23   24   22594..16799809
		 *
		 * Copy length table:
		 *      Extra              Extra               Extra
		 * Code Bits Lengths  Code Bits Lengths   Code Bits Lengths
		 * ---- ---- -------  ---- ---- -------   ---- ---- -------
		 *  0    0     2       8    1    10,11    16    5   70..101
		 *  1    0     3       9    1    12,13    17    5   102..133
		 *  2    0     4      10    2    14..17   18    6   134..197
		 *  3    0     5      11    2    18..21   19    7   198..325
		 *  4    0     6      12    3    22..29   20    8   326..581
		 *  5    0     7      13    3    30..37   21    9   582..1093
		 *  6    0     8      14    4    38..53   22   10   1094..2117
		 *  7    0     9      15    4    54..69   23   24   2118..16779333
		 *
		 * Bit count of insert extra bits and copy extra bits is in table above
		 * Extra bits data should add into base length as final length
		 *
		 * The insert code of IACC = 266 is 9, that is, the base length is 14
		 * and extra bit count is 2, should read 2 bits from compressed data.
		 * Assume that the extra bits data is 2, so the insert length is 14 + 2 = 16.
		 *
		 * The copy length is similar.
		 */
		QWORD IACC = (*(TREESI[s.BTYPEI]))(br);
		WORD cell = IACC >> 6;
		WORD ioff = (IACC >> 3) & 7;
		WORD coff = (IACC >> 0) & 7;
		/*
		 * +-----------------+
		 * |0x 08   |   10   |
		 * +--------+--------+
		 * |00001000|00010000|
		 * +--------+--------+
		 * | Insert |  Copy  |
		 * +--------+--------+
		 */
		WORD icode = ((INSERT_AND_COPY_CODE[cell] >> 8) & 0xFF) + ioff;
		WORD ccode = ((INSERT_AND_COPY_CODE[cell] >> 0) & 0xFF) + coff;
		DWORD ilength = INSERT_LENGTH[icode] + br.read(INSERT_EXTRA[icode]);
		DWORD clength = COPY_LENGTH[ccode] + br.read(COPY_EXTRA[ccode]);
		for (DWORD i = 0; i < ilength; i++)
		{
			// TODO NBLTYPESL >= 2
			if (!s.BLENL)
			{
				Memory::violation();
			}
			s.BLENL--;

			BYTE p1 = (length > 0) ? output[length - 1] : 0;
			BYTE p2 = (length > 1) ? output[length - 2] : 0;
			BYTE cmode = contextMode[s.BTYPEL];
			DWORD CIDL = ContextMap::compute(p1, p2, cmode);
			output[length++] = (*(TREESL[s.CMAPL[s.BTYPEL * 64 + CIDL]]))(br);
		}
		if (length < output.length)
		{
			DWORD dcode = 0;
			DWORD distance;
			if (IACC < 128)
			{
				distance = s.distance[0];
			}
			else
			{
				// TODO NBLTYPESD >= 2
				if (!s.BLEND)
				{
					Memory::violation();
				}
				s.BLEND--;

				DWORD CIDD = ((clength > 5) ? 5 : clength) - 2;

				dcode = (*(TREESD[s.CMAPD[s.BTYPED * 64 + CIDD]]))(br);
				if (dcode < 16)
				{
					distance = s.distance[DISTANCE_INDEX[dcode]] + DISTANCE_OFFSET[dcode];
					if (!distance || distance > (1 << 24))
						Memory::violation();
				}
				else if (dcode < NDIRECT + 16)
				{
					distance = dcode - 15;
				}
				else
				{
					WORD ndistbits = 1 + ((dcode - NDIRECT - 16) >> (NPOSTFIX + 1));
					DWORD dextra = br.read(ndistbits);
					DWORD hcode = (dcode - NDIRECT - 16) >> NPOSTFIX;
					DWORD lcode = (dcode - NDIRECT - 16) & ((1 << NPOSTFIX) - 1);
					DWORD offset = ((2 + (hcode & 1)) << ndistbits) - 4;
					distance = ((offset + dextra) << NPOSTFIX) + lcode + NDIRECT + 1;
				}
			}
			DWORD maxdis = (1 << s.WBITS) - 16;
			if (length < maxdis)
				maxdis = length;

			if (distance > maxdis)
			{
				StaticDictionary::dictionary(output, length, maxdis, distance, clength);
			}
			else
			{
				if (dcode)
					s.updateLastDistance(distance);
				Memory::copy(output.address + length, output.address + (length - distance), clength);
				length += clength;
			}
		}
	}

	for (WORD i = 0; i < NTREESL; delete TREESL[i++]);
	for (WORD i = 0; i < NBLTYPESI; delete TREESI[i++]);
	for (WORD i = 0; i < NTREESD; delete TREESD[i++]);
	delete[] TREESL;
	delete[] TREESI;
	delete[] TREESD;

	Memory::free(contextMode);
}

bool DecodeMetaBlock(State &s, Compression::NibbleReader &br, Memory::string &output, QWORD &length)
{
	BYTE islast = br.read(1);
	BYTE empty = 0;
	BYTE uncompressed = 0;
	if (islast)
		empty = br.read(1);

	if (!empty)
	{
		BYTE MNIBBLES = br.read(2);
		MNIBBLES = (MNIBBLES == 3) ? 0 : (MNIBBLES + 4);
		if (MNIBBLES)
		{
			DWORD MLEN = br.read(MNIBBLES * 4) + 1;
			output.resize(output.length + MLEN);
			if (!islast)
				uncompressed = br.read(1);

			if (uncompressed)
			{
				if (br.boundary())
					Memory::violation();
				Memory::copy(output.address + length, br.stream.address + (br.position >> 3), MLEN);
				br.skip(MLEN * 8);
				length += MLEN;
			}
			else
				DecodeCompressedMetaBlock(s, br, output, length);
		}
		else
		{
			BYTE reserved = br.read(1);
			if (reserved)
			{
				Memory::violation();
			}
			BYTE MSKIPBYTES = br.read(2);
			DWORD skip = br.read(MSKIPBYTES * 8) + 1;
			br.boundary();
			br.skip(skip * 8);
		}
	}

	return !islast;
}

Memory::string uncompress(const Memory::string &data)
{
	Memory::string output(0);
	Compression::NibbleReader br(data);
	State s;
	s.WBITS = ReadWBITS(br);
	QWORD length = 0;
	while (DecodeMetaBlock(s, br, output, length));
	return output;
}

Memory::string Compression::Brotli::decoding(const Memory::string &data)
{
	return uncompress(data);
}