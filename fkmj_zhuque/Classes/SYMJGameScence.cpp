#include "SYMJGameScence.h"
#include "SYMJPlayer.h"
#include "GameLib.h"
#include "CMD_SYMJ.h"
#include "SYMJPlayer.h"
#include "SYMJGameLogic.h"
#include "SYMJSoundFun.h"
#include "ScriptXMLparse.h"

FV_SINGLETON_STORAGE(SYMJGameScence);

SYMJGameScence::SYMJGameScence()
	:m_iBankerUser(0)
	,m_iCurrentUser(0)
	,m_iUserAction(0)
	,m_pLocal(NULL)
	,m_pTouchCardNode(NULL)
	,m_nPlayCount(0)
	,m_pGameRecord(NULL)
	,m_wRecordSelfChairID(0)
	,m_RecordTimeID(0)
	,m_RecordTime(0)
	,GameBase(0,0)
	,m_iOutCardWaiteTime(30)
{
	CCAssert(false,"");
}
SYMJGameScence::SYMJGameScence(int iGameType,int iOption)
	:m_iBankerUser(0)
	,m_iCurrentUser(0)
	,m_iUserAction(0)
	,m_pLocal(NULL)
	,m_pTouchCardNode(NULL)
	,m_nPlayCount(0)
	,m_pGameRecord(NULL)
	,m_wRecordSelfChairID(0)
	,m_RecordTimeID(0)
	,m_RecordTime(0)
	,GameBase(iGameType,iOption)
	,m_iOutCardWaiteTime(30)
{

	init();
}
SYMJGameScence::~SYMJGameScence()
{

}
bool SYMJGameScence::init()
{
	if (!GameBase::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("GameHNMJ/xml/GameScence.xml",this);
	cocos2d::ScriptXMLparse kScriptXml1("GameHNMJ/xml/ScriptValueStr.xml");

	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"GameTypeScenceNode");
	WidgetScenceXMLparse kScence2("GameHNMJ/xml/GameScence_THJ.xml",pNode);
	WidgetScenceXMLparse kScence3("GameHNMJ/xml/GameSetScence.xml",pNode);

	if (WidgetFun::getChildWidget(this,"GameTalkList"))
	{
		cocos2d::ListViewEx* pTalkList = WidgetFun::getListViewWidget(this,"GameTalkList");
		pTalkList->removeAllChildren();
		for (int i=0;i<10;i++)
		{
			cocos2d::Node* pItem = WidgetManager::Instance().createWidget("HNMJGameTalkListItem",pTalkList);
			std::string kTxt = utility::getScriptString(utility::toString("GameTalkTxt",i));
			WidgetFun::setText(pItem,"TalkListItemTxt",kTxt);
			WidgetFun::setWidgetUserInfo(pItem,"Button_TalkDefine","Idex",utility::toString(i));
			WidgetFun::setWidgetUserInfo(pItem,"Button_TalkDefine","Txt",utility::toString(kTxt));
		}
		pTalkList->forceDoLayout();

		for (int m = 0;m<6;m++)
		{
			cocos2d::Node* pNode = WidgetFun::getChildWidget(this,utility::toString("BiaoQing",m));
			for (int n = 0;n<8;n++)
			{
				std::string kFileName = utility::toString("GameHNMJ/Talk/EE",m*8+n,".png");
				WidgetFun::setButtonImagic(pNode,utility::toString("HNMJButton_BiaoQing",n),kFileName,true);
				WidgetFun::setWidgetUserInfo(pNode,utility::toString("HNMJButton_BiaoQing",n),"File",kFileName);
			}
		}
	}

	initPrivate();

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i] = new SYMJPlayer(i,WidgetFun::getChildWidget(this,utility::toString("Player",i)));
	}
	m_pLocal = m_pPlayer[0];

	WidgetManager::addButtonCB("HNMJButton_GameExit",this,button_selector(GameBase::Button_ExitGameBase));

	initButton();
	initNet();
	initTouch();
	initSetButton();
	return true;
}
void SYMJGameScence::EnterScence()
{
	for (int i = 0;i<GAME_PLAYER;i++)
	{
		m_pPlayer[i]->PlayerLeave();
	}
	WidgetFun::setVisible(this,"PrivateInfo",false);
	WidgetFun::setVisible(this,"PrivateEndPlane",false);
	WidgetFun::setVisible(this,"GameHNMJPlane",true);
	WidgetFun::setVisible(this,"GoldRoomInfo",true);
	defaultState();
	SYMJSoundFun::playBackMusic("csmj.mp3"); 
}

