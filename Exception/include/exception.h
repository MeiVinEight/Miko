#ifndef EXCEPTION_H
#define EXCEPTION_H

#ifdef EXCEPTION_SHARED
	#define EXCEPTIONAPI __declspec(dllexport)
#else
	#define EXCEPTIONAPI
#endif

#include <memory.h>

namespace Exception
{
	EXCEPTIONAPI
	Memory::string message(DWORD);

	class exception
	{
		public:
		class frame // Needs PDB file
		{
			public:
			void *address = nullptr;
			void *offset = nullptr;
			void *module = nullptr;
			Memory::string function = Memory::string(0);
			Memory::string library = Memory::string(0);

			frame() = delete;
			EXCEPTIONAPI
			frame(void *);
			EXCEPTIONAPI
			frame(const Exception::exception::frame &);
			EXCEPTIONAPI
			frame(Exception::exception::frame &&) noexcept;
			EXCEPTIONAPI
			~frame();
			EXCEPTIONAPI
			Exception::exception::frame &operator=(const Exception::exception::frame&);
			EXCEPTIONAPI
			Exception::exception::frame &operator=(Exception::exception::frame &&) noexcept;
		};

		Exception::exception::frame *stack = nullptr;
		DWORD count = 0;
		Memory::string message = Memory::string(0);
		EXCEPTIONAPI
		exception(const char *);
		EXCEPTIONAPI
		exception(Memory::string );
		EXCEPTIONAPI
		exception(const Exception::exception &);
		EXCEPTIONAPI
		exception(Exception::exception &&) noexcept;
		EXCEPTIONAPI
		~exception();
		EXCEPTIONAPI
		Exception::exception &operator=(const Exception::exception &);
		EXCEPTIONAPI
		Exception::exception &operator=(Exception::exception &&) noexcept;
	};
}

#endif //EXCEPTION_H
