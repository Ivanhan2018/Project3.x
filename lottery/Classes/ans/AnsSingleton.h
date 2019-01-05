/*
** project defination
** author zhouwei
** date: 2012.04.25
*/

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <cassert>

namespace ans
{
class SingletonBase{};

template <typename T, typename Base = SingletonBase>
class Singleton : public Base
{
public:
    static inline T* instance()
	{
        if (!sm_pInst)
		{
            sm_pInst = new T;
        }
        return sm_pInst;
    }

    static void release()
	{
		if (sm_pInst != 0)
		{
			delete sm_pInst;
			sm_pInst = 0;
		}
    }

protected:
	Singleton(): Base() {assert(sm_pInst == 0);}

	template<typename A1>
	Singleton(A1 a1): Base(a1) {assert(sm_pInst == 0);}

	template<typename A1, typename A2>
	Singleton(A1 a1, A2 a2): Base(a1, a2) {assert(sm_pInst == 0);}

	template<typename A1, typename A2, typename A3>
	Singleton(A1 a1, A2 a2, A3 a3): Base(a1, a2, a3) {assert(sm_pInst == 0);}

	template<typename A1, typename A2, typename A3, typename A4>
	Singleton(A1 a1, A2 a2, A3 a3, A4 a4): Base(a1, a2, a3, a4) {assert(sm_pInst == 0);}

	template<typename A1, typename A2, typename A3, typename A4, typename A5>
	Singleton(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5): Base(a1, a2, a3, a4, a5) {assert(sm_pInst == 0);}

	virtual ~Singleton(){}

public:
    static T* sm_pInst;
};

}

// -----------------------------------------------------------------------
// declarations
// -----------------------------------------------------------------------
#define SINGLETON_DEFINE_INST(T, ...)\
	template<>\
	T* ans::Singleton<T, ##__VA_ARGS__>::sm_pInst = NULL

#define SINGLETON_FRIEND_SUB(T, ...)\
	friend T* ans::Singleton<T, ##__VA_ARGS__>::instance();\
	friend void ans::Singleton<T, ##__VA_ARGS__>::release();

#endif
