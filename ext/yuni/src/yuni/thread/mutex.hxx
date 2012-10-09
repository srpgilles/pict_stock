#ifndef __YUNI_THREADS_MUTEX_HXX__
# define __YUNI_THREADS_MUTEX_HXX__

# include <cassert>


namespace Yuni
{




	inline void Mutex::lock()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		EnterCriticalSection(&pSection);
		# else
		::pthread_mutex_lock(&pLock);
		# endif
		# endif
	}


	inline void Mutex::unlock()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		LeaveCriticalSection(&pSection);
		# else
		::pthread_mutex_unlock(&pLock);
		# endif
		# endif
	}


	# ifndef YUNI_NO_THREAD_SAFE
	# ifndef YUNI_OS_WINDOWS
	inline pthread_mutex_t& Mutex::pthreadMutex()
	{
		return pLock;
	}

	inline const pthread_mutex_t& Mutex::pthreadMutex() const
	{
		return pLock;
	}
	# endif
	# endif



	inline MutexLocker::MutexLocker(Mutex& m) :
		pMutex(m)
	{
		m.lock();
	}


	inline MutexLocker::~MutexLocker()
	{
		pMutex.unlock();
	}




} // namespace Yuni

#endif // __YUNI_THREADS_MUTEX_HXX__
