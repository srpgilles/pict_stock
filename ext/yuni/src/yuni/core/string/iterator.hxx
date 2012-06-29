#ifndef __YUNI_CORE_STRING_ITERATOR_HXX__
# define __YUNI_CORE_STRING_ITERATOR_HXX__


namespace Yuni
{

	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::utf8iterator
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::utf8begin(uint offset)
	{
		return utf8iterator(*this, offset);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::const_utf8iterator
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::utf8begin(uint offset) const
	{
		return const_utf8iterator(*this, offset);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::null_iterator
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::utf8end()
	{
		return null_iterator(*this);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::null_iterator
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::utf8end() const
	{
		return null_iterator(*this);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::null_iterator
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::end()
	{
		return null_iterator(*this);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::null_iterator
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::end() const
	{
		return null_iterator(*this);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::iterator
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::begin()
	{
		return iterator(*this, 0);
	}


	template<unsigned int ChunkSizeT, bool ExpandableT, bool ZeroTerminatedT>
	inline typename CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::const_iterator
	CString<ChunkSizeT,ExpandableT,ZeroTerminatedT>::begin() const
	{
		return const_iterator(*this, 0);
	}



} // namespace Yuni

#endif // __YUNI_CORE_STRING_ITERATOR_HXX__


