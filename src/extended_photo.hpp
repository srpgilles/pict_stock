#ifndef EXTENDED_PHOTO_HPP_
# define EXTENDED_PHOTO_HPP_

# include <exiv2/src/image.hpp>
# include <exiv2/src/exif.hpp>
# include <yuni/core/string.h>
# include <yuni/io/io.h>

# include "pict_stock.hpp"
# include "private/tools.hpp"
# include "private/relevant_informations.hpp"

namespace PictStock
{

	class YUNI_DECL ExtendedPhoto
	{
	public:


		//! Status obtained while building the photo object
		enum Status
		{
			epFine = 0,
			epExiv2Problem,
			epInternalProblem
		};

		//! Smart pointer most adapted for the class
		typedef Yuni::SmartPtr<ExtendedPhoto> Ptr;

		//! Container most adapted for the class
		typedef std::vector<Ptr> Vector;

		//! Constructor
		//@{
		ExtendedPhoto(LoggingFacility& logs, const YString& filename);
		//@}

		//! Return true if any problem occurred (either from this class or exiv2)
		inline bool problem() const;

		//! Return the original path of the photo
		inline YString originalPath() const;


		/*!
		 * \brief Modify the date in exif data and add a comment to specify this was done
		 *
		 * \param[in] newDate In format YYYYMMDD
		 */
		bool modifyDate(const Yuni::CString<8, false>& newDate);


		/*!
		** Read all exif data.
		**
		** This method is directly adapted from the examples provided on exiv2
		** website
		**
		** \return True if informations could be printed, false otherwise
		** (typically if no exif data)
		**
		*/
		bool printExifData(std::ostream& out) const;

		/*!
		** \brief Get relevant informations for processing the photo
		*/
		Private::RelevantInformations::Ptr informations() const;

		/*!
		 * Operator==
		 *
		 * TODO Find a better way to proceed: it is most inefficient due to the lack of
		 * simple methods in exiv2 library (namely the possibility to compare easily
		 * exif data from two different files)
		 *
		 * I earned a bit by beginning with the size check: if file sizes are not equal
		 * the photo is not the same (should resolve many comparisons quickly)
		 */
		friend bool operator==(const ExtendedPhoto& photo1, const ExtendedPhoto& photo2);

	public:

		//! Logging facility
		mutable LoggingFacility& logs;

	private:

		/*!
		** Identify the camera if possible, and deduce from it the photographer
		**
		**
		** \return True if photographer is known, false otherwise
		**
		*/
		bool identifyPhotographer();

		/*!
		** Extract the date at which the picture was taken
		**
		** \return True if success
		**
		*/
		bool extractDate();

		/*!
		** To find an exif tag, exiv2 library requires a key, so basically we have
		** to convert the tag string into a key and then pass it to findKey
		**
		** The present method intends to act as a shortcut, and to handle more
		** directly (by a return value) the case in which no such tag was found
		**
		*/
		bool findExifKey(const std::string& key, Yuni::String& value) const;



	private:

		//! Path of the photo prior to the sorting
		YString pOriginalPath;

		//! Smart pointer to exiv2 Image object
		Exiv2::Image::AutoPtr pImage;

		//! Object which manages date data
		Private::RelevantInformations::Ptr pRelevantInformations;

		/*! True if a problem occurred with exiv library */
		Status pStatus;


	private:

		/*!
		**
		** Key is the serial number of Canon camera, value struct listing name and
		** abbreviation of the photographer
		**
		*/
		static const Private::Photographer::List pPhotographers;


	};

	//! Operator== : two #ExtendedPhoto are equals if they share the same object *pImage
	bool operator==(const ExtendedPhoto& photo1, const ExtendedPhoto& photo2);

}// namespace PictStock


# include "extended_photo.hxx"

#endif /* EXTENDED_PHOTOS_HPP_ */
