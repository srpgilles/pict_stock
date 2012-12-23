
# include "highestpriorityfirst.h"

namespace Yuni
{
namespace Job
{
namespace Scheduler
{


	HighestPriorityFirst::HighestPriorityFirst(Private::QueueService::WaitingRoom& room) :
		pWaitingRoom(room),
		pStarted(0),
		pMaximumThreadCount(2)
	{
	}


	HighestPriorityFirst::~HighestPriorityFirst()
	{
		if (pStarted)
		{
			pStarted = 0;
			pServiceMutex.lock();
			pThreads.stop();
			pServiceMutex.unlock();
		}
	}


	bool HighestPriorityFirst::schedulerStart()
	{
		if (!pStarted)
		{
			pStarted = 1;
			pServiceMutex.lock();

			// Creating all threads we need
			pThreads.clear();
			for (uint i = 0; i != pMaximumThreadCount; ++i)
				pThreads += new Yuni::Private::Jobs::QueueThread<SchedulerType>(*this);

			// Starting all threads at once
			pThreads.start();
			pServiceMutex.unlock();
		}
		return true;
	}


	bool HighestPriorityFirst::schedulerStop(uint timeout)
	{
		if (pStarted)
		{
			pStarted = 0;
			pServiceMutex.lock();
			// Stopping all threads
			pThreads.stop(timeout);
			pThreads.clear();
			pServiceMutex.unlock();
		}
		return true;
	}


	bool HighestPriorityFirst::maximumThreadCount(uint n)
	{
		if (n < 1 || n > 512)
			return false;
		pMaximumThreadCount = n;
		return true;
	}




} // namespace Scheduler
} // namespace Job
} // namespace Yuni

