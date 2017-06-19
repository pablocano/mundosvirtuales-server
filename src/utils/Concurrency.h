#include "SystemCall.h"
#include "util.h"

#include <thread>

class Concurrency
{
protected:
	FUN_CALLBACK m_main;
	FUN_CALLBACK m_post;
	unsigned int m_nTimeout;
	std::thread m_thread;
	std::thread m_thMain;

	void run()
	{
		m_thMain = std::thread(m_main);
		
		if (m_nTimeout > 0)
		{
			SystemCall::sleep(m_nTimeout);
			SystemCall::killThread(m_thMain.native_handle());
			SystemCall::joinThread(m_thMain.native_handle());
		}
		else
		{
			m_thMain.join();
		}

		if (m_post)
			m_post();
	}

public:
	Concurrency(FUN_CALLBACK _main, FUN_CALLBACK _post = nullptr, unsigned int nTimeout = 0) :
		m_main(_main), m_post(_post), m_thread(), m_thMain(), m_nTimeout(nTimeout) { }

	~Concurrency()
	{
		
	}

	Concurrency(Concurrency const&) = delete;
	Concurrency& operator =(Concurrency const&) = delete;

	void exec()
	{
		m_thread = std::thread(&Concurrency::run, this);
		m_thread.detach();
	}
};
