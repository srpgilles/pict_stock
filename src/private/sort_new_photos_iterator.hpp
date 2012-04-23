#ifndef SORT_NEW_PHOTOS_ITERATOR_HPP_
# define SORT_NEW_PHOTOS_ITERATOR_HPP_

# include <yuni/io/directory/iterator.h>
# include "../extended_photo.hpp"

namespace SgPhoto
{
namespace Private
{


	class SortNewPhotosIterator : public Yuni::IO::Directory::IIterator<true>
	{
	public:

		//! Flow
		typedef Yuni::IO::Flow Flow;


	public:

		//! Constructor & destructor
		//@{

		/*!
		 * Constructor
		 *
		 * \param[in] inputDirectory All JPG files in this folder (or subfolders) will be sort
		 */
		explicit SortNewPhotosIterator(LoggingFacility& logs,
			const Yuni::String& inputDirectory);

		//! Destructor
		virtual ~SortNewPhotosIterator();
		//@}

		/*!
		 * \brief Return the list of all pictures sort by date
		 *
		 * \param[out] out PicturesToProcess Key is date under format YYYYMMDD,
		 * value if a list of #ExtendedPhoto pointers
		 */
		void picturesToProcess(std::map<DateString, ExtendedPhoto::Vector>& out) const;

	public:

		//! Logs
		LoggingFacility& logs;



	protected:

		//! Overload IIterator methods
		//@{
		virtual bool onStart(const Yuni::String& rootFolder);

		virtual Flow onBeginFolder(const Yuni::String&, const Yuni::String&, const Yuni::String& name);

		virtual void onEndFolder(const Yuni::String&, const Yuni::String&, const Yuni::String&);

		virtual Flow onFile(const Yuni::String&, const Yuni::String& folder, const Yuni::String& name, Yuni::uint64 size);

		virtual void onTerminate();
		//@}

		//! List of all jpeg files to process, sort by date
		std::map<DateString, ExtendedPhoto::Vector> pPicturesToProcess;

	};
} // namespace Private
} // namespace SgPhoto

# include "sort_new_photos_iterator.hxx"

#endif /* SORT_NEW_PHOTOS_ITERATOR_HPP_ */