bool SYMJGameScence::IsInGame()
{
	return WidgetFun::isWidgetVisble(this,"GameHNMJPlane");
}
void SYMJGameScence::HideAll()
{
	WidgetFun::setVisible(this,"GameHNMJPlane",false);
	defaultPrivateState();
	defaultRecordState();
	defaultMaster();
	removeWaringSound();
}
void SYMJGameScence::defaultState()
{
	m_pTouchCardNode = NULL;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->defaultState();
		m_pPlayer[i]->EndGame();
	}

	if (WidgetFun::getChildWidget(this,"GameSetPlane"))
	{
		WidgetFun::setVisible(this,"GameSetPlane",false);
	}
	WidgetFun::setVisible(this,"TimeNode",false);
	WidgetFun::setVisible(this,"LastCardNode",false);
	WidgetFun::setVisible(this,"SaiZiNode",false);
	WidgetFun::setVisible(this,"SelfActionNode",false);
	WidgetFun::setVisible(this,"GameJieSuanNode",false);
	WidgetFun::setVisible(this,"FreeStateNode",false);
	WidgetFun::setVisible(this,"GameEndNiaoCardAni",false);
	WidgetFun::setVisible(this,"TuoGuanStateNode",false);
	WidgetFun::setVisible(this,"GameHaiDiNode",false);
	WidgetFun::setVisible(this,"GameFanBeiNode",false);

	defaultPrivateState();
	defaultPlayerActionState();
	defaultRecordState();
	defaultMaster(true);
	remmoveAutoAction();
}
bool SYMJGameScence::isHZ()
{
	return m_iGameType == GAME_OPTION_TYPE_HZ;
}
bool SYMJGameScence::isTHJ()
{
	return m_iGameType == GAME_OPTION_TYPE_THJ;
}
void SYMJGameScence::defaultPlayerActionState()
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"TimeNode");
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",i),false);
	}
	WidgetFun::getChildWidget(pRootNode,"ActPlayerLastTime")->stopAllActions();
	WidgetFun::setText(pRootNode,"ActPlayerLastTime","0");
}
std::string SYMJGameScence::getStringHuRight(dword kValue)
{
	dword dwRight[] = {
		CHR_PING_HU						,									//平胡
		CHR_MEN_QING					,									//门清
		CHR_DA_PENG_DUI					,									//大碰对
		CHR_QI_XIAO_DUI					,									//七小对
		CHR_QING_YI_SE					,									//清一色
		CHR_LONG_QI_DUI					,									//龙七对
		CHR_MEN_QING_YI_SE				,									//门清清一色
		CHR_MEN_QING_DA_PENG_DUI		,									//门清大碰对
		CHR_QING_YI_DA_PENG_DUI			,									//清一色大碰对
		CHR_FENG_YI_SE					,									//风一色
		CHR_SHI_SAN_YAO					,									//十三幺
		CHR_SHI_BA_LUO_HAN				,									//十八罗汉
		CHR_MEN_QING_DA_PENG_DUI_QING_YI_SE	,						//清一色大碰对门清

		CHR_DIAN_GANG						,									//点杠
		CHR_ZI_MO,		
		CHR_FANGPAO	,
		CHR_JIEPAO,	

		CHR_GANG_KAI				,									//杠上开花
		CHR_QIANG_GANG_HU			,									//抢杠胡
		CHR_GANG_SHANG_PAO		,									//杠上炮

	};
	std::string pszRight[] = {
		ScriptData<std::string>("HuRight0").Value(),
		ScriptData<std::string>("HuRight1").Value(),
		ScriptData<std::string>("HuRight2").Value(),
		ScriptData<std::string>("HuRight3").Value(),
		ScriptData<std::string>("HuRight4").Value(),
		ScriptData<std::string>("HuRight5").Value(),
		ScriptData<std::string>("HuRight6").Value(),
		ScriptData<std::string>("HuRight7").Value(),
		ScriptData<std::string>("HuRight8").Value(),
		ScriptData<std::string>("HuRight9").Value(),
		ScriptData<std::string>("HuRight10").Value(),
		ScriptData<std::string>("HuRight11").Value(),
		ScriptData<std::string>("HuRight12").Value(),

		ScriptData<std::string>("HU_TYPE_DIANGANG").Value(),
		ScriptData<std::string>("HU_TYPE_ZIMO").Value(),
		ScriptData<std::string>("HU_TYPE_FANGPAO").Value(),
		ScriptData<std::string>("HU_TYPE_JIEPAO").Value(),

		ScriptData<std::string>("HuRight13").Value(),
		ScriptData<std::string>("HuRight14").Value(),
		ScriptData<std::string>("HuRight15").Value(),
	};
	std::string kTxt1;
	std::string kTxt2;
	SYMJ::CChiHuRight	kChiHuRight;	
	kChiHuRight.SetRightData(&kValue,MAX_RIGHT_COUNT );
	for( BYTE j = 0; j < CountArray(pszRight); j++ )
	{
		if( !(kChiHuRight&dwRight[j]).IsEmpty() && j <13)
		{
			kTxt2 = pszRight[j];
		}
		if( !(kChiHuRight&dwRight[j]).IsEmpty() && j >=13)
		{
			kTxt1 = pszRight[j];
		}
	}
	return kTxt1+kTxt2;
}

