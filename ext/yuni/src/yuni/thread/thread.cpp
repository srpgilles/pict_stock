
#include "../yuni.h"
#include <time.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <cassert>
#ifndef YUNI_OS_WINDOWS
#	ifndef YUNI_OS_HAIKU
#		include <sys/errno.h>
#	endif
#	include <unistd.h>
#	include <sys/time.h>
#else
#	include "../core/system/windows.hdr.h"
#	include <process.h>
#	include "../core/system/gettimeofday.h"
#endif

#include "thread.h"



namespace Yuni
{
namespace Private
{
namespace Thread
{


	# ifndef YUNI_NO_THREAD_SAFE

	/*!
	** \brief This procedure will be run in a separate thread and will run IThread::onExecute()
	*/
	extern "C"  YUNI_THREAD_FNC_RETURN  threadCallbackExecute(void* arg)
	{
		// assert, for the debugger
		assert(arg && "Yuni Thread Internal: invalid argument (pthread callback)");
		if (!arg)
			return 0;

		// Get back our object.
		Yuni::Thread::IThread& thread = *((Yuni::Thread::IThread *) arg);
		assert(!thread.pStarted && "Yuni Thread: The thread is already started");

		# ifndef YUNI_OS_WINDOWS
		// pthread - Adjust cancellation behaviors
		// note : the following option should not be used with C++
		// ::pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
		::pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		# endif

		if (thread.onStarting())
		{
			// onStarting authorized us to continue. So say we are now running.
			{
				Yuni::MutexLocker flagLocker(thread.pInnerFlagMutex);
				thread.pStarted = true;
				// signal the start() method in the parent thread
				thread.pSignalStartup.notify();
			}
			// Launch the code
			do
			{
				if (!thread.onExecute())
					break;
				if (thread.pShouldStop || !thread.pStarted)
					break;
				// Notifying the thread that it has just been paused
				thread.onPause();
				// Waiting for being waked up
				thread.pSignalWakeUp.wait();

				// We have been waked up ! But perhaps we should abort as soon as
				// possible...
				{
					Yuni::MutexLocker flagLocker(thread.pInnerFlagMutex);
					// The signal must be reset for future use
					// However for thread-safety issues, we have to lock the thread itself
					if (thread.pShouldStop || !thread.pStarted)
						break;
				}

				// If we don't have to stop, the signal must be reset for future use
				// But the the thread itself must be locked
				{
					::Yuni::Thread::IThread::ThreadingPolicy threadLocker(thread);
					thread.pSignalWakeUp.reset();
				}
			}
			while (true);

			// The thread has stopped, execute the user's stop handler.
			thread.onStop();

			// We have stopped executing user code, and are exiting.
			// Signal any threads waiting for our termination.
			Yuni::MutexLocker flagLocker(thread.pInnerFlagMutex);
			thread.pShouldStop = true;
			thread.pStarted = false;
		}
		else
		{
			// The startup failed. So, pStarted is left to false.
			// signal the start() method in the parent thread
			Yuni::MutexLocker flagLocker(thread.pInnerFlagMutex);
			thread.pStarted = false;
			thread.pSignalStartup.notify();
		}

		thread.pSignalHaveStopped.notify();
		return 0;
	}


	# endif // ifndef YUNI_NO_THREAD_SAFE



} // namespace Yuni
} // namespace Private
} // namespace Thread





namespace Yuni
{
namespace Thread
{



	IThread::IThread()
		:pStarted(false), pShouldStop(true)
	{
		# ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		pThreadHandle = NULL;
		# endif
		# endif
	}


	IThread::~IThread()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		assert(pStarted == false && "A thread can not be destroyed while being still started");
		# endif
		# endif
		if (pStarted)
		{
			// It is dangerous to call this method here. The VTable might be corrupted
			// and it may produce strange results
			stop();
		}
	}


	Error IThread::start()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		ThreadingPolicy::MutexLocker locker(*this);
		{
			Yuni::MutexLocker flagLocker(pInnerFlagMutex);
			if (pStarted)
			{
				// The thread is already running, bail out.
				// We have to wake it up
				pSignalWakeUp.notify();
				return errNone;
			}
			// We're starting, so we should not stop too soon :)
			pShouldStop = false;
		}

		// Reset the signal
		pSignalStartup.reset();
		pSignalHaveStopped.reset();
		pSignalMustStop.reset();
		pSignalWakeUp.reset();

		# ifdef YUNI_OS_WINDOWS
		pThreadHandle = CreateThread(NULL, 0, Yuni::Private::Thread::threadCallbackExecute,
			this, 0, NULL);
		if (!pThreadHandle)
			return errThreadCreation;
		# else
		// Lock the startup condition before creating the thread,
		// then wait for it. The thread will signal the condition when it
		// successfully have set isRunning _and_ called the triggers.
		// Then we can check the isRunning status and determine if the startup
		// was a success or not.
		if (::pthread_create(&pThreadID, NULL, Yuni::Private::Thread::threadCallbackExecute, this))
			return errThreadCreation;
		# endif

