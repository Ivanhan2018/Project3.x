#pragma once

#include "NetLog.h"
#include "FvSmartPointer.h"

template <class T>
class FvProvider1 : public FvReferenceCount
{
	FV_NOT_COPY_COMPARE(FvProvider1);
public:
	FvProvider1():m_pkPointer0(&m_kCache0){}
	FvProvider1(const T& kT):m_kCache0(kT), m_pkPointer0(&m_kCache0){}
	virtual ~FvProvider1(){}

	const T& GetData0()const {NetAssert(m_pkPointer0);return *m_pkPointer0;}
	void Attach0(const T& kCache){m_pkPointer0 = &kCache;}
	void Detach0(){NetAssert(m_pkPointer0);m_kCache0 = (*m_pkPointer0);m_pkPointer0 = &m_kCache0;}
	void Detach(){Detach0();}
	bool IsDetach0(){return m_pkPointer0 == &m_kCache0;}
	void SetValue0(const T& kT){m_kCache0 = kT;}

protected:
	T  m_kCache0;
	const T* m_pkPointer0;
};


template <class T, class U>
class FvProvider2 : public FvReferenceCount
{
	FV_NOT_COPY_COMPARE(FvProvider2);
public:
	FvProvider2():m_pkPointer0(&m_kCache0), m_pkPointer1(&m_kCache1){}
	FvProvider2(const T& kT, const U& kU):m_kCache0(kT), m_kCache1(kU), m_pkPointer0(&m_kCache0), m_pkPointer1(&m_kCache1){}
	virtual ~FvProvider2(){}

	const T& GetData0()const{NetAssert(m_pkPointer0);return *m_pkPointer0;}
	const U& GetData1()const{NetAssert(m_pkPointer1);return *m_pkPointer1;}
	void Attach0(const T& kCache){m_pkPointer0 = &kCache;}
	void Attach1(const U& kCache){m_pkPointer1 = &kCache;}
	void Detach0(){NetAssert(m_pkPointer0);m_kCache0 = (*m_pkPointer0);m_pkPointer0 = &m_kCache0;}
	void Detach1(){NetAssert(m_pkPointer1);m_kCache1 = (*m_pkPointer1);m_pkPointer1 = &m_kCache1;}
	void Detach(){Detach0();Detach1();}
	bool IsDetach0(){return m_pkPointer0 == &m_kCache0;}
	bool IsDetach1(){return m_pkPointer1 == &m_kCache1;}
	void SetValue0(const T& kT){m_kCache0 = kT;}
	void SetValue1(const U& kU){m_kCache1 = kU;}

protected:
	T  m_kCache0;
	const T* m_pkPointer0;
	U  m_kCache1;
	const U* m_pkPointer1;
};

template <class T, class U, class V>
class FvProvider3 : public FvReferenceCount
{
	FV_NOT_COPY_COMPARE(FvProvider3);
public:
	FvProvider3():m_pkPointer0(&m_kCache0),m_pkPointer1(&m_kCache1),m_pkPointer2(&m_kCache2){}
	FvProvider3(const T& kT, const U& kU, const V& kV)
		:m_kCache0(kT),m_kCache1(kU),m_kCache2(kV),m_pkPointer0(&m_kCache0),m_pkPointer1(&m_kCache1),m_pkPointer2(&m_kCache2){}
	virtual ~FvProvider3(){}

	const T& GetData0() const{NetAssert(m_pkPointer0);return *m_pkPointer0;}
	const U& GetData1() const{NetAssert(m_pkPointer1);return *m_pkPointer1;}
	const V& GetData2() const{NetAssert(m_pkPointer2);return *m_pkPointer2;}
	void Attach0(const T& kCache){m_pkPointer0 = &kCache;}
	void Attach1(const U& kCache){m_pkPointer1 = &kCache;}
	void Attach2(const V& kCache){m_pkPointer2 = &kCache;}
	void Detach0(){NetAssert(m_pkPointer0);m_kCache0 = (*m_pkPointer0);m_pkPointer0 = &m_kCache0;}
	void Detach1(){NetAssert(m_pkPointer1);m_kCache1 = (*m_pkPointer1);m_pkPointer1 = &m_kCache1;}
	void Detach2(){NetAssert(m_pkPointer2);m_kCache2 = (*m_pkPointer2);m_pkPointer2 = &m_kCache2;}
	void Detach(){Detach0();Detach1();Detach2();}
	bool IsDetach0(){return m_pkPointer0 == &m_kCache0;}
	bool IsDetach1(){return m_pkPointer1 == &m_kCache1;}
	bool IsDetach2(){return m_pkPointer2 == &m_kCache2;}
	void SetValue0(const T& kT){m_kCache0 = kT;}
	void SetValue1(const U& kU){m_kCache1 = kU;}
	void SetValue2(const V& kV){m_kCache2 = kV;}

protected:
	T  m_kCache0;
	const T* m_pkPointer0;
	U  m_kCache1;
	const U* m_pkPointer1;
	V  m_kCache2;
	const V* m_pkPointer2;
};

///
template <class TProvider>
class FvProviderOwner
{
public:
	FvProviderOwner()
	{
		m_spProvider = new TProvider();
	}
	~FvProviderOwner(){Detach();}
	void Detach()
	{
		NetAssert(m_spProvider);
		m_spProvider->Detach();
	}
	const FvSmartPointer<TProvider>& Provider()const{return m_spProvider;}
private:
	FvSmartPointer<TProvider> m_spProvider;
};


namespace Provider_Demo
{
	static void Test()
	{
		int i_1 = 0;
		FvProvider1<int> pd_1;
		pd_1.Attach0(i_1);
		int i_1_ = pd_1.GetData0();
		pd_1.Detach0();

		int i_2 = 0;
		float f_2 = 0.0f;
		FvProvider2<int, float> pd_2;
		pd_2.Attach0(i_2);
		pd_2.Attach1(f_2);
		int i_2_ = pd_2.GetData0();
		float f_2_ = pd_2.GetData1();
		pd_2.Detach0();
		pd_2.Detach1();

		int i_3 = 0;
		float f_3 = 0.0f;
		double d_3 = 0.0;
		FvProvider3<int, float, double> pd_3;
		pd_3.Attach0(i_3);
		pd_3.Attach1(f_3);
		int i_3_ = pd_3.GetData0();
		float f_3_ = pd_3.GetData1();
		double d_3_ = pd_3.GetData2();
		pd_3.Detach0();
		pd_3.Detach1();

	}
}