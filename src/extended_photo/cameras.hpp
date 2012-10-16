#ifndef CAMERAS_HPP_
# define CAMERAS_HPP_

# include <tuple>
# include <vector>
# include <yuni/core/string.h>
# include "../pict_stock.hpp"
# include "private/cameras_helpers.hpp"


namespace GenericTools
{
	class SqliteWrapper;
}



namespace PictStock
{

	/*!
	** \brief This class handles all the known photographers and cameras, including
	** their storing inside sqlite database
	*/
	class YUNI_DECL Cameras : private Yuni::NonCopyable<Cameras>
	{

	public:

		//! Tuple type for rows of the table cameras in sqlite database
		typedef std::tuple<NSCameras::Private::Keyword, NSCameras::Private::Value,
			NSCameras::Private::Owner> Tuple;

		/*!
		** \brief Yields the StringType hold by each element of the input tuple
		**
		** For instance, if TupleT = [Keyword, Value, Owner] keyword type will yield
		** 	[Keyword::StringType, Value::StringType, Owner::StringType]
		*/
		typedef NSCameras::Private::TupleString<Tuple>::type  TupleString;

	public:

		//! Constructor(s)
		//@{
		/*!
		 * \brief Constructor
		 */
		explicit Cameras(GenericTools::SqliteWrapper& database);
		//@}

		//! Return the list of known keywords
		const std::vector<NSCameras::Private::Keyword::StringType>& keywords() const;

		/*!
		** \brief Identify the photographer is possible
		**
		** This method should be called when in exif data a likely couple keyword/value has been
		** identified
		**
		** We want to check then whether this couple is known or not
		*/
		bool identifyPhotographer(
			const NSCameras::Private::Keyword::StringType& currentKeyword,
			const NSCameras::Private::Value::StringType& valueToCheck,
			NSCameras::Private::Owner::StringType& photographer) const;

	private:

		/*!
		**\brief Rows in sqlite database table related to cameras.
		**
		** IMPORTANT: This vector is assumed to be sort by keyword.
		*/
		std::vector<TupleString> pRows;

		//! It's convenient to have a list of all known keywords
		std::vector<NSCameras::Private::Keyword::StringType> pKeywords;

	};

} // namespace PictStock

# include "cameras.hxx"

#endif /* PHOTOGRAPHERS_HPP_ */
