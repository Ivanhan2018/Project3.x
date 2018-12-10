#include "ServerListData.h"
#include "Macro.h"
//构造函数
CGameListItem::CGameListItem(enItemGenre ItemGenre)
{
	//属性数据
	m_ItemGenre=ItemGenre;
	m_pParentListItem=0;

	return;
}

//析构函数
CGameListItem::~CGameListItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameTypeItem::CGameTypeItem() : CGameListItem(ItemGenre_Type)
{
	//属性数据
	zeromemory(&m_GameType,sizeof(m_GameType));
}

//析构函数
CGameTypeItem::~CGameTypeItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameKindItem::CGameKindItem() : CGameListItem(ItemGenre_Kind)
{
	//更新变量
	m_dwUpdateTime=0;
	m_bUpdateItem=false;

	//扩展数据
	m_dwProcessVersion=0L;

	//属性数据
	zeromemory(&m_GameKind,sizeof(m_GameKind));

	return;
}

//析构函数
CGameKindItem::~CGameKindItem()
{
}


//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameServerItem::CGameServerItem() : CGameListItem(ItemGenre_Server)
	,m_bSignuped(false)
{
	//辅助变量
	m_pGameKindItem=0;

	//扩展数据
	m_ServerStatus=ServerStatus_Normal;

	//属性数据
	zeromemory(&m_GameServer,sizeof(m_GameServer));
	zeromemory(&m_GameMatch,sizeof(m_GameMatch));
}

//析构函数
CGameServerItem::~CGameServerItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameInsideItem::CGameInsideItem() : CGameListItem(ItemGenre_Inside)
{
}

//析构函数
CGameInsideItem::~CGameInsideItem()
{
}

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CServerListData * __gServerListData=0;				//列表数据


CServerListData* CServerListData::shared()
{
	if (__gServerListData == 0)
		__gServerListData = new CServerListData();
	return __gServerListData;
}

void CServerListData::purge()
{
	if (__gServerListData)
		delete __gServerListData;
	__gServerListData = 0;
}

//////////////////////////////////////////////////////////////////////////////////
//构造函数
CServerListData::CServerListData()
{
	//接口变量
	m_pIServerListDataSink=0;

	//设置质数
	//m_GameTypeItemMap.resize(PRIME_TYPE);
	//m_GameKindItemMap.resize(PRIME_KIND);
	//m_GameNodeItemMap.resize(PRIME_NODE);
	//m_GamePageItemMap.resize(PRIME_PAGE);
	//m_GameServerItemMap.resize(PRIME_SERVER);

	//设置对象
	//ASSERT(m_pServerListData==0);
	m_dwAllOnLineCount=0;

	return;
}

//析构函数
CServerListData::~CServerListData()
{
	//变量定义
	//uint16 wKey=0;

	//删除种类
	CGameTypeItemMap::iterator itTypeItem = m_GameTypeItemMap.begin();
	for (; itTypeItem != m_GameTypeItemMap.end(); ++itTypeItem)
	{
		SafeDelete(itTypeItem->second);
	}
	m_GameTypeItemMap.clear();

	//删除类型
	CGameKindItemMap::iterator itKindItem = m_GameKindItemMap.begin();
	for (; itKindItem != m_GameKindItemMap.end(); ++itKindItem)
	{
		SafeDelete(itKindItem->second);
	}
	m_GameKindItemMap.clear();

	//删除房间
	CGameServerItemMap::iterator itServerItem = m_GameServerItemMap.begin();
	for (; itServerItem != m_GameServerItemMap.end(); ++itServerItem)
	{
		SafeDelete(itServerItem->second);
	}
	m_GameServerItemMap.clear();

	//删除定制
	CGameListItemArray::iterator itListItem = m_GameListItemWait.begin();
	for (; itListItem != m_GameListItemWait.end(); ++itListItem)
	{
		SafeDelete(*itListItem);
	}
	m_GameListItemWait.clear();

	return;
}
CGameServerItem*  CServerListData::getGameServerByKind(word wKindID,word wServerType)
{
	CGameServerItem* pMinPlayerCoutServer = NULL;
	CServerListData* pListData = CServerListData::shared();
	CGameServerItemMap::iterator itor = pListData->GetServerItemMapBegin();
	CGameServerItem* pListItem = pListData->EmunGameServerItem(itor);
	while (pListItem != NULL)
	{
		CGameServerItem* pActListItem = pListItem;
		pListItem = pListData->EmunGameServerItem(itor);
		if (pActListItem->m_GameServer.wKindID != wKindID)
		{
			continue;
		}
		//if (wServerType != INVALID_WORD && pActListItem->m_GameServer.wServerType != wServerType)
		//{
		//	continue;
		//}
		if (!pMinPlayerCoutServer)
		{
			pMinPlayerCoutServer = pActListItem;
			continue;
		}
		if (pMinPlayerCoutServer->m_GameServer.dwOnLineCount > 
			pActListItem->m_GameServer.dwOnLineCount)
		{
			pMinPlayerCoutServer = pActListItem;
		}
	}
	return pMinPlayerCoutServer;
}

