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



	class ExtendedPhoto
	{
	public:


		//! Status obtained while building the photo object
		enum Status
		{
			epFine = 0,
			epExiv2Problem,
			epInternalProblem
		};

		//! Constructor
		//@{
		ExtendedPhoto(LoggingFacility& logs, const YString& filename);
		//@}

		//! Return true if any problem occurred (either from this class or exiv2)
		bool problem() const;

		//! Return the date as a string (format YYYYMMDD)
		YString date() const;

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

	public:
	
		//! Logging facility
		mutable LoggingFacility& logs;

	private:


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
}// namespace SgPhoto


# include "extended_photo.hxx"

#endif /* EXTENDED_PHOTOS_HPP_ */