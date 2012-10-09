#ifndef __YUNI_IO_FILE_STREAM_H__
# define __YUNI_IO_FILE_STREAM_H__

# include "../../yuni.h"
# include "../../core/static/assert.h"
# include "../../core/string.h"
# include "../../core/static/remove.h"
# include "openmode.h"
# include <stdio.h>



namespace Yuni
{
namespace IO
{
namespace File
{

	/*!
	** \brief Seek origin
	*/
	enum SeekOrigin
	{
		//! From the begining of the stream
		seekOriginBegin,
		//! From the current position in the stream
		seekOriginCurrent,
		//! From the end of the stream
		seekOriginEnd,
	};




	/*!
	** \brief A low-level implementation for reading and writing files
	**
	** The file will be automatically closed (if not already done) at the
	** destruction of the object.
	**
	** Here is a simple example for reading a file, line by line :
	** \code
	** IO::File::Stream file;
	** // opening out file
	** if (file.open("myfile.txt"))
	** {
	** 		// A buffer. The given capacity will be the maximum length for a single line
	** 		Clob buffer;
	** 		while (file.readline(buffer))
	** 		{
	** 			// do something with the buffer
	** 			// here we will merely dump it to the std::cout
	** 			std::cout << buffer << std::endl;
	** 		}
	** }
	** // the file will be implicitely closed here
	** \endcode
	** Obviously, you can also consider \p IO::File::ReadLineByLine.
	**
	** When writing a data into a file, the data will be written 'as it' if it can
	** be represented by a mere c-string. Otherwise a 'Yuni::String' will be used
	** to perform the convertion.
	** \code
	** IO::File::Stream file;
	** if (file.open("out.txt", IO::OpenMode::write | IO::OpenMode::truncate))
	** {
	** 		file << "Without implicit convertion: Hello world !\n";
	** 		file << "With implicit convertion   : " << 42 << '\n';
	** }
	** \endcode
	**
	** \internal This implementation is most of the time a C++ wrapper over the standard
	**   routines 'fopen', 'fclose'... The implementation is a bit different on Windows
	**   because 'fopen' only handles ansi filenames.
	*/
	class Stream
	{
	public:
		//! The native handle type
		typedef FILE* HandleType;

	public:
		//! \name Constructor & Destructor
		//@{
		/*!
		** \brief Default Constructor
		*/
		Stream();
		/*!
		** \brief Copy constructor (not allowed, it will fail at compile time)
		*/
		Stream(const Stream& rhs);
		/*!
		** \brief Open a file
		*/
		explicit Stream(const AnyString& filename, int mode = OpenMode::read);
		/*!
		** \brief Destructor
		**
		** The file will be closed if necessary
		*/
		~Stream();
		//@}


		//! \name Open / Close a file
		//@{
		/*!
		** \brief Open a file
		**
		** The file will be previously closed if necessary.
		**
		** \param filename Any string reprensenting an UTF8 relative or absolute filename
		** \param mode The open mode to use
		** \return True if the operation succeeded, false otherwise
		*/
		bool open(const AnyString& filename, int mode = OpenMode::read);

		/*!
		** \brief Open a file for writing
		**
		** This method is strictly equivalent to :
		** \code
		** open(filename, IO::OpenMode::write | IO::OpenMode::truncate);
		** \endcode
		** The file will be previously closed if necessary.
		**
		** \param filename Any string reprensenting an UTF8 relative or absolute filename
		** \return True if the operation succeeded, false otherwise
		*/
		bool openRW(const AnyString& filename);

		/*!
		** \brief Close the file if opened
		*/
		bool close();
		//@}


		//! \name Stream
		//@{
		/*!
		** \brief Get if a file is currently opened
		*/
		bool opened() const;

		/*!
		** \brief Get if the end-of-file has been reached
		*/
		bool eof() const;

		/*!
		** \brief Get the current value of the file position indicator
		*/
		ssize_t tell() const;

		/*!
		** \brief Set the position in the stream
		**
		** \param offset A relative offset
		** \param origin Origin of the offset
		** \return True if the operation succeeded, false otherwise
		*/
		bool seek(ssize_t offset, SeekOrigin origin = seekOriginCurrent);

		/*!
		** \brief Set the position indicator associated with the currently opened file from the beginning of the file
		**
		** \param offset A relative offset
		** \return True if the operation succeeded, False otherwise
		*/
		bool seekFromBeginning(ssize_t offset);

		/*!
		** \brief Set the position indicator associated with the currently opened file from the end of the file
		**
		** \param offset A relative offset
		** \return True if the operation succeeded, False otherwise
		*/
		bool seekFromEndOfFile(ssize_t offset);

		/*!
		** \brief Move the position indicator from the current position in the stream
		**
		** \param offset A relative offset
		** \return True if the operation succeeded, False otherwise
		*/
		bool seekMove(ssize_t offset);

