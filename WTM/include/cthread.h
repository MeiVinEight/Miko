#ifndef CTHREAD_H
#define CTHREAD_H

#include <WTMSpec.h>
#include <WinType.h>

namespace WTM
{
	class thread
	{
		public:
		void *object = nullptr;

		WTMAPI thread(void *);
		WTMAPI thread(const WTM::thread &);
		WTMAPI thread(WTM::thread &&) noexcept;
		WTMAPI virtual ~thread() noexcept;
		WTMAPI WTM::thread &operator=(const WTM::thread &);
		WTMAPI WTM::thread &operator=(WTM::thread &&) noexcept;
		WTMAPI DWORD ID() const;
		WTMAPI Memory::string name() const;
		WTMAPI void name(const Memory::string &) const;
		WTMAPI bool alive() const;
		WTMAPI bool wait(DWORD) const;
		WTMAPI void wait() const;
		WTMAPI DWORD suspend() const;
		WTMAPI DWORD resume() const;
		WTMAPI DWORD code() const;
		WTMAPI static WTM::thread create(DWORD (*)(void *), void *);
		WTMAPI static WTM::thread current();
		WTMAPI static void sleep(DWORD);
	};
}

#endif //CTHREAD_H
