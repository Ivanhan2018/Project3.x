#include "SceneLogic.h"
#include "BJLRes.h"
#include "GlobalFrame.h"
#include "CMD_GameServer.h"
#include "BJLSceneControl.h"
#include "TaskInfo.h"
#include "utf-8.h"
#include "UpBankerList.h"
#include "GlobalUnits.h"
#include "ConfigMgr.h"
#include "CMD_BJL.h"
#include "MyNSString.h"
#include "MyConfig.h"
#include "MovingLabelLayer.h"

//是否在场景中央显示空闲时间、下注时间、开牌时间
//#define USE_GAMETIME

int compareCardValue(const void *a, const void *b)
{
	BYTE valuaA = *(BYTE *)a;
	BYTE valuaB = *(BYTE *)b;
	return SceneLogic::GetCardLogicValue(valuaB) - SceneLogic::GetCardLogicValue(valuaA);
}

bool SceneLogic::init()
{
	bool ret=false;
	do 
	{
		size = WINSIZE;

		//--手牌
		for(int i=0 ;i<GAME_PLAYER;i++)
		{
			memset(m_handCardData[i],0,sizeof(BYTE)*MAX_COUNT);
			m_handCardNum[i]=0;
			m_BackCardData[i]=0;
			memset(m_outCardData,0,sizeof(BYTE)*MAX_COUNT);
			m_outCardNum=0;
			m_wOpenUser[i] =false;
		}
		m_gameState=1;//1没开始
		m_isOut =false;
		m_beilv = 1;
		m_baseScore=0;
		for(int i =0;i<8;i++)
		{
			m_lBetScore[i] = 0;  //游戏结束重新初始化为0
		}
		NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SceneLogic::onUserStart),"GameStart" , NULL);
		NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneLogic::onUpdateTable),MSG_UI_ANS_PLAYERUPDATE,NULL);
		NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneLogic::onMessageGame),GBEVENT_UI_GAMEMESSAGE,NULL);
		NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneLogic::onMessageScene),GBEVENT_UI_SCENEMESSAGE,NULL);
		NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneLogic::onUpdataMatch),MSG_UI_ANS_MATCH,NULL);
		NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneLogic::onViewMessage),MESSAGE_TOLOGIC,NULL);
		NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SceneLogic::onLinkResult),MSG_UI_ANS_GAMELINK , NULL);				//连接服务器
		NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SceneLogic::onUpdateScoreAndGoldEgg),MSG_UI_ANS_UPDATESCOREANDGOLDEGG , NULL);			//--充值刷新金币

		m_chipManager = ChipManager::create();
		this->addChild(m_chipManager);

		m_cardTableLayer = CardTableLayer::create();  //头像  信息
		this->addChild(m_cardTableLayer,0);

		operlayer =  OperaLayer::create();
		this->addChild(operlayer,0);

		m_pCardManager = CardManager::create();
		this->addChild(m_pCardManager,1);

		MyConfig& myConfig=MyConfig::Instance();

		SpriteGameStare = Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_CallScoreTip));
		SpriteGameStare->setAnchorPoint(Vec2::ZERO);
		SpriteGameStare->setPosition(ccp(myConfig.m_CallScoreTipPos[0],myConfig.m_CallScoreTipPos[1]));
		SpriteGameStare->setVisible(false);
		this->addChild(SpriteGameStare);
		SpriteGameTime = Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_GameState0));
		SpriteGameTime->setAnchorPoint(Vec2::ZERO);
		setGameStateTip(0);
		SpriteGameTime->setVisible(false);
		this->addChild(SpriteGameTime);

		SpriteChangeBanker = Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_ChangeBankerTip1));
		SpriteChangeBanker->setAnchorPoint(Vec2::ZERO);
		SpriteChangeBanker->setPosition(ccp(myConfig.m_ChangeBankerTipPos[0],myConfig.m_ChangeBankerTipPos[1]));
		SpriteChangeBanker->setVisible(false);
		this->addChild(SpriteChangeBanker);
		m_ChangeBanker=0;
		ret = true;

	} while (0);
	return ret;
}


void SceneLogic::onEnter()
{
	pMsgDispatch->setGameStatus(true);
	Node::onEnter();
	//游戏场景消息
}

