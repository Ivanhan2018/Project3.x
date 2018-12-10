#ifndef __FvSmartPointer_H__
#define __FvSmartPointer_H__

#include "FvKernel.h"
#include "cocos2d.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <sys/atomics.h>
#endif
class FvReferenceCount
{
public:

	FvReferenceCount()
		: m_iCount(0)
	{
	}

	FvReferenceCount( const FvReferenceCount & /*kOther*/ )
		: m_iCount(0)
	{
	}

	virtual ~FvReferenceCount() {}

	inline void IncRef() const
	{
		++m_iCount;
	}

	inline void DecRef() const
	{
		if (--m_iCount == 0)
			delete const_cast<FvReferenceCount*>(this);
	}

	inline long RefCount() const
	{
		return m_iCount;
	}

private:

	FvReferenceCount & operator=( const FvReferenceCount & kOther )
	{
		return *this;
	}

	volatile mutable long m_iCount;

	friend class FvSafeReferenceCount;
};


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

class FvSafeReferenceCount : public FvReferenceCount
{
public:

	inline void __fastcall IncRef() const
	{
		__asm lock add [ecx]FvReferenceCount.m_iCount, 1;
	}

	inline void __fastcall DecRef() const
	{
		if (this->IntDecRef() == 1)
			delete const_cast<FvSafeReferenceCount*>(this);
	}

	inline int __fastcall IncRefTry() const
	{
		irt_retry:
			__asm mov eax, [ecx]FvReferenceCount.m_iCount
			__asm test eax,eax
			__asm jz irt_end

			__asm mov edx, eax
			__asm inc edx
			__asm lock cmpxchg [ecx]FvReferenceCount.m_iCount, edx
			__asm jnz irt_retry
		irt_end:;
	}

private:
	
#pragma warning (push)
#pragma warning (disable: 4035)
	inline int __fastcall IntDecRef() const
	{
		__asm mov eax, -1
			__asm lock xadd [ecx]FvReferenceCount.m_iCount, eax;
	}
#pragma warning (pop)
};

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

class FvSafeReferenceCount : public FvReferenceCount
{
public:

	inline void IncRef() const
	{
		__sync_fetch_and_sub ( ( volatile int  * )&m_iCount ,1);
	}

	inline void DecRef() const
	{
		if (this->IntDecRef() == 1)
			delete const_cast<FvSafeReferenceCount*>(this);
	}


private:

	inline int IntDecRef() const
	{
		return __sync_fetch_and_sub ( ( volatile int  * )&m_iCount ,1) + 1;
	}
};
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include <libkern/OSAtomic.h>
class FvSafeReferenceCount : public FvReferenceCount
{
public:
    
	inline void IncRef() const
	{
		OSAtomicIncrement32( ( volatile int32_t *)&m_iCount );
	}
    
	inline void DecRef() const
	{
		if (this->IntDecRef() == 1)
			delete const_cast<FvSafeReferenceCount*>(this);
	}
    
private:
	
	inline int IntDecRef() const
	{
		return OSAtomicIncrement32( ( volatile int32_t * )&m_iCount ) + 1;
	}
};
#else

class FvSafeReferenceCount : public FvReferenceCount
{
public:

	inline void IncRef() const
	{
		__asm__ volatile (
			"lock addl $1, %0"
			:						
		: "m"	(this->m_iCount) 	
			: "memory" 				
			);
	}

	inline void DecRef() const
	{
		if (this->IntDecRef() == 1)
			delete const_cast<FvSafeReferenceCount*>(this);
	}

	
	inline int IncRefTry() const
	{
		int ret, incced_val;
		__asm__ volatile (
			"1:\n\t\t"
			"mov %2, %0\n\t\t"	// %0 == eax
			"test %%eax, %%eax\n\t\t"
			"jz 2f\n\t\t"
			"mov %%eax, %1\n\t\t"
			"inc %1\n\t\t"
			"lock cmpxchg %1, %2\n\t\t"
			"jnz 1b\n"
			"2:"
			: 	"=&a"	(ret), 			
			"=&q"	(incced_val) 	
			: "m"		(this->m_iCount)	
			: "memory"					
			);
		return ret;
	}

private:

	inline int IntDecRef() const
	{
		int ret;
		__asm__ volatile (
			"mov $-1, %0  \n\t"
			"lock xadd %0, %1"
			: "=&a"	(ret)				
			: "m"	(this->m_iCount)		
			: "memory"
			);
		return ret;
	}
};

#endif // _WIN32

template <class Ty>
inline void IncrementReferenceCount( const Ty &Q )
{
	Q.IncRef();
}

template <class Ty>
inline bool HasZeroReferenceCount( const Ty &Q )
{
	return (Q.RefCount() == 0);
}

template <class Ty>
inline void DecrementReferenceCount( const Ty &Q )
{
	Q.DecRef();
}

template <class Ty>
inline int IncrementReferenceCountAttempt( const Ty &Q )
{
	return Q.IncRefTry();
}


template <class Ty> inline
void NullPointerCheck( const Ty * /*P*/ )
{
	
}

template<class Ty> 
class FvConstSmartPointer
{
public:
	static const bool STEAL_REFERENCE = true;
	static const bool NEW_REFERENCE = false;
	enum TRY_tag { FALLIBLE };

	typedef Ty Object;
	typedef FvConstSmartPointer<Ty> This;

public:

