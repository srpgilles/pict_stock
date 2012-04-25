#ifndef EXTENDED_PHOTO_HPP_
# define EXTENDED_PHOTO_HPP_

# include <iostream>

# include <exiv2/image.hpp>
# include <exiv2/exif.hpp>
# include <yuni/core/string.h>
# include <yuni/io/io.h>

# include "sg_photo.hpp"
# include "private/tools.hpp"
# include "photographer.hpp"

namespace SgPhoto
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

		//! Return the date as a string (format YYYYMMDD)
		inline DateString date() const;

		//! Return the time as a string (format hhmm)
		inline HourString time() const;

		//! Return the original path of the photo
		inline YString originalPath() const;

		/*!
		 * \brief Name given in the output tree, without file extension
		 *
		 * \param[out] name Name
		 *
		 * This name does not consider possible files that would obtain the
		 * same name; a suffix will have to be added to avoid overwriting
		 * other pictures (handled in #SortNewPhotos class)
		 */
		void newNameWithoutExtension(YString& name) const;

		/*!
		 * \brief Modify the date in exif data and add a comment to specify this was done
		 *
		 * \param[in] newDate In format YYYYMMDD
		 */
		bool modifyDate(const DateString& newDate);


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
		 * Operator==
		 *
		 * TODO Find a better way to proceed: it is most inefficient due to the lack of
		 * simple methods in exiv2 library (namely the possibility to compare easily
		 * exif data from two different files)
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

		//! Smart pointer to the photographer
		Photographer::Ptr pPhotographer;

		// TODO Introduce time stamp instead of strings (but be sure there is portability!)
		/*!
		** Date of the photo, in format YYYYMMDD
		*/
		DateString pStringDate;

		/*!
		** Time of the photo, in format hhmm
		*/
		HourString pStringTime;

		/*! true if a problem occurred with exiv library */
		Status pStatus;



	private:

		/*!
		**
		** Key is the serial number of Canon camera, value struct listing name and
		** abbreviation of the photographer
		**
		*/
		static const Photographer::List pPhotographers;


	};

	//! Operator== : two #ExtendedPhoto are equals if they share the same object *pImage
	bool operator==(const ExtendedPhoto& photo1, const ExtendedPhoto& photo2);

}// namespace SgPhoto


# include "extended_photo.hxx"

#endif /* EXTENDED_PHOTOS_HPP_ */