//设置接口
void CServerListData::SetServerListDataSink(IServerListDataSink * pIServerListDataSink)
{
	//设置变量
	m_pIServerListDataSink=pIServerListDataSink;

	return;
}

//完成通知
void CServerListData::OnEventListFinish()
{
	//事件通知
	//ASSERT(m_pIServerListDataSink!=0);
	if (m_pIServerListDataSink!=0) m_pIServerListDataSink->OnGameItemFinish();

	return;
}

//完成通知
void CServerListData::OnEventKindFinish(uint16 wKindID)
{
	//事件通知
	//ASSERT(m_pIServerListDataSink!=0);
	if (m_pIServerListDataSink!=0) m_pIServerListDataSink->OnGameKindFinish(wKindID);

	return;
}

//下载通知
void CServerListData::OnEventDownLoadFinish(uint16 wKindID)
{
	CGameKindItemMap::iterator it = m_GameKindItemMap.find(wKindID);

	if (it == m_GameKindItemMap.end())
		return;

	//变量定义
	CGameKindItem * pGameKindItem=it->second;

	//更新信息
	if (pGameKindItem!=0)
	{
		//获取版本
		//tchar szProcessPath[MAX_PATH]={0};
		//GetProcessPathByProcessFullName(pGameKindItem->m_GameKind.szProcessName, szProcessPath, MAX_PATH);
		//CWHService::GetModuleVersion(szProcessPath,pGameKindItem->m_dwProcessVersion);
		
		//更新子项
		//ASSERT(m_pIServerListDataSink!=0);
		if (m_pIServerListDataSink!=0) m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
	}

	return;
}

//设置人数
void CServerListData::SetKindOnLineCount(uint16 wKindID, uint32 dwOnLineCount)
{
	//搜索类型
	CGameKindItem * pGameKindItem=SearchGameKind(wKindID);

	//设置人数
	if (pGameKindItem!=0)
	{
		//设置变量
		///pGameKindItem->m_GameKind.dwOnLineCount=dwOnLineCount;

		//事件通知
		//ASSERT(m_pIServerListDataSink!=0);
		if (m_pIServerListDataSink!=0) m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
	}

	return;
}

//设置人数
void CServerListData::SetServerOnLineCount(uint16 wServerID, uint32 dwOnLineCount)
{
	//搜索房间
	CGameServerItem * pGameServerItem=SearchGameServer(wServerID);

	//设置人数
	if (pGameServerItem!=0)
	{
		//设置变量
		m_dwAllOnLineCount -= pGameServerItem->m_GameServer.dwOnLineCount;
		m_dwAllOnLineCount += dwOnLineCount;

		//设置变量
		pGameServerItem->m_GameServer.dwOnLineCount=dwOnLineCount;

		//事件通知
		//ASSERT(m_pIServerListDataSink!=0);
		if (m_pIServerListDataSink!=0) m_pIServerListDataSink->OnGameItemUpdate(pGameServerItem);

		//查找类型
		CGameKindItem * pGameKindItem = SearchGameKind(pGameServerItem->m_GameServer.wKindID);
		if(pGameKindItem)
		{
			//变量定义
			uint32 dwGameKindOnline=0;
			CGameServerItemMap::iterator it = m_GameServerItemMap.end();
			do
			{
				//获取房间
				CGameServerItem * pGameServerItem2=EmunGameServerItem(it);

				//设置房间
				if((pGameServerItem2 != 0) && (pGameServerItem2->m_GameServer.wKindID == pGameServerItem->m_GameServer.wKindID))
				{
					dwGameKindOnline += pGameServerItem2->m_GameServer.dwOnLineCount;
				}

			} while (it != m_GameServerItemMap.end());

			//设置变量
			///pGameKindItem->m_GameKind.dwOnLineCount = dwGameKindOnline;

			//事件通知
			//ASSERT(m_pIServerListDataSink!=0);
			if (m_pIServerListDataSink!=0) m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
		}
	}

	return;
}

//设置人数
void CServerListData::SetServerOnLineFinish()
{
	//事件通知
	//ASSERT(m_pIServerListDataSink!=0);
	if (m_pIServerListDataSink!=0) m_pIServerListDataSink->OnGameItemUpdateFinish();
}

