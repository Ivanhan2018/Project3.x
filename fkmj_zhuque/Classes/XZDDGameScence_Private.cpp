#include "XZDDGameScence.h"
#include "GameLib.h"
#include "XZDDPlayer.h"
#include "MissionWeiXin.h"

void XZDDGameScence::pushGameRule(std::vector<std::string>&kRuleList, dword dwGameRule,int nRuleTag,bool bShow)
{
	if (FvMask::HasAny(dwGameRule,_MASK_((dword)nRuleTag))&&bShow)
	{
		kRuleList.push_back(ScriptData<std::string>(utility::toString("SCMJRule",(int)nRuleTag)).Value());
	}
}
void XZDDGameScence::initPrivate()
{
	WidgetScenceXMLparse kScence("GameXZDD/xml/HNGamePrivateScence.xml",this);

	WidgetManager::addButtonCB("Button_PrivateWeiXinFriend",this,button_selector(XZDDGameScence::XZDDButton_WeiXinFriend));
	WidgetManager::addButtonCB("Button_PrivateEndWeiXinShare",this,button_selector(XZDDGameScence::XZDDButton_WeiXinImagic));
	WidgetManager::addButtonCB("ButtonPlayerHeadClick",this,button_selector(XZDDGameScence::ButtonPlayerHeadClick));
}
void XZDDGameScence::defaultPrivateState()
{
	if (WidgetFun::getChildWidget(this,"PrivateDismissPlane"))
	{
		WidgetFun::setVisible(this,"PrivateDismissPlane",false);
		WidgetFun::setVisible(this,"Button_PrivateWeiXinFriend",false);
		WidgetFun::setVisible(this,"TalkImagic",false);
	}
	if (WidgetFun::getChildWidget(this,"GameJieSuanNode"))
	{
		WidgetFun::setVisible(this,"GameJieSuanNode",false);
	}
}
void XZDDGameScence::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	WidgetFun::setVisible(this,"PrivateInfo",true);
	WidgetFun::setVisible(this,"MenuNode",false);
	setPrivateInfo(pNetInfo);
	setPlayCount(pNetInfo->dwPlayCout);
	bool bShowDismiss = false;
	if (pNetInfo->dwCreateUserID == m_pLocal->GetUserID())
	{
		if (pNetInfo->bStartGame || pNetInfo->dwPlayCout == 0)
		{
			bShowDismiss = true;
		}
	}
	if (pNetInfo->bStartGame)
	{
		bShowDismiss = true;
	}
	WidgetFun::setVisible(this,"Button_PrivateWeiXinFriend",!pNetInfo->bStartGame );
	WidgetFun::setVisible(this,"Button_DismissPrivate",bShowDismiss);

	if (HaveOptionRule(GAME_OPTION_RULE_SHOW_WIN_LOSE1))
	{
		for (int i = 0;i<MAX_PLAYER && i<(int)pNetInfo->kWinLoseScore.size();i++)
		{
			XZDDPlayer* pPlayer = getPlayerByChairID(i);
			if (!pPlayer)
			{
				continue;
			}
			WidgetFun::setText(pPlayer->getPlayerNode(),"GoldTxt",1000+(int)pNetInfo->kWinLoseScore[i]);
		}
	}

	if (HaveOptionRule(GAME_OPTION_RULE_SHOW_WIN_LOSE_ZERO))
	{
		for (int i = 0;i<MAX_PLAYER && i<(int)pNetInfo->kWinLoseScore.size();i++)
		{
			XZDDPlayer* pPlayer = getPlayerByChairID(i);
			if (!pPlayer)
			{
				continue;
			}
			WidgetFun::setText(pPlayer->getPlayerNode(),"GoldTxt",(int)pNetInfo->kWinLoseScore[i]);
		}
	}
}

