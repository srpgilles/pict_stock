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


	void PhotoDirectory::print(std::ostream& out) const
	{
		for (auto it = pTree.cbegin(), end = pTree.cend(); it != end; ++it)
		{
			out << "Date = " << it->first << '\n';

			for (auto dir = it->second.cbegin(), end = it->second.cend(); dir != end; ++dir)
				out << '\t' << *dir << '\n';
		}
	}
		


} // namespace SgPḧoto
