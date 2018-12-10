#include "HNMJGameScence.h"
#include "HNMJPlayer.h"
#include "GameLib.h"
#include "CMD_HNMJ.h"
#include "HNMJPlayer.h"
#include "HNMJGameLogic.h"
#include "HNMJSoundFun.h"
#include "ScriptXMLparse.h"

FV_SINGLETON_STORAGE(HNMJGameScence);

HNMJGameScence::HNMJGameScence()
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
HNMJGameScence::HNMJGameScence(int iGameType,unsigned int iOption)
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
HNMJGameScence::~HNMJGameScence()
{

}
bool HNMJGameScence::init()
{
	if (!GameBase::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("GameHNMJ/xml/GameScence.xml",this);
	cocos2d::ScriptXMLparse kScriptXml1("GameHNMJ/xml/ScriptValueStr.xml");

	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"GameTypeScenceNode");
	if (isHZ())
	{
		WidgetScenceXMLparse kScence2("GameHNMJ/xml/GameScence_HZ.xml",pNode);
		WidgetScenceXMLparse kScence3("GameHNMJ/xml/GameSetScence.xml",pNode);
	}
	else if(isTHJ())
	{
		WidgetScenceXMLparse kScence2("GameHNMJ/xml/GameScence_THJ.xml",pNode);
		WidgetScenceXMLparse kScence3("GameHNMJ/xml/GameSetScence.xml",pNode);
	}
	else
	{
		WidgetScenceXMLparse kScence2("GameHNMJ/xml/GameScence_HN.xml",pNode);
	}

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
		m_pPlayer[i] = new HNMJPlayer(i,WidgetFun::getChildWidget(this,utility::toString("Player",i)));
	}
	m_pLocal = m_pPlayer[0];

	WidgetManager::addButtonCB("HNMJButton_GameExit",this,button_selector(GameBase::Button_ExitGameBase));
	WidgetManager::addButtonCB("HNMJButton_ZengSongOk",this,button_selector(GameBase::Button_CB_SaveAndExit));
	
	initButton();
	initNet();
	initTouch();
	initSetButton();
	return true;
}
void HNMJGameScence::EnterScence()
{
	for (int i = 0;i<GAME_PLAYER;i++)
	{
		m_pPlayer[i]->PlayerLeave();
	}
	WidgetFun::setVisible(this,"PrivateInfo",false);
	WidgetFun::setVisible(this,"PrivateEndPlane",false);
	WidgetFun::setVisible(this,"GameHNMJPlane",true);
	WidgetFun::setVisible(this,"GoldRoomInfo",true);
	if (WidgetFun::getChildWidget(this,"HNMJButton_CB_SaveAndExit"))
	{
		WidgetFun::setVisible(this,"HNMJButton_CB_SaveAndExit",false);
	}
	if (WidgetFun::getChildWidget(this,"PlaneGameEndLastCard"))
	{
		WidgetFun::setVisible(this,"PlaneGameEndLastCard",false);
	}
	defaultState();
	HNMJSoundFun::playBackMusic("csmj.mp3"); 
}

