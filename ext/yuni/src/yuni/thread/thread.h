#ifndef __YUNI_THREAD_THREAD_H__
# define __YUNI_THREAD_THREAD_H__

# include "../yuni.h"
# include "mutex.h"
# include "signal.h"
# include "../core/string.h"
# include <vector>
# include "fwd.h"



namespace Yuni
{
namespace Thread
{

	/*!
	** \brief Base class interface for Threads (abstract)
	**
	** Example for implementing your own thread :
	** \code
	** #include <yuni/yuni.h>
	** #include <yuni/thread/thread.h>
	**
	**
	** class MyThread : public Yuni::Thread::IThread
	** {
	** public:
	**	MyThread() {}
	**	virtual ~MyThread() {}
	**
	** protected:
	**	virtual bool onExecute()
	**	{
	**		// do some time-consumming work here
	**		// ...
	**		// from time to time, you should check if the work should not be stopped
	**		if (shouldAbort())
	**			return false;
	**
	**		// continuing our long task
	**		// ...
	**
	**		// the work is done
	**		return true;
	**	}
	** };
	** \endcode
	**
	**
	**
	** \internal The thread is really created when started (with the method start()), and
	**   destroyed when stopped by the method stop() (or when the object is
	**   destroyed too).
	**
	** \warning : Windows Server 2003 and Windows XP:  The target thread's initial
	**   stack is not freed when stopping the native thread, causing a resource leak
	*/
	class YUNI_DECL IThread : public Policy::ObjectLevelLockable<IThread>
	{
	public:
		//! The threading policy
		typedef Policy::ObjectLevelLockable<IThread>  ThreadingPolicy;
		//! The most suitable smart pointer for the class
		typedef SmartPtr<IThread> Ptr;

	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default constructor
		*/
		IThread();
		/*!
		** \brief Destructor
		*/
		virtual ~IThread();
		//@}


		//! \name Execution flow
		//@{
		/*!
		** \brief Start the execution of the thread, if not already started
		**
		** If already started, the thread will be woke up
		** \return True if the thread has been started
		*/
		Error start();

		/*!
		** \brief Stop the execution of the thread and wait for it, if not already stopped
		**
		** \param timeout The timeout in milliseconds before killing the thread (default: 5000ms)
		** \return An error status (`errNone` if succeeded)
		*/
		Error stop(uint timeout = defaultTimeout);

		/*!
		** \brief Wait for an infinite amount of time for the end of the thread
		**
		** The thread is not stopped during the process.
		** \return An error status (`errNone` if succeeded)
		*/
		Error wait();

		/*!
		** \brief Wait for the end of the thread
		**
		** The thread is not stopped during the process.
		** \param timeout The timeout in milliseconds
		** \return An error status (`errNone` if succeeded)
		*/
		Error wait(uint timeout);

		/*!
		** \brief Restart the thread
		**
		** \param timeout The timeout in milliseconds before killing the thread (default: 5000ms)
		** \return True if the thread has been stopped then started
		**
		** \see stop()
		** \see start()
		*/
		Error restart(uint timeout = defaultTimeout);

		/*!
		** \brief Ask to Stop the execution of the thread as soon as possible
		**
		** After a call to this method, the method suspend() will return true,
		** which indicates that the thread should stop.
		** \see suspend()
		*/
		void gracefulStop();

		/*!
		** \brief Get if the thread is currently running
		** \return True if the thread is running
		*/
		bool started() const;

		/*!
		** \brief Interrupt the thread if suspended
		**
		** A call to this methid will interrupt a suspended state.
		** This method has no effect if the thread is not started.
		*/
		void wakeUp();
		//@}


		//! \name Operators
		//@{
		//! Get if the thread is currently stopped
		bool operator ! () const;
		//@}


	protected:
		/*!
		** \brief Suspend the execution of the thread of X miliseconds
		**
		** This is a convenient method to know if the thread should stop as soon as possible.
		** If you do not want to suspend the execution of thread, you should use the
		** method `shouldAbort()` instead.
		**
		** \attention This method must only be called inside the execution of the thread
		**
		** \param delay The delay in miliseconds. O will only return if the thread should exit
		** \return True indicates that the thread should stop immediately
		*/
		bool suspend(uint delay = 0);

