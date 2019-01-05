#include "DDZSceneLogic.h"
#include "RoomFrame.h"
#include "DDZDDZRes.h"
#include "DDZCMD_DDZ.h"
#include "DDZDDZLogic.h"
#include "GlobalFrame.h"
#include "CMD_Game.h"
#include "CMD_GameServer.h"
#include "DDZSceneControl.h"
#include "TaskInfo.h"
#include "utf-8.h"
#include "DDZGameTaskCommon.h"
#include "GlobalUnits.h"
#include "ConfigMgr.h"
#include "DDZCMD_DDZClient.h"
#include "MyNSString.h"
#include "DDZSceneView.h"

int DDZcompareCardValue(const void *a, const void *b)
{
	BYTE valuaA = *(BYTE *)a;
	BYTE valuaB = *(BYTE *)b;
	return DDZSceneLogic::GetCardLogicValue(valuaB) - DDZSceneLogic::GetCardLogicValue(valuaA);
}

bool DDZSceneLogic::init()
{
	bool ret=false;
	do 
	{
		m_cbTimeCallBanker = 20;					//叫地主时间
		m_cbTimeRodBanker = 30;					//抢地主时间
		m_cbTimeOutCard = 20;						//出牌时间
		m_cbTimeStartGame = 30;					//开始时间
		m_cbTimeHeadOutCard = 10;					//首出时间
		m_lCellScore = 1;
		//--手牌
		for(int i=0 ;i<DDZGAME_PLAYER;i++)
		{
			memset(m_handCardData[i],0,sizeof(BYTE)*MAX_COUNT);
			m_handCardNum[i]=0;
			m_BackCardData[i]=0;
			memset(m_outCardData,0,sizeof(BYTE)*MAX_COUNT);
			m_outCardNum=0;
			m_wOpenUser[i] =false;
		}
		m_eGameState = DDZGame_null;
		m_isOut =false;
		m_beilv = 1;
		m_baseScore=0;
		m_bLastCallScore = 0;
		m_wStartUser = INVALID_CHAIR;
		EntityMgr::instance()->getDispatch()->setGameStatus(true);

		NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DDZSceneLogic::onUserStart),"GameStart" , NULL);
		NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DDZSceneLogic::onUpdateTable),MSG_UI_ANS_PLAYERUPDATE,NULL);
		NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DDZSceneLogic::onMessageGame),GBEVENT_UI_GAMEMESSAGE,NULL);
		NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DDZSceneLogic::onMessageScene),GBEVENT_UI_SCENEMESSAGE,NULL);
		NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DDZSceneLogic::onUpdataMatch),MSG_UI_ANS_MATCH,NULL);
		NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(DDZSceneLogic::onViewMessage),MESSAGE_TOLOGIC,NULL);
		NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DDZSceneLogic::onLinkResult),MSG_UI_ANS_GAMELINK , NULL);				//���ӷ�����
		NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DDZSceneLogic::onUpdateScoreAndGoldEgg),MSG_UI_ANS_UPDATESCOREANDGOLDEGG , NULL);			//--��ֵˢ�½��
		this->schedule(schedule_selector(DDZSceneLogic::checkNet), 8);
		ret = true;

	} while (0);
	return ret;
}
void DDZSceneLogic::onEnter()
{
	Node::onEnter();
	//��Ϸ������Ϣ
}

void DDZSceneLogic::onExit()
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

void DDZSceneLogic::checkNet(float ft)
{
	static int NetData=0;
	if (EntityMgr::instance()->getDispatch()->isGameLinkOK()) 
	{
		NetData=0;
		unschedule(schedule_selector(DDZSceneLogic::checkNet));
		schedule(schedule_selector(DDZSceneLogic::checkNet), 8);
	}else{
		//NetData++;
 	//	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,NetData>2?(NetData=0,Integer::create(DDZmsgToView_socketoff)):Integer::create(DDZmsgToView_socketoffConnect));
		//unschedule(schedule_selector(DDZSceneLogic::checkNet));
		//schedule(schedule_selector(DDZSceneLogic::checkNet), 4);

		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,Integer::create(DDZmsgToView_socketoff));
		unschedule(schedule_selector(DDZSceneLogic::checkNet));
	}
}

int DDZSceneLogic::switchViewChairID( unsigned int nChairID )
{
	tagUserData*    m_pMeUserDate=EntityMgr::instance()->roomFrame()->getMeUserItem()->getUserData();
	unsigned int nViewChairID = (nChairID + DDZGAME_PLAYER - m_pMeUserDate->wChairID);
	switch (DDZGAME_PLAYER)
	{
	case 2: { nViewChairID+=1; break; }
	case 3: { nViewChairID+=1; break; }
	case 4: { nViewChairID+=2; break; }
	case 5: { nViewChairID+=2; break; }
	case 6: { nViewChairID+=3; break; }
	case 7: { nViewChairID+=3; break; }
	case 8: { nViewChairID+=4; break; }
	}
	return nViewChairID % DDZGAME_PLAYER;
}

void DDZSceneLogic::sendSeverData( WORD wSubCmdID )
{
	if (EntityMgr::instance()->getDispatch()->isGameLinkOK()) {
		EntityMgr::instance()->getDispatch()->sendSocketDate(wSubCmdID);
	}else{
		//?????????????????????
		//sendViewMessage(LOGIC_SOCKET_OFF);
	}
}

void DDZSceneLogic::sendSeverData( WORD wSubCmdID,void* pData,WORD wDataSize )
{
	if (EntityMgr::instance()->getDispatch()->isGameLinkOK()) {
		EntityMgr::instance()->getDispatch()->sendSocketDate(wSubCmdID,pData,wDataSize);
	}else{
		//?????????????????????
		//sendViewMessage(LOGIC_SOCKET_OFF);
	}
}

void DDZSceneLogic::sendFrameData( WORD wSubCmdID,void* pData,WORD wDataSize )
{
	if (EntityMgr::instance()->getDispatch()->isGameLinkOK()) {
		EntityMgr::instance()->getDispatch()->sendFrameDate(wSubCmdID,pData,wDataSize);
	}else{

	}
}

//游戏消息
void DDZSceneLogic::onMessageGame(Object* obj)
{
	tagGameBuffer* tag = (tagGameBuffer*)obj;
	onGameMessage(tag->wSubCmdID,tag->dateBuffer,tag->nDataSize);
}

//游戏场景消息
void DDZSceneLogic::onMessageScene(Object* obj)
{
	tagSceneBuffer* tag = (tagSceneBuffer*)obj;
	onSceneMessage(tag->cbState,tag->dateBuffer,tag->nDataSize);
}

void DDZSceneLogic::onUpdataMatch( Object* obj )
{
	//MatchError* match = MatchError::MatchErrorWith();
	//addChild(match,101);
}

//跟新桌子
void DDZSceneLogic::onUpdateTable(Object* obj)
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

	DDZCCuserInfor* _userData =DDZCCuserInfor::create(DDZmsgToView_UserInfor);
	_userData->m_userInforID = pUserDate->dwUserID;
	_userData->m_ViewID = nViewID;
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

