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
namespace Database
{

	namespace TableCameras
	{
		struct Keyword
		{
			typedef std::string WrappedType;
			static YString FieldName() { return "Keyword"; }
		};

		struct Value
		{
			typedef Yuni::CString<80, false> WrappedType;
			static YString FieldName() { return "Value"; }
		};

		struct Owner
		{
			typedef Yuni::CString<8, false> WrappedType;
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
		** \brief Yields the WrappedType hold by each element of the input tuple
		**
		** For instance, if TupleT = [Keyword, Value, Owner] keyword type will yield
		** 	[Keyword::WrappedType, Value::WrappedType, Owner::WrappedType]
		*/
		typedef Private::TupleWrappedType<Tuple>::type TupleWrappedType;

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
		void addNewCamera(const TableCameras::Keyword::WrappedType& currentKeyword,
			const TableCameras::Value::WrappedType& valueToCheck,
			const TableCameras::Owner::WrappedType& photographer);

		//! Add a new photographer
		inline void addNewPhotographer(const TablePhotographers::FirstName::WrappedType& firstName,
			const TablePhotographers::LastName::WrappedType& lastName,
			const TablePhotographers::Abbreviation::WrappedType& abbreviation);

		//! Return the list of known keywords
		const std::unordered_set<TableCameras::Keyword::WrappedType>& keywords() const;

		/*!
		** \brief Identify the photographer is possible
		**
		** This method should be called when in exif data a likely couple keyword/value has been
		** identified
		**
		** We want to check then whether this couple is known or not
		**
		** \param[out] photographer The photographer
		** \param[in] currentKeyword Keyword used in exif to identify the camera
		** \param[in] valueToCheck Value associated to this keyword for the current picture
		**
		** \return True if a photographer was found
		*/
		bool identifyPhotographer(
			const TableCameras::Keyword::WrappedType& currentKeyword,
			const TableCameras::Value::WrappedType& valueToCheck,
			Photographer::Ptr& photographer) const;


		/*!
		** \brief Identify the photographer from its abbreviation
		**
		** \param[out] photographer The photographer
		** \param[in] abbreviation Abbreviation related to the photographer (unique)
		**
		** \return True if a photographer was found
		*/
		void identifyPhotographerAbbr(
			const TableCameras::Owner::WrappedType& abbreviation,
			Photographer::Ptr& photographer) const;


		//! Same as above with conversion
		template<class StringT>
		void identifyPhotographerAbbr(
			const StringT& abbreviation,
			Photographer::Ptr& photographer) const;


	private:

		/*!
		** \brief Helper function of #addNewCameras, which add the new tuple in memory
		**
		** Addition to database is performed in #addNewCameras
		 */
		void addNewTuple(const TupleWrappedType& tuple);

		# ifndef NDEBUG
		/*!
		** \brief Print on screen the list of all cameras and related informations
		 */
		void printRows() const;
		# endif // NDEBUG


	private:

		//! Reference to database object
		GenericTools::SqliteWrapper& pDatabase;

		/*!
		**\brief Rows in sqlite database table related to cameras.
		**
		** IMPORTANT: This vector is assumed to be sort by keyword.
		*/
		std::vector<TupleWrappedType> pRows;

		//! It's convenient to have a list of all known keywords
		std::unordered_set<TableCameras::Keyword::WrappedType> pKeywords;

		//! Photographers
		std::unique_ptr<Photographers> pPhotographersPtr;

	};

} // namespace Database
} // namespace PictStock

# include "cameras.hxx"

#endif /* PHOTOGRAPHERS_HPP_ */