std::string SYMJGameScence::getStringGang(int nChairID,CMD_WeaveItem* pWeave,int iWeaveCout )
{
	std::string kStr;
	for (int i = 0;i<iWeaveCout;i++)
	{
		CMD_WeaveItem* pTemp = pWeave+i;
		if (pTemp->cbWeaveKind == WIK_GANG || pTemp->cbWeaveKind == WIK_CS_GANG)
		{
			if ( pTemp->wProvideUser == nChairID && pTemp->cbPublicCard == 0)
			{
				kStr +=ScriptData<std::string>("AnGang").Value();
			}
			else if (pTemp->wProvideUser == nChairID && pTemp->cbPublicCard != 0)
			{
				kStr +=ScriptData<std::string>("ZiMoMingGang").Value();
			}
			else
			{
				kStr +=ScriptData<std::string>("MingGang").Value();
			}
		}
	
	}
	return kStr;
}


void SYMJGameScence::showSaiZi(unsigned int iValue)
{
	word wSice1 = word(iValue>>16);
	word wSice2 = word(iValue);
	if (wSice1 > 0)
	{
		BYTE SiceFirst = (wSice1 >> 8);
		BYTE SiceSecond = (wSice1);
		std::string kImagic = WidgetFun::getWidgetUserInfo(this,"SaiZiNode","Imagic");
		WidgetFun::setImagic(this,"SaiZi0",utility::toString(kImagic,(int)SiceFirst,".png"));
		WidgetFun::setImagic(this,"SaiZi1",utility::toString(kImagic,(int)SiceSecond,".png"));
		WidgetFun::setWidgetUserInfo(this,"SaiZiNode","NextValue",utility::toString((int)wSice2));
		WidgetFun::runWidgetAction(this,"SaiZiNode","ActionStart1");
	}
	else if (wSice2 > 0)
	{
		BYTE SiceFirst = (wSice2 >> 8);
		BYTE SiceSecond = (wSice2);
		std::string kImagic = WidgetFun::getWidgetUserInfo(this,"SaiZiNode","Imagic");
		WidgetFun::setImagic(this,"SaiZi0",utility::toString(kImagic,(int)SiceFirst,".png"));
		WidgetFun::setImagic(this,"SaiZi1",utility::toString(kImagic,(int)SiceSecond,".png"));
		WidgetFun::runWidgetAction(this,"SaiZiNode","ActionStart2");
		getPlayerByChairID(m_iBankerUser)->setZhuang();
	}
}
void SYMJGameScence::remmoveAutoAction()
{
	TimeManager::Instance().removeByFun(TIME_CALLBACK(SYMJGameScence::HuPaiAutoAction,this));
	TimeManager::Instance().removeByFun(TIME_CALLBACK(SYMJGameScence::AutoCard,this));
}
void SYMJGameScence::AutoCard()
{
	if (!HaveOptionRule(GAME_OPTION_RULE_AUTO_CARD))
	{
		return;
	}
	if (m_iCurrentUser != m_pLocal->GetChairID())
	{
		return;
	}
	if (WidgetFun::isWidgetVisble(this,"SelfActionNode"))
	{
		HNMJButton_GuoAction(NULL,NULL);
		return;
	}
	CMD_C_OutCard OutCard;
	OutCard.cbCardData = m_pLocal->getNewInCard();
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));
}
void SYMJGameScence::HuPaiAutoAction()
{
	HNMJButton_HuAction(NULL,NULL);
}
void SYMJGameScence::OnPlayWaring()
{
	SYMJSoundFun::playEffectEX("warning.mp3");
}
void SYMJGameScence::removeWaringSound()
{
	TimeManager::Instance().removeByFun(TIME_CALLBACK(SYMJGameScence::OnPlayWaring,this));
}
void SYMJGameScence::setCurrentPlayer(int iCurrentPlayer,DWORD iUserAction,int nActionCard/*=0*/)
{
	remmoveAutoAction();
	if (iCurrentPlayer < 0 || iCurrentPlayer > MAX_PLAYER)
	{
		CCASSERT(false,"");
		return;
	}
	defaultPlayerActionState();

	m_iCurrentUser = iCurrentPlayer;

	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"TimeNode");

	SYMJPlayer* pPlyer = getPlayerByChairID(m_iCurrentUser);
	if (!pPlyer)
	{
		return;
	}
	int iWaiteTime = TIME_OPERATE_CARD;
	if (iUserAction == WIK_NULL)
	{
		iWaiteTime = m_iOutCardWaiteTime;
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",pPlyer->GetChairID()),true);
	}
	WidgetFun::getChildWidget(pRootNode,"ActPlayerLastTime")->runAction(
		cocos2d::MoveExTxtTime::create(iWaiteTime));
	removeWaringSound();
	TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(SYMJGameScence::OnPlayWaring,this),iWaiteTime-3);
	if(pPlyer == m_pLocal && iUserAction != WIK_NULL)
	{
		WidgetFun::setWidgetUserInfo(this,"NotifyCard",utility::toString(nActionCard));
		checkActionBtns(iUserAction,nActionCard);
	}
	bool bHu = ((iUserAction)&WIK_CHI_HU || (iUserAction)&WIK_ZI_MO);
	if (pPlyer == m_pLocal && HaveOptionRule(GAME_OPTION_RULE_AUTO_HU) && bHu)
	{
		WidgetFun::setVisible(this,"SelfActionNode",false);
		TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(SYMJGameScence::HuPaiAutoAction,this),2);
	}
	if(pPlyer == m_pLocal && HaveOptionRule(GAME_OPTION_RULE_AUTO_CARD) && !bHu)
	{
		TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(SYMJGameScence::AutoCard,this),iWaiteTime);
	}
}
void SYMJGameScence::setActionBtnCard(cocos2d::Node* pActionBtnCard,int nCard,std::string kImagicFront)
{
	if (pActionBtnCard==NULL|| nCard == 0 )
	{
		return;
	}
	BYTE cbValue= SYMJ::CGameLogic::Instance().GetCardValue(nCard);
	BYTE cbColor=SYMJ::CGameLogic::Instance().GetCardColor(nCard);
	WidgetFun::setButtonImagic(pActionBtnCard,utility::toString(kImagicFront,(int)cbColor,(int)cbValue,".png"),true);
}

