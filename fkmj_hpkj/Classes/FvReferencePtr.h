//{future header message}
#ifndef __FvReferencePtr_H__
#define __FvReferencePtr_H__

#include "FvDoubleLink.h"

//+-----------------------------------------------------------------------------------------------------------------

class FvReferencePtrList
{
public:
	FvReferencePtrList();
	~FvReferencePtrList();

	void _ClearReference()const;

protected:
	void _AddReference(FvDoubleLinkNode2<void*>& kNode)const;

private:
	mutable FvDoubleLink<void*> m_ReferencePtrList;
	template <class > friend class FvReferencePtr;
	friend class FvDynamicReferencePtr;
};

#define FV_REFERENCE_HEAD \
	void _ClearReference()const{REFERENCE_PTR_LIST._ClearReference();}\
	FvReferencePtrList REFERENCE_PTR_LIST;

//+-----------------------------------------------------------------------------------------------------------------

template <class T> class FvReferencePtr
{
public:
	//+--------------------------------------------------
	FvReferencePtr(T* pObject = (T*)NULL);
	FvReferencePtr(const FvReferencePtr& ptr);
	~FvReferencePtr();

	//+--------------------------------------------------
	operator T*() const;
	T& operator*() const;
	T* operator->() const;

	//+--------------------------------------------------
	FvReferencePtr& operator=(const FvReferencePtr& ptr);
	FvReferencePtr& operator=(T* pObject);

	//+--------------------------------------------------
	bool operator==(T* pObject) const;
	bool operator!=(T* pObject) const;
	bool operator==(const FvReferencePtr& ptr) const;
	bool operator!=(const FvReferencePtr& ptr) const;

protected:

	void _SetObj(T* pkObj)
	{
		if(_GetObj())
		{
			m_kNode.Detach();
			m_kNode.m_Content = NULL;
		}
		if(pkObj)
		{
			m_kNode.m_Content = pkObj;
			pkObj->REFERENCE_PTR_LIST._AddReference((FvDoubleLinkNode2<void*>&)m_kNode);
		}
	}
	T* _GetObj()const{return m_kNode.m_Content;}
	//+--------------------------------------------------

	FvDoubleLinkNode2<T*> m_kNode;
};
//+-----------------------------------------------------------------------------------------------------------------

template <class T>
inline FvReferencePtr<T>::FvReferencePtr(T* pkObj)
{
	if(pkObj)
	{
		m_kNode.m_Content = pkObj;
		pkObj->REFERENCE_PTR_LIST._AddReference((FvDoubleLinkNode2<void*>&)m_kNode);
	}
}
//+-----------------------------------------------------------------------------------------------------------------

template <class T>
inline FvReferencePtr<T>::FvReferencePtr(const FvReferencePtr& ptr)
{
	if(ptr._GetObj())
	{
		m_kNode.m_Content = ptr._GetObj();
		ptr._GetObj()->REFERENCE_PTR_LIST._AddReference((FvDoubleLinkNode2<void*>&)m_kNode);
	}
}
//+-----------------------------------------------------------------------------------------------------------------
template <class T>
inline FvReferencePtr<T>::~FvReferencePtr()
{
}
//+-----------------------------------------------------------------------------------------------------------------
template <class T>
inline FvReferencePtr<T>::operator T*() const
{
	return _GetObj();
}
//+-----------------------------------------------------------------------------------------------------------------
template <class T>
inline T& FvReferencePtr<T>::operator*() const
{
	return *_GetObj();
}
//+-----------------------------------------------------------------------------------------------------------------
template <class T>
inline T* FvReferencePtr<T>::operator->() const
{
	return _GetObj();
}
//+-----------------------------------------------------------------------------------------------------------------
template <class T>
inline FvReferencePtr<T>& FvReferencePtr<T>::operator=(const FvReferencePtr& ptr)
{
	if (_GetObj() != ptr._GetObj())
	{
		_SetObj(ptr._GetObj());
	}
	return *this;
}
//+-----------------------------------------------------------------------------------------------------------------
template <class T>
inline FvReferencePtr<T>& FvReferencePtr<T>::operator=(T* pObject)
{
	_SetObj(pObject);
	return *this;
}
//+-----------------------------------------------------------------------------------------------------------------
template <class T>
inline bool FvReferencePtr<T>::operator==(T* pObject) const
{
	return (_GetObj() == pObject);
}
//+-----------------------------------------------------------------------------------------------------------------
template <class T>
inline bool FvReferencePtr<T>::operator!=(T* pObject) const
{
	return (_GetObj() != pObject);
}
//+-----------------------------------------------------------------------------------------------------------------
template <class T>
inline bool FvReferencePtr<T>::operator==(const FvReferencePtr& ptr) const
{
	return (_GetObj() == ptr._GetObj());
}
//+-----------------------------------------------------------------------------------------------------------------
template <class T>
inline bool FvReferencePtr<T>::operator!=(const FvReferencePtr& ptr) const
{
	return (_GetObj() != ptr._GetObj());
}

//+-----------------------------------------------------------------------------------------------------------------


//+-----------------------------------------------------------------------------------------
//+-----------------------------------------------------------------------------------------
///<²âÊÔÓÃÀý>
namespace Reference_Demo
{
	class FvRefTest1
	{
	public:
		FV_REFERENCE_HEAD;
	};

	static void Test()
	{
		FvRefTest1* pNew = new FvRefTest1();
		FvRefTest1* pNewErr = pNew;
		FvReferencePtr<FvRefTest1> rpRefTest_1 = pNew;
		FvReferencePtr<FvRefTest1> rpRefTest_2 = rpRefTest_1;
		FvReferencePtr<FvRefTest1> rpRefTest_3 = pNew;
		rpRefTest_3 = 0;
		delete pNew;

		///<Const>
		pNew = new FvRefTest1();
		const FvRefTest1* rpConstObj = pNew;
		FvReferencePtr<const FvRefTest1> rpRefTest_4 = rpConstObj;
		rpConstObj = rpRefTest_4;
		delete pNew;
	};
}

#endif//__FvReferencePtr_H__