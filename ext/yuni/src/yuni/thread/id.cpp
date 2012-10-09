
#include "../yuni.h"
# ifdef YUNI_OS_LINUX
#	ifndef _GNU_SOURCE
#	define _GNU_SOURCE /* ou _BSD_SOURCE or _SVID_SOURCE */
#	endif
#	include <unistd.h>
#	include <sys/types.h>
#	include <sys/syscall.h>
# endif
#include "id.h"
#include "../core/system/windows.hdr.h"
# ifndef YUNI_NO_THREAD_SAFE
#	include "pthread.h"
#	ifdef YUNI_OS_WINDOWS
#		include "../core/system/windows.hdr.h"
# 	endif
# endif


namespace Yuni
{
namespace Thread
{

	uint64 ID()
	{
		# ifndef YUNI_NO_THREAD_SAFE
		#	ifndef YUNI_OS_WINDOWS
		#		ifdef YUNI_OS_LINUX
		return (uint64) syscall(SYS_gettid);
		#		else
		// man : The pthread_self() function returns the thread ID of the calling thread
		return (uint64) pthread_self();
		#		endif
		#	else
		return (uint64) GetCurrentThreadId();
		#	endif
		# else
		return 0;
		# endif
	}



} // namespace Thread
} // namespace Yuni