void SYMJGameScence::checkActionBtns(DWORD nUserAction,int nActionCard)
{
	DWORD iUserAction = nUserAction;
	if (iUserAction == WIK_NULL)
	{
		return;
	}
	cocos2d::Node* pSelfActionNode = WidgetFun::getChildWidget(this,"SelfActionNode");
	cocos2d::Vec2 kStartPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pSelfActionNode,"StartPos"));
	checkActionBtn(true,"HNMJButton_GuoAction",0,kStartPos);
	checkActionBtn(iUserAction&WIK_LEFT || iUserAction&WIK_CENTER 
		|| iUserAction&WIK_RIGHT,"HNMJButton_ChiAction",nActionCard,kStartPos);
	checkActionBtn(iUserAction&WIK_PENG,"HNMJButton_PengAction",nActionCard,kStartPos);
	checkActionBtn(iUserAction&WIK_GANG,"HNMJButton_GangAction",nActionCard,kStartPos);
	checkActionBtn(iUserAction&WIK_CS_GANG,"HNMJButton_BuAction",nActionCard,kStartPos);
	checkActionBtn(iUserAction&WIK_CHI_HU || iUserAction&WIK_ZI_MO,"HNMJButton_HuAction",nActionCard,kStartPos);

	WidgetFun::setVisible(this,"HNMJButton_GangCS",false);
	WidgetFun::setVisible(this,"HNMJButton_XiaoHuAction",false);
