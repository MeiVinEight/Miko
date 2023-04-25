#ifndef WTM_H
#define WTM_H

#ifdef WTM_SHARED
	#define WTMAPI __declspec(dllexport)
#else
	#define WTMAPI __declspec(dllimport)
#endif

#include <memory.h>

namespace WTM
{
	class thread
	{
		public:
		void *object = nullptr;

		WTMAPI thread(void *);
		WTMAPI thread(const WTM::thread &);
		WTMAPI thread(WTM::thread &&) noexcept;
		WTMAPI ~thread() noexcept;
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

#endif //WTM_H
