#include "LrbyClientEngine.h"
#include "LrbyGameConfig.h"
#include "DataThread.h"
#include "FishSceneData.h"
#include "UINotificationCenter.h"
#include "MyNSString.h"

//////////////////////////////////////////////////////////////////////////

CLrbyGameConfig g_LrbyGameConfig;
extern CDataThread g_DataThread;


LrbyClientEngine::LrbyClientEngine(void)
{
	//m_wChairCount = BY_GAME_PLAYER;
	m_pGameClientView = NULL;								//游戏视图
	//m_pClientKernel = NULL;	//游戏内核
	for (int i = 0; i<BY_GAME_PLAYER; i++)
	{
		m_userInfo[i] = nullptr;
	}
	m_isGameStart = false;
	m_isMyself = false;

	// new add by zengxin

	//NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LrbyClientEngine::onUserStart),"GameStart" , NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LrbyClientEngine::onUpdateTable),MSG_UI_ANS_PLAYERUPDATE,NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LrbyClientEngine::onMessageGame),GBEVENT_UI_GAMEMESSAGE,NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LrbyClientEngine::onMessageScene),GBEVENT_UI_SCENEMESSAGE,NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LrbyClientEngine::onUpdataMatch),MSG_UI_ANS_MATCH,NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LrbyClientEngine::onViewMessage),MESSAGE_TOLOGIC,NULL);
	//NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LrbyClientEngine::onLinkResult),MSG_UI_ANS_GAMELINK , NULL);				//���ӷ�����
	//NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LrbyClientEngine::onUpdateScoreAndGoldEgg),MSG_UI_ANS_UPDATESCOREANDGOLDEGG , NULL);			//--��ֵˢ�½��
	//this->schedule(schedule_selector(DDZSceneLogic::checkNet), 8);
	EntityMgr::instance()->getDispatch()->setGameStatus(true);

	//消息
	Director::getInstance()->getScheduler()->scheduleSelector(schedule_selector(CUINotificationCenter::scheduleSelector), CUINotificationCenter::getInstance(), 1.0 / 60.0f, false);
}

bool LrbyClientEngine::init()
{	
	return Node::init();
}

void LrbyClientEngine::onEnter()
{
	Node::onEnter();
}

void LrbyClientEngine::onExit()
{
	Node::onExit();
}

LrbyClientEngine::~LrbyClientEngine(void)
{
	g_DataThread.stop();
	m_pGameClientView = NULL;
	for (int i = 0; i<BY_GAME_PLAYER; i++)
	{
		m_userInfo[i] = nullptr;
	}

	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_PLAYERUPDATE);
	NotificationCenter::getInstance()->removeObserver(this,GBEVENT_UI_GAMEMESSAGE);
	NotificationCenter::getInstance()->removeObserver(this,GBEVENT_UI_SCENEMESSAGE);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_MATCH);
	NotificationCenter::getInstance()->removeObserver(this,MESSAGE_TOLOGIC);
	//NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_GAMELINK);
	//NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_UPDATESCOREANDGOLDEGG);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(CUINotificationCenter::scheduleSelector), CUINotificationCenter::getInstance());
}


void LrbyClientEngine::CloseGameView()
{
	//log("LrbyClientEngine::CloseGameView");

	m_pGameClientView = nullptr;
	//CGameSink::CloseGameView();
}

//开始消息
void LrbyClientEngine::Start()
{
	//log("LrbyClientEngine::Start");
	if (m_pGameClientView == NULL || !m_pGameClientView->isVisible())
		return;
	
	//m_pClientKernel->SendUserReady(NULL,0);
}

// 初始化游戏视图
void* LrbyClientEngine::InitGameView()
{
	//log("LrbyClientEngine::InitGameView");
	if(m_pGameClientView==NULL)
	{
		m_pGameClientView = LrbyClientView::create();
		g_DataThread.start();
	}

	this->addChild(m_pGameClientView);

	return m_pGameClientView;
}
// 获取游戏视图
void* LrbyClientEngine::GetGameView()
{
	return m_pGameClientView;
}
DWORD LrbyClientEngine::GetGameVersion()
{
	int nProductVer = 7;
	int nMainVer = 0;
	int nSubVer = 0;
	int nBuildVer = 0;
	long gameVersion = ((nProductVer & 0xFF) << 24)
		| ((nMainVer & 0xFF) << 16) | ((nSubVer & 0xFF) << 8)
		| (nBuildVer & 0xFF);
	return gameVersion;
}

