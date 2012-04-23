#include "sort_new_photos_iterator.hpp"


namespace SgPhoto
{
namespace Private
{

	using namespace Yuni;

	SortNewPhotosIterator::SortNewPhotosIterator(LoggingFacility& logs,
		const String& inputDirectory)
		: logs(logs)
	{
		if (!IO::Directory::Exists(inputDirectory))
		{
			logs.fatal() << "Error: input directory " << inputDirectory
				<< " doesn't exists\n";
			exit(-1); // TODO proper error handling to foresee
		}

		{
			// Now we want to iterate through the new files to add in the photo directory
			add(inputDirectory);
			start();
			wait();
		}
	}


	SortNewPhotosIterator::~SortNewPhotosIterator()
	{
		// For code robustness and to avoid corrupt vtable
		stop();
	}


	bool SortNewPhotosIterator::onStart(const String& rootFolder)
	{
		logs.debug() << " [+] " << rootFolder;
		return true;
	}

	SortNewPhotosIterator::Flow SortNewPhotosIterator::onBeginFolder(const String&, const String&, const String& name)
	{
		logs.debug() << " [+] " << name;


		return IO::flowContinue;
	}

	void SortNewPhotosIterator::onEndFolder(const String&, const String&, const String&)
	{ }


	SortNewPhotosIterator::Flow SortNewPhotosIterator::onFile(const String&, const String& folder,
		const String& name, uint64 /*size*/)
	{
		// Only consider files which extension is JPG or jpg
		{
			String ext;
			IO::ExtractExtension(ext, name);

			if (ext.toLower() != ".jpg")
			{
				logs.debug() << "SKIP not jpeg file " << name;
				return IO::flowContinue;
			}
		}

		String fullName;
		fullName << folder << IO::Separator << name;
		ExtendedPhoto::Ptr photoPtr = new ExtendedPhoto(logs, fullName);

		if (photoPtr->problem())
		{
			// TODO Handle the case in which a photo can't be processed
			logs.error() << "SKIP PHOTO " << fullName;
			return IO::flowContinue;
		}

		pPicturesToProcess[photoPtr->date()].push_back(photoPtr);

		return IO::flowContinue;
	}

	void SortNewPhotosIterator::onTerminate()
	{
		logs.debug() << "END\n";
	}




} // namespace Private
} // namespace SgPhoto