bool HNMJGameScence::IsInGame()
{
	return WidgetFun::isWidgetVisble(this,"GameHNMJPlane");
}
void HNMJGameScence::HideAll()
{
	WidgetFun::setVisible(this,"GameHNMJPlane",false);
	defaultPrivateState();
	defaultRecordState();
	defaultMaster();
	removeWaringSound();

	cocos2d::Node* pEndPlane = WidgetFun::getChildWidget(this,"PrivateEndPlane");
	if (pEndPlane && pEndPlane->getActionByTag(1))
	{
		pEndPlane->setVisible(true);
	}
}
void HNMJGameScence::defaultState()
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
	WidgetFun::setVisible(this,"CSGangActionNode",false);
	WidgetFun::setVisible(this,"GameJieSuanNode",false);
	WidgetFun::setVisible(this,"FreeStateNode",false);
	WidgetFun::setVisible(this,"GameEndNiaoCardAni",false);
	WidgetFun::setVisible(this,"TuoGuanStateNode",false);
	WidgetFun::setVisible(this,"GameHaiDiNode",false);
	WidgetFun::setVisible(this,"GamePiaoNode",false);
	
	defaultPrivateState();
	defaultPlayerActionState();
	defaultRecordState();
	defaultMaster(true);
	remmoveAutoAction();
}
bool HNMJGameScence::isHZ()
{
	return m_iGameType == GAME_OPTION_TYPE_HZ;
}
bool HNMJGameScence::isTHJ()
{
	return m_iGameType == GAME_OPTION_TYPE_THJ;
}
void HNMJGameScence::defaultPlayerActionState()
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"TimeNode");
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",i),false);
	}
	WidgetFun::getChildWidget(pRootNode,"ActPlayerLastTime")->stopAllActions();
	WidgetFun::setText(pRootNode,"ActPlayerLastTime","0");
}
std::string HNMJGameScence::getStringHuRight(dword kValue)
{
	dword dwRight[] = {
		CHR_PENGPENG_HU		,							//碰碰胡
		CHR_JIANGJIANG_HU		,						//将将胡
		CHR_QING_YI_SE		,							//清一色
		CHR_HAI_DI_LAO		,							//海底捞
		CHR_HAI_DI_PAO		,							//海底捞
		CHR_QI_XIAO_DUI		,							//七小对
		CHR_HAOHUA_QI_XIAO_DUI	,						//豪华七小对
		CHR_GANG_KAI			,						//杠上开花
		CHR_QIANG_GANG_HU		,						//抢杠胡
		CHR_GANG_SHANG_PAO	,							//杠上跑
		CHR_QUAN_QIU_REN		,						//全求人
		CHR_XIAO_DA_SI_XI	,							//大四喜
		CHR_XIAO_BAN_BAN_HU		,						//板板胡
		CHR_XIAO_QUE_YI_SE		,						//缺一色
		CHR_XIAO_LIU_LIU_SHUN	,						//六六顺
		CHR_SHUANG_HAOHUA_QI_XIAO_DUI	,				//双豪华七小对
		CHR_HONGZHONG_WU,								//无红中多加一鸟
		CHR_TIAN_HU		,						    	//4红中天胡
		CHR_GANG_SHUANG_KAI								//长沙双杠开
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
		ScriptData<std::string>("HuRight13").Value(),
		ScriptData<std::string>("HuRight14").Value(),
		ScriptData<std::string>("HuRight15").Value(),
		ScriptData<std::string>("HuRight16").Value(),
		ScriptData<std::string>("HuRight17").Value(),
		ScriptData<std::string>("HuRight18").Value(),
	};
	std::string kTxt;
	HNMJ::CChiHuRight	kChiHuRight;	
	kChiHuRight.SetRightData(&kValue,MAX_RIGHT_COUNT );
	for( BYTE j = 0; j < CountArray(pszRight); j++ )
	{
		if( !(kChiHuRight&dwRight[j]).IsEmpty() )
		{
			kTxt+=pszRight[j];
		}
	}
	return kTxt;
}

