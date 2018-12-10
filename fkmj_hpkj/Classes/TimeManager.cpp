#include "TimeManager.h"
#include "NetLog.h"
#include "TimeNode.h"

FV_SINGLETON_STORAGE(TimeManager);

TimeManager::TimeManager()
	:m_fPastTime(0)
	,m_iID(0)
{
	init();
}
TimeManager::~TimeManager(void)
{
	cocos2d::Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(TimeManager::update),this);
}

void TimeManager::init()
{
	cocos2d::Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(TimeManager::update),this,0.0f,false);
}
bool TimeManager::isUseRef(TimeNode* pNode)
{
	Time_CBInfoList::iterator itor = m_kCBList.begin();
	while (itor != m_kCBList.end())
	{
		Time_CBInfoBase* pCB = *itor;
		if (pCB->pPoint == pNode)
		{
			return true;
		}
		else
		{
			itor++;
		}
	}
	return false;
}
void TimeManager::deleteCBInfo(Time_CBInfoBase* pCB)
{
	NetLog::Log(utility::toString("del Time CB ",pCB->iIdex));
	Time_CBInfoList::iterator itor = m_kActCallList.begin();
	while (itor != m_kActCallList.end())
	{
		if (*itor == pCB)
		{
			*itor = NULL;
			break;
		}
		itor++;
	}
	delete pCB;
}
void TimeManager::removeByID(int iID)
{
	Time_CBInfoList::iterator itor = m_kCBList.begin();
	while (itor != m_kCBList.end())
	{
		Time_CBInfoBase* pCB = *itor;
		if (pCB->iIdex == iID)
		{
			itor = m_kCBList.erase(itor);
			deleteCBInfo(pCB);
		}
		else
		{
			itor++;
		}
	}
}
void TimeManager::removeByPoint(TimeNode* pNode)
{
	Time_CBInfoList::iterator itor = m_kCBList.begin();
	while (itor != m_kCBList.end())
	{
		Time_CBInfoBase* pCB = *itor;
		if (pCB->pPoint == pNode)
		{
			itor = m_kCBList.erase(itor);
			deleteCBInfo(pCB);
		}
		else
		{
			itor++;
		}
	}
}
void TimeManager::removeByFun(Time_Schedule pFun,TimeNode* pRef)
{
	Time_CBInfoList::iterator itor = m_kCBList.begin();
	while (itor != m_kCBList.end())
	{
		Time_CBInfo* pInfo = dynamic_cast<Time_CBInfo*>(*itor);
		if (!pInfo)
		{
			itor++;
			continue;
		}
		if (pInfo->fun == pFun && pInfo->pPoint == pRef)
		{
			itor = m_kCBList.erase(itor);
			deleteCBInfo(pInfo);
		}
		else
		{
			itor++;
		}
	}
}
void TimeManager::removeByFunInt(Time_ScheduleInt pFun,TimeNode* pRef)
{
	Time_CBInfoList::iterator itor = m_kCBList.begin();
	while (itor != m_kCBList.end())
	{
		Time_CBInfoInt* pInfo = dynamic_cast<Time_CBInfoInt*>(*itor);
		if (!pInfo)
		{
			itor++;
			continue;
		}
		if (pInfo->fun == pFun && pInfo->pPoint == pRef)
		{
			itor = m_kCBList.erase(itor);
			deleteCBInfo(pInfo);
		}
		else
		{
			itor++;
		}
	}
}
void TimeManager::removeByPointAndTag(TimeNode* pNode,int iTag)
{
	CCAssert(iTag != 0,"");
	Time_CBInfoList::iterator itor = m_kCBList.begin();
	while (itor != m_kCBList.end())
	{
		Time_CBInfoBase* pCB = *itor;
		if (pCB->pPoint == pNode && pCB->iTag == iTag)
		{
			itor = m_kCBList.erase(itor);
			deleteCBInfo(pCB);
		}
		else
		{
			itor++;
		}
	}
}
void TimeManager::removeByTime_CBInfo(Time_CBInfoBase* pInfo)
{
	if (!pInfo)
	{
		return;
	}
	Time_CBInfoList::iterator itor = m_kCBList.begin();
	while (itor != m_kCBList.end())
	{
		Time_CBInfoBase* pCB = *itor;
		if (pCB == pInfo)
		{
			itor = m_kCBList.erase(itor);
			deleteCBInfo(pCB);
		}
		else
		{
			itor++;
		}
	}
}
void TimeManager::addCBList(Time_CBInfoBase* pCB)
{
	pCB->pPoint->useTimeNode();
	m_kCBList.push_back(pCB);
}
void TimeManager::update(float fTime)
{
	m_fPastTime+=fTime;
	Time_CBInfoList kDelCallList;
	Time_CBInfoList::iterator itor = m_kCBList.begin();
	while (itor != m_kCBList.end())
	{
		Time_CBInfoBase* pCB = *itor;
		if (pCB->fDestTime <= m_fPastTime)
		{
			bool bDel = false;
			if ((pCB->fTotalTime >= 0 && pCB->fTotalTime <= m_fPastTime))
			{
				bDel = true;
			}
			if (bDel)
			{
				kDelCallList.push_back(pCB);
				itor = m_kCBList.erase(itor);
			}
			else
			{
				pCB->fDestTime = pCB->fPastTime+m_fPastTime;
				itor++;
			}
			m_kActCallList.push_back(pCB);
		}
		else
		{
			itor++;
		}
	}
	for (int i = 0;i<(int)m_kActCallList.size();i++)
	{
		Time_CBInfoBase*& pCB = m_kActCallList[i];//这里使用引用 以避免执行下面callFun删除指针为空情况
		if (!pCB)
		{
			continue;
		}
		if (pCB->callFun())
		{
			continue;
		}
		if (pCB)
		{
			removeByTime_CBInfo(pCB);
		}
	}
	m_kActCallList.clear();
	for (int i = 0;i<(int)kDelCallList.size();i++)
	{
		delete kDelCallList[i];
	}
	kDelCallList.clear();
}
float TimeManager::getPastTime()
{
	return m_fPastTime;
}
int TimeManager::getDay(bool bIncludeYear)
{
	time_t tt = time(NULL);
	tm* t= localtime(&tt);
	if (bIncludeYear)
	{
		return t->tm_year*60*12+t->tm_mon*60+t->tm_mday;
	}
	return t->tm_mon*60+t->tm_mday;
}
void TimeManager::getMonDay(int iDest,int&iYear,int& iMon,int& iDay)
{
	iYear = iYear/(60*12);
	iMon = (iDest/60)%12;
	iDay = iDest%60;
}