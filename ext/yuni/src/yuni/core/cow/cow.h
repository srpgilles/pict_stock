#ifndef __YUNI_CORE_COW_COW_H__
# define __YUNI_CORE_COW_COW_H__

# include "../../yuni.h"
# include "../smartptr.h"


namespace Yuni
{

	/*!
	** \brief COW Idiom (copy-on-write, also known as implicit sharing)
	*/
	template <class T,                                                        // The original type
		template <class> class OwspP = Policy::Ownership::ReferenceCountedMT, // Ownership policy
		template <class> class ChckP = Policy::Checking::None,                // Checking policy
		class ConvP                  = Policy::Conversion::Allow,             // Conversion policy
		template <class> class StorP = Policy::Storage::Pointer,              // Storage policy
		template <class> class ConsP = Policy::Constness::DontPropagateConst  // Constness policy
		>
	class YUNI_DECL COW
	{
	public:
		//! \name Type alias
		//@{
		//! Original type
		typedef T  Type;
		//! The Storage policy
		typedef StorP<T>  StoragePolicy;
		//! The Ownership policy
		typedef OwspP<typename StorP<T>::PointerType>  OwnershipPolicy;
		//! The conversion policy
		typedef ConvP  ConversionPolicy;
		//! The checking policy
		typedef ChckP<typename StorP<T>::StoredType>  CheckingPolicy;
		//! The Constness policy
		typedef ConsP<T>  ConstnessPolicy;

		//! the Pointer type
		typedef typename StoragePolicy::PointerType    PointerType;
		//! The Stored type
		typedef typename StoragePolicy::StoredType     StoredType;
		//! The Reference type
		typedef typename StoragePolicy::ReferenceType  ReferenceType;
		//! The Pointer type (const)
		typedef typename ConstnessPolicy::Type*        ConstPointerType;
		//! The Reference type (const)
		typedef typename ConstnessPolicy::Type&        ConstReferenceType;


		//! Alias to itself
		typedef COW<T,OwspP,ChckP,ConvP,StorP,ConsP> COWType;
		//! Alias to smartptr
		typedef SmartPtr<T,OwspP,ChckP,ConvP,StorP,ConsP> SmartPtrType;
		//! A smart pointer with a const pointer
		typedef SmartPtr<T,OwspP,ChckP,ConvP,StorP,Policy::Constness::PropagateConst> ConstSmartPtrType;
		//! A smart pointer with a non-const pointer
		typedef SmartPtr<T,OwspP,ChckP,ConvP,StorP,Policy::Constness::DontPropagateConst> NonConstSmartPtrType;

		//! The Type to use for the copy constructor
		typedef typename Static::If<OwnershipPolicy::destructiveCopy, SmartPtrType, const SmartPtrType>::ResultType  CopyType;

		//! Vector
		typedef std::vector<COWType>  Vector;
		//! List
		typedef std::list<COWType>  List;
		//@} // alias

	public:
		COW()
		{
		}

		COW(const COW& rhs) :
			pValue(rhs.pValue)
		{
		}

		template<class U>
		COW(const U& rhs) :
			pValue(rhs)
		{
		}

		T& operator * ()
		{
			copy();
			return *pValue;
		}

		const T& operator * () const
		{
			return *pValue;
		}


		T* operator -> ()
		{
			copy();
			return pValue.operator -> ();
		}

		const T* operator -> () const
		{
			return pValue.operator -> ();
		}

		COW& operator = (const COW& rhs)
		{
			pValue = rhs.pValue;
			return *this;
		}

		template<class U>
		COW& operator = (const U& rhs)
		{
			pValue = rhs;
			return *this;
		}

		bool operator ! () const
		{
			return !pValue;
		}

		bool operator == (const COW& rhs) const
		{
			return pValue == rhs.pValue;
		}

		bool operator != (const COW& rhs) const
		{
			return pValue != rhs.pValue;
		}


	private:
		//! Perform a deep-copy if required
		void copy();

	private:
		//! The real value
		SmartPtrType pValue;

	}; // class COW - copy-on-write






} // namespace Yuni

# include "cow.hxx"

#endif // __YUNI_CORE_COW_COW_H__
