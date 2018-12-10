#include "NCMJGameScence.h"
#include "NCMJPlayer.h"
#include "GameLib.h"
#include "CMD_NCMJ.h"
#include "NCMJPlayer.h"
#include "NCMJGameLogic.h"
#include "NCMJSoundFun.h"
#include "ScriptXMLparse.h"

FV_SINGLETON_STORAGE(NCMJGameScence);

NCMJGameScence::NCMJGameScence()
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
{

	init();
}
NCMJGameScence::~NCMJGameScence()
{

}
bool NCMJGameScence::init()
{
	if (!GameBase::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("GameNCMJ/xml/GameScence.xml",this);
	cocos2d::ScriptXMLparse kScriptXml1("GameNCMJ/xml/ScriptValueStr.xml");

	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"GameTypeScenceNode");
	WidgetScenceXMLparse kScence2("GameNCMJ/xml/GameScence_THJ.xml",pNode);

	if (WidgetFun::getChildWidget(this,"GameTalkList"))
	{
		cocos2d::ListViewEx* pTalkList = WidgetFun::getListViewWidget(this,"GameTalkList");
		pTalkList->removeAllChildren();
		for (int i=0;i<10;i++)
		{
			cocos2d::Node* pItem = WidgetManager::Instance().createWidget("NCMJGameTalkListItem",pTalkList);
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
				std::string kFileName = utility::toString("GameNCMJ/Talk/EE",m*8+n,".png");
				WidgetFun::setButtonImagic(pNode,utility::toString("NCMJButton_BiaoQing",n),kFileName,true);
				WidgetFun::setWidgetUserInfo(pNode,utility::toString("NCMJButton_BiaoQing",n),"File",kFileName);
			}
		}
	}

	initPrivate();

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i] = new NCMJPlayer(i,WidgetFun::getChildWidget(this,utility::toString("Player",i)));
	}
	m_pLocal = m_pPlayer[0];

	WidgetManager::addButtonCB("NCMJButton_GameExit",this,button_selector(GameBase::Button_ExitGameBase));

	initButton();
	initNet();
	initTouch();
	initSetButton();
	return true;
}
void NCMJGameScence::EnterScence()
{
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->PlayerLeave();
	}
	WidgetFun::setVisible(this,"PrivateInfo",false);
	WidgetFun::setVisible(this,"PrivateEndPlane",false);
	WidgetFun::setVisible(this,"GameNCMJPlane",true);
	defaultState();
	NCMJSoundFun::playBackMusic("raw/csmj.mp3");
}
bool NCMJGameScence::IsInGame()
{
	return WidgetFun::isWidgetVisble(this,"GameNCMJPlane");
}
void NCMJGameScence::HideAll()
{
	WidgetFun::setVisible(this,"GameNCMJPlane",false);
	defaultPrivateState();
	defaultRecordState();
	cocos2d::Node* pEndPlane = WidgetFun::getChildWidget(this,"PrivateEndPlane");
	if (pEndPlane && pEndPlane->getActionByTag(1))
	{
		pEndPlane->stopAllActions();
		pEndPlane->setVisible(true);
	}
}
void NCMJGameScence::defaultState()
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

	defaultPrivateState();
	defaultPlayerActionState();
	defaultRecordState();
	defaultMaster();
}
void NCMJGameScence::defaultPlayerActionState()
{
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"TimeNode");
	for (int i = 0;i<MAX_PLAYER;i++)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",i),false);
	}
	WidgetFun::getChildWidget(pRootNode,"ActPlayerLastTime")->stopAllActions();
	WidgetFun::setText(pRootNode,"ActPlayerLastTime","0");
}
std::string NCMJGameScence::getStringHuRight(dword kValue)
{
	dword dwRight[] = {
		CHR_PENGPENG_HU		,							//碰碰胡
		CHR_JIANGJIANG_HU		,							//将将胡
		CHR_QING_YI_SE		,							//清一色
		CHR_HAI_DI_LAO		,							//海底捞
		CHR_HAI_DI_PAO		,							//海底捞
		CHR_QI_XIAO_DUI		,							//七小对
		CHR_HAOHUA_QI_XIAO_DUI	,							//豪华七小对
		CHR_GANG_KAI			,							//杠上开花
		CHR_QIANG_GANG_HU		,							//抢杠胡
		CHR_GANG_SHANG_PAO	,							//杠上跑
		CHR_QUAN_QIU_REN		,							//全求人
		CHR_TIAN_HU		,							//天胡
		CHR_DI_HU		,							//地胡
		CHR_SHI_SAN_LAN		,							//十三烂
		CHR_QI_XING									//七星
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
	};
	std::string kTxt;
	NCMJ::CChiHuRight	kChiHuRight;	
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

std::string NCMJGameScence::getStringGang(int nChairID,CMD_WeaveItem* pWeave,int iWeaveCout )
{
	std::string kStr;
	for (int i = 0;i<iWeaveCout;i++)
	{
		CMD_WeaveItem* pTemp = pWeave+i;
		if (pTemp->cbWeaveKind == WIK_GANG)
		{
			NCMJPlayer* pPlayer = getPlayerByChairID(pTemp->wProvideUser);
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
				kStr = utility::toString(kStr,"(",pPlayer->GetNickName(),")");
			}
		}
	
	}
	return kStr;
}


