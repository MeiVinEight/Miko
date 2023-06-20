#ifndef CPROCESS_H
#define CPROCESS_H

#include <WTMSpec.h>
#include <WinType.h>

namespace WTM
{
	class process
	{
		public:
		void *object = nullptr;

		WTMAPI process(void *);
		WTMAPI process(const WTM::process &);
		WTMAPI process(WTM::process &&) noexcept;
		WTMAPI virtual ~process();
		WTMAPI WTM::process &operator=(const WTM::process &);
		WTMAPI WTM::process &operator=(WTM::process &&) noexcept;
		WTMAPI void suspend() const;
		WTMAPI void resume() const;
		WTMAPI DWORD ID() const;
		WTMAPI bool alive() const;
		WTMAPI bool wait(DWORD) const;
		WTMAPI void wait() const;
		WTMAPI void privilege(const char *) const;
		WTMAPI WTM::process parent() const;
		WTMAPI Memory::string name() const;
		WTMAPI void terminate(DWORD) const;
		WTMAPI static WTM::process current();
		WTMAPI static WTM::process open(DWORD);
	};
}

#endif //CPROCESS_H
