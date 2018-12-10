#include "ClientUserManager.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CClientUserItem::CClientUserItem()
{
	//设置变量
	m_cbCompanion=CP_NORMAL;
	zeromemory(&m_UserInfo,sizeof(m_UserInfo));
	zeromemory(m_szUserNote,sizeof(m_szUserNote));
	zeromemory(&m_CustomFaceInfo,sizeof(m_CustomFaceInfo));
	zeromemory(&m_PropertyPackage,sizeof(m_PropertyPackage));
	m_UserInfo.wTableID=INVALID_TABLE;
	m_UserInfo.wChairID=INVALID_CHAIR;

	return;
}

//析构函数
CClientUserItem::~CClientUserItem()
{
}

//重要等级
long CClientUserItem::GetImportOrder()
{
	//构造等级
	int nOrder=0;
	if (m_cbCompanion==CP_FRIEND) nOrder+=1000;
	if (m_UserInfo.cbMemberOrder!=0) nOrder+=m_UserInfo.cbMemberOrder*100L;
	if (m_UserInfo.cbMasterOrder!=0) nOrder+=m_UserInfo.cbMasterOrder*10000L;

	return nOrder;
}

//用户胜率
float CClientUserItem::GetUserWinRate()
{
	long lPlayCount=GetUserPlayCount();
	if (lPlayCount!=0L) return (float)(m_UserInfo.lWinCount*100.0f/(float)lPlayCount);

	return 0.0f;
}

//用户输率
float CClientUserItem::GetUserLostRate()
{
	long lPlayCount=GetUserPlayCount();
	if (lPlayCount!=0L) return (float)(m_UserInfo.lLostCount*100.0f/(float)lPlayCount);

	return 0.0f;
}

//用户和率
float CClientUserItem::GetUserDrawRate()
{
	long lPlayCount=GetUserPlayCount();
	if (lPlayCount!=0L) return (float)(m_UserInfo.lDrawCount*100.0f/(float)lPlayCount);

	return 0.0f;
}

//用户逃率
float CClientUserItem::GetUserFleeRate()
{
	long lPlayCount=GetUserPlayCount();
	if (lPlayCount!=0L) return (float)(m_UserInfo.lFleeCount*100.0f/(float)lPlayCount);

	return 0.0f;
}

