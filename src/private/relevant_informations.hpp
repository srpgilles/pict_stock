
#ifndef RELEVANT_INFORMATIONS_HPP_
# define RELEVANT_INFORMATIONS_HPP_

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

		//! Return the proper information associated with index given
		Yuni::CString<10, false> value(Elements::Nature natureIndex) const;

	public:

		mutable LoggingFacility& logs;

	private:

		//! Photographer informations
		Photographer::Ptr pPhotographerPtr;

		//! Date informations
		Date::Ptr pDatePtr;

	};



} // namespace PictStock
} // namespace Private




#endif /* RELEVANT_INFORMATIONS_HPP_ */