void SceneLogic::onExit()
{
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_PLAYERUPDATE);
	NotificationCenter::getInstance()->removeObserver(this,GBEVENT_UI_GAMEMESSAGE);
	NotificationCenter::getInstance()->removeObserver(this,GBEVENT_UI_SCENEMESSAGE);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_MATCH);
	NotificationCenter::getInstance()->removeObserver(this,MESSAGE_TOLOGIC);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_GAMELINK);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_UPDATESCOREANDGOLDEGG);
	Node::onExit();
}

void SceneLogic::checkNet(float ft)
{
	static int NetData=0;
	if (pMsgDispatch->isGameLinkOK()) 
	{
		NetData=0;
		unschedule(schedule_selector(SceneLogic::checkNet));
		schedule(schedule_selector(SceneLogic::checkNet), 8);
	}else{
		NetData++;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,NetData>2?(NetData=0,Integer::create(msgToView_socketoff)):Integer::create(msgToView_socketoffConnect));
		unschedule(schedule_selector(SceneLogic::checkNet));
		schedule(schedule_selector(SceneLogic::checkNet), 4);
	}
}

int SceneLogic::switchViewChairID( unsigned int nChairID )
{
	tagUserData*    m_pMeUserDate=EntityMgr::instance()->roomFrame()->getMeUserItem()->getUserData();
	unsigned int nViewChairID = (nChairID + GAME_PLAYER - m_pMeUserDate->wChairID);
	switch (GAME_PLAYER)
	{
	case 2: { nViewChairID+=1; break; }
	case 3: { nViewChairID+=1; break; }
	case 4: { nViewChairID+=2; break; }
	case 5: { nViewChairID+=2; break; }
	case 6: { nViewChairID+=3; break; }
	case 7: { nViewChairID+=3; break; }
	case 8: { nViewChairID+=4; break; }
	}
	return nViewChairID % GAME_PLAYER;
}

void SceneLogic::sendSeverData( WORD wSubCmdID )
{
	if (pMsgDispatch->isGameLinkOK()) {
		pMsgDispatch->sendSocketDate(wSubCmdID);
	}else{
	}
}

void SceneLogic::sendSeverData( WORD wSubCmdID,void* pData,WORD wDataSize )
{
	if (pMsgDispatch->isGameLinkOK()) {
		pMsgDispatch->sendSocketDate(wSubCmdID,pData,wDataSize);
	}else{
	}
}

void SceneLogic::sendFrameData( WORD wSubCmdID,void* pData,WORD wDataSize )
{
	if (pMsgDispatch->isGameLinkOK()) {
		pMsgDispatch->sendFrameDate(wSubCmdID,pData,wDataSize);
	}else{

	}
}

void SceneLogic::setGameStateTip(int _state){
	if(_state<0||_state>2)
		return;
	MyConfig& myConfig=MyConfig::Instance();
	const char *szArr[3]={texture_name::s_GameState0,texture_name::s_GameState1,texture_name::s_GameState2};
	float *pPos[3]={&myConfig.m_GameFreeTipPos[0],&myConfig.m_GameStartTipPos[0],&myConfig.m_GameEndTipPos[0]};
	SpriteGameTime->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(szArr[_state]));
	SpriteGameTime->setPosition(ccp(pPos[_state][0],pPos[_state][1]));
}

void SceneLogic::setCallScoreTip(bool isMeBanker)
{
	m_isMeBanker=isMeBanker;
	SpriteGameStare->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(isMeBanker?texture_name::s_BankerCallScoreTip:texture_name::s_CallScoreTip));
}

void SceneLogic::setChangeBankerTip(int j)
{
	if(m_ChangeBanker==1 && m_ChangeBanker!=j)
	{
	    // 自己下庄了
		NotificationCenter::getInstance()->postNotification("OnMeDownBanker");
	}
	SpriteChangeBanker->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(j==1?texture_name::s_ChangeBankerTip1:(j==2?texture_name::s_ChangeBankerTip2:texture_name::s_ChangeBankerTip3)));
	SpriteChangeBanker->setVisible(true);
	m_ChangeBanker=j;
	scheduleOnce(schedule_selector(SceneLogic::ChangeBankerTip_callback_once),1);
}

void SceneLogic::ChangeBankerTip_callback_once(float t)
{
	SpriteChangeBanker->setVisible(false);
}

//游戏消息
void SceneLogic::onMessageGame(Object* obj)
{
	tagGameBuffer* tag = (tagGameBuffer*)obj;
	onGameMessage(tag->wSubCmdID,tag->dateBuffer,tag->nDataSize);
}

