#include "definitions.h"
BYTE HEX[17] = {"0123456789abcdef"};
BYTE ESCAPE[256] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	'b' , 't' , 'n' , 0x00, 'f' , 'r' , 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, '\"', 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, '\\', 0x00, 0x00, 0x00,
};

BYTE UTF8W(QWORD x)
{
	BYTE ret = 0;
	while (x)
	{
		x >>= 1;
		ret++;
	}
	if (ret <= 7) return 1;
	else if (ret <= 11) return 2;
	else if (ret <= 16) return 3;
	else if (ret <= 21) return 4;
	else return 0;
}
void Whitespace(const String::string &str, QWORD &pos)
{
	for (; pos < str.length && str[pos] <= 0x20; pos++);
}
String::string ReadEscapeString(const String::string &str, QWORD &pos)
{
	QWORD capacity = 16;
	BYTE *buffer = new BYTE[capacity];
	QWORD len = 0;
	if (str[pos] != '\"') throw Exception::exception("Expected string"); // TODO where
	pos++;
	while (true)
	{
		if (str[pos] == '\"')
		{
			pos++;
			break;
		}
		if (len + 1 > capacity)
		{
			BYTE *nbuf = new BYTE[capacity *= 2];
			Memory::copy(nbuf, buffer, len);
			delete[] buffer;
			buffer = nbuf;
		}
		QWORD byt = (BYTE) str[pos++];
		if (byt == '\\')
		{
			// escape
			BYTE escaped = str[pos++];
			switch (escaped)
			{
				case 'b':
				{
					byt = '\b';
					break;
				}
				case 't':
				{
					byt = '\t';
					break;
				}
				case 'n':
				{
					byt = '\n';
					break;
				}
				case 'f':
				{
					byt = '\f';
					break;
				}
				case 'r':
				{
					byt = '\r';
					break;
				}
				case '\"':
				case '\'':
				case '\\':
				{
					byt = escaped;
					break;
				}
				case 'u':
				{
					// \uXXXX
					byt = 0;
					for (QWORD i = 0; i < 4; i++)
					{
						byt <<= 4;
						BYTE c = str[pos + i];
						if (c >= '0' && c <= '9')
						{
							byt |= c - '0';
						}
						else if ((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
						{
							if (c >= 'A' && c <= 'F')
								c += 0x20;
							byt |= 10 + (c - 'a');
						}
						else
						{
							char msg[] = "Unknown unicode: \\u0000";
							Memory::copy(msg + 19, str.address + pos, 4);
							throw Exception::exception(msg);
						}
					}
					pos += 4;
					break;
				}
				default:
				{
					BYTE msg[] = "Invalid escape sequence: \\0";
					msg[26] = escaped;
					throw Exception::exception(msg);
				}
			}
		}
		BYTE size = UTF8W(byt);
		Capacity(buffer, capacity, len + size);

		// UTF-8 encoding
		//  7 bits 0XXXXXXX
		// 11 bits 110XXXXX 10XXXXXX
		// 16 bits 1110XXXX 10XXXXXX 10XXXXXX
		// 21 bits 11110XXX 10XXXXXX 10XXXXXX 10XXXXXX
		BYTE c = (size - 1) ? ((((1 << (size + 1)) - 2) << (7 - size)) | (byt >> (6 * (size - 1)))) : byt;
		buffer[len++] = c;
		while (--size)
		{
			buffer[len++] = 0x80 | ((byt >> (6 * (size - 1))) & 0x3F);
		}
	}
	String::string ret = String::string(buffer, len);
	delete[] buffer;
	return ret;
}
JSON::object resolve(const String::string &str, QWORD &pos)
{
	JSON::object obj;

	Whitespace(str, pos);
	if (pos < str.length)
	{
		switch (str[pos])
		{
			case '{':
			{
				obj.type = JSON::type::COMPONENT;
				pos++;
				Whitespace(str, pos);
				if (str[pos] != '}')
				{
					bool continu = true;
					while (continu)
					{
						Whitespace(str, pos);
						String::string name = ReadEscapeString(str, pos);
						Whitespace(str, pos);
						if (str[pos] != ':')
							throw Exception::exception("Expected \':\'");
						pos++;
						obj[name] = ::resolve(str, pos);
						Whitespace(str, pos);
						BYTE symbol = str[pos++];
						switch (symbol)
						{
							case '}':
								pos--;
								continu = false;
							case ',':
								break;
							default:
								throw Exception::exception("Expected '}'");
						}
					}
				}
				pos++;
				break;
			}
			case '[':
			{
				obj.type = JSON::type::ARRAY;
				pos++;
				Whitespace(str, pos);
				if (str[pos] != ']')
				{
					QWORD i = 0;
					bool continu = true;
					while (continu)
					{
						obj[i++] = ::resolve(str, pos);
						Whitespace(str, pos);
						BYTE symbol = str[pos++];
						switch (symbol)
						{
							case ']':
								pos--;
								continu = false;
							case ',':
								break;
							default:
								throw Exception::exception("Expected ']'");
						}
					}
				}
				pos++;
				break;
			}
			case '\"':
			{
				obj = ReadEscapeString(str, pos);
				break;
			}
			default:
			{
				Whitespace(str, pos);
				bool digit = true;
				QWORD len = 0;
				while (pos + len < str.length)
				{
					if (str[pos + len] == ',' || str[pos + len] == '}' || str[pos + len] == ']' || str[pos + len] <= 0x20)
						break;

					digit &= (str[pos + len] >= '0' && str[pos] <= '9');
					len++;
				}
				if (len == 0) throw Exception::exception("Expected value");
				String::string value = String::string(str.address + pos, len);
				if (value == "null")
				{
					obj.type = JSON::type::UNKNOWN;
				}
				else if (value == "false")
				{
					obj = false;
				}
				else if (value == "true")
				{
					obj = true;
				}
				else
				{
					if (digit)
						obj = String::integer(value);
					else
						obj = String::floating(value);
				}
				pos += len;
			}
		}
	}
	return obj;
}

String::string JSON::stringify(const String::string &str)
{
	QWORD bsize = str.length + 2;
	BYTE *buf = new BYTE[bsize];
	QWORD len = 0;
	buf[len++] = '\"';
	for (QWORD i = 0; i < str.length; i++)
	{
		BYTE byt = str[i];
		if (ESCAPE[byt])
		{
			Capacity(buf, bsize, len + 3);
			buf[len++] = '\\';
			buf[len++] = ESCAPE[byt];
		}
		else if (byt < 0x20)
		{
			Capacity(buf, bsize, len + 7);
			buf[len++] = '\\';
			buf[len++] = 'u';
			buf[len++] = '0';
			buf[len++] = '0';
			buf[len++] = HEX[(byt >> 4) & 0xF];
			buf[len++] = HEX[(byt >> 0) & 0xF];
		}
		else
		{
			Capacity(buf, bsize, len + 2);
			buf[len++] = byt;
		}
	}
	buf[len++] = '\"';
	String::string ret(buf, bsize);
	delete[] buf;
	return ret;
}
String::string JSON::stringify(const JSON::object &obj)
{
	return obj.stringify();
}
JSON::object JSON::resolve(const String::string &str)
{
	QWORD pos = 0;
	return ::resolve(str, pos);
}