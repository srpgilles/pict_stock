#ifndef __YUNI_IO_FILE_STREAM_HXX__
# define __YUNI_IO_FILE_STREAM_HXX__

# include "../../core/string.h"


namespace Yuni
{
namespace IO
{
namespace File
{

	inline Stream::Stream() :
		pFd(NULL)
	{
		// Do nothing
	}


	inline Stream::~Stream()
	{
		// The check is mandatory to avoid SegV on some platform (Darwin for example)
		if (pFd)
			(void)::fclose(pFd);
	}


	inline bool Stream::openRW(const AnyString& filename)
	{
		// note: 'this' added this to avoid name resolution conflicts
		return this->open(filename, IO::OpenMode::write | IO::OpenMode::truncate);
	}


	inline bool Stream::opened() const
	{
		return (NULL != pFd);
	}


	inline bool Stream::eof() const
	{
		# ifdef YUNI_OS_MSVC
		return ::feof(pFd) ? true : false;
		# else
		return ::feof(pFd);
		# endif
	}

	inline ssize_t Stream::tell() const
	{
		return (ssize_t)::ftell(pFd);
	}



	inline bool Stream::seekFromBeginning(ssize_t offset)
	{
		return (0 == ::fseek(pFd, (long)offset, SEEK_SET));
	}


	inline bool Stream::seekFromEndOfFile(ssize_t offset)
	{
		return (0 == ::fseek(pFd, (long)offset, SEEK_END));
	}


	inline bool Stream::seekMove(ssize_t offset)
	{
		return (0 == ::fseek(pFd, (long)offset, SEEK_CUR));
	}


	inline bool Stream::flush()
	{
		return (0 == ::fflush(pFd));
	}


	inline char Stream::get()
	{
		return (char)::fgetc(pFd);
	}


	inline bool Stream::readline(char* buffer, size_t maxSize)
	{
		return (NULL != fgets(buffer, (int)maxSize, pFd));
	}


	template<class StringT>
	inline bool
	Stream::readline(StringT& buffer, bool trim)
	{
		return readline<StringT::chunkSize, StringT>(buffer, trim);
	}


	template<uint CustomChunkT, class StringT>
	bool
	Stream::readline(StringT& buffer, bool trim)
	{
		// The buffer must be reserved to its full capacity just before
		// Assuming we have a mere Yuni::String, the internal may be null.
		buffer.reserve(CustomChunkT);
		// Read data from the file
		if ((NULL != ::fgets((char*)buffer.data(), static_cast<int>(CustomChunkT), pFd)))
		{
			// We may have read less than expected. So we have to resize the string
			// to perform maintenance (about the internal size and the final zero)
			buffer.resize(static_cast<uint>(::strlen(buffer.c_str())));
			if (trim)
				buffer.trimRight("\r\n");
			return true;
		}
		buffer.clear();
		return false;
	}



	inline bool Stream::put(char c)
	{
		return (0 == ::fputc((int) c, pFd));
	}



	namespace // anonymous
	{

		template<int IsStringT, class U>
		struct StreamTraitsWrite
		{
			static inline uint64 Perform(FILE* pFd, const AnyString& string)
			{
				return (uint64) ::fwrite(
					string.c_str(),  // raw data
					1,               // nb items
					string.size(),   // length
					pFd);            // file descriptor
			}
			static inline uint64 Perform(FILE* pFd, const U& string, uint64 size)
			{
				YUNI_STATIC_ASSERT(Traits::IsString<U>::yes, InvalidTypeForBuffer);
				return (uint64) ::fwrite(
					Traits::CString<U>::Perform(string),  // raw data
					1,                                    // nb items
					(size_t)size,                         // size
					pFd);                                 // file descriptor
			}
		};

		template<class U>
		struct StreamTraitsWrite<0,U>
		{
			static inline uint64 Perform(FILE* pFd, const U& u)
			{
				// TODO : Find a faster alternative
				String translator;
				translator << u;
				return (uint64) ::fwrite(translator.c_str(), 1, translator.size(), pFd);
			}

			static inline uint64 Perform(FILE* pFd, const U& u, uint64 size)
			{
				String translator;
				translator << u;
				return (uint64)::fwrite(translator.c_str(), 1, translator.size() > size ? size : translator.size(), pFd);
			}
		};


