
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
		// man : The pthread_self() function returns the thread ID of the calling thread
		return (uint64) pthread_self();
		#	else
		return (uint64) GetCurrentThreadId();
		#	endif
		# else
		return 0;
		# endif
	}



} // namespace Thread
} // namespace Yuni

