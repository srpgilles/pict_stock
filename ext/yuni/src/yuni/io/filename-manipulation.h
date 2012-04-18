#ifndef __YUNI_IO_FILENAME_MANIPULATION_H__
# define __YUNI_IO_FILENAME_MANIPULATION_H__

# include "io.h"


namespace Yuni
{
namespace IO
{

	/*!
	** \brief Extract the path part of a filename
	**
	** The path part will be extracted according the system-(un)dependant path-separator
	** \code
	**    String outputString;
	**	  IO::Filename::ExtractFilePath(outputString, "/tmp/foo.txt");
	**    std::cout << outputString << std::endl; // writes `/tmp`
	** \endcode
	**
	** \param[out] out The output string
	** \param[in] p The filename
	** \param[in] systemDependant Consider only the system-dependant path-separator
	*/
	template<class StringT1, class StringT2>
	void ExtractFilePath(StringT1& out, const StringT2& p, const bool systemDependant = false);



	/*!
	** \brief Extract the bare file name
	**
	** The file name will be extracted according the last occurence
	** of the system-dependant path-separator (if systemDependant = true)
	**
	** \param[out] out        The bare filename from the original one
	** \param p               The original filename
	** \param systemDependant Consider only the system-dependant path-separator
	*/
	template<class StringT1, class StringT2>
	void ExtractFileName(StringT1& out, const StringT2& p, const bool systemDependant = true);


	/*!
	** \brief Extract the bare file name without its extension
	**
	** The file name will be extracted according the last occurence
	** of the system-dependant path-separator (if systemDependant = true).
	**
	** \param[out] out        The bare file name without its extension
	** \param p               The original file name
	** \param systemDependant Consider only the system-dependant path-separator
	*/
	template<class StringT1, class StringT2>
	void ExtractFileNameWithoutExtension(StringT1& out, const StringT2& p, const bool systemDependant = true);


	/*!
	** \brief Extract the extention of a file name
	**
	** \param out Variable where the result will be appended
	** \param filename The original filename
	** \param dot True to include the dot when extracting the extension
	** \param clear True to clear `out` before processing
	** \return True if an extension has been found
	*/
	template<class StringT1, class StringT2>
	bool ExtractExtension(StringT1& out, const StringT2& filename, bool dot = true, bool clear = true);


	/*!
	** \brief Get if a path is absolute
	**
	** \param filename The path or the filename to test
	** \return True if the given filename is an absolute path, false otherwise (or empty)
	*/
	template<class StringT> bool IsAbsolute(const StringT& filename);


	/*!
	** \brief Get if a path is relative
	**
	** \param filename The path or the filename to test
	** \return True if the given filename is an absolute path, false otherwise (or empty)
	*/
	template<class StringT> bool IsRelative(const StringT& filename);


	/*!
	** \brief Make a path absolute
	**
	** The current directory will be used when the given path is not absolute.
	** \param[out] out         The variable where to write the result
	** \param      filename    The filename to make absolute
	** \param      clearBefore True to clean \p out before
	*/
	template<class StringT1, class StringT2>
	void MakeAbsolute(StringT1& out, const StringT2& filename, bool clearBefore = true);

	/*!
	** \brief Make a path absolute
	**
	** The current directory will be used when the given path is not absolute.
	** \param[out] out         The variable where to write the result
	** \param      path        The filename to make absolute
	** \param      currentPath A custom current path to use if the filename is not absolute
	** \param      clearBefore True to clean \p out before
	*/
	template<class StringT1, class StringT2, class StringT3>
	void MakeAbsolute(StringT1& out, const StringT2& path, const StringT3& currentPath, bool clearBefore = true);


	/*!
	** \brief Replace the extension
	**
	** \code
	** std::string s = "file.avi";
	** Yuni::IO::ReplaceExtension(s, ".mpeg");
	** std::cout << s << std::endl; // file.mpeg
	** \endcode
	**
	** \param[in,out] filename     The original filename
	** \param         newExtension The new extension (dot included, ex: `.ota`)
	** \return True if the extension has been replaced (means `found and replaced`)
	*/
	template<class StringT1, class StringT2>
	bool ReplaceExtension(StringT1& filename, const StringT2& newExtension);



	/*!
	** \brief Normalize a filename
	**
	** The input can be a Windows-style or a Unix-style path, with mixed slashes and anti-slashes.
	** This routine removes dot segments (`.` and `..`) from a given filename (when
	** possible).
	** Any final slash will be removed.
	**
	** \bug The relative filenames like "C:..\\folder1\\folder2" are not handled properly
	**
	** \param[out] out            A string (any class compliant to std::string) where to write the result
	** \param      in             A path/filename to normalize
	** \param      inLength       Length of \p in (optional, -1 for autodetection)
	** \param      replaceSlashes True to replace slashes according the local OS conventions. False to keep
	**                            as it.
	*/
	template<class StringT1, class StringT2>
	void Normalize(StringT1& out, const StringT2& in, unsigned int inLength = (unsigned int)-1,
		bool replaceSlashes = true);






} // namespace IO
} // namespace Yuni

# include "filename-manipulation.hxx"

#endif // __YUNI_IO_FILENAME_MANIPULATION_H__
