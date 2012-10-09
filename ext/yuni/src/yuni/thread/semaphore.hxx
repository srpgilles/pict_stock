#ifndef __YUNI_THREADS_SEMAPHORE_HXX__
# define __YUNI_THREADS_SEMAPHORE_HXX__


namespace Yuni
{

	inline uint Semaphore::maxReaders() const
	{
		return pMaxReaders;
	}


	inline void Semaphore::acquire()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		# else
		#	ifdef YUNI_OS_MAC
		sem_wait(pSemaphore);
		#	else
		sem_wait(& pSemaphore);
		#	endif
		# endif
		# endif
	}


	inline void Semaphore::acquire(uint n)
	{
		# ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		# else
		#	ifdef YUNI_OS_MAC
		for (uint i = 0; i != n; ++i)
			sem_wait(pSemaphore);
		#	else
		for (uint i = 0; i != n; ++i)
			sem_wait(& pSemaphore);
		#	endif
		# endif
		# endif
	}


	inline void Semaphore::release()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		# else
		#	ifdef YUNI_OS_MAC
		sem_post(pSemaphore);
		#	else
		sem_post(& pSemaphore);
		#	endif
		# endif
		# endif
	}


	inline void Semaphore::release(uint n)
	{
		# ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		# else
		#	ifdef YUNI_OS_MAC
		for (uint i = 0; i != n; ++i)
			sem_post(pSemaphore);
		#	else
		for (uint i = 0; i != n; ++i)
			sem_post(& pSemaphore);
		#	endif
		# endif
		# endif
	}


	inline Semaphore& Semaphore::operator -- ()
	{
		acquire();
		return *this;
	}


	inline Semaphore& Semaphore::operator ++ ()
	{
		release();
		return *this;
	}





} // namespace Yuni

#endif // __YUNI_THREADS_SEMAPHORE_HXX__

