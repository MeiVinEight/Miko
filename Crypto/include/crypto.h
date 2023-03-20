#ifndef CRYPTO_H
#define CRYPTO_H

#ifdef CRYPTO_SHARED
	#define CRYPTOAPI __declspec(dllexport)
#else
	#define CRYPTOAPI __declspec(dllimport)
#endif

#include <memory.h>

namespace Crypto
{
	namespace BASE64
	{
		CRYPTOAPI Memory::string encode(const Memory::string &);
		CRYPTOAPI Memory::string decode(const Memory::string &);
	}
	class CRC
	{
		public:
		static const BYTE CRC4_ITU          = 0x00;
		static const BYTE CRC5_EPC          = 0x01;
		static const BYTE CRC5_ITU          = 0x02;
		static const BYTE CRC5_USB          = 0x03;
		static const BYTE CRC6_ITU          = 0x04;
		static const BYTE CRC7_MMC          = 0x05;
		static const BYTE CRC8              = 0x06;
		static const BYTE CRC8_ITU          = 0x07;
		static const BYTE CRC8_ROCH         = 0x08;
		static const BYTE CRC8_MAXIM        = 0x09;
		static const BYTE CRC16_IBM         = 0x0A;
		static const BYTE CRC16_MAXIM       = 0x0B;
		static const BYTE CRC16_USB         = 0x0C;
		static const BYTE CRC16_MODBUS      = 0x0D;
		static const BYTE CRC16_CCITT       = 0x0E;
		static const BYTE CRC16_CCITT_FALSE = 0x0F;
		static const BYTE CRC16_X25         = 0x10;
		static const BYTE CRC16_XMODEM      = 0x11;
		static const BYTE CRC16_XMODEM2     = 0x12;
		static const BYTE CRC16_DNP         = 0x13;
		static const BYTE CRC32             = 0x14;
		static const BYTE CRC32_C           = 0x15;
		static const BYTE CRC32_KOOPMAN     = 0x16;
		static const BYTE CRC32_MPEG_2      = 0x17;
		static const BYTE CRC64_ISO         = 0x18;
		static const BYTE CRC64_ECMA        = 0x19;
		const BYTE type = 0;
		QWORD crc = 0;

		CRYPTOAPI CRC(BYTE);
		CRYPTOAPI void update(const void *, QWORD);
		CRYPTOAPI QWORD value() const;
	};
	class MD4
	{
		private:
		DWORD ABCD[4] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476};
		BYTE block[64] = {0};
		QWORD position = 0;
		QWORD length = 0;
		public:
		CRYPTOAPI void update(const void *, QWORD);
		CRYPTOAPI Memory::string value() const;
	};
	class MD5
	{
		private:
		DWORD ABCD[4] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476};
		BYTE block[64] = {0};
		QWORD position = 0;
		QWORD length = 0;
		public:
		CRYPTOAPI void update(const void *, QWORD);
		CRYPTOAPI Memory::string value() const;
	};
}

#endif //CRYPTO_H