void DDZSceneLogic::onViewMessage( Object* obj )
{
	Integer* _int=(Integer*)obj;
	switch(_int->getValue())
	{
	case DDZmsgToLogic_changeTable:
		{
			EntityMgr::instance()->getDispatch()->SendPacketWithChangeTable();
			EntityMgr::instance()->roomFrame()->clearUserDataList();
			Director::getInstance()->replaceScene(DDZSceneControl::sharedDDZSceneControl()->getScene(SCENE_game,true));
		}
		break;
	case DDZmsgToLogic_openStart:
		{
			EntityMgr::instance()->getDispatch()->SendPacketWithUserStatus(US_READY);
		}
		break;
	case DDZmagToLogc_gameStart:
		{
			EntityMgr::instance()->getDispatch()->SendPacketWithUserStatus(US_READY);
		}
		break;
	case DDZmsgToLogic_outCard:
		{
			DDZCCsendCard* _data=(DDZCCsendCard* )_int;

			BYTE cbBuffer[sizeof(DDZCMD_C_OutCard)];
			memset(cbBuffer, 0 , sizeof(DDZCMD_C_OutCard));
			DDZCMD_C_OutCard *OutCard = (DDZCMD_C_OutCard *)cbBuffer;
			OutCard->cbCardCount =_data->m_handCardNum;
			memcpy(OutCard->cbCardData,_data->m_handCardData,sizeof(BYTE)*_data->m_handCardNum);
			sendSeverData(SUB_C_OUT_CARD, OutCard, sizeof(DDZCMD_C_OutCard)-sizeof(OutCard->cbCardData)+OutCard->cbCardCount*sizeof(BYTE));

			//--自己的牌显示
			DDZCCoutCard* _outCard =DDZCCoutCard::create(DDZmsgToView_outCard);
			_outCard->m_outUser = 1;
			_outCard->m_numCard =_data->m_handCardNum;
			memcpy(_outCard->m_outkCard,_data->m_handCardData,_outCard->m_numCard);
			_outCard->m_bankUser=m_nBankerUser;
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_outCard);
			//--减掉手牌
			DDZDDZLogic _logic;
			BYTE		outCardData[MAX_COUNT];
			WORD	outCardNum=m_handCardNum[1];
			memcpy(outCardData,m_handCardData[1],MAX_COUNT);

			if(_logic.removeCard(OutCard->cbCardData,OutCard->cbCardCount,outCardData,outCardNum))
			{
				outCardNum-=OutCard->cbCardCount;
			}

			if(OutCard->cbCardCount == 2){
				const unsigned char num1 = OutCard->cbCardData[0];
				const unsigned char num2 = OutCard->cbCardData[1];
				if(num1 == 0x4F &&num2 == 0x4E)
				{
					NotificationCenter::getInstance()->postNotification("WANGZHA");
				}
			}
			//--手牌变化
			DDZCCsendCard* msgdata=DDZCCsendCard::create(DDZmsgToView_sendCard);
			memcpy(msgdata->m_handCardData,outCardData,sizeof(BYTE)*MAX_COUNT);
			msgdata->m_handCardNum = outCardNum;
			msgdata->m_bOpen =m_wOpenUser[1];
			msgdata->m_bEffect =false;
			msgdata->m_userID =1;
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,msgdata);

		}
		break;
	case DDZmsgToLogic_passCard:
		{
			sendSeverData(SUB_C_PASS_CARD);
		}
		break;
	case DDZmsgToLogc_openCard:
		{
			DDZCCviewOpenCardNum* _cardNum=(DDZCCviewOpenCardNum*)_int;
			BYTE cbBuffer[sizeof(DDZCMD_C_OPENCARD)];
			memset(cbBuffer, 0 , sizeof(DDZCMD_C_OPENCARD));
			DDZCMD_C_OPENCARD *OpenCard = (DDZCMD_C_OPENCARD *)cbBuffer;
			OpenCard->bOpenUser = getMeUserDate()->wChairID;
			OpenCard->bNum  =_cardNum->m_num;
			sendSeverData(REC_SUB_C_OPNE_CARD,OpenCard,sizeof(DDZCMD_C_OPENCARD));
		}
		break;
	case DDZmsgToLogc_LandLord:
		{
			DDZCClandLord*_data =(DDZCClandLord*)_int;
			//叫分
			//int aaa = m_bLastCallScore;
			/*if(_data->m_isLand == true){
				DDZCCviewOpenCardNum* _cardNum=(DDZCCviewOpenCardNum*)_int;
				BYTE cbBuffer[sizeof(DDZCMD_C_CallScore)];
				memset(cbBuffer, 0, sizeof(DDZCMD_C_CallScore));
				DDZCMD_C_CallScore *CallScore = (DDZCMD_C_CallScore *)cbBuffer;
				CallScore->cbCallScore = (m_bLastCallScore==255?0:m_bLastCallScore)+1;
				EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_CALL_SCORE,CallScore,sizeof(DDZCMD_C_CallScore));
			}else{
				BYTE cbBuffer[sizeof(DDZCMD_C_CallScore)];
				memset(cbBuffer, 0, sizeof(DDZCMD_C_CallScore));
				DDZCMD_C_CallScore *CallScore = (DDZCMD_C_CallScore *)cbBuffer;
				CallScore->cbCallScore = 255;
				EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_CALL_SCORE,CallScore,sizeof(DDZCMD_C_CallScore));
			}*/

			//欢乐玩法，叫、抢地主		 根据倍数来判断，叫与抢
			if(_data->m_isLand == true)
			{//叫操作
				
				if (m_bLastCallScore != 0)//抢地主
				{
					BYTE cbBuffer[sizeof(CMD_C_RodBanker)];
					memset(cbBuffer, 0, sizeof(CMD_C_RodBanker));
					CMD_C_RodBanker *robBank = (CMD_C_RodBanker *)cbBuffer;
					robBank->cbRodInfo = CB_ROD_BANKER;
					EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_ROD_BANKER,robBank,sizeof(CMD_C_RodBanker));
					CCLOG("xin:...aaaaaaa...rob lord yes");
				}
				else
				{//叫地主
					BYTE cbBuffer[sizeof(CMD_C_CallBanker)];
					memset(cbBuffer, 0, sizeof(CMD_C_CallBanker));
					CMD_C_CallBanker *callBank = (CMD_C_CallBanker *)cbBuffer;
					callBank->cbCallInfo = CB_CALL_BENKER;
					EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_CALL_BANKER,callBank,sizeof(CMD_C_CallBanker));
					CCLOG("xin:...aaaaaaa...call lord yes");
				}
			}
			else
			{//不叫操作
				if (m_bLastCallScore == 0)
				{//不叫
					BYTE cbBuffer[sizeof(CMD_C_CallBanker)];
					memset(cbBuffer, 0, sizeof(CMD_C_CallBanker));
					CMD_C_CallBanker *callBank = (CMD_C_CallBanker *)cbBuffer;
					callBank->cbCallInfo = CB_NO_CALL_BENKER;
					EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_CALL_BANKER,callBank,sizeof(CMD_C_CallBanker));
					CCLOG("xin:...aaaaaaa.00..call lord no");
				}
				else
				{//不抢
					BYTE cbBuffer[sizeof(CMD_C_RodBanker)];
					memset(cbBuffer, 0, sizeof(CMD_C_RodBanker));
					CMD_C_RodBanker *robBank = (CMD_C_RodBanker *)cbBuffer;
					robBank->cbRodInfo = CB_NO_ROD_BANKER;
					EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_ROD_BANKER,robBank,sizeof(CMD_C_RodBanker));
					CCLOG("xin:...aaaaaaa.11..rob lord no");
				}
			}

		}
		break;
	case DDZmsgToLogic_MoreGame:
		{
			if (EntityMgr::instance()->getDispatch()->isGameLinkOK())
			{
				Director::getInstance()->replaceScene(DDZSceneControl::sharedDDZSceneControl()->getScene(SCENE_game,false));
				EntityMgr::instance()->getDispatch()->SendPacketWithUserStatus(US_READY);
			}
			else
			{
				NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,Integer::create(DDZmsgToView_socketoff));
			}
		}
		break;
	case DDZmagToLogc_jiabei:
		{
			DDZCMD_C_DAOFAN _addData ;
			_addData.m_bUserDaofanID = getMeUserDate()->wChairID;
			sendSeverData(REC_SUB_C_DAOFAN,&_addData,sizeof(DDZCMD_C_DAOFAN));
		}
		break;
	case DDZmsgToLogic_face:
		{
			DDZCCfaceAndVioce * obj1=(DDZCCfaceAndVioce*)obj;
			CMD_GF_EXPRESSION expression;
			expression.wType    = obj1->m_Type;
			expression.wIndex   = obj1->m_KeyID;
			expression.wChairID = getMeUserDate()->wChairID;
			//sendSeverData(SUB_GF_EXPRESSION, &expression, sizeof(expression));
			if(obj1->m_Type == 2)			//（1表情 2语音）
			{
				BYTE cbBuffer[sizeof(CMD_GR_C_UserChat)];
				memset(cbBuffer, 0, sizeof(CMD_GR_C_UserChat));
				CMD_GR_C_UserChat *userChat = (CMD_GR_C_UserChat *)cbBuffer;
				Dictionary * strings = Dictionary::createWithContentsOfFile("xml/strings.xml");
				const char* buff=((CCString*)strings->objectForKey(CCString::createWithFormat("wav%d",obj1->m_KeyID)->getCString()))->getCString();
				userChat->wChatLength = 128;
				userChat->dwChatColor = 0XFF0000;
				userChat->dwTargerUserID = EntityMgr::instance()->getDispatch()->m_dwUserID;
				MyNSString::mbs2wc(buff, strlen(buff), userChat->szChatString);
				sendFrameData(SUB_GF_USER_CHAT,userChat,sizeof(CMD_GR_C_UserChat));
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

bool DDZSceneLogic::onSceneMessage( unsigned char cbStation,const void* pBuffer,WORD wDataSize )
{
	switch(cbStation)
	{
	case GAME_SCENE_FREE:              //空闲状态
		{
			CCLOG("----------dou di zhu---------get ready---------");
			DDZCMD_S_GameFree* pStatusPlay=(DDZCMD_S_GameFree *)pBuffer;
			//int  dfeiugvsk= pStatusPlay->cbTimeLeave;
			//各种状态的操作步长
			m_lCellScore = pStatusPlay->lCellScore;
			m_cbTimeCallBanker = pStatusPlay->cbTimeCallBanker;
			m_cbTimeRodBanker = pStatusPlay->cbTimeRodBanker;
			m_cbTimeOutCard = pStatusPlay->cbTimeOutCard;
			m_cbTimeStartGame = pStatusPlay->cbTimeStartGame;
			m_cbTimeHeadOutCard = pStatusPlay->cbTimeHeadOutCard;
			m_eGameState = DDZGame_Wait;
			DDZCCbeilv*_beidata =DDZCCbeilv::create(DDZmsgToView_beiLv);
			_beidata->m_base = m_lCellScore;
			_beidata->m_beilv = 0;
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_beidata);
			CCLOG("..................dou di zhu  ... state =  get ready...............GAME_SCENE_FREE...........");
			//EntityMgr::instance()->getDispatch()->SendPacketWithGameOption();
		}
		break;
	case GAME_SCENE_CALL:////叫地主状态
		{
			CCLOG("..................dou di zhu  ... state =  GAME_STATUS_PLAY..........................");
			//DDZCMD_S_StatusCall;
			if (wDataSize!=sizeof(DDZCMD_S_StatusCall)) return false;
			OnGameSceneCallLord(pBuffer, wDataSize);
			m_eGameState = DDZGame_Start;
		}
		break;
	case GAME_SCENE_ROD://抢地主状态
		{
			//DDZCMD_S_StatusRod
			if (wDataSize!=sizeof(DDZCMD_S_StatusRod)) return false;
			OnGameSceneRobLord(pBuffer, wDataSize);
			m_eGameState = DDZGame_Start;
		}
		break;
	case GAME_SCENE_PLAY:	//游戏进行		//游戏状态			斗地主恢复对局
		{
			CCLOG("...............dou di zhu , back to the game..................");
			//效验数据
			if (wDataSize!=sizeof(DDZ_CMD_S_StatusPlay)) return false;
			OnGameScenePlay(pBuffer, wDataSize);
			m_eGameState = DDZGame_Run;
		}
		break;
	default:
		break;
	}
	return  true;
}

bool DDZSceneLogic::onGameMessage( WORD wSubCmdID, const void* pBuffer, WORD wDataSize )
{
	switch (wSubCmdID)
	{
	case DDZ_SUB_S_GAME_START:		//游戏开始 100
		{
			CCLOG("DDZ_SUB_S_GAME_START 100");
			return onSubSendCard(pBuffer,wDataSize);
		}
	//case DDZ_SUB_S_CALL_SCORE:		//用户叫分 101
	//	{
	//		CCLOG("DDZ_SUB_S_CALL_SCORE 101");
	//		return onSubLandScore(pBuffer,wDataSize);
	//	}
	case DDZ_SUB_S_CALL_BANKER:		//用户叫地主
		{
			CCLOG("DDZ_SUB_S_CALL_SCORE 101");
			return onCallLord(pBuffer,wDataSize);
		}
	case DDZ_SUB_S_ROD_BANKER:		//用户抢地主
		{
			CCLOG("DDZ_SUB_S_CALL_SCORE 101");
			return onRobLord(pBuffer,wDataSize);
		}
	case DDZ_SUB_S_BANKER_INFO:		//ׯ庄家信息 102
		{
			CCLOG("DDZ_SUB_S_BANKER_INFO 102");
			return onSubGameStart(pBuffer,wDataSize);
		}
	case DDZ_SUB_S_OUT_CARD:		//用户出牌 103
		{
			CCLOG("DDZ_SUB_S_OUT_CARD 106");
			return onSubOutCard(pBuffer,wDataSize);
		}
	case DDZ_SUB_S_PASS_CARD:		//用户放弃 104
		{
			CCLOG("DDZ_SUB_S_PASS_CARD 107");
			return onSubPassCard(pBuffer,wDataSize);
		}
	case DDZ_SUB_S_GAME_CONCLUDE:	//游戏结束 109
		{
			CCLOG("DDZ_SUB_S_GAME_CONCLUDE 105");
			return onSubGameEnd(pBuffer,wDataSize);
		}
	case DDZ_SUB_S_SET_BASESCORE:	//设置基数 106
		{
			CCLOG("DDZ_SUB_S_SET_BASESCORE 106");
			/*
			//Ч������
			ASSERT(wDataSize==sizeof(LONG));
			if(wDataSize!=sizeof(LONG)) return false;

			LONG lCellScore=*(LONG*)pData;
			m_GameClientView.SetCellScore(lCellScore);
			return true;
			*/
		}
	case DDZ_SUB_S_CHEAT_CARD:		//作弊扑克 107	
		{
			CCLOG("DDZ_SUB_S_CHEAT_CARD 107");
			//return OnSubCheatCard(pData,wDataSize);
		}
	case SUB_GF_USER_CHAT:		//用户聊天 10
		{
			CCLOG("SUB_GF_USER_CHAT 10");
			return onUserChat(pBuffer,wDataSize);
		}
	case SUB_GF_USER_EXPRESSION:		//用户表情 11
		{
			CCLOG("SUB_GF_USER_EXPRESSION 11");
			return onUserExpression(pBuffer,wDataSize);
		}
	}

	return true;
}

//游戏命令
bool  DDZSceneLogic::gameMain(WORD wSubCmdID, const void *pData, WORD wDataSize)
{
    CCLOG("***********DDZSceneLogic::gameMain************");
   
	switch (wSubCmdID)
	{
        case HLDDZ_SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pData,wDataSize);
		}
#if 0
        case HLDDZ_SUB_S_CALL_BANKER:		//用户叫地主
		{
			return OnSubCallBanker(pData,wDataSize);
		}
        case HLDDZ_SUB_S_ROD_BANKER:		//用户抢地主
		{
			return OnSubRodBanker(pData,wDataSize);
		}
        case HLDDZ_SUB_S_DOUBLE:			//加倍信息
		{
			return OnSubDouble(pData,wDataSize);
		}
        case HLDDZ_SUB_S_VALID_CARD:		//用户明牌
		{
			return OnSubValidCard(pData,wDataSize);
		}
        case HLDDZ_SUB_S_BANKER_INFO:		//庄家信息
		{
			return OnSubBankerInfo(pData,wDataSize);
		}
        case HLDDZ_SUB_S_OUT_CARD:		//用户出牌
		{
			return OnSubOutCard(pData,wDataSize);
		}
        case HLDDZ_SUB_S_PASS_CARD:		//用户放弃
		{
			return OnSubPassCard(pData,wDataSize);
		}
        case HLDDZ_SUB_S_OUT_START_START:	//出牌开始
		{
			return OnSubOutCardStart(pData,wDataSize);
		}
        case HLDDZ_SUB_S_GAME_CONCLUDE:	//游戏结束
		{
			return OnSubGameConclude(pData,wDataSize);
		}
        case HLDDZ_SUB_S_SET_BASESCORE://设置基数
		{
			return OnSubSetBaseScore(pData, wDataSize);
		}
        case HLDDZ_SUB_S_NEWBANK:         //新的地主
		{
			return OnSubNewBank(pData, wDataSize);
		}
            //////GJ130423 托管标志
        case HLDDZ_SUB_S_TRUSTEE:			//用户托管
		{
            return OnSubTrustee(pData,wDataSize);
		}
#endif
        default:
            break;
	}
    
	return true;
};

