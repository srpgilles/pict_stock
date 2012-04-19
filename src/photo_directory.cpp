#include "photo_directory.hpp"

using namespace Yuni;

namespace SgPhoto
{



	PhotoDirectory::PhotoDirectory(LoggingFacility& logs, Yuni::String pFolder)
		: logs(logs)
	{
		Private::PhotoDirectoryIterator iterator(logs);
		iterator.add(pFolder);

		if (!iterator.start())
		{
			logs.fatal() << "Problem encountered while scanning photo directory";
			exit(EXIT_FAILURE);
		}

		iterator.wait();
		iterator.validDirectories(pTree);
	}


	PhotoDirectory::~PhotoDirectory()
	{ }





} // namespace SgPḧoto
