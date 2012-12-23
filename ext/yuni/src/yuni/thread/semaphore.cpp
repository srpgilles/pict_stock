
#include "semaphore.h"
#include <cassert>
#include <iostream>
#include "../core/string.h"
#ifdef YUNI_OS_MAC
# include <unistd.h>
#endif
#ifdef YUNI_OS_LINUX
# include <fcntl.h>    // for contants
# include <sys/stat.h>
# include <limits.h>
#endif



namespace Yuni
{

	// inline for keeping the method local
	inline void Semaphore::initialize(uint readers)
	{
		# ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		# else

		// In every cases, for all UNIX operating systems, readers must not be greater than SEM_VALUE_MAX
		if (readers > SEM_VALUE_MAX)
		{
			# ifndef NDEBUG
			std::cerr << "\nYuni::Semaphore: readers > SEM_VALUE_MAX\n";
			# endif
			readers = SEM_VALUE_MAX;
		}

		#	ifdef YUNI_OS_MAC

		// OS X does not support unnamed semaphores
		CString<128,false> name;
		name << "yuni_" << getpid() << '-' << (void*) this;

		pSemaphore = sem_open(name.c_str(), O_CREAT, 0600, readers);
		if (pSemaphore == SEM_FAILED)
		{
			std::cerr << "impossible to create a new semaphore called " << name << std::endl;
			assert(false && "impossible to initialize a semaphore");
			exit(-1);
		}

		#	else

		// UNIX implementation - unamed semaphores
		// However it is not supported by OS X
		if (0 != sem_init(& pSemaphore, 0, readers))
		{
			switch (errno)
			{
				case EINVAL:
					{
						std::cerr << "`readers` exceeds SEM_VALUE_MAX (=" << (uint) (SEM_VALUE_MAX) << std::endl;
						assert(false && "impossible to initialize a semaphore: `readers` exceeds SEM_VALUE_MAX");
						break;
					}
				case ENOSYS:
					{
						std::cerr << "the system does not support process-shared semaphores\n";
						assert(false && "impossible to initialize a semaphore: the system does not support process-shared semaphores)");
						break;
					}
				default:
					{
						std::cerr << "impossible to initialize a semaphore\n";
						assert(false && "impossible to initialize a semaphore");
					}
			}
			exit(-1);
		}
		#	endif

		# endif
		# endif
	}


	// inline for keeping the method local
	inline void Semaphore::finalize()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		# ifdef YUNI_OS_WINDOWS
		# else
		#	ifdef YUNI_OS_MAC
		sem_close(pSemaphore);
		#	else
		if (0 != sem_destroy(& pSemaphore))
		{
			std::cerr << "impossible to destroy a semaphore\n";
			assert(false && "impossible to destroy a semaphore");
			exit(-1);
		}
		#	endif
		# endif
		# endif
	}




	Semaphore::Semaphore(uint readers) :
		pMaxReaders(readers)
	{
		initialize(readers);
	}


	Semaphore::Semaphore(const Semaphore& rhs) :
		pMaxReaders(rhs.pMaxReaders)
	{
		initialize(pMaxReaders);
	}


	Semaphore::~Semaphore()
	{
		finalize();
	}


	Semaphore& Semaphore::operator = (const Semaphore& copy)
	{
		uint readers = copy.maxReaders();
		if (readers != pMaxReaders)
		{
			finalize();
			initialize(readers);
		}
		return *this;
	}




} // namespace Yuni
