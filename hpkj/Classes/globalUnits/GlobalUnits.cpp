#include "GlobalUnits.h"
#include <algorithm>
#include "ConfigMgr.h"
//全局信息
CGlobalUnits g_GlobalUnits;

CServerListMgr::CServerListMgr()
{

}

CServerListMgr::~CServerListMgr()
{
	m_GameServerList.clear();
}

//自定义排序函数  
int SortByScore(const tagGameServer *v1, const tagGameServer *v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致  
{  
	int bRet = 0;
	tagGameServer server1 = *(tagGameServer *)v1;
	tagGameServer server2 = *(tagGameServer *)v2;
	if((server1.lMinServerScore - server2.lMinServerScore)>0)
	{
		bRet = -1;
	}else if((server1.lMinServerScore - server2.lMinServerScore)<0)
	{
		bRet = 1;
	}
	return bRet;//降序排列  
}  

void CServerListMgr::sortItems()
{
	size_t sortSize = m_GameServerList.size();
	for (unsigned int i = 0; i < sortSize; i++)
	{
		for (unsigned int j = i+1; j < sortSize; j++)
		{
			if(m_GameServerList.at(i).lMinServerScore<m_GameServerList.at(j).lMinServerScore)
			{
				tagGameServer temp = m_GameServerList.at(i);
				m_GameServerList.at(i) = m_GameServerList.at(j);
				m_GameServerList.at(j) = temp;
			}
		}
	}
	for (unsigned int i = 0; i < sortSize; i++)
	{
		pConfigMgr->m_Config.nServerEnter[i] = m_GameServerList.at(i).lMinServerScore;
		CCLOG("============================");
		CCLOG("lMinServerScore = %d", m_GameServerList.at(i).lMinServerScore);
		CCLOG("============================");
	}
}

void CServerListMgr::insertItem(tagGameServer item[],unsigned int nCount)
{

	for (unsigned int i = 0; i < nCount; i++)
	{
		m_GameServerList.push_back(item[i]);
		CCLOG("============================");
		CCLOG("wServerPort = %d", item[i].wServerPort);
		CCLOG("============================");
	}
}

void CServerListMgr::clearItem()
{
	m_GameServerList.clear();
}

void CServerListMgr::removeItem(tagGameServer* pItem)
{

}

//获取全服总人数
DWORD CServerListMgr::getServerOnLineCount()
{
	DWORD dwOnlineCount = 0;
	GameServerList::iterator iter = m_GameServerList.begin();
	for (; iter != m_GameServerList.end(); iter++)
	{
		dwOnlineCount += (*iter).dwOnLineCount;
	}
	return dwOnlineCount;
}

tagGameServer CServerListMgr::getCurrentServer()
{
	tagGameServer tag;
	memset(&tag, 0, sizeof(tag));
	if (m_GameServerList.size() <= 0)
	{
		return tag;
	}
	return m_GameServerList[0];
}

tagGameServer CServerListMgr::getGameServer(unsigned int nKind)
{

	for (GameServerList::iterator iter = m_GameServerList.begin() ; iter != m_GameServerList.end(); iter++)
	{
		if (nKind == iter->wKindID)
		{
			return *iter;
		}
	}
	tagGameServer tag;
	memset(&tag, 0, sizeof(tag));
	return tag;
}

tagGameServer CServerListMgr::getGameServerByIndex(int index)
{
	tagGameServer tag;
	memset(&tag, 0, sizeof(tag));


	for (int i = 0 ; i < m_GameServerList.size() ; i++)
	{
		if (i == index)
		{
			tag = m_GameServerList.at(i);
		}
	}
	return tag;
}

CGlobalUnits::CGlobalUnits()
{
	m_pGloblaUserData = NULL;

	m_nGabalUserLevel = 0;
	m_nOnlineCount    = 0;
	m_nHighVer        = 1;
	m_nLowVer         = 0;

	m_bDownVisible    = false;
	memset(m_szText, 0, 128);

}

CGlobalUnits::~CGlobalUnits()
{
	if (m_pGloblaUserData)
	{
		delete  m_pGloblaUserData;
	}
}

void CGlobalUnits::setGolbalUserData(tagGlobalUserData* tag)
{
	m_pGloblaUserData = tag;
	m_GloblaUserData=*tag;
}

void CUserDateMgr::insertItem(tagUserData* pItem)
{
	if (!pItem)
	{
		assert(false);
		return;
	}
	m_GameUsrDateList.push_back(pItem);
}

void CUserDateMgr::removeItem(tagUserData* pItem)
{

}


