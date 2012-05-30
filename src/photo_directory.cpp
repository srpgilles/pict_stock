#include "photo_directory.hpp"

using namespace Yuni;

namespace PictStock
{


	PhotoDirectory::PhotoDirectory(LoggingFacility& logs, const Yuni::String& folder, const YString& pathFormat)
		: logs(logs),
		  pMainFolder(folder),
		  pPathFormat(new Private::PathFormat(logs, pathFormat))
	{
		Private::PhotoDirectoryIterator iterator(logs);
		iterator.add(folder);

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


	bool PhotoDirectory::createDateFolder(YString& folder, const ExtendedPhoto& photo)
	{
		assert(!(!pPathFormat));
		auto& pathFormat = *pPathFormat;
		pathFormat.determineMinimalFolder(folder, photo);

		if (!IO::Directory::Create(folder))
			return false;

		// Add new folder in the tree
		String key;
		pathFormat.determineFolderKey(key, photo);
		pTree.insert(std::make_pair(key, std::list<String>(1, folder)));

		return true;
	}
		


} // namespace SgPḧoto
