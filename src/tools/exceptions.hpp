#ifndef EXCEPTIONS_HPP_
# define EXCEPTIONS_HPP_

# include <yuni/core/string.h>

namespace GenericTools
{
	class YUNI_DECL Exception : public std::exception
	{
	public:

		//! Constructor & destructor
		//@{
		/*!
		** \brief Constructor
		**
		** \param msg Message describing the problem encountered
		*/
		Exception(const AnyString& msg);

		virtual ~Exception() throw();

		//@}

		//! Overwrite what method
		virtual const char* what() const throw();

	private:

		//! Text describing the exception
		YString pMessage;
	};
} // namespace GenericTools

#endif /* EXCEPTIONS_HPP_ */
