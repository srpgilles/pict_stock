#include "photo_directory.hpp"

using namespace Yuni;

namespace SgPhoto
{



	PhotoDirectory::PhotoDirectory(Yuni::String pFolder)
	{
		Private::PhotoDirectoryIterator iterator;
		iterator.add(pFolder);

		if (!iterator.start())
		{
			std::cerr << "Problem encountered while scanning photo directory\n";
			exit(EXIT_FAILURE);
		}

		iterator.wait();
		iterator.validDirectories(pTree);
	}


	PhotoDirectory::~PhotoDirectory()
	{ }





} // namespace SgPḧoto
