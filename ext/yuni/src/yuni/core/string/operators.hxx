#ifndef __YUNI_CORE_STRING_OPERATORS_H__
# define __YUNI_CORE_STRING_OPERATORS_H__




//! \name Operator overload for stream printing
//@{

template<unsigned int SizeT, bool ExpT,bool ZeroT>
inline std::ostream& operator << (std::ostream& out, const Yuni::CString<SizeT,ExpT,ZeroT>& rhs)
{
	out.write(rhs.data(), static_cast<std::streamsize>(rhs.size()));
	return out;
}

inline std::ostream& operator << (std::ostream& out, const Yuni::UTF8::Char& rhs)
{
	rhs.write(out);
	return out;
}


namespace Yuni
{
namespace Private
{

	template<class ContainerT>
	class OStreamYuniHelper
	{
	public:
		static void Print(std::ostream& out, const ContainerT& rhs)
		{
			switch (rhs.size())
			{
				case 0:
					break;
				case 1:
					{
						out << rhs[0];
						break;
					}
				default:
					{
						const Yuni::String::Vector::const_iterator end = rhs.end();
						Yuni::String::Vector::const_iterator i = rhs.begin();
						out << *i;
						++i;
						for (; i != end; ++i)
						{
							out.write(", ", 2);
							out << *i;
						}
						break;
					}
			}
		}

	}; // class OStreamYuniHelper

} // namespace Private
} // namespace Yuni



template<class T, class AllocatorT>
std::ostream& operator << (std::ostream& out, const std::vector<T, AllocatorT>& rhs)
{
	typedef std::vector<T, AllocatorT>  SourceType;
	Yuni::Private::OStreamYuniHelper<SourceType>::Print(out, rhs);
	return out;
}


template<class T, class AllocatorT>
std::ostream& operator << (std::ostream& out, const std::list<T, AllocatorT>& rhs)
{
	typedef std::list<T, AllocatorT>  SourceType;
	Yuni::Private::OStreamYuniHelper<SourceType>::Print(out, rhs);
	return out;
}




template<unsigned int SizeT, bool ExpT,bool ZeroT>
inline Yuni::CString<SizeT,ExpT,ZeroT>
operator + (const Yuni::CString<SizeT,ExpT,ZeroT>& rhs, const char* const u)
{
	return Yuni::CString<SizeT,ExpT,ZeroT>(rhs) += u;
}

template<unsigned int SizeT, bool ExpT,bool ZeroT>
inline Yuni::CString<SizeT,ExpT,ZeroT>
operator + (const Yuni::CString<SizeT,ExpT,ZeroT>& rhs, const wchar_t* u)
{
	return Yuni::CString<SizeT,ExpT,ZeroT>(rhs) += u;
}

template<unsigned int SizeT, bool ExpT,bool ZeroT>
inline Yuni::CString<SizeT,ExpT,ZeroT>
operator + (const Yuni::CString<SizeT,ExpT,ZeroT>& rhs, const char u)
{
	return Yuni::CString<SizeT,ExpT,ZeroT>(rhs) += u;
}

template<unsigned int SizeT, bool ExpT,bool ZeroT>
inline Yuni::CString<SizeT,ExpT,ZeroT>
operator + (const Yuni::CString<SizeT,ExpT,ZeroT>& rhs, const wchar_t u)
{
	return Yuni::CString<SizeT,ExpT,ZeroT>(rhs) += u;
}



template<unsigned int SizeT, bool ExpT,bool ZeroT>
inline Yuni::CString<SizeT,ExpT,ZeroT>
operator + (const wchar_t* u, const Yuni::CString<SizeT,ExpT,ZeroT>& rhs)
{
	return Yuni::CString<SizeT,ExpT,ZeroT>(u) += rhs;
}

template<unsigned int SizeT, bool ExpT,bool ZeroT>
inline Yuni::CString<SizeT,ExpT,ZeroT>
operator + (const char* const u, const Yuni::CString<SizeT,ExpT,ZeroT>& rhs)
{
	return Yuni::CString<SizeT,ExpT,ZeroT>(u) += rhs;
}

template<unsigned int SizeT, bool ExpT,bool ZeroT>
inline Yuni::CString<SizeT,ExpT,ZeroT>
operator + (const char u, const Yuni::CString<SizeT,ExpT,ZeroT>& rhs)
{
	return Yuni::CString<SizeT,ExpT,ZeroT>(u) += rhs;
}

template<unsigned int SizeT, bool ExpT,bool ZeroT>
inline Yuni::CString<SizeT,ExpT,ZeroT>
operator + (const wchar_t u, const Yuni::CString<SizeT,ExpT,ZeroT>& rhs)
{
	return Yuni::CString<SizeT,ExpT,ZeroT>(u) += rhs;
}




template<unsigned int SizeT, bool ExpT,bool ZeroT, class TraitsT, class AllocT>
inline Yuni::CString<SizeT,ExpT,ZeroT>
operator + (const std::basic_string<char,TraitsT,AllocT>& u, const Yuni::CString<SizeT,ExpT,ZeroT>& rhs)
{
	return Yuni::CString<SizeT,ExpT,ZeroT>(u) += rhs;
}


template<unsigned int SizeT, bool ExpT,bool ZeroT, class TraitsT, class AllocT>
inline Yuni::CString<SizeT,ExpT,ZeroT>
operator + (const Yuni::CString<SizeT,ExpT,ZeroT>& rhs, const std::basic_string<char,TraitsT,AllocT>& u)
{
	return Yuni::CString<SizeT,ExpT,ZeroT>(rhs) += u;
}


template<unsigned int SizeT, bool ExpT,bool ZeroT>
inline bool
operator == (const char* lhs, const Yuni::CString<SizeT,ExpT,ZeroT>& rhs)
{
	return rhs == lhs;
}

template<unsigned int SizeT, bool ExpT,bool ZeroT>
inline bool
operator != (const char* lhs, const Yuni::CString<SizeT,ExpT,ZeroT>& rhs)
{
	return rhs != lhs;
}

template<int N, unsigned int SizeT, bool ExpT,bool ZeroT>
inline bool
operator == (const char lhs[N], const Yuni::CString<SizeT,ExpT,ZeroT>& rhs)
{
	return rhs == lhs;
}

template<int N, unsigned int SizeT, bool ExpT,bool ZeroT>
inline bool
operator != (const char lhs[N], const Yuni::CString<SizeT,ExpT,ZeroT>& rhs)
{
	return rhs != lhs;
}

template<class TraitsT, class AllocT, unsigned int SizeT, bool ExpT,bool ZeroT>
inline bool
operator == (const std::basic_string<char,TraitsT,AllocT>& lhs, const Yuni::CString<SizeT,ExpT,ZeroT>& rhs)
{
	return rhs == lhs;
}

template<class TraitsT, class AllocT, unsigned int SizeT, bool ExpT,bool ZeroT>
inline bool
operator != (const std::basic_string<char,TraitsT,AllocT>& lhs, const Yuni::CString<SizeT,ExpT,ZeroT>& rhs)
{
	return rhs != lhs;
}


//@}


#endif // __YUNI_CORE_STRING_OPERATORS_H__