//	WidgetFun::setVisible(this,"HNMJButton_BuAction",false);

	WidgetFun::setVisible(this,"SelfActionNode",true);
	if (WidgetFun::isWidgetVisble(this,"HNMJButton_ChiAction"))
	{
		WidgetFun::setVisible(this,"ChiCardBg",false);
		WidgetFun::setWidgetUserInfo(this,"HNMJButton_ChiAction","Chi_Type",utility::toString(nUserAction));
		WidgetFun::setWidgetUserInfo(this,"HNMJButton_ChiAction","Chi_Card",utility::toString(nActionCard));

		WidgetFun::setWidgetUserInfo(this,"HNMJButton_ChiAction_Card","Chi_Type",utility::toString(nUserAction));
		WidgetFun::setWidgetUserInfo(this,"HNMJButton_ChiAction_Card","Chi_Card",utility::toString(nActionCard));
	}
	if (WidgetFun::isWidgetVisble(this,"HNMJButton_XiaoHuAction"))
	{
		WidgetFun::setWidgetUserInfo(this,"HNMJButton_GuoAction","IsXiaoHu","XiaoHu");
	}
	else
	{
		WidgetFun::setWidgetUserInfo(this,"HNMJButton_GuoAction","IsXiaoHu","");
	}

	if (HaveOptionRule(GAME_OPTION_RULE_HIDE_GUO)&& (iUserAction&WIK_CHI_HU || iUserAction&WIK_ZI_MO)) 
	{
		WidgetFun::setVisible(this,"HNMJButton_GuoAction",false);
	}
}

void SYMJGameScence::checkActionBtn(bool bcheck,const std::string& BtnName,int nCard, cocos2d::Vec2& kPos)
{
	cocos2d::Node* pSelfActionNode = WidgetFun::getChildWidget(this,"SelfActionNode");
	cocos2d::Vec2 kAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pSelfActionNode,"AddPos"));
	std::string kImagicFront = WidgetFun::getWidgetUserInfo(pSelfActionNode,"ImagicFront");

	WidgetFun::setVisible(this,BtnName,false);
	if (bcheck)
	{
		kPos +=kAddPos;
		WidgetFun::setVisible(this,BtnName,true);
		WidgetFun::setPos(this,BtnName,kPos);
		cocos2d::Node* pActionBtnCard = WidgetFun::getChildWidget(this,utility::toString(BtnName,"_Card"));
		if(nCard != 0 && pActionBtnCard)
		{
			pActionBtnCard->setVisible(true);
			setActionBtnCard(pActionBtnCard,nCard,kImagicFront);
		}
		else if(pActionBtnCard)
		{
			pActionBtnCard->setVisible(false);
		}
	}
}