std::string HNMJGameScence::getStringGang(int nChairID,CMD_WeaveItem* pWeave,int iWeaveCout )
{
	std::string kStr;
	for (int i = 0;i<iWeaveCout;i++)
	{
		CMD_WeaveItem* pTemp = pWeave+i;
		if (pTemp->cbWeaveKind == WIK_GANG ||pTemp->cbWeaveKind == WIK_CS_GANG ||pTemp->cbWeaveKind == WIK_BU_ZHANG)
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


void HNMJGameScence::showSaiZi(unsigned int iValue)
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
void HNMJGameScence::remmoveAutoAction()
{
	TimeManager::Instance().removeByFun(TIME_CALLBACK(HNMJGameScence::HuPaiAutoAction,this));
	TimeManager::Instance().removeByFun(TIME_CALLBACK(HNMJGameScence::AutoCard,this));
}
void HNMJGameScence::AutoCard()
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
void HNMJGameScence::HuPaiAutoAction()
{
	HNMJButton_HuAction(NULL,NULL);
}
void HNMJGameScence::OnPlayWaring()
{
	HNMJSoundFun::playEffectEX("warning.mp3");
}
void HNMJGameScence::removeWaringSound()
{
	TimeManager::Instance().removeByFun(TIME_CALLBACK(HNMJGameScence::OnPlayWaring,this));
}
void HNMJGameScence::setCurrentPlayer(int iCurrentPlayer,DWORD iUserAction,int nActionCard/*=0*/)
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

	HNMJPlayer* pPlyer = getPlayerByChairID(m_iCurrentUser);
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
	TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(HNMJGameScence::OnPlayWaring,this),iWaiteTime-3);
	if(pPlyer == m_pLocal && iUserAction != WIK_NULL)
	{
		checkActionBtns(iUserAction,nActionCard);
	}
	bool bHu = ((iUserAction)&WIK_CHI_HU || (iUserAction)&WIK_ZI_MO);
	if (pPlyer == m_pLocal && HaveOptionRule(GAME_OPTION_RULE_AUTO_HU) && bHu)
	{
		WidgetFun::setVisible(this,"SelfActionNode",false);
		TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(HNMJGameScence::HuPaiAutoAction,this),2);
	}
	if(pPlyer == m_pLocal && HaveOptionRule(GAME_OPTION_RULE_AUTO_CARD) && !bHu)
	{
		TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(HNMJGameScence::AutoCard,this),iWaiteTime);
	}
}
void HNMJGameScence::setCSgangAction(cocos2d::Node* pNode,DWORD cbActionCard,DWORD cbActionMask)
{
	WidgetFun::setVisible(pNode,"ChiCardBg",false);
	WidgetFun::setEnable(pNode,"HNMJButton_ChiAction",cbActionMask&WIK_LEFT || cbActionMask&WIK_CENTER || cbActionMask&WIK_RIGHT);
	WidgetFun::setEnable(pNode,"HNMJButton_PengAction",cbActionMask&WIK_PENG);
	WidgetFun::setEnable(pNode,"HNMJButton_HuAction",cbActionMask&WIK_CHI_HU || cbActionMask&WIK_ZI_MO);
	WidgetFun::setEnable(pNode,"HNMJButtonCS_BuAction",cbActionMask&WIK_BU_ZHANG);
	if ( HaveOptionRule(GAME_OPTION_TING_CS_GANG))
	{
		WidgetFun::setEnable(pNode,"HNMJButtonCS_GangCS",cbActionMask&WIK_CS_GANG && m_pLocal->isGangTing(cbActionCard));
	}
	else
	{
		WidgetFun::setEnable(pNode,"HNMJButtonCS_GangCS",cbActionMask&WIK_CS_GANG);
	}
	if (cbActionMask&WIK_LEFT || cbActionMask&WIK_CENTER || cbActionMask&WIK_RIGHT)
	{
		WidgetFun::setWidgetUserInfo(pNode,"HNMJButton_ChiAction","Chi_Type",utility::toString(cbActionMask));
		WidgetFun::setWidgetUserInfo(pNode,"HNMJButton_ChiAction","Chi_Card",utility::toString(cbActionCard));

	}
}
void HNMJGameScence::showCSGangAction(int wProviderUser,BYTE cbActionCard1,DWORD cbActionMask1,BYTE cbActionCard2,DWORD cbActionMask2)
{
	std::string kName;
	HNMJPlayer* pPlyer = getPlayerByChairID(wProviderUser);
	if (pPlyer)
	{
		kName = pPlyer->GetNickName();
	}
	WidgetFun::setVisible(this,"SelfActionNode",false);
	WidgetFun::setVisible(this,"CSGangActionNode",true);
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"CSGangActionNode");
	cocos2d::Node* pCsGangActionNode1 = WidgetFun::getChildWidget(pRootNode,"CSGangActionNode0");
	cocos2d::Node* pCsGangActionNode2 = WidgetFun::getChildWidget(pRootNode,"CSGangActionNode1");
	WidgetFun::setPlaceTextKey(pRootNode,"CSGangActionTxt","Txt",kName);
	std::string kImagicFront = WidgetFun::getWidgetUserInfo(this,"CSGangActionNode","ImagicFront");
	m_pLocal->setCardImagic(WidgetFun::getChildWidget(pRootNode,"CSGangImagicHead0"),cbActionCard1,kImagicFront);
	m_pLocal->setCardImagic(WidgetFun::getChildWidget(pRootNode,"CSGangImagicHead1"),cbActionCard2,kImagicFront);
	m_pLocal->setCardImagic(WidgetFun::getChildWidget(pRootNode,"CSGangImagicLine0"),cbActionCard1,kImagicFront);
	m_pLocal->setCardImagic(WidgetFun::getChildWidget(pRootNode,"CSGangImagicLine1"),cbActionCard2,kImagicFront);
	setCSgangAction(pCsGangActionNode1,cbActionCard1,cbActionMask1);
	setCSgangAction(pCsGangActionNode2,cbActionCard2,cbActionMask2);
}
void HNMJGameScence::setActionBtnCard(cocos2d::Node* pActionBtnCard,int nCard,std::string kImagicFront)
{
	if (pActionBtnCard==NULL|| nCard == 0 )
	{
		return;
	}
	BYTE cbValue= HNMJ::CGameLogic::Instance().GetCardValue(nCard);
	BYTE cbColor=HNMJ::CGameLogic::Instance().GetCardColor(nCard);
	WidgetFun::setButtonImagic(pActionBtnCard,utility::toString(kImagicFront,(int)cbColor,(int)cbValue,".png"),true);
}

