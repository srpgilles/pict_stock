#ifndef __YUNI_THREADS_MUTEX_HXX__
# define __YUNI_THREADS_MUTEX_HXX__


namespace Yuni
{

	inline Mutex::Mutex()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		enum
		{
			/*!
			** \brief The spin count for the critical section object
			**
			** On single-processor systems, the spin count is ignored and the critical section
			** spin count is set to 0 (zero). On multiprocessor systems, if the critical section
			** is unavailable, the calling thread spinsdwSpinCount times before performing a
			** wait operation on a semaphore associated with the critical section. If the critical
			** section becomes free during the spin operation, the calling thread avoids the
			** wait operation.
			** \see http://msdn.microsoft.com/en-us/library/ms683476%28v=vs.85%29.aspx
			*/
			spinCount = 3000,
		};
		InitializeCriticalSectionAndSpinCount(&pSection, spinCount);
		# else
		#	ifndef NDEBUG
		pthread_mutexattr_init(&pMutexAttr);
		pthread_mutexattr_settype(&pMutexAttr, PTHREAD_MUTEX_ERRORCHECK);
		::pthread_mutex_init(&pPthreadLock, &pMutexAttr);
		#	else
		::pthread_mutex_init(&pPthreadLock, NULL);
		#	endif
		# endif
		# endif
	}


	/*
	inline Mutex::Mutex(const bool recursive)
	{
		# ifndef YUNI_NO_THREAD_SAFE
		if (recursive)
		{
			::pthread_mutexattr_t mutexattr;
			::pthread_mutexattr_init(&mutexattr);
			# if defined(YUNI_OS_DARWIN) || defined(YUNI_OS_FREEBSD) || defined(YUNI_OS_SOLARIS) || defined(YUNI_OS_SUNOS) || defined(YUNI_OS_HAIKU) || defined(YUNI_OS_CYGWIN)
			::pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
			# else
			::pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE_NP);
			# endif
			::pthread_mutex_init(&pPthreadLock, &mutexattr);
			::pthread_mutexattr_destroy(&mutexattr);
		}
		else
			::pthread_mutex_init(&pPthreadLock, NULL);
		# else
		(void) recursive;
		# endif
	}
	*/


	inline Mutex::~Mutex()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		DeleteCriticalSection(&pSection);
		# else
		pthread_mutex_destroy(&pPthreadLock);
		#	ifndef NDEBUG
		pthread_mutexattr_destroy(&pMutexAttr);
		#	endif
		# endif
		# endif
	}



	inline Mutex::Mutex(const Mutex&)
	{
		// Do nothing on purpose
	}


	inline Mutex& Mutex::operator = (const Mutex&)
	{
		// Do nothing on purpose
		return *this;
	}


	inline void Mutex::lock()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		EnterCriticalSection(&pSection);
		# else
		pthread_mutex_lock(&pPthreadLock);
		# endif
		# endif
	}


	inline void Mutex::unlock()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		LeaveCriticalSection(&pSection);
		# else
		pthread_mutex_unlock(&pPthreadLock);
		# endif
		# endif
	}


	# ifndef YUNI_NO_THREAD_SAFE
	# ifndef YUNI_OS_WINDOWS
	inline pthread_mutex_t& Mutex::pthreadMutex()
	{
		return pPthreadLock;
	}
	# endif
	# endif



	inline MutexLocker::MutexLocker(Mutex& m)
		:pMutex(m)
	{
		m.lock();
	}


	inline MutexLocker::~MutexLocker()
	{
		pMutex.unlock();
	}




} // namespace Yuni

#endif // __YUNI_THREADS_MUTEX_HXX__
