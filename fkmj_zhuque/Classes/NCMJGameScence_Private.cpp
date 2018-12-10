#include "NCMJGameScence.h"
#include "GameLib.h"
#include "NCMJPlayer.h"
#include "MissionWeiXin.h"

#define NCPushGameRule(GameRule,RuleTag)\
	if (FvMask::HasAny(GameRule,_MASK_((dword)RuleTag)))\
	{\
	kRuleList.push_back(ScriptData<std::string>(utility::toString("NCRule",(int)RuleTag)).Value());\
	}\

void NCMJGameScence::initPrivate()
{
	WidgetScenceXMLparse kScence("GameNCMJ/xml/GamePrivateScence.xml",this);

	WidgetManager::addButtonCB("NCMJButton_WeiXinFriend",this,button_selector(NCMJGameScence::NCMJButton_WeiXinFriend));
	WidgetManager::addButtonCB("NCMJButton_PrivateEndWeiXinShare",this,button_selector(NCMJGameScence::NCMJButton_WeiXinImagic));
	WidgetManager::addButtonCB("ButtonPlayerHeadClick",this,button_selector(NCMJGameScence::ButtonPlayerHeadClick));
}
void NCMJGameScence::defaultPrivateState()
{
	WidgetFun::setVisible(this,"PrivateDismissPlane",false);
}
void NCMJGameScence::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	WidgetFun::setVisible(this,"PrivateInfo",true);
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
	WidgetFun::setVisible(this,"NCMJButton_WeiXinFriend",!pNetInfo->bStartGame 
		&& pNetInfo->dwCreateUserID == m_pLocal->GetUserID());
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

	for (int i = 0;i<MAX_PLAYER && i<(int)pNetInfo->kWinLoseScore.size();i++)
	{
		NCMJPlayer* pPlayer = getPlayerByChairID(i);
		if (!pPlayer)
		{
			continue;
		}
		WidgetFun::setText(pPlayer->getPlayerNode(),"GoldTxt",(int)pNetInfo->kWinLoseScore[i]);
	}
}

void NCMJGameScence::setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo)
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
	WidgetFun::setText(this,"RoomGameType",ScriptData<std::string>(utility::toString("NCGameType",(int)pNetInfo->bGameTypeIdex)).Value() );

	std::vector<std::string> kRuleList;
	NCPushGameRule(pNetInfo->bGameRuleIdex,GAME_TYPE_ZZ_ZHANIAO2);
	NCPushGameRule(pNetInfo->bGameRuleIdex,GAME_TYPE_ZZ_ZHANIAO4);
	NCPushGameRule(pNetInfo->bGameRuleIdex,GAME_TYPE_ZZ_ZHANIAO6);
	NCPushGameRule(pNetInfo->bGameRuleIdex,GAME_TYPE_ZZ_2612);
	NCPushGameRule(pNetInfo->bGameRuleIdex,GAME_TYPE_ZZ_136);

	for (int i=0;i<5;i++)
	{
		WidgetFun::setVisible(this,utility::toString("RoomGameRule",i),false);
	}
	for (int i=0;i<(int)kRuleList.size();i++)
	{
		WidgetFun::setVisible(this,utility::toString("RoomGameRule",i),true);
		WidgetFun::setText(this,utility::toString("RoomGameRule",i),kRuleList[i]);
	}
}
void NCMJGameScence::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
{
	defaultPrivateState();
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"PrivateEndPlane"); 
	if (pNetInfo->lPlayerWinLose.size() != MAX_PLAYER
		||pNetInfo->lPlayerAction.size() != MAX_PLAYER*MAX_PRIVATE_ACTION)
	{
		CCAssert(false,"pNetInfo->lPlayerWinLose.size() != MAX_PLAYER");
		return;
	}
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
		WidgetFun::setVisible(pNode,"FangZhu",pPlayer->GetUserID() == m_kPrivateRoomInfo.dwCreateUserID);
		WidgetFun::setImagic(pNode,"Head","Head/Head0.png");
		ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(pNode,"Head"),pPlayer->GetUserInfo()->szHeadHttp,pPlayer->GetUserID());
	}
	int iTime = 12;
	cocos2d::Action* pAction = cocos2d::CCVisibleAction::create(iTime,true);
	pAction->setTag(1);
	pRootNode->runAction(pAction);
}
void NCMJGameScence::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)
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
void NCMJGameScence::NCMJButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*)
{
	if (!IsInGame())
	{
		return;
	}
	std::string kStr = "";
	kStr = ScriptData<std::string>(utility::toString("NCRule",7)).Value();
	if (FvMask::HasAny(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GAME_TYPE_ZZ_ZHANIAO2)))
	{
		kStr = ScriptData<std::string>(utility::toString("NCRule",(int)GAME_TYPE_ZZ_ZHANIAO2)).Value();
	}
	if (FvMask::HasAny(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GAME_TYPE_ZZ_ZHANIAO4)))
	{
		kStr = ScriptData<std::string>(utility::toString("NCRule",(int)GAME_TYPE_ZZ_ZHANIAO4)).Value();
	}
	if (FvMask::HasAny(m_kPrivateRoomInfo.bGameRuleIdex,_MASK_(GAME_TYPE_ZZ_ZHANIAO6)))
	{
		kStr = ScriptData<std::string>(utility::toString("NCRule",(int)GAME_TYPE_ZZ_ZHANIAO6)).Value();
	}
	std::string kTitle = ScriptData<std::string>(utility::toString("NCGameType",(int)m_kPrivateRoomInfo.bGameTypeIdex)).Value();
	std::string kRoomID = WidgetFun::getText(this,"RoomID");
	MissionWeiXin::Instance().shareUrlWeiXin(utility::getScriptString("WeiXinSharUrl"),
		utility::toString(kTitle,"-",kRoomID),
		utility::getScriptReplaceValue("PrivateTxt1",kRoomID,kStr),
		MissionWeiXin::SHARE_SESSION);
}
void NCMJGameScence::NCMJButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*)
{
	static NCMJGameScence* pTemp = NULL;
	pTemp = this;
	std::string kStr = utility::toString(time(NULL),".png");
	cocos2d::utils::captureScreen([](bool b, std::string name){
		if (b)
		{
			MissionWeiXin::Instance().shareScreenWeiXin(name);
			pTemp->Button_ExitGameBase(NULL,NULL);
			pTemp->ExitGame();
			WidgetFun::setVisible(pTemp,"PrivateEndPlane",false); 
		}
	},kStr);

}
void NCMJGameScence::ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	if (!IsInGame())
	{
		return;
	}
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

void NCMJGameScence::setPlayCount(int nCount)
{
	m_nPlayCount = nCount;
}
int NCMJGameScence::getPlayCount()
{
	return m_nPlayCount;
}
