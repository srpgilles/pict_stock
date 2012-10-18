#ifndef PHOTOGRAPHER_HPP_
# define PHOTOGRAPHER_HPP_

# include <yuni/core/string.h>
# include <yuni/core/smartptr.h>
# include <list>
# include <map>
# include "private/table_photographers.hpp"
# include "../pict_stock.hpp"

namespace PictStock
{
namespace ExtendedPhoto
{

	/*!
	** \brief This class handles a single photographer
	**
	** There are no informations about its cameras: the relationship
	** works in the opposite way (it is the cameras which knows to whom
	** it belongs)
	 */

	class YUNI_DECL Photographer : private Yuni::NonCopyable<Photographer>
	{

	public:

		//! Smart pointer most adapted for the class
		typedef Yuni::SmartPtr<Photographer> Ptr;

		//! Vector of smart pointers
		typedef std::vector<Ptr> Vector;

		//! Tuple type for rows of the table cameras in sqlite database
		typedef TablePhotographers::Tuple Tuple;

		/*!
		** \brief Yields the StringType hold by each element of the input tuple
		**
		** For instance, if TupleT = [Keyword, Value, Owner] keyword type will yield
		** 	[Keyword::StringType, Value::StringType, Owner::StringType]
		*/
		typedef TablePhotographers::TupleString TupleString;


	public:

		/*!
		** \brief Constructor
		**
		** \param[in] row Tuple that stem directly from a row in sqlite database
		**
		*/
		explicit Photographer(const TupleString& row);

		//! First name of the photographer
		const TablePhotographers::FirstName::StringType& firstName() const;

		//! Last name of the photographer
		const TablePhotographers::LastName::StringType& lastName() const;

		//! Abbreviation sued to designate the photographer
		const TablePhotographers::Abbreviation::StringType& abbreviation() const;

	private:

		//! First name of the photographer
		TablePhotographers::FirstName::StringType pFirstName;

		//! Last name of the photographer
		TablePhotographers::LastName::StringType pLastName;

		//! Abbreviation sued to designate the photographer
		TablePhotographers::Abbreviation::StringType pAbbreviation;

		friend bool operator==(const Photographer& lhs, const Photographer& rhs);
		friend bool operator!=(const Photographer& lhs, const Photographer& rhs);
	};

	inline bool operator==(const Photographer& lhs, const Photographer& rhs);
	inline bool operator!=(const Photographer& lhs, const Photographer& rhs);

} // namespace ExtendedPhoto
} // namespace PictStock

#include "photographer.hxx"

#endif /* PHOTOGRAPHER_HPP_ */
