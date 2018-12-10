#pragma once
#include <vector>
#include <functional>

#include "CSocketMission.h"


struct NetCall
{
	int iSub;
	std::function<void(void*, int)> pFun;
};

struct CallBase
{
	virtual void CallFun() = 0;
};

template <class T>
struct CallStruct
	:public CallBase
{
	virtual void CallFun()
	{
		pFun(kInfo);
	}
	T kInfo;
	std::function<void(T)> pFun;
};

struct CallVoid
	:public CallBase
{
	virtual void CallFun()
	{
		pFun();
	}
	std::function<void()> pFun;
};

class CCallMission
	: public CSocketMission
{
public:
	CCallMission(std::string kName,const char* url=0, int port=0 );
	virtual ~CCallMission();
public:
	void addLinkCallFun(std::function<void()> pFun)
	{
		CallVoid *pCallInfo = new CallVoid;
		pCallInfo->pFun = pFun;
		m_kLinkCallFun.push_back(pCallInfo);
	}

	template <class T>
	void addLinkCallStruct(std::function<void(T)> pFun,T kInfo)
	{
		CallStruct<T> *pCallInfo = new CallStruct<T>;
		pCallInfo->kInfo = kInfo;
		pCallInfo->pFun = pFun;
		m_kLinkCallFun.push_back(pCallInfo);
	}

	void addNetCall(std::function<void(void*, int)> pFun,int iSub)
	{
		NetCall kCallInfo;
		kCallInfo.pFun = pFun;
		kCallInfo.iSub = iSub;
		m_kNetCallFun.push_back(kCallInfo);
	}

	void clearCall();

	virtual bool start();
	virtual void stop();
public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);
private:
	std::string				m_kClassName;
	std::vector<CallBase*>	m_kLinkCallFun;
	std::vector<NetCall>	m_kNetCallFun;
};
