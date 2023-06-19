#include <base64.h>

/*
 * References: RFC2045 - Section 6.8. Base64 Content-Transfer-Encoding
 *
 *
 *                     Table 1: The Base64 Alphabet
 *
 *   Value Encoding  Value Encoding  Value Encoding  Value Encoding
 *       0 A            17 R            34 i            51 z
 *       1 B            18 S            35 j            52 0
 *       2 C            19 T            36 k            53 1
 *       3 D            20 U            37 l            54 2
 *       4 E            21 V            38 m            55 3
 *       5 F            22 W            39 n            56 4
 *       6 G            23 X            40 o            57 5
 *       7 H            24 Y            41 p            58 6
 *       8 I            25 Z            42 q            59 7
 *       9 J            26 a            43 r            60 8
 *      10 K            27 b            44 s            61 9
 *      11 L            28 c            45 t            62 +
 *      12 M            29 d            46 u            63 /
 *      13 N            30 e            47 v
 *      14 O            31 f            48 w         (pad) =
 *      15 P            32 g            49 x
 *      16 Q            33 h            50 y
 *
 *
 * +------+------------+---------------+---------------+---------------+
 * |      |Text (ASCII)|       M       |       a       |       n       |
 * |Source+------------+---------------+---------------+---------------+
 * |      |   Octets   |   77 (0x4D)   |   97 (0x61)   |   110 (0x6E)  |
 * +------+------------+---------------+---------------+---------------+
 * |        Bits       |0|1|0|0|1|1|0|1|0|1|1|0|0|0|0|1|0|1|1|0|1|1|1|0|
 * +------+------------+-----------+-----------+-----------+-----------+
 * |      |   Sextets  |     19    |     22    |     5     |     46    |
 * |      +------------+-----------+-----------+-----------+-----------+
 * |Base64|  Character |     T     |     W     |     F     |     u     |
 * |      +------------+-----------+-----------+-----------+-----------+
 * |      |   Octets   | 84 (0x54) | 87 (0x57) | 70 (0x46) | 117 (0x75)|
 * +------+------------+-----------+-----------+-----------+-----------+
 *
 * +------+------------+---------------+---------------+---------------+
 * |      |Text (ASCII)|       M       |       a       |               |
 * |Source+------------+---------------+---------------+               |
 * |      |   Octets   |   77 (0x4D)   |   97 (0x61)   |               |
 * +------+------------+---------------+---------------+---------------+
 * |        Bits       |0|1|0|0|1|1|0|1|0|1|1|0|0|0|0|1|0|0| | | | | | |
 * +------+------------+-----------+-----------+-----------+-----------+
 * |      |   Sextets  |     19    |     22    |     4     |  Padding  |
 * |      +------------+-----------+-----------+-----------+-----------+
 * |Base64|  Character |     T     |     W     |     E     |     =     |
 * |      +------------+-----------+-----------+-----------+-----------+
 * |      |   Octets   | 84 (0x54) | 87 (0x57) | 69 (0x45) | 61 (0x3D) |
 * +------+------------+-----------+-----------+-----------+-----------+
 *
 * +------+------------+---------------+---------------+---------------+
 * |      |Text (ASCII)|       M       |               |               |
 * |Source+------------+---------------+               |               |
 * |      |   Octets   |   77 (0x4D)   |               |               |
 * +------+------------+---------------+---------------+---------------+
 * |        Bits       |0|1|0|0|1|1|0|1|0|0|0|0| | | | | | | | | | | | |
 * +------+------------+-----------+-----------+-----------+-----------+
 * |      |   Sextets  |     19    |     16    |  Padding  |  Padding  |
 * |      +------------+-----------+-----------+-----------+-----------+
 * |Base64|  Character |     T     |     Q     |     =     |     =     |
 * |      +------------+-----------+-----------+-----------+-----------+
 * |      |   Octets   | 84 (0x54) | 81 (0x51) | 61 (0x3D) | 61 (0x3D) |
 * +------+------------+-----------+-----------+-----------+-----------+
 */

BYTE B64[64] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/'
};

Memory::string Cryptography::BASE64::encode(const Memory::string &data)
{
	QWORD length = (data.length / 3) + !!(data.length % 3);
	length <<= 2;
	Memory::string output(length);
	QWORD i = 0;
	QWORD position = 0;
	while ((position >> 3) < data.length)
	{
		QWORD available = (data.length << 3) - position;
		QWORD readed = available < 6 ? available : 0;
		BYTE x = (data[position >> 3] << (6 - readed)) & 0x3F;
		if (available >= 6)
		{
			x = 0;
			switch (position & 0x7)
			{
				case 0:
				{
					x = data[position >> 3] >> 2;
					position += 6;
					break;
				}
				case 2:
				{
					x = data[position >> 3] & 0x3F;
					position += 6;
					break;
				}
				case 4:
				{
					x = (data[position >> 3] & 0xF) << 2;
					position += 4;
					x |= data[position >> 3] >> 6;
					position += 2;
					break;
				}
				case 6:
				{
					x = (data[position >> 3] & 3) << 4;
					position += 2;
					x |= data[position >> 3] >> 4;
					position += 4;
					break;
				}
			}
		}
		position += readed;
		output[i++] = B64[x];
	}
	QWORD padding = (3 - (((data.length << 3) % 6) >> 1)) % 3;
	Memory::fill(output.address + i, '=', padding);
	return output;
}
Memory::string Cryptography::BASE64::decode(const Memory::string &data)
{
	if (data.length & 3)
	{
		Memory::violation();
	}

	QWORD length = (data.length >> 2) * 3;
	QWORD append = 0;
	append += (data.length && data[data.length - 1] == '=');
	append += (data.length && data[data.length - 2] == '=');
	length -= append;
	Memory::string output(length);
	QWORD j = 0;
	BYTE temp = 0;
	BYTE pos = 0;
	for (QWORD i = 0; i < data.length - append; i++)
	{
		BYTE c = data[i];
		BYTE x = 0;
		if (c >= 'A' && c <= 'Z')
		{
			x = c - 'A';
		}
		else if (c >= 'a' && c <= 'z')
		{
			x = 26 + (c - 'a');
		}
		else if (c >= '0' && c <= '9')
		{
			x = 52 + (c - '0');
		}
		else if (c == '+' || c == '/')
		{
			x = 62 + (c == '/');
		}
		else
		{
			Memory::violation();
		}
		switch (pos)
		{
			case 0:
			{
				temp = x << 2;
				pos = 6;
				break;
			}
			case 2:
			{
				temp |= x;
				output[j++] = temp;
				temp = 0;
				pos = 0;
				break;
			}
			case 4:
			{
				temp |= x >> 2;
				output[j++] = temp;
				temp = x << 6;
				pos = 2;
				break;
			}
			case 6:
			{
				temp |= x >> 4;
				output[j++] = temp;
				temp = x << 4;
				pos = 4;
				break;
			}
		}
	}
	return output;
}