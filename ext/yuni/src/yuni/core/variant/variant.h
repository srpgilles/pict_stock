#ifndef __YUNI_CORE_VARIANT_VARIANT_H__
# define __YUNI_CORE_VARIANT_VARIANT_H__

# include "../../yuni.h"
# include "../string.h"
# include "../static/remove.h"
# include "dataholder/dataholder.h"
# include "dataholder/string.h"
# include "dataholder/array.h"



namespace Yuni
{

	/*!
	** \brief Variant type implementation.
	**
	** This is an implementation of Variants that handles
	** a limited range of types (classical ones), but that
	** can make automagic conversions between them, if such
	** a conversion is possible.
	**
	** It differs from Any, which can handle almost any type
	** (with the notable exception of arrays), but cannot
	** automatically convert from a type to another.
	**
	** How to use:
	** \code
	** Variant v = 12;
	**
	** // for direct conversions, without checking
	** std::cout << v.to<float>() << std::endl;
	** // with conversion checking
	** String s;
	** if (v.to(s))
	** 	std::cout << s << std::endl;
	** else
	** 	std::cout << "impossible to converter the variant into a string" << std::endl;
	** \endcode
	**
	** \ingroup Core
	*/
	class YUNI_DECL Variant
	{
	public:
		template<class T>
		struct New
		{
			//! Instanciate a new variant
			static Variant Instance();
			//! Instanciate a new variant with array support
			static Variant Array();
		};

		//! Data holder
		typedef Private::Variant::IDataHolder  IDataHolder;

		enum InnerType
		{
			//! Nil / Not assigned
			typeNil,
			//! Bool
			typeBool,
			//! char
			typeChar,
			//! sint32
			typeInt32,
			//! sint64
			typeInt64,
			//! uint32
			typeUInt32,
			//! uint64
			typeUInt64,
			//! string
			typeString,
			//! struct, with members
			typeClass,
			//! array of variants
			typeArray
		};

	public:
		//! \name Constructors
		//@{
		//! Constructs an empty Variant
		Variant();
		//! Constructs a copy of an existing Variant.
		Variant(const Variant& rhs);
		//! Constructs a Variant based on an existing variable of simple type.
		template<class T> Variant(const T& rhs);
		//! Constructs from a dataholder
		Variant(const IDataHolder* rhs, bool ref = false);
		//! Constructs from a dataholder
		Variant(IDataHolder* rhs, bool ref = false);
		//! Constructor from nullptr
		Variant(const NullPtr&);

		//! Destructor
		~Variant() {}
		//@}


		//! \name Alteration methods
		//@{
		//! Assignment from an existing Variant
		void assign(const Variant& rhs);
		//! Assignment from standard type (uint32)
		void assign(uint32 rhs);
		//! Assignment from standard type (sint32)
		void assign(sint32 rhs);
		//! Assignment from standard type (uint64)
		void assign(uint64 rhs);
		//! Assignment from standard type (sint64)
		void assign(sint64 rhs);
		//! Assignment from standard type (char)
		void assign(char rhs);
		//! Assignment from standard type (bool)
		void assign(bool rhs);
		//! Assignment from standard type (double)
		void assign(double rhs);
		//! Assignment from standard type (string)
		void assign(const String& rhs);
		//! Assignment from standard type (string)
		void assign(const char* rhs);

		//! operator + (variant)
		void add(const Variant& value);
		//! operator add (string)
		void add(const char* value);
		//! operator +
		template<class T> void add(const T& value);

		//! operator * (variant)
		void sub(const Variant& value);
		//! operator * (string)
		void sub(const char* value);
		//! operator *
		template<class T> void sub(const T& value);

		//! operator + (variant)
		void div(const Variant& value);
		//! operator add (string)
		void div(const char* value);
		//! operator +
		template<class T> void div(const T& value);

		//! operator * (variant)
		void mult(const Variant& value);
		//! operator * (string)
		void mult(const char* value);
		//! operator *
		template<class T> void mult(const T& value);

		//! Resets the Variant to an empty one.
		void clear();

		/*!
		** \brief Share content with another variant
		*/
		void shareContentFrom(const Variant& rhs);
		/*!
		** \brief Revoke reference sharing
		*/
		void shareContentFrom(const NullPtr&);
		//@}


		//! \name Information methods
		//@{
		/*!
		** \brief Get the type held by the inner value
		*/
		InnerType type() const;
		/*!
		** \brief Returns true if the Variant is empty.
		*/
		bool empty() const;
		/*!
		** \brief Get if empty
		*/
		bool isnil() const;
		//@}


		//! \name Retrieval methods
		//@{
		/*!
		** \brief Retrieve the inner value into a given type
		** \return True if the conversion succeeded, false otherwise
		*/
		template<class T> bool to(T& out) const;

		/*!
		** \brief Retrieve the inner value into a given type
		*/
		template<class T> T to() const;
		//@}


		//! \name Operator overloads
		//@{
		//! operator =
		Variant&  operator = (const Variant& rhs);
		//! operator =
		template<class T> Variant&  operator = (const T& rhs);
		//! operator =
		Variant&  operator = (const IDataHolder* rhs);
		//! operator =
		Variant&  operator = (IDataHolder* rhs);
		//! operator nullptr
		Variant&  operator = (const NullPtr&);
		//! operator +=
		template<class T> Variant& operator += (const T& rhs);
		//! operator -=
		template<class T> Variant& operator -= (const T& rhs);
		//! operator *=
		template<class T> Variant& operator *= (const T& rhs);
		//! operator /=
		template<class T> Variant& operator /= (const T& rhs);
		//! operator []
		Variant operator [] (uint index);
		const Variant operator [] (uint index) const;

		//! Invoke method with no parameter
		Variant operator () (const String& method);
		//! Invoke method with 1 parameter
		Variant operator () (const String& method, const Variant& a1);
		//! Invoke method with 2 parameters
		Variant operator () (const String& method, const Variant& a1, const Variant& a2);
		//! Invoke method with 3 parameters
		Variant operator () (const String& method, const Variant& a1, const Variant& a2, const Variant& a3);
		//! Invoke method with 4 parameters
		Variant operator () (const String& method, const Variant& a1, const Variant& a2, const Variant& a3, const Variant& a4);
		//@}


	private:
		//! Perform a deep copy if the pointer is non unique
		void deepCopyIfNonUnique();

	private:
		//! Pointer to storage object
		IDataHolder::Ptr pData;
		//! Flag to know if the content is currently shared
		bool pShareContent;

	}; // class Variant






} // namespace Yuni

# include "variant.hxx"

#endif // __YUNI_CORE_VARIANT_VARIANT_H__
