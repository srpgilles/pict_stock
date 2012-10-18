
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



//! Invalid timeout
#define INVALID_TIMEOUT  2147483648u



#ifndef YUNI_NO_THREAD_SAFE
# ifdef YUNI_OS_WINDOWS

#	define YUNI_WAITING_FOR_NATIVE_THREAD_TO_FINISH \
		do \
		{ \
			if (pThreadHandle) \
			{ \
				WaitForSingleObject(pThreadHandle, INFINITE); \
				pThreadHandle = NULL; \
			} \
		} \
		while (0)

# else

#	define YUNI_WAITING_FOR_NATIVE_THREAD_TO_FINISH \
		do \
		{ \
			if (pThreadIDValid) \
			{ \
				::pthread_join(pThreadID, NULL); \
				pThreadIDValid = false; \
			} \
		} \
		while (0)

# endif

#endif // ifndef YUNI_NO_THREAD_SAFE





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
		# ifndef NDEBUG
		thread.pInnerFlagMutex.lock();
		assert(!thread.pStarted && "Yuni Thread: The thread is already started");
		thread.pInnerFlagMutex.unlock();
		# endif

		# ifndef YUNI_OS_WINDOWS
		// pthread - Adjust cancellation behaviors
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
				if (! thread.onExecute())
					break;
				if (thread.pShouldStop ||  ! thread.pStarted)
					break;

				// Notifying the thread that it has just been paused and waiting for
				// being waked up
				thread.onPause();
				thread.pSignalWakeUp.wait();

				// We have been waked up ! But perhaps we should abort as soon as possible
				// (if waked up from the method "stop" for example)
				{
					Yuni::MutexLocker flagLocker(thread.pInnerFlagMutex);
					// The signal must be reset for future use
					// However for thread-safety issues, we have to lock the thread itself
					if (thread.pShouldStop || !thread.pStarted)
						break;
				}

				// If we don't have to stop, the signal must be reset for future use
				// But the the thread itself must be locked before
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


	IThread::IThread() :
		# ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		pThreadHandle(NULL),
		# else
		// pThreadID((pthread_t) NULL), // no portable value
		pThreadIDValid(false),
		# endif
		# endif
		pStarted(false),
		pShouldStop(true)
	{
		// does nothing, only variable initialization
	}


	IThread::~IThread()
	{
		# ifndef YUNI_NO_THREAD_SAFE // if not threadsafe, stopped for sure
		// We have at least to lock the object itself to avoid race conditions
		// (which the method 'stop' does).
		ThreadingPolicy::MutexLocker locker(*this);
		// However, it is dangerous to call the method "stop" here. The VTable will
		// be corrupted if the virtual methods "onKill" or "onStop" are called
		// (or if these methods call another virtual methods of the current object)
		# ifndef NDEBUG
		pInnerFlagMutex.lock();
		assert(pStarted == false &&
			"A thread must be stopped before being destroyed to avoid corrupted vtable. Please call stop() before");
		pInnerFlagMutex.unlock();
		# endif

		// Anyway we have to stop the native thread in any case. Even if the thread
		// should already been stopped we have to call pthread_join or TerminateThread
		// to avoid memory leaks. Even if stopping the thread from here is dangerous,
		// it would be worst in release mode to not have this behavior.
		stopWL(defaultTimeout);
		# endif
	}



	# ifndef YUNI_NO_THREAD_SAFE
	Error IThread::start()
	{
		ThreadingPolicy::MutexLocker locker(*this);
		{
			Yuni::MutexLocker flagLocker(pInnerFlagMutex);
			if (pStarted)
			{
				// The thread is already running, bail out. But we have to wake it up
				pSignalWakeUp.notify();
				return errNone;
			}
			// We're starting, so we should not stop too soon :)
			pShouldStop = false;
		}

		// It is possible that the native thread is still not really stopped
		YUNI_WAITING_FOR_NATIVE_THREAD_TO_FINISH;

		// Reset the signal
		pSignalStartup.reset();
		pSignalHaveStopped.reset();
		pSignalMustStop.reset();
		pSignalWakeUp.reset();

		# ifdef YUNI_OS_WINDOWS
		pThreadHandle = CreateThread(NULL, 0, Yuni::Private::Thread::threadCallbackExecute,
			this, 0, NULL);
		if (!pThreadHandle)
		# else
		// Lock the startup condition before creating the thread,
		// then wait for it. The thread will signal the condition when it
		// successfully have set isRunning _and_ called the triggers.
		// Then we can check the isRunning status and determine if the startup
		// was a success or not.
		pThreadIDValid = (0 == ::pthread_create(&pThreadID, NULL, Yuni::Private::Thread::threadCallbackExecute, this));
		if (not pThreadIDValid)
		# endif
		{
			Yuni::MutexLocker flagLocker(pInnerFlagMutex);
			pStarted = false;
			return errThreadCreation;
		}

		// Unlock and wait to be signalled by the new thread.
		// This MUST happen.
		pSignalStartup.wait();
		// Checking if the thread has really started
		// The thread may have been aborted by the startup handler.
		Yuni::MutexLocker flagLocker(pInnerFlagMutex);
		return (!pStarted) ? errAborted : errNone;
	}
	# endif


	# ifdef YUNI_NO_THREAD_SAFE
	Error IThread::start()
	{
		if (onStarting())
		{
			onExecute();
			onStop();
		}
		return errNone;
	}


	Error IThread::stop(uint timeout)
	{
		assert(timeout < INVALID_TIMEOUT && "Invalid range for timeout, IThread::stop");
		(void) timeout; // unused
		return errNone;
	}
	# endif




	# ifndef YUNI_NO_THREAD_SAFE
	Error IThread::stop(uint timeout)
	{
		assert(timeout < INVALID_TIMEOUT && "Invalid range for timeout, IThread::stop");

		ThreadingPolicy::MutexLocker locker(*this);
		return stopWL(timeout);
	}


	Error IThread::stopWL(uint timeout)
	{
		// Checking the thread status
		{
			Yuni::MutexLocker flagLocker(pInnerFlagMutex);
			if (!pStarted)
			{
				// Already marked as stopped. The native thread should really be finished
				// soon if not already the case
				YUNI_WAITING_FOR_NATIVE_THREAD_TO_FINISH;
				return errNone;
			}

			// Early indicates that this thread should stop, should it check that value.
			pShouldStop = true;
		}

		pSignalWakeUp.notify();
		pSignalMustStop.notify();

		if (!pSignalHaveStopped.wait(timeout)) // We timed out.
		{
			// We are out of time, no choice but to kill our thread
			# ifdef YUNI_OS_WINDOWS
			if (pThreadHandle)
				TerminateThread(pThreadHandle, 0);
			# else
			if (pThreadIDValid)
				::pthread_cancel(pThreadID);
			# endif

			// Stopping the native thread - we should call onKill after that the
			// thread is really stopped
			YUNI_WAITING_FOR_NATIVE_THREAD_TO_FINISH;

			// The thread is no longer running
			pInnerFlagMutex.lock();
			pStarted = false;
			pInnerFlagMutex.unlock();

			// Notifying the user that the thread has been brutally killed
			onKill();

			return errTimeout;
		}
		else
		{
			YUNI_WAITING_FOR_NATIVE_THREAD_TO_FINISH;

			// The thread is no longer running
			pInnerFlagMutex.lock();
			pStarted = false;
			pInnerFlagMutex.unlock();

			return errNone;
		}
	}
	# endif



	Error IThread::wait()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		while (errTimeout == wait(604800000u))
		{
			// infinite wait
		}
		# endif
		return errNone;
	}



	Error IThread::wait(uint timeout)
	{
		assert(timeout < INVALID_TIMEOUT && "Invalid range for timeout, IThread::wait");

		# ifndef YUNI_NO_THREAD_SAFE
		ThreadingPolicy::MutexLocker locker(*this);
		{
			Yuni::MutexLocker flagLocker(pInnerFlagMutex);
			if (!pStarted) // already stopped, nothing to do.
				return errNone;
		}

		if (!pSignalHaveStopped.wait(timeout)) // We timed out.
			return errTimeout;

		# else // YUNI_NO_THREAD_SAFE
		(void) timeout;
		# endif

		return errNone;
	}


	void IThread::wakeUp()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		ThreadingPolicy::MutexLocker locker(*this);
		Yuni::MutexLocker flagLocker(pInnerFlagMutex);
		if (pStarted)
			pSignalWakeUp.notify();
		# endif
	}


	bool IThread::suspend(uint delay)
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
		(void) delay; // unused
		return false;
		# endif
	}


	void IThread::gracefulStop()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		pMutex.lock();
		{
			pInnerFlagMutex.lock();
			pShouldStop = true;
			pInnerFlagMutex.unlock();

			pSignalWakeUp.notify();
			pSignalMustStop.notify();
		}
		pMutex.unlock();
		# endif
	}


	Error IThread::restart(uint timeout)
	{
		assert(timeout < INVALID_TIMEOUT && "Invalid range for timeout, IThread::restart");

		Error status = stop(timeout);
		return (status != errNone) ? status : start();
	}





} // namespace Thread
} // namespace Yuni

