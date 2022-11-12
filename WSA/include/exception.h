#ifndef WSA_EXCEPTION_H
#define WSA_EXCEPTION_H

#include "wsadef.h"

namespace WSA
{
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

			frame() = delete;
			WSA_API
			frame(void *);
			WSA_API
			frame(const WSA::exception::frame &);
			WSA_API
			frame(WSA::exception::frame &&);
			WSA_API
			~frame();
			WSA_API
			WSA::exception::frame &operator=(const WSA::exception::frame&);
			WSA_API
			WSA::exception::frame &operator=(WSA::exception::frame &&);
		};

		static const BYTE INTERNAL = 0;
		static const BYTE EXTERNAL = 1;
		WSA::exception::frame *stack = 0;
		DWORD count = 0;
		BYTE type = 0;
		WORD value = 0;
		WSA_API
		exception(BYTE, WORD);
		WSA_API
		exception();
		WSA_API
		exception(const WSA::exception &);
		WSA_API
		exception(WSA::exception &&);
		WSA_API
		~exception();
		WSA_API
		WSA::exception &operator=(const WSA::exception &);
		WSA_API
		WSA::exception &operator=(WSA::exception &&);
	};
}

#endif //WSA_EXCEPTION_H