void DDZSceneLogic::onUserStart(Object *pSender)
{
	//--游戏准备开始
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,Integer::create(DDZmsgToView_start));
}

bool DDZSceneLogic::onUserStartView( const void * pBuffer, WORD wDataSize )
{
	if (wDataSize!= sizeof(DDZCMD_S_READY))
	{
		CCLOG("发牌数据校验错误！");
		return false;
	}

	//--游戏开始表现
	DDZCMD_S_READY* _ready=(DDZCMD_S_READY*)pBuffer;
	DDZCCopenStart* _start=DDZCCopenStart::create(DDZmsgToView_startView);
	_start->m_bOpenCard =_ready->bOpenCard;
	_start->m_bCurrentUser = switchViewChairID(_ready->bCurrentUser);
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_start);
	return true;
}

bool DDZSceneLogic::onUserBEeiLv( const void * pBuffer, WORD wDataSize )
{
	if (wDataSize!= sizeof(DDZCMD_S_BEILV))
	{
		CCLOG("发牌数据校验错误！");
		return false;
	}
	DDZCMD_S_BEILV *_beilv =(DDZCMD_S_BEILV*)pBuffer;
	DDZCCbeilv*_beidata =DDZCCbeilv::create(DDZmsgToView_beiLv);
	_beidata->m_base =m_baseScore;
	_beidata->m_beilv = _beilv->beillu[getMeUserDate()->wChairID];
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_beidata);
	return true;
}

bool DDZSceneLogic::onUserAdd( const void * pBuffer, WORD wDataSize )
{
	if (wDataSize!= sizeof(DDZCMD_S_CallScore))
	{
		CCLOG("发牌数据校验错误！");
		return false;
	}

	//变量定义
	//CMD_S_CallScore * pCallScore=(CMD_S_CallScore *)pBuffer;
	DDZCMD_S_DAOFAN* _daofang =(DDZCMD_S_DAOFAN *)pBuffer;
	DDZCCaddView* _addView =DDZCCaddView::create(DDZmsgToView_addView);
	_addView->m_addID = switchViewChairID(_daofang->m_bUserDaofanID);
	_addView->m_AddType =1;
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_addView);
	return true;
}

bool DDZSceneLogic::onSubSendCard( const void * pBuffer, WORD wDataSize )
{
	if (wDataSize!=sizeof(DDZCMD_S_GameStart))
	{
		CCLOG("onSubSendCard  wrong data");
		return false;
	}
	m_isCallLord = false;
	m_eGameState = DDZGame_Start;
	//变量定义
	DDZCMD_S_GameStart * pGameStart=(DDZCMD_S_GameStart *)pBuffer;

	m_beilv = pGameStart->wStartTime;

	//--游戏开始表现
	WORD chairID = EntityMgr::instance()->getDispatch()->m_wChairID;
	DDZCCopenStart* _start=DDZCCopenStart::create(DDZmsgToView_startView);
	_start->m_bOpenCard =false;
	_start->m_bCurrentUser = switchViewChairID(chairID);
// 	_start->m_bCurrentUser = switchViewChairID(pGameStart->wCurrentUser);
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_start);

	//	发牌
	m_wCurrentUser = switchViewChairID(chairID);
	m_wStartUser = switchViewChairID(pGameStart->wCurrentUser);
	for (auto i = 0; i < DDZGAME_PLAYER; i++)
	{
		m_handCardNum[i] = NORMAL_COUNT;
		if (m_wCurrentUser == i)
		{
			memset(m_handCardData[i], 0, sizeof(BYTE) * MAX_COUNT);
			memcpy(m_handCardData[i], pGameStart->cbCardData, sizeof(BYTE) * NORMAL_COUNT);
			DDZDDZLogic _logic;
			_logic.sortCardList(m_handCardData[i],m_handCardNum[i],ST_ORDER);
		}
		DDZCCsendCard* pSendCard = DDZCCsendCard::create(DDZmsgToView_sendCard);
		memcpy(pSendCard->m_handCardData, m_handCardData[m_wCurrentUser], sizeof(BYTE) * MAX_COUNT);
		pSendCard->m_bOpen = false;
		pSendCard->m_bEffect = true;
		pSendCard->m_handCardNum = m_handCardNum[i];
		pSendCard->m_userID = i;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW, pSendCard);
	}
	scheduleOnce(schedule_selector(DDZSceneLogic::showMyFirstJiaoDiZhuBtnViews), 4.5f);