void SYMJGameScence::showJieSuanInfo(CMD_S_GameEnd* pGameEnd)
{
	float delayTime = 2.0f;
	cocos2d::Node* pNiaoCardAni = WidgetFun::getChildWidget(this,"GameEndNiaoCardAni");
	pNiaoCardAni->setVisible(false);
	if (pGameEnd->cbNiaoCount>0)
	{
		pNiaoCardAni->runAction(cocos2d::CCVisibleAction::create(delayTime,true));
		delayTime += pGameEnd->cbNiaoCount*0.7;
		setGameEndNiaoCardAni(pNiaoCardAni,pGameEnd->cbCardDataNiao,pGameEnd->cbNiaoCount);
	}
	delayTime += 2.0f;

	cocos2d::Node* pJieSuanNode = WidgetFun::getChildWidget(this,"GameJieSuanNode");
	WidgetFun::setWidgetUserInfo(this,"GameJieSuanNode","Time",utility::toString((int)delayTime));
	cocos2d::CCVisibleAction* pAction = cocos2d::CCVisibleAction::create(delayTime,true);
	pAction->setTag(1);
	pJieSuanNode->runAction(pAction);
	pNiaoCardAni->runAction(cocos2d::CCVisibleAction::create(delayTime,false));

	TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(SYMJGameScence::upSelfFreeReadState,this),delayTime+1);

	 WidgetFun::setPlaceText(pJieSuanNode,"JieSuan_JuShu",utility::toString(getPlayCount()));
	 setJieSuanNiaoCard(pJieSuanNode,pGameEnd->cbCardDataNiao,pGameEnd->cbNiaoCount);
	 
	 bool isZimo = false;

	 for (int i=0;i<MAX_PLAYER;i++)
	 {
		int iChirID = m_pPlayer[i]->GetChairID();
		int nHuScore = pGameEnd->lGameScore[iChirID] - pGameEnd->lStartHuScore[iChirID]-pGameEnd->lGangScore[iChirID];

		cocos2d::Node* pJieSuanInfo = WidgetFun::getChildWidget(pJieSuanNode,utility::toString("JieSuanInfo",i));
		WidgetFun::setText(pJieSuanInfo,"Jiesuan_Nicke",m_pPlayer[i]->GetNickName());
		if (pGameEnd->lGameScore[iChirID]>=0)
		{
			WidgetFun::setText(pJieSuanInfo,"Jiesuan_GoldNum",utility::toString("+",pGameEnd->lGameScore[iChirID]));
		}
		else
		{
			WidgetFun::setText(pJieSuanInfo,"Jiesuan_GoldNum",pGameEnd->lGameScore[iChirID]);
		}

		WidgetFun::setVisible(pJieSuanInfo,"JieSuan_Zhuang",false);
		WidgetFun::setVisible(pJieSuanInfo,"JieSuan_Hu",false);
		WidgetFun::setVisible(pJieSuanInfo,"Jiesuan_HuType",false);
		if (nHuScore>0 )
		{
			WidgetFun::setVisible(pJieSuanInfo,"JieSuan_Hu",true);
			if (pGameEnd->wProvideUser[iChirID] == iChirID )
			{
				isZimo = true;
			}
		}
		if (iChirID == m_iBankerUser)
		{
			WidgetFun::setVisible(pJieSuanInfo,"JieSuan_Zhuang",true);
		}
		cocos2d::Node* pCardNode = WidgetFun::getChildWidget(pJieSuanInfo,"JieSuanCardNode");
		m_pPlayer[i]->showJieSuanCard(pCardNode,pGameEnd->WeaveItemArray[iChirID],pGameEnd->cbWeaveCount[iChirID],
			pGameEnd->cbCardData[iChirID],pGameEnd->cbCardCount[iChirID]);
	 }

	 for (int i=0;i<MAX_PLAYER;i++)
	 {
		 int iChirID = m_pPlayer[i]->GetChairID();
		 int nHuScore = pGameEnd->lGameScore[iChirID] - pGameEnd->lStartHuScore[iChirID]-pGameEnd->lGangScore[iChirID];

		 cocos2d::Node* pJieSuanInfo = WidgetFun::getChildWidget(pJieSuanNode,utility::toString("JieSuanInfo",i));
		 WidgetFun::setVisible(pJieSuanInfo,"Jiesuan_HuType",true);

		 WidgetFun::setNodeKeepAfter(pJieSuanInfo,WidgetFun::getChildWidget(pJieSuanInfo,"Jiesuan_Nicke"),"Jiesuan_HuType");
		 std::string kHuType;
		 if(nHuScore>0 && isZimo)
		 {
			if (pGameEnd->cbNiaoCount>0)
			{
				kHuType += utility::toString(ScriptData<std::string>("ZHONG_NIAO").Value(),(int)pGameEnd->cbNiaoPick,ScriptData<std::string>("ZHONG_GE").Value());
			}
		 }
		 else if(nHuScore>0 && !isZimo)
		 {
			 if (pGameEnd->cbNiaoCount>0)
			 {
				 kHuType += utility::toString(ScriptData<std::string>("ZHONG_NIAO").Value(),(int)pGameEnd->cbNiaoPick,ScriptData<std::string>("ZHONG_GE").Value());
			 }
		 }
		 else if (nHuScore<0 && !isZimo)
		 {
		 }

		 if (pGameEnd->lGangScore[iChirID]>0)
		 {
			 kHuType += getStringGang(iChirID,pGameEnd->WeaveItemArray[iChirID],pGameEnd->cbWeaveCount[iChirID]);
		 }
		 if (pGameEnd->lStartHuScore[iChirID] >0)
		 {
			 kHuType += getStringHuRight(pGameEnd->dwStartHuRight[iChirID]);
		 }

			kHuType += getStringHuRight(pGameEnd->dwChiHuRight[iChirID]);
		
		 if (pGameEnd->cbFanBei[iChirID]>1)
		 {
			 kHuType += ScriptData<std::string>("JiaCui").Value();
		 }

		 WidgetFun::setText(pJieSuanInfo,"Jiesuan_HuType",kHuType);
	 }

	 int nChairID = m_pLocal->GetChairID();
	 cocos2d::Node* pJieSuanTitle = WidgetFun::getChildWidget(pJieSuanNode,"JieSuan_Title");
	 cocos2d::Node* pJieSuanNPC = WidgetFun::getChildWidget(pJieSuanNode,"JieSuan_NPC");
	 if (pGameEnd->lGameScore[nChairID]>=0)
	 {
		 std::string kTitleWin = WidgetFun::getWidgetUserInfo(pJieSuanTitle,"Title_Win");
		 std::string kNPCXiao = WidgetFun::getWidgetUserInfo(pJieSuanNPC,"NPC_Smile");

		 WidgetFun::setImagic(pJieSuanTitle,kTitleWin,false);
		 WidgetFun::setImagic(pJieSuanNPC,kNPCXiao,false);
	 }
	 else 
	 {
		 std::string kTitleLose = WidgetFun::getWidgetUserInfo(pJieSuanTitle,"Title_Lose");
		 std::string kNPCKu = WidgetFun::getWidgetUserInfo(pJieSuanNPC,"NPC_Cry");

		 WidgetFun::setImagic(pJieSuanTitle,kTitleLose,false);
		 WidgetFun::setImagic(pJieSuanNPC,kNPCKu,false);
	 }
	 if (pGameEnd->cbLiuJu)
	 {
		 std::string kTitlePing =  WidgetFun::getWidgetUserInfo(pJieSuanTitle,"Title_LiuJu");
		 WidgetFun::setImagic(pJieSuanTitle,kTitlePing,false);
	 }

}
void SYMJGameScence::setJieSuanNiaoCard( cocos2d::Node* pNode,BYTE* pNiaoCard,BYTE cbCardNum )
{
	ASSERT(pNode);

	cocos2d::Node* pNiaoNode = WidgetFun::getChildWidget(pNode,"JieSuan_CardNiao");
 	pNiaoNode->removeAllChildren();

	int iIdex = 0;
	int iOder = 0;
	cocos2d::Vec2 kStartPos = cocos2d::Vec2(0,0);
	std::string kHandSkin = utility::toString("HNMJ_HAND_",iIdex);
	cocos2d::Vec2 kHandAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNiaoNode,"JieSuanAddPos"));
	for (int i=0;i<cbCardNum;i++)
	{
		ASSERT(SYMJ::CGameLogic::Instance().IsValidCard(pNiaoCard[i]));

		cocos2d::Node* pCard = WidgetManager::Instance().createWidget(kHandSkin,pNiaoNode);
		pCard->setTag(1);
		pCard->setLocalZOrder(iOder);
		pCard->setPosition(kStartPos);
		{
			kStartPos += kHandAddPos;
		}
		if (iIdex == 0)
		{
			std::string kImagic = WidgetFun::getWidgetUserInfo(pCard,"MingImagic");
			m_pLocal->setCardImagic(pCard,pNiaoCard[i],kImagic);
		}
		iOder++;
	}

}


