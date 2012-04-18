#ifndef __YUNI_CORE_VARIANT_VARIANT_HXX__
# define __YUNI_CORE_VARIANT_VARIANT_HXX__


namespace Yuni
{

	inline Variant::Variant() :
		pData(NULL)
	{}


	inline Variant::Variant(const Variant& rhs) :
		pData(NULL)
	{
		assign(rhs);
	}


	template <typename T>
	inline Variant::Variant(const T& rhs) :
		pData(NULL)
	{
		assign(rhs);
	}


	template <typename T>
	inline void Variant::assign(const T& rhs)
	{
		delete pData;
		pData = new Private::Variant::Data<typename Static::Remove::Const<T>::Type>(rhs);
	}


	template <typename T>
	inline T Variant::to() const
	{
		return (pData) ? pData->to<T>() : T();
	}


	inline bool Variant::empty() const
	{
		return (NULL == pData);
	}


	inline Variant & Variant::operator = (const Variant& rhs)
	{
		assign(rhs);
		return *this;
	}


	template <typename T>
	inline Variant& Variant::operator = (const T& rhs)
	{
		assign(rhs); return *this;
	}


	inline void Variant::assign(const char* rhs)
	{
		assign(String(rhs));
	}





} // namespace Yuni

#endif // __YUNI_CORE_VARIANT_VARIANT_HXX__