/*
	if (chairID == pGameStart->wStartUser)			//自己为第一个叫地主的玩家，显示叫地主界面
	{
		//m_wCurrentUser=switchViewChairID(pCallScore->wCurrentUser);
		//抢地主成功 INVALID_WORD
		/ *DDZCCtimeControl * timeControl=DDZCCtimeControl::create(DDZmsgToView_time);
		timeControl->m_timeUserID = m_wCurrentUser;
		timeControl->m_timeNum	 = 30;
		timeControl->m_gameState = DDZGame_landLord;
		timeControl->m_timeKey     = 1;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,timeControl);* /

		scheduleOnce(schedule_selector(DDZSceneLogic::showMyFirstJiaoDiZhuBtnViews), 4.5f);
	}	
*/

	return true;
}

void DDZSceneLogic::showMyFirstJiaoDiZhuBtnViews(float dt)
{
	DDZCCtimeControl * timeControl=DDZCCtimeControl::create(DDZmsgToView_time);
// 	timeControl->m_timeUserID = m_wCurrentUser;
	timeControl->m_timeUserID = m_wStartUser;
	timeControl->m_timeNum	 = m_cbTimeCallBanker;
	timeControl->m_gameState = DDZGame_landLord;
	timeControl->m_timeKey     = 1;
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,timeControl);
}

//游戏开始
bool DDZSceneLogic::OnSubGameStart(const void * pdata, WORD wDataSize)
{
    CCLOG("DDZSceneLogic::OnSubGameStart");
    //HLDDZ_S_Actor->WaitOtherPlayerEnterStatusOff();
    // 数据校验
    CCLOG("wDataSize=%d,sizeof(HLDDZ_CMD_S_GameStart)=%d",wDataSize,sizeof(HLDDZ_CMD_S_GameStart));
    
    if(wDataSize!=sizeof(HLDDZ_CMD_S_GameStart))return false;

    // 数据结构
    HLDDZ_CMD_S_GameStart *gameStart = (HLDDZ_CMD_S_GameStart*)pdata;
    
	CCLOG("开始玩家:%d",gameStart->wStartUser);
	CCLOG("当前玩家:%d",gameStart->wCurrentUser);
	//CCLOG("初始倍数:%d",gameStart->wStartTime);
	//CCLOG("是否明牌:%d",gameStart->bValidCard);
	CCLOG("明牌扑克:%d",gameStart->cbValidCardData);
	CCLOG("明牌位置:%d",gameStart->cbValidCardIndex);
	//CCLOG("是否比赛:%d",gameStart->bMatchGame);
    
    
    //HLDDZActor->MasterSendHandCard(gameStart->wStartUser,gameStart->wCurrentUser, NORMAL_COUNT, gameStart->cbCardData);
    
    return true;
}


bool DDZSceneLogic::onSubOpenCard( const void * pBuffer, WORD wDataSize )
{
	if (wDataSize!=sizeof(DDZCMD_S_OPENCARD))
	{
		CCLOG("发牌数据校验错误！");
		return false;
	}
	DDZCMD_S_OPENCARD* _data =(DDZCMD_S_OPENCARD*)pBuffer;

	m_wOpenUser[switchViewChairID(_data->bOpenUser)]=true;

	//--明牌效果表现
	DDZCCopenCard* openCard = DDZCCopenCard::create(DDZmsgToView_openCard);
	openCard->m_openID = switchViewChairID(_data->bOpenUser);
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,openCard);

	//--手牌表现
	if(openCard->m_openID == 1)return true;			//--自己明牌，不做变化
	DDZCCsendCard* msgdata=DDZCCsendCard::create(DDZmsgToView_sendCard);
	memcpy(msgdata->m_handCardData,m_handCardData[openCard->m_openID],sizeof(BYTE)*MAX_COUNT);
	msgdata->m_handCardNum = m_handCardNum[openCard->m_openID];
	msgdata->m_bOpen =m_wOpenUser[openCard->m_openID];
	msgdata->m_bEffect =false;
	msgdata->m_userID =openCard->m_openID;
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,msgdata);

	return true;
}

bool DDZSceneLogic::onCallLord(const void * pBuffer, WORD wDataSize)
{
	if (wDataSize!=sizeof(CMD_S_CallBanker))
	{
		CCLOG("call lord data is wrong");
		return false;
	}
	m_eGameState = DDZGame_landLord;

	//变量定义
	CMD_S_CallBanker * pCallScore=(CMD_S_CallBanker *)pBuffer;
	if (pCallScore->cbCallInfo == 1)
	{//叫地主
		m_bLastCallScore = m_bLastCallScore == 0?1:m_bLastCallScore * 2;
		m_beilv = 1;
	}
	else
	{//不叫
		m_bLastCallScore = 0;
		m_beilv = 1;
	}
	
	//倍率变化
	DDZCCbeilv*_beidata =DDZCCbeilv::create(DDZmsgToView_beiLv);
	_beidata->m_base = m_lCellScore;
	_beidata->m_beilv = m_beilv;
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_beidata);
	if (pCallScore->wLastUser != INVALID_WORD)
	{
		//首次抢地主 INVALID_WORD
		m_wTempUser = switchViewChairID(pCallScore->wLastUser);
		DDZCClandLord* landLord = DDZCClandLord::create(DDZmsgToView_landLord);
		landLord->m_landID = m_wTempUser;
		landLord->m_isLand = true;
		if(pCallScore->cbCallInfo == CB_CALL_BENKER){//叫地主
			m_isCallLord = true;
			landLord->m_LandKey = 2;
		}else{
			landLord->m_LandKey = 1;
		}
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,landLord);
	}

	if (pCallScore->wCurrentUser !=INVALID_WORD)
	{
		m_wCurrentUser=switchViewChairID(pCallScore->wCurrentUser);
		//抢地主成功 INVALID_WORD
		DDZCCtimeControl * timeControl=DDZCCtimeControl::create(DDZmsgToView_time);
		timeControl->m_timeUserID = m_wCurrentUser;
		if(pCallScore->cbCallInfo == CB_CALL_BENKER)
		{//叫地主
			timeControl->m_timeNum	 = m_cbTimeRodBanker;
		}
		else
		{
			timeControl->m_timeNum	 = m_cbTimeCallBanker;
		}
		timeControl->m_gameState = DDZGame_landLord;
		if (!m_isCallLord)
		{
			timeControl->m_timeKey     = 1;
		}
		else
		{
			timeControl->m_timeKey     = 3;
		}
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,timeControl);
	}
	return true;
}

bool DDZSceneLogic::onRobLord(const void * pBuffer, WORD wDataSize)
{
	if (wDataSize!=sizeof(CMD_S_RodBanker))
	{
		CCLOG("rob lord data is wrong");
		return false;
	}
	m_eGameState = DDZGame_landLord;

	//变量定义
	CMD_S_RodBanker * pCallScore=(CMD_S_RodBanker *)pBuffer;
	//m_bLastCallScore = m_bLastCallScore == 0?1:m_bLastCallScore * 2;
	//if (pCallScore->cbRodlInfo == 1)
	//{//叫地主
	//	m_bLastCallScore = m_bLastCallScore == 0?1:m_bLastCallScore * 2;
	//}
	//else
	//{
	//	m_bLastCallScore = 0;
	//}
	m_bLastCallScore = pCallScore->wRodBankerTime;//抢地主倍数

	m_beilv = pCallScore->wRodBankerTime;

	//倍率变化
	DDZCCbeilv*_beidata =DDZCCbeilv::create(DDZmsgToView_beiLv);
	_beidata->m_base = m_lCellScore;
	_beidata->m_beilv = m_beilv;
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_beidata);

	if (pCallScore->wLastUser != INVALID_WORD)
	{
		//首次抢地主 INVALID_WORD
		m_wTempUser = switchViewChairID(pCallScore->wLastUser);
		DDZCClandLord* landLord = DDZCClandLord::create(DDZmsgToView_landLord);
		landLord->m_landID = m_wTempUser;
		landLord->m_isLand = true;
		if(pCallScore->cbRodlInfo == CB_ROD_BANKER){//抢地主
			m_isCallLord = true;
			landLord->m_LandKey = 2;
		}else{//不抢
			landLord->m_LandKey = 1;
		}
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,landLord);
	}

	if (pCallScore->wCurrentUser !=INVALID_WORD)
	{
		m_wCurrentUser=switchViewChairID(pCallScore->wCurrentUser);
		//抢地主成功 INVALID_WORD
		DDZCCtimeControl * timeControl=DDZCCtimeControl::create(DDZmsgToView_time);
		timeControl->m_timeUserID = m_wCurrentUser;
		timeControl->m_timeNum	 = m_cbTimeRodBanker;
		timeControl->m_gameState = DDZGame_landLord;
		if (!m_isCallLord)
		{
			timeControl->m_timeKey     = 1;
		}
		else
		{
			timeControl->m_timeKey     = 3;
		}
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,timeControl);
	}
	return true;
}

