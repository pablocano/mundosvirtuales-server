#pragma once

#include <atomic>
#include <thread>

class Runnable
{
public:
    Runnable(bool isDetach = false) : m_stop(), m_thread(), m_isDetach(isDetach) { }
    virtual ~Runnable() { try { stop(); } catch(...) { /*??*/ } }

    Runnable(Runnable const&) = delete;
    Runnable& operator =(Runnable const&) = delete;

    void stop() { m_stop = true; if (!m_isDetach) m_thread.join(); }
	void start() { m_thread = std::thread(&Runnable::run, this); if (m_isDetach) m_thread.detach(); }

protected:
    virtual void run() = 0;
    std::atomic<bool> m_stop;
	std::atomic<bool> m_isDetach;

private:
    std::thread m_thread;
};
