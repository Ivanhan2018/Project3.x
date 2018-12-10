#include "CCallMission.h"
#include "utility.h"

CCallMission::CCallMission(std::string kName,const char* url, int port)
	:CSocketMission(url,port)
	,m_kClassName(kName)
{
}

CCallMission::~CCallMission()
{
	clearCall();
}

void CCallMission::clearCall()
{
	for (int i = 0;i<(int)m_kLinkCallFun.size();i++)
	{
		delete m_kLinkCallFun[i];
	}
	m_kLinkCallFun.clear();
}
bool CCallMission::start()
{
	if (isAlive())
	{
		return true;
	}
	return CSocketMission::start();
}
void CCallMission::stop()
{
	if (m_kLinkCallFun.size())
	{
		onEventTCPSocketLink();
	}
	else
	{
		CSocketMission::stop();
	}
}
void CCallMission::onEventTCPSocketLink()
{
	utility::log(utility::toString(m_kClassName,":onEventTCPSocketLink").c_str());

	if (m_kLinkCallFun.size() == 0)
	{
		return;
	}
	CallBase* pCall = (*m_kLinkCallFun.begin());
	pCall->CallFun();
	m_kLinkCallFun.erase(m_kLinkCallFun.begin());
	delete pCall;
}
void CCallMission::onEventTCPSocketShut()
{
	utility::log(utility::toString(m_kClassName,":onEventTCPSocketShut").c_str());

}
void CCallMission::onEventTCPSocketError(int errorCode)
{
	utility::log(utility::toString(m_kClassName,":onEventTCPSocketShut").c_str());
}
bool CCallMission::onEventTCPSocketRead(int main, int sub, void* data, int dataSize)
{
	utility::log(utility::toString(m_kClassName,":onEventTCPSocketRead").c_str());

	for (int i = 0;i<(int)m_kNetCallFun.size();i++)
	{
		NetCall& kNetInfo = m_kNetCallFun[i];
		if (kNetInfo.iSub == sub)
		{
			kNetInfo.pFun(data,dataSize);
			return true;
		}
	}
	CCASSERT(false,"");
	return false;
}