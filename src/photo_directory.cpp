#include "photo_directory.hpp"

using namespace Yuni;

namespace PictStock
{


	PhotoDirectory::PhotoDirectory(LoggingFacility& logs, Yuni::String folder)
		: logs(logs),
		  pMainFolder(folder)
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


	bool PhotoDirectory::createDateFolder(const DateString& date, YString& folder)
	{
		// TODO Should be handled with regexp later (more peculiarly the same regex
		// than the one used when scanning already existing directories...)
		DateString year, month, day;
		year.assign(date, 4);
		month.assign(date, 2, 4);
		day.assign(date, 2, 6);

		assert(folder.size() == 0 && "Only output parameter");
		folder << pMainFolder << IO::Separator;
		folder << year << IO::Separator
			<< 'M' << month << IO::Separator
			<< 'J' << day;

		if (!IO::Directory::Create(folder))
			return false;

		// Add new folder in the tree
		pTree.insert(std::make_pair(date, std::list<String>(1, folder)));

		return true;
	}
		


} // namespace SgPḧoto