void NCMJGameScence::showSaiZi(unsigned int iValue)
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
void NCMJGameScence::showUserAction(int iUserAction)
{
	int iCout = 0;
	int iStartPos = 0;
	int iNextPos = 0;
	if (BYTE(iUserAction)&WIK_GANG)
	{
		iCout++;
	}
	if (BYTE(iUserAction)&WIK_PENG)
	{
		iCout++;
	}
	if (BYTE(iUserAction)&WIK_LEFT || BYTE(iUserAction)&WIK_CENTER || BYTE(iUserAction)&WIK_RIGHT)
	{
		iCout++;
	}
	if (BYTE(iUserAction)&WIK_CHI_HU || BYTE(iUserAction)&WIK_ZI_MO)
	{
		iCout++;
	}
	if (BYTE(iUserAction)&WIK_XIAO_HU)
	{
		iCout++;
	}
	if (iCout == 1)
	{
		iStartPos = -140;
		iNextPos = 250;
	}
	if (iCout == 2)
	{
		iStartPos = -200;
		iNextPos = 280;
	}
	if (iCout == 3)
	{
		iStartPos = -300;
		iNextPos = 280;
	}
	if (iCout == 4)
	{
		iStartPos = -300;
		iNextPos = 250;
	}
	if (BYTE(iUserAction)&WIK_CHI_HU || BYTE(iUserAction)&WIK_ZI_MO)
	{
		WidgetFun::setPos(this,"NCMJButton_HuAction",cocos2d::Vec2(iStartPos,0));
		iStartPos+=iNextPos;
	}
	if (BYTE(iUserAction)&WIK_XIAO_HU )
	{
		WidgetFun::setPos(this,"NCMJButton_XiaoHuAction",cocos2d::Vec2(iStartPos,0));
		iStartPos+=iNextPos;
	}
	if (BYTE(iUserAction)&WIK_GANG)
	{
		WidgetFun::setPos(this,"NCMJButton_GangAction",cocos2d::Vec2(iStartPos,0));
		iStartPos+=iNextPos;
	}
	if (BYTE(iUserAction)&WIK_PENG)
	{
		WidgetFun::setPos(this,"NCMJButton_PengAction",cocos2d::Vec2(iStartPos,0));
		iStartPos+=iNextPos;
	}
	if (BYTE(iUserAction)&WIK_LEFT || BYTE(iUserAction)&WIK_CENTER || BYTE(iUserAction)&WIK_RIGHT)
	{
		WidgetFun::setPos(this,"NCMJButton_PengAction",cocos2d::Vec2(iStartPos,0));
		iStartPos+=iNextPos;
	}
	WidgetFun::setPos(this,"NCMJButton_GuoAction",cocos2d::Vec2(iStartPos,0));

	WidgetFun::setVisible(this,"SelfActionNode",true);
	WidgetFun::setVisible(this,"NCMJButton_GangAction",(bool)(BYTE(iUserAction)&WIK_GANG));
	WidgetFun::setVisible(this,"NCMJButton_PengAction",(bool)(BYTE(iUserAction)&WIK_PENG));
	WidgetFun::setVisible(this,"NCMJButton_HuAction",(bool)(BYTE(iUserAction)&WIK_CHI_HU) || (bool)(BYTE(iUserAction)&WIK_ZI_MO));
	WidgetFun::setVisible(this,"NCMJButton_XiaoHuAction",(bool)(BYTE(iUserAction)&WIK_XIAO_HU));
	WidgetFun::setVisible(this,"NCMJButton_ChiAction",(bool)(BYTE(iUserAction)&WIK_LEFT) 
		|| (bool)(BYTE(iUserAction)&WIK_CENTER)
		|| (bool)(BYTE(iUserAction)&WIK_RIGHT));

	if (WidgetFun::isWidgetVisble(this,"NCMJButton_ChiAction"))
	{
		WidgetFun::setWidgetUserInfo(this,"NCMJButton_ChiAction","Chi_Type",utility::toString(iUserAction));
	}
}
void NCMJGameScence::setCurrentPlayer(int iCurrentPlayer,int iUserAction)
{
	if (iCurrentPlayer < 0 || iCurrentPlayer > MAX_PLAYER)
	{
		CCASSERT(false,"");
		return;
	}
	defaultPlayerActionState();

	m_iCurrentUser = iCurrentPlayer;

	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"TimeNode");

	NCMJPlayer* pPlyer = getPlayerByChairID(m_iCurrentUser);
	if (!pPlyer)
	{
		return;
	}
	WidgetFun::getChildWidget(pRootNode,"ActPlayerLastTime")->runAction(
		cocos2d::MoveExTxtTime::create(TIME_OPERATE_CARD));

	if (iUserAction == WIK_NULL)
	{
		WidgetFun::setVisible(pRootNode,utility::toString("TimePoint",pPlyer->getIdex()),true);
	}

	if(pPlyer == m_pLocal && iUserAction != WIK_NULL)
	{
		showUserAction(iUserAction);
	}
}

