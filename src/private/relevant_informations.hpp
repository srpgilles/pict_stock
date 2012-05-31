
#ifndef RELEVANT_INFORMATIONS_HPP_
# define RELEVANT_INFORMATIONS_HPP_

# include <bitset>
# include "date.hpp"
# include "photographer.hpp"
# include "element.hpp"

namespace PictStock
{
namespace Private
{


	class YUNI_DECL RelevantInformations
	{
	public:

		//! Smart pointer most adapted for the class
		typedef Yuni::SmartPtr<RelevantInformations> Ptr;

		//! Constructor & destructor
		//@{

		/*!
		 * Constructor
		 *
		 * \param[in] logs Logging facility
		 *
		 */
		explicit RelevantInformations(LoggingFacility& logs);

		//@}

		//! Set the photographer
		void setPhotographer(const Photographer::Ptr& photographer);

		//! Set the date
		void setDate(const Date& date);


		//! Return the proper information associated with index given
		Yuni::CString<10, false> value(unsigned int natureIndex) const;

		/*!
		** \brief Set the value of a given index
		**
		** This is intended to be used when a regex has been operated over a path
		 */
		void setValue(unsigned int natureIndex, const AnyString& value);

		/*!
		** \brief Change the date
		**
		** \param newDate New date, in format YYYYMMDD. Hours, minutes and seconds
		** will be emptied
		*/
		void changeDate(const Yuni::CString<8, false>& newDate);


		/*!
		** \brief Create a new RelevantInformations object featuring only informations useful
		** for the current study
		**
		** For instance, if the folder part of a path contains only year and month,
		** returned object will have these values filled and all others set to
		** empty string
		**
		** \param[in] arePresent Bitset telling for each element whether they are considered
		** or not. Such a bitset is expected to be one of those defined in #PathFormatHelper class
		 */
		RelevantInformations onlyUsefulOnes(const std::bitset<Elements::size>& arePresent) const;


		friend bool operator==(const RelevantInformations& lhs, const RelevantInformations& rhs);
		friend bool operator!=(const RelevantInformations& lhs, const RelevantInformations& rhs);

	public:

		mutable LoggingFacility& logs;

	private:

		//! Photographer informations
		Photographer::Ptr pPhotographerPtr;

		//! Date informations
		Date pDate;

	};


	bool operator==(const RelevantInformations& lhs, const RelevantInformations& rhs);
	bool operator!=(const RelevantInformations& lhs, const RelevantInformations& rhs);


} // namespace PictStock
} // namespace Private




#endif /* RELEVANT_INFORMATIONS_HPP_ */
