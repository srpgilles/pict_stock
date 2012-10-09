#ifndef __YUNI_CORE_MATH_TRIGONOMETRIC_H__
# define __YUNI_CORE_MATH_TRIGONOMETRIC_H__

# include "math.h"


namespace Yuni
{
namespace Math
{


	//! \name Standard trigonometric functions
	//@{
	/*!
	** \brief The cosine function
	*/
	template<typename T> inline T Cos(T x);
	/*!
	** \brief The sine function
	*/
	template<typename T> inline T Sin(T x);
	/*!
	** \brief The tangent function
	*/
	template<typename T> inline T Tan(T x);
	//@}


	//! \name Inverse trigonometric functions
	//@{
	/*!
	** \brief The arc cosine function
	*/
	template<typename T> inline T ACos(T x);
	/*!
	** \brief The arc sine function
	*/
	template<typename T> inline T ASin(T x);
	/*!
	** \brief The arc tangent function
	*/
	template<typename T> inline T ATan(T x);
	/*!
	** \brief 2-parameter arc tangent function
	*/
	template<typename T> inline T ATan2(T y, T x);
	//@}


	//! \name Inverse hyperbolic functions
	//@{
	/*!
	** \brief The inverse hyperbolic sine function
	*/
	template<typename T> inline T ASinH(T x);
	/*!
	** \brief The inverse hyperbolic cosine function
	*/
	template<typename T> inline T ACosH(T x);
	/*!
	** \brief The inverse hyperbolic tangent function
	*/
	template<typename T> inline T ATanH(T x);
	//@}




} // namespace Math
} // namespace Yuni

# include "trigonometric.hxx"

#endif // __YUNI_CORE_MATH_TRIGONOMETRIC_H__