// 游戏初始化
bool LrbyClientEngine::OnInitClientKernel(TableData* clientKernel)
{
	for (int i=0; i<BY_GAME_PLAYER; ++i)
	{
		m_userInfo[i] = nullptr;
	}
	return false;//CGameSink::OnInitClientKernel(clientKernel);
}

void LrbyClientEngine::ReleaseGameView()
{
	m_pGameClientView=NULL;
}

// 救济金结果
bool LrbyClientEngine::OnGetAlmsResult(BYTE cbRetCode)
{
	return true;
}

// 旁观状态
void LrbyClientEngine::OnEventLookonMode(bool bLookonUser, void* pData, WORD wDataSize)
{
	return ;
}

// 网络消息 
bool LrbyClientEngine::OnEventFrameMessage(WORD wSubCmdID, void* pData, WORD wDataSize)
{
	
	return false;
}
// 游戏消息
bool LrbyClientEngine::OnEventGameMessage(WORD wSubCmdID, void* pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_C_TIME_STAMP:
	{
		OnSubTimeStamp(pData, wDataSize);		
	}
	break;
	case SUB_S_GAME_CONFIG:
	{
		OnSubGameConfig(pData, wDataSize);		
	}
	break;
	case SUB_S_FISH_TRACE:
		{
			if (wDataSize % sizeof(CMD_S_FishTrace) != 0)
			{
				return false;
			}
			CMD_S_FishTrace* fishTrace = static_cast<CMD_S_FishTrace*>(pData);
			OnSubFishTrace(pData, wDataSize);
		}
		break;
	case SUB_S_EXCHANGE_FISHSCORE:
		{
			OnSubExchangeFishScore(pData, wDataSize);
		}
		break;
	case SUB_S_USER_FIRE:
		{
			OnSubUserFire(pData, wDataSize);
		}
		break;
	case SUB_S_CATCH_FISH:
		{
			OnSubCatchFish(pData, wDataSize);
		}
		break;
	case SUB_S_BULLET_ION_TIMEOUT:
		{
			OnSubBulletIonTimeout(pData, wDataSize);
		}
		break;
	case SUB_S_LOCK_TIMEOUT:
		{
			OnSubLockTimeout(pData, wDataSize);
		}
		break;
	case SUB_S_CATCH_SWEEP_FISH:
		{
			OnSubCatSweepFish(pData, wDataSize);
		}
		break;
	case SUB_S_CATCH_SWEEP_FISH_RESULT:
		{
			OnSubCatSweepFishResult(pData, wDataSize);
		}
		break;
	case SUB_S_HIT_FISH_LK:
		{
			OnSubHitFishLK(pData, wDataSize);
		}
		break;
	case SUB_S_SWITCH_SCENE:
		{
			OnSubSwitchScene(pData, wDataSize);
		}
		break;
	case SUB_S_STOCK_OPERATE_RESULT:
		{
			OnSubStockOperateResult(pData, wDataSize);
		}
		break;
	case SUB_S_SCENE_END:
		{
			OnSubSceneEnd(NULL, 0);
		}
		break;
	case SUB_S_ONLINE_AWARD:
		{
			OnOnLineAward(pData, wDataSize);
		}
		break;
	case SUB_S_AWARD_RESULT:
		{
			if (wDataSize != sizeof(CMD_S_AwardScore))
			{
				return false;
			}

			CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GM_AwardScore,0, pData, wDataSize);
			CUINotificationCenter::getInstance()->postNotification("game_fishMessage", pNotifRef);	
		}
		break;
	case SUB_S_FIRE_SCORE:
		{
			//no use msg
			if (wDataSize != sizeof(CMD_S_FishScore))
			{
					return false;
			}
			CMD_S_FishScore* fishscore = static_cast<CMD_S_FishScore*>(pData);

			CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GM_FireScore,0, fishscore, wDataSize);
			CUINotificationCenter::getInstance()->postNotification("game_fishMessage", pNotifRef);
		}
		break;
	case SUB_S_NOW_HARVEST:
		{
			if(wDataSize != sizeof(CMD_S_FireHarvest))
			{
				return false;
			}

			CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GM_HARVEST,0, pData, wDataSize);
			CUINotificationCenter::getInstance()->postNotification("game_fishMessage", pNotifRef);	
		}
	default:break;
  
	}
	return true;
}

