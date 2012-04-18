
#include "signal.h"
#include <cassert>

#ifndef YUNI_NO_THREAD_SAFE
# ifdef YUNI_OS_WINDOWS
#	include "../core/system/windows.hdr.h"
# else
#	include <time.h>
#	include <sys/time.h>
#	include <errno.h>
#	include "../core/system/gettimeofday.h"
# endif
#endif


namespace Yuni
{
namespace Thread
{


	Signal::Signal()
	{
		#ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		// Making sure that our pseudo HANDLE type is valid
		assert(sizeof(HANDLE) >= sizeof(void*) && "Invalid type for Signal::pHandle");

		pHandle = (void*) CreateEvent(
				NULL,     // default security attributes
				TRUE,     // manual-reset event
				FALSE,    // initial state is nonsignaled
				NULL );   // unamed

		# else
		pSignalled = false;
		::pthread_mutex_init(&pMutex, nullptr);
		::pthread_cond_init(&pCondition, nullptr);
		# endif
		#endif
	}


	Signal::~Signal()
	{
		#ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		CloseHandle(pHandle);
		# else
		::pthread_cond_destroy(&pCondition);
		::pthread_mutex_destroy(&pMutex);
		# endif
		#endif
	}



	bool Signal::reset()
	{
		#ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		return (pHandle && ResetEvent(pHandle));
		# else

		::pthread_mutex_lock(&pMutex);
		pSignalled = false;
		::pthread_mutex_unlock(&pMutex);
		return true;
		# endif

		#else // NO THREADSAFE
		return true;
		#endif
	}


	void Signal::wait()
	{
		#ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS

		if (pHandle)
			WaitForSingleObject(pHandle, INFINITE);

		# else

		// The pthread_cond_wait will unlock the mutex and wait for
		// signalling.
		::pthread_mutex_lock(&pMutex);

		while (!pSignalled)
		{
			// Wait for signal
			// Note that the pthread_cond_wait routine will automatically and
			// atomically unlock mutex while it waits.
			//
			// Spurious wakeups from this function can occur.
			// Therefore we must check out pSignalled variable to ensure we have
			// really been signalled.
			::pthread_cond_wait(&pCondition, &pMutex);
		}

		// The condition was signalled: the mutex is now locked again.
		::pthread_mutex_unlock(&pMutex);
		# endif

		# else // NO THREADSAFE
		# endif
	}


	bool Signal::wait(unsigned int timeout)
	{
		#ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		if (pHandle)
		{
			if (WAIT_OBJECT_0 == WaitForSingleObject(pHandle, static_cast<DWORD>(timeout)))
				return true;
		}
		return false;
		# else

		::pthread_mutex_lock(&pMutex);
		if (pSignalled)
		{
			::pthread_mutex_unlock(&pMutex);
			return true;
		}

		Yuni::timeval now;
		struct timespec t;

		// Set the timespec t at [timeout] milliseconds in the future.
		assert(timeout < 2147483648u && "Invalid range for timeout (Signal::wait(timeout))");
		YUNI_SYSTEM_GETTIMEOFDAY(&now, NULL);
		t.tv_nsec  =  static_cast<long>  (now.tv_usec * 1000 + ((static_cast<int>(timeout) % 1000) * 1000000));
		t.tv_sec   =  static_cast<time_t>(now.tv_sec + timeout / 1000 + (t.tv_nsec / 1000000000L));
		t.tv_nsec  %= 1000000000L;

		int error = 0;

		do
		{
			// Wait for signal
			// Note that the pthread_cond_wait routine will automatically and
			// atomically unlock mutex while it waits.
			//
			// Avoid spurious wakeups (see wait() above for explanations)
			error = ::pthread_cond_timedwait(&pCondition, &pMutex, &t);
		}
		while (!pSignalled        // Condition not verified
			&& error != ETIMEDOUT // We have not timedout
			&& error != EINVAL);   // When t is in the past, we got EINVAL. We consider this as a timeout.

		bool result = (pSignalled != false);
		::pthread_mutex_unlock(&pMutex);
		// The condition was signalled or has timeoutted:
		return result;
		# endif

		# else // NO THREADSAFE
		return true;
		# endif
	}


	bool Signal::notify()
	{
		#ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS

		return (pHandle && SetEvent(pHandle));

		# else

		::pthread_mutex_lock(&pMutex);
		pSignalled = true;
		::pthread_cond_signal(&pCondition);
		::pthread_mutex_unlock(&pMutex);
		return true;

		# endif

		# else // NO THREADSAFE
		return true;
		# endif
	}





} // namespace Thread
} // namespace Yuni