void SYMJGameScence::setGameEndNiaoCardAni(cocos2d::Node* pNode,BYTE* pNiaoCard,BYTE cbCardNum)
{
	ASSERT(pNode);

	cocos2d::Node* pNiaoNode = WidgetFun::getChildWidget(pNode,"NiaoCardAniNode");
	pNiaoNode->removeAllChildren();

	cocos2d::Vec2 kHandAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNiaoNode,"JieSuanAddPos"));
	int iOder = 0;

	float fWidth = (700 - kHandAddPos.x * (int)cbCardNum)/(int)(cbCardNum+1)+kHandAddPos.x;

	cocos2d::Vec2 kStartPos = cocos2d::Vec2(-350-kHandAddPos.x/2,0);
	std::string kHandSkin = "HNMJ_NIAO_CARD_NODE";
	float fCardDelay = 3.f;
	for (int i=0;i<cbCardNum;i++)
	{
		kStartPos.x += fWidth;
		ASSERT(SYMJ::CGameLogic::Instance().IsValidCard(pNiaoCard[i]));

		cocos2d::Node* pCardNode = WidgetManager::Instance().createWidget(kHandSkin,pNiaoNode);
		pCardNode->setTag(1);
		pCardNode->setLocalZOrder(iOder);
		pCardNode->setPosition(kStartPos);
		
		cocos2d::Node* pCard = WidgetFun::getChildWidget(pCardNode,"HNMJ_NIAO_CARD");
		std::string kImagic  = WidgetFun::getWidgetUserInfo(pCard,"MingImagic");
		m_pLocal->setCardImagic(pCard,pNiaoCard[i],kImagic);
		
		WidgetFun::setWidgetUserInfo(pCard,"ShowDelayTime",utility::toString(fCardDelay));

		float fBombDelay = utility::parseFloat(WidgetFun::getWidgetUserInfo(pCardNode,"BombDelay"));
		cocos2d::Node* pBomb = WidgetFun::getChildWidget(pCardNode,"Bomb");
		WidgetFun::setWidgetUserInfo(pBomb,"BombDelayTime",utility::toString(fCardDelay+fBombDelay));
		pCard->setVisible(false);
		pBomb->setVisible(false);
		WidgetFun::runWidgetAction(pCard,"ShowAni");
		WidgetFun::runWidgetAction(pBomb,"ShowAni");

		fCardDelay += fBombDelay+0.7;
		iOder++;
	}
}

bool SYMJGameScence::BackKey()
{
	if(!IsInGame())
	{
		return false;
	}
	if (WidgetFun::getChildWidget(this,"GameSetPlane"))
	{
		WidgetFun::setVisible(this,"GameSetPlane",true);
	}
	else
	{
		GameBase::BackKey();
	}
	return true;
}

void SYMJGameScence::setGameState(int nState)
{
	m_nGameState = nState;
}
int SYMJGameScence::getGameState()
{
	return m_nGameState;
}

void SYMJGameScence::OnAutoOutCard(int cbCardData)
{
	ASSERT(m_nGameState == HNMJ_STATE_PLAYING);
	if ( m_nGameState != HNMJ_STATE_PLAYING ) return;

	CMD_C_OutCard OutCard;
	OutCard.cbCardData = cbCardData;
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));
}