//插入种类
bool CServerListData::InsertGameType(tagGameType * pGameType)
{
	//效验参数
	//ASSERT(pGameType!=0);
	if (pGameType==0) return false;

	
	//变量定义
	CGameTypeItem * pGameTypeItem=0;
	bool bInsert = false;
	CGameTypeItemMap::iterator it = m_GameTypeItemMap.find(pGameType->wTypeID);
	
	if (it == m_GameTypeItemMap.end())
	{
		pGameTypeItem = new CGameTypeItem;
		bInsert = true;
	}
	else
	{
		pGameTypeItem = it->second;
	}		
	
	if (pGameTypeItem == 0) return false;

	//设置数据
	memcpy(&pGameTypeItem->m_GameType,pGameType,sizeof(tagGameType));


	//插入处理
	if (bInsert==true)
	{
		//设置索引
		m_GameTypeItemMap[pGameType->wTypeID]=pGameTypeItem;

		//界面更新
		if (m_pIServerListDataSink)
			m_pIServerListDataSink->OnGameItemInsert(pGameTypeItem);
	}
	else
	{
		//界面更新
		if (m_pIServerListDataSink)
			m_pIServerListDataSink->OnGameItemUpdate(pGameTypeItem);
	}

	return true;
}

//插入类型
bool CServerListData::InsertGameKind(tagGameKind * pGameKind)
{
	//效验参数
	//ASSERT(pGameKind!=0);
	if (pGameKind==0) return false;

	//变量定义
	CGameKindItem * pGameKindItem=0;
	bool bInsert = false;
	CGameKindItemMap::iterator it = m_GameKindItemMap.find(pGameKind->wKindID);

	if (it == m_GameKindItemMap.end())
	{
		pGameKindItem = new CGameKindItem;
		bInsert = true;
	}
	else
	{
		pGameKindItem = it->second;
	}		

	if (pGameKindItem == 0) return false;


	//设置数据
	memcpy(&pGameKindItem->m_GameKind,pGameKind,sizeof(tagGameKind));

	//插入处理
	if (bInsert==true)
	{
		//设置索引
		m_GameKindItemMap[pGameKind->wKindID]=pGameKindItem;

		//插入子项
		if (m_pIServerListDataSink)
			m_pIServerListDataSink->OnGameItemInsert(pGameKindItem);
	}
	else
	{
		//更新子项
		if (m_pIServerListDataSink)
			m_pIServerListDataSink->OnGameItemUpdate(pGameKindItem);
	}

	return true;
}

//插入房间
bool CServerListData::InsertGameServer(tagGameServer * pGameServer)
{
	//效验参数
	//ASSERT(pGameServer!=0);
	if (pGameServer==0) return false;

	//变量定义
	CGameServerItem * pGameServerItem=0;
	bool bInsert = false;
	CGameServerItemMap::iterator it = m_GameServerItemMap.find(pGameServer->wServerID);

	if (it == m_GameServerItemMap.end())
	{
		pGameServerItem = new CGameServerItem;
		bInsert = true;
	}
	else
	{
		pGameServerItem = it->second;
	}		

	if (pGameServerItem == 0) return false;

	//设置数据
	memcpy(&pGameServerItem->m_GameServer,pGameServer,sizeof(tagGameServer));
	m_dwAllOnLineCount += pGameServer->dwOnLineCount;

	pGameServerItem->m_pGameKindItem=SearchGameKind(pGameServer->wKindID);

	//查找类型
	if(bInsert==true && pGameServerItem->m_pGameKindItem)
	{
		//变量定义
		uint32 dwGameKindOnline=0;
		CGameServerItemMap::iterator it = m_GameServerItemMap.end();
		do
		{
			//获取房间
			CGameServerItem * pGameServerItem2=EmunGameServerItem(it);

			//设置房间
			if((pGameServerItem2 != 0) && (pGameServerItem2->m_GameServer.wKindID == pGameServerItem->m_GameServer.wKindID))
			{
				dwGameKindOnline += pGameServerItem2->m_GameServer.dwOnLineCount;
			}

		} while (it != m_GameServerItemMap.end());

		//设置变量
		///pGameServerItem->m_pGameKindItem->m_GameKind.dwOnLineCount = dwGameKindOnline;

		//事件通知
		//ASSERT(m_pIServerListDataSink!=0);
		if (m_pIServerListDataSink!=0) m_pIServerListDataSink->OnGameItemUpdate(pGameServerItem->m_pGameKindItem);
	}


	//插入处理
	if (bInsert==true)
	{
		//设置索引
		m_GameServerItemMap[pGameServer->wServerID]=pGameServerItem;

		//插入子项
		if (m_pIServerListDataSink)
			m_pIServerListDataSink->OnGameItemInsert(pGameServerItem);
	}
	else 
	{
		//更新子项
		if (m_pIServerListDataSink)
			m_pIServerListDataSink->OnGameItemUpdate(pGameServerItem);
	}

	return true;
}

