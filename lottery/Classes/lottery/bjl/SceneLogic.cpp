#include "SceneLogic.h"
#include "DDZRes.h"
#include "GlobalFrame.h"
#include "CMD_Game.h"
#include "CMD_GameServer.h"
#include "CMD_Commom.h"
#include "BJLSceneControl.h"
#include "TaskInfo.h"
#include "utf-8.h"
#include "GameTaskCommon.h"
#include "GlobalUnits.h"
#include "ConfigMgr.h"
#include "CMD_DDZClient.h"
#include "MyNSString.h"
#include "MyConfig.h"//by hxh
#include "MovingLabelLayer.h"//by hxh
#include "CCInteger64.h"//by hxh
#include "SoundControl.h"

//是否在场景中央显示空闲时间、下注时间、开牌时间<by hxh>
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
		size = Director::getInstance()->getWinSize();

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
		m_eGameState = EBJLGameState_Null;
		m_isOut =false;
		m_beilv = 1;
		m_baseScore=0;
		for(int i =0;i<8;i++)
		{
			m_lBetScore[i] = 0;  //游戏结束重新初始化为0
		}
		NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SceneLogic::AddGameRecord), "AddGameRecord", NULL);//<by hxh>
		NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SceneLogic::onUserStart),"GameStart" , NULL);
		NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneLogic::onUpdateTable),MSG_UI_ANS_PLAYERUPDATE,NULL);
		NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneLogic::onMessageGame),GBEVENT_UI_GAMEMESSAGE,NULL);
		NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneLogic::onMessageScene),GBEVENT_UI_SCENEMESSAGE,NULL);
		NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneLogic::onUpdataMatch),MSG_UI_ANS_MATCH,NULL);
		NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(SceneLogic::onViewMessage),MESSAGE_TOLOGIC,NULL);
		NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SceneLogic::onLinkResult),MSG_UI_ANS_GAMELINK , NULL);				//连接服务器
		NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(SceneLogic::onUpdateScoreAndGoldEgg),MSG_UI_ANS_UPDATESCOREANDGOLDEGG , NULL);			//--充值刷新金币
		//this->schedule(schedule_selector(SceneLogic::checkNet), 8);

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
		//SpriteGameStare = Sprite::create("bjl/BJL/BJL_04.png");
		SpriteGameStare->setAnchorPoint(Vec2::ZERO);
		SpriteGameStare->setPosition(ccp(myConfig.m_CallScoreTipPos[0],myConfig.m_CallScoreTipPos[1]));
		SpriteGameStare->setVisible(false);
		this->addChild(SpriteGameStare);
		SpriteGameTime = Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_GameState0));
		SpriteGameTime->setAnchorPoint(Vec2::ZERO);
		//SpriteGameTime->setPosition(ccp(size.width*0.45,size.height*0.58));
		setGameStateTip(0);
		SpriteGameTime->setVisible(false);
		this->addChild(SpriteGameTime);

		// add by hxh 20160720
#if 1
		SpriteChangeBanker = Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_ChangeBankerTip1));
		SpriteChangeBanker->setAnchorPoint(Vec2::ZERO);
		SpriteChangeBanker->setPosition(ccp(myConfig.m_ChangeBankerTipPos[0],myConfig.m_ChangeBankerTipPos[1]));
		SpriteChangeBanker->setVisible(false);
		this->addChild(SpriteChangeBanker);
		m_ChangeBanker=0;
#endif
		ret = true;

	} while (0);
	return ret;
}


void SceneLogic::onEnter()
{
	EntityMgr::instance()->getDispatch()->setGameStatus(true);
	Node::onEnter();
	//游戏场景消息
}

void SceneLogic::onExit()
{
	NotificationCenter::getInstance()->removeObserver(this, "AddGameRecord");//<by hxh>
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
	if (EntityMgr::instance()->getDispatch()->isGameLinkOK()) 
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
	if (EntityMgr::instance()->getDispatch()->isGameLinkOK()) {
		EntityMgr::instance()->getDispatch()->sendSocketDate(wSubCmdID);
	}else{
		//?????????????????????
		//sendViewMessage(LOGIC_SOCKET_OFF);
	}
}

void SceneLogic::sendSeverData( WORD wSubCmdID,void* pData,WORD wDataSize )
{
	if (EntityMgr::instance()->getDispatch()->isGameLinkOK()) {
		EntityMgr::instance()->getDispatch()->sendSocketDate(wSubCmdID,pData,wDataSize);
	}else{
		//?????????????????????
		//sendViewMessage(LOGIC_SOCKET_OFF);
	}
}

