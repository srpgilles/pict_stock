#ifndef __YUNI_CORE_STRING_STRING_HXX__
# define __YUNI_CORE_STRING_STRING_HXX__

# include <iostream>
# include <ctype.h>
# include <cassert>
# ifdef YUNI_HAS_VA_COPY
#	include <stdarg.h>
# endif // YUNI_HAS_VA_COPY



namespace Yuni
{

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline int CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::ToLower(int c)
	{
		if (static_cast<unsigned int>(c) - 'A' < 26)
			return c | 32;
		return c;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline int CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::ToUpper(int c)
	{
		if (static_cast<unsigned int>(c) - 'a' < 26)
			return c & 0x5f;
		return c;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::IsSpace(int c)
	{
		return (c == ' ') || (static_cast<unsigned int>(c) - '\t' < 5);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::IsDigit(int c)
	{
		return static_cast<unsigned int>(c) - '0' < 10;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::IsDigitNonZero(int c)
	{
		return static_cast<unsigned int>(c) - '1' < 9;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::IsAlpha(int c)
	{
		return (static_cast<unsigned int>(c) | 32) - 'a' < 26;
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::CString()
	{}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::~CString()
	{}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class ModelT, bool ConstT, class ModelT2, bool ConstT2>
	inline
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::CString(
		const IIterator<ModelT,ConstT>& begin, const IIterator<ModelT2,ConstT2>& end)
	{
		assign(begin, end);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class ModelT, bool ConstT, class ModelT2, bool ConstT2, class StringT>
	inline
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::CString(
		const IIterator<ModelT,ConstT>& begin, const IIterator<ModelT2,ConstT2>& end, const StringT& separator)
	{
		assign(begin, end, separator);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::CString(const CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>& rhs)
		:AncestorType(rhs)
	{}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class U>
	inline CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::CString(const U& rhs)
	{
		assign(rhs);
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::CString(size_t n, char c)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		// Resizing the buffer to `n`
		resize(n);
		// Note: the string may have a fixed-length capacity
		(void)::memset(AncestorType::data, c, AncestorType::size * sizeof(Char));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::CString(size_t n, unsigned char c)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		// Resizing the buffer to `n`
		resize(n);
		// Note: the string may have a fixed-length capacity
		(void)::memset(AncestorType::data, static_cast<char>(c), AncestorType::size * sizeof(Char));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<unsigned int SizeT, bool ExpT, bool ZeroT>
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::CString(const CString<SizeT,ExpT,ZeroT>& s,
		Size offset, Size n)
	{
		if (offset < s.size())
		{
			Size length = s.size() - offset;
			if (length > n)
				length = n;
			if (!adapter)
				AncestorType::assign((const char* const) s.c_str() + offset, length);
			else
				adapt(s.c_str() + offset, length);
		}
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<unsigned int SizeT, bool ExpT, bool ZeroT>
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::CString(const CString<SizeT,ExpT,ZeroT>& s, Size offset)
	{
		if (offset < s.size())
		{
			if (!adapter)
				AncestorType::assign((const char* const) s.c_str() + offset, s.size() - offset);
			else
				adapt(s.c_str() + offset, s.size() - offset);
		}
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class TraitsT, class AllocT>
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::CString(const std::basic_string<char,TraitsT,AllocT>& s,
		Size offset, Size n)
	{
		if (offset < s.size())
		{
			Size length = s.size() - offset;
			if (length > n)
				length = n;
			if (!adapter)
				AncestorType::assign((const char* const) s.c_str() + offset, length);
			else
				adapt(s.c_str() + offset, length);
		}
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class TraitsT, class AllocT>
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::CString(const std::basic_string<char,TraitsT,AllocT>& s, Size offset)
	{
		if (offset < s.size())
		{
			if (!adapter)
				AncestorType::assign((const char* const) s.c_str() + offset, s.size() - offset);
			else
				adapt(s.c_str() + offset, s.size() - offset);
		}
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::CString(const char* const cstring, Size blockSize)
	{
		if (!adapter)
			AncestorType::assign(cstring, blockSize);
		else
			adapt(cstring, blockSize);
	}




	namespace
	{
		template<bool ValidT, bool AdapterT>
		struct TraitsSelectorAssign
		{
			template<class U, class CStringT>
			static inline void Perform(const U& u, CStringT& string)
			{
				string.assign(Traits::CString<U>::Perform(u), Traits::Length<U,typename CStringT::Size>::Value(u));
			};
		};

		template<> struct TraitsSelectorAssign<true, true>
		{
			template<class U, class CStringT>
			static inline void Perform(const U& u, CStringT& string)
			{
				string.adapt(Traits::CString<U>::Perform(u), Traits::Length<U,typename CStringT::Size>::Value(u));
			};
		};


		template<bool AdapterT> struct TraitsSelectorAssign<false, AdapterT>
		{
			template<class U, class CStringT>
			static inline void Perform(const U& u, CStringT& string)
			{
				YUNI_STATIC_ASSERT(!AdapterT, CString_Adapter_ReadOnly);
				typedef typename Static::Remove::Const<U>::Type UType;
				Yuni::Extension::CString::Assign<CStringT, UType>::Perform(string, u);
			}
		};

		template<bool ValidT>
		struct TraitsSelectorAppend
		{
			template<class U, class CStringT>
			static inline void Perform(const U& u, CStringT& string)
			{
				string.append(Traits::CString<U>::Perform(u), Traits::Length<U,typename CStringT::Size>::Value(u));
			};
		};

		template<> struct TraitsSelectorAppend<false>
		{
			template<class U, class CStringT>
			static inline void Perform(const U& u, CStringT& string)
			{
				typedef typename Static::Remove::Const<U>::Type UType;
				Yuni::Extension::CString::Append<CStringT, UType>::Perform(string, u);
			}
		};

	} // anonymous namespace



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class U>
	inline
	void CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::assign(const U& u)
	{
		TraitsSelectorAssign<
			(Traits::CString<U>::valid && Traits::Length<U>::valid),             // Standard CString ?
			(0 != CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::adapter) // Adapter ?
			>::
			template Perform<U, CStringType>(u, *this);
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class U>
	inline
	void CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::append(const U& u)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		TraitsSelectorAppend<Traits::CString<U>::valid && Traits::Length<U>::valid>::
			template Perform<U, CStringType>(u, *this);
	}


	template<class T>
	struct AppendIterator
	{
		template<class StringT, class IteratorT, class IteratorT2>
		static void Perform(StringT& s, const IteratorT& begin, const IteratorT2& end)
		{
			for (IteratorT i = begin; i != end; ++i)
				s << *i;
		}

		template<class StringT, class IteratorT, class IteratorT2, class SeparatorT>
		static void Perform(StringT& s, const IteratorT& begin, const IteratorT2& end, const SeparatorT& separator)
		{
			if (begin != end)
			{
				s << *begin;
				IteratorT i = begin;
				++i;
				for (; i != end; ++i)
					s << separator << *i;
			}
		}

		template<class StringT, class IteratorT, class IteratorT2, class SeparatorT, class EnclosureT>
		static void Perform(StringT& s, const IteratorT& begin, const IteratorT2& end, const SeparatorT& separator,
			const EnclosureT& enclosure)
		{
			if (begin != end)
			{
				s << enclosure << *begin << enclosure;
				IteratorT i = begin;
				++i;
				for (; i != end; ++i)
					s << separator << enclosure << *i << enclosure;
			}
		}

	}; // class AppendIterator


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class ModelT, bool ConstT, class ModelT2, bool ConstT2>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::assign(const IIterator<ModelT,ConstT>& begin,
		const IIterator<ModelT2,ConstT2>& end)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		typedef typename IIterator<ModelT,ConstT>::value_type  HeldType;
		clear();
		AppendIterator<HeldType>::Perform(*this, begin, end);
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class ModelT, bool ConstT, class ModelT2, bool ConstT2, class StringT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::assign(const IIterator<ModelT,ConstT>& begin,
		const IIterator<ModelT2,ConstT2>& end, const StringT& separator)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		typedef typename IIterator<ModelT,ConstT>::value_type  HeldType;
		clear();
		AppendIterator<HeldType>::Perform(*this, begin, end, separator);
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class ModelT, bool ConstT, class ModelT2, bool ConstT2, class StringT, class EnclosureT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::assign(const IIterator<ModelT,ConstT>& begin,
		const IIterator<ModelT2,ConstT2>& end, const StringT& separator, const EnclosureT& enclosure)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		typedef typename IIterator<ModelT,ConstT>::value_type  HeldType;
		clear();
		AppendIterator<HeldType>::Perform(*this, begin, end, separator, enclosure);
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class ModelT, bool ConstT, class ModelT2, bool ConstT2>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::append(const IIterator<ModelT,ConstT>& begin,
		const IIterator<ModelT2,ConstT2>& end)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		typedef typename IIterator<ModelT,ConstT>::value_type  HeldType;
		AppendIterator<HeldType>::Perform(*this, begin, end);
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class ModelT, bool ConstT, class ModelT2, bool ConstT2, class StringT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::append(const IIterator<ModelT,ConstT>& begin,
		const IIterator<ModelT2,ConstT2>& end, const StringT& separator)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		typedef typename IIterator<ModelT,ConstT>::value_type  HeldType;
		AppendIterator<HeldType>::Perform(*this, begin, end, separator);
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class ModelT, bool ConstT, class ModelT2, bool ConstT2, class StringT, class EnclosureT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::append(const IIterator<ModelT,ConstT>& begin,
		const IIterator<ModelT2,ConstT2>& end, const StringT& separator, const EnclosureT& enclosure)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		typedef typename IIterator<ModelT,ConstT>::value_type  HeldType;
		AppendIterator<HeldType>::Perform(*this, begin, end, separator, enclosure);
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::append(const StringT& s, Size size)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);

		AncestorType::append(Traits::CString<StringT>::Perform(s), size);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::append(const StringT& str, Size size, Size offset)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		# ifndef NDEBUG
		Size len = Traits::Length<StringT,Size>::Value(str);
		assert(size + offset <= len && "Bound check error in CString::append(s, size, offset) !");
		# endif // NDEBUG
		AncestorType::append(Traits::CString<StringT>::Perform(str) + offset, size);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class U>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::write(const U& u, Size size)
	{
		append(u, size);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class U>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::write(const U& u)
	{
		append(u);
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class U>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::push_back(const U& u)
	{
		append(u);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class U>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::push_front(const U& u)
	{
		insert(0, u);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::assign(const StringT& str, Size size)
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);

		if (!adapter)
			AncestorType::assign(Traits::CString<StringT>::Perform(str), size);
		else
			adapt(Traits::CString<StringT>::Perform(str), size);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::assign(const StringT& str, Size size, Size offset)
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		# ifndef NDEBUG
		Size len = Traits::Length<StringT,Size>::Value(str);
		assert(size + offset <= len && "Buffer overflow in CString::assign(s, size, offset) !");
		# endif // NDEBUG
		if (!adapter)
			AncestorType::assign(Traits::CString<StringT>::Perform(str) + offset, size);
		else
			adapt(Traits::CString<StringT>::Perform(str), size);
	}




	namespace
	{
		/*!
		** \brief Find the end of a sequence, started and terminated by a given character (usually a quote)
		**
		** This method is not a simple find(), because it takes care of escaped
		** characters
		**
		** \param str The sequence
		** \param quote The character to find, usually a quote
		*/
		template <class StringT>
		typename StringT::Size
		FindEndOfSequence(const char* str, char quote, typename StringT::Size maxLen)
		{
			if (str)
			{
				typename StringT::Size pos(0);
				while (pos < maxLen)
				{
					char c = str[pos];
					if ('\\' == c)
					{
						pos += 2;
						continue;
					}
					else
					{
						if (quote == c)
							return pos;
						++pos;
					}
				}
			}
			return StringT::npos;
		}

	} // anonymous namespace

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::assignFromEscapedCharacters(const char* const str,
		Size maxLen, Size offset)
	{
		clear();
		if (!maxLen || offset >= maxLen)
			return;

		// Preparing the copy
		AncestorType::size = maxLen - offset;
		AncestorType::reserve(AncestorType::size);

		// Browsing all char
		Size retPos(0);
		for (Size i = offset; i < maxLen; ++i, ++retPos)
		{
			if ('\\' == str[i] && i + 1 != maxLen)
			{
				switch (str[i + 1])
				{
					case 'r'  : AncestorType::data[retPos] = '\r'; break;
					case 'n'  : AncestorType::data[retPos] = '\n'; break;
					case '\\' : AncestorType::data[retPos] = '\\'; break;
					case ';'  : AncestorType::data[retPos] = ';'; break;
					case 'a'  : AncestorType::data[retPos] = '\a'; break;
					case 'f'  : AncestorType::data[retPos] = '\f'; break;
					case 't'  : AncestorType::data[retPos] = '\t'; break;
					case '\'' : AncestorType::data[retPos] = '\''; break;
					case '"'  : AncestorType::data[retPos] = '"'; break;
					default   : AncestorType::data[retPos] = str[i]; continue;
				}
				--(AncestorType::size);
				++i;
				continue;
			}
			AncestorType::data[retPos] = str[i];
		}
		if (zeroTerminated)
			AncestorType::data[AncestorType::size] = '\0';
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>&
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::clear()
	{
		if (!adapter)
			AncestorType::clear();
		else
			AncestorType::size = 0;
		return *this;
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::reserve(Size minCapacity)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		AncestorType::reserve(minCapacity);
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::put(char c)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		AncestorType::put(c);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::put(const unsigned char c)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		AncestorType::put(static_cast<char>(c));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class U>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::put(const U& rhs)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		append(rhs);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	void CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::fill(const StringT& pattern)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		typedef typename Static::Remove::Const<StringT>::Type UType;
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		if (AncestorType::size)
		{
			Yuni::Extension::CString::Fill<CString, UType>::
				Perform(AncestorType::data, AncestorType::size, pattern);
		}
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	void CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::fill(Size offset, const StringT& pattern)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		typedef typename Static::Remove::Const<StringT>::Type UType;
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		if (offset < AncestorType::size)
		{
			Yuni::Extension::CString::Fill<CString, UType>::
				Perform(AncestorType::data + offset, AncestorType::size - offset, pattern);
		}
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::hasChar(char c) const
	{
		for (Size i = 0; i != AncestorType::size; ++i)
		{
			if (c == AncestorType::data[i])
				return true;
		}
		return false;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	unsigned int
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::countChar(char c) const
	{
		unsigned int r = 0;
		for (Size i = 0; i != AncestorType::size; ++i)
		{
			if (c == AncestorType::data[i])
				++r;
		}
		return r;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::hasChar(unsigned char c) const
	{
		return hasChar(static_cast<char>(c));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	unsigned int
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::countChar(unsigned char c) const
	{
		return countChar(static_cast<char>(c));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::find(char c) const
	{
		for (Size i = 0; i != AncestorType::size; ++i)
		{
			if (c == AncestorType::data[i])
				return i;
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::find(char c, Size offset) const
	{
		for (Size i = offset; i < AncestorType::size; ++i)
		{
			if (c == AncestorType::data[i])
				return i;
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::find(const char* const cstr, Size offset, Size len) const
	{
		if (cstr && len && len <= AncestorType::size)
		{
			Size end = AncestorType::size - len + 1;
			for (Size i = offset; i < end; ++i)
			{
				if (AncestorType::data[i] == *cstr)
				{
					if (!::memcmp(AncestorType::data + i, cstr, len))
						return i;
				}
			}
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::find(const StringT& s, Size offset) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		// Find the substring
		if (Traits::Length<StringT,Size>::isFixed)
		{
			// We can make some optimisations when the length is known at compile compile time
			// This part of the code should not bring better performances but it should
			// prevent against bad uses of the API, like using a char* for looking for a single char.

			// The value to find is actually empty, npos will be the unique answer
			if (0 == Traits::Length<StringT,Size>::fixedLength)
				return npos;
			// The string is actually a single POD item
			if (1 == Traits::Length<StringT,Size>::fixedLength)
				return find(Traits::CString<StringT>::Perform(s), offset, 1);
			// Researching for the substring with a known length
			return find(Traits::CString<StringT>::Perform(s), offset, Traits::Length<StringT,Size>::fixedLength);
		}
		// A mere CString, with a known length at runtime only
		return find(Traits::CString<StringT>::Perform(s), offset, Traits::Length<StringT,Size>::Value(s));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::ifind(char c) const
	{
		c = static_cast<char>(ToLower(c));
		for (Size i = 0; i != AncestorType::size; ++i)
		{
			if (c == static_cast<char>(ToLower(AncestorType::data[i])))
				return i;
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::ifind(char c, Size offset) const
	{
		c = static_cast<char>(ToLower(c));
		for (Size i = offset; i < AncestorType::size; ++i)
		{
			if (c == static_cast<char>(ToLower(AncestorType::data[i])))
				return i;
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::ifind(const char* const cstr,
		Size offset, Size len) const
	{
		if (cstr && len && len <= AncestorType::size)
		{
			Size end = AncestorType::size - len + 1;
			for (Size i = offset; i < end; ++i)
			{
				if (ToLower(AncestorType::data[i]) == ToLower(*cstr))
				{
					if (!CompareInsensitive(AncestorType::data + i, len, cstr, len))
						return i;
				}
			}
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::ifind(const StringT& s, Size offset) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		// Find the substring
		if (Traits::Length<StringT,Size>::isFixed)
		{
			// We can make some optimisations when the length is known at compile compile time
			// This part of the code should not bring better performances but it should
			// prevent against bad uses of the API, like using a char* for looking for a single char.

			// The value to find is actually empty, npos will be the unique answer
			if (0 == Traits::Length<StringT,Size>::fixedLength)
				return npos;
			// The string is actually a single POD item
			if (1 == Traits::Length<StringT,Size>::fixedLength)
				return ifind(Traits::CString<StringT>::Perform(s), offset, 1);
			// Researching for the substring with a known length
			return ifind(Traits::CString<StringT>::Perform(s), offset, Traits::Length<StringT,Size>::fixedLength);
		}
		// A mere CString, with a known length at runtime only
		return ifind(Traits::CString<StringT>::Perform(s), offset, Traits::Length<StringT,Size>::Value(s));
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::rfind(char c) const
	{
		Size i = AncestorType::size;
		while (i--)
		{
			if (c == AncestorType::data[i])
				return i;
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::rfind(char c, Size offset) const
	{
		Size i = (offset >= AncestorType::size) ? AncestorType::size : 1+offset;
		while (i--)
		{
			if (c == AncestorType::data[i])
				return i;
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::rfind(const char* const cstr, Size offset, Size len) const
	{
		if (len && len <= AncestorType::size && offset >= len)
		{
			Size i = (offset >= AncestorType::size) ? AncestorType::size : 1+offset;
			i -= len - 1;
			while (i--)
			{
				if (AncestorType::data[i] == *cstr)
				{
					if (!::memcmp(AncestorType::data + i, cstr, len))
						return i;
				}
			}
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::rfind(const StringT& s, Size offset) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		// Find the substring
		if (Traits::Length<StringT,Size>::isFixed)
		{
			// We can make some optimisations when the length is known at compile compile time
			// This part of the code should not bring better performances but it should
			// prevent against bad uses of the API, like using a char* for looking for a single char.

			// The value to find is actually empty, npos will be the unique answer
			if (0 == Traits::Length<StringT,Size>::fixedLength)
				return npos;
			// The string is actually a single POD item
			if (1 == Traits::Length<StringT,Size>::fixedLength)
				return rfind(Traits::CString<StringT>::Perform(s), offset, 1);
			// Researching for the substring with a known length
			return rfind(Traits::CString<StringT>::Perform(s), offset, Traits::Length<StringT,Size>::fixedLength);
		}
		// A mere CString, with a known length at runtime only
		return rfind(Traits::CString<StringT>::Perform(s), offset, Traits::Length<StringT,Size>::Value(s));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::irfind(char c) const
	{
		c = static_cast<char>(ToLower(c));
		Size i = AncestorType::size;
		while (i--)
		{
			if (c == static_cast<char>(ToLower(AncestorType::data[i])))
				return i;
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::irfind(char c, Size offset) const
	{
		c = static_cast<char>(ToLower(c));
		Size i = (offset >= AncestorType::size) ? AncestorType::size : 1+offset;
		while (i--)
		{
			if (c == static_cast<char>(ToLower(AncestorType::data[i])))
				return i;
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::irfind(const char* const cstr,
		Size offset, Size len) const
	{
		if (len && len <= AncestorType::size && offset >= len)
		{
			Size i = (offset >= AncestorType::size) ? AncestorType::size : 1+offset;
			i -= len - 1;
			while (i--)
			{
				if (ToLower(AncestorType::data[i]) == ToLower(*cstr))
				{
					if (!CompareInsensitive(AncestorType::data + i, len, cstr, len))
						return i;
				}
			}
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::irfind(const StringT& s, Size offset) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		// Find the substring
		if (Traits::Length<StringT,Size>::isFixed)
		{
			// We can make some optimisations when the length is known at compile compile time
			// This part of the code should not bring better performances but it should
			// prevent against bad uses of the API, like using a char* for looking for a single char.

			// The value to find is actually empty, npos will be the unique answer
			if (0 == Traits::Length<StringT,Size>::fixedLength)
				return npos;
			// The string is actually a single POD item
			if (1 == Traits::Length<StringT,Size>::fixedLength)
				return irfind(Traits::CString<StringT>::Perform(s), offset, 1);
			// Researching for the substring with a known length
			return irfind(Traits::CString<StringT>::Perform(s), offset, Traits::Length<StringT,Size>::fixedLength);
		}
		// A mere CString, with a known length at runtime only
		return irfind(Traits::CString<StringT>::Perform(s), offset, Traits::Length<StringT,Size>::Value(s));
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::contains(char c) const
	{
		for (Size i = 0; i != AncestorType::size; ++i)
		{
			if (c == AncestorType::data[i])
				return true;
		}
		return false;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::contains(const char* const cstr, Size len) const
	{
		if (cstr && len && len <= AncestorType::size)
		{
			Size end = AncestorType::size - len + 1;
			for (Size i = 0; i != end; ++i)
			{
				if (AncestorType::data[i] == *cstr)
				{
					if (!::memcmp(AncestorType::data + i, cstr, len))
						return true;
				}
			}
		}
		return false;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::contains(const StringT& s) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		// Find the substring
		if (Traits::Length<StringT,Size>::isFixed)
		{
			// We can make some optimisations when the length is known at compile compile time
			// This part of the code should not bring better performances but it should
			// prevent against bad uses of the API, like using a char* for looking for a single char.

			// The value to find is actually empty, npos will be the unique answer
			if (0 == Traits::Length<StringT,Size>::fixedLength)
				return false;
			// The string is actually a single POD item
			if (1 == Traits::Length<StringT,Size>::fixedLength)
				return contains(Traits::CString<StringT>::Perform(s), 1);
			// Researching for the substring with a known length
			return contains(Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::fixedLength);
		}
		// A mere CString, with a known length at runtime only
		return contains(Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::Value(s));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::icontains(char c) const
	{
		c = static_cast<char>(ToLower(c));
		for (Size i = 0; i != AncestorType::size; ++i)
		{
			if (c == static_cast<char>(ToLower(AncestorType::data[i])))
				return true;
		}
		return false;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::icontains(const char* const cstr, Size len) const
	{
		if (cstr && len && len <= AncestorType::size)
		{
			Size end = AncestorType::size - len + 1;
			for (Size i = 0; i != end; ++i)
			{
				if (ToLower(AncestorType::data[i]) == ToLower(*cstr))
				{
					if (!CompareInsensitive(AncestorType::data + i, len, cstr, len))
						return true;
				}
			}
		}
		return false;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::icontains(const StringT& s) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		// Find the substring
		if (Traits::Length<StringT,Size>::isFixed)
		{
			// We can make some optimisations when the length is known at compile compile time
			// This part of the code should not bring better performances but it should
			// prevent against bad uses of the API, like using a char* for looking for a single char.

			// The value to find is actually empty, npos will be the unique answer
			if (0 == Traits::Length<StringT,Size>::fixedLength)
				return false;
			// The string is actually a single POD item
			if (1 == Traits::Length<StringT,Size>::fixedLength)
				return icontains(Traits::CString<StringT>::Perform(s), 1);
			// Researching for the substring with a known length
			return icontains(Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::fixedLength);
		}
		// A mere CString, with a known length at runtime only
		return icontains(Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::Value(s));
	}





	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	unsigned int
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::indexOf(Size offset, char c) const
	{
		for (; offset < AncestorType::size; ++offset)
		{
			if (c == AncestorType::data[offset])
				return offset;
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::startsWith(const char* const cstr, Size len) const
	{
		return (cstr && len && len <= AncestorType::size)
			? (0 == ::memcmp(AncestorType::data, cstr, len))
			: false;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::startsWith(const StringT& s) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		return startsWith(Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::Value(s));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::startsWith(char c) const
	{
		return (0 != AncestorType::size) && (AncestorType::data[0] == c);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::istartsWith(const char* const cstr, Size len) const
	{
		if (cstr && len && len <= AncestorType::size)
		{
			for (unsigned int i = 0; i != len; ++i)
			{
				if (ToLower(cstr[i]) != ToLower(AncestorType::data[i]))
					return false;
			}
			return true;
		}
		return false;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::istartsWith(const StringT& s) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		return startsWith(Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::Value(s));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::istartsWith(char c) const
	{
		return (0 != AncestorType::size) && (ToLower(AncestorType::data[0]) == ToLower(c));
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::endsWith(const char* const cstr, Size len) const
	{
		return (cstr && len && len <= AncestorType::size)
			? (0 == ::memcmp(AncestorType::data + (AncestorType::size - len), cstr, len))
			: false;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::endsWith(const StringT& s) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		return endsWith(Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::Value(s));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::endsWith(char c) const
	{
		return (0 != AncestorType::size) && (AncestorType::data[AncestorType::size - 1] == c);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::iendsWith(const char* const cstr, Size len) const
	{
		if (cstr && len && len <= AncestorType::size)
		{
			unsigned int offset = 0;
			for (unsigned int i = AncestorType::size - len; i != AncestorType::size; ++i, ++offset)
			{
				if (ToLower(cstr[offset]) != ToLower(AncestorType::data[i]))
					return false;
			}
			return true;
		}
		return false;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::iendsWith(const StringT& s) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		return endsWith(Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::Value(s));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::iendsWith(char c) const
	{
		return (0 != AncestorType::size) && (ToLower(AncestorType::data[AncestorType::size - 1]) == ToLower(c));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::removeLast()
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		if (AncestorType::size != 0)
		{
			--(AncestorType::size);
			if (zeroTerminated)
				AncestorType::data[AncestorType::size] = Char();
		}
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::removeTrailingSlash()
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		if (AncestorType::size != 0)
		{
			if (('/' == AncestorType::data[AncestorType::size - 1] || '\\' == AncestorType::data[AncestorType::size - 1]))
			{
				--AncestorType::size;
				if (zeroTerminated)
					AncestorType::data[AncestorType::size] = Char();
			}
		}
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::chop(unsigned int n)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		if (AncestorType::size >= n)
		{
			AncestorType::size -= n;
			if (zeroTerminated)
				AncestorType::data[AncestorType::size] = Char();
		}
		else
		{
			AncestorType::size = 0;
			if (zeroTerminated && AncestorType::capacity)
				AncestorType::data[0] = Char();
		}
	}




	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::glob(const StringT& pattern) const
	{
		// TODO This method should be completly removed
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		return Yuni::Private::CStringImpl::Glob(AncestorType::data, AncestorType::size,
			Traits::CString<StringT>::Perform(pattern), Traits::Length<StringT,Size>::Value(pattern));
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	unsigned int
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::indexOf(Size offset,
		const char* const cstr, Size len) const
	{
		if (cstr && len && len <= AncestorType::size)
		{
			Size end = AncestorType::size - len + 1;
			for (; offset < end; ++offset)
			{
				if (AncestorType::data[offset] == *cstr)
				{
					if (!::memcmp(AncestorType::data + offset, cstr, len))
						return offset;
				}
			}
		}
		return npos;
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	unsigned int
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::indexOf(Size offset, const StringT& s) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		// Find the substring
		if (Traits::Length<StringT,Size>::isFixed)
		{
			// We can make some optimisations when the length is known at compile compile time
			// This part of the code should not bring better performances but it should
			// prevent against bad uses of the API, like using a char* for looking for a single char.

			// The value to find is actually empty, npos will be the unique answer
			if (0 == Traits::Length<StringT,Size>::fixedLength)
				return npos;
			// The string is actually a single POD item
			if (1 == Traits::Length<StringT,Size>::fixedLength)
				return indexOf(offset, Traits::CString<StringT>::Perform(s), 1);
			// Researching for the substring with a known length
			return indexOf(offset, Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::fixedLength);
		}
		// A mere CString, with a known length at runtime only
		return indexOf(offset, Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::Value(s));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::find_first_of(char c, Size offset) const
	{
		return indexOf(offset, c);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::ifind_first_of(char c, Size offset) const
	{
		c = static_cast<char>(ToLower(c));
		for (Size i = offset; i < AncestorType::size; ++i)
		{
			// alias to the current character
			if (c == static_cast<char>(ToLower(AncestorType::data[i])))
				return i;
		}
		return npos;
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::find_first_of(const StringT& seq, Size offset) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		// The given sequence
		const char* const s = Traits::CString<StringT>::Perform(seq);
		Size len = Traits::Length<StringT,Size>::Value(seq);
		Size j;

		for (Size i = offset; i < AncestorType::size; ++i)
		{
			// alias to the current character
			char c = AncestorType::data[i];
			for (j = 0; j != len; ++j)
			{
				if (s[j] == c)
					return i;
			}
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::find_first_not_of(char c, Size offset) const
	{
		for (Size i = offset; i < AncestorType::size; ++i)
		{
			// alias to the current character
			if (c == AncestorType::data[i])
				continue;
			else
				return i;
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::ifind_first_not_of(char c, Size offset) const
	{
		c = static_cast<char>(ToLower(c));
		for (Size i = offset; i < AncestorType::size; ++i)
		{
			// alias to the current character
			if (c == static_cast<char>(ToLower(AncestorType::data[i])))
				continue;
			else
				return i;
		}
		return npos;
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::find_first_not_of(const StringT& seq, Size offset) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		// The given sequence
		const char* const s = Traits::CString<StringT>::Perform(seq);
		Size len = Traits::Length<StringT,Size>::Value(seq);
		Size j;

		for (Size i = offset; i < AncestorType::size; ++i)
		{
			bool stop = true;
			// alias to the current character
			char c = AncestorType::data[i];
			for (j = 0; j != len; ++j)
			{
				if (s[j] == c)
				{
					stop = false;
					break;
				}
			}
			if (stop)
				return i;

		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::ifind_first_of(const StringT& seq, Size offset) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		// The given sequence
		const char* const s = Traits::CString<StringT>::Perform(seq);
		Size len = Traits::Length<StringT,Size>::Value(seq);
		Size j;

		for (Size i = offset; i < AncestorType::size; ++i)
		{
			// alias to the current character
			char c = static_cast<char>(ToLower(AncestorType::data[i]));
			for (j = 0; j != len; ++j)
			{
				if (static_cast<char>(ToLower(s[j])) == c)
					return i;
			}
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::ifind_first_not_of(const StringT& seq, Size offset) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		// The given sequence
		const char* const s = Traits::CString<StringT>::Perform(seq);
		Size len = Traits::Length<StringT,Size>::Value(seq);
		Size j;

		for (Size i = offset; i < AncestorType::size; ++i)
		{
			bool stop = true;
			// alias to the current character
			char c = static_cast<char>(ToLower(AncestorType::data[i]));
			for (j = 0; j != len; ++j)
			{
				if (static_cast<char>(ToLower(s[j])) == c)
				{
					stop = false;
					break;
				}
			}
			if (stop)
				return i;
		}
		return npos;
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::find_last_of(char c) const
	{
		Size i = AncestorType::size;
		while (i--)
		{
			if (c == AncestorType::data[i])
				return i;
		}
		return npos;
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::ifind_last_of(char c) const
	{
		c = static_cast<char>(ToLower(c));
		Size i = AncestorType::size;
		while (i--)
		{
			if (c == static_cast<char>(ToLower(AncestorType::data[i])))
				return i;
		}
		return npos;
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::find_last_of(char c, Size offset) const
	{
		c = static_cast<char>(ToLower(c));
		Size i = ((offset >= AncestorType::size) ? AncestorType::size : 1+offset);
		while (i--)
		{
			if (c == static_cast<char>(ToLower(AncestorType::data[i])))
				return i;
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::find_last_of(const StringT& seq, Size offset) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		// The given sequence
		const char* const s = Traits::CString<StringT>::Perform(seq);
		Size len = Traits::Length<StringT,Size>::Value(seq);
		Size j;

		Size i = ((offset >= AncestorType::size) ? AncestorType::size : 1+offset);
		while (i--)
		{
			// alias to the current character
			char c = AncestorType::data[i];
			for (j = 0; j != len; ++j)
			{
				if (s[j] == c)
					return i;
			}
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::ifind_last_of(const StringT& seq, Size offset) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		// The given sequence
		const char* const s = Traits::CString<StringT>::Perform(seq);
		Size len = Traits::Length<StringT,Size>::Value(seq);
		Size j;

		Size i = ((offset >= AncestorType::size) ? AncestorType::size : 1+offset);
		while (i--)
		{
			// alias to the current character
			char c = static_cast<char>(ToLower(AncestorType::data[i]));
			for (j = 0; j != len; ++j)
			{
				if (static_cast<char>(ToLower(s[j])) == c)
					return i;
			}
		}
		return npos;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::replace(char from, char to)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		for (Size i = 0; i != AncestorType::size; ++i)
		{
			if (from == AncestorType::data[i])
				AncestorType::data[i] = to;
		}
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::ireplace(char from, char to)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		from = static_cast<char>(ToLower(from));
		for (Size i = 0; i != AncestorType::size; ++i)
		{
			if (from == AncestorType::data[i])
				AncestorType::data[i] = to;
		}
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::replace(Size offset, char from, char to)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		for (Size i = offset; i != AncestorType::size; ++i)
		{
			if (from == AncestorType::data[i])
				AncestorType::data[i] = to;
		}
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT1, class StringT2>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::replace(const StringT1& from, const StringT2& to)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		replace<StringT1, StringT2>(0, from, to);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT1, class StringT2>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::ireplace(const StringT1& from, const StringT2& to)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		ireplace<StringT1, StringT2>(0, from, to);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT1, class StringT2>
	void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::replace(Size offset, const StringT1& from, const StringT2& to)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		YUNI_STATIC_ASSERT(Traits::CString<StringT1>::valid, CString_InvalidTypeForBuffer1);
		YUNI_STATIC_ASSERT(Traits::CString<StringT2>::valid, CString_InvalidTypeForBuffer2);
		YUNI_STATIC_ASSERT(Traits::Length<StringT1>::valid,  CString_InvalidTypeForBufferSize1);
		YUNI_STATIC_ASSERT(Traits::Length<StringT2>::valid,  CString_InvalidTypeForBufferSize2);

		Size lenfrom = Traits::Length<StringT1,Size>::Value(from);
		if (lenfrom && offset < AncestorType::size)
		{
			Size lento = Traits::Length<StringT2,Size>::Value(to);
			Size pos;
			do
			{
				pos = find(from, offset);
				if (pos == npos)
					return;
				erase(pos, lenfrom);
				insert(pos, to);
				offset = pos + lento;
			}
			while (offset < AncestorType::size);
		}
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT1, class StringT2>
	void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::ireplace(Size offset, const StringT1& from, const StringT2& to)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		YUNI_STATIC_ASSERT(Traits::CString<StringT1>::valid, CString_InvalidTypeForBuffer1);
		YUNI_STATIC_ASSERT(Traits::CString<StringT2>::valid, CString_InvalidTypeForBuffer2);
		YUNI_STATIC_ASSERT(Traits::Length<StringT1>::valid,  CString_InvalidTypeForBufferSize1);
		YUNI_STATIC_ASSERT(Traits::Length<StringT2>::valid,  CString_InvalidTypeForBufferSize2);

		Size lenfrom = Traits::Length<StringT1,Size>::Value(from);
		if (lenfrom && offset < AncestorType::size)
		{
			Size lento = Traits::Length<StringT2,Size>::Value(to);
			Size pos;
			do
			{
				pos = ifind(from, offset);
				if (pos == npos)
					return;
				erase(pos, lenfrom);
				insert(pos, to);
				offset = pos + lento;
			}
			while (offset < AncestorType::size);
		}
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::ireplace(Size offset, char from, char to)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		from = static_cast<char>(ToLower(from));
		for (Size i = offset; i < AncestorType::size; ++i)
		{
			if (from == AncestorType::data[i])
				AncestorType::data[i] = to;
		}
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::consume(Size n)
	{
		if (n)
			Yuni::Private::CStringImpl::Consume<CStringType, adapter>::Perform(*this, n);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class U>
	inline U
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::to() const
	{
		typedef typename Static::Remove::Const<U>::Type UType;
		return Yuni::Extension::CString::Into<UType>::Perform(*this);
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class U>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::to(U& out) const
	{
		typedef typename Static::Remove::Const<U>::Type UType;
		return Yuni::Extension::CString::Into<UType>::Perform(*this, out);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class ModelT, bool ConstT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::erase(const IIterator<ModelT,ConstT>& it, Size len)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		erase(it.offset(), len);
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::erase(Size offset, Size len)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		// Only valid if the offset is strictly less than the current size
		if (offset < AncestorType::size && len)
		{
			// If the number of items to erase is greater than the current size
			// then all characters after 'offset' are dead
			if (offset + len > AncestorType::size)
				AncestorType::size = offset;
			else
			{
				// Actually we have to erase a part of the cstr
				(void)::memmove(AncestorType::data + sizeof(Char) * (offset),
					AncestorType::data + sizeof(Char) * (offset + len), sizeof(Char) * (AncestorType::size - offset - len));
				// Reducing the cstr's size
				AncestorType::size -= len;
			}
			if (zeroTerminated)
				AncestorType::data[AncestorType::size] = Char();
		}
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline int
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::at(Size offset) const
	{
		return (offset < AncestorType::size) ? AncestorType::data[offset] : 0;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class ModelT, bool ConstT, class StringT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::insert(const IIterator<ModelT,ConstT>& it, const StringT& string)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		insert(it.offset(), string);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::insert(Size offset, const StringT& s)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		if (Traits::Length<StringT,Size>::isFixed)
		{
			// We can make some optimisations when the length is known at compile compile time
			// This part of the code should not bring better performances but it should
			// prevent against bad uses of the API, like using a char* for looking for a single char.

			// The value to find is actually empty, false will be the unique answer
			if (0 == Traits::Length<StringT,Size>::fixedLength)
				return false;
			// The string is actually a single POD item
			if (1 == Traits::Length<StringT,Size>::fixedLength)
				return insert(offset, Traits::CString<StringT>::Perform(s), 1);
			// Researching for the substring with a known length
			return insert(offset, Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::fixedLength);
		}

		return insert(offset, Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::Value(s));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::insert(Size offset, const StringT& s, Size size)
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);

		return insert(offset, Traits::CString<StringT>::Perform(s), size);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::insert(Size offset, const char* const cstr, Size size)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		if (size > 0)
		{
			if (offset >= AncestorType::size)
			{
				append(cstr, size);
				return true;
			}
			AncestorType::insert(offset, cstr, size);
			return true;
		}
		return false;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::insert(Size offset, char c)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		if (offset >= AncestorType::size)
		{
			append(&c, 1);
			return true;
		}
		AncestorType::insert(offset, &c, 1);
		return true;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::prepend(const StringT& s)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		return insert(0, s);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::prepend(const StringT& s, Size size)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);

		return insert(0, Traits::CString<StringT>::Perform(s), size);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::prepend(const char* const cstr, Size size)
	{
		return insert(0, cstr, size);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::prepend(char c)
	{
		return insert(0, c);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::overwrite(Size offset, const char* const cstr, Size size)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		if (offset < AncestorType::size && size)
		{
			if (offset + size > AncestorType::size)
				YUNI_MEMCPY(AncestorType::data + offset, AncestorType::capacity, cstr, sizeof(Char) * (AncestorType::size - offset));
			else
				YUNI_MEMCPY(AncestorType::data + offset, AncestorType::capacity, cstr, sizeof(Char) * size);
		}
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::overwrite(Size offset, const StringT& s)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		// Find the substring
		if (Traits::Length<StringT,Size>::isFixed)
		{
			// We can make some optimisations when the length is known at compile compile time
			// This part of the code should not bring better performances but it should
			// prevent against bad uses of the API, like using a char* for looking for a single char.

			// The value to find is actually empty, nothing to do
			if (0 == Traits::Length<StringT,Size>::fixedLength)
				return;
			// The string is actually a single POD item
			if (1 == Traits::Length<StringT,Size>::fixedLength)
				return overwrite(offset, Traits::CString<StringT>::Perform(s), 1);
			// Researching for the substring with a known length
			return overwrite(offset, Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::fixedLength);
		}

		return overwrite(offset, Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::Value(s));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::overwrite(const StringT& s)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		// Find the substring
		if (Traits::Length<StringT,Size>::isFixed)
		{
			// We can make some optimisations when the length is known at compile compile time
			// This part of the code should not bring better performances but it should
			// prevent against bad uses of the API, like using a char* for looking for a single char.

			// The value to find is actually empty, nothing to do
			if (0 == Traits::Length<StringT,Size>::fixedLength)
				return;
			// The string is actually a single POD item
			if (1 == Traits::Length<StringT,Size>::fixedLength)
				return overwrite(0, Traits::CString<StringT>::Perform(s), 1);
			// Researching for the substring with a known length
			return overwrite(0, Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::fixedLength);
		}

		return overwrite(0, Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::Value(s));
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::overwriteRight(Size offset, const StringT& s)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		// Find the substring
		if (Traits::Length<StringT,Size>::isFixed)
		{
			// We can make some optimisations when the length is known at compile compile time
			// This part of the code should not bring better performances but it should
			// prevent against bad uses of the API, like using a char* for looking for a single char.

			// The value to find is actually empty, nothing to do
			if (0 == Traits::Length<StringT,Size>::fixedLength)
				return;
			// The string is actually a single POD item
			if (1 == Traits::Length<StringT,Size>::fixedLength)
				return overwrite(AncestorType::size - offset - 1, Traits::CString<StringT>::Perform(s), 1);
			// Researching for the substring with a known length
			return overwrite(AncestorType::size - offset - Traits::Length<StringT,Size>::fixedLength,
				Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::fixedLength);
		}

		Size len = Traits::Length<StringT,Size>::Value(s);
		return overwrite(AncestorType::size - offset - len, Traits::CString<StringT>::Perform(s), len);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::overwriteRight(const StringT& s)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		// Find the substring
		if (Traits::Length<StringT,Size>::isFixed)
		{
			// We can make some optimisations when the length is known at compile compile time
			// This part of the code should not bring better performances but it should
			// prevent against bad uses of the API, like using a char* for looking for a single char.

			// The value to find is actually empty, nothing to do
			if (0 == Traits::Length<StringT,Size>::fixedLength)
				return;
			// The string is actually a single POD item
			if (1 == Traits::Length<StringT,Size>::fixedLength)
				return overwrite(AncestorType::size - 1, Traits::CString<StringT>::Perform(s), 1);
			// Researching for the substring with a known length
			return overwrite(AncestorType::size - Traits::Length<StringT,Size>::fixedLength,
				Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::fixedLength);
		}

		Size len = Traits::Length<StringT,Size>::Value(s);
		return overwrite(AncestorType::size - len, Traits::CString<StringT>::Perform(s), len);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::overwriteCenter(const StringT& s)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		// Find the substring
		if (Traits::Length<StringT,Size>::isFixed)
		{
			// We can make some optimisations when the length is known at compile compile time
			// This part of the code should not bring better performances but it should
			// prevent against bad uses of the API, like using a char* for looking for a single char.

			// The value to find is actually empty, nothing to do
			if (0 == Traits::Length<StringT,Size>::fixedLength)
				return;
			// The string is actually a single POD item
			if (1 == Traits::Length<StringT,Size>::fixedLength)
				return overwrite((AncestorType::size >> 1), Traits::CString<StringT>::Perform(s), 1);
			// Researching for the substring with a known length
			return overwrite((AncestorType::size >> 1) - (Traits::Length<StringT,Size>::fixedLength >> 1),
				Traits::CString<StringT>::Perform(s), Traits::Length<StringT,Size>::fixedLength);
		}

		Size len = Traits::Length<StringT,Size>::Value(s);
		return overwrite((AncestorType::size >> 1) - (len >> 1), Traits::CString<StringT>::Perform(s), len);
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::truncate(Size newSize)
	{
		if (newSize < AncestorType::size)
			AncestorType::size = newSize;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::utf8size() const
	{
		// this routine is not exactly as fast as strlen, but it should make no
		// measurable difference
		unsigned int i = 0;
		Size r = 0;
		for (; i != AncestorType::size; ++i)
		{
			if ((AncestorType::data[i] & 0xc0) != 0x80)
				++r;
		}
		return r;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::utf8valid() const
	{
		unsigned int offset;
		return (UTF8::errNone == utf8valid(offset));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	UTF8::Error
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::utf8valid(Size& offset) const
	{
		offset = 0;
		UTF8::Char c;
		UTF8::Error e;
		do
		{
			if (UTF8::errNone != (e = utf8next<false>(offset, c)))
				return (e == UTF8::errOutOfBound) ? UTF8::errNone : e;
		}
		while (true);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::utf8validFast() const
	{
		unsigned int i = 0;
		unsigned int l;
		while (i != AncestorType::size)
		{
			if (!(l = UTF8::Char::Size(AncestorType::data + i)))
				return false;
			i += l;
			if (i > AncestorType::size)
				return false;
		}
		return true;
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<bool InvalidateOffsetIfErrorT>
	UTF8::Error
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::utf8next(Size& offset, UTF8::Char& out) const
	{
		if (offset >= AncestorType::size)
		{
			out.reset();
			if (InvalidateOffsetIfErrorT)
				offset = npos;
			return UTF8::errOutOfBound;
		}

		// Retrieving the lead character
		unsigned char lead = UTF8::Char::Mask8Bits(AncestorType::data[offset]);

		// {1}
		if (lead < 0x80)
		{
			++offset;
			out.pValue = lead;
			return UTF8::errNone;
		}
		// {2}
		if ((lead >> 5) == 0x6)
		{
			out.pValue = lead;
			if (++offset >= AncestorType::size)
			{
				if (InvalidateOffsetIfErrorT)
					offset = AncestorType::size;
				return UTF8::errNotEnoughData;
			}

			lead = UTF8::Char::Mask8Bits(AncestorType::data[offset]);
			if (UTF8::Char::IsTrail(lead))
			{
				out.pValue = ((out.pValue << 6) & 0x7ff) + ((lead) & 0x3f);
				++offset;
				return UTF8::errNone;
			}
			out.reset();
			if (InvalidateOffsetIfErrorT)
				offset = AncestorType::size;
			else
				--offset;
			return UTF8::errIncompleteSequence;
		}

		// {3}
		if ((lead >> 4) == 0xe)  // 1110 xxxx
		{
			out.pValue = lead;
			if (offset + 2 >= AncestorType::size)
			{
				if (InvalidateOffsetIfErrorT)
					offset = AncestorType::size;
				return UTF8::errNotEnoughData;
			}
			lead = UTF8::Char::Mask8Bits(AncestorType::data[++offset]);
			if (UTF8::Char::IsTrail(lead))
			{
				out.pValue = ((out.pValue << 12) & 0xffff) + ((lead << 6) & 0xfff);
				lead = UTF8::Char::Mask8Bits(AncestorType::data[++offset]);
				if (UTF8::Char::IsTrail(lead))
				{
					out.pValue += (lead) & 0x3f;
					++offset;
					return UTF8::errNone;
				}
				--offset;
			}
			out.reset();
			if (InvalidateOffsetIfErrorT)
				offset = AncestorType::size;
			else
				--offset;
			return UTF8::errIncompleteSequence;
		}

		// {4}
		if ((lead >> 3) == 0x1e) // 1111 0xxx
		{
			out.pValue = lead;
			if (offset + 3 >= AncestorType::size)
			{
				if (InvalidateOffsetIfErrorT)
					offset = AncestorType::size;
				return UTF8::errNotEnoughData;
			}

			lead = UTF8::Char::Mask8Bits(AncestorType::data[++offset]);
			if (UTF8::Char::IsTrail(lead))
			{
				out.pValue = ((out.pValue << 18) & 0x1fffff) + ((lead << 12) & 0x3ffff);
				lead = UTF8::Char::Mask8Bits(AncestorType::data[++offset]);
				if (UTF8::Char::IsTrail(lead))
				{
					out.pValue += (lead << 6) & 0xfff;
					lead = UTF8::Char::Mask8Bits(AncestorType::data[++offset]);
					if (UTF8::Char::IsTrail(lead))
					{
						out.pValue += (lead) & 0x3f;
						++offset;
						return UTF8::errNone;
					}
					--offset;
				}
				--offset;
			}

			out.reset();
			if (InvalidateOffsetIfErrorT)
				offset = AncestorType::size;
			else
				--offset;

			return UTF8::errIncompleteSequence;
		}
		out.reset();
		if (InvalidateOffsetIfErrorT)
			offset = AncestorType::size;
		return UTF8::errInvalidLead;
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::size() const
	{
		return AncestorType::size;
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::length() const
	{
		return AncestorType::size;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline size_t
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::sizeInBytes() const
	{
		return (size_t) AncestorType::size * sizeof(Char);
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::capacity() const
	{
		return AncestorType::capacity;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline size_t
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::max_size() const
	{
		return static_cast<Size>(-1);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline size_t
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::capacityInBytes() const
	{
		return (size_t) AncestorType::capacity * sizeof(Char);
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline const char*
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::data() const
	{
		return AncestorType::data;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline char*
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::data()
	{
		return AncestorType::data;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline const char*
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::c_str() const
	{
		return AncestorType::data;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::empty() const
	{
		return (0 == AncestorType::size);
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::null() const
	{
		return (NULL == AncestorType::data);
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::notEmpty() const
	{
		return (0 != AncestorType::size);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::shrink()
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		AncestorType::shrink();
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::resize(Size len)
	{
		if (AncestorType::expandable)
		{
			// Dynamic cstr
			if (!adapter)
				AncestorType::reserve(len + AncestorType::zeroTerminated);
			AncestorType::size = len;
		}
		else
		{
			// Static cstr
			if (len <= AncestorType::capacity)
				AncestorType::size = len;
			else
				AncestorType::size = AncestorType::capacity;
		}
		// Zero-Terminated cstrs
		if (!adapter && zeroTerminated)
			AncestorType::data[AncestorType::size] = Char();
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::resize(Size len, const StringT& pattern)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		if (len > AncestorType::size)
		{
			Size previousLength = AncestorType::size;
			resize(len);
			fill(previousLength, pattern);
		}
		else
			resize(len);
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::trimRight(char c)
	{
		while (AncestorType::size > 0 && AncestorType::data[AncestorType::size - 1] == c)
			--AncestorType::size;
		if (zeroTerminated)
			AncestorType::data[AncestorType::size] = Char();
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::trimLeft(char c)
	{
		if (AncestorType::size > 0)
		{
			Size offset = 0;
			while (offset < AncestorType::size && AncestorType::data[offset] == c)
				++offset;
			if (offset)
			{
				if (!adapter)
					erase(0, offset);
				else
				{
					AncestorType::data += offset;
					AncestorType::size -= offset;
				}
			}
		}
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::trimRight(const StringT& whitespaces)
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);

		const uint ptrlen = Traits::Length<StringT, uint>::Value(whitespaces);
		if (!ptrlen || empty())
			return;

		const char* const ptr = Traits::CString<StringT>::Perform(whitespaces);
		unsigned int i;
		while (AncestorType::size > 0)
		{
			i = 0;
			for (; i != ptrlen; ++i)
			{
				if (ptr[i] == AncestorType::data[AncestorType::size - 1])
				{
					--AncestorType::size;
					break;
				}
			}
			if (ptrlen == i) // nothing has been found. Aborting
				break;
		}
		// Making sure that the string is zero-terminated if required
		// The const_cast is only here to make it compile when the string
		// is an adapter
		if (!adapter && zeroTerminated)
			const_cast<char*>(AncestorType::data)[AncestorType::size] = Char();
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::trimLeft(const StringT& whitespaces)
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);

		const uint ptrlen = Traits::Length<StringT, uint>::Value(whitespaces);
		if (!ptrlen || empty())
			return;

		const char* const ptr = Traits::CString<StringT>::Perform(whitespaces);
		Size count = 0;
		unsigned int i;
		while (count < AncestorType::size)
		{
			i = 0;
			for (; i != ptrlen; ++i)
			{
				if (ptr[i] == AncestorType::data[count])
				{
					++count;
					break;
				}
			}
			if (ptrlen == i) // nothing has been found. Aborting
				break;
		}
		// Remove the first 'count' characters
		consume(count);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::decalOffset(Size count)
	{
		AncestorType::data += count;
		AncestorType::size -= count;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::trim()
	{
		// It seems more interesting to trim from the end first, to reduce the size
		// of the cstr as soon as possible and to reduce the amount of data to move
		// if a region must be removed by trimLeft.
		trimRight(" \t\r\n");
		trimLeft(" \t\r\n");
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::trim(char c)
	{
		// It seems more interesting to trim from the end first, to reduce the size
		// of the cstr as soon as possible and to reduce the amount of data to move
		// if a region must be removed by trimLeft.
		trimRight(c);
		trimLeft(c);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::trim(const StringT& whitespaces)
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForLength);

		// It seems more interesting to trim from the end first, to reduce the size
		// of the cstr as soon as possible and to reduce the amount of data to move
		// if a region must be removed by trimLeft.
		trimRight(whitespaces);
		trimLeft(whitespaces);
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::assignWithoutChecking(
		const char* const block, Size blockSize)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		return AncestorType::assignWithoutChecking(block, blockSize);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::appendWithoutChecking(
		const char* const block, Size blockSize)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		return AncestorType::appendWithoutChecking(block, blockSize);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::assignWithoutChecking(char c)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		return AncestorType::assignWithoutChecking(c);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Size
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::appendWithoutChecking(char c)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		return AncestorType::appendWithoutChecking(c);
	}





	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>&
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::format(const StringT& format, ...)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::zeroTerminated, CString_FormatMustBeZeroTerminated);

		// Empty the cstr
		clear();
		// Dealing with the variadic arguments
		va_list parg;
		va_start(parg, format);
		vappendFormat(Traits::CString<StringT>::Perform(format), parg);
		va_end(parg);
		return *this;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>&
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::appendFormat(const StringT& format, ...)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::zeroTerminated, CString_FormatMustBeZeroTerminated);

		// Dealing with the variadic arguments
		va_list parg;
		va_start(parg, format);
		vappendFormat(Traits::CString<StringT>::Perform(format), parg);
		va_end(parg);
		return *this;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::vappendFormat(const char* format, va_list args)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		// Nothing to do if the format is empty
		if (!format || '\0' == *format)
			return;

		// There is no way to know by advance the size occupied by the formatted
		// string
		// Instead of allocating a new temporary cstr, we will directly use
		// this cstr as much as possible.
		//
		// The returned size
		int i;

		if (expandable)
		{
			// Pre-Allocating a minimal amount of free space
			if (AncestorType::capacity - AncestorType::size < 10)
			{
				// -1 because reserve() always count a final zero
				AncestorType::reserve(AncestorType::size + chunkSize - 1);
			}
			do
			{
				// In C99, vnsprintf may modify its parameter arg
				va_list ag;
				YUNI_VA_COPY(ag, args);
				i = Private::CStringImpl::vnsprintf<Char>(AncestorType::data + AncestorType::size,
					(AncestorType::capacity - AncestorType::size), format, ag);
				va_end(ag);

				if (i < 0)
				{
					if (i == -1)
					{
						// The string was truncated
						// -1 because reserve() always count a final zero
						AncestorType::reserve(AncestorType::capacity + chunkSize - 1);
						continue;
					}
					// An error occured
					return;
				}
				AncestorType::size += (Size) i;
				if (zeroTerminated)
					AncestorType::data[AncestorType::size] = Char();
				return;
			}
			while (true);
		}
		else
		{
			// In this case, the cstr can not be expanded
			// We will only try once
			if (AncestorType::capacity != AncestorType::size)
			{
				i = Private::CStringImpl::vnsprintf<Char>(AncestorType::data + AncestorType::size,
					(AncestorType::capacity - AncestorType::size), format, args);
				if (i >= 0)
				{
					AncestorType::size += (Size) i;
					if (zeroTerminated)
						AncestorType::data[AncestorType::size] = Char();
				}
				else
				{
					if (i == -1)
					{
						// The string was truncated
						AncestorType::size = AncestorType::capacity;
						if (zeroTerminated)
							AncestorType::data[AncestorType::capacity] = Char();
					}
				}
			}
		}
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>&
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::toLower()
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		for (Size i = 0; i < AncestorType::size; ++i)
		{
			if (UTF8::Char::IsASCII((unsigned char)AncestorType::data[i]))
				AncestorType::data[i] = static_cast<Char>(ToLower(AncestorType::data[i]));
		}
		return *this;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>&
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::toUpper()
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		for (Size i = 0; i < AncestorType::size; ++i)
		{
			if (UTF8::Char::IsASCII((unsigned char)AncestorType::data[i]))
				AncestorType::data[i] = (Char) ToUpper(AncestorType::data[i]);
		}
		return *this;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<template<class,class> class ListT, class UType, class Alloc, class StringT>
	void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::split(ListT<UType,Alloc>& out, const StringT& sep,
		bool keepEmptyElements, bool trimElements, bool emptyBefore) const
	{
		// Empty the container
		if (emptyBefore)
			out.clear();
		// String empty
		if (this->notEmpty())
		{
			// Indexes
			Size indx = 0;
			Size len  = 0;
			Size newIndx;

			// Temporary buffer
			WritableType segment;

			do
			{
				newIndx = this->find_first_of(sep, indx);
				if (npos == newIndx)
				{
					segment.assign(AncestorType::data + indx, AncestorType::size - indx);
					if (trimElements)
						segment.trim();
					if (segment.notEmpty() || keepEmptyElements)
						out.push_back(segment.to<UType>());
					return;
				}

				if ((len = newIndx - indx) || keepEmptyElements)
				{
					segment.assign(AncestorType::data + indx, len);
					if (trimElements)
						segment.trim();
					if (segment.notEmpty() || keepEmptyElements)
						out.push_back(segment.to<UType>());
				}
				indx = newIndx + 1;
			}
			while (true);
		}
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline char
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::first() const
	{
		return (AncestorType::size) ? AncestorType::data[0] : '\0';
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline char
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::last() const
	{
		return (AncestorType::size) ? AncestorType::data[AncestorType::size - 1] : '\0';
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::convertSlashesIntoBackslashes()
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		for (unsigned int i = 0; i != AncestorType::size; ++i)
		{
			if ('/' == AncestorType::data[i])
				AncestorType::data[i] = '\\';
		}
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::convertBackslashesIntoSlashes()
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		for (unsigned int i = 0; i != AncestorType::size; ++i)
		{
			if ('\\' == AncestorType::data[i])
				AncestorType::data[i] = '/';
		}
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::duplicate(int n)
	{
		YUNI_STATIC_ASSERT(!adapter, CString_Adapter_ReadOnly);
		if (n > 0 && AncestorType::size > 0)
		{
			if (AncestorType::size == 1)
			{
				// Resize the string
				resize(AncestorType::size * (n + 1));
				// Caraceter copy
				for (unsigned int i = 1; i != AncestorType::size; ++i)
					AncestorType::data[i] = AncestorType::data[0];
			}
			else
			{
				Size seglen = AncestorType::size;
				Size offset = AncestorType::size;
				// Resize the string
				resize(AncestorType::size * (n + 1));

				while (offset < AncestorType::size)
				{
					if (seglen + offset > AncestorType::size)
						YUNI_MEMCPY(AncestorType::data + offset, AncestorType::capacity, AncestorType::data, AncestorType::size - offset);
					else
						YUNI_MEMCPY(AncestorType::data + offset, AncestorType::capacity, AncestorType::data, seglen);
					offset += seglen;
				}
			}
		}
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline int
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::Compare(const char* const s1, unsigned int l1,
		const char* const s2, unsigned int l2)
	{
		return Yuni::Private::CStringImpl::Compare(s1, l1, s2, l2);
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline int
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::CompareInsensitive(const char* const s1, unsigned int l1,
		const char* const s2, unsigned int l2)
	{
		return Yuni::Private::CStringImpl::CompareInsensitive(s1, l1, s2, l2);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::equals(const StringT& rhs) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		return ((AncestorType::size == Traits::Length<StringT,Size>::Value(rhs))
			&& (!AncestorType::size
				|| Yuni::Private::CStringImpl::Equals(AncestorType::data, Traits::CString<StringT>::Perform(rhs), AncestorType::size)));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::equalsInsensitive(const StringT& rhs) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		return ((AncestorType::size == Traits::Length<StringT,Size>::Value(rhs))
			&& (!AncestorType::size
				|| Yuni::Private::CStringImpl::EqualsInsensitive(AncestorType::data, Traits::CString<StringT>::Perform(rhs), AncestorType::size)));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline int
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::compare(const StringT& rhs) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		return Yuni::Private::CStringImpl::Compare(AncestorType::data, AncestorType::size,
			Traits::CString<StringT>::Perform(rhs), Traits::Length<StringT,Size>::Value(rhs));
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline int
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::compareInsensitive(const StringT& rhs) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		return Yuni::Private::CStringImpl::CompareInsensitive(AncestorType::data, AncestorType::size,
			Traits::CString<StringT>::Perform(rhs), Traits::Length<StringT,Size>::Value(rhs));
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline const char&
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::operator [] (Size offset) const
	{
		assert(offset < AncestorType::size && "string, operator[] : index out of bounds");
		return AncestorType::data[offset];
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline char&
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::operator [] (Size offset)
	{
		assert(offset < AncestorType::size && "string, operator[] : index out of bounds");
		return AncestorType::data[offset];
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::operator != (const StringT& rhs) const
	{
		return !(*this == rhs);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::operator ! () const
	{
		return !AncestorType::size;
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class U>
	inline CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>&
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::operator += (const U& rhs)
	{
		append(rhs);
		return *this;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class U>
	inline CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>&
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::operator << (const U& rhs)
	{
		append(rhs);
		return *this;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>&
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::operator = (const CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>& rhs)
	{
		assign(rhs);
		return *this;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class U>
	inline CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>&
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::operator = (const U& rhs)
	{
		assign(rhs);
		return *this;
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::operator < (const StringT& rhs) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		return Yuni::Private::CStringImpl::Compare(AncestorType::data, AncestorType::size,
			Traits::CString<StringT>::Perform(rhs), Traits::Length<StringT,Size>::Value(rhs)) < 0;
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::operator > (const StringT& rhs) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		return Yuni::Private::CStringImpl::Compare(AncestorType::data, AncestorType::size,
			Traits::CString<StringT>::Perform(rhs), Traits::Length<StringT,Size>::Value(rhs)) > 0;
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::operator <= (const StringT& rhs) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		return Yuni::Private::CStringImpl::Compare(AncestorType::data, AncestorType::size,
			Traits::CString<StringT>::Perform(rhs), Traits::Length<StringT,Size>::Value(rhs)) <= 0;
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::operator >= (const StringT& rhs) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);

		return Yuni::Private::CStringImpl::Compare(AncestorType::data, AncestorType::size,
			Traits::CString<StringT>::Perform(rhs), Traits::Length<StringT,Size>::Value(rhs)) >= 0;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::operator == (const StringT& rhs) const
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT>::valid, CString_InvalidTypeForBuffer);
		YUNI_STATIC_ASSERT(Traits::Length<StringT>::valid,  CString_InvalidTypeForBufferSize);
		return equals(rhs);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline bool
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::operator == (const CString& rhs) const
	{
		return equals(rhs);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>&
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::operator *= (int n)
	{
		duplicate(n);
		return *this;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::adapt(const char* cstring, Size length)
	{
		Yuni::Private::CStringImpl::AdapterAssign<CStringType, adapter>::Perform(*this,
			cstring, length);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::adapt(const StringT& string)
	{
		Yuni::Private::CStringImpl::AdapterAssign<CStringType, adapter>::Perform(*this,
			Traits::CString<StringT>::Perform(string),
			Traits::Length<StringT,Size>::Value(string));
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::adapt(const StringT& string, Size length, Size offset)
	{
		Yuni::Private::CStringImpl::AdapterAssign<CStringType, adapter>::Perform(*this,
			Traits::CString<StringT>::Perform(string) + offset,
			length);
	}

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::adaptWithoutChecking(const char* const cstring, Size length)
	{
		AncestorType::data = const_cast<char*>(cstring);
		AncestorType::size = length;
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT1, class StringT2>
	inline void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::extractKeyValue(StringT1& key, StringT2& value, bool ignoreCase) const
	{
		// ReInitializing
		key.clear();
		value.clear();

		if (empty()) // The string is empty, Nothing to do
			return;

		Size left = find_first_not_of(" \t\n\r");
		if (left == npos)
			return;

		// Section
		if ('{' == AncestorType::data[left])
		{
			key.append('{');
			return;
		}
		if ('}' == AncestorType::data[left])
		{
			key.append('}');
			return;
		}
		if ('[' == AncestorType::data[left])
		{
			key.append('[');
			++left;
			Size right = find(']', left);
			if (right != npos && right != left)
			{
				value.append(*this, right - left, left);
				value.trim();
			}
			return;
		}

		// If not a section, it should be a key/value
		// Looking for the symbol `=`
		Size equal = left;
		do
		{
			equal = find_first_of("=/;", equal);
			if (equal < AncestorType::size)
			{
				if ('/' == AncestorType::data[equal])
				{
					if (equal + 1 < AncestorType::size && AncestorType::data[equal + 1] != '/')
					{
						++equal;
						continue;
					}
				}
				if (equal == left)
					return;
				if ('=' == AncestorType::data[equal])
					break;
			}
			return;
		}
		while (true);

		// Getting our key
		key.assign(*this, equal - left, left);
		key.trimRight(" \t\n\r");
		if (ignoreCase)
			key.toLower();

		// Looking for the first interesting char
		Size rv(equal);
		++rv; // After the symbol `=`
		while (rv < AncestorType::size && (AncestorType::data[rv] == ' ' || AncestorType::data[rv] == '\t' ||AncestorType::data[rv] == '\r' ||AncestorType::data[rv] == '\n'))
			++rv;
		if (rv < AncestorType::size) // Empty value
		{
			char c = AncestorType::data[rv];
			switch (c)
			{
				case ';':
					// Empty value
					break;
				case '"':
				case '\'':
					{
						// Value enclosed in a string
						++rv;
						typename AncestorType::Size next;
						next = FindEndOfSequence<CStringType>(AncestorType::data + rv, c, AncestorType::size - rv);
						if (next != npos)
							value.append(AncestorType::data + rv, next);
							//value.assignFromEscapedCharacters(AncestorType::data, next, rv);
						else
							value.append(AncestorType::data + rv, AncestorType::size - rv);
						break;
					}
				case '/':
					{
						// Empty value if we have a comment otherwise '/' is a valid entry
						if (rv + 1 >= AncestorType::size || AncestorType::data[rv + 1] == '/')
							break;
						// no break
					}
				default:
					{
						// Standard value
						Size semicolon = find_first_of(';', rv);
						if (npos != semicolon)
							value.append(*this, semicolon - rv, rv);
						else
							value.append(*this, AncestorType::size - rv, rv);
						value.trimRight(" \t\n\r");
						break;
					}
			}
		}
	}



	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	template<class StringT, class PredicateT>
	void
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::words(const StringT& separators, const PredicateT& predicate) const
	{
		if (0 == AncestorType::size)
			return;

		// Indexes
		Size offset = 0;
		Size len  = 0;
		Size newIndx;
		// Temporary buffer
		AnyString word;

		do
		{
			newIndx = find_first_of(separators, offset);

			if (newIndx < AncestorType::size)
			{
				len = newIndx - offset;
				if (len)
				{
					word.assign(AncestorType::data + offset, len);
					predicate(word);
				}
			}
			else
			{
				word.adapt(AncestorType::data + offset, AncestorType::size - offset);
				predicate(word);
				break;
			}

			offset = newIndx + 1;
		}
		while (true);
	}




} // namespace Yuni

#endif // __YUNI_CORE_STRING_STRING_HXX__