// 场景消息
bool LrbyClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, void * pData, WORD wDataSize)
{
	switch (cbGameStatus) {
	case GAME_STATUS_FREE:
	case GAME_STATUS_PLAY:
		if (wDataSize != sizeof(CMD_S_GameStatus)) return false;
		CMD_S_GameStatus* gamestatus = static_cast<CMD_S_GameStatus*>(pData);
		//if (gamestatus->game_version != GAME_VERSION) return false;

		CMD_S_FishScore fishscore;
		for (WORD i = 0; i < BY_GAME_PLAYER; ++i) {
			fishscore.fish_score[i] =  gamestatus->fish_score[i];
		}
		//allow_fire_ = true;
		//last_fire_time_ = timeGetTime();
		//show_user_info_chairid_ = client_kernel_->GetMeChairID();		

		CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GM_FireScore,0, &fishscore, sizeof(fishscore));
		CUINotificationCenter::getInstance()->postNotification("game_fishMessage", pNotifRef);
		
		return true;
	}
	return true;
}
// 桌面滚动消息
bool LrbyClientEngine::AddGameTableMessage(TCHART* strMessage, WORD wType)
{
	return true;
}
// 比赛结果消息
bool LrbyClientEngine::OnEventMatchMessage(WORD wSubCmdID, void* pData, WORD wDataSize)
{
	return true;
}
// 用户进入
void LrbyClientEngine::OnEventUserEnter(tagDDZUserInfoHead* userInfo, WORD wChairID, bool bLookonMode)
{
	if (!bLookonMode)
	{
		m_userInfo[wChairID] =userInfo;
		CUINotificationCenter::getInstance()->postNotification("update_userinfo", NULL);
	}
}
// 用户离开
void LrbyClientEngine::OnEventUserLeave(tagDDZUserInfoHead* userInfo, WORD wChairID, bool bLookonMode)
{
	if (!bLookonMode)
	{
		if (wChairID != INVALID_CHAIR)
		{
			m_userInfo[wChairID] = NULL;
		}
		CUINotificationCenter::getInstance()->postNotification("update_userinfo", NULL);
	}
}
// 用户积分
void LrbyClientEngine::OnEventUserScore(tagDDZUserInfoHead* userInfo, WORD wChairID, bool bLookonMode)
{
}
// 用户状态
void LrbyClientEngine::OnEventUserStatus(tagDDZUserInfoHead* userInfo, WORD wChairID, bool bLookonMode)
{

}
// 用户会员
void LrbyClientEngine::OnEventUserMemberOrder(tagDDZUserInfoHead* userInfo, WORD wChairID,bool bLookonMode)
{
	return;
}
// 桌子状态
void LrbyClientEngine::OnEventTableStatus(BYTE cbTableStatus)
{
	return;
}
// 重置框架
void LrbyClientEngine::ResetGameFrame()
{
	return;
}
// 消息显示与处理
bool LrbyClientEngine::OnMessage(WORD wType, const char* szText)
{
	return true;
}
// 获取旁观
bool LrbyClientEngine::GetAllowLookon()
{
	return true;
}

bool LrbyClientEngine::OnTimer(unsigned int uTimerID, unsigned int uBindParameter)
{
	log("LrbyClientEngine::OnTimer");

	return true;
}

