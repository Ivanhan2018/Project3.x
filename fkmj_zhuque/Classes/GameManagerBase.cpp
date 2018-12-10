#include "GameManagerBase.h"
#include "NoticeMsg.h"
#include "UserInfo.h"
#include "utility.h"
#include "JniFun.h"
#include "GameScriptNet.h"

FV_SINGLETON_STORAGEBASE(GameManagerBase);

#define MaxReconnectTime  15.0f

GameManagerBase::GameManagerBase()
	:mCServerItem(NULL)
	,m_fHeatTickTime(0)
	,mGameServerItem(NULL)
	,m_eInReconnect(ReconnectStatus_NULL)
{
	mCServerItem = dynamic_cast<CServerItem *>(IServerItem::create());
	mCServerItem->SetServerItemSink(this);
	mCServerItem->SetStringMessageSink(this);

	cocos2d::Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(GameManagerBase::upDataTime,this),this,0.0f,false,"GameManagerBase");
}

GameManagerBase::~GameManagerBase()
{
	if (IServerItem::get())
	{
		mCServerItem->SetServerItemSink(0);
		mCServerItem->SetStringMessageSink(0);
		mCServerItem->SetStringMessageSink(0);
	}

	if (IServerItem::get())
	{
		IServerItem::get()->IntermitConnect(true);
	}

	IServerItem::destory();
	mCServerItem = 0;

	cocos2d::Director::getInstance()->getScheduler()->unschedule("GameManagerBase",this);
}
int GameManagerBase::getActServerID()
{
	if (mGameServerItem)
	{
		return mGameServerItem->m_GameServer.wServerID;
	}
	return -1;
}
CGameServerItem* GameManagerBase::SearchGameServer(int iServerID)
{
	return CServerListData::shared()->SearchGameServer(iServerID);
}
bool GameManagerBase::connectGameServerByKindID(word wKindID,word wServerType)
{
	CGameServerItem* pMinPlayerCoutServer = CServerListData::getGameServerByKind(wKindID,wServerType);
	if (pMinPlayerCoutServer)
	{
		connectGameServer(pMinPlayerCoutServer);
	}
	else
	{
		CCASSERT(false,"GameManager::connectGameServerByKindID");
	}
	return true;
}
bool GameManagerBase::connectGameServerByServerID(int iServerID)
{
	CGameServerItem* pServer = CServerListData::shared()->SearchGameServer(iServerID);
	if (pServer)
	{
		connectGameServer(pServer);
	}
	else
	{
		//CCASSERT(false,"GameManager::connectGameServerByServerID");
	}
	return true;
}

bool GameManagerBase::IsPrivateGame()
{
	if (!mGameServerItem)
	{
		return false;
	}
	return mGameServerItem->IsPrivateRoom();
}
bool GameManagerBase::connectGameServer(CGameServerItem* pGameServerItem)
{
	utility::log("GameManagerBase::connectGameServer");
	if (mCServerItem->IsService())
	{
		disconnectServer();
	}

	utility::log("GameManagerBase::connectGameServer connectServer");
	mGameServerItem = pGameServerItem;
	
	connectServer();

	RestTick();

	return true;
}

void GameManagerBase::onEnterTransitionDidFinish()
{	
}

//////////////////////////////////////////////////////////////////////////
// IServerListDataSink
//完成通知
void GameManagerBase::OnGameItemFinish()
{
	//setDatas();
}

//完成通知
void GameManagerBase::OnGameKindFinish(uint16 wKindID)
{
}

//更新通知
void GameManagerBase::OnGameItemUpdateFinish()
{
}

//插入通知
void GameManagerBase::OnGameItemInsert(CGameListItem * pGameListItem)
{
}

//更新通知
void GameManagerBase::OnGameItemUpdate(CGameListItem * pGameListItem)
{
}

//删除通知
void GameManagerBase::OnGameItemDelete(CGameListItem * pGameListItem)
{
}

//////////////////////////////////////////////////////////////////////////
//登陆信息

//请求失败
void GameManagerBase::onGRRequestFailure(const std::string& szDescribeString)
{
	PLAZZ_PRINTF(("GameManager::onGRRequestFailure"));
}

//登陆成功
void GameManagerBase::OnGRLogonSuccess(void* data, int dataSize)
{
	PLAZZ_PRINTF(("GameManager::OnGRLogonSuccess"));

	//效验参数
	ASSERT(dataSize==sizeof(CMD_GR_LogonSuccess));
	if (dataSize!=sizeof(CMD_GR_LogonSuccess)) return;

	//消息处理
	CMD_GR_LogonSuccess * pLogonSuccess=(CMD_GR_LogonSuccess *)data;

	//玩家属性
	//UserInfo::Instance().setUserRight(pLogonSuccess->dwUserRight);
	//UserInfo::Instance().setMasterRight(pLogonSuccess->dwMasterRight);

	RestTick();
}

