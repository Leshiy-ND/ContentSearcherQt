#include "thread_pool.h"



void OneOffThreadPool::worker()
{
	while (isRunning)
	{
		std::function<void()> task;
		if (queue.try_pop(task))
		{
			task();
		}
		else
		{
			std::this_thread::yield();
		}
	}
	stoppedThreadCount++;
	std::this_thread::yield();
}

OneOffThreadPool::OneOffThreadPool(int _threadCount) : isRunning(true), craetedThreadCount(_threadCount), stoppedThreadCount(0), wereThreadsJoined(false)
{
	try
	{
		if (craetedThreadCount <= 0)
		{
			craetedThreadCount = std::thread::hardware_concurrency();
			if (craetedThreadCount == 0) throw std::logic_error("hardware_concurrency hint gived 0");
		}

		for (size_t i = 0; i < craetedThreadCount; i++)
		{
			workers.push_back(std::thread(&OneOffThreadPool::worker, this));
		}
	}
	catch(...)
	{
		declareStopAndJoin();
		workers.clear();
		throw;
	}
}

OneOffThreadPool::~OneOffThreadPool()
{
	declareStopAndJoin();
}

std::size_t OneOffThreadPool::getQueueSize()
{
	return queue.size();
}

bool OneOffThreadPool::emptyQueue()
{
	return queue.empty();
}

std::size_t OneOffThreadPool::getCreatedThreadCount()
{
	return craetedThreadCount;
}

void OneOffThreadPool::declareStopAndJoin()
{
	declareStop();
	join();
}

void OneOffThreadPool::declareStop()
{
	isRunning = false;
}

void OneOffThreadPool::join()
{
	if (wereThreadsJoined) return;
	wereThreadsJoined = true;
	for (auto &&thread : workers) thread.join();
	workers.clear();
}

std::size_t OneOffThreadPool::getStoppedThreadsCount()
{
	return stoppedThreadCount;
}

bool OneOffThreadPool::stopDeclared()
{
	return !isRunning;
}

bool OneOffThreadPool::fullyStopped()
{
	return (craetedThreadCount == stoppedThreadCount);
}

void OneOffThreadPool::submit(std::function<void ()> _lambda)
{
	if (!isRunning) return;
	queue.push(_lambda);
}