void XZDDGameScence::setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	m_kPrivateRoomInfo = *pNetInfo;
	WidgetFun::setText(this,"RoomID",utility::toString(pNetInfo->dwRoomNum));
	if (pNetInfo->bPlayCoutIdex == 0)
	{
		WidgetFun::setPlaceTextKey(this,"RoomNum","Txt",utility::toString((int)pNetInfo->dwPlayCout,"/",pNetInfo->dwPlayTotal));
	}
	else
	{
		WidgetFun::setPlaceTextKey(this,"RoomNum","Txt",utility::toString((int)pNetInfo->dwPlayCout,"/",pNetInfo->dwPlayTotal));
	}

	std::vector<std::string> kRuleList;
	pushGameRule(kRuleList,pNetInfo->bGameRuleIdex,GAME_RULE_XZ,true);
	pushGameRule(kRuleList,pNetInfo->bGameRuleIdex,GAME_RULE_XL,true);
	pushGameRule(kRuleList,pNetInfo->bGameRuleIdex,GAME_RULE_HUAN3,true);
	pushGameRule(kRuleList,pNetInfo->bGameRuleIdex,GAME_RULE_3FAN,true);
	pushGameRule(kRuleList,pNetInfo->bGameRuleIdex,GAME_RULE_4FAN,true);
	pushGameRule(kRuleList,pNetInfo->bGameRuleIdex,GAME_RULE_ZIMO_DI,true);
	pushGameRule(kRuleList,pNetInfo->bGameRuleIdex,GAME_RULE_ZIMO_FAN,true);
	pushGameRule(kRuleList,pNetInfo->bGameRuleIdex,GAME_RULE_YAOJIU_JIANGDUI,true);
	pushGameRule(kRuleList,pNetInfo->bGameRuleIdex,GAME_RULE_TIANDI_HU,true);
	pushGameRule(kRuleList,pNetInfo->bGameRuleIdex,GAME_RULE_5FAN,true);
	pushGameRule(kRuleList,pNetInfo->bGameRuleIdex,GAME_RULE_DAODAO_HU,true);
	pushGameRule(kRuleList,pNetInfo->bGameRuleIdex,GAME_RULE_DIANPAO,true);
	pushGameRule(kRuleList,pNetInfo->bGameRuleIdex,GAME_RULE_ZIMOHU,true);
	pushGameRule(kRuleList,pNetInfo->bGameRuleIdex,GAME_RULE_QIDUI,true);

	for (int i=0;i<MAX_GAME_RULE;i++)
	{
		WidgetFun::setVisible(this,utility::toString("RoomGameRule",i),false);
	}
	for (int i=0;i<(int)kRuleList.size() && i < MAX_GAME_RULE;i++)
	{
		WidgetFun::setVisible(this,utility::toString("RoomGameRule",i),true);
		WidgetFun::setText(this,utility::toString("RoomGameRule",i),kRuleList[i]);
	}
}

