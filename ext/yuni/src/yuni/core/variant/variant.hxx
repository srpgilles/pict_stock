#ifndef __YUNI_CORE_VARIANT_VARIANT_HXX__
# define __YUNI_CORE_VARIANT_VARIANT_HXX__


namespace Yuni
{

	template<class T>
	inline Variant Variant::New<T>::Instance()
	{
		assert(false && "Variant<T>::New : not implemented");
		return Variant();
	}


	template<> inline Variant Variant::New<char*>::Instance()       { return Variant(String()); }
	template<> inline Variant Variant::New<const char*>::Instance() { return Variant(String()); }
	template<> inline Variant Variant::New<String>::Instance()      { return Variant(String()); }

	template<> inline Variant Variant::New<float>::Instance()       { return Variant(double()); }
	template<> inline Variant Variant::New<double>::Instance()      { return Variant(double()); }

	template<> inline Variant Variant::New<uint32>::Instance()      { return Variant(uint32()); }
	template<> inline Variant Variant::New<uint64>::Instance()      { return Variant(uint64()); }
	template<> inline Variant Variant::New<sint32>::Instance()      { return Variant(sint32()); }
	template<> inline Variant Variant::New<sint64>::Instance()      { return Variant(sint64()); }

	template<> inline Variant Variant::New<bool>::Instance()        { return Variant(bool()); }
	template<> inline Variant Variant::New<char>::Instance()        { return Variant(char()); }


	template<class T>
	inline Variant Variant::New<T>::Array()
	{
		// We must specify the type to avoid invalid cast
		Private::Variant::IDataHolder* holder =
			new Private::Variant::ArrayData<typename Static::Remove::Const<T>::Type>();
		return holder;
	}






	template<class T>
	inline Variant::Variant(const T& rhs) :
		pShareContent(false)
	{
		assign(rhs);
	}


	inline void Variant::assign(const char* rhs)
	{
		assign(String(rhs));
	}


	template<class T>
	inline void Variant::add(const T& value)
	{
		if (!pData)
		{
			assign(value);
		}
		else
		{
			deepCopyIfNonUnique();
			pData->add(value);
		}
	}


	template<class T>
	inline void Variant::mult(const T& value)
	{
		if (!(!pData))
		{
			deepCopyIfNonUnique();
			pData->mult(value);
		}
	}


	inline void Variant::mult(const char*)
	{
		// do nothing
	}


	template<class T>
	inline void Variant::sub(const T& value)
	{
		if (!(!pData))
		{
			deepCopyIfNonUnique();
			pData->sub(value);
		}
		else
		{
			assign(value);
			mult(-1);
		}
	}


	template<class T>
	inline void Variant::div(const T& value)
	{
		if (!(!pData))
		{
			deepCopyIfNonUnique();
			pData->div(value);
		}
	}


	inline void Variant::div(const char*)
	{
		// do nothing
	}


	template<class T>
	inline T Variant::to() const
	{
		return (!(!pData)) ? pData->to<T>() : T();
	}


	template<class T>
	inline bool Variant::to(T& out) const
	{
		if (!(!pData))
			return pData->to(out);
		out = T();
		return false;
	}


	inline bool Variant::empty() const
	{
		return !pData;
	}


	inline bool Variant::isnil() const
	{
		return !pData;
	}


	inline Variant& Variant::operator = (const Variant& rhs)
	{
		assign(rhs);
		return *this;
	}


	inline Variant& Variant::operator = (const NullPtr&)
	{
		pData = nullptr;
		pShareContent = false;
		return *this;
	}


	inline Variant&  Variant::operator = (Private::Variant::IDataHolder* rhs)
	{
		assign(Variant(rhs));
		return *this;
	}


	template<class T>
	inline Variant& Variant::operator = (const T& rhs)
	{
		assign(rhs);
		return *this;
	}


	template<class T>
	inline Variant& Variant::operator += (const T& rhs)
	{
		add(rhs);
		return *this;
	}


