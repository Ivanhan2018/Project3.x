#pragma once
#include "cocos2d.h"
#include "FvSingleton.h"

class TimeNode;

typedef void (TimeNode::*Time_Schedule)();
typedef void (TimeNode::*Time_ScheduleInt)(int);

#define TIME_CALLBACK(pFun,pPoint) Time_Schedule(&pFun),pPoint
#define TIME_CALLBACK_Int(pFun,pPoint) Time_ScheduleInt(&pFun),pPoint


struct Time_CBInfoBase
{
	Time_CBInfoBase()
		:iIdex(0)
		,fTotalTime(0)
		,iTag(0)
	{}
	double fTotalTime;
	int iIdex;
	int iTag;
	double fDestTime;
	double fPastTime;
	TimeNode* pPoint;
	virtual bool callFun() = 0;
};

struct Time_CBInfo1
	:Time_CBInfoBase
{
	virtual bool callFun()
	{
		return fun();
	}
	std::function<bool()> fun;
};

template<class T> 
struct Time_CBInfo2
	:Time_CBInfoBase
{
	Time_CBInfo2()
	{}
	virtual bool callFun()
	{
		return fun(_Value);
	}
	std::function<bool(T)> fun;
	T _Value;
};

template<class T1,class T2> 
struct Time_CBInfo3
	:Time_CBInfoBase
{
	Time_CBInfo3()
	{}
	virtual bool callFun()
	{
		return fun(_Value1,_Value2);
	}
	std::function<bool(T1,T2)> fun;
	T1 _Value1;
	T2 _Value2;
};

template<class T1,class T2,class T3> 
struct Time_CBInfo4
	:Time_CBInfoBase
{
	Time_CBInfo4()
	{}
	virtual bool callFun()
	{
		return fun(_Value1,_Value2,_Value3);
	}
	std::function<bool(T1,T2,T3)> fun;
	T1 _Value1;
	T2 _Value2;
	T3 _Value3;
};

struct Time_CBInfo
	:Time_CBInfoBase
{
	Time_CBInfo()
		:bStill(false)
	{}
	virtual bool callFun()
	{
		(pPoint->*fun)();
		return bStill;
	}
	bool bStill;
	Time_Schedule fun;
};

struct Time_CBInfoInt
	:Time_CBInfoBase
{
	Time_CBInfoInt()
		:bStill(false)
		,iValue(0)
	{}
	virtual bool callFun()
	{
		(pPoint->*fun)(iValue);
		return bStill;
	}
	bool bStill;
	int  iValue;
	Time_ScheduleInt fun;
};

typedef std::vector<Time_CBInfoBase*> Time_CBInfoList;