// 申请订单结果
bool LrbyClientEngine::OnApplyOrderResult(signed int nResultCode, uint16_t wCardTotal, uint32_t dwOrderAmount,const char* lpszOrderID,TCHART* lpszDesc)
{
	char szBuf[512]={0};
	// 失败
	if ((nResultCode!=0) || (wCardTotal==0)|| (dwOrderAmount==0) || (lpszOrderID[0]==0))
	{
		
	}
	else // 成功
	{		
		signed int size = sizeof(wCardTotal);
		signed int sizeTotal = size;
		memcpy(szBuf, &wCardTotal, size);
		size = sizeof(dwOrderAmount);		
		memcpy(&szBuf[sizeTotal], &dwOrderAmount, size);
		sizeTotal+= size;
		strncpy(&szBuf[sizeTotal],lpszOrderID, sizeof(szBuf)-sizeTotal);

		CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create( em_GR_ApplyOrderResult, 0,szBuf,sizeof(szBuf));
		CUINotificationCenter::getInstance()->postNotification("ongamesinkupdate", pNotifRef);
	}
	return true;
}
// 支付结果
bool LrbyClientEngine::OnPayResult(signed int nResultCode, uint8_t cbScoreType, LONGLONG lScore,TCHART* lpszDesc)
{
	char szBuf[512]={0};
	// 失败
	if (nResultCode!=0)//非0即失败
	{
		////TChar2Utf8(lpszDesc,szBuf,sizeof(szBuf)-1);
		CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create( em_GR_PayResult, nResultCode,szBuf,sizeof(szBuf));
		CUINotificationCenter::getInstance()->postNotification("ongamesinkupdate", pNotifRef);
	}
	else // 成功
	{


		//兑换炮
		CMD_C_ExchangeFishScore exchangeFishScore;
		memset(&exchangeFishScore, 0, sizeof(CMD_C_ExchangeFishScore));
		exchangeFishScore.increase = 2;
		//verysharp //exchangeFishScore.iExchangeCount = 0;
		//发送消息
		//SendData(SUB_C_EXCHANGE_FISHSCORE, &exchangeFishScore, sizeof(CMD_C_ExchangeFishScore));
		CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create( em_GR_PayResult, nResultCode,szBuf,sizeof(szBuf));
		CUINotificationCenter::getInstance()->postNotification("ongamesinkupdate", pNotifRef);
	}
	return true;
}

bool LrbyClientEngine::OnSubExchangeFishScore(void* data, WORD data_size)		//换鱼币
{
	if (data_size != sizeof(CMD_S_ExchangeFishScore)) 
	{
		return false;
	}
		
	CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GM_ExchangeFishScore,0, data, data_size);
	CUINotificationCenter::getInstance()->postNotification("game_fishMessage", pNotifRef);

	return true;
}

bool LrbyClientEngine::OnSubFishTrace(void* data, WORD data_size)
{
	if (data_size % sizeof(CMD_S_FishTrace) != 0) 
		return false;
	if (g_LrbyGameConfig.IsSwitchingScene())
		return true;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
	//进入临界区
	EnterCriticalSection(&g_DataThread.m_crsec);

	//copy data
	CMD_S_FishTrace* fish_trace = static_cast<CMD_S_FishTrace*>(data);
	WORD fish_trace_count = data_size / sizeof(CMD_S_FishTrace);
	CMD_S_FishTrace item_fish_trace = { 0 };

	for (WORD i = 0; i < fish_trace_count; ++i)
	{
		memcpy(&item_fish_trace, fish_trace, sizeof(CMD_S_FishTrace));
		// add by hxh 20161227
#ifdef USE_TRACETIME
		CMD_C_TimeStamp* pTimeStamp=&m_pGameClientView->m_TimeStamp;
		if(pTimeStamp->tClientSendTime>0 && pTimeStamp->tServerRecvTime>0)
		{
			long long msElapsed=LrbyClientView::GetClientTickCount()-item_fish_trace.BuildTraceTime-pTimeStamp->tClientServerTimeInterval;
			CCLOG("i=%d,msElapsed=%lld",i,msElapsed);
			bool bDel=(msElapsed>10000);// 假定鱼的存活时间是10秒
			if(bDel)
			{
				++fish_trace;
				continue;
			}
		}
#endif
		g_DataThread.m_vecFishTrace.push_back(item_fish_trace);
		++fish_trace;	
	}

	SetEvent(g_DataThread.m_hEvent);
	//离开临界区
	LeaveCriticalSection(&g_DataThread.m_crsec);	

#else
	pthread_mutex_lock( &g_DataThread.m_mtx );

	//copy data
	CMD_S_FishTrace* fish_trace = static_cast<CMD_S_FishTrace*>(data);
	WORD fish_trace_count = data_size / sizeof(CMD_S_FishTrace);
	CMD_S_FishTrace item_fish_trace = { 0 };

	for (WORD i = 0; i < fish_trace_count; ++i)
	{
		memcpy(&item_fish_trace, fish_trace, sizeof(CMD_S_FishTrace));
		g_DataThread.m_vecFishTrace.push_back(item_fish_trace);
		++fish_trace;	
	}

	pthread_cond_signal(&g_DataThread.m_cond);
	pthread_mutex_unlock(&g_DataThread.m_mtx);

#endif
	//CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GM_FishTrace,0, data, data_size);
	//CUINotificationCenter::getInstance()->postNotification("game_fishMessage", pNotifRef);

	return true;
}

