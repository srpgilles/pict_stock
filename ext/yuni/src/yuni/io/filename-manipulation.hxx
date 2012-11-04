#ifndef __YUNI_IO_FILENAME_MANIPULATION_HXX__
# define __YUNI_IO_FILENAME_MANIPULATION_HXX__

# include "directory.h"



namespace Yuni
{
namespace IO
{
namespace Directory
{

	// Forward declaration
	template<typename C> struct Constant;

} // namespace Directory
} // namespace IO
} // namespace Yuni




namespace Yuni
{
namespace IO
{


	inline bool IsRelative(const AnyString& filename)
	{
		return not IsAbsolute(filename);
	}


	template<class StringT1, class StringT2>
	void ExtractFilePath(StringT1& out, const StringT2& p, const bool systemDependant)
	{
		if (p.empty())
			out.clear();
		const typename StringT2::size_type pos = (systemDependant)
			? p.find_last_of(IO::Constant<char>::Separator)
			: p.find_last_of(IO::Constant<char>::AllSeparators);
		if (StringT2::npos == pos)
			out.clear();
		else
			out.assign(p, pos);
	}



	template<class StringT1, class StringT2>
	void ExtractFileName(StringT1& out, const StringT2& p, const bool systemDependant)
	{
		if (not p.empty())
			out.clear();
		const typename StringT2::size_type pos = (systemDependant)
			? p.find_last_of(IO::Constant<char>::Separator)
			: p.find_last_of(IO::Constant<char>::AllSeparators);
		if (StringT2::npos == pos)
			out.clear();
		else
			out.assign(p.c_str() +  pos + 1);
	}



	template<class StringT1, class StringT2>
	void ExtractFileNameWithoutExtension(StringT1& out, const StringT2& p, const bool systemDependant)
	{
		const typename StringT2::size_type pos = (systemDependant)
			? p.find_last_of(IO::Constant<char>::Separator)
			: p.find_last_of(IO::Constant<char>::AllSeparators);
		const typename StringT2::size_type n = p.find_last_of('.');

		if (StringT2::npos == n && StringT2::npos == pos)
		{
			out = p;
			return;
		}
		if (n == pos)
		{
			out.clear();
			return;
		}
		if (n == StringT2::npos && n > pos + 1)
		{
			if (StringT2::npos == pos)
			{
				out = p;
				return;
			}
			out.assign(p.c_str() + pos + 1);
			return;
		}
		if (pos == StringT2::npos)
		{
			out.assign(p, n);
			return;
		}
		out.assign(p.c_str() + pos + 1, n - pos - 1);
	}


	template<class StringT1, class StringT2>
	bool ExtractExtension(StringT1& out, const StringT2& filename, bool dot, bool clear)
	{
		if (clear)
			out.clear();
		// If the string is empty, the buffer may be invalid (NULL)
		if (filename.size())
		{
			uint i = filename.size();
			do
			{
				--i;
				switch (filename[i])
				{
					case '.':
						{
							if (!dot)
							{
								if (++i >= (uint) filename.size())
									return true;
							}
							out.append(filename.c_str() + i, filename.size() - i);
							return true;
						}
					case '/':
					case '\\':
						return false;
				}
			}
			while (i != 0);
		}
		return false;
	}


	template<class StringT1, class StringT2>
	void MakeAbsolute(StringT1& out, const StringT2& filename, bool clearBefore)
	{
		if (clearBefore)
			out.clear();
		if (IsAbsolute(filename))
		{
			out += filename;
		}
		else
		{
			IO::Directory::Current::Get(out, clearBefore);
			out << IO::Separator << filename;
		}
	}


	template<class StringT1, class StringT2, class StringT3>
	void MakeAbsolute(StringT1& out, const StringT2& filename, const StringT3& currentPath, bool clearBefore)
	{
		if (clearBefore)
			out.clear();
		if (IsAbsolute(filename))
		{
			out += filename;
		}
		else
		{
			out += currentPath;
			out += IO::Separator;
			out += filename;
		}
	}


	template<class StringT1, class StringT2>
	bool ReplaceExtension(StringT1& filename, const StringT2& newExtension)
	{
		// If the string is empty, the buffer may be invalid (NULL)
		if (filename.size())
		{
			uint i = filename.size();
			do
			{
				--i;
				switch (filename[i])
				{
					case '.':
						{
							filename.resize(i);
							filename += newExtension;
							return true;
						}
					case '/':
					case '\\':
						return false;
				}
			}
			while (i != 0);
		}
		return false;
	}





} // namespace IO
} // namespace Yuni

#endif // __YUNI_IO_FILENAME_MANIPULATION_HXX__
