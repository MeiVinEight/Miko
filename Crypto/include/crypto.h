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
		static const BYTE CRC64             = 0x18; // Hashtab's CRC64 in OpenHashTab is CRC64-WE
		static const BYTE CRC64_ISO         = 0x19;
		static const BYTE CRC64_ECMA        = 0x1A;
		const BYTE type = 0;
		QWORD crc = 0;

		CRYPTOAPI CRC(BYTE);
		CRYPTOAPI void update(const void *, QWORD);
		CRYPTOAPI QWORD value() const;
	};
	class MessageDigest
	{
		protected:
		Memory::string block;
		QWORD position = 0;
		Memory::string digest;
		QWORD length = 0;
		void (*computation)(BYTE *, BYTE *);
		public:
		static const DWORD BLOCK_SIZE_32 = 64;
		static const DWORD BLOCK_SIZE_64 = 128;
		CRYPTOAPI MessageDigest(QWORD, QWORD, void (*)(BYTE *, BYTE *));
		CRYPTOAPI virtual bool appendix(Memory::string &, QWORD &) = 0;
		CRYPTOAPI virtual void transform(Memory::string &) = 0;
		CRYPTOAPI virtual void update(const void *, QWORD);
		CRYPTOAPI virtual Memory::string value();

	};
	class MD4: public MessageDigest
	{
		public:
		CRYPTOAPI MD4();
		CRYPTOAPI bool appendix(Memory::string &, QWORD &) override;
		CRYPTOAPI void transform(Memory::string &) override;
	};
	class MD5: public MessageDigest
	{
		public:
		CRYPTOAPI MD5();
		CRYPTOAPI bool appendix(Memory::string &, QWORD &) override;
		CRYPTOAPI void transform(Memory::string &) override;
	};
	class SHA1: public MessageDigest
	{
		public:
		CRYPTOAPI SHA1();
		CRYPTOAPI bool appendix(Memory::string &, QWORD &) override;
		CRYPTOAPI void transform(Memory::string &) override;
	};
}

#endif //CRYPTO_H