bool LrbyClientEngine::OnSubUserFire(void* data, WORD data_size) 
{
	if (data_size != sizeof(CMD_S_UserFire)) return false;

	CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GM_UserFire,0, data, data_size);
	CUINotificationCenter::getInstance()->postNotification("game_fishMessage", pNotifRef);

	return true;
}

bool LrbyClientEngine::OnSubCatchFish(void* data, WORD data_size)	//抓住鱼
{
	if (data_size != sizeof(CMD_S_CatchFish))
	{
		return false;
	}

	CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GM_CatchFish,0, data, data_size);
	CUINotificationCenter::getInstance()->postNotification("game_fishMessage", pNotifRef);

	return true;
}

bool LrbyClientEngine::OnSubBulletIonTimeout(void* data, WORD data_size)
{
	if (data_size != sizeof(CMD_S_BulletIonTimeout))
	{
		return false;
	}

	CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GM_BulletIONTimeOut,0, data, data_size);
	CUINotificationCenter::getInstance()->postNotification("game_fishMessage", pNotifRef);

	return true;
}

bool LrbyClientEngine::OnSubLockTimeout(void* data, WORD data_size)
{
	//20150602lock_ = false;
	m_pGameClientView->setScreenLock(false);	//锁屏时间结束

	return true;
}

bool LrbyClientEngine::OnSubCatSweepFish(void* data, WORD data_size)
{
	if (data_size != sizeof(CMD_S_CatchSweepFish)) 
	{
		return false;
	}

	CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GM_CatchSweepFish,0, data, data_size);
	CUINotificationCenter::getInstance()->postNotification("game_fishMessage", pNotifRef);

	return true;
}

bool LrbyClientEngine::OnSubCatSweepFishResult(void* data, WORD data_size)
{
	if (data_size != sizeof(CMD_S_CatchSweepFishResult))
	{
			return false;
	}

	CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GM_CatchSweepFishResult,0, data, data_size);
	CUINotificationCenter::getInstance()->postNotification("game_fishMessage", pNotifRef);

	return true;
}

bool LrbyClientEngine::OnSubHitFishLK(void* data, WORD data_size)
{

	if (data_size != sizeof(CMD_S_HitFishLK))
	{
		return false;
	}

	CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GM_HitFishLK,0, data, data_size);
	CUINotificationCenter::getInstance()->postNotification("game_fishMessage", pNotifRef);

	return true;
}

bool LrbyClientEngine::OnSubSwitchScene(void* data, WORD data_size)
{
	if (data_size != sizeof(CMD_S_SwitchScene)) 
	{
		return false;
	}

	CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GM_SwitchScene,0, data, data_size);
	CUINotificationCenter::getInstance()->postNotification("game_fishMessage", pNotifRef);

	return true;
}

bool LrbyClientEngine::OnSubStockOperateResult(void* data, WORD data_size)
{


	if (data_size != sizeof(CMD_S_StockOperateResult)) 
	{
		return false;
	}
	CMD_S_StockOperateResult* stock_operate_result = static_cast<CMD_S_StockOperateResult*>(data);
	if (stock_operate_result->operate_code == 0)
	{
	} 
	else if (stock_operate_result->operate_code == 3) 
	{
	}
	 else
	{
	}
	return true;
}

