#ifndef __YUNI_THREADS_MUTEX_H__
# define __YUNI_THREADS_MUTEX_H__

# include "../yuni.h"
# ifndef YUNI_NO_THREAD_SAFE
#	include "pthread.h"
#	ifdef YUNI_OS_WINDOWS
#		include "../core/system/windows.hdr.h"
# 	endif
# endif



namespace Yuni
{

	/*!
	** \brief  Mechanism to avoid the simultaneous use of a common resource
	**
	** \ingroup Threads
	*/
	class YUNI_DECL Mutex
	{
	public:
		/*!
		** \brief A class-level locking mechanism
		**
		** A class-level locking operation locks all objects in a given class during that operation
		*/
		template<class T>
		class ClassLevelLockable
		{
		public:
			//! A dedicated mutex for the class T
			static Mutex mutex;

		}; // class ClassLevelLockable


	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default constructor, non recursive by default
		*/
		Mutex();
		/*!
		** \brief Default constructor
		*/
		//explicit Mutex(const bool recursive);
		/*!
		** \brief Copy constructor
		**
		** This constructor does actually nothing but it allows the compilation
		** of other classes which would implement a copy constructor
		*/
		Mutex(const Mutex&);
		/*!
		** \brief Destructor
		*/
		~Mutex();
		//@}

		//! \name Lock & Unlock
		//@{
		/*!
		** \brief Lock the mutex
		*/
		void lock();

		/*!
		** \brief Release the lock
		*/
		void unlock();
		//@}

		# ifndef YUNI_NO_THREAD_SAFE
		# ifndef YUNI_OS_WINDOWS
		//! \name PThread wrapper
		//@{
		/*!
		** \brief Get the original PThread mutex
		*/
		::pthread_mutex_t& pthreadMutex();
		//@}
		# endif
		# endif


		//! \name Operators
		//@{
		//! Operator = (do nothing)
		Mutex& operator = (const Mutex&);
		//@}


	private:
		# ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		//! The critical section
		CRITICAL_SECTION pSection;
		# else
		//! The PThread mutex
		::pthread_mutex_t pPthreadLock;
		# ifndef NDEBUG
		pthread_mutexattr_t pMutexAttr;
		# endif
		# endif
		# endif

	}; // class Mutex




	/*!
	** \brief Locks a mutex in the constructor and unlocks it in the destructor (RAII).
	**
	** This class is especially usefull for `get` accessor` and/or returned values
	** which have to be thread-safe.
	** This is a very common C++ idiom, known as "Resource Acquisition Is Initialization" (RAII).
	**
	** \code
	**	  class Foo
	**	  {
	**	  public:
	**		  Foo() : pValue(42) {}
	**		  ~Foo() {}
	**		  int getValue()
	**		  {
	**			  MutexLocker locker(pMutex);
	**			  return pValue;
	**		  }
	**		  void setValue(const int i)
	**		  {
	**			  pMutex.lock();
	**			  pValue = i;
	**			  pMutex.unlock();
	**		  }
	**	  private:
	**		  int pValue;
	**		  Mutex pMutex;
	**	  };
	** \endcode
	*/
	class MutexLocker
	{
	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Constructor
		**
		** \param m The mutex to lock
		*/
		MutexLocker(Mutex& m);
		//! Destructor
		~MutexLocker();
		//@}

	private:
		//! Reference to the real mutex
		Mutex& pMutex;

	}; // MutexLocker




	//! All mutexes for each class
	template<class T> Mutex Mutex::ClassLevelLockable<T>::mutex;




} // namespace Yuni

# include "mutex.hxx"

#endif // __YUNI_THREADS_MUTEX_H__
