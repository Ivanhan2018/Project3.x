#include "CServerItem.h"
//////////////////////////////////////////////////////////////////////////
//
int __gServerItemRefCount = 0;
IServerItem* __gServerItem = 0;

IServerItem* IServerItem::create()
{
	if (__gServerItemRefCount == 0)
	{
		__gServerItem = new CServerItem();
	}

	__gServerItemRefCount++;
	PLAZZ_PRINTF("IServerItem::create ref:%d\n", __gServerItemRefCount);

	return __gServerItem;
}

void IServerItem::destory()
{
	if (__gServerItemRefCount > 0)
	{
		__gServerItemRefCount--;
		if (__gServerItemRefCount <= 0)
		{
			__gServerItemRefCount = 0;
			delete __gServerItem;
			__gServerItem = 0;
		}
	}
	PLAZZ_PRINTF("IServerItem::destory ref:%d\n", __gServerItemRefCount);

}

IServerItem* IServerItem::get()
{
	return __gServerItem;
}

//////////////////////////////////////////////////////////////////////////
CServerItem::CServerItem() 
	: m_wReqTableID(INVALID_TABLE)
	, m_wReqChairID(INVALID_CHAIR)
	, mFindTableID(INVALID_TABLE)
	, mIsGameReady(false)
	, m_pMeUserItem(0)
	, mIServerItemSink(0)
	, mIServerMatchSink(0)
	, mIServerPrivateSink(NULL)
	, mIStringMessageSink(0)
	, mSocketEngine(0)
	, mServiceStatus(ServiceStatus_Unknow)
	, mIClientKernelSink(0)
	, mGameStatus(GAME_STATUS_FREE)
{
	mIsQuickSitDown=false;
	memset(&mUserAttribute,   0, sizeof(mUserAttribute));
	memset(&mServerAttribute, 0, sizeof(mServerAttribute));

	mSocketEngine = ISocketEngine::create();
	mUserManager  = new CPlazaUserManager();
	mUserManager->SetUserManagerSink(this);
	mSocketEngine->setSocketEngineSink(this);
}

CServerItem::~CServerItem()
{
	IntermitConnect(true);
	mIServerItemSink=0;
	if (mUserManager)
		mUserManager->SetUserManagerSink(0);
	if (mSocketEngine)
		mSocketEngine->setSocketEngineSink(0);
// 	delete mUserManager;
// 	delete mSocketEngine;
	ISocketEngine::destory(mSocketEngine);
	mSocketEngine = 0;
	SafeDelete(mUserManager);
	mUserManager  = 0;
}

//////////////////////////////////////////////////////////////////////////
//配置接口

//设置接口
bool CServerItem::SetServerItemSink(IServerItemSink* pIServerItemSink)
{
	mIServerItemSink = pIServerItemSink;
	return true;
}

//设置接口
bool CServerItem::SetStringMessageSink(IStringMessageSink* pIStringMessageSink)
{
	mIStringMessageSink = pIStringMessageSink;
	return true;
}
//设置接口
bool CServerItem::SetServerMatchSink(IServerMatchSink* pIServerMatchSink)
{
	mIServerMatchSink = pIServerMatchSink;
	return true;
}
//设置接口
bool CServerItem::SetServerPrivateSink(IServerPrivateSink* pIServerMatchSink)
{
	mIServerPrivateSink = pIServerMatchSink;
	return true;
}

//////////////////////////////////////////////////////////////////////////
//属性接口
//用户属性
const tagUserAttribute& CServerItem::GetUserAttribute() const
{
	return mUserAttribute;
}

//房间属性
const tagServerAttribute& CServerItem::GetServerAttribute() const 
{
	return mServerAttribute;
}

//服务状态
enServiceStatus CServerItem::GetServiceStatus()
{
	return mServiceStatus;
}

//是否服务状态
bool CServerItem::IsService()
{
	return GetServiceStatus() == ServiceStatus_ServiceIng;
}

