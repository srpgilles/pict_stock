#ifndef PHOTO_DIRECTORY_HXX_
#define PHOTO_DIRECTORY_HXX_

namespace SgPhoto
{

	template<class LanguageT>
	PhotoDirectory<LanguageT>::PhotoDirectory(LoggingFacility& logs, Yuni::String pFolder)
		: logs(logs)
	{
		Private::PhotoDirectoryIterator<LanguageT> iterator(logs);
		iterator.add(pFolder);

		if (!iterator.start())
		{
			logs.fatal() << "Problem encountered while scanning photo directory";
			exit(EXIT_FAILURE);
		}

		iterator.wait();
		iterator.validDirectories(pTree);
	}


	template<class LanguageT>
	PhotoDirectory<LanguageT>::~PhotoDirectory()
	{ }


	template<class LanguageT>
	inline const ValidDirectoriesType PhotoDirectory<LanguageT>::tree() const
	{
		return pTree;
	}


} // namespace SgPḧoto


#endif /* PHOTO_DIRECTORY_HXX_ */