bool DDZSceneLogic::onSubLandScore( const void * pBuffer, WORD wDataSize )
{
	if (wDataSize!=sizeof(DDZCMD_S_CallScore))
	{
		CCLOG("叫分数据校验错误！");
		return false;
	}
	m_eGameState = DDZGame_landLord;
	//变量定义
	DDZCMD_S_CallScore * pCallScore=(DDZCMD_S_CallScore *)pBuffer;
	m_bLastCallScore = pCallScore->cbUserCallScore==255?m_bLastCallScore:pCallScore->cbUserCallScore;
	//CMD_S_LandScore* _data =(CMD_S_LandScore*)pBuffer;
	CCLOG("==============================");
	CCLOG("pCallScore->wCurrentUser = %d", pCallScore->wCurrentUser);
	CCLOG("pCallScore->wCallScoreUser = %d", pCallScore->wCallScoreUser);
	CCLOG("pCallScore->cbCurrentScore = %d", pCallScore->cbCurrentScore);
	CCLOG("pCallScore->cbUserCallScore = %d", pCallScore->cbUserCallScore);
	CCLOG("==============================");
	//倍率变化
	DDZCCbeilv*_beidata =DDZCCbeilv::create(DDZmsgToView_beiLv);
	_beidata->m_base = m_lCellScore;
	_beidata->m_beilv = 1;
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_beidata);
	if (pCallScore->wCallScoreUser!=INVALID_WORD)
	{
		//首次抢地主 INVALID_WORD
		m_wTempUser = switchViewChairID(pCallScore->wCallScoreUser);
		DDZCClandLord* landLord = DDZCClandLord::create(DDZmsgToView_landLord);
		landLord->m_landID = m_wTempUser;
		landLord->m_isLand = true;
		if(pCallScore->cbUserCallScore>=1&&pCallScore->cbUserCallScore<=3){
			m_isCallLord = true;
			landLord->m_LandKey = 2;
		}else{
			landLord->m_LandKey = 1;
		}
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,landLord);
	}

	if (pCallScore->wCurrentUser !=INVALID_WORD)
	{
		m_wCurrentUser=switchViewChairID(pCallScore->wCurrentUser);
		//抢地主成功 INVALID_WORD
		DDZCCtimeControl * timeControl=DDZCCtimeControl::create(DDZmsgToView_time);
		timeControl->m_timeUserID = m_wCurrentUser;
		timeControl->m_timeNum	 = 20;
		timeControl->m_gameState = DDZGame_landLord;
		if (!m_isCallLord)
		{
			timeControl->m_timeKey     = 1;
		}
		else
		{
			timeControl->m_timeKey     = 3;
		}
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,timeControl);
	}
	return true;
}

bool DDZSceneLogic::onSubGameStart( const void * pBuffer, WORD wDataSize )
{
	//m_beilv = 1;
	if (wDataSize!=sizeof(DDZCMD_S_BankerInfo))
	{
		CCLOG("onSubGameStart  wrong data");
		return false;
	}
	DDZCMD_S_BankerInfo * pGameStart=(DDZCMD_S_BankerInfo *)pBuffer;

	if(m_bLastCallScore<1)
	{
		m_bLastCallScore = 1;
		DDZCCbeilv*_beidata =DDZCCbeilv::create(DDZmsgToView_beiLv);
		_beidata->m_base = m_lCellScore;
		_beidata->m_beilv = m_beilv;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_beidata);
	}
	WORD chairID = EntityMgr::instance()->getDispatch()->m_wChairID;
	m_wCurrentUser = switchViewChairID(pGameStart->wCurrentUser);
	m_nBankerUser = switchViewChairID(pGameStart->wBankerUser);
	//底牌变化 
	memcpy(m_BackCardData,pGameStart->cbBankerCard,sizeof(BYTE)*DDZGAME_PLAYER);
	memcpy(&m_handCardData[m_nBankerUser][17],pGameStart->cbBankerCard,sizeof(BYTE)*DDZGAME_PLAYER);
	m_handCardNum[m_nBankerUser]+=3;
	DDZDDZLogic _logic;
	if (m_wCurrentUser == m_nBankerUser)
	{
		_logic.sortCardList(m_handCardData[m_nBankerUser],m_handCardNum[m_nBankerUser],ST_ORDER);
	}
	else
	{
		_logic.sortCardList(m_handCardData[m_wCurrentUser],m_handCardNum[m_wCurrentUser],ST_ORDER);
	}

	//手牌变化
	for (int i =0;i<DDZGAME_PLAYER ;i++)
	{
		DDZCCsendCard* msgdata=DDZCCsendCard::create(DDZmsgToView_sendCard);
		memcpy(msgdata->m_handCardData,m_handCardData[i],sizeof(BYTE)*MAX_COUNT);
		msgdata->m_handCardNum = m_handCardNum[i];
		msgdata->m_bOpen =m_wOpenUser[i];
		msgdata->m_bEffect =false;
		msgdata->m_userID =i;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,msgdata);
	}
	
	//--玩家出牌
	DDZCCtimeControl * timeControl=DDZCCtimeControl::create(DDZmsgToView_time);
	timeControl->m_timeUserID = m_wCurrentUser;
	timeControl->m_timeNum	 = m_cbTimeHeadOutCard;
	timeControl->m_gameState = DDZGame_Run;
	if (m_wOpenUser[m_wCurrentUser] )
		timeControl->m_timeKey =1;	//不能明牌
	else
		timeControl->m_timeKey   = 0;	//能明牌
	timeControl->m_timeKey =1;	//不能明牌
	timeControl->m_BankUser=m_nBankerUser;
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,timeControl);
	

	//m_DDZSceneView->setBankId(m_nBankerUser);//设置地主id

	//--加倍请求
	//CCaddView* _addView =CCaddView::create(msgToView_addView);
	//_addView->m_addID = 1;
	//_addView->m_AddType =3;
	//NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_addView);

	//--底牌变化 //--最后手牌任务
	DDZCCbackData* _backData =DDZCCbackData::create(DDZmsgToView_backCard);
	memcpy(_backData->m_data,pGameStart->cbBankerCard,sizeof(BYTE)*DDZGAME_PLAYER);
	_backData->m_taskKey = NULL;
	_backData->m_isNew=true;
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_backData); 

	//地主信息变化
	DDZCClandView*  _userinfor=DDZCClandView::create(DDZmsgToview_LordView);
	_userinfor->m_landUserID =m_nBankerUser;
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_userinfor); 

	/*
	//add jiabei view 加倍界面展示
	DDZCCaddView* _addView =DDZCCaddView::create(DDZmsgToView_addView);
	_addView->m_addID = 1;
	_addView->m_AddType =3;
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_addView);
	

	//--玩家加倍，时钟设置
	DDZCCtimeControl * timeControl=DDZCCtimeControl::create(DDZmsgToView_time);
	timeControl->m_timeUserID = m_wCurrentUser;
	timeControl->m_timeNum	 = 5;
	//DDZGame_Run
	timeControl->m_gameState = DDZGame_jiaBei;
	//if (m_wOpenUser[m_wCurrentUser] )
	//	timeControl->m_timeKey =1;	//不能明牌
	//else
	//	timeControl->m_timeKey   = 0;	//能明牌
	timeControl->m_timeKey =1;	//不能明牌
	timeControl->m_BankUser=m_nBankerUser;
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,timeControl);
	*/
	return true;
}