//自己状态
bool CServerItem::IsPlayingMySelf()
{
	return ((m_pMeUserItem!=0)&&(m_pMeUserItem->GetUserStatus()==US_PLAYING));
}

//设置状态
void CServerItem::SetServiceStatus(enServiceStatus ServiceStatus)
{
	//设置变量
	mServiceStatus=ServiceStatus;
	return;
}

//配置房间
bool CServerItem::SetServerAttribute(CGameServerItem * pGameServerItem,word wAVServerPort,dword dwAVServerAddr)
{
	//房间属性
	CGameKindItem * pGameKindItem=pGameServerItem->m_pGameKindItem;
	memcpy(&mGameKind,&pGameKindItem->m_GameKind,sizeof(mGameKind));
	memcpy(&mGameServer,&pGameServerItem->m_GameServer,sizeof(mGameServer));

	mServerAttribute.wKindID		= mGameServer.wKindID;
	mServerAttribute.wServerID		= mGameServer.wServerID;
	///mServerAttribute.wServerType	= mGameServer.wServerType;
	
	mServerAttribute.wAVServerPort	= wAVServerPort;
	mServerAttribute.dwAVServerAddr	= dwAVServerAddr;
	strncpy(mServerAttribute.szServerName, mGameServer.szServerName, LEN_SERVER);

	CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();
	//加载配置
	mParameterGame=pParameterGlobal->GetParameterGame(&mGameKind);
	mParameterServer=pParameterGlobal->GetParameterServer(&mGameServer);

	return true;
}

//////////////////////////////////////////////////////////////////////////
//连接接口

//配置房间
bool CServerItem::ConnectServer(CGameServerItem * pGameServerItem,word wAVServerPort,dword dwAVServerAddr)
{
	//IntermitConnect(true);
	if (mServiceStatus != ServiceStatus_Unknow &&
		mServiceStatus != ServiceStatus_NetworkDown)
	{
		cocos2d::log("CServerItem::System_Tips_1");
		return false;
	}

	//房间属性
	SetServerAttribute(pGameServerItem, wAVServerPort, dwAVServerAddr);

	//关闭判断
	ASSERT(mServerAttribute.wServerID!=0);
	if (mServerAttribute.wServerID==0)
	{
		SetServiceStatus(ServiceStatus_Unknow);
		if (mIStringMessageSink)
			mIStringMessageSink->InsertPromptString(("System_Tips_2"), DLG_MB_OK);
		return false;
	}

	//创建组件
    const char * szServerAddr="127.0.0.1";
	PLAZZ_PRINTF("Connect %s:%d",szServerAddr, mGameServer.wServerPort);
	if (!mSocketEngine->connect(szServerAddr, mGameServer.wServerPort))
	{
		return false;
	}

	//设置状态
	SetServiceStatus(ServiceStatus_Entering);
	return true;
}

//中断连接
bool CServerItem::IntermitConnect(bool force)
{	
	cocos2d::log("CServerItem::IntermitConnect1");
	if (mServiceStatus==ServiceStatus_Unknow ||
		mServiceStatus==ServiceStatus_NetworkDown)
		return false;
	//设置状态
	cocos2d::log("CServerItem::IntermitConnect2");
	SetServiceStatus(ServiceStatus_NetworkDown);

	OnGFGameClose(0);

	restSeverState();
	return true;
}

//////////////////////////////////////////////////////////////////////////
//网络接口

//发送函数
bool CServerItem::SendSocketData(word wMainCmdID, word wSubCmdID)
{
	return SendSocketData(wMainCmdID, wSubCmdID, 0, 0);
}

//发送函数
bool CServerItem::SendSocketData(word wMainCmdID, word wSubCmdID, void * data, word dataSize)
{
	return mSocketEngine->send(wMainCmdID, wSubCmdID, (unsigned char*)data, dataSize);
}