	template<class T>
	inline Variant& Variant::operator -= (const T& rhs)
	{
		sub(rhs);
		return *this;
	}


	template<class T>
	inline Variant& Variant::operator *= (const T& rhs)
	{
		mult(rhs);
		return *this;
	}


	template<class T>
	inline Variant& Variant::operator /= (const T& rhs)
	{
		div(rhs);
		return *this;
	}


	inline void Variant::shareContentFrom(const NullPtr&)
	{
		pShareContent = false;
		pData = nullptr;
	}


	inline void Variant::deepCopyIfNonUnique()
	{
		// pValue must not null
		if (/*!(!pValue) &&*/ !pShareContent && !pData->unique())
		{
			Private::Variant::IDataHolder* pointer = Private::Variant::IDataHolder::Ptr::WeakPointer(pData);
			pData = pointer->clone();
		}
	}



} // namespace Yuni





inline Yuni::Variant operator + (const Yuni::Variant& lhs, const Yuni::Variant& rhs)
{
	return Yuni::Variant(lhs) += rhs;
}

inline Yuni::Variant operator - (const Yuni::Variant& lhs, const Yuni::Variant& rhs)
{
	return Yuni::Variant(lhs) -= rhs;
}

inline Yuni::Variant operator * (const Yuni::Variant& lhs, const Yuni::Variant& rhs)
{
	return Yuni::Variant(lhs) *= rhs;
}

inline Yuni::Variant operator / (const Yuni::Variant& lhs, const Yuni::Variant& rhs)
{
	return Yuni::Variant(lhs) /= rhs;
}


# define YUNI_VARIANT_OPERATOR(TYPE) \
	/*inline Yuni::Variant operator + (TYPE lhs, const Yuni::Variant& rhs) \
	{ \
		Yuni::Variant a(lhs); \
		a += rhs; \
		return a; \
	} \
	inline Yuni::Variant operator - (TYPE lhs, const Yuni::Variant& rhs) \
	{ \
		Yuni::Variant a(lhs); \
		a -= rhs; \
		return a; \
	} \
	inline Yuni::Variant operator * (TYPE lhs, const Yuni::Variant& rhs) \
	{ \
		Yuni::Variant a(lhs); \
		a *= rhs; \
		return a; \
	} \
	inline Yuni::Variant operator / (TYPE lhs, const Yuni::Variant& rhs) \
	{ \
		Yuni::Variant a(lhs); \
		a /= rhs; \
		return a; \
	} */ \
	\
	\
	inline Yuni::Variant operator + (const Yuni::Variant& lhs, TYPE rhs) \
	{ \
		return Yuni::Variant(lhs) += rhs; \
	} \
	inline Yuni::Variant operator - (const Yuni::Variant& lhs, TYPE rhs) \
	{ \
		return Yuni::Variant(lhs) -= rhs; \
	} \
	inline Yuni::Variant operator * (const Yuni::Variant& lhs, TYPE rhs) \
	{ \
		return Yuni::Variant(lhs) *= rhs; \
	} \
	inline Yuni::Variant operator / (const Yuni::Variant& lhs, TYPE rhs) \
	{ \
		return Yuni::Variant(lhs) /= rhs; \
	}


YUNI_VARIANT_OPERATOR(Yuni::uint32)
YUNI_VARIANT_OPERATOR(Yuni::sint32)
YUNI_VARIANT_OPERATOR(Yuni::uint64)
YUNI_VARIANT_OPERATOR(Yuni::sint64)
YUNI_VARIANT_OPERATOR(char)
YUNI_VARIANT_OPERATOR(bool)
YUNI_VARIANT_OPERATOR(double)
YUNI_VARIANT_OPERATOR(const Yuni::String&)

#undef YUNI_VARIANT_OPERATOR



inline std::ostream& operator << (std::ostream& out, const Yuni::Variant& rhs)
{
	out << rhs.to<Yuni::String>();
	return out;
}


#endif // __YUNI_CORE_VARIANT_VARIANT_HXX__
