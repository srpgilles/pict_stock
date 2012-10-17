#ifndef CAMERAS_HPP_
# define CAMERAS_HPP_

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
	namespace Private
	{
		struct Keyword
		{
			typedef std::string StringType;
			static YString FieldName() { return "Keyword"; }
		};

		struct Value
		{
			typedef Yuni::CString<80, false> StringType;
			static YString FieldName() { return "Value"; }
		};

		struct Owner
		{
			typedef Yuni::CString<8, false> StringType;
			static YString FieldName() { return "Owner"; }
		};
	} // namespace Private



	/*!
	** \brief This class handles all the known photographers and cameras, including
	** their storing inside sqlite database
	*/
	class YUNI_DECL Cameras : private Yuni::NonCopyable<Cameras>
	{

	public:

		//! Tuple type for rows of the table cameras in sqlite database
		typedef std::tuple<Private::Keyword, Private::Value,
			Private::Owner> Tuple;

		/*!
		** \brief Yields the StringType hold by each element of the input tuple
		**
		** For instance, if TupleT = [Keyword, Value, Owner] keyword type will yield
		** 	[Keyword::StringType, Value::StringType, Owner::StringType]
		*/
		typedef Private::TupleString<Tuple>::type TupleString;

	public:

		//! Constructor(s)
		//@{
		/*!
		 * \brief Constructor
		 */
		explicit Cameras(GenericTools::SqliteWrapper& database);
		//@}

		//! Add a new camera
		void addNewCamera(const Private::Keyword::StringType& currentKeyword,
			const Private::Value::StringType& valueToCheck,
			const Private::Owner::StringType& photographer);

		//! Return the list of known keywords
		const std::vector<Private::Keyword::StringType>& keywords() const;

		/*!
		** \brief Identify the photographer is possible
		**
		** This method should be called when in exif data a likely couple keyword/value has been
		** identified
		**
		** We want to check then whether this couple is known or not
		*/
		bool identifyPhotographer(
			const Private::Keyword::StringType& currentKeyword,
			const Private::Value::StringType& valueToCheck,
			Private::Owner::StringType& photographer) const;

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
		std::vector<Private::Keyword::StringType> pKeywords;

	};

} // namespace ExtendedPhoto
} // namespace PictStock

# include "cameras.hxx"

#endif /* PHOTOGRAPHERS_HPP_ */