void SceneLogic::sendFrameData( WORD wSubCmdID,void* pData,WORD wDataSize )
{
	if (EntityMgr::instance()->getDispatch()->isGameLinkOK()) {
		EntityMgr::instance()->getDispatch()->sendFrameDate(wSubCmdID,pData,wDataSize);
	}else{

	}
}

void SceneLogic::setGameStateTip(int _state){
#if 1
	if(_state<0||_state>2)
		return;
	MyConfig& myConfig=MyConfig::Instance();
	const char *szArr[3]={texture_name::s_GameState0,texture_name::s_GameState1,texture_name::s_GameState2};
	float *pPos[3]={&myConfig.m_GameFreeTipPos[0],&myConfig.m_GameStartTipPos[0],&myConfig.m_GameEndTipPos[0]};
	SpriteGameTime->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(szArr[_state]));
	SpriteGameTime->setPosition(ccp(pPos[_state][0],pPos[_state][1]));
#else
	if(_state==0){
		SpriteGameTime->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_GameState0));
		MyConfig& myConfig=MyConfig::Instance();
		SpriteGameTime->setPosition(ccp(myConfig.m_GameFreeTipPos[0],myConfig.m_GameFreeTipPos[1]));
		return;
	}
	if(_state==1){
		SpriteGameTime->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_GameState1));
		MyConfig& myConfig=MyConfig::Instance();
		SpriteGameTime->setPosition(ccp(myConfig.m_GameStartTipPos[0],myConfig.m_GameStartTipPos[1]));
		return;
	}
	if(_state==2){
		SpriteGameTime->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_GameState2));
		MyConfig& myConfig=MyConfig::Instance();
		SpriteGameTime->setPosition(ccp(myConfig.m_GameEndTipPos[0],myConfig.m_GameEndTipPos[1]));
		return;
	}
#endif
}

//返回游戏状态：空闲、下注、结果
EBJLGameState	SceneLogic::GetGameState()
{
	return m_eGameState;
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
	//MatchError* match = MatchError::MatchErrorWith();
	//this->addChild(match,101);
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

	DWORD dwSelfUserID = EntityMgr::instance()->roomFrame()->getMeUserDate()->dwUserID;
	if (pUserDate->dwUserID == dwSelfUserID)
	{
		if (pUserDate->lScore <= 0)
		{
			cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
			Scene* pScene = Director::getInstance()->getRunningScene();
			PromptBox* box = PromptBox::PromptBoxWith(Vec2(sVSize.width*0.5,sVSize.height * 0.5), mPromptTypeGetError);
			box->setPromptText(ConfigMgr::instance()->text("t7"));
			pScene->addChild(box);
		}
	}
}

