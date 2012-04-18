#ifndef __YUNI_IO_FILE_HXX__
# define __YUNI_IO_FILE_HXX__

# include "../../core/traits/cstring.h"
# include "../../core/traits/length.h"
# include "../../core/static/remove.h"



namespace Yuni
{
namespace IO
{
namespace File
{


	inline uint64 Size(const AnyString& filename)
	{
		uint64 size;
		return (Size(filename, size)) ? size : 0;
	}




	inline bool Exists(const AnyString& filename)
	{
		return ((Yuni::IO::typeFile & Yuni::IO::TypeOf(filename)) != 0);
	}



	template<class StringT>
	inline bool CreateEmptyFile(const StringT& filename)
	{
		IO::File::Stream file(filename, OpenMode::write | OpenMode::truncate);
		return file.opened();
	}


	template<class StringT, class U>
	bool SetContent(const StringT& filename, const U& content)
	{
		IO::File::Stream file(filename, OpenMode::write | OpenMode::truncate);
		if (file.opened())
		{
			file += content;
			return true;
		}
		return false;
	}


	template<class StringT, class U>
	bool AppendContent(const StringT& filename, const U& content)
	{
		IO::File::Stream file(filename, OpenMode::write | OpenMode::append);
		if (file.opened())
		{
			file += content;
			return true;
		}
		return false;
	}



	template<class StringT, class U>
	bool SaveToFile(const StringT& filename, const U& content)
	{
		IO::File::Stream file(filename, OpenMode::write | OpenMode::truncate);
		if (file.opened())
		{
			file += content;
			return true;
		}
		return false;
	}


	template<class StringT1, class StringT2>
	IO::Error
	LoadFromFile(StringT1& out, const StringT2& filename, const uint64 hardlimit)
	{
		out.clear();
		Yuni::IO::File::Stream f(filename);
		if (f.opened())
		{
			char buffer[2096 + 1];
			size_t numRead = 0;
			uint64 totalRead = 0;
			while ((numRead = f.read(buffer, sizeof(buffer) - 1)) != 0)
			{
				// This assignment is mandatory to prevent some bad use with
				// strlen (according to StringT1).
				// In some string implementation, the class might use strlen
				// on the buffer even if the length is given
				buffer[numRead] = '\0';

				totalRead += numRead;
				if (totalRead > hardlimit)
				{
					const size_t minus = (size_t)(totalRead - hardlimit);
					if (minus < numRead)
					{
						numRead -= minus;
						out.append((const char*) buffer, (unsigned int) numRead);
					}
					return errMemoryLimit;
				}
				// we use the standard method `append()` to allow the use of std::string
				out.append((const char*) buffer, (unsigned int) numRead);
			}
			return errNone;
		}
		return errNotFound;
	}


	template<class StringT, class PredicateT>
	bool
	ReadLineByLine(const StringT& filename, const PredicateT& predicate)
	{
		IO::File::Stream file;
		if (file.open(filename))
		{
			String line;
			while (file.readline<4096u, String>(line))
				predicate(line);

			return true;
		}
		return false;
	}






} // namespace File
} // namespace IO
} // namespace Yuni

#endif // __YUNI_IO_FILE_H__