//登陆失败
void GameManagerBase::OnGRLogonFailure(long lErrorCode, const std::string& sDescribeString)
{
	PLAZZ_PRINTF(("GameManager::OnGRLogonFailure"));
}

//登陆完成
void GameManagerBase::OnGRLogonFinish()
{
	PLAZZ_PRINTF(("GameManager::OnGRLogonFinish"));

	m_eInReconnect = ReconnectStatus_NULL;
	TimeManager::Instance().removeByFun(TIME_CALLBACK(GameManagerBase::closeClinet,this));

	//重入判断
	if (mCServerItem && (mCServerItem->GetMeUserItem()!=NULL)&&(mCServerItem->GetMeUserItem()->GetUserStatus() >= US_SIT))
	{
		//启动进程
		if (!StartGame())
		{
			mCServerItem->OnGFGameClose(GameExitCode_CreateFailed);
		}
		return;
	}
	else 
	{
		UserInfo::Instance().setPrivateInGameServerID(-1);
	}
	CB_GameLogonFinsh();
}
void GameManagerBase::CB_GameLogonFinsh()
{
	if (mCServerItem)
	{
		if (!mCServerItem->PerformQuickSitDown())
		{
			NoticeMsg::Instance().ShowTopMsgByScript("RoomFull");
		}
	}
}
//更新通知
void GameManagerBase::OnGRUpdateNotify(byte cbMustUpdate, const std::string& szDownHttp)
{
	NoticeMsg::Instance().ShowTopMsg("Update");
	PLAZZ_PRINTF(("GameManager::OnGRUpdateNotify"));
	JniFun::showWebView(szDownHttp.c_str());
}

//////////////////////////////////////////////////////////////////////////
//配置信息

//列表配置
void GameManagerBase::OnGRConfigColumn()
{
	PLAZZ_PRINTF(("GameManager::OnGRConfigColumn"));
}
//房间配置
void GameManagerBase::OnGRConfigServer()
{
	PLAZZ_PRINTF(("GameManager::OnGRConfigServer"));
	//创建桌子
	if (!mCServerItem) return;
	word tChairCount = mCServerItem->GetServerAttribute().wChairCount;

}
//道具配置
void GameManagerBase::OnGRConfigProperty()
{
	PLAZZ_PRINTF(("OnGRConfigProperty\n"));
}
//玩家权限配置
void GameManagerBase::OnGRConfigUserRight()
{
	PLAZZ_PRINTF(("OnGRConfigUserRight\n"));
}

//配置完成
void GameManagerBase::OnGRConfigFinish()
{
	PLAZZ_PRINTF(("OnGRConfigFinish\n"));

}

//////////////////////////////////////////////////////////////////////////
//用户信息

//用户进入
void GameManagerBase::OnGRUserEnter(IClientUserItem* pIClientUserItem)
{
	PLAZZ_PRINTF(("GameManager::OnGRUserEnter"));

	char szDescribe[256] = { 0 };
	sprintf(szDescribe, ("OnGRUserEnter:%s\n"), pIClientUserItem->GetNickName());
	//PLAZZ_PRINTF(szDescribe);
}
//用户更新
void GameManagerBase::OnGRUserUpdate(IClientUserItem* pIClientUserItem)
{
	PLAZZ_PRINTF(("GameManager::OnGRUserUpdate"));

	if (pIClientUserItem == IServerItem::get()->GetMeUserItem())
	{
	}
}
//用户删除
void GameManagerBase::OnGRUserDelete(IClientUserItem* pIClientUserItem)
{
	PLAZZ_PRINTF("GameManager::OnGRUserDelete");
}

//////////////////////////////////////////////////////////////////////////
//框架消息

//用户邀请
void GameManagerBase::OnGFUserInvite(const std::string& szMessage)
{
	PLAZZ_PRINTF("GameManager::OnGFUserInvite");
}
//用户邀请失败
void GameManagerBase::OnGFUserInviteFailure(const std::string& szMessage)
{
	PLAZZ_PRINTF("GameManager::OnGFUserInviteFailure");
}

//房间退出
void GameManagerBase::OnGFServerClose(const std::string& szMessage)
{
	PLAZZ_PRINTF("ServerScene::OnGFServerClose");

}

//启动游戏
bool GameManagerBase::StartGame()
{
	PLAZZ_PRINTF("GameManager::StartGame");

	this->CreateKernel();

	return true;
}
//创建游戏内核
bool GameManagerBase::CreateKernel()
{
	PLAZZ_PRINTF("GameManager::CreateKernel");

	IClientKernelSink* pKernelSink = CreateGame(mGameServerItem->m_GameServer.wKindID);
	if (!pKernelSink)
	{
		IServerItem::get()->IntermitConnect(true);
		return false;
	}
	pKernelSink->clearInfo();
	pKernelSink->OnGameServerItem(mGameServerItem);
	IServerItem::get()->SetClientKernelSink(pKernelSink);
	IServerItem::get()->OnGFGameReady();
	return false;
}