//删除种类
bool CServerListData::DeleteGameType(uint16 wTypeID)
{
	//查找种类
	CGameTypeItemMap::iterator it = m_GameTypeItemMap.find(wTypeID);
	if (it == m_GameTypeItemMap.end())
	{
		return false;
	}

	CGameTypeItem * pGameTypeItem= it->second;
	
	//删除通知
	//ASSERT(m_pIServerListDataSink!=0);
	if (m_pIServerListDataSink!=0) m_pIServerListDataSink->OnGameItemDelete(pGameTypeItem);

	//重置数据
	SafeDelete(pGameTypeItem);

	//删除数据
	m_GameTypeItemMap.erase(it);

	return true;
}

//删除类型
bool CServerListData::DeleteGameKind(uint16 wKindID)
{
	//查找类型
	CGameKindItemMap::iterator it = m_GameKindItemMap.find(wKindID);
	if (it == m_GameKindItemMap.end())
	{
		return false;
	}

	CGameKindItem * pGameKindItem=it->second;

	//删除通知
	//ASSERT(m_pIServerListDataSink!=0);
	if (m_pIServerListDataSink!=0) m_pIServerListDataSink->OnGameItemDelete(pGameKindItem);

	//删除数据
	SafeDelete(pGameKindItem);

	//删除数据
	m_GameKindItemMap.erase(it);

	return true;
}

//删除房间
bool CServerListData::DeleteGameServer(uint16 wServerID)
{
	//查找房间
	CGameServerItemMap::iterator it = m_GameServerItemMap.find(wServerID);
	if (it == m_GameServerItemMap.end())
	{
		return false;
	}

	CGameServerItem * pGameServerItem=it->second;

	//设置数据
	m_dwAllOnLineCount -= pGameServerItem->m_GameServer.dwOnLineCount;

	//删除通知
	//ASSERT(m_pIServerListDataSink!=0);
	if (m_pIServerListDataSink!=0) m_pIServerListDataSink->OnGameItemDelete(pGameServerItem);

	//删除数据
	SafeDelete(pGameServerItem);

	//删除数据
	m_GameServerItemMap.erase(it);

	return false;
}

//枚举种类
CGameTypeItem * CServerListData::EmunGameTypeItem(CGameTypeItemMap::iterator &it)
{
	//调整参数
	if (it==m_GameTypeItemMap.end()) it=m_GameTypeItemMap.begin();
	if (it==m_GameTypeItemMap.end()) return 0;

	//查找房间
	CGameTypeItem * pGameTypeItem=it->second;
	it++;
	return pGameTypeItem;
}

//枚举类型
CGameKindItem * CServerListData::EmunGameKindItem(CGameKindItemMap::iterator &it)
{
	//调整参数
	if (it==m_GameKindItemMap.end()) it=m_GameKindItemMap.begin();
	if (it==m_GameKindItemMap.end()) return 0;

	//查找房间
	CGameKindItem * pGameKindItem=it->second;
	it++;
	return pGameKindItem;
}

//枚举房间
CGameServerItem * CServerListData::EmunGameServerItem(CGameServerItemMap::iterator &it)
{
	//调整参数
	if (it==m_GameServerItemMap.end()) return 0;

	//查找房间
	CGameServerItem * pGameServerItem=it->second;
	it++;
	return pGameServerItem;
}

//查找种类
CGameTypeItem * CServerListData::SearchGameType(uint16 wTypeID)
{
	CGameTypeItemMap::iterator it = m_GameTypeItemMap.find(wTypeID);
	if (it == m_GameTypeItemMap.end())
		return 0;

	return it->second;
}

//查找类型
CGameKindItem * CServerListData::SearchGameKind(uint16 wKindID)
{
	CGameKindItemMap::iterator it = m_GameKindItemMap.find(wKindID);
	if (it == m_GameKindItemMap.end())
		return 0;

	return it->second;
}

//查找房间
CGameServerItem * CServerListData::SearchGameServer(uint16 wServerID)
{
	CGameServerItemMap::iterator it = m_GameServerItemMap.find(wServerID);
	if (it == m_GameServerItemMap.end())
		return 0;

	return it->second;
}

//获取总在线人数
uint32 CServerListData::GetAllOnLineCount()
{
	//定义变量
	uint32 dwAllOnLineCount=0;
	CGameKindItemMap::iterator it = m_GameKindItemMap.end();

	//遍历
	do
	{
		CGameKindItem *pGameKindItem = EmunGameKindItem(it);
		if(pGameKindItem)
		{
			///dwAllOnLineCount += pGameKindItem->m_GameKind.dwOnLineCount;
		}
	}while(it != m_GameKindItemMap.end());

	return dwAllOnLineCount;
}
//////////////////////////////////////////////////////////////////////////////////