void HNMJGameScence::checkActionBtns(DWORD nUserAction,int nActionCard)
{
	DWORD iUserAction = nUserAction;
	if (iUserAction == WIK_NULL)
	{
		return;
	}
	cocos2d::Node* pSelfActionNode = WidgetFun::getChildWidget(this,"SelfActionNode");
	WidgetFun::setVisible(pSelfActionNode,"HNMJButton_GangAction:GangActionNode",false);
	WidgetFun::setVisible(pSelfActionNode,"HNMJButton_GangCS:GangActionNode",false);
	WidgetFun::setVisible(pSelfActionNode,"HNMJButton_BuAction:GangActionNode",false);

	cocos2d::Vec2 kStartPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pSelfActionNode,"StartPos"));
	checkActionBtn(true,"HNMJButton_GuoAction",0,kStartPos);
	checkActionBtn(iUserAction&WIK_LEFT || iUserAction&WIK_CENTER 
		|| iUserAction&WIK_RIGHT,"HNMJButton_ChiAction",nActionCard,kStartPos);
	checkActionBtn(iUserAction&WIK_PENG,"HNMJButton_PengAction",nActionCard,kStartPos);
	checkActionBtn(iUserAction&WIK_GANG,"HNMJButton_GangAction",nActionCard,kStartPos);
	if ( HaveOptionRule(GAME_OPTION_TING_CS_GANG))
	{
		int iGangCard = (int)m_kcbGangCardData[0];
		checkActionBtn(iUserAction&WIK_CS_GANG && m_pLocal->isGangTing(iGangCard),"HNMJButton_GangCS",iGangCard,kStartPos); 
	}
	else
	{
		checkActionBtn(iUserAction&WIK_CS_GANG,"HNMJButton_GangCS",nActionCard,kStartPos);
	}
	checkActionBtn(iUserAction&WIK_CHI_HU || iUserAction&WIK_ZI_MO,"HNMJButton_HuAction",nActionCard,kStartPos);
	checkActionBtn(iUserAction&WIK_XIAO_HU,"HNMJButton_XiaoHuAction",0,kStartPos);
	checkActionBtn(iUserAction&WIK_BU_ZHANG,"HNMJButton_BuAction",0,kStartPos);

	WidgetFun::setVisible(this,"SelfActionNode",true);
	if (WidgetFun::isWidgetVisble(pSelfActionNode,"HNMJButton_ChiAction"))
	{
		WidgetFun::setVisible(pSelfActionNode,"ChiCardBg",false);
		WidgetFun::setWidgetUserInfo(pSelfActionNode,"HNMJButton_ChiAction","Chi_Type",utility::toString(nUserAction));
		WidgetFun::setWidgetUserInfo(pSelfActionNode,"HNMJButton_ChiAction","Chi_Card",utility::toString(nActionCard));

		WidgetFun::setWidgetUserInfo(pSelfActionNode,"HNMJButton_ChiAction_Card","Chi_Type",utility::toString(nUserAction));
		WidgetFun::setWidgetUserInfo(pSelfActionNode,"HNMJButton_ChiAction_Card","Chi_Card",utility::toString(nActionCard));
	}
	if (WidgetFun::isWidgetVisble(pSelfActionNode,"HNMJButton_XiaoHuAction"))
	{
		WidgetFun::setWidgetUserInfo(pSelfActionNode,"HNMJButton_GuoAction","IsXiaoHu","XiaoHu");
	}
	else
	{
		WidgetFun::setWidgetUserInfo(pSelfActionNode,"HNMJButton_GuoAction","IsXiaoHu","");
	}

	if (HaveOptionRule(GAME_OPTION_RULE_HIDE_GUO)&& (iUserAction&WIK_CHI_HU || iUserAction&WIK_ZI_MO)) 
	{
		WidgetFun::setVisible(pSelfActionNode,"HNMJButton_GuoAction",false);
	}
}

