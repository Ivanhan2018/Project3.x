#include "HNPrivateScenceBase.h"
#include "GameLib.h"
#include "CServerItem.h"

FV_SINGLETON_STORAGEBASE(HNPrivateScenceBase);

HNPrivateScenceBase::HNPrivateScenceBase()
	:m_iJoinCout(0)
	,m_eLinkAction(Type_Link_NULL)
{
	zeromemory(&m_kCreatePrivateNet,sizeof(m_kCreatePrivateNet));
	zeromemory(&m_kPrivateInfo,sizeof(m_kPrivateInfo));
	memset(m_kJoinNumTxt,0,sizeof(m_kJoinNumTxt));
	CServerItem::get()->SetServerPrivateSink(this);
}
HNPrivateScenceBase::~HNPrivateScenceBase()
{

}
void HNPrivateScenceBase::OnSocketSubPrivateInfo(CMD_GR_Private_Info* pNetInfo)
{
	m_kPrivateInfo = *pNetInfo;

	if (m_eLinkAction == Type_Link_Create)
	{
		CServerItem::get()->SendSocketData(MDM_GR_PRIVATE,SUB_GR_CREATE_PRIVATE,&m_kCreatePrivateNet,sizeof(m_kCreatePrivateNet));
		zeromemory(&m_kCreatePrivateNet,sizeof(m_kCreatePrivateNet));
	}
	if (m_eLinkAction == Type_Link_Join)
	{
		CMD_GR_Join_Private kSendNet;
		kSendNet.dwRoomNum = utility::parseInt(m_kJoinNumTxt);
		CServerItem::get()->SendSocketData(MDM_GR_PRIVATE,SUB_GR_JOIN_PRIVATE,&kSendNet,sizeof(kSendNet));
	}
	m_eLinkAction = Type_Link_NULL;
	UserInfo::Instance().setPrivateInGameServerID(GameManagerBase::InstanceBase().getActServerID());
}
void HNPrivateScenceBase::upJoinNumTxt()
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"JoinRoomPlane");
	for (int i = 0;i<m_iJoinCout;i++)
	{
		WidgetFun::setText(pNode,utility::toString("JoinTxt",i),utility::toString(m_kJoinNumTxt[i]));
	}
	for (int i = m_iJoinCout;i<6;i++)
	{
		WidgetFun::setText(pNode,utility::toString("JoinTxt",i),"");
	}
}
void HNPrivateScenceBase::OnSocketSubPrivateCreateSuceess(CMD_GR_Create_Private_Sucess* pNetInfo)
{
	//NoticeMsg::Instance().ShowTopMsg(utility::toString((int)pNetInfo->dwRoomNum));
}
void HNPrivateScenceBase::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	m_kPrivateRoomInfo = *pNetInfo;

}
void HNPrivateScenceBase::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
{
	UserInfo::Instance().setPrivateInGameServerID(-1);
}
void HNPrivateScenceBase::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)
{

}
void HNPrivateScenceBase::Button_SureeDismissPrivate(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_GR_Dismiss_Private kNetInfo;
	kNetInfo.bDismiss = 1;
	CServerItem::get()->SendSocketData(MDM_GR_PRIVATE,SUB_GR_PRIVATE_DISMISS,&kNetInfo,sizeof(kNetInfo));
}
void HNPrivateScenceBase::Button_DismissPrivate(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	if (m_kPrivateRoomInfo.dwPlayCout == 0 && WidgetFun::getUserInfoValue(pUserInfo,"Pop") != "1")
	{
		PopScence::Instance().showAccessPlane(utility::getScriptString("PrivateTxt0"),this,
			button_selector(HNPrivateScenceBase::Button_SureeDismissPrivate),NULL);
		return;
	}
	PopScence::Instance().showAccessPlane(utility::getScriptString("SureDismissPrivate"),this,
		button_selector(HNPrivateScenceBase::Button_SureeDismissPrivate),NULL);
}
void HNPrivateScenceBase::Button_DismissPrivateNot(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_GR_Dismiss_Private kNetInfo;
	kNetInfo.bDismiss = 0;
	CServerItem::get()->SendSocketData(MDM_GR_PRIVATE,SUB_GR_PRIVATE_DISMISS,&kNetInfo,sizeof(kNetInfo));
}
void HNPrivateScenceBase::Button_JoinNum(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	std::string kStr = (WidgetFun::getUserInfoValue(pUserInfo,"Idex"));
	if (!kStr.size())
	{
		CCAssert(false,"");
		return;
	}
	if (m_iJoinCout >=6)
	{
		return;
	}
	m_kJoinNumTxt[m_iJoinCout] = kStr[0];
	m_kJoinNumTxt[m_iJoinCout+1] = 0;
	m_iJoinCout++;
	upJoinNumTxt();

	if(m_iJoinCout == 6)
	{
		m_kActJoinNum = utility::parseInt(m_kJoinNumTxt);
		int iServerID = m_kActJoinNum/10000-10;
		CGameServerItem* pGameServer = GameManagerBase::InstanceBase().SearchGameServer(iServerID);
		if (pGameServer && pGameServer->IsPrivateRoom())
		{
			GameManagerBase::InstanceBase().connectGameServerByServerID(iServerID);
			m_eLinkAction = Type_Link_Join;
		}
		else
		{
			NoticeMsg::Instance().ShowTopMsgByScript("JoinRoomNumError");
		}
	}
}
void HNPrivateScenceBase::Button_JoinNumDel(cocos2d::Ref*,WidgetUserInfo*)
{
	m_iJoinCout--;
	if (m_iJoinCout < 0)
	{
		m_iJoinCout = 0;
	}
	upJoinNumTxt();
}
void HNPrivateScenceBase::Button_JoinNumReset(cocos2d::Ref*,WidgetUserInfo*)
{
	m_iJoinCout = 0;
	upJoinNumTxt();
}
void HNPrivateScenceBase::ConnectAndCreatePrivateByKindID(int iKindID,const CMD_GR_Create_Private& kNet)
{
	GameManagerBase::InstanceBase().connectGameServerByKindID(iKindID,GAME_GENRE_EDUCATE);
	memcpy(&m_kCreatePrivateNet,&kNet,sizeof(kNet));
	m_eLinkAction = Type_Link_Create;
}
void HNPrivateScenceBase::ConnectAndCreatePrivateByServerID(int iServerID,const CMD_GR_Create_Private& kNet)
{
	GameManagerBase::InstanceBase().connectGameServerByServerID(iServerID);
	memcpy(&m_kCreatePrivateNet,&kNet,sizeof(kNet));
	m_eLinkAction = Type_Link_Create;
}