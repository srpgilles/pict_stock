
# include "rename.h"
# include "../../thread/mutex.h"

# ifdef YUNI_OS_LINUX
#	include <stdio.h>
#	include <stdlib.h>
#	include <string.h>
#	include <unistd.h>
#	include <limits.h>
#	include <sys/user.h>
#	include <sys/prctl.h>

# endif




namespace Yuni
{

	# ifdef YUNI_OS_LINUX

	static char **Argv = NULL;      // pointer to argument vector
	static char *LastArgv = NULL;   // end of argv
	static char Argv0[128];         // program name


	static void LinuxInitializeRenameProcess(int argc, char **argv, char **envp)
	{
		int i;
		for (i = 0; envp[i] != NULL; i++)
			continue;
		__environ = (char **) malloc(sizeof (char *) * (i + 1));

		for (i = 0; envp[i] != NULL; i++)
			__environ[i] = strdup(envp[i]);
		__environ[i] = NULL;

		Argv = argv;
		LastArgv = (i > 0)
			? envp[i - 1] + strlen(envp[i - 1])
			: argv[argc - 1] + strlen(argv[argc - 1]);

		char* tmp = strrchr(argv[0], '/');
		if (!tmp)
			tmp = argv[0];
		else
			++tmp;

		strncpy(Argv0, tmp, sizeof(Argv0));
		Argv0[sizeof(Argv0) - 1] = 0;
	}


	static void LinuxRenameProcess(const char* text, uint size)
	{
		static char buffer[2048];

		char* p = buffer;

		// This can't overflow buf due to the relative size of Argv0
		(void) strcpy(p, Argv0);
		(void) strcat(p, ": ");
		p += strlen(p);


		uint buffersize = (uint) (sizeof(buffer) - (p - buffer) - 1);
		if (size > buffersize)
			size = buffersize;
		memcpy(p, text, size);
		p[size] = '\0';

		(void) strcpy(Argv[0], buffer);
		Argv[1] = NULL;
	}


	char** RenameProcess(int argc, char** argv, const AnyString& newprocessname)
	{
		// Global locking for multithreading
		static Yuni::Mutex globalMutex;
		Yuni::MutexLocker locker(globalMutex);

		static bool initialized = false;
		if (not initialized)
		{
			LinuxInitializeRenameProcess(argc, argv, environ);
			initialized = true;
		}
		LinuxRenameProcess(newprocessname.c_str(), newprocessname.size());
		return argv;
	}



	# else

	char** RenameProcess(int /*argc*/, char** argv, const AnyString& /*newprocessname*/)
	{
		// does nothing
		return argv;
	}

	# endif

} // namespace Yuni


