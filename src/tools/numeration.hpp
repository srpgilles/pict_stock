#ifndef NUMERATION_HPP_
# define NUMERATION_HPP_

# include <yuni/core/string.h>

namespace GenericTools
{

	/*!
	** \brief The point of this class is simply to provide a numeration of a given string-size.
	**
	** For instance 01, 02, ..., 10, 11...
	**
	*/
	class YUNI_DECL Numeration
	{

	private:

		// Allow numbers up to 10^(1+maximumStringLength)
		enum
		{
			maximumStringLength = 5
		};

	public:

		typedef Yuni::CString<maximumStringLength, false> TinyString;

	public:

		//! Constructors
		//@{
		/*!
		 ** \param[in] nbElementsToDisplay Numeration will go from 1 to this number
		 ** For instance if 104 tghe values will be 001, 002, ..., 104
		 */
		explicit Numeration(unsigned int nbElementsToDisplay);

		//@}

		//! Increment the index and return the string modified
		const TinyString& next();

	private:

		//! String that will display the current element (the one of current #pIndex)
		TinyString pString;

		//! String without the left-0 padding (index as a string, no more no less then...)
		TinyString pStringWithoutPadding;

		//! Current index
		unsigned int pIndex;

		//! Number of elements to display
		unsigned int pSize;

	};


} // namespace GenericTools


#endif /* NUMERATION_HPP_ */
