#ifndef __m_queue_h__
#define __m_queue_h__

#include <queue>
#include "platform_threading.h"

template <class T>
class m_queue:private std::queue<T>
{
private:
	mutable std::mutex mutex;
public:
	bool empty() const
	{
	    std::lock_guard<std::mutex> guard(this->mutex);
		return std::queue<T>::empty();
	}
	T pop()
	{
        std::lock_guard<std::mutex> guard(this->mutex);
		T r=std::queue<T>::front();
		std::queue<T>::pop();
		return r;
	}
	void push(const T& a)
	{
        std::lock_guard<std::mutex> guard(this->mutex);
		std::queue<T>::push(a);
	}
	size_t size() const
	{
        std::lock_guard<std::mutex> guard(this->mutex);
		return std::queue<T>::size();
	}
};


#endif
