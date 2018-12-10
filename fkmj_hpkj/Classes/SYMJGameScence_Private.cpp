#include "SYMJGameScence.h"
#include "GameLib.h"
#include "SYMJPlayer.h"
#include "MissionWeiXin.h"

void SYMJGameScence::pushGameRule(std::vector<std::string>&kRuleList, dword dwGameRule,int nRuleTag,bool bShow)
{
	if (FvMask::HasAny(dwGameRule,_MASK_((dword)nRuleTag))&&bShow)
	{
		kRuleList.push_back(ScriptData<std::string>(utility::toString("MJRule",(int)nRuleTag)).Value());
	}
}

void SYMJGameScence::initPrivate()
{
	WidgetScenceXMLparse kScence("GameHNMJ/xml/HNGamePrivateScence.xml",this);

	WidgetManager::addButtonCB("Button_PrivateWeiXinFriend",this,button_selector(SYMJGameScence::Button_WeiXinFriend));
	WidgetManager::addButtonCB("HNMJButton_PrivateEndWeiXinShare",this,button_selector(SYMJGameScence::Button_WeiXinImagic));
	WidgetManager::addButtonCB("ButtonPlayerHeadClick",this,button_selector(SYMJGameScence::ButtonPlayerHeadClick));
	WidgetManager::addButtonCB("Button_PrivateReturnHome",this,button_selector(SYMJGameScence::Button_ExitGameBase));
}
void SYMJGameScence::defaultPrivateState()
{
	WidgetFun::setVisible(this,"PrivateDismissPlane",false);
}
void SYMJGameScence::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	m_kRoomInfo = *pNetInfo;
	WidgetFun::setVisible(this,"PrivateInfo",true);
	WidgetFun::setVisible(this,"GoldRoomInfo",false);
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
	WidgetFun::setVisible(this,"Button_PrivateWeiXinFriend",!pNetInfo->bStartGame);
	WidgetFun::setVisible(this,"Button_DismissPrivate",bShowDismiss);

	for (int i=0;i<MAX_PLAYER;i++)
	{
		if (m_pPlayer[i]->GetUserID() == pNetInfo->dwCreateUserID)
		{
			m_pPlayer[i]->showFangZhu(true);
		}
		else
		{
			m_pPlayer[i]->showFangZhu(false);
		}
	}

	if (HaveOptionRule(GAME_OPTION_RULE_SHOW_WIN_LOSE1))
	{
		for (int i = 0;i<MAX_PLAYER && i<(int)pNetInfo->kWinLoseScore.size();i++)
		{
			SYMJPlayer* pPlayer = getPlayerByChairID(i);
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
			SYMJPlayer* pPlayer = getPlayerByChairID(i);
			if (!pPlayer)
			{
				continue;
			}
			WidgetFun::setText(pPlayer->getPlayerNode(),"GoldTxt",(int)pNetInfo->kWinLoseScore[i]);
		}
	}
}
void SYMJGameScence::getGameRuleStr(std::vector<std::string>& kRuleList,dword	dwGameRuleIdex)
{
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_SY_FENG,		!HaveOptionRule(GAME_OPTION_RULE_HIDE_0));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_SY_CHI,		!HaveOptionRule(GAME_OPTION_RULE_HIDE_1));
}
void SYMJGameScence::setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
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
	if(!HaveOptionRule(GAME_OPTION_RULE_HIDE_GAMETYPE))
	{
		kRuleList.push_back(ScriptData<std::string>(utility::toString("HNGameType",(int)pNetInfo->bGameTypeIdex)).Value());
	}
	getGameRuleStr(kRuleList,pNetInfo->bGameRuleIdex);
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
void SYMJGameScence::setOutCardWaiteTime(int iOutCardWaiteTime)
{
	m_iOutCardWaiteTime = iOutCardWaiteTime;
}
void SYMJGameScence::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
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
		WidgetFun::setVisible(pNode,"FangZhu",pPlayer->GetUserID() == m_kRoomInfo.dwCreateUserID);
		ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(pNode,"Head"),pPlayer->GetUserInfo()->szHeadHttp,pPlayer->GetUserID());
		
		WidgetFun::setVisible(pNode,"DaYingJia",false);
		WidgetFun::setVisible(pNode,"ZuiJiaPaoShou",false);

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

	cocos2d::Node* pJieSuanNode = WidgetFun::getChildWidget(this,"GameJieSuanNode");
	if (pJieSuanNode->getActionByTag(1) == NULL 
		&& pJieSuanNode->isVisible() == false)
	{
		WidgetFun::setVisible(pRootNode,"PrivateEndPlane",true);
	}
	else
	{
		int iTime = utility::parseInt(WidgetFun::getWidgetUserInfo(pJieSuanNode,"Time"))+3;
		WidgetFun::getChildWidget(pRootNode,"PrivateEndPlane")->runAction(cocos2d::CCVisibleAction::create(iTime,true));
	}
}
void SYMJGameScence::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"PrivateDismissPlane");
	if (pNetInfo->dwDissUserCout == 0)
	{
		pNode->runAction(cocos2d::CCVisibleAction::create(3.0f,false));
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
void SYMJGameScence::Button_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*)
{
	if (!IsInGame())
	{
		return;
	}
	if(isHZ())
	{
	}
	else
	{
		std::string kHNWeiXinSharDes,kGameTypeStr;

		kGameTypeStr = ScriptData<std::string>(utility::toString("HNGameType",(int)m_kRoomInfo.bGameTypeIdex)).Value();

		kHNWeiXinSharDes = utility::getScriptReplaceValue("HNWeiXinSharDes",
			kGameTypeStr,(int)m_kRoomInfo.dwRoomNum,(int)m_kRoomInfo.dwPlayTotal);
		
		std::string kRuleTxt;
		std::vector<std::string> kRuleList;
		getGameRuleStr(kRuleList,m_kRoomInfo.bGameRuleIdex);
		for (int i = 0;i<(int)kRuleList.size();i++)
		{
			kRuleTxt += kRuleList[i];
			kRuleTxt += ",";
		}
		utility::StringReplace(kHNWeiXinSharDes,"#",kRuleTxt);

		MissionWeiXin::Instance().shareUrlWeiXin(utility::getScriptString("HNWeiXinSharUrl"),
			utility::getScriptString("HNWeiXinSharTitle"),
			kHNWeiXinSharDes,
			MissionWeiXin::SHARE_SESSION);
	}
}
void SYMJGameScence::Button_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*)
{
	static SYMJGameScence* pTemp = NULL;
	pTemp = this;
	cocos2d::utils::captureScreen([](bool b, std::string name){
		if (b)
		{
			MissionWeiXin::Instance().shareScreenWeiXin(name);
		}
	},"screenshot.png");

}
void SYMJGameScence::ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
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

void SYMJGameScence::setPlayCount(int nCount)
{
	m_nPlayCount = nCount;
}
int SYMJGameScence::getPlayCount()
{
	return m_nPlayCount;
}