void GameManagerBase::connectServer()
{
	if (mCServerItem->GetServiceStatus() == ServiceStatus_ServiceIng
		|| 	mCServerItem->GetServiceStatus() == ServiceStatus_Entering)
	{
		return;
	}

	RestTick();

	///< 载入游戏基础数据
	loadGameBaseData(mGameServerItem->m_GameServer.wKindID);
	mCServerItem->ConnectServer(mGameServerItem, 0, 0);
}

void GameManagerBase::disconnectServer()
{
	if (mCServerItem)
	{
		if (IServerItem::get())
		{
			IServerItem::get()->IntermitConnect(true);
		}
	}
}
//进入事件
bool GameManagerBase::InsertUserEnter(const char* pszUserName)
{
	return true;
}
//离开事件
bool GameManagerBase::InsertUserLeave(const char* pszUserName)
{
	return true;
}
//断线事件
bool GameManagerBase::InsertUserOffLine(const char* pszUserName)
{
	return true;
}
//普通消息(窗口输出)
bool GameManagerBase::InsertNormalString(const char* pszString)
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pszString));
	return true;
}
//系统消息(窗口输出)
bool GameManagerBase::InsertSystemString(const char* pszString)
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pszString));
	return true;
}
//系统消息(窗口输出)
bool GameManagerBase::InsertSystemStringScript(const char* pszString)
{
	NoticeMsg::Instance().ShowTopMsgByScript(pszString);
	return true;
}
//提示消息(对话框方式??)0:确认 1:确认,取消
int GameManagerBase::InsertPromptString(const char* pszString, int iButtonType)
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pszString));
	return true;
}
bool GameManagerBase::InsertGladString(const char* pszContent, const char* pszNickName, const char* pszNum, dword colText, dword colName, dword colNum)
{
	return true;
}
void GameManagerBase::HeartTick()
{
	if (!IServerItem::get())
	{
		return;
	}
	RestTick();
}
void GameManagerBase::onEventTCPSocketError(int errorCode)
{
	if (mCServerItem && mCServerItem->IsInGame())
	{
		TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(GameManagerBase::closeClinet,this),3.0f);
	}
	TimeManager::Instance().removeByFun(TIME_CALLBACK(UserInfo::checkInGameServer,UserInfo::pInstance()));
	TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(UserInfo::checkInGameServer,UserInfo::pInstance()),1.0f);
}
void GameManagerBase::closeClinet()
{
	if (mCServerItem)
	{
		mCServerItem->OnGFGameClose(GameExitCode_Normal);
	}
	TimeManager::Instance().removeByFun(TIME_CALLBACK(UserInfo::checkInGameServer,UserInfo::pInstance()));
	TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(UserInfo::checkInGameServer,UserInfo::pInstance()),1.0f);
}
void GameManagerBase::RestTick()
{
	m_eInReconnect = ReconnectStatus_NULL;
	m_fHeatTickTime = 0;
}
void GameManagerBase::StartGameReconnect()
{
	if (IServerItem::get()->IsService())
	{
		disconnectServer();
		UserInfo::Instance().checkInGameServer();
	}
}
void GameManagerBase::upDataTime(float fTime)
{
	if (!IServerItem::get() || !mGameServerItem)
	{
		return;
	}
	if (IServerItem::get()->IsService() && m_eInReconnect == ReconnectStatus_NULL)
	{
		m_fHeatTickTime += fTime;
	}
	if (m_fHeatTickTime > MaxReconnectTime)
	{
		m_fHeatTickTime = 0;
		NoticeMsg::Instance().ShowNoticeMsgByScript("NetReconnectError");
		m_eInReconnect = ReconnectStatus_DisConnect;
		disconnectServer();
		UserInfo::Instance().checkInGameServer();
	}
}
void GameManagerBase::applicationDidEnterBackground()
{
}
void GameManagerBase::applicationWillEnterForeground()
{
	cocos2d::log("GameManagerBase::applicationWillEnterForeground");
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	if (IServerItem::get())
	{
		cocos2d::log("GameManagerBase::StartGameReconnect");
		StartGameReconnect();
	}
#endif
	GameScriptNet::InstanceBase().OnGetConfig();
}
bool GameManagerBase::BackKey()
{
	if (mCServerItem)
	{
		return mCServerItem->BackKey();
	}
	return false;
}

SCORE GameManagerBase::getGameDiFen()
{
	if (!mGameServerItem)
	{
		return 0;
	}
	return mGameServerItem->m_GameServer.lCellScore;
}