void SceneLogic::onViewMessage( Object* obj )
{
	Integer* _int=(Integer*)obj;
	switch(_int->getValue())
	{
	case msgToLogic_changeTable:
		{
			Director::getInstance()->replaceScene(BJLSceneControl::sharedSceneControl()->getScene(SCENE_game,true));
			EntityMgr::instance()->getDispatch()->SendPacketWithChangeTable();
		}
		break;
	case msgToLogic_openStart:
		{
			EntityMgr::instance()->getDispatch()->SendPacketWithUserStatus(US_READY);
		}
		break;
	case magToLogc_gameStart:
		{
			EntityMgr::instance()->getDispatch()->SendPacketWithUserStatus(US_READY);
		}
		break;
	case msgToLogic_outCard:
		{
			//CCsendCard* _data=(CCsendCard* )_int;

			//BYTE cbBuffer[sizeof(CMD_C_OutCard)];
			//memset(cbBuffer, 0 , sizeof(CMD_C_OutCard));
			//CMD_C_OutCard *OutCard = (CMD_C_OutCard *)cbBuffer;
			////OutCard.wOutCardUser =getMeUserDate()->wChairID;
			//OutCard->cbCardCount =_data->m_handCardNum;
			//memcpy(OutCard->cbCardData,_data->m_handCardData,sizeof(BYTE)*_data->m_handCardNum);
			//sendSeverData(SUB_C_OUT_CARD, OutCard, sizeof(CMD_C_OutCard)-sizeof(OutCard->cbCardData)+OutCard->cbCardCount*sizeof(BYTE));

			////--自己的牌显示
			//CCoutCard* _outCard =CCoutCard::create(msgToView_outCard);
			//_outCard->m_outUser = 1;
			//_outCard->m_numCard =_data->m_handCardNum;
			//memcpy(_outCard->m_outkCard,_data->m_handCardData,_outCard->m_numCard);
			//_outCard->m_bankUser=m_nBankerUser;
			//NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_outCard);
			////--减掉手牌
			//DDZLogic _logic;
			//BYTE		outCardData[MAX_COUNT];
			//WORD	outCardNum=m_handCardNum[1];
			//memcpy(outCardData,m_handCardData[1],MAX_COUNT);

			////if(_logic.removeCard(OutCard.bCardData,OutCard.bCardCount,outCardData,outCardNum))
			////{
			////	outCardNum-=OutCard.bCardCount;
			////}

			//if(OutCard->cbCardCount == 2){
			//	const unsigned char num1 = OutCard->cbCardData[0];
			//	const unsigned char num2 = OutCard->cbCardData[1];
			//	if(num1 == 0x4F &&num2 == 0x4E)
			//	{
			//		//NotificationCenter::getInstance()->postNotification("WANGZHA");
			//	}
			//}
			////--手牌变化
			//CCsendCard* msgdata=CCsendCard::create(msgToView_sendCard);
			//memcpy(msgdata->m_handCardData,outCardData,sizeof(BYTE)*MAX_COUNT);
			//msgdata->m_handCardNum = outCardNum;
			//msgdata->m_bOpen =m_wOpenUser[1];
			//msgdata->m_bEffect =false;
			//msgdata->m_userID =1;
			//NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,msgdata);

		}
		break;
	case msgToLogic_passCard:
		{
			//sendSeverData(SUB_C_PASS_CARD);
		}
		break;
	case 	msgToLogc_openCard:
		{
			//CCviewOpenCardNum* _cardNum=(CCviewOpenCardNum*)_int;
			//BYTE cbBuffer[sizeof(CMD_C_OPENCARD)];
			//memset(cbBuffer, 0 , sizeof(CMD_C_OPENCARD));
			//CMD_C_OPENCARD *OpenCard = (CMD_C_OPENCARD *)cbBuffer;
			//OpenCard->bOpenUser = getMeUserDate()->wChairID;
			//OpenCard->bNum  =_cardNum->m_num;
			//sendSeverData(REC_SUB_C_OPNE_CARD,OpenCard,sizeof(CMD_C_OPENCARD));
		}
		break;
	case msgToLogc_LandLord:
		{
			//CClandLord*_data =(CClandLord*)_int;
			//if(_data->m_isLand == true){
			//	CCviewOpenCardNum* _cardNum=(CCviewOpenCardNum*)_int;
			//	BYTE cbBuffer[sizeof(CMD_C_CallScore)];
			//	memset(cbBuffer, 0, sizeof(CMD_C_CallScore));
			//	CMD_C_CallScore *CallScore = (CMD_C_CallScore *)cbBuffer;
			//	CallScore->cbCallScore = (m_bLastCallScore==255?0:m_bLastCallScore)+1;
			//	EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_CALL_SCORE,CallScore,sizeof(CMD_C_CallScore));
			//}else{
			//	BYTE cbBuffer[sizeof(CMD_C_CallScore)];
			//	memset(cbBuffer, 0, sizeof(CMD_C_CallScore));
			//	CMD_C_CallScore *CallScore = (CMD_C_CallScore *)cbBuffer;
			//	CallScore->cbCallScore = 255;
			//	EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_CALL_SCORE,CallScore,sizeof(CMD_C_CallScore));
			//}
		}
		break;
	case msgToLogic_MoreGame:
		{
			if (EntityMgr::instance()->getDispatch()->isGameLinkOK())
			{
				Director::getInstance()->replaceScene(BJLSceneControl::sharedSceneControl()->getScene(SCENE_game,false));
				EntityMgr::instance()->getDispatch()->SendPacketWithUserStatus(US_READY);
			}
			else
			{
				NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,Integer::create(msgToView_socketoff));
			}
		}
		break;
	case magToLogc_jiabei:
		{
			//CMD_C_DAOFAN _addData ;
			//_addData.m_bUserDaofanID = getMeUserDate()->wChairID;
			//sendSeverData(REC_SUB_C_DAOFAN,&_addData,sizeof(CMD_C_DAOFAN));
		}
		break;
	case msgToLogic_face:
		{
			CCfaceAndVioce * obj1=(CCfaceAndVioce*)obj;
			CMD_GF_EXPRESSION expression;
			expression.wType    = obj1->m_Type;
			expression.wIndex   = obj1->m_KeyID;
			expression.wChairID = getMeUserDate()->wChairID;
			//sendSeverData(SUB_GF_EXPRESSION, &expression, sizeof(expression));
			if(obj1->m_Type == 2)			//（1表情 2语音）
			{
				//此处不明
				//BYTE cbBuffer[sizeof(CMD_GR_C_UserChat)];
				//memset(cbBuffer, 0, sizeof(CMD_GR_C_UserChat));
				//CMD_GR_C_UserChat *userChat = (CMD_GR_C_UserChat *)cbBuffer;
				//Dictionary * strings = Dictionary::createWithContentsOfFile("bjl/xml/strings.xml");//by hxh
				//const char* buff=((String*)strings->objectForKey(String::createWithFormat("wav%d",obj1->m_KeyID)->getCString()))->getCString();
				//userChat->wChatLength = 128;
				//userChat->dwChatColor = 0XFF0000;
				//userChat->dwTargerUserID = EntityMgr::instance()->getDispatch()->m_dwUserID;
				//MyNSString::mbs2wc(buff, strlen(buff), userChat->szChatString);
				//sendFrameData(SUB_GF_USER_CHAT,userChat,sizeof(CMD_GR_C_UserChat));
			}else{
				BYTE cbBuffer[sizeof(CMD_GR_C_UserExpression)];
				memset(cbBuffer, 0, sizeof(CMD_GR_C_UserExpression));
				CMD_GR_C_UserExpression *userExpression = (CMD_GR_C_UserExpression *)cbBuffer;
				userExpression->dwTargetUserID = EntityMgr::instance()->getDispatch()->m_dwUserID;
				userExpression->wItemIndex = obj1->m_KeyID;
				sendFrameData(SUB_GF_USER_EXPRESSION,userExpression,sizeof(CMD_GR_C_UserExpression));
			}
		}
		break;
	}
}

