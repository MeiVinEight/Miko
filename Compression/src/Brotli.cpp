#include "definitions.h"

class State
{
	public:
	WORD WBITS = 0;
	DWORD distance[4] = {4, 11, 15, 16};
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
	Huffman *children[2]{NULL, NULL};
	QWORD value = 0;
	~Huffman()
	{
		delete this->children[0];
		delete this->children[1];
	}
	Huffman *operator[](WORD i) const
	{
		return this->children[i];
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
		for (; idx; idx--)
			que[idx] = que[idx] - 1;

		que[0] = val;
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

WORD ReadVarInt(Compression::NibbleReader &br)
{
	WORD bits = br.read(1);
	if (!bits)
		return 1;

	bits = br.read(3);
	return br.read(bits) + (1 << bits) + 1;
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
	// alphabet bit
	WORD abb = 0;
	QWORD x = alphabetSize - 1;
	while (x)
	{
		abb++;
		x >>= 1;
	}

	Huffman *node = new Huffman;
	WORD prefixType = br.read(2);
	if (prefixType == 1)
	{
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
	}
	else
	{
		// TODO complex prefix code
		throw Exception::exception("Temporary unsupported: complex prefix code");
	}
	return node;
}

QWORD DecodeWithHuffman(Compression::NibbleReader &br, Huffman *huffman)
{
	while ((*huffman)[0] || (*huffman)[1])
	{
		huffman = (*huffman)[br.read(1)];
	}
	return huffman->value;
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
			DWORD code = DecodeWithHuffman(br, huffman);
			if (code > RELMAX)
			{
				context[i++] = code - RELMAX;
				i++;
			}
			else
			{
				DWORD rep = (1 << code) + br.read(code);
				if (i + rep > contextMapSize)
				{
					throw Exception::exception("Corrupted context map");
				}
				Memory::fill(context + i, 0, rep);
				i += rep;
			}
		}
		delete huffman;

		if (br.read(1))
			IMTF(context);
	}
	else
	{
		Memory::fill(context, 0, context.length);
	}
	return context;
}

/* References: RFC7932 - Section 5. Encoding of Literal Insertion Lengths and Copy Lengths */
WORD INSERT_AND_COPY_CODE[] = {
	0x0000, 0x0008, 0x0000, 0x0008, 0x0800, 0x0808,
	0x0010, 0x1000, 0x0810, 0x1008,0x1010
};
DWORD INSERT_LENGTH[] = {
	0, 1, 2, 3, 4, 5, 6, 8,
	10, 14, 18, 26, 34, 50, 66, 98,
	130, 194, 322, 578, 1090, 2114, 6210, 22594
};
BYTE INSERT_EXTRA[] = {
	0, 0, 0, 0, 0, 0, 1, 1,
	2, 2, 3, 3, 4, 4, 5, 5,
	6, 7, 8, 9, 10, 12, 14, 24
};
DWORD COPY_LENGTH[] = {
	2, 3, 4, 5, 6, 7, 8, 9,
	10, 12, 14, 18, 22, 30, 38, 54,
	70, 102, 134, 198, 326, 582, 1094, 2118
};
BYTE COPY_EXTRA[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 2, 2, 3, 3, 4, 4,
	5, 5, 6, 7, 8, 9, 10, 24
};
BYTE DISTANCE_INDEX[] = {
	0, 1, 2, 3,
	0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1
};
int DISTANCE_OFFSET[] = {
	0, 0, 0, 0,
	-1, 1, -2, 2, -3, 3,
	-1, 1, -2, 2, -3, 3
};

void DecodeCompressedMetaBlock(State &s, Compression::NibbleReader &br, Memory::string &output, QWORD &length)
{
	WORD NBLTYPESL = ReadVarInt(br);
	// TODO NBLTYPESL >= 2
	WORD NBLTYPESI = ReadVarInt(br);
	// TODO NBLTYPESI >= 2
	WORD NBLTYPESD = ReadVarInt(br);
	// TODO NBLTYPESD >= 2

	BYTE NPOSTFIX = br.read(2);
	BYTE NDIRECT = br.read(4) << NPOSTFIX;

	BYTE *contextMode = (BYTE *)Memory::allocate(NBLTYPESL);
	for (WORD i = 0; i < NBLTYPESL; contextMode[i++] = br.read(2));

	WORD NTREESL = ReadVarInt(br);
	// TODO NTREESL >= 2
	WORD NTREESD = ReadVarInt(br);
	// TODO NTREESD >= 2

	Huffman **TREESL = new Huffman*[NTREESL];
	Huffman **TREESI = new Huffman*[NBLTYPESI];
	Huffman **TREESD = new Huffman*[NTREESD];
	for (WORD i = 0; i < NTREESL; TREESL[i++] = DecodeHuffman(br, 256));
	for (WORD i = 0; i < NBLTYPESI; TREESI[i++] = DecodeHuffman(br, 704));
	for (WORD i = 0; i < NTREESD; TREESD[i++] = DecodeHuffman(br, 16ULL + NDIRECT + (48ULL << NPOSTFIX)));

	// TODO NBLTYPESI >= 2

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
	QWORD IACC = DecodeWithHuffman(br, TREESI[0]);
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
		output[length + i] = DecodeWithHuffman(br, TREESL[0]);
	}
	length += ilength;

	// TODO More data

	DWORD dcode = DecodeWithHuffman(br, TREESD[0]);
	DWORD distance = 0;
	if (dcode < 16)
		distance = s.distance[DISTANCE_INDEX[dcode]] + DISTANCE_OFFSET[dcode];
	else
	{
		// TODO dcode >= 16
	}
	s.updateLastDistance(distance);
	Memory::copy(output + length, output + (length - distance), clength);

	// TODO More data

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
					throw Exception::exception("Wrong data format");
				Memory::copy(output + length, br.stream + (br.position >> 3), MLEN);
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
				throw Exception::exception("Wrong data format");
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