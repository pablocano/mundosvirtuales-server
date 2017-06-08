#pragma once

#include <list>
#include <thread>
#include <mutex>
#include <condition_variable> 

template <typename T> class Queue
{
protected:
	std::list<T> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_condv;

public:
	Queue();

	void add(T item);

	T remove();

	void remove(T item);

	int size();
};

template<typename T>
Queue<T>::Queue() : m_mutex(), m_condv()
{
}

template<typename T>
void Queue<T>::add(T item)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_queue.push_back(item);
	m_condv.notify_one();
}

template<typename T>
T Queue<T>::remove()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	while (m_queue.size() == 0)
	{
		m_condv.wait(lock);
	}
	T item = m_queue.front();
	m_queue.pop_front();
	return item;
}

template<typename T>
void Queue<T>::remove(T item)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	while (m_queue.size() == 0)
	{
		m_condv.wait(lock);
	}
	m_queue.remove(item);
}

template<typename T>
int Queue<T>::size()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	return (int) m_queue.size();
}