		template<>
		struct StreamTraitsWrite<0, bool>
		{
			static inline uint64 Perform(FILE* pFd, bool u)
			{
				return u
					? (uint64)::fwrite("true", 1, 4, pFd)
					: (uint64)::fwrite("false", 1, 5, pFd);
			}
			static inline uint64 Perform(FILE* pFd, bool u, uint64 size)
			{
				return u
					? (uint64)::fwrite("true", 1, 4 > size ? (size_t)size : 4, pFd)
					: (uint64)::fwrite("false", 1, 5 > size ? (size_t)size : 5, pFd);
			}

		};

	} // anonymous namespace



	template<class U>
	inline uint64 Stream::write(const U& buffer)
	{
		return StreamTraitsWrite<Traits::CString<U>::valid, U>::Perform(pFd, buffer);
	}


	template<class U>
	inline uint64 Stream::write(const U& buffer, uint64 size)
	{
		return StreamTraitsWrite<Traits::CString<U>::valid, U>::Perform(pFd, buffer, size);
	}


	inline bool Stream::operator ! () const
	{
		return (NULL == pFd);
	}


	inline Stream& Stream::operator += (const char c)
	{
		(void)::fputc((int) c, pFd);
		return *this;
	}


	inline Stream& Stream::operator << (const char c)
	{
		(void)::fputc((int) c, pFd);
		return *this;
	}


	template<class U>
	inline Stream& Stream::operator += (const U& u)
	{
		write(u);
		return *this;
	}


	template<class U>
	inline Stream& Stream::operator << (const U& u)
	{
		write(u);
		return *this;
	}


	template<uint CSizeT, bool ExpT>
	inline uint64
	Stream::read(CString<CSizeT, ExpT>& buffer, uint64 size)
	{
		assert(pFd && "File not opened");
		assert(size <= uint64(2 * 1024) * 1024 * 1024);
		if (!size)
			return 0;

		// special case for static strings
		if (!buffer.expandable && size > buffer.chunkSize)
			size = buffer.chunkSize;

		// Resizing the buffer
		buffer.resize((uint) size);

		// Assert to prevent SegV
		assert(buffer.capacity() != 0 && "When reading a file, the buffer must have reserved some space");

		typedef CString<CSizeT, ExpT> StringType;
		typedef typename StringType::Char C;
		// Reading the file
		size_t result = ::fread(const_cast<char*>(buffer.data()), 1, (size_t) (sizeof(C) * size), pFd);
		// Setting the good size, because we may have read less than asked
		if (result < (size_t) buffer.size())
			buffer.truncate((uint)result);
		// Making sure that the buffer is zero-terminated if required
		if (buffer.zeroTerminated)
			*((C*)(buffer.data() + buffer.size())) = C();
		return result;
	}


	template<uint ChunkSizeT, bool ExpandableT>
	inline uint64
	Stream::chunkRead(CString<ChunkSizeT, ExpandableT>& buffer)
	{
		// Resizing the buffer
		buffer.reserve(buffer.chunkSize);
		// Assert to prevent SegV
		assert(buffer.capacity() != 0 && "When reading a file, the buffer must have reserved some space");

		typedef CString<ChunkSizeT, ExpandableT> StringType;
		typedef typename StringType::Char C;
		// Reading the file
		const uint64 result = ::fread(buffer.data(), 1, sizeof(C) * buffer.chunkSize, pFd);
		// Setting the good size, because we may have read less than asked
		if (result < buffer.size())
			buffer.truncate((typename StringType::Size) result);
		// Making sure that the buffer is zero-terminated if required
		if (buffer.zeroTerminated)
			*((C*)(buffer.data() + buffer.size())) = C();
		return result;
	}



	template<class U>
	inline Stream& Stream::operator >> (U&  rhs)
	{
		(void)read(rhs, rhs.size());
		return *this;
	}


	inline Stream::HandleType Stream::nativeHandle() const
	{
		return pFd;
	}




} // namespace File
} // namespace IO
} // namespace Yuni

#endif // __YUNI_IO_FILE_STREAM_HXX__
