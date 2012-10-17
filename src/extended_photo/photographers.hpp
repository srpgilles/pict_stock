#ifndef PHOTOGRAPHERS_HPP_
# define PHOTOGRAPHERS_HPP_

# include <tuple>
# include <vector>
# include <yuni/core/string.h>
# include "../pict_stock.hpp"
# include "private/helpers.hpp"


namespace GenericTools
{
	class SqliteWrapper;
}



namespace PictStock
{
namespace ExtendedPhoto
{
	/*!
	** \brief This class handles all the known photographers, including
	** their storing inside sqlite database
	**
	** This class should be hidden in #Cameras
	*/
	class YUNI_DECL Photographers : private Yuni::NonCopyable<Photographers>
	{

	public:

		//! Tuple type for rows of the table cameras in sqlite database
		typedef std::tuple<nsExtendedPhoto::Private::Keyword, nsExtendedPhoto::Private::Value,
			nsExtendedPhoto::Private::Owner> Tuple;

		/*!
		** \brief Yields the StringType hold by each element of the input tuple
		**
		** For instance, if TupleT = [Keyword, Value, Owner] keyword type will yield
		** 	[Keyword::StringType, Value::StringType, Owner::StringType]
		*/
		typedef nsExtendedPhoto::Private::TupleString<Tuple>::type  TupleString;

	public:

		//! Constructor(s)
		//@{
		/*!
		 * \brief Constructor
		 */
		explicit Cameras(GenericTools::SqliteWrapper& database);
		//@}

		//! Add a new camera
		void addNewCamera(const nsExtendedPhoto::Private::Keyword::StringType& currentKeyword,
			const nsExtendedPhoto::Private::Value::StringType& valueToCheck,
			const nsExtendedPhoto::Private::Owner::StringType& photographer);

		//! Return the list of known keywords
		const std::vector<nsExtendedPhoto::Private::Keyword::StringType>& keywords() const;

		/*!
		** \brief Identify the photographer is possible
		**
		** This method should be called when in exif data a likely couple keyword/value has been
		** identified
		**
		** We want to check then whether this couple is known or not
		*/
		bool identifyPhotographer(
			const nsExtendedPhoto::Private::Keyword::StringType& currentKeyword,
			const nsExtendedPhoto::Private::Value::StringType& valueToCheck,
			nsExtendedPhoto::Private::Owner::StringType& photographer) const;

	private:

		//! Reference to database object
		GenericTools::SqliteWrapper& pDatabase;

		/*!
		**\brief Rows in sqlite database table related to cameras.
		**
		** IMPORTANT: This vector is assumed to be sort by keyword.
		*/
		std::vector<TupleString> pRows;

		//! It's convenient to have a list of all known keywords
		std::vector<nsExtendedPhoto::Private::Keyword::StringType> pKeywords;

	};

} // namespace ExtendedPhoto
} // namespace PictStock

# include "cameras.hxx"

#endif /* PHOTOGRAPHERS_HPP_ */