bool DDZSceneLogic::onSubOutCard( const void * pBuffer, WORD wDataSize )
{
	DDZCMD_S_OutCard * pOutCard=(DDZCMD_S_OutCard *)pBuffer;

	m_eGameState = DDZGame_Run;
	m_wTempUser =switchViewChairID(pOutCard->wOutCardUser);
/*
	if (pOutCard->wCurrentUser == INVALID_CHAIR)
	{
		//	游戏结束了
		return false;
	}
*/
	m_wCurrentUser =switchViewChairID(pOutCard->wCurrentUser);

	m_outCardNum = pOutCard->cbCardCount;
	memcpy(m_outCardData,pOutCard->cbCardData, m_outCardNum);

	CCLOG("==============================");
	CCLOG("pCallScore->wCurrentUser = %d", pOutCard->wCurrentUser);
	CCLOG("pCallScore->wOutCardUser = %d", pOutCard->wOutCardUser);
	CCLOG("pCallScore->cbCardCount = %d", pOutCard->cbCardCount);
	for(int i = 0 ; i < pOutCard->cbCardCount ; i++){
		CCLOG("pCallScore->cbCardData[%d] = %d", i, pOutCard->cbCardData[i]);
	}
	CCLOG("==============================");

	//--出牌表现,自己的先表现
	if (m_wTempUser!=1)
	{
		DDZCCoutCard* _outCard =DDZCCoutCard::create(DDZmsgToView_outCard);
		_outCard->m_outUser = m_wTempUser;
		_outCard->m_numCard =m_outCardNum;
		memcpy(_outCard->m_outkCard,pOutCard->cbCardData,_outCard->m_numCard);
		_outCard->m_bankUser=m_nBankerUser;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_outCard);
	}

	//--减掉手牌
	DDZDDZLogic _logic;
	_logic.removeCard(m_outCardData,m_outCardNum,m_handCardData[m_wTempUser],m_handCardNum[m_wTempUser]);
	m_handCardNum[m_wTempUser]-=m_outCardNum;
	unsigned char cardType = _logic.getCardType(m_outCardData, m_outCardNum);
	if(cardType == CT_BOMB_CARD||cardType == CT_MISSILE_CARD)
	{
		DDZCCbeilv*_beidata =DDZCCbeilv::create(DDZmsgToView_beiLv);
		_beidata->m_base = m_lCellScore;
		m_beilv = m_beilv*2;
		_beidata->m_beilv = m_beilv;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_beidata);
	}

	//--手牌变化
	for (int i =0;i<DDZGAME_PLAYER ;i++)
	{
		if (m_wTempUser!=1 && i==1)		//--自己手牌只有自己出牌时才变化
			continue;

		DDZCCsendCard* msgdata=DDZCCsendCard::create(DDZmsgToView_sendCard);
		memcpy(msgdata->m_handCardData,m_handCardData[i],sizeof(BYTE)*MAX_COUNT);
		msgdata->m_handCardNum = m_handCardNum[i];
		msgdata->m_bOpen =m_wOpenUser[i];
		msgdata->m_bEffect =false;
		msgdata->m_userID =i;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,msgdata);
	}

/*
	if(m_wTempUser == m_wCurrentUser){
		m_outCardNum =0;
		memset(m_outCardData,0,sizeof(BYTE)*MAX_COUNT);
	}
*/
	m_outCardNum =0;
	memset(m_outCardData,0,sizeof(BYTE)*MAX_COUNT);

	//--下家
	if (m_handCardNum[0]>0 && m_handCardNum[1]>0 && m_handCardNum[2]>0)
	{
		DDZCCtimeControl * timeControl=DDZCCtimeControl::create(DDZmsgToView_time);
		timeControl->m_timeUserID = m_wCurrentUser;
		timeControl->m_timeNum	 = m_cbTimeOutCard;
		timeControl->m_gameState = DDZGame_Run;
		if(m_wTempUser == m_wCurrentUser){
			timeControl->m_timeKey  =2;
		}else{
			timeControl->m_timeKey  =3;
		}
		timeControl->m_BankUser=m_nBankerUser;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,timeControl);
	}

	//--加倍隐藏设置
	//if (m_isOut == false)
	//{
	//	m_isOut =true;
	//	DDZCCaddView* _addView =DDZCCaddView::create(DDZmsgToView_addView);
	//	_addView->m_addID = 1;
	//	_addView->m_AddType =2;
	//	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_addView);
	//}
	return true;
}

bool DDZSceneLogic::onSubPassCard( const void * pBuffer, WORD wDataSize )
{
	DDZCMD_S_PassCard* pPassCard=(DDZCMD_S_PassCard *)pBuffer;
	m_wTempUser =switchViewChairID(pPassCard->wPassCardUser);
	m_wCurrentUser =switchViewChairID(pPassCard->wCurrentUser);

	//--弃牌表现
	DDZCCpassCard* _passCard =DDZCCpassCard::create(DDZmsgToView_passCard);
	_passCard->m_passUser =m_wTempUser;
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_passCard);

	//--一轮胜利,出牌跟新
	if (pPassCard->cbTurnOver == 1)
	{
		//		该方法 应该 没用 
		m_outCardNum =0;
		memset(m_outCardData,0,sizeof(BYTE)*MAX_COUNT);

		DDZCCoutCard* _outCard =DDZCCoutCard::create(DDZmsgToView_outCard);
		_outCard->m_outUser = m_wTempUser;
		_outCard->m_numCard =0;
		_outCard->m_bankUser=m_nBankerUser;
		memset(_outCard->m_outkCard,0,MAX_COUNT);
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_outCard);
	}

	//--下家
	DDZCCtimeControl * timeControl=DDZCCtimeControl::create(DDZmsgToView_time);
	timeControl->m_timeUserID = m_wCurrentUser;
	timeControl->m_timeNum	 = m_cbTimeOutCard;
	timeControl->m_gameState = DDZGame_Run;
	timeControl->m_timeKey =pPassCard->cbTurnOver==1?2:3;
	timeControl->m_BankUser=m_nBankerUser;
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,timeControl);

	return true;
}

bool DDZSceneLogic::onSubGameEnd( const void * pBuffer, WORD wDataSize )
{
	//CMD_S_GameConclude* pGameEnd = (CMD_S_GameConclude*)pBuffer;

	if (wDataSize!=sizeof(DDZCMD_S_GameEnd))
	{
		CCLOG("DDZSceneLogic::onSubGameEnd  recv data error");
		return false;
	}

	DDZCMD_S_GameEnd* pEnd = (DDZCMD_S_GameEnd*)pBuffer;
	DDZCCgameOver* pOver = DDZCCgameOver::create(DDZmsgToView_gameOver);
	tagUserData* pUserData = EntityMgr::instance()->roomFrame()->getMeUserDate();
	if (pUserData)
	{
		pOver->m_UserID = pUserData->dwUserID;
		pOver->m_isPochang = pUserData->lScore <= 0 ? true : false;
	}
	pOver->m_gameEndData = pEnd;
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW, pOver);

	if (pEnd->bFanChunTian || pEnd->bChunTian)
	{
		m_beilv *= 2;
		DDZCCbeilv*_beidata =DDZCCbeilv::create(DDZmsgToView_beiLv);
		_beidata->m_base = m_lCellScore;
		_beidata->m_beilv = m_beilv;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_beidata);
	}

	m_bLastCallScore = 0;
	m_beilv = 0;
	m_eGameState = DDZGame_Over;

	return true;
}

bool DDZSceneLogic::onUserOnLine( const void * pBuffer, WORD wDataSize )
{
	if (wDataSize!=sizeof(DDZCMD_S_UserOffLine))return false;
	DDZCMD_S_UserOffLine* _data =(DDZCMD_S_UserOffLine*)pBuffer;

	//--玩家断线提醒
	DDZCConline * _onLineData =DDZCConline::create(DDZmsgToView_OnLine);
	_onLineData->m_isOnline = _data->m_isOnline;
	_onLineData->m_userID =switchViewChairID(_data->m_userID);
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_onLineData);
	return true;
}

bool DDZSceneLogic::onUserChat( const void * pBuffer, WORD wDataSize )
{
	if(wDataSize<sizeof(CMD_GR_S_UserChat)-sizeof(TCHART)*LEN_USER_CHAT)
	{
		return false;
	}
	CMD_GR_S_UserChat* pExpression = (CMD_GR_S_UserChat*)pBuffer;
	DDZCCfaceAndVioce * _faceData=DDZCCfaceAndVioce::create(DDZmsgToView_DDZVoiceAndFace);
	_faceData->m_KeyID =-1;
	_faceData->m_Type =2;
	_faceData->m_UserID =getUserRealChairID(pExpression->dwSendUserID);
	memset(_faceData->voice, 0, sizeof(_faceData->voice));
	MyNSString::wc2mbs(pExpression->szChatString, pExpression->wChatLength, _faceData->voice);
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_faceData);
	return true;
}

bool DDZSceneLogic::onUserExpression( const void * pBuffer, WORD wDataSize )
{
	if(wDataSize<sizeof(CMD_GR_S_UserExpression))
	{
		return false;
	}
	CMD_GR_S_UserExpression* pExpression = (CMD_GR_S_UserExpression*)pBuffer;
	DDZCCfaceAndVioce * _faceData=DDZCCfaceAndVioce::create(DDZmsgToView_DDZVoiceAndFace);
	_faceData->m_KeyID =pExpression->wItemIndex;
	_faceData->m_Type =1;
	_faceData->m_UserID =getUserRealChairID(pExpression->dwSendUserID);
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_faceData);
	return true;
}

bool DDZSceneLogic::onRoomLimit( const void * pBuffer, WORD wDataSize )
{
#if 0
	CMD_GR_RoomScoreLimit* limitData=(CMD_GR_RoomScoreLimit*)pBuffer;
	CCRoomLimit* _data=CCRoomLimit::create(msgToView_RoomLimit);

	g_GlobalUnits.getGolbalUserData()->lScore=limitData->lScore;//--更新大厅分数				--牌队伍之前没有更新人物数据

	if (limitData->lScore < limitData->lMinScore)    //--分数太低了
	{
		tagUserData* pUserDate = getMeUserDate();
		if (pUserDate)
		{
			ConfigMgr::instance()->m_Config.m_isGameBankrupcy=(getMeUserDate()->lScore-ConfigMgr::instance()->m_Config.m_nGolbalBankruptcy)<=0?true:false;	//--是否破产
			_data->m_RoomLimitKey=ConfigMgr::instance()->m_Config.m_isGameBankrupcy?0:1;			//0破产1.太低
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_data);
			return true;
		}
		else
		{
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,Integer::create(msgToView_socketoff));
			return true;
		}
	}
	else	if (limitData->lScore > limitData->lMaxScore)    //--分数太高了
	{
		_data->m_RoomLimitKey=2;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_data);
		return true;
	}
#endif
	return true;
}

