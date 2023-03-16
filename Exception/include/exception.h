#ifndef EXCEPTION_H
#define EXCEPTION_H

#ifdef EXCEPTION_SHARED
	#define EXCEPTIONAPI __declspec(dllexport)
#else
	#define EXCEPTIONAPI
#endif

#include <memory.h>
#include <sstring.h>

namespace Exception
{
	EXCEPTIONAPI
	String::string message(DWORD);

	class exception
	{
		public:
		class frame // Needs PDB file
		{
			public:
			void *address = nullptr;
			void *offset = nullptr;
			void *module = nullptr;
			String::string function = Memory::string(0);
			String::string library = Memory::string(0);

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
		String::string message;
		EXCEPTIONAPI
		exception(String::string);
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
