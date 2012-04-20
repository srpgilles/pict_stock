#ifndef EXTENDED_PHOTO_HPP_
# define EXTENDED_PHOTO_HPP_

# include <iostream>

# include <exiv2/image.hpp>
# include <exiv2/exif.hpp>
# include <yuni/core/string.h>
# include <yuni/io/io.h>

# include "sg_photo.hpp"
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
		 * \brief Name given in the output tree
		 * 
		 * \param[in] index If several photos in the same minute, index is used
		 * to avoid overwritting the already existing photo
		 */
		YString newName(unsigned int index = 0) const;

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

		/*!
		** Vector including the date as a bunch of integers. Index is handled
		** by #DateEnum
		*/
		std::vector<unsigned int> pDate;

		/*!
		** \brief Date as a string YYYYMMDD
		*/
		YString pStringDate;

		/*! true if a problem occurred with exiv library */
		Status pStatus;

		/*!
		** \brief New name of the photo, with possibly an extension to add if several photos
		** share the same time and photographer
		*/
		YString pNewName;



	private:

		/*!
		**
		** Key is the serial number of Canon camera, value struct listing name and
		** abbreviation of the photographer
		**
		*/
		static const Photographer::List pPhotographers;


	private:

		//! Enum dedicated to interpret date extracted from exif
		enum DateEnum
		{
			year = 0,
			month = 1,
			day = 2,
			hour = 3,
			minute = 4,
			second = 5
		};

	};
}// namespace SgPhoto


# include "extended_photo.hxx"

#endif /* EXTENDED_PHOTOS_HPP_ */