void DDZSceneLogic::onLinkResult( Object* obj )
{
	Integer* Interger = (Integer*)obj;

	switch (Interger->getValue())
	{
	case 0:			// 0为连接失败 
		break;
	case 1:			// 1为连接成功
		{
			//EntityMgr::instance()->getDispatch()->sendLoginPacket();

			DDZSceneControl* pDDZSceneControl = DDZSceneControl::sharedDDZSceneControl();
			Scene* pScene = pDDZSceneControl->getScene(SCENE_game,true);
			Director::getInstance()->replaceScene(pScene);
		}
		break;
	case 2:			// 2为没有服务器
		{
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,Integer::create(DDZmsgToView_socketoff));
		}
		break;
	default:
		break;
	}
}

void DDZSceneLogic::onUpdateScoreAndGoldEgg( Object* obj )
{
	sendSeverData(SUB_GF_ROOM_SCOREUPDATA, 0, 0);
}

int DDZSceneLogic::getUserRealChairID(DWORD userID)
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

void DDZSceneLogic::studioUI(Node* pParent, Node* pChild)
{
	pParent->runAction(Sequence::create(DelayTime::create(2), CallFunc::create(([=]{
		auto ws = Director::getInstance()->getWinSize();

		auto layer = LayerColor::create();
		Director::getInstance()->getRunningScene()->addChild(layer);

		auto sp = Sprite::createWithSpriteFrameName("ddz_btn_buy_selected.png");
		layer->addChild(sp);
		sp->setPosition(ws/2);
		layer->setName("studioUI");

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = [=](Touch* touch, Event* event){
			if(!pChild){
				return false;
			}
			auto target = event->getCurrentTarget();
			if(!target){
				return false;
			}
			Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
			cocos2d::Size s = target->getContentSize();
			Rect rect = Rect(0, 0, s.width, s.height);

			if (rect.containsPoint(locationInNode))
			{
				return true;
			}
			return false;
		};
		listener->onTouchMoved = [=](Touch* touch, Event* event){
			pChild->setPosition(pChild->getPosition() + touch->getDelta());
			Vec2 vPos = pChild->getPosition();
			cocos2d::Size sParentSize = pParent->getContentSize();
			float fPercentX = vPos.x / sParentSize.width;
			float fPercentY = vPos.y / sParentSize.height;
			log("pos  Vec2(%f,%f);", vPos.x, vPos.y);
			log("percent	(X = %f, Y = %f)", fPercentX, fPercentY);
		};
		listener->onTouchEnded = [=](Touch* touch, Event* event){
			pChild->setPosition(pChild->getPosition() + touch->getDelta());
			Vec2 vPos = pChild->getPosition();
			cocos2d::Size sParentSize = pParent->getContentSize();
			float fPercentX = vPos.x / sParentSize.width;
			float fPercentY = vPos.y / sParentSize.height;
			log("pos  Vec2(%f,%f);", vPos.x, vPos.y);
			log("percent	(X = %f, Y = %f)", fPercentX, fPercentY);
		};
		layer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, sp);

	})), nullptr));
}


DDZGame_State DDZSceneLogic::getGameState() const
{
	return m_eGameState;
}

bool DDZSceneLogic::OnGameSceneRobLord(const void *pBuffer, WORD wDataSize)
{
	CCLOG("DDZSceneLogic::OnGameSceneRobLord");

	if (wDataSize!=sizeof(DDZCMD_S_StatusRod)) return false;
	DDZCMD_S_StatusRod* pStatusPlay=(DDZCMD_S_StatusRod *)pBuffer;

	m_lCellScore = pStatusPlay->lCellScore;
	m_cbTimeCallBanker = pStatusPlay->cbTimeCallBanker;
	m_cbTimeRodBanker = pStatusPlay->cbTimeRodBanker;
	m_cbTimeOutCard = pStatusPlay->cbTimeOutCard;
	m_cbTimeStartGame = pStatusPlay->cbTimeStartGame;
	m_cbTimeHeadOutCard = pStatusPlay->cbTimeHeadOutCard;


	//================显示叫地主阶段界面===================================

	m_isCallLord = false;

	//===============展示 桌子上的牌========================

	//---游戏开始表现
	WORD chairID = EntityMgr::instance()->getDispatch()->m_wChairID;

	//	发牌
	m_wCurrentUser = switchViewChairID(chairID);

	//抢地主
	m_isCallLord = true;
	m_DDZSceneView->setIsFistLandLord(false);

	
	//展示之前的玩家叫地主信息
	for (auto i = 0; i < DDZGAME_PLAYER; i++)
	{
		if (i != pStatusPlay->wCurrentUser)
		{//不是当前操作玩家
			if(pStatusPlay->cbRodBankerInfo[i] == CB_NO_ROD_BANKER){//不抢地主
				//展示对应玩家不叫的文字
				int tempUser = switchViewChairID(i);
				DDZCClandLord* landLord = DDZCClandLord::create(DDZmsgToView_landLord);
				landLord->m_landID = tempUser;
				landLord->m_isLand = true;
				landLord->m_LandKey = 1;
				NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,landLord);
			}
			else if (pStatusPlay->cbRodBankerInfo[i] == CB_ROD_BANKER)//抢地主
			{
				int tempUser = switchViewChairID(i);
				DDZCClandLord* landLord = DDZCClandLord::create(DDZmsgToView_landLord);
				landLord->m_landID = tempUser;
				landLord->m_isLand = true;
				landLord->m_LandKey = 2;
				NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,landLord);
			}

		}
	}
	

	//发牌
	for (auto i = 0; i < DDZGAME_PLAYER; i++)
	{
		m_handCardNum[i] = NORMAL_COUNT;
		if (m_wCurrentUser == i)
		{
			memset(m_handCardData[i], 0, sizeof(BYTE) * MAX_COUNT);
			memcpy(m_handCardData[i], pStatusPlay->cbHandCardData[chairID], sizeof(BYTE) * NORMAL_COUNT);
			DDZDDZLogic _logic;
			_logic.sortCardList(m_handCardData[i],m_handCardNum[i],ST_ORDER);
		}
		DDZCCsendCard* pSendCard = DDZCCsendCard::create(DDZmsgToView_sendCard);
		memcpy(pSendCard->m_handCardData, m_handCardData[m_wCurrentUser], sizeof(BYTE) * MAX_COUNT);
		pSendCard->m_bOpen = false;
		pSendCard->m_bEffect = false;
		pSendCard->m_handCardNum = m_handCardNum[i];
		pSendCard->m_userID = i;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW, pSendCard);
	}

	//===============================================倍率=====叫，抢地主表现======================================================
	m_bLastCallScore = pStatusPlay->lCellScore;
	m_beilv = pStatusPlay->wRodBankerTime * pStatusPlay->wStartTime;
	DDZCCbeilv*_beidata =DDZCCbeilv::create(DDZmsgToView_beiLv);
	_beidata->m_base = m_lCellScore;
	_beidata->m_beilv = m_beilv;
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_beidata);
	/*
	if (m_bLastCallScore != INVALID_WORD && m_bLastCallScore >= 1)
	{
		//倍率变化
		DDZCCbeilv*_beidata =DDZCCbeilv::create(DDZmsgToView_beiLv);
		_beidata->m_base = m_bLastCallScore;
		_beidata->m_beilv = 1;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_beidata);
	}
	*/
	if (pStatusPlay->wCurrentUser !=INVALID_WORD)//展示按钮
	{
		m_wCurrentUser=switchViewChairID(pStatusPlay->wCurrentUser);
		//抢地主成功 INVALID_WORD
		DDZCCtimeControl * timeControl=DDZCCtimeControl::create(DDZmsgToView_time);
		timeControl->m_timeUserID = m_wCurrentUser;
		timeControl->m_timeNum	 = m_cbTimeRodBanker;
		timeControl->m_gameState = DDZGame_landLord;
		if (!m_isCallLord)
		{
			timeControl->m_timeKey     = 1;
		}
		else
		{
			timeControl->m_timeKey     = 3;
		}
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,timeControl);
	}

	EntityMgr::instance()->getDispatch()->SendPacketWithRequestMyTableUserInfo(DDZGAME_PLAYER);

	return true;
}