void HNMJGameScence::checkActionBtn(bool bcheck,const std::string& BtnName,int nCard, cocos2d::Vec2& kPos)
{
	cocos2d::Node* pSelfActionNode = WidgetFun::getChildWidget(this,"SelfActionNode");
	cocos2d::Vec2 kAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pSelfActionNode,"AddPos"));
	std::string kImagicFront = WidgetFun::getWidgetUserInfo(pSelfActionNode,"ImagicFront");

	WidgetFun::setVisible(pSelfActionNode,BtnName,false);
	if (bcheck)
	{
		WidgetFun::setVisible(pSelfActionNode,BtnName,true);
		WidgetFun::setPos(pSelfActionNode,BtnName,kPos);
		cocos2d::Node* pActionBtnCard = WidgetFun::getChildWidget(pSelfActionNode,utility::toString(BtnName,"_Card"));
		if(nCard != 0 && pActionBtnCard)
		{
			pActionBtnCard->setVisible(true);
			setActionBtnCard(pActionBtnCard,nCard,kImagicFront);
		}
		else if(pActionBtnCard)
		{
			pActionBtnCard->setVisible(false);
		}
		kPos +=kAddPos;
	}
}
void HNMJGameScence::showJieSuanInfo(CMD_S_GameEnd* pGameEnd)
{
	float delayTime = 2.0f;
	cocos2d::Node* pNiaoCardAni = WidgetFun::getChildWidget(this,"GameEndNiaoCardAni");
	pNiaoCardAni->setVisible(false);
	if (pGameEnd->cbNiaoCount>0)
	{
		if (HaveOptionRule(GAME_OPTION_RULE_NIAO_FLY))
		{
			setGameEndNiaoCardAniFly(pGameEnd,delayTime);
		}
		else if (HaveOptionRule(GAME_OPTION_RULE_NIAO_ANIM_2))
		{
			pNiaoCardAni->runAction(cocos2d::CCVisibleAction::create(delayTime,true));
			setGameEndNiaoCardAni2(pNiaoCardAni,pGameEnd->cbCardDataNiao,pGameEnd->cbNiaoCount,delayTime);
		}
		else
		{
			pNiaoCardAni->runAction(cocos2d::CCVisibleAction::create(delayTime,true));
			setGameEndNiaoCardAni1(pNiaoCardAni,pGameEnd->cbCardDataNiao,pGameEnd->cbNiaoCount,delayTime);
		}
	}
	if (HaveOptionRule(GAME_OPTION_RULE_NIAO_DIRECT))
	{
		delayTime += 2.0f;
	}
	else
	{
		delayTime += 2.0f;
	}
	GameManagerBase::InstanceBase().OnGameEnd(this,delayTime+3.0f);

	cocos2d::Node* pJieSuanNode = WidgetFun::getChildWidget(this,"GameJieSuanNode");
	WidgetFun::setWidgetUserInfo(this,"GameJieSuanNode","Time",utility::toString((int)delayTime));
	cocos2d::CCVisibleAction* pAction = cocos2d::CCVisibleAction::create(delayTime,true);
	pAction->setTag(1);
	pJieSuanNode->runAction(pAction);
	pNiaoCardAni->runAction(cocos2d::CCVisibleAction::create(delayTime,false));

	TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(HNMJGameScence::upSelfFreeReadState,this),delayTime+1);

	 WidgetFun::setPlaceText(pJieSuanNode,"JieSuan_JuShu",utility::toString(getPlayCount()));
	 setJieSuanNiaoCard(pJieSuanNode,pGameEnd->cbCardDataNiao,pGameEnd->cbNiaoCount);
	 
	 bool isLiuJu = true;
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
		if (nHuScore > 0)
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
			pGameEnd->cbCardData[iChirID],pGameEnd->cbCardCount[iChirID],pGameEnd->cbChiHuCardData[iChirID]);
		
		if(pGameEnd->lGameScore[iChirID] != 0)
		{
			isLiuJu = false;
		}
	 }

	 for (int i=0;i<MAX_PLAYER;i++)
	 {
		 int iChirID = m_pPlayer[i]->GetChairID();
		 int nHuScore = pGameEnd->lGameScore[iChirID] - pGameEnd->lStartHuScore[iChirID]-pGameEnd->lGangScore[iChirID];

		 cocos2d::Node* pJieSuanInfo = WidgetFun::getChildWidget(pJieSuanNode,utility::toString("JieSuanInfo",i));
		 WidgetFun::setVisible(pJieSuanInfo,"Jiesuan_HuType",true);

		 WidgetFun::setNodeKeepAfter(pJieSuanInfo,WidgetFun::getChildWidget(pJieSuanInfo,"Jiesuan_Nicke"),"Jiesuan_HuType");
		 std::string kHuType;
		 if(nHuScore > 0)
		 {
			if (isZimo)
			{
				 kHuType = ScriptData<std::string>("HU_TYPE_ZIMO").Value();
			}
			else
			{
				 kHuType = ScriptData<std::string>("HU_TYPE_JIEPAO").Value();
			}
		 }
		 else if (nHuScore<0 && !isZimo)
		 {
			kHuType = ScriptData<std::string>("HU_TYPE_FANGPAO").Value();
		 }

		 if (FvMask::HasAny(m_kRoomInfo.bGameRuleIdex,_MASK_(GAME_TYPE_ZZ_ZHANIAO_DIFF)))
		 {
			 if ((int)pGameEnd->cbChairIDNiaoCout[iChirID] > 0)
			 {
				 kHuType += utility::toString(ScriptData<std::string>("ZHONG_NIAO").Value(),(int)pGameEnd->cbChairIDNiaoCout[iChirID],
					 ScriptData<std::string>("ZHONG_GE").Value());
			 }
		 }
		 else
		 {
			 if ((int)pGameEnd->cbNiaoPick > 0 && nHuScore > 0)
			 {
				kHuType += utility::toString(ScriptData<std::string>("ZHONG_NIAO").Value(),(int)pGameEnd->cbNiaoPick,ScriptData<std::string>("ZHONG_GE").Value());
			 }
		 }

		 if (pGameEnd->lGangScore[iChirID]>0)
		 {
			 kHuType += getStringGang(iChirID,pGameEnd->WeaveItemArray[iChirID],pGameEnd->cbWeaveCount[iChirID]);
		 }
		// if (pGameEnd->lStartHuScore[iChirID] >0)
		 {
			 kHuType += getStringHuRight(pGameEnd->dwStartHuRight[iChirID]);
		 }
		 //if (pGameEnd->lGameScore[iChirID] >0)
		 {
			 kHuType += getStringHuRight(pGameEnd->dwChiHuRight1[iChirID]);
			 kHuType += getStringHuRight(pGameEnd->dwChiHuRight2[iChirID]);
		 }

		 if (pGameEnd->cbPiaoBei[iChirID] >1)
		 {
			 kHuType += utility::toString(ScriptData<std::string>("PIAO").Value(),
				 (int)(pGameEnd->cbPiaoBei[iChirID]-1),ScriptData<std::string>("PIAO_Bei").Value());
		 }

		 WidgetFun::setText(pJieSuanInfo,"Jiesuan_HuType",kHuType);
	 }

	 int nChairID = m_pLocal->GetChairID();
	 cocos2d::Node* pJieSuanTitle = WidgetFun::getChildWidget(pJieSuanNode,"JieSuan_Title");
	 cocos2d::Node* pJieSuanNPC = WidgetFun::getChildWidget(pJieSuanNode,"JieSuan_NPC");
	 if (pGameEnd->lGameScore[nChairID]>0)
	 {
		 std::string kTitleWin = WidgetFun::getWidgetUserInfo(pJieSuanTitle,"Title_Win");
		 std::string kNPCXiao = WidgetFun::getWidgetUserInfo(pJieSuanNPC,"NPC_Smile");

		 WidgetFun::setImagic(pJieSuanTitle,kTitleWin,false);
		 WidgetFun::setImagic(pJieSuanNPC,kNPCXiao,false);
	 }
	 else if (pGameEnd->lGameScore[nChairID]== 0)
	 {
		 std::string kTitlePing = WidgetFun::getWidgetUserInfo(pJieSuanTitle,"Title_Ping");
		 if (isLiuJu == true)
		 {
			 kTitlePing =  WidgetFun::getWidgetUserInfo(pJieSuanTitle,"Title_LiuJu");
		 }
		 std::string kNPCXiao = WidgetFun::getWidgetUserInfo(pJieSuanNPC,"NPC_Smile");

		 WidgetFun::setImagic(pJieSuanTitle,kTitlePing,false);
		 WidgetFun::setImagic(pJieSuanNPC,kNPCXiao,false);
	 }
	 else if (pGameEnd->lGameScore[nChairID] < 0)
	 {
		 std::string kTitleLose = WidgetFun::getWidgetUserInfo(pJieSuanTitle,"Title_Lose");
		 std::string kNPCKu = WidgetFun::getWidgetUserInfo(pJieSuanNPC,"NPC_Cry");

		 WidgetFun::setImagic(pJieSuanTitle,kTitleLose,false);
		 WidgetFun::setImagic(pJieSuanNPC,kNPCKu,false);
	 }

}
void HNMJGameScence::setJieSuanNiaoCard( cocos2d::Node* pNode,BYTE* pNiaoCard,BYTE cbCardNum )
{
	ASSERT(pNode);

	if (WidgetFun::getChildWidget(pNode,"JieSuanNiaoNode"))
	{
		WidgetFun::setVisible(pNode,"JieSuanNiaoNode",cbCardNum != 0);
	}

	cocos2d::Node* pNiaoNode = WidgetFun::getChildWidget(pNode,"JieSuan_CardNiao");
 	pNiaoNode->removeAllChildren();

	int iIdex = 0;
	int iOder = 0;
	cocos2d::Vec2 kStartPos = cocos2d::Vec2(0,0);
	std::string kHandSkin = utility::toString("HNMJ_HAND_",iIdex);
	cocos2d::Vec2 kHandAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNiaoNode,"JieSuanAddPos"));
	for (int i=0;i<cbCardNum;i++)
	{
		ASSERT(HNMJ::CGameLogic::Instance().IsValidCard(pNiaoCard[i]));

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


void HNMJGameScence::setGameEndNiaoCardAni1(cocos2d::Node* pNode,BYTE* pNiaoCard,BYTE cbCardNum,float& delayTime)
{
	ASSERT(pNode);

	cocos2d::Node* pNiaoNode = WidgetFun::getChildWidget(pNode,"NiaoCardAniNode");
	pNiaoNode->removeAllChildren();

	cocos2d::Vec2 kHandAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNiaoNode,"JieSuanAddPos"));
	int iOder = 0;

	float fWidth = (700 - kHandAddPos.x * (int)cbCardNum)/(int)(cbCardNum+1)+kHandAddPos.x;

	cocos2d::Vec2 kStartPos = cocos2d::Vec2(-350-kHandAddPos.x/2,0);
	std::string kHandSkin = "HNMJ_NIAO_CARD_NODE1";
	for (int i=0;i<cbCardNum;i++)
	{
		kStartPos.x += fWidth;
		ASSERT(HNMJ::CGameLogic::Instance().IsValidCard(pNiaoCard[i]));

		cocos2d::Node* pCardNode = WidgetManager::Instance().createWidget(kHandSkin,pNiaoNode);
		pCardNode->setTag(1);
		pCardNode->setLocalZOrder(iOder);
		pCardNode->setPosition(kStartPos);
		
		cocos2d::Node* pCard = WidgetFun::getChildWidget(pCardNode,"HNMJ_NIAO_CARD");
		std::string kImagic  = WidgetFun::getWidgetUserInfo(pCard,"MingImagic");
		m_pLocal->setCardImagic(pCard,pNiaoCard[i],kImagic);
		
		WidgetFun::setWidgetUserInfo(pCard,"ShowDelayTime",utility::toString(delayTime));

		float fBombDelay = utility::parseFloat(WidgetFun::getWidgetUserInfo(pCardNode,"BombDelay"));
		cocos2d::Node* pBomb = WidgetFun::getChildWidget(pCardNode,"Bomb");
		WidgetFun::setWidgetUserInfo(pBomb,"BombDelayTime",utility::toString(delayTime+fBombDelay));
		pCard->setVisible(false);
		pBomb->setVisible(false);
		WidgetFun::runWidgetAction(pCard,"ShowAni");
		WidgetFun::runWidgetAction(pBomb,"ShowAni");

		if (!HaveOptionRule(GAME_OPTION_RULE_NIAO_DIRECT))
		{
			delayTime += fBombDelay+0.7;
		}
		iOder++;
	}
}
void HNMJGameScence::setGameEndNiaoCardAni2(cocos2d::Node* pNode,BYTE* pNiaoCard,BYTE cbCardNum,float& delayTime)
{

	ASSERT(pNode);

	cocos2d::Node* pNiaoNode = WidgetFun::getChildWidget(pNode,"NiaoCardAniNode");
	pNiaoNode->removeAllChildren();

	cocos2d::Vec2 kHandAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNiaoNode,"JieSuanAddPos"));
	int iOder = 0;

	float fWidth = (700 - kHandAddPos.x * (int)cbCardNum)/(int)(cbCardNum+1)+kHandAddPos.x;

	cocos2d::Vec2 kStartPos = cocos2d::Vec2(-350-kHandAddPos.x/2,0);
	std::string kHandSkin = "HNMJ_NIAO_CARD_NODE2";
	for (int i=0;i<cbCardNum;i++)
	{
		kStartPos.x += fWidth;
		ASSERT(HNMJ::CGameLogic::Instance().IsValidCard(pNiaoCard[i]));

		cocos2d::Node* pCardNode = WidgetManager::Instance().createWidget(kHandSkin,pNiaoNode);
		pCardNode->setTag(1);
		pCardNode->setLocalZOrder(iOder);
		pCardNode->setPosition(kStartPos);

		cocos2d::Node* pCard = WidgetFun::getChildWidget(pCardNode,"HNMJ_NIAO_CARD");
		WidgetFun::setWidgetUserInfo(pCard,"ShowDelayTime",utility::toString(delayTime));

		BYTE cbValue= HNMJ::CGameLogic::Instance().GetCardValue(pNiaoCard[i]);
		BYTE cbColor=HNMJ::CGameLogic::Instance().GetCardColor(pNiaoCard[i]);
		WidgetFun::setWidgetUserInfo(pCard,"Pic1",utility::toString("GameHNMJ/2/handmah_",(int)cbColor,(int)cbValue,".png"));
		WidgetFun::setWidgetUserInfo(pCard,"Pic2",utility::toString("GameHNMJ/2/mingmah_",(int)cbColor,(int)cbValue,".png"));

		WidgetFun::runWidgetAction(pCard,"ShowAni");

		if (!HaveOptionRule(GAME_OPTION_RULE_NIAO_DIRECT))
		{
			delayTime += 0.7f;
		}
		iOder++;
	}
}

