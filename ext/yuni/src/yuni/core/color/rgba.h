#ifndef __YUNI_CORE_COLOR_RGBA_H__
# define __YUNI_CORE_COLOR_RGBA_H__

# include "../../yuni.h"
# include "fwd.h"


namespace Yuni
{
namespace Color
{

	/*!
	** \brief 32Bits RGB Color Model (additive color model + transparency)
	*/
	template<class T = uint8>
	class RGBA
	{
	public:
		//! Type used for channels
		typedef T Type;
		//! The most appropriate type for printing
		typedef typename Yuni::Private::Color::Print<T>::Type  PrintType;
		//! The most appropriate type for calculations
		template<class V>
		struct Calculation
		{
			typedef typename Yuni::Private::Color::Calculation<T, V>::Type  Type;
		};
		enum
		{
			//! A non-zero value if the class has a transparency channel
			hasTransparency = 1,
			//! The upper bound for this color model
			upperBound = Yuni::Private::Color::DefaultValues<T>::upperBound,
			//! The lower bound for this color model
			lowerBound = Yuni::Private::Color::DefaultValues<T>::lowerBound,
		};

	public:
		//! \name Constructors
		//@{
		/*!
		** \brief Default constructor
		*/
		RGBA();
		/*!
		** \brief Copy constructor
		*/
		RGBA(const RGBA& rhs);
		/*!
		** \brief Constructor with default values
		*/
		template<class R, class G, class B, class A> RGBA(R r, G g, B b, A a);
		/*!
		** \brief Constructor with default values
		*/
		template<class R, class G, class B> RGBA(R r, G g, B b);
		/*!
		** \brief Constructor with default values
		*/
		template<class U> RGBA(const U& value);
		//@}


		//! \name Assign
		//@{
		/*!
		** \brief Assign all channels at once
		*/
		template<class R, class G, class B, class A> void assign(R r, G g, B b, A a);
		/*!
		** \brief Reset all channels at once
		**
		** The alpha channel will have a default value (opaque)
		*/
		template<class R, class G, class B> void assign(R r, G g, B b);
		/*!
		** \brief Reset all channels at once from a single value
		*/
		template<class U> void assign(const U& value);
		//@}


		//! \name Misc
		//@{
		/*!
		** \brief Get if the color will be visible
		**
		** This method is equivalent to manually check the alpha channel
		** with the lower bound
		*/
		bool visible() const;

		/*!
		** \brief Check if all channels are valid
		**
		** This method is only useful when the channels are modified manually.
		*/
		bool valid() const;
		//@}


		//! \name Stream printing
		//@{
		//! Print the color value
		template<class StreamT> void print(StreamT& out) const;
		//@}


		//! \name Operators
		//@{
		//! Increase or decrease all channels
		template<class U> RGBA& operator += (const U& value);
		//! Increase or decrease all channels
		template<class U> RGBA& operator -= (const U& value);
		//! Assignment
		RGBA& operator = (const RGBA& rhs);
		//! Assignment
		RGBA& operator = (const Yuni::NullPtr&);
		//! Assignment
		template<class U> RGBA& operator = (const U& rhs);
		//! Comparison
		template<class U> bool operator == (const U& rhs) const;
		//! Comparison
		template<class U> bool operator != (const U& rhs) const;
		//! Index access
		T& operator [] (const uint i);
		//@}


	public:
		//! The red channel
		Type red;
		//! The green channel
		Type green;
		//! The blue channel
		Type blue;
		//! The alpha channel (transparency)
		Type alpha;

	}; // class RGBA





} // namespace Color
} // namespace Yuni

# include "rgba.hxx"

#endif // __YUNI_CORE_COLOR_RGBA_H__