bool DDZSceneLogic::OnGameSceneCallLord(const void *pBuffer, WORD wDataSize)
{
	CCLOG("DDZSceneLogic::OnGameSceneCallLord");

	if (wDataSize!=sizeof(DDZCMD_S_StatusCall)) return false;
	DDZCMD_S_StatusCall* pStatusPlay=(DDZCMD_S_StatusCall *)pBuffer;

	m_lCellScore = pStatusPlay->lCellScore;
	m_cbTimeCallBanker = pStatusPlay->cbTimeCallBanker;
	m_cbTimeRodBanker = pStatusPlay->cbTimeRodBanker;
	m_cbTimeOutCard = pStatusPlay->cbTimeOutCard;
	m_cbTimeStartGame = pStatusPlay->cbTimeStartGame;
	m_cbTimeHeadOutCard = pStatusPlay->cbTimeHeadOutCard;


	//================显示叫地主阶段界面===================================

	m_isCallLord = false;

	//===============展示 桌子上的牌========================

	//---游戏开始表现
	WORD chairID = EntityMgr::instance()->getDispatch()->m_wChairID;

	//	发牌
	m_wCurrentUser = switchViewChairID(chairID);

	//当前为叫地主
	m_isCallLord = false;
	m_DDZSceneView->setIsFistLandLord(true);

	
	//展示之前的玩家叫地主信息
	for (auto i = 0; i < DDZGAME_PLAYER; i++)
	{
		if (i != pStatusPlay->wCurrentUser)
		{//不是当前操作玩家
			if(pStatusPlay->cbCallBankerInfo[i] == CB_NO_CALL_BENKER){//不叫地主
				//展示对应玩家不叫的文字
				int tempUser = switchViewChairID(i);
				DDZCClandLord* landLord = DDZCClandLord::create(DDZmsgToView_landLord);
				landLord->m_landID = tempUser;
				landLord->m_isLand = true;
				landLord->m_LandKey = 1;
				NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,landLord);
			}
		}
	}
	

	//发牌
	for (auto i = 0; i < DDZGAME_PLAYER; i++)
	{
		m_handCardNum[i] = NORMAL_COUNT;
		if (m_wCurrentUser == i)
		{
			memset(m_handCardData[i], 0, sizeof(BYTE) * MAX_COUNT);
			memcpy(m_handCardData[i], pStatusPlay->cbHandCardData[chairID], sizeof(BYTE) * NORMAL_COUNT);
			DDZDDZLogic _logic;
			_logic.sortCardList(m_handCardData[i],m_handCardNum[i],ST_ORDER);
		}
		DDZCCsendCard* pSendCard = DDZCCsendCard::create(DDZmsgToView_sendCard);
		memcpy(pSendCard->m_handCardData, m_handCardData[m_wCurrentUser], sizeof(BYTE) * MAX_COUNT);
		pSendCard->m_bOpen = false;
		pSendCard->m_bEffect = false;
		pSendCard->m_handCardNum = m_handCardNum[i];
		pSendCard->m_userID = i;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW, pSendCard);
	}

	//===============================================倍率=====叫，抢地主表现======================================================
	m_bLastCallScore = 0;
	m_beilv = pStatusPlay->wStartTime;
	//倍率变化
	DDZCCbeilv*_beidata =DDZCCbeilv::create(DDZmsgToView_beiLv);
	_beidata->m_base = m_lCellScore;
	_beidata->m_beilv = m_beilv;
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_beidata);
	/*
	if (m_bLastCallScore != INVALID_WORD && m_bLastCallScore >= 1)
	{
		//倍率变化
		DDZCCbeilv*_beidata =DDZCCbeilv::create(DDZmsgToView_beiLv);
		_beidata->m_base = 1;
		_beidata->m_beilv = 1;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_beidata);
	}
	*/

	if (pStatusPlay->wCurrentUser !=INVALID_WORD)//展示按钮
	{
		m_wCurrentUser=switchViewChairID(pStatusPlay->wCurrentUser);
		//抢地主成功 INVALID_WORD
		DDZCCtimeControl * timeControl=DDZCCtimeControl::create(DDZmsgToView_time);
		timeControl->m_timeUserID = m_wCurrentUser;
		timeControl->m_timeNum	 = m_cbTimeCallBanker;
		timeControl->m_gameState = DDZGame_landLord;
		if (!m_isCallLord)
		{
			timeControl->m_timeKey     = 1;
		}
		else
		{
			timeControl->m_timeKey     = 3;
		}
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,timeControl);
	}

	EntityMgr::instance()->getDispatch()->SendPacketWithRequestMyTableUserInfo(DDZGAME_PLAYER);

	return true;
}

bool DDZSceneLogic::OnGameScenePlay(const void *pBuffer, WORD wDataSize)
{
	CCLOG("DDZSceneLogic::OnGameScenePlay");
	if (wDataSize!=sizeof(DDZ_CMD_S_StatusPlay)) return false;
	DDZ_CMD_S_StatusPlay* pStatusPlay=(DDZ_CMD_S_StatusPlay *)pBuffer;

	m_lCellScore = pStatusPlay->lCellScore;
	m_cbTimeCallBanker = pStatusPlay->cbTimeCallBanker;
	m_cbTimeRodBanker = pStatusPlay->cbTimeRodBanker;
	m_cbTimeOutCard = pStatusPlay->cbTimeOutCard;
	m_cbTimeStartGame = pStatusPlay->cbTimeStartGame;
	m_cbTimeHeadOutCard = pStatusPlay->cbTimeHeadOutCard;

	//============展示 桌子上的牌=======================

	//--游戏开始表现
	WORD chairID = EntityMgr::instance()->getDispatch()->m_wChairID;

	//	发牌
	m_wCurrentUser = switchViewChairID(chairID);
	int temp = pStatusPlay->wCurrentUser;
	//m_wCurrentUser = switchViewChairID(pStatusPlay->wCurrentUser);
	for (auto i = 0; i < DDZGAME_PLAYER; i++)
	{
		/*m_handCardNum[i] = pStatusPlay->cbHandCardCount[i];*/
		int tempNumIndex = 0;	//自己数据 的位置
		if (i == 1)
		{
			tempNumIndex = chairID;

		}
		if (i == 2)
		{
			tempNumIndex = (chairID + 1) % 3;

		}
		if (i == 0)
		{
			tempNumIndex = (chairID + 2) % 3;
		}
		m_handCardNum[i] = pStatusPlay->cbHandCardCount[tempNumIndex];

		if (m_wCurrentUser == i)
		{
			memset(m_handCardData[i], 0, sizeof(BYTE) * MAX_COUNT);
			memcpy(m_handCardData[i], pStatusPlay->cbHandCardData[chairID], sizeof(BYTE) * MAX_COUNT);
			DDZDDZLogic _logic;
			_logic.sortCardList(m_handCardData[i],m_handCardNum[i],ST_ORDER);
		}
		DDZCCsendCard* pSendCard = DDZCCsendCard::create(DDZmsgToView_sendCard);
		memcpy(pSendCard->m_handCardData, m_handCardData[m_wCurrentUser], sizeof(BYTE) * MAX_COUNT);
		pSendCard->m_bOpen = false;
		pSendCard->m_bEffect = false;
		pSendCard->m_handCardNum = m_handCardNum[i];
		pSendCard->m_userID = i;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW, pSendCard);
	}

	//======================初始化信息=================
	m_outCardNum = pStatusPlay->cbTurnCardCount;
	m_wCurrentUser =switchViewChairID(pStatusPlay->wCurrentUser);
	m_wTempUser =switchViewChairID(pStatusPlay->wTurnWiner);
	m_nBankerUser = switchViewChairID(pStatusPlay->wBankerUser);
	int bombCount = pStatusPlay->cbBombCount;;
	//m_beilv = pStatusPlay->cbBombCount;
	memcpy(m_outCardData,pStatusPlay->cbTurnCardData,m_outCardNum);

	//====================底分====倍率==============
	m_bLastCallScore = pStatusPlay->lCellScore;
	int bei = pStatusPlay->wStartTime * pStatusPlay->wRodBankerTime;
	for (auto i = 0; i < bombCount; i++)
	{
		bei = bei * 2;
	}
	m_beilv = bei;
	if (m_bLastCallScore != INVALID_WORD && m_bLastCallScore >= 1)
	{
		//倍率变化
		DDZCCbeilv*_beidata =DDZCCbeilv::create(DDZmsgToView_beiLv);
		_beidata->m_base = m_lCellScore;
		_beidata->m_beilv = bei;		//需要返回，游戏由于叫抢地主，基础倍数的变化，不仅仅是炸弹导致饿倍数问题
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_beidata);
	}

	//======================人物形象=================================
	//地主信息变化
	DDZCClandView*  _userinfor=DDZCClandView::create(DDZmsgToview_LordView);
	_userinfor->m_landUserID =m_nBankerUser;
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_userinfor); 

	//=======================底牌表现======================
	//--底牌变化 //--最后手牌任务
	DDZCCbackData* _backData =DDZCCbackData::create(DDZmsgToView_backCard);
	memcpy(_backData->m_data,pStatusPlay->cbBankerCard,sizeof(BYTE)*DDZGAME_PLAYER);
	_backData->m_taskKey = NULL;
	_backData->m_isNew=true;
	NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_backData); 

	

	//=======================最后打出的牌==================
	
	if (m_wTempUser!=1)
	{
		DDZCCoutCard* _outCard =DDZCCoutCard::create(DDZmsgToView_outCard);
		_outCard->m_outUser = m_wTempUser;
		_outCard->m_numCard =m_outCardNum;
		memcpy(_outCard->m_outkCard,pStatusPlay->cbTurnCardData,_outCard->m_numCard);
		_outCard->m_bankUser=m_nBankerUser;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_outCard);
	}

	//======================按钮显示=======================
	//wTurnWiner
	//--下家
	if (m_handCardNum[0]>0 && m_handCardNum[1]>0 && m_handCardNum[2]>0)
	{
		DDZCCtimeControl * timeControl=DDZCCtimeControl::create(DDZmsgToView_time);
		timeControl->m_timeUserID = m_wCurrentUser;
		timeControl->m_timeNum	 = m_cbTimeOutCard;
		timeControl->m_gameState = DDZGame_Run;
		if(m_wTempUser == m_wCurrentUser){
			timeControl->m_timeKey  =2;
		}else{
			timeControl->m_timeKey  =3;
		}
		timeControl->m_BankUser=m_nBankerUser;
		NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,timeControl);
	}
	EntityMgr::instance()->getDispatch()->SendPacketWithRequestMyTableUserInfo(DDZGAME_PLAYER);
}