void HNMJGameScence::setGameEndNiaoCardAniFly(CMD_S_GameEnd* pGameEnd,float& delayTime)
{
	if (!FvMask::HasAny(m_kRoomInfo.bGameRuleIdex,_MASK_(GAME_TYPE_ZZ_ZHANIAO_DIFF)))
	{
		for (int i=0;i<GAME_PLAYER;i++)
		{
			HNMJPlayer * pPlayer = getPlayerByChairID(i);
			int nHuScore = pGameEnd->lGameScore[i] - pGameEnd->lStartHuScore[i]-pGameEnd->lGangScore[i];
			if (nHuScore>0 && pGameEnd->cbNiaoPick>0)
			{
				pPlayer->runAniNiaoFly(delayTime);
				delayTime+=0.5f;
			}
		}
		return;
	}
	for (int i=0;i<GAME_PLAYER;i++)
	{
		int nNum = pGameEnd->cbChairIDNiaoCout[i];
		if (nNum>0)
		{
			HNMJPlayer * pPlayer = getPlayerByChairID(i);
			if (pPlayer)
			{
				pPlayer->runAniNiaoFly(delayTime);
				delayTime+=0.5f;
			}
		}
	}
}


bool HNMJGameScence::BackKey()
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

void HNMJGameScence::setGameState(int nState)
{
	m_nGameState = nState;
}
int HNMJGameScence::getGameState()
{
	return m_nGameState;
}