	FvConstSmartPointer( const Object *P = 0, bool alreadyIncremented = false )
	{
		m_pkObject = P;
		if (m_pkObject)
		{
			if (!alreadyIncremented)
			{
				IncrementReferenceCount( *m_pkObject );
			}
			else
			{
				if (HasZeroReferenceCount( *m_pkObject ))
				{
					//FV_ASSERT_DEV( 0 &&
					//	"FvConstSmartPointer::FvConstSmartPointer - The reference "
					//	"count should not be zero\nfor objects that already "
					//	"have their reference incremented" );
				}
			}
		}
	}

	FvConstSmartPointer( const Object *P, TRY_tag ) :
	m_pkObject( IncrementReferenceCountAttempt( *P ) ? P : 0 ) { }

	FvConstSmartPointer( const This& P )
	{
		m_pkObject = P.Get();
		if (m_pkObject) IncrementReferenceCount( *m_pkObject );
	}

	This& operator=( const This& X )
	{
		if (m_pkObject != X.Get())
		{
			const Object* pOldObj = m_pkObject;
			m_pkObject = X.Get();
			if (m_pkObject) IncrementReferenceCount( *m_pkObject );
			if (pOldObj) DecrementReferenceCount( *pOldObj );
		}
		return *this;
	}

	~FvConstSmartPointer()
	{
		if (m_pkObject) DecrementReferenceCount( *m_pkObject );
		m_pkObject = 0;
	}

	const Object * Get() const
	{
		return m_pkObject;
	}

	const Object * GetObject() const
	{
		return this->Get();
	}

	bool HasObject() const
	{
		return m_pkObject != 0;
	}

	bool Exists() const
	{
		return m_pkObject != 0;
	}

	const Object& operator*() const
	{
		NullPointerCheck( m_pkObject );
		return *m_pkObject;
	}

	const Object* operator->() const
	{
		NullPointerCheck( m_pkObject );
		return m_pkObject;
	}

	friend bool operator==( const FvConstSmartPointer<Ty>& A,
		const FvConstSmartPointer<Ty>& B )
	{
		return A.m_pkObject == B.m_pkObject;
	}

	friend bool operator==( const FvConstSmartPointer<Ty>& A,
		const Ty* B )
	{
		return A.m_pkObject == B;
	}

	friend bool operator==( const Ty* A,
		const FvConstSmartPointer<Ty>& B )
	{
		return A == B.m_pkObject;
	}

	friend bool operator!=( const FvConstSmartPointer<Ty>& A,
		const FvConstSmartPointer<Ty>& B )
	{
		return A.m_pkObject != B.m_pkObject;
	}

	friend bool operator!=( const FvConstSmartPointer<Ty>& A,
		const Ty* B )
	{
		return A.m_pkObject != B;
	}

	friend bool operator!=( const Ty* A,
		const FvConstSmartPointer<Ty>& B )
	{
		return A != B.m_pkObject;
	}

	friend bool operator<( const FvConstSmartPointer<Ty>& A,
		const FvConstSmartPointer<Ty>& B )
	{
		return A.m_pkObject < B.m_pkObject;
	}

	friend bool operator<( const FvConstSmartPointer<Ty>& A,
		const Ty* B )
	{
		return A.m_pkObject < B;
	}

	friend bool operator<( const Ty* A,
		const FvConstSmartPointer<Ty>& B )
	{
		return A < B.m_pkObject;
	}

	friend bool operator>( const FvConstSmartPointer<Ty>& A,
		const FvConstSmartPointer<Ty>& B )
	{
		return A.m_pkObject > B.m_pkObject;
	}

	friend bool operator>( const FvConstSmartPointer<Ty>& A,
		const Ty* B )
	{
		return A.m_pkObject > B;
	}

	friend bool operator>( const Ty* A,
		const FvConstSmartPointer<Ty>& B )
	{
		return A > B.m_pkObject;
	}

	typedef const Ty * FvConstSmartPointer<Ty>::*unspecified_bool_type;
	operator unspecified_bool_type() const
	{
		return m_pkObject == 0? 0 : &FvConstSmartPointer<Ty>::m_pkObject;
	}

protected:
	const Object *m_pkObject;		
};

template <class Ty>
class FvSmartPointer : public FvConstSmartPointer<Ty>
{
public:

	typedef FvSmartPointer<Ty> This;
	typedef FvConstSmartPointer<Ty> ConstProxy;
	typedef Ty Object;

public:
	
	FvSmartPointer( Object *P = 0, bool alreadyIncremented = false ) :
	  ConstProxy( P, alreadyIncremented ) { }

	FvSmartPointer( Object *P, typename ConstProxy::TRY_tag tag ) :
	ConstProxy( P, tag ) { }

	FvSmartPointer( const This& P ) : ConstProxy( P ) { }

	template<class DerivedType>
	FvSmartPointer( FvConstSmartPointer<DerivedType>& dt ) :
	ConstProxy( dt.Get() )
	{
	}

	This& operator=( const This& P )
	{
	  ConstProxy::operator=(P);
	  return *this;
	}

	template<class DerivedType>
	This& operator=( FvConstSmartPointer<DerivedType>& dt )
	{
	  ConstProxy::operator=(dt.Get());
	  return *this;
	}

	Object * Get() const
	{
	  return const_cast<Object *>( this->m_pkObject );
	}

	Object * GetObject() const
	{
	  return this->Get();
	}

	Object& operator*() const
	{
	  NullPointerCheck( this->m_pkObject );
	  return *const_cast<Object *>( this->m_pkObject );
	}

	Object* operator->() const
	{
	  NullPointerCheck( this->m_pkObject );
	  return const_cast<Object *>( this->m_pkObject );
	}
};

#endif // __FvSmartPointer_H__