		/*!
		** \brief Flush the last I/O operations
		**
		** \return True if the operation succeeded, False otherwise
		*/
		bool flush();
		//@}


		//! \name Read
		//@{
		/*!
		** \brief Read a single char
		*/
		char get();

		/*!
		** \brief Read a line from the file
		**
		** \param buffer The buffer where to write the line
		** \param maxSize The maximum allowed size for the buffer
		*/
		bool readline(char* buffer, size_t maxSize);

		/*!
		** \brief Read a line from the file
		**
		** It reads a line into the buffer pointed to by #s until either a terminating
		** newline or EOF, which it replaces with ’\0’.
		** The maximum number of char read is `buffer.chunkSize`. For code robutness
		** (to prevent against misuses) this routine will reserve space according to
		** the chunk size.
		**
		** The newline, if any, is retained.
		**
		** \param buffer The buffer where to write the line
		** \param trim   True to remove any trailing linefeed
		*/
		template<class StringT> bool readline(StringT& buffer, bool trim = true);

		/*!
		** \brief Read a line from the file (with a custom chunk size)
		**
		** It reads a line into the buffer pointed to by #s until either a terminating
		** newline or EOF, which it replaces with ’\0’.
		** The maximum number of char read is `CustomChunkT`. For code robutness
		** (to prevent against misuses) this routine will reserve space according to
		** the chunk size.
		**
		** The newline, if any, is retained.
		**
		** \param buffer The buffer where to write the line
		** \param trim   True to remove any trailing linefeed
		*/
		template<uint CustomChunkT, class StringT> bool readline(StringT& buffer, bool trim = true);


		/*!
		** \brief Read data into a string buffer
		**
		** \param buffer An arbitrary buffer (Yuni::String)
		** \param size The size in bytes to read
		** \return The number of bytes that have been read
		*/
		template<uint CSizeT, bool ExpT, bool ZeroT>
		uint read(CString<CSizeT,ExpT,ZeroT>&  buffer, uint size);

		/*!
		** \brief Read a buffer
		**
		** \param buffer A raw buffer where to store the data which will be read from the file
		** \param size Size (in bytes) of the data to read (and size of the buffer)
		** \return The number of bytes that have been read
		*/
		uint read(char* buffer, uint size);

		/*!
		** \brief Read data into a string buffer
		**
		** 'buffer.size() * sizeof(C)' bytes will be read from the stream and store
		** into the given buffer.
		** Use the method 'buffer.resize()' to change the buffer size before calling this method.
		**
		** \param buffer An arbitrary buffer
		** \return The number of bytes that have been read
		*/
		template<uint CSizeT, bool ExpT, bool ZeroT>
		uint chunckRead(CString<CSizeT,ExpT,ZeroT>&  buffer);
		//@}


		//! \name Write
		//@{
		/*!
		** \brief Write a chr to the stream
		*/
		bool put(char c);

		/*!
		** \brief Write a raw buffer
		**
		** \param buffer An arbitrary buffer
		** \param size Size of the buffer to write
		** \return The number of bytes that have been written
		*/
		uint write(const char* buffer, uint size);

		/*!
		** \brief Write any arbitrary buffer
		**
		** \param buffer An arbitrary buffer (const char*, String, CString)
		** \return The number of bytes that have been written
		*/
		template<class U> uint write(const U& buffer);

		/*!
		** \brief Write any arbitrary buffer
		**
		** \param buffer An arbitrary buffer (const char*, String, CString)
		** \param size Size of the buffer to write
		** \return The number of bytes that have been written
		*/
		template<class U> uint write(const U& buffer, uint size);
		//@}


		//! \name Locking
		//@{
		/*!
		** \brief Lock the file for a shared access
		**
		** \warning The implementation is missing on Windows (#346)
		** \warning The user must call `unlock`
		*/
		bool lockShared();
		/*!
		** \brief Lock the file for an exclusive access
		**
		** \warning The implementation is missing on Windows (#346)
		** \warning The user must call `unlock`
		*/
		bool lockExclusive();

		/*!
		** \brief Unlock the file
		**
		** \warning The implementation is missing on Windows (#346)
		*/
		void unlock();
		//@}


		//! \name Native
		//@{
		//! Get the OS Dependant handle
		HandleType nativeHandle() const;
		//@}


		//! \name Operators
		//@{
		//! True if the stream if not opened
		bool operator ! () const;
		//! operator += (write)
		template<class U> Stream& operator += (const U& u);
		//! operator += (write)
		Stream& operator += (const char c);

		//! operator << (write)
		template<class U> Stream& operator << (const U& u);
		//! operator << (write)
		Stream& operator << (const char c);

		//! Operator >> (read)
		template<class U> Stream& operator >> (U& rhs);
		//@}


	private:
		//! A FILE pointer
		HandleType pFd;

	}; // class Stream






} // namespace File
} // namespace IO
} // namespace Yuni

# include "stream.hxx"

#endif // __YUNI_IO_FILE_STREAM_H__