//发送登录
bool CServerItem::SendLogonPacket()
{
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	
	//变量定义
	CMD_GR_LogonUserID LogonUserID;
	zeromemory(&LogonUserID,sizeof(LogonUserID));

	LogonUserID.wKindID = mGameKind.wKindID;
	//游戏版本
	LogonUserID.dwProcessVersion=DF::shared()->GetGameVersion();

	LogonUserID.dwPlazaVersion=0x10000;//DF::shared()->GetPlazaVersion();

	//登录信息
	LogonUserID.dwUserID=pGlobalUserData->dwUserID;
	strncpy(LogonUserID.szPassword,pGlobalUserData->szPassword,countarray(LogonUserID.szPassword));

	//发送数据
	SendSocketData(MDM_GR_LOGON,SUB_GR_LOGON_USERID,&LogonUserID,sizeof(LogonUserID));
	return true;
}

//发送配置
bool CServerItem::SendUserRulePacket()
{	
	//发送数据
	return true;
}

//发送旁观
bool CServerItem::SendLookonPacket(word wTableID, word wChairID)
{
	return true;
}

//发送坐下
bool CServerItem::SendSitDownPacket(word wTableID, word wChairID, const char* lpszPassword)
{
	if (!IsService())
		return false;

	//自己状态
	if (m_pMeUserItem->GetUserStatus()>=US_PLAYING)
	{
		//提示消息
		if (wTableID != m_pMeUserItem->GetTableID() && wChairID != m_pMeUserItem->GetChairID())
		{
			if (mIStringMessageSink)
				mIStringMessageSink->InsertPromptString(("InGame"), DLG_MB_OK);
			return false;
		}
	}

	CMD_GR_UserSitDown UserSitReq;
	zeromemory(&UserSitReq,0);
	memset(&UserSitReq,0,sizeof(UserSitReq));
	UserSitReq.wTableID=wTableID;
	UserSitReq.wChairID=wChairID;
	if (lpszPassword)
	{
		strcpy(UserSitReq.szTablePass,lpszPassword);
	}
	//发送数据包
	SendSocketData(MDM_GR_USER,SUB_GR_USER_SITDOWN,&UserSitReq,sizeof(UserSitReq));
	return true;
}

//发送起立
bool CServerItem::SendStandUpPacket(word wTableID, word wChairID, byte cbForceLeave)
{
	CMD_GR_UserStandUp UserStandUp;
	zeromemory(&UserStandUp,sizeof(UserStandUp));

	//构造数据
	UserStandUp.wTableID=wTableID;
	UserStandUp.wChairID=wChairID;
	UserStandUp.cbForceLeave=cbForceLeave;

	//发送数据
	SendSocketData(MDM_GR_USER,SUB_GR_USER_STANDUP,&UserStandUp,sizeof(UserStandUp));
	return true;
}

//发送聊天
bool CServerItem::SendUserChatPacket(dword dwTargetUserID, const char* pszChatString, dword dwColor)
{
	return true;
}

//发送表情
bool CServerItem::SendExpressionPacket(dword dwTargetUserID, word wItemIndex)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
//内部函数

//聊天效验
bool CServerItem::EfficacyUserChat(const char* pszChatString, word wExpressionIndex)
{
	//变量定义
	ASSERT(m_pMeUserItem!=0);
	byte cbMemberOrder=m_pMeUserItem->GetMemberOrder();
	byte cbMasterOrder=m_pMeUserItem->GetMasterOrder();

	//权限判断
	if (CUserRight::CanRoomChat(mUserAttribute.dwUserRight)==false)
	{
		return false;
	}

	//速度判断
	static dword dwChatTime=0;
	dword dwCurrentTime=(dword)time(0);
	if ((cbMasterOrder==0)&&((dwCurrentTime-dwChatTime)<=TIME_USER_CHAT))
	{
		return false;
	}

	//设置变量
	dwChatTime=dwCurrentTime;
	return true;
}