class TimeManager
	:public FvSingleton<TimeManager>
	,cocos2d::Ref
{
public:
	TimeManager();
	~TimeManager();
public:
	void init();
	void update(float fTime);
	void addCBList(Time_CBInfoBase* pCB);
	bool isUseRef(TimeNode* pNode);
	void deleteCBInfo(Time_CBInfoBase* pCB);
	void removeByID(int iID);
	void removeByPoint(TimeNode* pNode);
	void removeByFun(Time_Schedule pFun,TimeNode* pRef);
	void removeByFunInt(Time_ScheduleInt pFun,TimeNode* pRef);
	void removeByPointAndTag(TimeNode* pNode,int iTag);
	void removeByTime_CBInfo(Time_CBInfoBase* pInfo);
	float getPastTime();
	int getDay(bool bIncludeYear = true);
	void getMonDay(int iDest,int&iYear,int& iMon,int& iDay);
public:
	template<class T>
	Time_CBInfoBase* addCerterTimeCB1(std::function<bool(T)> fun,TimeNode* pPoint,float fTime,T pUserInfo,float fStillTime = 0)
	{
		m_iID++;
		Time_CBInfo2<T>* pInfo = new Time_CBInfo2<T>;
		pInfo->pPoint = pPoint;
		pInfo->iIdex = m_iID;
		pInfo->fun = fun;
		pInfo->_Value = pUserInfo;
		pInfo->fDestTime = fTime+m_fPastTime;
		pInfo->fPastTime = fTime;
		pInfo->fTotalTime = fStillTime;
		if (pInfo->fTotalTime >= 0)
		{
			pInfo->fTotalTime += m_fPastTime;
		}
		addCBList(pInfo);
		return pInfo;
	}
	template<class T1,class T2>
	Time_CBInfoBase* addCerterTimeCB2(std::function<bool(T1,T2)> fun,TimeNode* pPoint,float fTime,T1 pValue1,T2 pValue2,float fStillTime = 0)
	{
		m_iID++;
		Time_CBInfo3<T1,T2>* pInfo = new Time_CBInfo3<T1,T2>;
		pInfo->pPoint = pPoint;
		pInfo->iIdex = m_iID;
		pInfo->fun = fun;
		pInfo->_Value1 = pValue1;
		pInfo->_Value2 = pValue2;
		pInfo->fDestTime = fTime+m_fPastTime;
		pInfo->fPastTime = fTime;
		pInfo->fTotalTime = fStillTime;
		if (pInfo->fTotalTime >= 0)
		{
			pInfo->fTotalTime += m_fPastTime;
		}
		addCBList(pInfo);
		return pInfo;
	}
	template<class T1,class T2,class T3>
	Time_CBInfoBase* addCerterTimeCB3(std::function<bool(T1,T2,T3)> fun,TimeNode* pPoint,float fTime,T1 pValue1,T2 pValue2,T3 pValue3,float fStillTime = 0)
	{
		m_iID++;
		Time_CBInfo4<T1,T2,T3>* pInfo = new Time_CBInfo4<T1,T2,T3>;
		pInfo->pPoint = pPoint;
		pInfo->iIdex = m_iID;
		pInfo->fun = fun;
		pInfo->_Value1 = pValue1;
		pInfo->_Value2 = pValue2;
		pInfo->_Value3 = pValue3;
		pInfo->fDestTime = fTime+m_fPastTime;
		pInfo->fPastTime = fTime;
		pInfo->fTotalTime = fStillTime;
		if (pInfo->fTotalTime >= 0)
		{
			pInfo->fTotalTime += m_fPastTime;
		}
		addCBList(pInfo);
		return pInfo;
	}

	Time_CBInfoBase* addCerterTimeCB0(std::function<bool()> fun,TimeNode* pPoint,float fTime,float fStillTime = 0)
	{
		m_iID++;
		Time_CBInfo1* pInfo = new Time_CBInfo1;
		pInfo->pPoint = pPoint;
		pInfo->iIdex = m_iID;
		pInfo->fun = fun;
		pInfo->fDestTime = fTime+m_fPastTime;
		pInfo->fPastTime = fTime;
		pInfo->fTotalTime = fStillTime;
		if (pInfo->fTotalTime >= 0)
		{
			pInfo->fTotalTime += m_fPastTime;
		}
		addCBList(pInfo);
		return pInfo;
	}

	Time_CBInfo* addCerterTimeCB(Time_Schedule fun,TimeNode* pObject,float fTime,bool bStill = false)
	{
		m_iID++;
		Time_CBInfo* pInfo = new Time_CBInfo;
		pInfo->iIdex = m_iID;
		pInfo->pPoint = pObject;
		pInfo->fun = fun;
		pInfo->fDestTime = fTime+m_fPastTime;
		pInfo->fPastTime = fTime;
		pInfo->fTotalTime = -1;
		pInfo->bStill = bStill;
		addCBList(pInfo);
		return pInfo;
	}

	Time_CBInfoInt* addCerterTimeInt(Time_ScheduleInt fun,TimeNode* pObject,float fTime,int iValue,bool bStill = false)
	{
		m_iID++;
		Time_CBInfoInt* pInfo = new Time_CBInfoInt;
		pInfo->iIdex = m_iID;
		pInfo->pPoint = pObject;
		pInfo->iValue = iValue;
		pInfo->fun = fun;
		pInfo->fDestTime = fTime+m_fPastTime;
		pInfo->fPastTime = fTime;
		pInfo->fTotalTime = -1;
		pInfo->bStill = bStill;
		addCBList(pInfo);
		return pInfo;
	}
	
private:
	int				m_iID;
	double			m_fPastTime;
	Time_CBInfoList	m_kCBList;
	Time_CBInfoList m_kActCallList;
};