bool LrbyClientEngine::OnSubSceneEnd(void* data, WORD data_size)
{
	g_LrbyGameConfig.setSwitchingScene(false);
	return true;
}

bool LrbyClientEngine::OnSubGameConfig(void* data, WORD data_size)
{
	int conmfi_size = sizeof(CMD_S_GameConfig);

	if (data_size != conmfi_size)//(CMD_S_GameConfig))
	{
		return false;
	}
	//设置游戏通用变量
	CMD_S_GameConfig* game_config = static_cast<CMD_S_GameConfig*>(data);
	g_LrbyGameConfig.setGameConfig(game_config);

	CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GM_CONFIG,game_config->min_bullet_multiple);
	CUINotificationCenter::getInstance()->postNotification("game_fishMessage", pNotifRef);

	m_pGameClientView->setGameConfigStatue(true);

	//if (!IsLookonMode())
	//verysharp测试代码，配置的时候上完所有分
	{
		//默认鱼币 全部买子弹
		CMD_C_ExchangeFishScore exchangeFishScore;
		memset(&exchangeFishScore, 0, sizeof(CMD_C_ExchangeFishScore));
		exchangeFishScore.increase = 2;		
		//verysharp //exchangeFishScore.iExchangeCount = 0;
		//发送消息
		//EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_EXCHANGE_FISHSCORE, &exchangeFishScore, sizeof(CMD_C_ExchangeFishScore));
	}
		
	return true;
}

bool LrbyClientEngine::OnSystemMessage(WORD wType, TCHART* strMessage)
{
	char szMsg[1024*3+LEN_NICKNAME*3]={0};
	//TChar2Utf8(strMessage,szMsg,sizeof(szMsg));
	MyNSString::wc2mbs(strMessage,sizeof(szMsg),szMsg);
	CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GM_MESSAGE, 0, szMsg, strlen(szMsg)+1);
	CUINotificationCenter::getInstance()->postNotification("game_fishMessage", pNotifRef);
	return true;
}

bool LrbyClientEngine::OnOnLineAward(void* data, WORD data_size)
{
	if (data_size != sizeof(CMD_S_OnLineAward)) return false;

	CUINotificationBuffer *pNotifRef = CUINotificationBuffer::create(em_GM_OnLineAward,0, data, data_size);
	CUINotificationCenter::getInstance()->postNotification("game_fishMessage", pNotifRef);

	return true;
}

bool LrbyClientEngine::OnSubTimeStamp(void* data, WORD data_size)
{
{
	if (data_size != sizeof(CMD_C_TimeStamp)) return false;
	CMD_C_TimeStamp* pRsp = static_cast<CMD_C_TimeStamp*>(data);
	if(!m_pGameClientView||m_pGameClientView->m_TimeStamp.tClientSendTime!=pRsp->tClientSendTime)
		return false;
	memcpy(&m_pGameClientView->m_TimeStamp,pRsp,sizeof(CMD_C_TimeStamp));
    m_pGameClientView->m_TimeStamp.tClientRecvTime=LrbyClientView::GetClientTickCount();
	m_pGameClientView->m_TimeStamp.AutoCalc();

	return true;
}
}

int LrbyClientEngine::switchViewChairID( unsigned int nChairID )
{
	tagUserData*    m_pMeUserDate=EntityMgr::instance()->roomFrame()->getMeUserItem()->getUserData();
	unsigned int nViewChairID = (nChairID + BY_GAME_PLAYER - m_pMeUserDate->wChairID);
	switch (BY_GAME_PLAYER)
	{
	case 2: { nViewChairID+=1; break; }
	case 3: { nViewChairID+=1; break; }
	case 4: { nViewChairID+=2; break; }
	case 5: { nViewChairID+=2; break; }
	case 6: { nViewChairID+=3; break; }
	case 7: { nViewChairID+=3; break; }
	case 8: { nViewChairID+=4; break; }
	}
	return nViewChairID % BY_GAME_PLAYER;
}

