#include "NNGameScence.h"
#include "GameLib.h"
#include "NNPlayer.h"
#include "MissionWeiXin.h"


void NNGameScence::initPrivate()
{
	WidgetScenceXMLparse kScence("GameNN/xml/GamePrivateScence.xml",this);

	WidgetManager::addButtonCB("NNButton_WeiXinFriend",this,button_selector(NNGameScence::XZDDButton_WeiXinFriend));
	WidgetManager::addButtonCB("NNButton_PrivateWeiXinShare",this,button_selector(NNGameScence::XZDDButton_WeiXinImagic));
	WidgetManager::addButtonCB("ButtonPlayerHeadClick",this,button_selector(NNGameScence::ButtonPlayerHeadClick));
}
void NNGameScence::defaultPrivateState()
{
	WidgetFun::setVisible(this,"PrivateDismissPlane",false);
}
void NNGameScence::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	WidgetFun::setVisible(this,"PrivateInfo",true);
	setPrivateInfo(pNetInfo);
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
	WidgetFun::setVisible(this,"NNButton_WeiXinFriend",!pNetInfo->bStartGame 
		&& pNetInfo->dwCreateUserID == m_pLocal->GetUserID());
	WidgetFun::setVisible(this,"Button_DismissPrivate",bShowDismiss);

	m_kRoomInfo = *pNetInfo;
}

void NNGameScence::setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo)
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
	if(FvMask::HasAny(pNetInfo->bGameRuleIdex,_MASK_(GAME_RULE_SUIJIZHUANG)))
	{
		WidgetFun::setText(this,"RoomGameType",utility::getScriptString("NNGameType0"));
	}

	if(FvMask::HasAny(pNetInfo->bGameRuleIdex,_MASK_(GAME_RULE_QIANGZHUANG)))
	{
		WidgetFun::setText(this,"RoomGameType",utility::getScriptString("NNGameType1"));
	}
	if (HaveOptionRule(GAME_OPTION_RULE_SHOW_WIN_LOSE1))
	{
		for (int i = 0;i<MAX_PLAYER && i<(int)pNetInfo->kWinLoseScore.size();i++)
		{
			NNPlayer* pPlayer = getPlayerByChairID(i);
			if (!pPlayer)
			{
				continue;
			}
			WidgetFun::setText(pPlayer->getPlayerNode(),"GoldTxt",100000+(int)pNetInfo->kWinLoseScore[i]);
		}
	}
}
void NNGameScence::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
{
	defaultPrivateState();
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"PrivateEndPlane"); 
	if (pNetInfo->lPlayerWinLose.size() != MAX_PLAYER
		||pNetInfo->lPlayerAction.size() != MAX_PLAYER*MAX_PRIVATE_ACTION)
	{
		CCAssert(false,"pNetInfo->lPlayerWinLose.size() != MAX_PLAYER");
		return;
	}
	for (int i=0;i<MAX_PLAYER;i++)
	{
		WidgetFun::setVisible(pRootNode,utility::toString(utility::toString("PrivateEndPlayer",i)),false);
	}
	int n =0;
	for (int i  = 0;i<MAX_PLAYER;i++)
	{
		GamePlayer* pPlayer = m_pPlayer[i];
		if (!pPlayer || pPlayer->GetUserInfo() == NULL)
		{
			continue;
		}
		int iChairID = pPlayer->GetChairID();
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pRootNode,utility::toString(utility::toString("PrivateEndPlayer",n)));
		pNode->setVisible(true);
		WidgetFun::setText(pNode,"Name",pPlayer->GetNickName());
		WidgetFun::setText(pNode,"ID",(int)pPlayer->GetUserID());
		WidgetFun::setText(pNode,"TotalScore",utility::toString((int)pNetInfo->lPlayerWinLose[iChairID]));
		ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(pNode,"Head"),pPlayer->GetUserInfo()->szHeadHttp,pPlayer->GetUserID());
		n++;
	}
	WidgetFun::setVisible(pRootNode,"PrivateEndPlane",true);
}
void NNGameScence::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"PrivateDismissPlane");
	pNode->setVisible(true);
	if (pNetInfo->dwDissUserCout == 0)
	{
		pNode->runAction(cocos2d::CCVisibleAction::create(3.0f,false));
		return;
	}
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
void NNGameScence::XZDDButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*)
{
	if (!IsInGame())
	{
		return;
	}
	std::string kShowTxt = utility::getScriptReplaceValue("NNPrivateTxt1",WidgetFun::getText(this,"RoomID"));

	if(FvMask::HasAny(m_kRoomInfo.bGameRuleIdex,_MASK_(GAME_RULE_SUIJIZHUANG)))
	{
		utility::StringReplace(kShowTxt,"#",utility::getScriptString("NNGameType0"));
	}

	if(FvMask::HasAny(m_kRoomInfo.bGameRuleIdex,_MASK_(GAME_RULE_QIANGZHUANG)))
	{
		utility::StringReplace(kShowTxt,"#",utility::getScriptString("NNGameType1"));
	}
	utility::StringReplace(kShowTxt,"#",utility::toString((int)m_kRoomInfo.dwPlayTotal));
	MissionWeiXin::Instance().shareUrlWeiXin(utility::getScriptString("NNWeiXinSharUrl"),
		utility::getScriptString("NNWeiXinSharTitle"),
		kShowTxt,
		MissionWeiXin::SHARE_SESSION);
}
void NNGameScence::XZDDButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*)
{
	static NNGameScence* pTemp = NULL;
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
void NNGameScence::ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
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