void XZDDGameScence::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
{
	defaultPrivateState();
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"PrivateEndPlane"); 
	if (pNetInfo->lPlayerWinLose.size() != MAX_PLAYER
		||pNetInfo->lPlayerAction.size() != MAX_PLAYER*MAX_PRIVATE_ACTION)
	{
		CCAssert(false,"pNetInfo->lPlayerWinLose.size() != MAX_PLAYER");
		return;
	}

	int nMaxPao = 0;
	int nMaxWin = 0;
	int nMaxPaoID = -1;
	int nMaxWinID = -1;
	for (int i  = 0;i<MAX_PLAYER;i++)
	{
		GamePlayer* pPlayer = m_pPlayer[i];
		if (!pPlayer)
		{
			continue;
		}
		int iChairID = pPlayer->GetChairID();
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pRootNode,utility::toString(utility::toString("PrivateEndPlayer",i)));
		WidgetFun::setText(pNode,"Name",pPlayer->GetNickName());
		WidgetFun::setText(pNode,"ID",(int)pPlayer->GetUserID());
		WidgetFun::setText(pNode,"ActionTxt0",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+0]));
		WidgetFun::setText(pNode,"ActionTxt1",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+1]));
		WidgetFun::setText(pNode,"ActionTxt2",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+2]));
		WidgetFun::setText(pNode,"ActionTxt3",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+3]));
		WidgetFun::setText(pNode,"ActionTxt4",utility::toString("x",(int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+4]));
		WidgetFun::setText(pNode,"TotalScore",utility::toString((int)pNetInfo->lPlayerWinLose[iChairID]));
		ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(pNode,"Head"),pPlayer->GetUserInfo()->szHeadHttp,pPlayer->GetUserID());
		WidgetFun::setVisible(pNode,"DaYingJia",false);
		WidgetFun::setVisible(pNode,"ZuiJiaPaoShou",false);
		WidgetFun::setVisible(pNode,"FangZhu",pPlayer->GetUserID() == m_kPrivateRoomInfo.dwCreateUserID);

		int nPaoNum = (int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+2];
		int nWinNum = (int)pNetInfo->lPlayerWinLose[iChairID];
		if (nPaoNum>nMaxPao)
		{
			nMaxPao = nPaoNum;
			nMaxPaoID = i;
		}
		if (nWinNum>nMaxWin)
		{
			nMaxWin = nWinNum;
			nMaxWinID = i;
		}
	}

	if (nMaxPao>0)
	{
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pRootNode,utility::toString(utility::toString("PrivateEndPlayer",nMaxPaoID)));
		WidgetFun::setVisible(pNode,"ZuiJiaPaoShou",true);
	}
	if (nMaxWin>0)
	{
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pRootNode,utility::toString(utility::toString("PrivateEndPlayer",nMaxWinID)));
		WidgetFun::setVisible(pNode,"DaYingJia",true);
	}
	cocos2d::Node* pJieSuanNode = WidgetFun::getChildWidget(this,"GameResoult1");
	if (pJieSuanNode->getActionByTag(1) == NULL 
		&& pJieSuanNode->isVisible() == false)
	{
		WidgetFun::setVisible(pRootNode,"PrivateEndPlane",true);
	}
	else
	{
		int iTime = 3;
		cocos2d::Action* pAction = cocos2d::CCVisibleAction::create(iTime,true);
		pAction->setTag(1);
		WidgetFun::getChildWidget(pRootNode,"PrivateEndPlane")->runAction(pAction);
	}

	if(HaveOptionRule(GAME_OPTION_RULE_PRIVATAEND_RETURN_HOME))
	{
		WidgetFun::setVisible(pRootNode,"Button_PrivateReturnHome",true);
		WidgetFun::setVisible(pRootNode,"Button_PrivateAgagin",false);
	}
}
void XZDDGameScence::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"PrivateDismissPlane");
	if (pNetInfo->dwDissUserCout == 0)
	{
		pNode->runAction(cocos2d::CCVisibleAction::create(1.0f,false));
		return;
	}

	if (!pNode->isVisible())
	{
		cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"DissPrivateStateWaiteTime");
		pNode->stopAllActions();
		pNode->runAction(cocos2d::MoveExTxtTime::create(pNetInfo->dwValue2));
	}
	pNode->setVisible(true);

	int kChairID[4] = {1,1,1,1};

	bool bShowSelfAction = true;
	int iIdex = 0;
	for (int i = 0;i<(int)pNetInfo->dwDissUserCout;i++)
	{
		std::string kName = "";
		GamePlayer* pPlayer = getPlayerByChairID(pNetInfo->dwDissChairID[i]);
		kChairID[pNetInfo->dwDissChairID[i]] = 0;
		if (pPlayer)
		{
			kName = pPlayer->GetNickName();
		}
		if (pPlayer == m_pLocal)
		{
			bShowSelfAction = false;
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),kName);
		WidgetFun::setPlaceTextKey(pNode,utility::toString("DissPrivateState",iIdex),"Text0",kName);
		iIdex++;
	}
	for (int i = 0;i<(int)pNetInfo->dwNotAgreeUserCout;i++)
	{
		std::string kName = "";
		GamePlayer* pPlayer = getPlayerByChairID(pNetInfo->dwNotAgreeChairID[i]);
		kChairID[pNetInfo->dwNotAgreeChairID[i]] = 0;
		if (pPlayer)
		{
			kName = pPlayer->GetNickName();
		}
		if (pPlayer == m_pLocal)
		{
			bShowSelfAction = false;
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),kName);
		WidgetFun::setPlaceTextKey(pNode,utility::toString("DissPrivateState",iIdex),"Text1",kName);
		iIdex++;
	}
	for (int i = 0;i<4;i++)
	{
		if (kChairID[i] == 0)
		{
			continue;
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),"");
		WidgetFun::setText(pNode,utility::toString("DissPrivateState",iIdex),"");
		std::string kName = "";
		GamePlayer* pPlayer = getPlayerByChairID(i);
		if (!pPlayer)
		{
			continue;
		}
		if (pPlayer)
		{
			kName = pPlayer->GetNickName();
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),kName);
		iIdex++;
	}
	WidgetFun::setVisible(pNode,"PrivateDismissAction",bShowSelfAction);
}
void XZDDGameScence::XZDDButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*)
{
	std::string kHNWeiXinSharDes,kGameTypeStr;

	std::vector<std::string> kRuleList;
	pushGameRule(kRuleList,m_kPrivateRoomInfo.bGameRuleIdex,GAME_RULE_XZ,true);
	pushGameRule(kRuleList,m_kPrivateRoomInfo.bGameRuleIdex,GAME_RULE_XL,true);
	pushGameRule(kRuleList,m_kPrivateRoomInfo.bGameRuleIdex,GAME_RULE_HUAN3,true);
	pushGameRule(kRuleList,m_kPrivateRoomInfo.bGameRuleIdex,GAME_RULE_3FAN,true);
	pushGameRule(kRuleList,m_kPrivateRoomInfo.bGameRuleIdex,GAME_RULE_4FAN,true);
	pushGameRule(kRuleList,m_kPrivateRoomInfo.bGameRuleIdex,GAME_RULE_ZIMO_DI,true);
	pushGameRule(kRuleList,m_kPrivateRoomInfo.bGameRuleIdex,GAME_RULE_ZIMO_FAN,true);
	pushGameRule(kRuleList,m_kPrivateRoomInfo.bGameRuleIdex,GAME_RULE_YAOJIU_JIANGDUI,true);

	for (int i=0;i<kRuleList.size();i++)
	{
		kGameTypeStr += kRuleList[i]+",";
	}

	kHNWeiXinSharDes = utility::getScriptReplaceValue("SCWeiXinSharDes",
		(int)m_kPrivateRoomInfo.dwRoomNum,(int)m_kPrivateRoomInfo.dwPlayTotal,kGameTypeStr);
	std::string kTitle = utility::getScriptString("SCWeiXinSharTitle")+"-"+utility::toString(m_kPrivateRoomInfo.dwRoomNum);
	MissionWeiXin::Instance().shareUrlWeiXin(utility::getScriptString("WeiXinSharUrl"),
		kTitle,
		kHNWeiXinSharDes,
		MissionWeiXin::SHARE_SESSION);
}
void XZDDGameScence::XZDDButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*)
{
	static XZDDGameScence* pTemp = NULL;
	pTemp = this;
	cocos2d::utils::captureScreen([](bool b, std::string name){
		if (b)
		{
			MissionWeiXin::Instance().shareScreenWeiXin(name);
			pTemp->Button_ExitGameBase(NULL,NULL);
			pTemp->ExitGame();
			WidgetFun::setVisible(pTemp,"PrivateEndPlane",false); 
		}
	},"screenshot.png");


}
void XZDDGameScence::ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	GamePlayer* pPlayer = getBasePlayerByUserID(utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"UserID")));
	if (!pPlayer)
	{
		return;
	}
	PopScence::Instance().showIPInfo(pPlayer->GetNickName(),
		utility::paseInt(pPlayer->GetUserID(),6),
		pPlayer->GetUserInfo()->szLogonIP,
		pPlayer->GetUserInfo()->szHeadHttp);
}
void XZDDGameScence::setPlayCount(int nCount)
{
	m_nPlayCount = nCount;
}
int XZDDGameScence::getPlayCount()
{
	return m_nPlayCount;
}