#include "photo_directory.hpp"

using namespace Yuni;

namespace SgPhoto
{
	namespace
	{
		/*!
		 * \brief A derived class from Yuni::IO::Directory::IIterator in charge
		 * of scanning the directory in which photos are to be stored
		 *
		 * The point is to determine which days are already present in this directory
		 * and for each day the associated path(s)
		 *
		 */
		class PhotoDirectoryIterator : public Yuni::IO::Directory::IIterator<true>
		{

		public:

			//! Constructor & destructor
			//@{
			//! Constructor
			PhotoDirectoryIterator();

			//! Destructor
			virtual ~PhotoDirectoryIterator();

			//@}

			//! Flow
			typedef Yuni::IO::Flow Flow;

			/*!
			** \brief Extract the list of valid directories found
			**
			** \param[out] list List of valid directories.
			** Key is YYYYMMDD, value is the list of paths associated
			** to this date
			*/
			void validDirectories(ValidDirectoriesType& list);

		private:
			//! Overload IIterator methods
			//@{
			virtual bool onStart(const Yuni::String& rootFolder);

			virtual Flow onBeginFolder(const Yuni::String& completePath,
				const Yuni::String&, const Yuni::String& name);

			virtual void onEndFolder(const Yuni::String&, const Yuni::String&, const Yuni::String&);

			virtual Flow onFile(const Yuni::String&, const Yuni::String& folder,
				const Yuni::String& name, Yuni::uint64 size);

			virtual void onTerminate();
			//@}


		 private:

			/*!
			** \brief Check the directory is valid, ie there are subfolders for
			** year, month and day
			*/
			bool checkValidity(const Yuni::String& directory);

		 private:

			enum class DateItemInDirectory
			{
				none = 0,
				year = 1,
				month = 2,
				day = 3
			};


			unsigned int pCounter;
			unsigned int pFolderCount;
			unsigned int pValidFolderCount;

			/*!
			** \brief List of valid directories found in photo directory
			**
			** Key is YYYYMMDD, value is the list of paths associated
			** to this date
			*/
			ValidDirectoriesType pValidDirectories;

		};

		PhotoDirectoryIterator::PhotoDirectoryIterator()
		{ }


		PhotoDirectoryIterator::~PhotoDirectoryIterator()
		{
			// For code robustness and to avoid corrupt vtable
			stop();
		}


		void PhotoDirectoryIterator::validDirectories(ValidDirectoriesType& list)
		{
			list = pValidDirectories;
		}


		bool PhotoDirectoryIterator::onStart(const String& rootFolder)
		{
			std::cout << " [+] " << rootFolder << std::endl;
			pCounter = 1;
			pFolderCount = 0;
			pValidFolderCount = 0;

			return true;
		}


		PhotoDirectoryIterator::Flow PhotoDirectoryIterator::onBeginFolder(const String& completePath,
			const String&, const String& name)
		{
			if (pCounter++ < 3)
			{
				// TODO: introduce log and put debug text below
				//std::cout << "No need to evaluate: not enough layer in path "
				//	"to behold day, month and year\n";
				return IO::flowContinue;
			}

			if (checkValidity(completePath))
				++pValidFolderCount;

			++pFolderCount;
			return IO::flowContinue;
		}

		void PhotoDirectoryIterator::onEndFolder(const String&, const String&, const String&)
		{
			--pCounter;
		}

		PhotoDirectoryIterator::Flow PhotoDirectoryIterator::onFile(const String&,
			const String& folder, const String& name, uint64 size)
		{
			return IO::flowContinue;
		}

		void PhotoDirectoryIterator::onTerminate()
		{
			std::cout << "\n";
			std::cout << pValidFolderCount << " / " << pFolderCount << " folder(s) are valid" << std::endl;
		}


		bool PhotoDirectoryIterator::checkValidity(const String& directory)
		{
			// Split the path in subfolders to see if they are legit directories
			std::vector<String> subPath;
			directory.split(subPath, IO::Separator);

			typedef std::vector<String>::const_iterator Iterator;

			Iterator end = subPath.end();

			String folderDate;

			DateItemInDirectory dateItems = DateItemInDirectory::none;

			for (Iterator it = subPath.begin(); it != end; ++it)
			{
				switch (dateItems)
				{
				case DateItemInDirectory::none:
				{
					// Here we have found nothing so far, and are looking
					// for year element. Year element is just an integer in
					// complete form
					unsigned int year;
					if ((it->to(year)) && (year >= 1900) && (year <= 2100))
						dateItems = DateItemInDirectory::year;

					folderDate = *it;

					break;
				}
				case DateItemInDirectory::year:
				{
					// We are now looking for month sub folder; expected format
					// is M**_
					if (!it->startsWith('M'))
						continue;

					if (it->size() != 3)
						continue;

					String::const_utf8iterator a = it->utf8begin() + 1;
					String::const_utf8iterator b = it->utf8end();

					String month(a, b);

					uint64 imonth;

					if ((month.to(imonth) && imonth > 0 && imonth <= 12))
						dateItems = DateItemInDirectory::month;

					folderDate += month;

					break;
				}
				case DateItemInDirectory::month:
				{
					// If month is known, only remaining element looked at
					// is day

					// TODO Too similar to day not to try templatize it...
					if (!it->startsWith('J'))
						continue;

					String::const_utf8iterator a = it->utf8begin() + 1;
					String::const_utf8iterator b = a + 2;

					String day(a, b);

					uint64 iday;

					if ((day.to(iday) && iday > 0 && iday <= 31))
						dateItems = DateItemInDirectory::day;

					folderDate += day;

					break;
				}
				case DateItemInDirectory::day:
				{
					// If full date was found, add date information to the
					// map container
					pValidDirectories[folderDate].push_back(directory);

					// logs.debugstd::cout << directory << "\n";



					return true;
					//break;
				}

				} // switch
			}

			return false;
		}


	} // namespace anonymous


	PhotoDirectory::PhotoDirectory(Yuni::String pFolder)
	{
		PhotoDirectoryIterator iterator;
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