void LrbyClientEngine::onMessageGame(Object* obj)
{
	tagGameBuffer* tag = (tagGameBuffer*)obj;
	int Applicead = tag->nDataSize;
	//CCLog("tagGameBuffer = %d",Applicead);
	OnEventGameMessage(tag->wSubCmdID,tag->dateBuffer,tag->nDataSize);
	return;
}

void LrbyClientEngine::onMessageScene(Object* obj)
{
	tagSceneBuffer* tag = (tagSceneBuffer*)obj;
	bool bLookonUser = true;
	OnEventSceneMessage(tag->cbState,bLookonUser,tag->dateBuffer,tag->nDataSize);
	return;
}

void LrbyClientEngine::onUpdataMatch(Object* obj)		//this Messager is userd to change gear
{
	return;
}

void LrbyClientEngine::onViewMessage(Object* obj)
{
	return;
}

void LrbyClientEngine::onUpdateNotification()
{
	/*
	CUINotificationBufferPtr NotifRefPtr(dynamic_cast<CUINotificationBuffer *>(pRef));
	switch (NotifRefPtr->GetID())
	{
		case MP_Msg://滚动消息
			{

			}
		this->unschedule(SEL_SCHEDULE(&LobbyScene::MessageUpdate));
		lobbyNoticeList.push_back((char*)NotifRefPtr->GetData());
		((Label*)lobbyNoticeTxt)->setString(*lobbyNoticeList.begin());
		if(lobbyNoticeList.size()<5)
		{
			lobbyNoticeCount = 2;
			lobbyNoticeTime = 0.02f;
		}
		else if(lobbyNoticeList.size()<10)
		{
			lobbyNoticeCount = 1;
			lobbyNoticeTime = 0.02f;
		}
		else if(lobbyNoticeList.size()<15)
		{
			lobbyNoticeCount = 1;
			lobbyNoticeTime = 0.01f;
		}
		else
		{
			lobbyNoticeCount = 1;
			lobbyNoticeTime = 0.01f;
			lobbyNoticeList.erase((lobbyNoticeList.begin()++));
		}
		this->schedule(SEL_SCHEDULE(&LobbyScene::MessageUpdate),lobbyNoticeTime);
	}
	*/
}

//跟新桌子
void LrbyClientEngine::onUpdateTable(Object* obj)
{
	tagUserBuffer* tag = (tagUserBuffer*)obj;
	DWORD dwUserID = tag->dwUserID;

	CUserItem* pUserItem = (CUserItem*)EntityMgr::instance()->roomFrame()->getUserItem(dwUserID);
	if (!pUserItem)
	{
		return;
	}

	tagUserData* pUserDate = pUserItem->getUserData();
	if (!pUserDate)
	{
		return;
	}
	unsigned int nViewID = switchViewChairID(pUserDate->wChairID);
	//if(pUserDate->cbUserStatus<=US_SIT)return;
	//DDZmsgToView_UserInfor
	FishCCuserInfor* _userData =FishCCuserInfor::create(FishmsgToView_UserInfor);
	_userData->m_userInforID = pUserDate->dwUserID;
	_userData->m_ViewID = pUserDate->wChairID;
	_userData->m_ChairID = pUserDate->wChairID;
	_userData->m_score =pUserDate->lScore;
	_userData->m_gender =pUserDate->cbGender;
	_userData->m_GoldEggs=pUserDate->lGoldEggs;
	_userData->m_UserState = pUserDate->cbUserStatus;
	memcpy(_userData->m_PosName,pUserDate->szDwellingPlace,sizeof(char)*32);
	if (strlen(pUserDate->szNickName)>2)
	{
		memcpy(_userData->m_name,pUserDate->szNickName,sizeof(char)*32);
	}
	else
	{
		memcpy(_userData->m_name,pUserDate->szName,sizeof(char)*32);
	}
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_userData);
	

	//string szUserID = CGlobalMath::getUserFace(pUserItem->m_UserData.wFaceID,pUserItem->m_UserData.cbGender);
	//m_DDZSceneView->SetUserState(pUserDate->cbUserStatus,nChairID,szUserID, &pUserItem->m_UserData,dwUserID);
}