//桌子效验
bool CServerItem::EfficacyTableRule(word wTableID, word wChairID, bool bReqLookon, char strDescribe[256])
{
	//效验参数
	ASSERT(wTableID<m_TableFrame.GetTableCount());
	ASSERT(wChairID<m_TableFrame.GetChairCount());

	//状态过滤
	if (mServiceStatus!=ServiceStatus_ServiceIng) return false;
	if (wTableID>=m_TableFrame.GetTableCount()) return false;
	if (wChairID>=m_TableFrame.GetChairCount()) return false;

	//变量定义
	ASSERT(CParameterGlobal::shared()!=0);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::shared();

	//变量定义
	ITableView * pITableView=m_TableFrame.GetTableViewItem(wTableID);
	//IClientUserItem * pITableUserItem=pITableView->GetClientUserItem(wChairID);

	//变量定义
	bool bGameStart=pITableView->GetPlayFlag();

	//游戏状态
	if ((bGameStart==true)&&(bReqLookon==false))
	{
		sprintf(strDescribe, ("Game alerdy start, you not come in!"));
		return false;
	}

	//其他判断
	if ((bReqLookon==false)&&(m_pMeUserItem->GetMasterOrder()==0))
	{
		//规则判断
		for (word i=0;i<m_TableFrame.GetChairCount();i++)
		{
			//获取用户
			IClientUserItem * pITableUserItem=pITableView->GetClientUserItem(i);
			if ((pITableUserItem==0)||(pITableUserItem==m_pMeUserItem)) continue;

			//厌恶玩家
			if (pParameterGlobal->m_bLimitDetest==true)
			{
				if (pITableUserItem->GetUserCompanion()==CP_DETEST)
				{
					//设置提示
					sprintf(strDescribe, ("System_Tips_15"), pITableUserItem->GetNickName());
					return false;
				}
			}

			//胜率效验
			if (mParameterGame->m_bLimitWinRate==true)
			{
				if (((word)(pITableUserItem->GetUserWinRate()*1000L))<mParameterGame->m_wMinWinRate)
				{
					sprintf(strDescribe, ("System_Tips_16"), pITableUserItem->GetNickName());
					return false;
				}
			}

			//逃率效验
			if (mParameterGame->m_bLimitFleeRate)
			{
				if (((word)(pITableUserItem->GetUserFleeRate()*1000L))<mParameterGame->m_wMaxFleeRate)
				{
					sprintf(strDescribe, ("System_Tips_17"), pITableUserItem->GetNickName());
					return false;
				}
			}

			//积分效验
			if (mParameterGame->m_bLimitGameScore)
			{
				//最高积分
				if (pITableUserItem->GetUserScore()>mParameterGame->m_lMaxGameScore)
				{
					sprintf(strDescribe, ("System_Tips_18"), pITableUserItem->GetNickName());
					return false;
				}

				//最低积分
				if (pITableUserItem->GetUserScore()<mParameterGame->m_lMinGameScore)
				{
					sprintf(strDescribe, ("System_Tips_19"), pITableUserItem->GetNickName());
					return false;
				}
			}
		}
	}

	return true;
}

int CServerItem::GetSpaceTableId()
{
	int tableCount = m_TableFrame.GetTableCount();
	for (int i = 0; i < tableCount; i++)
	{
		ITableView * tableView = m_TableFrame.GetTableViewItem(i);
		if (tableView)
		{
			for (byte j = 0; j < m_TableFrame.GetChairCount(); j++)
			{
				IClientUserItem * userItem = tableView->GetClientUserItem(j);
				if (!userItem)
				{
					return i;
				}
			}
		}
	}
	return INVALID_TABLE;
}

int CServerItem::GetSpaceChairId(int tableId)
{
	ITableView * tableView = m_TableFrame.GetTableViewItem(tableId);
	if (tableView)
	{
		for (byte j = 0; j < m_TableFrame.GetChairCount(); j++)
		{
			IClientUserItem * userItem = tableView->GetClientUserItem(j);
			if (!userItem)
			{
				return j;
			}
		}
	}
	return INVALID_CHAIR;
}

int CServerItem::GetTotalTableCount()
{
	return m_TableFrame.GetTableCount();
}
bool CServerItem::onEventTCPHeartTick()
{
	if (mIServerItemSink)
	{
		mIServerItemSink->HeartTick();
	}
	return true;
}