bool SceneLogic::onSceneMessage( unsigned char cbStation,const void* pBuffer,WORD wDataSize )
{
	CCLOG("xin::  SceneLogic::onSceneMessage;; cbStation = %d, wDataSize = %d", cbStation, wDataSize);
	switch(cbStation)
	{
	case GAME_SCENE_FREE:               //空闲状态 0
		{
			CCLOG("GAME_SCENE_FREE 0");
			return OnGameSceneFree(pBuffer,wDataSize);
		}
	case GAME_SCENE_CALL:	//百家乐玩法的   下注阶段 100
		{
			CCLOG("GAME_SCENE_CALL 100");
			return OnGameSceneOnBet(pBuffer,wDataSize);
		}
	case GAME_SCENE_PLAY:			//游戏状态 101		（百家乐玩法的 开牌阶段  101）
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
			m_eGameState = EBJLGameState_Idle;
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//游戏开始
		{
			CCLOG("SUB_S_GAME_START 100");
			m_eGameState = EBJLGameState_Start;
			return onSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON:		//用户下注
		{
			CCLOG("SUB_S_PLACE_JETTON 101");
			m_eGameState = EBJLGameState_Bet;
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
			m_eGameState = EBJLGameState_End;
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

void SceneLogic::AddGameRecord(Object *obj)
{
	int ZXP = ((Integer *)obj)->getValue();
	m_cardTableLayer->AddWinLostResult(ZXP);
	if(m_wBankerUser==INVALID_CHAIR)
	{
		LONGLONG lBankerScore=m_cardTableLayer->getBankerScore();
		m_cardTableLayer->setBankerScore(lBankerScore+m_GameEnd.lBankerTotallScore);
	}
	m_cardTableLayer->setBankUsedChengjiORrJuNumber(m_GameEnd.lBankerTotallScore,m_GameEnd.nBankerTime);
	m_cardTableLayer->setPlayAllScoreORchengji(m_GameEnd.lPlayAllScore);
}

void SceneLogic::onUserStart(Object *pSender)
{
	//--游戏准备开始
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,Integer::create(msgToView_start));
}

bool SceneLogic::onUserStartView( const void * pBuffer, WORD wDataSize )
{
	//if (wDataSize!= sizeof(CMD_S_READY))
	//{
	//	CCLOG("发牌数据校验错误！");
	//	return false;
	//}

	////--游戏开始表现
	//CMD_S_READY* _ready=(CMD_S_READY*)pBuffer;
	//CCopenStart* _start=CCopenStart::create(msgToView_startView);
	//_start->m_bOpenCard =_ready->bOpenCard;
	//_start->m_bCurrentUser = switchViewChairID(_ready->bCurrentUser);
	//NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_start);
	return true;
}

bool SceneLogic::onUserBEeiLv( const void * pBuffer, WORD wDataSize )
{
	//if (wDataSize!= sizeof(CMD_S_BEILV))
	//{
	//	CCLOG("发牌数据校验错误！");
	//	return false;
	//}
	//CMD_S_BEILV *_beilv =(CMD_S_BEILV*)pBuffer;
	//CCbeilv*_beidata =CCbeilv::create(msgToView_beiLv);
	//_beidata->m_base =m_baseScore;
	//_beidata->m_beilv = _beilv->beillu[getMeUserDate()->wChairID];
	//NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_beidata);
	return true;
}

bool SceneLogic::onUserAdd( const void * pBuffer, WORD wDataSize )
{
	//if (wDataSize!= sizeof(CMD_S_CallScore))
	//{
	//	CCLOG("发牌数据校验错误！");
	//	return false;
	//}

	//变量定义
	////CMD_S_CallScore * pCallScore=(CMD_S_CallScore *)pBuffer;
	//CMD_S_DAOFAN* _daofang =(CMD_S_DAOFAN *)pBuffer;
	//CCaddView* _addView =CCaddView::create(msgToView_addView);
	//_addView->m_addID = switchViewChairID(_daofang->m_bUserDaofanID);
	//_addView->m_AddType =1;
	//NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_addView);
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
	setGameState(EBJLGameState_Idle);
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
	//效验大小 
	assert(wDataSize==sizeof(CMD_S_StatusFree));
	if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

	CCLOG("－－－－小游戏－－－空闲状态－－－－－－－－");
	CMD_S_StatusFree* pStatusPlay=(CMD_S_StatusFree *)pBuffer;
	//空闲时间
	BYTE Tinme= pStatusPlay->cbTimeLeave -2;
	m_wBankerUser = pStatusPlay->wBankerUser;
	//设置庄家信息
	m_cardTableLayer->setBankerName(pStatusPlay->wBankerUser);
	CUserItem* pUserItem = (CUserItem*)EntityMgr::instance()->roomFrame()->getUserItem(pStatusPlay->wBankerUser);

	CCLOG("GAME_SCENE_FREE=0 wBankerUser=%d",m_wBankerUser);
	// 加上指针非空判断，避免崩溃<by hxh 20160707>
#ifdef USE_GAMETIME
	if(SpriteGameTime){
		SpriteGameTime->setVisible(true);
		setGameStateTip(0);
	}
#endif
	NotificationCenter::getInstance()->postNotification("StrameNumber", Integer::create(m_wBankerUser));
	NotificationCenter::getInstance()->postNotification("Strameobjet", Integer::create(Tinme));
	NotificationCenter::getInstance()->postNotification("MI_setCCteefTag");
	m_cardTableLayer->waitingMatching(false);
	operlayer->dissUnclickTouch();
	//if (!pStatusPlay->bEnableSysBanker)//不是系统坐庄
	{		
		m_cardTableLayer->setUserIsOnline(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore,pStatusPlay->lBankerWinScore,pStatusPlay->wBankerTime);
	}
	m_cardTableLayer->updateBankerInfo(
		pStatusPlay->wBankerUser,
		pStatusPlay->szBankerNickName,
		pStatusPlay->lBankerScore,
		pStatusPlay->lBankerWinScore,
		pStatusPlay->wBankerTime
		);
	return true;
}

bool SceneLogic::OnGameSceneOnBet(const void *pBuffer, WORD wDataSize)
{
	CCLOG("SceneLogic::OnGameSceneOnBet ;; wDataSize = %d,sizeof(CMD_S_StatusPlay) = %d",wDataSize,sizeof(CMD_S_StatusPlay));
	//效验数据
	if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
	CMD_S_StatusPlay* pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

	m_wBankerUser= pStatusPlay->wBankerUser;	//当前庄家
	CCLOG("GAME_SCENE_PLAY=101 wBankerUser=%d",m_wBankerUser);

	//====================================
	//下注时间
	BYTE Tinme= pStatusPlay->cbTimeLeave -2;
	m_wBankerUser = pStatusPlay->wBankerUser;
	CUserItem* pUserItem = (CUserItem*)EntityMgr::instance()->roomFrame()->getUserItem(pStatusPlay->wBankerUser);

	CCLOG("GAME_SCENE_FREE=0 wBankerUser=%d",m_wBankerUser);
	//-----------------------------------------------------------------------
	//CCLOG("CMD_S_GameStart 1");
	//玩家信息
	m_lPlayBetScore = pStatusPlay->lPlayBetScore;
	//CCLOG("CMD_S_GameStart 2");
	m_lPlayFreeSocre = pStatusPlay->lPlayFreeSocre;
	//CCLOG("CMD_S_GameStart 3");
	m_wBankerUser = pStatusPlay->wBankerUser;
	//CCLOG("CMD_S_GameStart 4");
	m_lBankerScore = pStatusPlay->lBankerScore;
	//CCLOG("CMD_S_GameStart 5");
	int rejsbvkfsd  = pStatusPlay->cbTimeLeave-2;

	CCLOG("wBankerUser = %d,lBankerScore=%lld",pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);

	//设置时间,标志
	NotificationCenter::getInstance()->postNotification("Strameobjet", Integer::create(rejsbvkfsd));
	NotificationCenter::getInstance()->postNotification("MI_setPlaceScore");

	bool isMeBanker=m_cardTableLayer->setBankerName(pStatusPlay->wBankerUser);
	m_cardTableLayer->setBankerScore(pStatusPlay->lBankerScore);
	m_cardTableLayer->setBankUsedChengjiORrJuNumber(pStatusPlay->lBankerWinScore, pStatusPlay->wBankerTime);
	// add by hxh 20160706
	setCallScoreTip(isMeBanker);
	m_cardTableLayer->waitingMatching(false);
	SpriteGameStare->setVisible(true);
	if(!isMeBanker){
		operlayer->initClickTouch();
	}
	//---------------
	//展示已经下注金币
	for (int i = 0; i < AREA_MAX; i++)
	{
		if(pStatusPlay->lAllBet[i] <= 0)
		{
			continue;
		}
		pStatusPlay->lAllBet[i];
		m_chipManager->showGetChipManager(i,pStatusPlay->lAllBet[i]);
		m_chipManager->showChipManangerSpriteForAll(i,pStatusPlay->lAllBet[i]);
		//showChipManangerSpriteForAll
			//showChipManagerSprite
	}
	
	m_cardTableLayer->updateBankerInfo(
		pStatusPlay->wBankerUser,
		pStatusPlay->szBankerNickName,
		pStatusPlay->lBankerScore,
		pStatusPlay->lBankerWinScore,
		pStatusPlay->wBankerTime
		);

	//展示已经下注金币
	//m_chipManager->showGetChipManager(pPlaceBet->cbBetArea,m_lBetScore[pPlaceBet->cbBetArea]);
	//m_chipManager->showChipManagerSprite(pPlaceBet->cbBetArea,pPlaceBet->lBetScore);

	SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_time_warimg);

	return true;
}

bool SceneLogic::OnGameScenePlay(const void *pBuffer, WORD wDataSize)
{
	CCLOG("SceneLogic::OnGameScenePlay ;; wDataSize = %d,sizeof(CMD_S_StatusPlay) = %d",wDataSize,sizeof(CMD_S_StatusPlay));
	//效验数据
	if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
	CMD_S_StatusPlay* pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

	m_wBankerUser= pStatusPlay->wBankerUser;	//当前庄家
	CCLOG("GAME_SCENE_PLAY=101 wBankerUser=%d",m_wBankerUser);

	//====================================
	//开牌时间
	BYTE Tinme= pStatusPlay->cbTimeLeave -2;
	m_wBankerUser = pStatusPlay->wBankerUser;
	CUserItem* pUserItem = (CUserItem*)EntityMgr::instance()->roomFrame()->getUserItem(pStatusPlay->wBankerUser);

	CCLOG("GAME_SCENE_FREE=0 wBankerUser=%d",m_wBankerUser);

	NotificationCenter::getInstance()->postNotification("StrameNumber", Integer::create(m_wBankerUser));
	NotificationCenter::getInstance()->postNotification("Strameobjet", Integer::create(Tinme));
	NotificationCenter::getInstance()->postNotification("MI_setCardTalayer");
	m_cardTableLayer->waitingMatching(false);
	operlayer->dissUnclickTouch();
	//if (!pStatusPlay->bEnableSysBanker)//不是系统坐庄
	{		
		//m_cardTableLayer->setUserIsOnline(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore,pStatusPlay->lBankerWinScore,pStatusPlay->wBankerTime);
	}
	//
	bool isMeBanker=m_cardTableLayer->setBankerName(pStatusPlay->wBankerUser);
	m_cardTableLayer->setBankerScore(pStatusPlay->lBankerScore);
	m_cardTableLayer->setBankUsedChengjiORrJuNumber(pStatusPlay->lBankerWinScore, pStatusPlay->wBankerTime);

	//************************************************************************************

	//更新庄家 玩家信息
	//LONGLONG lBankerScoregame = pGameEnd->lBankerScore;					//庄家成绩
	//LONGLONG lBankerTotallScoregame= pGameEnd->lBankerTotallScore;		//庄家成绩
	//int		nBankerTimeGame = pGameEnd->nBankerTime ;
	LONGLONG lPlayScoreGAMER[AREA_MAX]; //做庄次数
	LONGLONG lPlayAllScoreGame =pStatusPlay->lPlayAllScore;						//玩家成绩

	// add by hxh 20160706
#if 1
	operlayer->dissUnclickTouch();
#endif
	// add by hxh 20160811
	WORD meChairID=EntityMgr::instance()->getDispatch()->m_wChairID;
	if (operlayer->IsXIazhuNumber||(m_wBankerUser==meChairID)){
		//m_pCardManager->showGameOver();
		for (int i =0;i<AREA_MAX;i++){
			lPlayScoreGAMER[i] = pStatusPlay->lPlayScore[i];
			if (pStatusPlay->lPlayScore[i] != 0){
				m_pCardManager->SetScoreGameOver(pStatusPlay->lPlayScore[i],i,pStatusPlay->lPlayAllScore);
			}
		}
	}
	m_ucHandCardData[0] = pStatusPlay->cbTableCardArray[0][0];
	m_ucHandCardData[1] = pStatusPlay->cbTableCardArray[0][1];
	m_ucHandCardData[2] = pStatusPlay->cbTableCardArray[0][2];
	m_ucHandCardData1[0] = pStatusPlay->cbTableCardArray[1][0];
	m_ucHandCardData1[1] = pStatusPlay->cbTableCardArray[1][1];
	m_ucHandCardData1[2] = pStatusPlay->cbTableCardArray[1][2];

	//将手牌传入牌类
	m_pCardManager->SetHandCardData(m_ucHandCardData); 
	m_pCardManager->SetHandCardData1(m_ucHandCardData1); 
	m_pCardManager->showGameOverWithNoAnim(pStatusPlay->cbTableCardArray,pStatusPlay->cbCardCount);

	//m_pCardManager->showPlayerCard();
	//m_pCardManager->showPlayerCard1(pStatusPlay->cbTableCardArray,pStatusPlay->cbCardCount);

	//全局信息
	//LONGLONG lRevenueGame =pGameEnd->lRevenue;							//游戏税收
	m_OperaLayer->remaggerdisflase();
	operlayer->dissUnclickTouch();
	SpriteGameStare->setVisible(false);
	for(int i =0;i<8;i++)
	{
		m_lBetScore[i] = 0;  //游戏结束重新初始化为0
	}
	SoundControl::sharedSoundControl()->playSoundEffect(sound_name::s_time_warimg);

	m_cardTableLayer->updateBankerInfo(
		pStatusPlay->wBankerUser,
		pStatusPlay->szBankerNickName,
		pStatusPlay->lBankerScore,
		pStatusPlay->lBankerWinScore,
		pStatusPlay->wBankerTime
		);

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

	setGameState(EBJLGameState_Start);

	//设置时间,标志
	NotificationCenter::getInstance()->postNotification("Strameobjet", Integer::create(rejsbvkfsd));
	NotificationCenter::getInstance()->postNotification("MI_setPlaceScore");
#ifdef USE_GAMETIME
	setGameStateTip(1);
#endif
	bool isMeBanker=m_cardTableLayer->setBankerName(pGameStart->wBankerUser);//<add by hxh 20160718>
	m_cardTableLayer->setBankerScore(pGameStart->lBankerScore);
	// add by hxh 20160706
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
	//CMD_C_HumanArea sHumanAddArea;
	//memset(&sHumanAddArea,0,sizeof(CMD_C_HumanArea)); 
	m_lBetScore[pPlaceBet->cbBetArea] += pPlaceBet->lBetScore;

#if 0
	m_chipManager->showGetChipManager(pPlaceBet->cbBetArea,m_lBetScore[pPlaceBet->cbBetArea]);
	m_chipManager->showChipManagerSprite(pPlaceBet->cbBetArea,pPlaceBet->lBetScore);
#endif
	// by hxh 20160706
#if 1
	WORD meChairID=EntityMgr::instance()->getDispatch()->m_wChairID;
	if (meChairID == pPlaceBet->wChairID)
	{
		m_cardTableLayer->UpdatePlacelBankScore(pPlaceBet->lBetScore);
	}
	CCLOG("CMD_S_PlaceBet meChairID=%d,wChairID=%d,cbBetArea=%d[%s],lBetScore=%lld,cbAndroidUser=%d",meChairID,pPlaceBet->wChairID,pPlaceBet->cbBetArea,BET_AREA_NAME(pPlaceBet->cbBetArea),pPlaceBet->lBetScore,pPlaceBet->cbAndroidUser);
	if(meChairID==pPlaceBet->wChairID||true)
	{
		m_chipManager->showGetChipManager(pPlaceBet->cbBetArea,m_lBetScore[pPlaceBet->cbBetArea]);
		m_chipManager->showChipManagerSprite(pPlaceBet->cbBetArea,pPlaceBet->lBetScore);
	}
	if(meChairID==pPlaceBet->wChairID)
	{
		operlayer->setChouMaNumber(pPlaceBet->cbBetArea,pPlaceBet->lBetScore);
	}
#else
	SpriteGameStare->setVisible(true);
	operlayer->initClickTouch();
#endif
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

	// add by hxh 20160811
    memcpy(&m_GameEnd,pGameEnd,sizeof(CMD_S_GameEnd));

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

	// add by hxh 20160706
#if 1
	operlayer->dissUnclickTouch();
#endif
	// add by hxh 20160811
	WORD meChairID=EntityMgr::instance()->getDispatch()->m_wChairID;
    if (operlayer->IsXIazhuNumber||(m_wBankerUser==meChairID)){
		m_pCardManager->showGameOver();
		for (int i =0;i<AREA_MAX;i++){
			lPlayScoreGAMER[i] = pGameEnd->lPlayScore[i];
			if (pGameEnd->lPlayScore[i] != 0){
				m_pCardManager->SetScoreGameOver(pGameEnd->lPlayScore[i],i,pGameEnd->lPlayAllScore);}}
	}
	m_ucHandCardData[0] = pGameEnd->cbTableCardArray[0][0];
	m_ucHandCardData[1] = pGameEnd->cbTableCardArray[0][1];
	m_ucHandCardData[2] = pGameEnd->cbTableCardArray[0][2];
	m_ucHandCardData1[0] = pGameEnd->cbTableCardArray[1][0];
	m_ucHandCardData1[1] = pGameEnd->cbTableCardArray[1][1];
	m_ucHandCardData1[2] = pGameEnd->cbTableCardArray[1][2];

	//将手牌传入牌类
	m_pCardManager->SetHandCardData(m_ucHandCardData); 
	m_pCardManager->SetHandCardData1(m_ucHandCardData1); 
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
	// add by hxh 20160720
	WORD meChairID=EntityMgr::instance()->getDispatch()->m_wChairID;
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
	// add by hxh 20160720
	m_wBankerUser=pPassCard->wBankerUser;
	WORD meChairID=EntityMgr::instance()->getDispatch()->m_wChairID;
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

/*
	m_cardTableLayer->setBankerName(pPassCard->wBankerUser);//<add by hxh 20160718>
	m_cardTableLayer->setBankerScore(pPassCard->lBankerScore);
	//切换庄家的时候需要重新将成绩和局数初始化
	m_cardTableLayer->setBankUsedChengjiORrJuNumber(0,0);
*/
	m_cardTableLayer->updateBankerInfo(
		pPassCard->wBankerUser,
		pPassCard->szBankerNickName,
		pPassCard->lBankerScore,
		0,0);
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
	//assert(wDataSize==sizeof(CMD_S_CommandResult));
	//if (wDataSize!=sizeof(CMD_S_CommandResult)) return false;
	CMD_S_CommandResult* _data =(CMD_S_CommandResult*)pBuffer;

	return true;
}

//提示消息
bool SceneLogic::onSubTipInfo( const void * pBuffer, WORD wDataSize )
{
	//消息处理
	CCLOG("wDataSize=%d,sizeof(CMD_S_TipInfo)",wDataSize,sizeof(CMD_S_TipInfo));
	CMD_S_TipInfo* _data =(CMD_S_TipInfo*)pBuffer;
	int DataSize=256;//wcslen((wchar_t *)_data->szTipInfo)*2+2;
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
	//CCLOG("%d",sizeof(CMD_S_CancelBanker));
	//效验大小 
	//assert(wDataSize==sizeof(CMD_S_CancelBanker));
	//if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;
	CMD_S_CancelBanker* _data =(CMD_S_CancelBanker*)pBuffer;
	// add by hxh 20160720
	WORD meChairID=EntityMgr::instance()->getDispatch()->m_wChairID;
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
	//效验大小 
	//if (wDataSize%sizeof(tagServerGameRecord)!=0) return false;

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
			//EntityMgr::instance()->getDispatch()->sendLoginPacket();

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




