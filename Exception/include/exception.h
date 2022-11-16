#ifndef EXCEPTION_H
#define EXCEPTION_H

#ifdef EXCEPTION_SHARED
	#define EXCEPTION_API __declspec(dllexport)
#else
	#define EXCEPTION_API
#endif

#include <memory.h>

typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned long		DWORD;

namespace Exception
{
	EXCEPTION_API
	Memory::string message(DWORD);

	class exception
	{
		public:
		class frame // Needs PDB file
		{
			public:
			void *address = 0;
			void *offset = 0;
			void *module = 0;
			char *function = 0;
			char *library = 0;
			char *source = 0;
			DWORD line = 0;

			frame() = delete;
			EXCEPTION_API
			frame(void *);
			EXCEPTION_API
			frame(const Exception::exception::frame &);
			EXCEPTION_API
			frame(Exception::exception::frame &&);
			EXCEPTION_API
			~frame();
			EXCEPTION_API
			Exception::exception::frame &operator=(const Exception::exception::frame&);
			EXCEPTION_API
			Exception::exception::frame &operator=(Exception::exception::frame &&);
		};

		static const BYTE INTERNAL = 0;
		static const BYTE EXTERNAL = 1;
		Exception::exception::frame *stack = 0;
		DWORD count = 0;
		BYTE type = 0;
		WORD value = 0;
		EXCEPTION_API
		exception(BYTE, WORD);
		EXCEPTION_API
		exception();
		EXCEPTION_API
		exception(const Exception::exception &);
		EXCEPTION_API
		exception(Exception::exception &&);
		EXCEPTION_API
		~exception();
		EXCEPTION_API
		Exception::exception &operator=(const Exception::exception &);
		EXCEPTION_API
		Exception::exception &operator=(Exception::exception &&);
	};
}

#endif //EXCEPTION_H
