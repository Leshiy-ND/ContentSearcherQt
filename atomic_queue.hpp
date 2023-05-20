#ifndef ATOMIC_QUEUE_HPP
#define ATOMIC_QUEUE_HPP

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>




/**
* A variant of std::queue with well-defined behavior
* when being modified from multiple threads.
*/
template <typename T>
class AtomicQueue
{
private:
	std::queue<T> queue;
	std::mutex m;
	std::condition_variable condition;

public:
	/**
	 * Pushes an item to the %queue and notifies waiting pop() methods.
	 * Holds other moding methods during execution.
	 * @param  _item  Item to be added.
	*/
	void push(T _item)
	{
		std::lock_guard<std::mutex> lock(m);
		queue.push(_item);
		condition.notify_one();
	}

	/**
	 * Waits if the %queue is empty, and pops an item out of the queue.
	 * Holds other moding methods during execution.
	 * Will freeze an entire thread if used unwisely.
	 * @param  _item  Reference to your item holding variable.
	*/
	void pop(T& _item)
	{
		std::unique_lock<std::mutex> lock(m);
		condition.wait(lock, [this](){ return !queue.empty(); });
		_item = queue.front();
		queue.pop();
	}

	/**
	 * Tries to pops an item out of the %queue if it isn't empty.
	 * Holds other moding methods during execution.
	 * @param  _item  Reference to your item holding variable.
	 * @return  True if successed.
	*/
	bool try_pop(T& _item)
	{
		std::lock_guard<std::mutex> lock(m);
		if (queue.empty()) return false;
		_item = queue.front();
		queue.pop();
		return true;
	}

	/**  @return  The number of elements in the %queue.  */
	std::size_t size()
	{
		return queue.size();
	}

	/** @return  True if the %queue is empty.  */
	bool empty()
	{
		return queue.empty();
	}
};





#endif //ATOMIC_QUEUE_HPP