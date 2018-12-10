//{future header message}
#ifndef __FvSingleton_H__
#define __FvSingleton_H__
#include "cocos2d.h"

template <class T>
class FvSingleton
{
protected:
	static T *ms_pkInstance;

public:
	FvSingleton()
	{
		CCASSERT(ms_pkInstance == NULL,"");
		ms_pkInstance = static_cast< T * >( this );
	}

	~FvSingleton()
	{
		ms_pkInstance = 0;
	}

	static T &Instance()
	{
		if (ms_pkInstance == nullptr)
		{
			new T;
		}
		return *ms_pkInstance;
	}

	static T *pInstance()
	{
		return ms_pkInstance;
	}
};

#define FV_SINGLETON_STORAGE( TYPE )						\
template <>													\
TYPE * FvSingleton< TYPE >::ms_pkInstance = 0;			\


template <class T>
class FvSingletonBase
{
protected:
	static T *ms_pkInstanceBase;

public:
	FvSingletonBase()
	{
		CCASSERT(ms_pkInstanceBase == NULL,"");
		ms_pkInstanceBase = static_cast< T * >( this );
	}

	~FvSingletonBase()
	{
		ms_pkInstanceBase = 0;
	}

	static T &InstanceBase()
	{
		CCASSERT(ms_pkInstanceBase != NULL,"InstanceBase");
		return *ms_pkInstanceBase;
	}

	static T *pInstanceBase()
	{
		return ms_pkInstanceBase;
	}
};

#define FV_SINGLETON_STORAGEBASE( TYPE )						\
	template <>													\
	TYPE * FvSingletonBase< TYPE >::ms_pkInstanceBase = 0;			\



#endif // __FvSingleton_H__
