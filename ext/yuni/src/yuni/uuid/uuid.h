#ifndef __YUNI_UUID_UUID_H__
# define __YUNI_UUID_UUID_H__

# include "../yuni.h"
# include "fwd.h"
# include "../core/traits/cstring.h"
# include "../core/traits/length.h"



namespace Yuni
{

	/*!
	** \brief Universally unique identifier (UUID)
	**
	** Printing to cout a new UUID :
	** \code
	** #include <yuni/yuni.h>
	** #include <yuni/uuid/uuid.h>
	** #include <iostream>
	**
	** using namespace Yuni;
	**
	** int main()
	** {
	**	UUID uuid;
	**	uuid.generate();
	**	std::cout << "GUID: " << uuid << std::endl;
	**	return 0;
	** }
	** \endcode
	**
	** Conversions :
	** \code
	** UUID uuid;
	** uuid.generate();
	** String s = uuid;
	** std::cout << s << std::endl;
	**
	** uuid = "BDBBD619-DA48-4BAE-B62A-CFFF1C291A8C";
	** std::cout << uuid << std::endl;
	**
	** s = "70395DB8-E113-46D8-A2C0-522B59B03A96";
	** uuid = s;
	** std::cout << uuid << std::endl;
	** \endcode
	*/
	class YUNI_DECL UUID
	{
	public:
		enum Flag
		{
			//! Set the UUID to null
			fNull,
			//! Generate a new UUID
			fGenerate,
		};

	public:
		//! \name Constructor
		//@{
		/*!
		** \brief Default constructor
		**
		** The UUID will be set to null.
		*/
		UUID();
		/*!
		** \brief Copy constructor
		*/
		UUID(const UUID& rhs);
		/*!
		** \brief Constructor with flags
		**
		** \param flag Flag to know how to initialize the UUID
		*/
		UUID(Flag flag);
		/*!
		** \brief Constructor from a string
		*/
		template<class StringT> UUID(const StringT& string);
		//@}


		//! \name UUID
		//@{
		/*!
		** \brief Generate a new UUID
		*/
		void generate();

		/*!
		** \brief Set the UUID to null
		*/
		void clear();

		//! Get if the UUID is null (e.g. 000000000-0000-0000-0000-00000000000)
		bool null() const;

		/*!
		** \brief Assign from a string
		**
		** Contrary to the operator =, the internal value will remain untouched
		** if the conversion failed.
		*/
		template<class StringT> bool assign(const StringT& string);
		//@}


		//! \name Hashing
		//@{
		/*!
		** \brief Get the hash of the uuid
		*/
		size_t hash() const;
		//@}


		//! \name Operators
		//@{
		//! Copy operator
		UUID& operator = (const UUID& rhs);
		/*!
		** \brief Assignment from a string
		**
		** The UUID will be reset if the conversion failed.
		*/
		template<class StringT> UUID& operator = (const StringT& string);
		//! Null
		bool operator ! () const;
		//! Comparison
		bool operator == (const UUID& rhs) const;
		//! Comparison
		bool operator != (const UUID& rhs) const;
		//! Strict weak comparison
		bool operator < (const UUID& rhs) const;
		//@}


	protected:
		/*!
		** \brief Write the UUID into string in a C-String
		**
		** \param cstring A char* string, which must be 42-bytes length
		** \internal 42 bytes are required on Windows (more rooms are needed)
		*/
		void writeToCString(char cstring[42]) const;

		/*!
		** \brief Initialize the UUID from a c-string
		*/
		bool initializeFromCString(const char* cstring);

	public:
		union StorageType
		{
			unsigned char cstring[16];
			char signedcstring[16];
			uint32 n32[4];
		};

	private:
		//! Inner value
		StorageType pValue;
		// Friend
		friend class Yuni::Private::UUID::Helper;

	}; // class UUID





} // namespace Yuni

# include "uuid.hxx"

#endif // __YUNI_UUID_UUID_H__
