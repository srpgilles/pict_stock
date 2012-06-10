
#ifndef RELEVANT_INFORMATIONS_HPP_
# define RELEVANT_INFORMATIONS_HPP_

# include <bitset>
# include <ostream>
# include "date.hpp"
# include "photographer.hpp"
# include "private/date_helpers.hpp"
# include "private/photographer_helpers.hpp"
# include <tuple>


namespace PictStock
{
	namespace Private
	{
		//! Tuple of all possible relevant elements in the user-defined format
		typedef std::tuple<Year, Month, Day, Hour, Minute, Second, Photographer> TupleType;
	}

	/*!
	** \brief Class in charge of holding informations that can be used
	** to determine the path of the file in the photo directory
	*/
	class YUNI_DECL PathInformations
	{
	public:


		//! Smart pointer most adapted for the class
		typedef Yuni::SmartPtr<PathInformations> Ptr;

		/*!
		** \brief  Type of string used in path element
		**
		** Purpose is to limit the size to avoid uselessly big strings everywhere
		*/
		typedef Yuni::CString<6, false> ElementString;

	public:


		//! Constructor & destructor
		//@{

		/*!
		 * Constructor
		 *
		 * \param[in] logs Logging facility
		 *
		 */
		explicit PathInformations(LoggingFacility& logs);

		//@}

		//! Set the photographer
		void setPhotographer(const Photographer::Ptr& photographer);

		//! Set the date
		void setDate(const Date& date);

		/*!
		** \brief Get the value of one element
		**
		*/
		template<class ElementT>
		ElementString getElement() const;


		/*!
		** \brief Set the value of one element from an existing object
		**
		** For instance, take the month from input object and put it in current one
		**
		** This is useful when PathInformations object are used a key in a map
		**
		** \tparam[in] An element that can be found in the typelist Private::TupleType
		*/
		template<class ElementT>
		void setElement(const PathInformations& input);

		/*!
		** \brief Set the value of one element
		**
		** This is useful when PathInformations object are used a key in a map
		**
		** \tparam[in] An element that can be found in the typelist Private::TupleType
		*/
		template<class ElementT>
		void setElement(const AnyString& input);


		/*!
		** \brief Change the date
		**
		** \param newDate New date, in format YYYYMMDD. Hours, minutes and seconds
		** will be emptied
		*/
		void changeDate(const Yuni::CString<8, false>& newDate);


		friend bool operator==(const PathInformations& lhs, const PathInformations& rhs);
		friend bool operator!=(const PathInformations& lhs, const PathInformations& rhs);

		/*!
		** \brief Actual order doesn't matter much: the point is to be able to use RelevantInformations
		** as a key in a map
		*/
		friend bool operator<(const PathInformations& lhs, const PathInformations& rhs);

		/*!
		** \brief True if no relevant data in the object
		*/
		bool isEmpty() const;

		//! Print, for debug purposes
		void print(std::ostream& out) const;

	public:

		mutable LoggingFacility& logs;

	private:

		//! Photographer informations
		Photographer::Ptr pPhotographerPtr;

		//! Date informations
		Date pDate;

	};


	bool operator==(const PathInformations& lhs, const PathInformations& rhs);
	bool operator!=(const PathInformations& lhs, const PathInformations& rhs);
	bool operator<(const PathInformations& lhs, const PathInformations& rhs);


} // namespace PictStock


#endif /* RELEVANT_INFORMATIONS_HPP_ */