		// Unlock and wait to be signalled by the new thread.
		// This MUST happen.
		pSignalStartup.wait();

		// Checking if the thread has really started
		{
			Yuni::MutexLocker flagLocker(pInnerFlagMutex);
			// The thread may have been aborted by the startup handler.
			if (!pStarted)
				return errAborted;
		}
		return errNone;

		# else // YUNI_NO_THREAD_SAFE
		if (onStarting())
		{
			onExecute();
			onStop();
		}
		return errNone;
		# endif
	}


	Error IThread::stop(unsigned int timeout)
	{
		# ifdef YUNI_NO_THREAD_SAFE
		(void) timeout;
		# endif

		# ifndef YUNI_NO_THREAD_SAFE
		assert(timeout < 2147483648u && "Invalid range for timeout (IThread::stop(timeout))");
		ThreadingPolicy::MutexLocker locker(*this);

		{
			Yuni::MutexLocker flagLocker(pInnerFlagMutex);
			// Check the thread status
			if (!pStarted) // already stopped, nothing to do.
				return errNone;

			// Early indicates that this thread should stop, should it check that value.
			pShouldStop = true;
		}

		pSignalWakeUp.notify();
		pSignalMustStop.notify();
		// Our status
		Error status = errNone;

		if (!pSignalHaveStopped.wait(timeout)) // We timed out.
		{
			// We are out of time, no choice but to kill our thread
			# ifdef YUNI_OS_WINDOWS
			TerminateThread(pThreadHandle, 0);
			# else
			::pthread_cancel(pThreadID);
			# endif

			status = errTimeout;
			// Notify
			onKill();
		}

		// Wait for the thread be completely stopped
		# ifdef YUNI_OS_WINDOWS
		WaitForSingleObject(pThreadHandle, INFINITE);
		pThreadHandle = NULL;
		# else
		::pthread_join(pThreadID, NULL);
		# endif

		// The thread is no longer running, force status to stopped (ie, if we killed it)
		// It is thread-safe, since the thread is not running anymore AND we hold pMutex.
		pInnerFlagMutex.lock();
		pStarted = false;
		pInnerFlagMutex.unlock();

		return status;

		# else // YUNI_NO_THREAD_SAFE
		return errNone;
		# endif
	}



	Error IThread::wait()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		while (errTimeout == wait(604800000u))
			; // infinite wait
		return errNone;

		# else // YUNI_NO_THREAD_SAFE
		return errNone;
		# endif
	}



	Error IThread::wait(unsigned int timeout)
	{
		# ifndef YUNI_NO_THREAD_SAFE
		assert(timeout < 2147483648u && "Invalid range for timeout (IThread::wait(timeout))");
		ThreadingPolicy::MutexLocker locker(*this);
		{
			Yuni::MutexLocker flagLocker(pInnerFlagMutex);
			if (!pStarted) // already stopped, nothing to do.
				return errNone;
		}

		if (!pSignalHaveStopped.wait(timeout)) // We timed out.
			return errTimeout;

		return errNone;

		# else // YUNI_NO_THREAD_SAFE
		(void) timeout;
		return errNone;
		# endif
	}



	void IThread::wakeUp()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		ThreadingPolicy::MutexLocker locker(*this);
		Yuni::MutexLocker flagLocker(pInnerFlagMutex);
		if (pStarted)
			pSignalWakeUp.notify();

		# else // YUNI_NO_THREAD_SAFE
		# endif
	}


	bool IThread::suspend(const unsigned int delay)
	{
		# ifndef YUNI_NO_THREAD_SAFE
		// The thread may have to stop
		{
			Yuni::MutexLocker flagLocker(pInnerFlagMutex);
			if (pShouldStop || !pStarted)
				return true;
		}

		// We should rest for a while...
		if (delay)
		{
			// If the timeout has been reached, the thread can continue
			if (!pSignalMustStop.wait(delay))
				return false;
		}

		Yuni::MutexLocker flagLocker(pInnerFlagMutex);
		return (pShouldStop || !pStarted);

		# else // YUNI_NO_THREAD_SAFE
		(void) delay;
		return false;
		# endif
	}


	void IThread::gracefulStop()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		pMutex.lock();
		pInnerFlagMutex.lock();
		pShouldStop = true;
		pInnerFlagMutex.unlock();
		pSignalWakeUp.notify();
		pSignalMustStop.notify();
		pMutex.unlock();
		# else // YUNI_NO_THREAD_SAFE
		# endif
	}


	Error IThread::restart(unsigned int timeout)
	{
		assert(timeout < 2147483648u && "Invalid range for timeout (IThread::restart(timeout))");
		const Error status = stop(timeout);
		return (status != errNone) ? status : start();
	}




} // namespace Thread
} // namespace Yuni