//设置备注
void CClientUserItem::SetUserNoteInfo(const char* pszUserNote)
{
	//效验参数
	ASSERT(pszUserNote!=0);
	if (pszUserNote==0) return;

	//设置备注
	strncpy(m_szUserNote,pszUserNote,countarray(m_szUserNote));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPlazaUserManager::CPlazaUserManager()
{
	//组件接口
	m_pIUserManagerSink=0;
	//m_pIUserInformation=0;

	return;
}

//析构函数
CPlazaUserManager::~CPlazaUserManager()
{
	for (int i=0,l=(int)m_UserItemActive.size();i<l;++i)
	{
		delete m_UserItemActive[i];
	}
	for (int i=0,l=(int)m_PoolItem.size();i<l;++i)
	{
		delete m_PoolItem[i];
	}
	m_UserItemActive.clear();
	m_PoolItem.clear();
	return;
}


////设置接口
//bool CPlazaUserManager::SetUserInformation(IUserInformation * pIUserInformation)
//{
//	//设置接口
//	m_pIUserInformation=pIUserInformation;
//	return true;
//}

//设置接口
bool CPlazaUserManager::SetUserManagerSink(IUserManagerSink * pIUserManagerSink)
{
	m_pIUserManagerSink=pIUserManagerSink;
	return true;
}

//增加用户
IClientUserItem * CPlazaUserManager::ActiveUserItem(const tagUserInfo & UserInfo, const tagCustomFaceInfo & CustomFaceInfo)
{
	//变量定义
	CClientUserItem * pClientUserItem=(CClientUserItem*)SearchUserByUserID(UserInfo.dwUserID);
	if (!pClientUserItem)
	{
		pClientUserItem=new CClientUserItem;
		if (pClientUserItem==0) return 0;

		//插入用户
		ASSERT(pClientUserItem!=0);
		m_UserItemActive.push_back(pClientUserItem);
	}


	pClientUserItem->m_szUserNote[0]=0;
	pClientUserItem->m_cbCompanion=CP_NORMAL; 

	//设置数据
	memcpy(&pClientUserItem->m_UserInfo,&UserInfo,sizeof(UserInfo));
	memcpy(&pClientUserItem->m_CustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

 	//更新通知
 	ASSERT(m_pIUserManagerSink!=0);
 	if (m_pIUserManagerSink!=0) m_pIUserManagerSink->OnUserItemAcitve(pClientUserItem);


	return pClientUserItem;
}


//重置用户
bool CPlazaUserManager::ResetUserItem()
{
	for (int i = 0;i<(int)m_UserItemActive.size();i++)
	{
		if (m_pIUserManagerSink)
			m_pIUserManagerSink->OnUserItemDelete(m_UserItemActive[i]);
		
	}
	for (int i = 0;i<(int)m_UserItemActive.size();i++)
	{
		addPool(m_UserItemActive[i]);

	}
	//设置变量
	m_UserItemActive.clear();
	
	return true;
}
void CPlazaUserManager::addPool(CClientUserItem * pIClientUserItem)
{
	for (int i = 0;i<(int)m_PoolItem.size();i++)
	{
		if (m_PoolItem[i] == pIClientUserItem)
		{
			return;
		}
	}
	m_PoolItem.push_back(pIClientUserItem);
}
CClientUserItem * CPlazaUserManager::getPool()
{
	CClientUserItem * pClient = NULL;
	if (m_PoolItem.size())
	{
		pClient = m_PoolItem.back();
		m_PoolItem.pop_back();
		return pClient;
	}
	pClient = new CClientUserItem;
	m_UserItemActive.push_back(pClient);
	return pClient;
}
//删除用户
bool CPlazaUserManager::DeleteUserItem(IClientUserItem * pIClientUserItem)
{
	PLAZZ_PRINTF("CPlazaUserManager::DeleteUserItem\n");
	//查找用户
	CClientUserItem * pUserItemActive=0;
	for (int i=0,l=(int)m_UserItemActive.size();i<l;i++)
	{
		pUserItemActive=m_UserItemActive[i];
		if (pIClientUserItem==pUserItemActive)
		{
			//删除用户
			m_UserItemActive.erase(m_UserItemActive.begin()+i);
			addPool(pUserItemActive);

			//删除通知
			ASSERT(m_pIUserManagerSink!=0);
			if (m_pIUserManagerSink)
				m_pIUserManagerSink->OnUserItemDelete(pUserItemActive);

			//设置数据
			pUserItemActive->m_cbCompanion=CP_NORMAL;
			zeromemory(&pUserItemActive->m_UserInfo,sizeof(tagUserInfo));
			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//更新积分
bool CPlazaUserManager::UpdateUserItemScore(IClientUserItem * pIClientUserItem, const tagUserScore * pUserScore)
{
	//效验参数
	ASSERT(pUserScore!=0);
	ASSERT(pIClientUserItem!=0);

	//获取用户
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();

	//以往数据
	tagUserScore UserScore;
	UserScore.lScore=pUserInfo->lScore;
	UserScore.lInsure=pUserInfo->lInsureScore;
	UserScore.dwWinCount=pUserInfo->lWinCount;
	UserScore.dwLostCount=pUserInfo->lLostCount;
	UserScore.dwDrawCount=pUserInfo->lDrawCount;
	UserScore.dwFleeCount=pUserInfo->lFleeCount;
	UserScore.dwExperience=pUserInfo->lExperience;

	//设置数据
	pUserInfo->lScore=pUserScore->lScore;
	pUserInfo->lInsureScore=pUserScore->lInsure;
	pUserInfo->lWinCount=pUserScore->dwWinCount;
	pUserInfo->lLostCount=pUserScore->dwLostCount;
	pUserInfo->lDrawCount=pUserScore->dwDrawCount;
	pUserInfo->lFleeCount=pUserScore->dwFleeCount;
	pUserInfo->lExperience=pUserScore->dwExperience;

	//通知更新
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink)
		m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem,UserScore);

	return true;
}

//更新状态
bool CPlazaUserManager::UpdateUserItemStatus(IClientUserItem * pIClientUserItem, const tagUserStatus * pUserStatus)
{
	PLAZZ_PRINTF("CPlazaUserManager::UpdateUserItemStatus\n");
	//效验参数
	ASSERT(pUserStatus!=0);
	ASSERT(pIClientUserItem!=0);

	//获取用户
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();

	//以往数据
	tagUserStatus tUserStatus;
	tUserStatus.wTableID = pUserInfo->wTableID;
	tUserStatus.wChairID = pUserInfo->wChairID;
	tUserStatus.cbUserStatus = pUserInfo->cbUserStatus;

	//设置数据
	pUserInfo->wTableID=pUserStatus->wTableID;
	pUserInfo->wChairID=pUserStatus->wChairID;
	pUserInfo->cbUserStatus=pUserStatus->cbUserStatus;

	//通知更新
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink)
		m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem, tUserStatus);

	return true;
}

//更新属性
bool CPlazaUserManager::UpdateUserItemAttrib(IClientUserItem * pIClientUserItem, const tagUserAttrib * pUserAttrib)
{
	//效验参数
	ASSERT(pUserAttrib!=0);
	ASSERT(pIClientUserItem!=0);

	//以往数据
	tagUserAttrib UserAttrib;
	UserAttrib.cbCompanion=pIClientUserItem->GetUserCompanion();

	//设置数据
	pIClientUserItem->SetUserCompanion(pUserAttrib->cbCompanion);

	//通知更新
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink)
		m_pIUserManagerSink->OnUserItemUpdate(pIClientUserItem,UserAttrib);

	return true;
}