//游戏场景消息
void SceneLogic::onMessageScene(Object* obj)
{
	tagSceneBuffer* tag = (tagSceneBuffer*)obj;
	onSceneMessage(tag->cbState,tag->dateBuffer,tag->nDataSize);
}

void SceneLogic::onUpdataMatch( Object* obj )
{
}

//更新桌子
void SceneLogic::onUpdateTable(Object* obj)
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
	unsigned int nChairID = switchViewChairID(pUserDate->wChairID);
}

void SceneLogic::onViewMessage( Object* obj )
{
	Integer* _int=(Integer*)obj;
	switch(_int->getValue())
	{

	}
}

bool SceneLogic::onSceneMessage( unsigned char cbStation,const void* pBuffer,WORD wDataSize )
{
	switch(cbStation)
	{
	case GAME_SCENE_FREE:               //空闲状态 0
		{
			CCLOG("GAME_SCENE_FREE 0");
			return OnGameSceneFree(pBuffer,wDataSize);
		}
	case GAME_SCENE_PLAY:			//游戏状态 101
		{
			CCLOG("GAME_SCENE_PLAY 101");
			return OnGameScenePlay(pBuffer,wDataSize);
		}
	default:
		{
			return true;
		}
	}
	return  true;
}
bool SceneLogic::onGameMessage( WORD wSubCmdID, const void* pBuffer, WORD wDataSize )
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:		//游戏空闲
		{
			CCLOG("SUB_S_GAME_FREE 99");
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//游戏开始
		{
			CCLOG("SUB_S_GAME_START 100");
			return onSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON:		//用户下注
		{
			CCLOG("SUB_S_PLACE_JETTON 101");
			return onSubLandScore(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER:		//申请庄家
		{
			CCLOG("SUB_S_APPLY_BANKER 103");
			return onSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_CANCEL_BANKER:	//取消做庄
		{
			CCLOG("SUB_S_CANCEL_BANKER 108");
			return OnSubUserCancelBanker(pBuffer, wDataSize);
		}
	case SUB_S_CHANGE_BANKER:		//切换庄家
		{
			CCLOG("SUB_S_CHANGE_BANKER 104");
			return onSubPassCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			CCLOG("SUB_S_GAME_END 102");
			return onSubGameStart(pBuffer,wDataSize);
		}

	case SUB_S_PLACE_JETTON_FAIL:	//下注失败(忽略)
		{
			CCLOG("SUB_S_PLACE_JETTON_FAIL 107");
			return onSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_CHANGE_USER_SCORE:	//更新积分
		{
			CCLOG("SUB_S_CHANGE_USER_SCORE 105");
			return true;
		}
	case SUB_S_SEND_RECORD:		//游戏记录	
		{
			CCLOG("SUB_S_SEND_RECORD 106");
			return OnSubCheatCard(pBuffer,wDataSize);
		}
	case SUB_S_AMDIN_COMMAND:   //管理员命令申请 请求
		{
			CCLOG("SUB_S_AMDIN_COMMAND 109");
			return onUserExpression(pBuffer,wDataSize);
		}
	case SUB_S_TIP_INFO:   //提示消息
		{
			CCLOG("SUB_S_TIP_INFO 110");
			return onSubTipInfo(pBuffer,wDataSize);
		}
	}

	return true;
}
void SceneLogic::onUserStart(Object *pSender)
{

}

bool SceneLogic::onUserStartView( const void * pBuffer, WORD wDataSize )
{
	return true;
}

bool SceneLogic::onUserBEeiLv( const void * pBuffer, WORD wDataSize )
{
	return true;
}

bool SceneLogic::onUserAdd( const void * pBuffer, WORD wDataSize )
{
	if (wDataSize!= sizeof(CMD_S_CallScore))
	{
		CCLOG("发牌数据校验错误！");
		return false;
	}

	return true;
}

bool SceneLogic::onSubOpenCard( const void * pBuffer, WORD wDataSize )
{
	return true;
}
//游戏空闲
bool SceneLogic::OnSubGameFree( const void *pBuffer, WORD wDataSize )
{
	CCLOG("%d",wDataSize);
	CCLOG("%d",sizeof(CMD_S_GameFree));
	//效验大小 
	assert(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;
	CMD_S_GameFree* _data =(CMD_S_GameFree*)pBuffer;
	int bTimeFree =_data->cbTimeLeave-2;
	m_cardTableLayer->waitingMatching(false);
	m_pCardManager->remaggerDissGameOver();
	m_pCardManager->resetManager();
	m_chipManager->dissRemoveChuoValue();
	m_chipManager->initGetChipManager();
#ifdef USE_GAMETIME
	SpriteGameTime->setVisible(true);
	setGameStateTip(0);
#endif
	operlayer->dissUnclickTouch();
	operlayer->resetManagerOper();
	NotificationCenter::getInstance()->postNotification("Strameobjet", Integer::create(bTimeFree));
	NotificationCenter::getInstance()->postNotification("MI_setCCteefTag");
	SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_time_warimg);

	return true;
}

bool SceneLogic::OnGameSceneFree(const void *pBuffer, WORD wDataSize)
{
	CCLOG("wDataSize = %d,sizeof(CMD_S_StatusFree) = %d",wDataSize,sizeof(CMD_S_StatusFree));
	//wDataSize = 176,sizeof(CMD_S_StatusFree) = 176
	//效验大小 
	assert(wDataSize==sizeof(CMD_S_StatusFree));
	if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

	CCLOG("－－－－小游戏－－－空闲状态－－－－－－－－");
	CMD_S_StatusFree* pStatusPlay=(CMD_S_StatusFree *)pBuffer;
	//空闲时间
	BYTE Tinme= pStatusPlay->cbTimeLeave -4;
	WORD wBankerUser = pStatusPlay->wBankerUser;
	CUserItem* pUserItem = (CUserItem*)EntityMgr::instance()->roomFrame()->getUserItem(pStatusPlay->wBankerUser);

	CCLOG("GAME_SCENE_FREE=0 wBankerUser=%d",wBankerUser);
	// 加上指针非空判断，避免崩溃
#ifdef USE_GAMETIME
	if(SpriteGameTime){
		SpriteGameTime->setVisible(true);
		setGameStateTip(0);
	}
#endif
	NotificationCenter::getInstance()->postNotification("StrameNumber", Integer::create(wBankerUser));
	NotificationCenter::getInstance()->postNotification("Strameobjet", Integer::create(Tinme));
	NotificationCenter::getInstance()->postNotification("MI_setCCteefTag");
	m_cardTableLayer->waitingMatching(false);
	operlayer->dissUnclickTouch();
    m_cardTableLayer->setUserIsOnline(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore,pStatusPlay->lBankerWinScore,pStatusPlay->wBankerTime);

	return true;
}

bool SceneLogic::OnGameScenePlay(const void *pBuffer, WORD wDataSize)
{
	CCLOG("wDataSize = %d,sizeof(CMD_S_StatusPlay) = %d",wDataSize,sizeof(CMD_S_StatusPlay));
	//效验数据
	if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
	CMD_S_StatusPlay* pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

	WORD wBankerUser = pStatusPlay->wBankerUser;	//当前庄家
	CCLOG("GAME_SCENE_PLAY=101 wBankerUser=%d",wBankerUser);
	return true;
}

//游戏开始
bool SceneLogic::onSubSendCard( const void * pBuffer, WORD wDataSize )
{
	CCLOG("wDataSize = %d,sizeof(CMD_S_GameStart) = %d",wDataSize,sizeof(CMD_S_GameStart));

	//assert(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	//CCLOG("CMD_S_GameStart 1");
	//玩家信息
	m_lPlayBetScore = pGameStart->lPlayBetScore;
	//CCLOG("CMD_S_GameStart 2");
	m_lPlayFreeSocre = pGameStart->lPlayFreeSocre;
	//CCLOG("CMD_S_GameStart 3");
	m_wBankerUser = pGameStart->wBankerUser;
	//CCLOG("CMD_S_GameStart 4");
	m_lBankerScore = pGameStart->lBankerScore;
	//CCLOG("CMD_S_GameStart 5");
	int rejsbvkfsd  = pGameStart->cbTimeLeave-2;

	CCLOG("wBankerUser = %d,lBankerScore=%lld",pGameStart->wBankerUser,pGameStart->lBankerScore);

	//设置时间,标志
	NotificationCenter::getInstance()->postNotification("Strameobjet", Integer::create(rejsbvkfsd));
	NotificationCenter::getInstance()->postNotification("MI_setPlaceScore");
#ifdef USE_GAMETIME
	setGameStateTip(1);
#endif
	bool isMeBanker=m_cardTableLayer->setBankerName(pGameStart->wBankerUser);
	m_cardTableLayer->setBankerScore(pGameStart->lBankerScore);
	setCallScoreTip(isMeBanker);
	SpriteGameStare->setVisible(true);
	if(!isMeBanker){
		operlayer->initClickTouch();
	}
	SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_time_warimg);

	return true;
}
//用户下注
bool SceneLogic::onSubLandScore( const void * pBuffer, WORD wDataSize )
{
	CCLOG("%d",wDataSize);
	CCLOG("%d",sizeof(CMD_S_PlaceBet));
	//效验大小 
	assert(wDataSize==sizeof(CMD_S_PlaceBet));
	if (wDataSize!=sizeof(CMD_S_PlaceBet)) return false;
	CMD_S_PlaceBet * pPlaceBet=(CMD_S_PlaceBet *)pBuffer;
	m_lBetScore[pPlaceBet->cbBetArea] += pPlaceBet->lBetScore;

	WORD meChairID=pMsgDispatch->m_wChairID;
	CCLOG("CMD_S_PlaceBet meChairID=%d,wChairID=%d,cbBetArea=%d[%s],lBetScore=%lld,cbAndroidUser=%d",meChairID,pPlaceBet->wChairID,pPlaceBet->cbBetArea,BET_AREA_NAME(pPlaceBet->cbBetArea),pPlaceBet->lBetScore,pPlaceBet->cbAndroidUser);
	if(meChairID==pPlaceBet->wChairID||true)
	{
		m_chipManager->showGetChipManager(pPlaceBet->cbBetArea,m_lBetScore[pPlaceBet->cbBetArea]);
		m_chipManager->showChipManagerSprite(pPlaceBet->cbBetArea,pPlaceBet->lBetScore);
	}
	SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_add_gold);

	return true; 
}

//游戏结束
bool SceneLogic::onSubGameStart( const void * pBuffer, WORD wDataSize )
{
	////消息处理 
	CCLOG("%d",wDataSize);
	CCLOG("%d",sizeof(CMD_S_GameEnd));
	//效验大小 
	assert(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	BYTE cbTimeLeaveGame =pGameEnd->cbTimeLeave-1;
	NotificationCenter::getInstance()->postNotification("Strameobjet", Integer::create(cbTimeLeaveGame));
	NotificationCenter::getInstance()->postNotification("MI_setCardTalayer");
#ifdef USE_GAMETIME
	setGameStateTip(2);
#endif
	//更新庄家 玩家信息
	LONGLONG lBankerScoregame = pGameEnd->lBankerScore;					//庄家成绩
	LONGLONG lBankerTotallScoregame= pGameEnd->lBankerTotallScore;		//庄家成绩
	int		nBankerTimeGame = pGameEnd->nBankerTime ;
	LONGLONG lPlayScoreGAMER[AREA_MAX]; //做庄次数
	LONGLONG lPlayAllScoreGame =pGameEnd->lPlayAllScore;						//玩家成绩
	m_cardTableLayer->setBankUsedChengjiORrJuNumber(pGameEnd->lBankerTotallScore,pGameEnd->nBankerTime);
	m_cardTableLayer->setPlayAllScoreORchengji(pGameEnd->lPlayAllScore);
	operlayer->dissUnclickTouch();
    if (operlayer->IsXIazhuNumber){
		m_pCardManager->showGameOver();
		for (int i =0;i<AREA_MAX;i++){
			lPlayScoreGAMER[i] = pGameEnd->lPlayScore[i];
			if (pGameEnd->lPlayScore[i] != 0){
				m_pCardManager->SetScoreGameOver(pGameEnd->lPlayScore[i],i,pGameEnd->lPlayAllScore);}}
	}
	m_cbCardCount[0] = 	pGameEnd->cbTableCardArray[0][0];
	m_cbCardCount[1] = 	pGameEnd->cbTableCardArray[0][1];
	m_cbCardCount[2] = 	pGameEnd->cbTableCardArray[0][2];
	m_cbCardCount1[0] = pGameEnd->cbTableCardArray[1][0];
	m_cbCardCount1[1] = pGameEnd->cbTableCardArray[1][1];
	m_cbCardCount1[2] = pGameEnd->cbTableCardArray[1][2];

	//将手牌传入牌类
	m_pCardManager->SetHandCardData(m_cbCardCount); 
	m_pCardManager->SetHandCardData1(m_cbCardCount1); 
	m_pCardManager->showPlayerCard();
	m_pCardManager->showPlayerCard1(pGameEnd->cbTableCardArray,pGameEnd->cbCardCount);
	//全局信息
	LONGLONG lRevenueGame =pGameEnd->lRevenue;							//游戏税收
	m_OperaLayer->remaggerdisflase();
	operlayer->dissUnclickTouch();
	SpriteGameStare->setVisible(false);
	for(int i =0;i<8;i++)
	{
		m_lBetScore[i] = 0;  //游戏结束重新初始化为0
	}
	SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_time_warimg);

	return true;
}
//申请庄家
bool SceneLogic::onSubOutCard( const void * pBuffer, WORD wDataSize )
{
	CCLOG("wDataSize=%d,sizeof(CMD_S_ApplyBanker)=%d",wDataSize,sizeof(CMD_S_ApplyBanker));
	//效验大小 
	assert(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;
	CMD_S_ApplyBanker * pOutCard=(CMD_S_ApplyBanker *)pBuffer;
	WORD meChairID=pMsgDispatch->m_wChairID;
	if(pOutCard->wApplyUser==INVALID_CHAIR)
	{
		CCLOG("CMD_S_ApplyBanker wApplyUser==INVALID_CHAIR");
	}
	else
	{
		if(meChairID==pOutCard->wApplyUser)
		{
			// 自己申请上庄成功
			NotificationCenter::getInstance()->postNotification("OnMeApplyBankerSuc");
		}
		else
		{
			// 别人申请上庄成功
			CCLOG("CMD_S_ApplyBanker wApplyUser!=meChairID");
		}
		NotificationCenter::getInstance()->postNotification("OnBankerListAdd", Integer::create(pOutCard->wApplyUser));
	}
	return true;
}
//切换庄家
bool SceneLogic::onSubPassCard( const void * pBuffer, WORD wDataSize )
{
	CCLOG("wDataSize=%d,sizeof(CMD_S_ChangeBanker)=%d",wDataSize,sizeof(CMD_S_ChangeBanker));
	//效验大小 
	assert(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;
	CMD_S_ChangeBanker* pPassCard=(CMD_S_ChangeBanker *)pBuffer;
	WORD meChairID=pMsgDispatch->m_wChairID;
	if(pPassCard->wBankerUser==INVALID_CHAIR)
	{
		// 无人坐庄
		setChangeBankerTip(3);
	}
	else
	{
		if(meChairID==pPassCard->wBankerUser)
		{
			// 由您坐庄
			setChangeBankerTip(1);
			NotificationCenter::getInstance()->postNotification("OnMeUpBankerSuc");
		}
		else
		{
			// 切换庄家
			setChangeBankerTip(2);
		}
	}
	// 如果在上庄列表，删除之
	NotificationCenter::getInstance()->postNotification("OnBankerListDelete", Integer::create(pPassCard->wBankerUser));

	m_cardTableLayer->setBankerName(pPassCard->wBankerUser);
	m_cardTableLayer->setBankerScore(pPassCard->lBankerScore);
	//切换庄家的时候需要重新将成绩和局数初始化
	m_cardTableLayer->setBankUsedChengjiORrJuNumber(0,0);
	return true;
}
//下注失败
bool SceneLogic::onSubGameEnd( const void * pBuffer, WORD wDataSize )
{
	////消息处理
	CCLOG("%d",wDataSize);
	CCLOG("%d",sizeof(CMD_S_PlaceBetFail));
	//效验大小 
	assert(wDataSize==sizeof(CMD_S_PlaceBetFail));
	if (wDataSize!=sizeof(CMD_S_PlaceBetFail)) return false;
	CMD_S_PlaceBetFail * pPlaceBetFail=(CMD_S_PlaceBetFail *)pBuffer;
	return true;
}
//申请 请求
bool SceneLogic::onUserExpression( const void * pBuffer, WORD wDataSize )
{
	//消息处理
	CCLOG("%d",wDataSize);
	CCLOG("%d",sizeof(CMD_S_CommandResult));
	//效验大小 
	CMD_S_CommandResult* _data =(CMD_S_CommandResult*)pBuffer;

	return true;
}

//提示消息
bool SceneLogic::onSubTipInfo( const void * pBuffer, WORD wDataSize )
{
	//消息处理
	CCLOG("wDataSize=%d,sizeof(CMD_S_TipInfo)",wDataSize,sizeof(CMD_S_TipInfo));
	CMD_S_TipInfo* _data =(CMD_S_TipInfo*)pBuffer;
	int DataSize=wcslen((wchar_t *)_data->szTipInfo)*2+2;
	char str[1000]={0};
	MyNSString::wc2mbs(_data->szTipInfo,DataSize,str);
    MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(str, ccp(SCREEN_HEIGHT/2,SCREEN_WIDTH/2));
    this->addChild(layer,20);
	return true;
}

void SceneLogic::showMoveingLabelTips(const char* str)
{
    MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(str,ccp(size.width * 0.5,size.height * 0.5));
    this->addChild(layer,100);
}

//取消做庄
bool SceneLogic::OnSubUserCancelBanker( const void * pBuffer, WORD wDataSize )
{
	//消息处理
	CCLOG("wDataSize=%d,sizeof(CMD_S_CancelBanker)",wDataSize,sizeof(CMD_S_CancelBanker));
	CMD_S_CancelBanker* _data =(CMD_S_CancelBanker*)pBuffer;
	WORD meChairID=pMsgDispatch->m_wChairID;
	if(meChairID==_data->wCancelUser)
	{
		// 自己取消了申请
		NotificationCenter::getInstance()->postNotification("OnMeCancelUpBanker");
	}
	else
	{
		// 别人取消了申请
		int a=0;

	}
	// 如果在上庄列表，删除之
	NotificationCenter::getInstance()->postNotification("OnBankerListDelete", Integer::create(_data->wCancelUser));

	return true;
}
//游戏记录
bool SceneLogic::OnSubCheatCard( const void * pBuffer, WORD wDataSize )
{
	//消息处理
	CCLOG("SceneLogic::OnSubCheatCard wDataSize=%d,sizeof(tagServerGameRecord)=%d",wDataSize,sizeof(tagServerGameRecord));

	WORD temp  = wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex= 0; wIndex<temp; wIndex++)
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);
		BYTE m_cbKingWinner = pServerGameRecord->cbKingWinner;						//天王赢家
		bool m_bPlayerTwoPair =pServerGameRecord->bPlayerTwoPair;						//对子标识
		bool m_bBankerTwoPair = pServerGameRecord->bBankerTwoPair;						//对子标识
		BYTE m_cbPlayerCount =pServerGameRecord->cbPlayerCount;						//闲家点数
		BYTE m_cbBankerCount = pServerGameRecord->cbBankerCount;						//庄家点数
		m_cardTableLayer->initScrollView(m_cbPlayerCount,m_cbBankerCount);
		CCLOG("wIndex=%d,temp=%d,m_cbPlayerCount=%d,m_cbBankerCount=%d",wIndex,temp,m_cbPlayerCount,m_cbBankerCount);
	}
	return true;
}
void SceneLogic::onLinkResult( Object* obj )
{
	Integer* Interger = (Integer*)obj;

	switch (Interger->getValue())
	{
	case 0:			// 0为连接失败 
		break;
	case 1:			// 1为连接成功
		{
			BJLSceneControl* pSceneControl = BJLSceneControl::sharedSceneControl();
			Scene* pScene = pSceneControl->getScene(SCENE_game,true);
			Director::getInstance()->replaceScene(pScene);
		}
		break;
	case 2:			// 2为没有服务器
		{
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,Integer::create(msgToView_socketoff));
		}
		break;
	default:
		break;
	}
}

void SceneLogic::onUpdateScoreAndGoldEgg( Object* obj )
{
	sendSeverData(SUB_GF_ROOM_SCOREUPDATA, 0, 0);
}

int SceneLogic::getUserRealChairID(DWORD userID)
{
	CUserItem* pUserItem = (CUserItem*)EntityMgr::instance()->roomFrame()->getUserItem(userID);
	if (!pUserItem)
	{
		return 1;
	}

	tagUserData* pUserDate = pUserItem->getUserData();
	if (!pUserDate)
	{
		return 1;
	}
	int nChairID = switchViewChairID(pUserDate->wChairID);
	return nChairID;
}
//获取用户信息
void SceneLogic::userInforDeal( Integer* _data )
{
	CCLOG("grdggwegweg");
}






