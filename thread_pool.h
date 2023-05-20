#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <atomic>
#include "atomic_queue.hpp"
#include <functional>
#include <vector>
#include <thread>


/**
 * Thread Pool which starts as soon as you create it
 * and becomes useless as soon as you stop it.
 * Stops and joins all threads on destruction.
*/
class OneOffThreadPool
{
private:
	std::atomic_bool isRunning;
	std::atomic_int stoppedThreadCount;
	int craetedThreadCount;
	std::atomic_bool wereThreadsJoined;
	AtomicQueue<std::function<void()>> queue;
	std::vector<std::thread> workers;

	void worker();

public:
	/**
	 * @param  _threadCount  Desired number of threads. If less than or equal to 0 (zero), tries to refer to hardware_concurrency hint.
	*/
	OneOffThreadPool(int _threadCount = 0);

	~OneOffThreadPool();

	/**  @return  The number of functions in the %queue.  */
	std::size_t getQueueSize();

	/**  @return  True if the %queue is empty.  */
	bool emptyQueue();

	/**  @return  The number of created threads.  */
	std::size_t getCreatedThreadCount();

	/**
	 * Stops every thread as soon as it finishes its current task
	 * and calls every thread's join() method if that wasn't done before.
	*/
	void declareStopAndJoin();

	/**
	 * Commands every thread to stop as soon as it finishes its current task.
	 * @note  Doesn't call threads' join() method.
	*/
	void declareStop();

	/**
	 * Calls every thread's join() method if that wasn't done before.
	 * @warning  Will frezze this_thread until all threads are stopped.
	 *           Use wisely or call declareStopAndJoin() method instead.
	*/
	void join();

	/**  @return  The number of stopped threads.  */
	std::size_t getStoppedThreadsCount();

	/**  @return  True if stop command was already given.  */
	bool stopDeclared();

	/**  @return  True if all threads were stopped.  */
	bool fullyStopped();

	/**
	 * Pushes a function to the %queue and notifies waiting threads.
	 * @note  Does nothing if stop command was already given.
	 * @param  _lambda  A function in form of lambda expression.
	*/
	void submit(std::function<void ()> _lambda);
};





#endif //THREAD_POOL_H