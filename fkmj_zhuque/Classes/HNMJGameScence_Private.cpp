#include "HNMJGameScence.h"
#include "GameLib.h"
#include "HNMJPlayer.h"
#include "MissionWeiXin.h"

void HNMJGameScence::pushGameRule(std::vector<std::string>&kRuleList, dword dwGameRule,int nRuleTag,bool bShow)
{
	if (FvMask::HasAny(dwGameRule,_MASK_((dword)nRuleTag))&&bShow)
	{
		kRuleList.push_back(ScriptData<std::string>(utility::toString("MJRule",(int)nRuleTag)).Value());
	}
}

void HNMJGameScence::initPrivate()
{
	WidgetScenceXMLparse kScence("GameHNMJ/xml/HNGamePrivateScence.xml",this);

	WidgetManager::addButtonCB("Button_PrivateWeiXinFriend",this,button_selector(HNMJGameScence::Button_WeiXinFriend));
	WidgetManager::addButtonCB("HNMJButton_PrivateEndWeiXinShare",this,button_selector(HNMJGameScence::Button_WeiXinImagic));
	WidgetManager::addButtonCB("ButtonPlayerHeadClick",this,button_selector(HNMJGameScence::ButtonPlayerHeadClick));
	WidgetManager::addButtonCB("Button_PrivateReturnHome",this,button_selector(HNMJGameScence::Button_ExitGameBase));
}
void HNMJGameScence::defaultPrivateState()
{
	WidgetFun::setVisible(this,"PrivateDismissPlane",false);
}
void HNMJGameScence::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)
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
	if (HaveOptionRule(GAME_OPTION_RULE_ONLY_CREATE))
	{
		WidgetFun::setVisible(this,"Button_PrivateWeiXinFriend",!pNetInfo->bStartGame 
			&& pNetInfo->dwCreateUserID == m_pLocal->GetUserID());
	}
	else
	{
		WidgetFun::setVisible(this,"Button_PrivateWeiXinFriend",!pNetInfo->bStartGame);
	}
	if (WidgetFun::getChildWidget(this,"SetExitPlane"))
	{
		WidgetFun::setVisible(this,"SetExitPlane:Button_DismissPrivate",bShowDismiss);
	}
	if (WidgetFun::getChildWidget(this,"HomeDismissPrivatePlane"))
	{
		WidgetFun::setVisible(this,"HomeDismissPrivatePlane:Button_DismissPrivate",bShowDismiss);
	}
	if (WidgetFun::getChildWidget(this,"HomeOtherPrivatePlane"))
	{
		WidgetFun::setVisible(this,"HomeOtherPrivatePlane:Button_DismissPrivate",bShowDismiss);
	}

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
			HNMJPlayer* pPlayer = getPlayerByChairID(i);
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
			HNMJPlayer* pPlayer = getPlayerByChairID(i);
			if (!pPlayer)
			{
				continue;
			}
			WidgetFun::setText(pPlayer->getPlayerNode(),"GoldTxt",(int)pNetInfo->kWinLoseScore[i]);
		}
	}
}
void HNMJGameScence::getGameRuleStr(std::vector<std::string>& kRuleList,dword	dwGameRuleIdex)
{
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_258,		!HaveOptionRule(GAME_OPTION_RULE_HIDE_OTHER));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_ZIMOHU,		!HaveOptionRule(GAME_OPTION_RULE_HIDE_OTHER));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_QIDUI,		!HaveOptionRule(GAME_OPTION_RULE_HIDE_OTHER));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_QIANGGANGHU,!HaveOptionRule(GAME_OPTION_RULE_HIDE_OTHER));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_ZHANIAO2,	!HaveOptionRule(GAME_OPTION_RULE_HIDE_OTHER));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_ZHANIAO4,	!HaveOptionRule(GAME_OPTION_RULE_HIDE_OTHER));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_ZHANIAO6,	!HaveOptionRule(GAME_OPTION_RULE_HIDE_OTHER));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_HONGZHONG,	!HaveOptionRule(GAME_OPTION_RULE_HIDE_OTHER));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_CS_ZHUANGXIANFEN,!HaveOptionRule(GAME_OPTION_RULE_HIDE_OTHER));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_ZHANIAO3,	!HaveOptionRule(GAME_OPTION_RULE_HIDE_OTHER));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_ZHANIAO5,	!HaveOptionRule(GAME_OPTION_RULE_HIDE_OTHER));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_TYPE_ZZ_ZHANIAO1,	!HaveOptionRule(GAME_OPTION_RULE_HIDE_OTHER));
}
void HNMJGameScence::upDateTime()
{
	time_t t = time(0); 
	char tmp[64]; 
	strftime( tmp, sizeof(tmp), "%H:%M",localtime(&t) ); 
	WidgetFun::setText(this,"RoomGameRule0",tmp);
}
void HNMJGameScence::setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	WidgetFun::setText(this,"RoomID",utility::toString(pNetInfo->dwRoomNum));
	int nJushu = pNetInfo->dwPlayCout;
	if (pNetInfo->bStartGame == 0 && nJushu<pNetInfo->dwPlayTotal)
	{
		nJushu++;
	}
	WidgetFun::setPlaceTextKey(this,"RoomNum","Txt",utility::toString(nJushu,"/",pNetInfo->dwPlayTotal));

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
	int iIdex = 0;
	if (HaveOptionRule(GAME_OPTION_RULE_SHOW_TIME))
	{
		upDateTime();
		TimeManager::Instance().removeByFun(TIME_CALLBACK(HNMJGameScence::upDateTime,this));
		TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(HNMJGameScence::upDateTime,this),10,true);
		WidgetFun::setVisible(this,"RoomGameRule0",true);
		iIdex = 1;
	}
	for (;iIdex<(int)kRuleList.size() && iIdex < MAX_GAME_RULE;iIdex++)
	{
		WidgetFun::setVisible(this,utility::toString("RoomGameRule",iIdex),true);
		WidgetFun::setText(this,utility::toString("RoomGameRule",iIdex),kRuleList[iIdex]);
	}
	if(HaveOptionRule(GAME_OPTION_RULE_SHOW_EXIT_AND_SAVE) && WidgetFun::getChildWidget(this,"HNMJButton_CB_SaveAndExit"))
	{
		WidgetFun::setVisible(this,"HNMJButton_CB_SaveAndExit",false);
		if (pNetInfo->bStartGame == 0 && pNetInfo->dwPlayCout == 0 
			&& pNetInfo->dwCreateUserID == UserInfo::Instance().getUserID())
		{
			WidgetFun::setVisible(this,"HNMJButton_CB_SaveAndExit",true);
		}
	}
}
void HNMJGameScence::setOutCardWaiteTime(int iOutCardWaiteTime)
{
	m_iOutCardWaiteTime = iOutCardWaiteTime;
}
void HNMJGameScence::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
{
	defaultPrivateState();
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"PrivateEndPlane"); 
	if (pNetInfo->lPlayerWinLose.size() != MAX_PLAYER
		||pNetInfo->lPlayerAction.size() != MAX_PLAYER*MAX_PRIVATE_ACTION)
	{
		CCAssert(false,"pNetInfo->lPlayerWinLose.size() != MAX_PLAYER");
		return;
	}
	int nJushu = m_kRoomInfo.dwPlayCout;
	if (m_kRoomInfo.bStartGame == 0 && nJushu<m_kRoomInfo.dwPlayTotal)
	{
		nJushu++;
	}
	if (WidgetFun::getChildWidget(pRootNode,"PrivateEndExInfo"))
	{
		WidgetFun::setVisible(pRootNode,"PrivateEndExInfo",HaveOptionRule(GAME_OPTION_SHOW_PRIVATE_END_INFO));
	}
	if (WidgetFun::getChildWidget(pRootNode,"TimeTxt"))
	{
		WidgetFun::setText(pRootNode,"TimeTxt",pNetInfo->kPlayTime.toString2());
	}
	if (WidgetFun::getChildWidget(pRootNode,"NumTxt"))
	{
		WidgetFun::setText(pRootNode,"NumTxt",(int)m_kRoomInfo.dwRoomNum);
	}
	if (WidgetFun::getChildWidget(pRootNode,"JuShuTxt"))
	{
		WidgetFun::setText(pRootNode,"JuShuTxt",utility::toString(nJushu,"/",m_kRoomInfo.dwPlayTotal));
	}
	if (WidgetFun::getChildWidget(pRootNode,"CreaterIDTxt"))
	{
		WidgetFun::setText(pRootNode,"CreaterIDTxt",utility::paseInt(m_kRoomInfo.dwCreateUserID,6));
	}
	int nMaxPao = 0;
	int nMaxWin = 0;
	int nMaxPaoID = -1;
	int nMaxWinID = -1;
	int iStartIdex = 0;
	for (int i = 0;i< MAX_PLAYER;i++)
	{
		if (m_pPlayer[i]->GetUserID() == m_kRoomInfo.dwCreateUserID)
		{
			iStartIdex = i;
		}
	}
	for (int i  = 0;i<MAX_PLAYER;i++)
	{
		GamePlayer* pPlayer = m_pPlayer[(iStartIdex+i)%MAX_PLAYER];
		if (!pPlayer)
		{
			continue;
		}
		int iChairID = pPlayer->GetChairID();
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pRootNode,utility::toString(utility::toString("PrivateEndPlayer",i)));
		WidgetFun::setText(pNode,"Name",pPlayer->GetNickName());
		WidgetFun::setText(pNode,"ID",(int)pPlayer->GetUserID());
		WidgetFun::setText(pNode,"ActionTxt0",utility::toString((int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+0]));
		WidgetFun::setText(pNode,"ActionTxt1",utility::toString((int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+1]));
		WidgetFun::setText(pNode,"ActionTxt2",utility::toString((int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+2]));
		WidgetFun::setText(pNode,"ActionTxt3",utility::toString((int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+3]));
		WidgetFun::setText(pNode,"ActionTxt4",utility::toString((int)pNetInfo->lPlayerAction[iChairID*MAX_PRIVATE_ACTION+4]));
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
void HNMJGameScence::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)
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
void HNMJGameScence::Button_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*)
{
	if (!IsInGame())
	{
		return;
	}
	onWeiXinShareFriend();
}
void HNMJGameScence::onWeiXinShareFriend()
{
	if(isHZ())
	{
		int iCout = 0;
		if (FvMask::HasAny(m_kRoomInfo.bGameRuleIdex,_MASK_(GAME_TYPE_ZZ_ZHANIAO2)))
		{
			iCout = 2;
		}
		if (FvMask::HasAny(m_kRoomInfo.bGameRuleIdex,_MASK_(GAME_TYPE_ZZ_ZHANIAO3)))
		{
			iCout = 3;
		}
		if (FvMask::HasAny(m_kRoomInfo.bGameRuleIdex,_MASK_(GAME_TYPE_ZZ_ZHANIAO4)))
		{
			iCout = 4;
		}
		if (FvMask::HasAny(m_kRoomInfo.bGameRuleIdex,_MASK_(GAME_TYPE_ZZ_ZHANIAO5)))
		{
			iCout = 5;
		}
		if (FvMask::HasAny(m_kRoomInfo.bGameRuleIdex,_MASK_(GAME_TYPE_ZZ_ZHANIAO6)))
		{
			iCout = 6;
		}
		std::string kQu = utility::getScriptString("HZWeiXinSharTitleQu");

		std::string kTitle = utility::getScriptReplaceValue("HZWeiXinSharTitle",
			WidgetFun::getText(this,"RoomID"),kQu);

		std::string kDes =  utility::getScriptReplaceValue("HZWeiXinSharDes",
			kQu,(int)m_kRoomInfo.dwPlayTotal,iCout);

		MissionWeiXin::Instance().shareUrlWeiXin(utility::getScriptString("HNWeiXinSharUrl"),
			kTitle,
			kDes,
			MissionWeiXin::SHARE_SESSION);
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

		std::string kTitle = utility::getScriptString("HNWeiXinSharTitle");
		kTitle = utility::toString(kTitle,"-",WidgetFun::getText(this,"RoomID"));
		MissionWeiXin::Instance().shareUrlWeiXin(utility::getScriptString("HNWeiXinSharUrl"),
			kTitle,
			kHNWeiXinSharDes,
			MissionWeiXin::SHARE_SESSION);
	}
}
void HNMJGameScence::Button_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*)
{
	static HNMJGameScence* pTemp = NULL;
	pTemp = this;
	cocos2d::utils::captureScreen([](bool b, std::string name){
		if (b)
		{
			MissionWeiXin::Instance().shareScreenWeiXin(name);
		}
	},"screenshot.png");

}
void HNMJGameScence::ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	GamePlayer* pPlayer = getBasePlayerByUserID(utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"UserID")));
	if (!pPlayer)
	{
		return;
	}

	int nUserID = pPlayer->GetUserID();
	if (HaveOptionRule(GAME_OPTION_RULE_USERID_ADD))
	{
		nUserID += 300000;
	}
	PopScence::Instance().showIPInfo(pPlayer->GetNickName(),
		utility::paseInt(nUserID,6),
		pPlayer->GetUserInfo()->szLogonIP,
		pPlayer->GetUserInfo()->szHeadHttp);
}

void HNMJGameScence::setPlayCount(int nCount)
{
	m_nPlayCount = nCount;
}
int HNMJGameScence::getPlayCount()
{
	return m_nPlayCount;
}
