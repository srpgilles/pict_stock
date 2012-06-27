#include "exceptions.hpp"


namespace GenericTools
{

	Exception::Exception(const AnyString& msg)
			: pMessage(msg)
		{ }


	Exception::~Exception() throw()
	{ }


	const char* Exception::what() const throw()
	{
		return pMessage.c_str();
	}

} // namespace GenericTools