void NCMJGameScence::showJieSuanInfo(CMD_S_GameEnd* pGameEnd)
{
	float delayTime = 3.f;
	cocos2d::Node* pNiaoCardAni = WidgetFun::getChildWidget(this,"GameEndNiaoCardAni");
	pNiaoCardAni->setVisible(false);
	if (pGameEnd->cbNiaoCount>0)
	{
		pNiaoCardAni->runAction(cocos2d::CCVisibleAction::create(delayTime,true));
		delayTime += pGameEnd->cbNiaoCount*0.5;
		setGameEndNiaoCardAni(pNiaoCardAni,pGameEnd->cbCardDataNiao,pGameEnd->cbNiaoCount);
	}
	delayTime += 3.0f;

	cocos2d::Node* pJieSuanNode = WidgetFun::getChildWidget(this,"GameJieSuanNode");
	pJieSuanNode->runAction(cocos2d::CCVisibleAction::create(delayTime,true));
	pNiaoCardAni->runAction(cocos2d::CCVisibleAction::create(delayTime,false));

	 WidgetFun::setPlaceText(pJieSuanNode,"JieSuan_JuShu",utility::toString(getPlayCount()));
	 setJieSuanNiaoCard(pJieSuanNode,pGameEnd->cbCardDataNiao,pGameEnd->cbNiaoCount);
	 
	 bool isLiuJu = pGameEnd->cbLiuJu;
	 bool isZimo = false;
	 word wProvideUser = 0;
	 for (int i=0;i<MAX_PLAYER;i++)
	 {
		int iChirID = m_pPlayer[i]->GetChairID();
		int nHuScore = pGameEnd->lGameScore[iChirID] - pGameEnd->lStartHuScore[iChirID]-pGameEnd->lGangScoreInfo[iChirID];

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
			wProvideUser = pGameEnd->wProvideUser[iChirID];
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
		 int nHuScore = pGameEnd->lGameScore[iChirID] - pGameEnd->lStartHuScore[iChirID]-pGameEnd->lGangScoreInfo[iChirID];

		 cocos2d::Node* pJieSuanInfo = WidgetFun::getChildWidget(pJieSuanNode,utility::toString("JieSuanInfo",i));
		 WidgetFun::setVisible(pJieSuanInfo,"Jiesuan_HuType",true);

		 WidgetFun::setNodeKeepAfter(pJieSuanInfo,WidgetFun::getChildWidget(pJieSuanInfo,"Jiesuan_Nicke"),"Jiesuan_HuType");
		 std::string kHuType;
		 if(nHuScore>0 && isZimo)
		 {
			kHuType = ScriptData<std::string>("HU_TYPE_ZIMO").Value();
			if (pGameEnd->cbNiaoCount>0)
			{
				kHuType += utility::toString(ScriptData<std::string>("ZHONG_NIAO").Value(),(int)pGameEnd->cbNiaoPick,ScriptData<std::string>("ZHONG_GE").Value());
			}
		 }
		 else if(nHuScore>0 && !isZimo)
		 {
			 kHuType = ScriptData<std::string>("HU_TYPE_JIEPAO").Value();
			 if (pGameEnd->cbNiaoCount>0)
			 {
				 kHuType += utility::toString(ScriptData<std::string>("ZHONG_NIAO").Value(),(int)pGameEnd->cbNiaoPick,ScriptData<std::string>("ZHONG_GE").Value());
			 }
		 }
		 else if (nHuScore<0 && !isZimo && wProvideUser == iChirID)
		 {
			kHuType = ScriptData<std::string>("HU_TYPE_FANGPAO").Value();
		 }

		 if (!isLiuJu)
		 {
			 kHuType += getStringGang(iChirID,pGameEnd->WeaveItemArray[iChirID],pGameEnd->cbWeaveCount[iChirID]);
		 }
		 if (pGameEnd->lStartHuScore[iChirID] >0)
		 {
			 kHuType += getStringHuRight(pGameEnd->dwStartHuRight[iChirID]);
		 }
		 if (nHuScore >0)
		 {
			 kHuType += getStringHuRight(pGameEnd->dwChiHuRight[iChirID]);
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
void NCMJGameScence::setGameEndNiaoCardAni(cocos2d::Node* pNode,BYTE* pNiaoCard,BYTE cbCardNum)
{
	ASSERT(pNode);

	cocos2d::Node* pNiaoNode = WidgetFun::getChildWidget(pNode,"NiaoCardAniNode");
	pNiaoNode->removeAllChildren();

	cocos2d::Vec2 kHandAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNiaoNode,"JieSuanAddPos"));
	int iOder = 0;

	float fWidth = (700 - kHandAddPos.x * (int)cbCardNum)/(int)(cbCardNum+1)+kHandAddPos.x;

	cocos2d::Vec2 kStartPos = cocos2d::Vec2(-350-kHandAddPos.x/2,0);
	std::string kHandSkin = "NCMJ_NIAO_CARD_NODE";
	float fCardDelay = 3.f;
	for (int i=0;i<cbCardNum;i++)
	{
		kStartPos.x += fWidth;
		ASSERT(NCMJ::CGameLogic::Instance().IsValidCard(pNiaoCard[i]));

		cocos2d::Node* pCardNode = WidgetManager::Instance().createWidget(kHandSkin,pNiaoNode);
		pCardNode->setTag(1);
		pCardNode->setLocalZOrder(iOder);
		pCardNode->setPosition(kStartPos);

		cocos2d::Node* pCard = WidgetFun::getChildWidget(pCardNode,"NCMJ_NIAO_CARD");
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
void NCMJGameScence::setJieSuanNiaoCard( cocos2d::Node* pNode,BYTE* pNiaoCard,BYTE cbCardNum )
{
	ASSERT(pNode);

	cocos2d::Node* pNiaoNode = WidgetFun::getChildWidget(pNode,"JieSuan_CardNiao");
 	pNiaoNode->removeAllChildren();

	int iIdex = 0;
	int iOder = 0;
	cocos2d::Vec2 kStartPos = cocos2d::Vec2(0,0);
	std::string kHandSkin = utility::toString("NCMJ_HAND_",iIdex);
	cocos2d::Vec2 kHandAddPos = utility::parsePoint(WidgetFun::getWidgetUserInfo(pNiaoNode,"JieSuanAddPos"));
	for (int i=0;i<cbCardNum;i++)
	{
		ASSERT(NCMJ::CGameLogic::Instance().IsValidCard(pNiaoCard[i]));

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


void NCMJGameScence::setGameState(int nState)
{
	m_nGameState = nState;
}
int NCMJGameScence::getGameState()
{
	return m_nGameState;
}