		/*!
		** \brief Get if the thread should abort as soon as possible
		**
		** This is a convenient routine instead of `suspend(0)`, but a bit faster.
		** \attention This method must only be called inside the execution of the thread
		** \return True indicates that the thread should stop immediately
		*/
		bool shouldAbort();

		/*!
		** \brief Event: The thread has just been started
		**
		** This event is executed in the thread which has just been created.
		**
		** It can be directly stopped if returning false. However the `onStopped` event
		** will not be called.
		**
		** \return True to continue the execution of the thread, false to abort the
		** execution right now
		*/
		virtual bool onStarting() {return true;}

		/*!
		** \brief Event: The thread is running
		**
		** The thread has been successfully started (and `onStarting()` returned true).
		**
		** If this method returns false, the thread will be stopped (and the thread
		** context will be destroyed). The method `onStopped` will be called.
		** Otherwise, the thread will be paused for an infinite amount of time and the
		** method onPause() will be called. In this case it will be possible to re-run it
		** in calling wakeUp() or start().
		**
		** \attention This method should check from time to time if the thread has to stop. For that,
		** a call to `suspend(0)` is recommended.
		** \code
		**    if (suspend())
		**       return;
		** \endcode
		**
		** \return True to pause the thread and wait a `wake up` signal, False to effectively stop it.
		** \see suspend()
		** \see onStarting()
		** \see onStopped()
		** \see wakeUp()
		*/
		virtual bool onExecute() = 0;

		/*!
		** \brief Event: The thread has finished its job and is waiting for being re-executed
		**
		** This event will be fired if the method onExecute returns false.
		*/
		virtual void onPause() {}

		/*!
		** \brief Event: The thread has been gracefully stopped
		**
		** This event is executed in the thread.
		**
		** \attention You should not rely on this event to release your resources. There is no guaranty
		** that this method will be called, especially if the thread has been killed because
		** it did not stop before the timeout was reached.
		*/
		virtual void onStop() {}

		/*!
		** \brief Event: The thread has been killed
		**
		** This method might be called from any thread
		*/
		virtual void onKill() {}


	private:
		//! Private copy constructor
		IThread(const IThread&) :ThreadingPolicy() {/* not copyable */}
		//! Operator =
		IThread& operator = (const IThread&) {return *this;}

		# ifndef YUNI_NO_THREAD_SAFE
		//! Stop the native thread
		Error stopWL(uint timeout);
		# endif

	private:
		# ifndef YUNI_NO_THREAD_SAFE
		//! Mutex for starting up, see start()
		Signal pSignalStartup;
		//! Signal for stopping
		Signal pSignalHaveStopped;
		//! The thread must stop as soon as possible
		Signal pSignalMustStop;
		//! The thread can wake up
		Signal pSignalWakeUp;
		//! Mutex for protecting pShouldStop and pStarted
		Mutex pInnerFlagMutex;

		# ifdef YUNI_OS_WINDOWS
		void* pThreadHandle;
		# else
		//! ID of the thread, for pthread
		pthread_t pThreadID;
		//! Flag to determine whether pThreadID is valid or not
		// There is no portable value to determine if pThreadID is valid or not
		// We have to use a separate flag
		bool pThreadIDValid;
		# endif // YUNI_OS_WINDOWS
		# endif // YUNI_NO_THREAD_SAFE

		//! Get if the thread is running
		volatile bool pStarted;
		//! Should stop the thread ?
		volatile bool pShouldStop;
		//! Get if the thread is paused
		Atomic::Int<32> pPaused;

		# ifndef YUNI_NO_THREAD_SAFE
		// our friend
		friend class Yuni::Job::IJob;
		friend YUNI_THREAD_FNC_RETURN Yuni::Private::Thread::threadCallbackExecute(void* arg);
		# endif

	}; // class IThread





} // namespace Thread
} // namespace Yuni

# include "thread.hxx"

#endif // __YUNI_THREAD_THREAD_H__
