#pragma once

class SystemCall
{
public:

  /** returns the current system time in milliseconds*/
  static unsigned getCurrentSystemTime();

  /** returns the real system time in milliseconds (never the simulated one)*/
  static unsigned getRealSystemTime();

  /**
  * The function returns the thread cpu time of the calling thread in microseconds.
  * return thread cpu time of the calling thread
  */
  static unsigned long long getCurrentThreadTime();

  /** returns the time since aTime*/
  inline static int getTimeSince(unsigned aTime)
  {
    return (int)(getCurrentSystemTime() - aTime);
  }

  /** returns the real time since aTime*/
  inline static int getRealTimeSince(unsigned aTime)
  {
    return (int)(getRealSystemTime() - aTime);
  }

  /** returns the name of the local machine*/
  static const char* getHostName();

  /** returns the first ip m_address of the local machine*/
  static const char* getHostAddr();

  /** returns the first ip m_address of the local machine (unsigned int)  */
  static unsigned int getHostAddrInt();

  /** return m_address ip */
  static const char* getAddr(long n);

  /** Sleeps for some milliseconds.
  * \param ms The amout of milliseconds.
  */
  static void sleep(unsigned int ms);

  /** Returns the load and the physical memory usage in percent */
  static void getLoad(float& mem, float load[3]);
};
