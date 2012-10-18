#ifndef CAMERAS_HPP_
# define CAMERAS_HPP_

# include <tuple>
# include <vector>
# include <unordered_set>
# include <memory>
# include <yuni/core/string.h>
# include "../pict_stock.hpp"
# include "../tools/tools.hpp"
# include "private/helpers.hpp"
# include "private/table_photographers.hpp"
# include "photographer.hpp"
# include "photographers.hpp"

namespace GenericTools
{
	class SqliteWrapper;
}



namespace PictStock
{
namespace ExtendedPhoto
{

	namespace TableCameras
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
	} // namespace TableCameras



	/*!
	** \brief This class handles all the known photographers and cameras, including
	** their storing inside sqlite database
	*/
	class YUNI_DECL Cameras : private Yuni::NonCopyable<Cameras>
	{

	public:

		//! Tuple type for rows of the table cameras in sqlite database
		typedef std::tuple<TableCameras::Keyword, TableCameras::Value,
			TableCameras::Owner> Tuple;

		/*!
		** \brief Yields the StringType hold by each element of the input tuple
		**
		** For instance, if TupleT = [Keyword, Value, Owner] keyword type will yield
		** 	[Keyword::StringType, Value::StringType, Owner::StringType]
		*/
		typedef Private::TupleString<Tuple>::type TupleString;

		//! Name of the table in sqlite database
		static YString TableName() { return "Cameras"; }

	public:

		//! Constructor(s)
		//@{
		/*!
		 * \brief Constructor
		 */
		explicit Cameras(GenericTools::SqliteWrapper& database);
		//@}

		//! Add a new camera
		void addNewCamera(const TableCameras::Keyword::StringType& currentKeyword,
			const TableCameras::Value::StringType& valueToCheck,
			const TableCameras::Owner::StringType& photographer);

		//! Add a new photographer
		inline void addNewPhotographer(const TablePhotographers::FirstName::StringType& firstName,
			const TablePhotographers::LastName::StringType& lastName,
			const TablePhotographers::Abbreviation::StringType& abbreviation);

		//! Return the list of known keywords
		const std::unordered_set<TableCameras::Keyword::StringType>& keywords() const;

		/*!
		** \brief Identify the photographer is possible
		**
		** This method should be called when in exif data a likely couple keyword/value has been
		** identified
		**
		** We want to check then whether this couple is known or not
		**
		** \param[out] photographer The photographer
		** \param[in] currentKeyword Keyword used in exif to identify the camera
		** \param[in] valueToCheck Value associated to this keyword for the current picture
		**
		** \return True if a photographer was found
		*/
		bool identifyPhotographer(
			const TableCameras::Keyword::StringType& currentKeyword,
			const TableCameras::Value::StringType& valueToCheck,
			Photographer::Ptr photographer) const;


		/*!
		** \brief Identify the photographer from its abbreviation
		**
		** \param[out] photographer The photographer
		** \param[in] abbreviation Abbreviation related to the photographer (unique)
		**
		** \return True if a photographer was found
		*/
		void identifyPhotographerAbbr(
			const TableCameras::Owner::StringType& abbreviation,
			Photographer::Ptr photographer) const;


		//! Same as above with conversion
		template<class StringT>
		void identifyPhotographerAbbr(
			const StringT& abbreviation,
			Photographer::Ptr photographer) const;


	private:

		/*!
		** \brief Helper function of #addNewCameras, which add the new tuple in memory
		**
		** Addition to database is performed in #addNewCameras
		 */
		void addNewTuple(const TupleString& tuple);

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
		std::unordered_set<TableCameras::Keyword::StringType> pKeywords;

		//! Photographers
		std::unique_ptr<Photographers> pPhotographersPtr;

	};

} // namespace ExtendedPhoto
} // namespace PictStock

# include "cameras.hxx"

#endif /* PHOTOGRAPHERS_HPP_ */
