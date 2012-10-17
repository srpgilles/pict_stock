#ifndef PHOTOGRAPHER_HPP_
# define PHOTOGRAPHER_HPP_

# include <yuni/core/string.h>
# include <yuni/core/smartptr.h>
# include <list>
# include <map>

# include "../../pict_stock.hpp"

namespace PictStock
{
namespace ExtendedPhoto
{
namespace Private
{

	/*!
	** \brief This class, that handles a single photographer and all its camera,
	** should be accessed only through #Photographers class
	 */
	class YUNI_DECL Photographer : private Yuni::NonCopyable<Photographer>
	{

	public:

		//! Smart pointer most adapted for the class
		typedef Yuni::SmartPtr<Photographer> Ptr;

		//! List of smart pointers
		typedef std::list<Ptr> List;

		//! Vector of smart pointers
		typedef std::vector<Ptr> Vector;

		/*!
		** Type used to store the cameras owned by current photographer
		**
		** There is one entry per camera in the multimap; key is the exif key used for the
		** match in the exif file, and value the value to match the peculiar camera
		**
		** For instance, a camera might be identified by the pair
		** ("Exif.Canon.SerialNumber", "2280522782")
		*/
		typedef std::multimap<std::string, Yuni::String> ExifDataType;


	public:

		/*!
		** \brief Constructor
		**
		** \param[in] name Complete name of the photographer
		** \param[in] abbrev Abbreviation to use in the name of the photo
		** \param[in] cameras List of cameras associated to this photographer
		**
		*/
		explicit Photographer(const YString& name, const YString& abbrev,
			const std::multimap<std::string, Yuni::String>& cameras);

		//! Name of the photographer
		YString name() const;

		//! Abbreviation related to the photographer
		YString abbr() const;

		//! List of keys/values identifying the photographer in exif data (see #ExifDataType)
		const ExifDataType& exifData() const;


	private:

		//! Name of the photographer
		YString pName;

		//! Abbreviation (to use in photo's name)
		YString pAbbrev;

		//! List of keys/values identifying the photographer in exif data (see #ExifDataType)
		ExifDataType pCameras;


		friend bool operator==(const Photographer& lhs, const Photographer& rhs);
		friend bool operator!=(const Photographer& lhs, const Photographer& rhs);
	};

	bool operator==(const Photographer& lhs, const Photographer& rhs);
	bool operator!=(const Photographer& lhs, const Photographer& rhs);

} // namespace Private
} // namespace ExtendedPhoto
} // namespace PictStock

#include "photographer.hxx"

#endif /* PHOTOGRAPHER_HPP_ */