//更新头像
bool CPlazaUserManager::UpdateUserCustomFace(IClientUserItem * pIClientUserItem, dword dwCustomID, const tagCustomFaceInfo & CustomFaceInfo)
{
	//获取用户
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//设置变量
	pUserInfo->dwCustomID=dwCustomID;
	memcpy(pCustomFaceInfo,&CustomFaceInfo,sizeof(CustomFaceInfo));

	//通知更新
	ASSERT(m_pIUserManagerSink!=0);
	if (m_pIUserManagerSink)
		m_pIUserManagerSink->OnUserFaceUpdate(pIClientUserItem);

	return true;
}

//枚举用户
IClientUserItem * CPlazaUserManager::EnumUserItem(word wEnumIndex)
{
	if (wEnumIndex>=m_UserItemActive.size()) return 0;
	return m_UserItemActive[wEnumIndex];
}

//查找用户
IClientUserItem * CPlazaUserManager::SearchUserByUserID(dword dwUserID)
{
	//用户搜索
	for (int i=0,l=(int)m_UserItemActive.size();i<l;i++)
	{
		CClientUserItem * pClientUserItem=m_UserItemActive[i];
		if (pClientUserItem->m_UserInfo.dwUserID==dwUserID) return pClientUserItem;
	}

	return 0;
}

//查找用户
IClientUserItem * CPlazaUserManager::SearchUserByGameID(dword dwGameID)
{
	//用户搜索
	for (int i=0,l=(int)m_UserItemActive.size();i<l;i++)
	{
		CClientUserItem * pClientUserItem=m_UserItemActive[i];
		if (pClientUserItem->m_UserInfo.dwGameID==dwGameID) return pClientUserItem;
	}

	return 0;
}

//查找用户
IClientUserItem * CPlazaUserManager::SearchUserByNickName(const char* pszNickName)
{
	//用户搜索
	for (int i=0,l=(int)m_UserItemActive.size();i<l;i++)
	{
		CClientUserItem * pClientUserItem=m_UserItemActive[i];
		const char* pszTempNickName=pClientUserItem->GetNickName();
		if (strcmp(pszNickName,pszTempNickName)==0) return pClientUserItem;
	}

	return 0;
}
