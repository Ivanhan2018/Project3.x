#include "HNPrivateScenceHN.h"
#include "HNScenceManager.h"
#include "GameLib.h"
#include "CServerItem.h"
#include "HNGameManager.h"
#include "CMD_HNMJ.h"
#include "HNMJGameScence.h"


#define GAME_RULE_NO_WAIT 1

FV_SINGLETON_STORAGE(HNPrivateScenceHN);

HNPrivateScenceHN::HNPrivateScenceHN()
	:m_iPlayerCoutIdex(0)
	,m_dwPlayRule(0)
	,m_iGameTypeIndex(GAME_TYPE_ZZ)
{
	init();
}
HNPrivateScenceHN::~HNPrivateScenceHN()
{

}
bool HNPrivateScenceHN::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};

	WidgetScenceXMLparse kScence1("Script/HNPrivateScenceHN.xml",this);

	WidgetManager::addButtonCB("Button_JoinRoom",this,button_selector(HNPrivateScenceHN::Button_JoinNumReset));
	WidgetManager::addButtonCB("Button_JoinNum",this,button_selector(HNPrivateScenceHN::Button_JoinNum));
	WidgetManager::addButtonCB("Button_JoinNumDel",this,button_selector(HNPrivateScenceHN::Button_JoinNumDel));
	WidgetManager::addButtonCB("Button_JoinNumReset",this,button_selector(HNPrivateScenceHN::Button_JoinNumReset));
	WidgetManager::addButtonCB("Button_Show_Join_Private",this,button_selector(HNPrivateScenceHN::Button_Show_Join_Private));

	WidgetManager::addButtonCB("Button_DismissPrivate",this,button_selector(HNPrivateScenceHN::Button_DismissPrivate));
	WidgetManager::addButtonCB("Button_DismissPrivateNot",this,button_selector(HNPrivateScenceHN::Button_DismissPrivateNot));
	WidgetManager::addButtonCB("Button_PrivateAgagin",this,button_selector(HNPrivateScenceHN::Button_PrivateAgagin));

	WidgetManager::addButtonCB("Button_HN_Show_Create_Private",this,button_selector(HNPrivateScenceHN::Button_Show_Create_Private));
	WidgetManager::addButtonCB("Button_ZZCreateRoom",this,button_selector(HNPrivateScenceHN::Button_ZZCreateRoom));
	WidgetManager::addButtonCB("Button_CSCreateRoom",this,button_selector(HNPrivateScenceHN::Button_CSCreateRoom));
	
	WidgetManager::addButtonCB("ButtonRule_ZhuanZhuanMj",this,button_selector(HNPrivateScenceHN::ButtonRule_ZhuanZhuanMj));
	WidgetManager::addButtonCB("ButtonRule_ChangShaMj",this,button_selector(HNPrivateScenceHN::ButtonRule_ChangShaMj));

	WidgetManager::addButtonCB("Button_HN_PrivatePlayCout1",this,button_selector(HNPrivateScenceHN::Button_PrivatePlayCout));
	WidgetManager::addButtonCB("Button_HN_PrivatePlayCout0",this,button_selector(HNPrivateScenceHN::Button_PrivatePlayCout));

	WidgetManager::addButtonCB("Button_GameRule0",this,button_selector(HNPrivateScenceHN::ButtonGameRule));
	WidgetManager::addButtonCB("Button_GameRule1",this,button_selector(HNPrivateScenceHN::ButtonGameRule));
	WidgetManager::addButtonCB("Button_GameRule2",this,button_selector(HNPrivateScenceHN::ButtonGameRule));
	WidgetManager::addButtonCB("Button_GameRule3",this,button_selector(HNPrivateScenceHN::ButtonGameRule));
	WidgetManager::addButtonCB("Button_GameRule4",this,button_selector(HNPrivateScenceHN::ButtonGameRule));
	WidgetManager::addButtonCB("Button_GameRule5",this,button_selector(HNPrivateScenceHN::ButtonGameRule));
	WidgetManager::addButtonCB("Button_GameRule6",this,button_selector(HNPrivateScenceHN::ButtonGameRule));
	WidgetManager::addButtonCB("Button_GameRule7",this,button_selector(HNPrivateScenceHN::ButtonGameRule));

	setPlayerCoutIdex(0);
	ButtonRule_ZhuanZhuanMj(NULL,NULL);
	WidgetFun::setEnable(this,"ButtonRule_ZhuanZhuanMj",false);
	WidgetFun::setEnable(this,"ButtonRule_ChangShaMj",true);
	WidgetFun::setVisible(this,"PlaneCreate_ZZ",true);
	WidgetFun::setVisible(this,"PlaneCreate_CS",false);
	return true;
}
void HNPrivateScenceHN::hideAll()
{
	WidgetFun::setVisible(this,"CreateRoomPlane",false);
	WidgetFun::setVisible(this,"JoinRoomPlane",false);
}
bool HNPrivateScenceHN::BackKey()
{
	if (WidgetFun::isWidgetVisble(this,"CreateRoomPlane"))
	{
		WidgetFun::setVisible(this,"CreateRoomPlane",false);
		return true;
	}
	return false;
}
void HNPrivateScenceHN::setPlayerCoutIdex(int iIdex)
{
	for (int i = 0;i<2;i++)
	{
		WidgetFun::setChecked(this,utility::toString("Button_HN_PrivatePlayCout",i),iIdex == i);
	}
	m_iPlayerCoutIdex = iIdex;
}
void HNPrivateScenceHN::Button_Show_Create_Private(cocos2d::Ref*,WidgetUserInfo*)
{
	WidgetFun::setVisible(this,"CreateRoomPlane",true);
	UserInfo::Instance().checkInGameServer();
}
void HNPrivateScenceHN::Button_Show_Join_Private(cocos2d::Ref*,WidgetUserInfo* pInfo)
{
	WidgetFun::setVisible(this,"JoinRoomPlane",true);
	Button_JoinNumReset(NULL,NULL);
	UserInfo::Instance().checkInGameServer();
}

void HNPrivateScenceHN::Button_ZZCreateRoom(cocos2d::Ref*,WidgetUserInfo*)
{
	createRoom(HNMJGameScence::KIND_ID);
}
void HNPrivateScenceHN::Button_CSCreateRoom(cocos2d::Ref*,WidgetUserInfo*)
{
	createRoom(HNMJGameScence::KIND_ID);
}
void HNPrivateScenceHN::Button_PrivateAgagin(cocos2d::Ref*,WidgetUserInfo*)
{
	CMD_GR_Again_Private kNetInfo;
	CServerItem::get()->SendSocketData(MDM_GR_PRIVATE,SUB_GR_RIVATE_AGAIN,&kNetInfo,sizeof(kNetInfo));
}
void HNPrivateScenceHN::Button_PrivatePlayCout(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"));
	setPlayerCoutIdex(iIdex);
}

void HNPrivateScenceHN::ButtonGameRule( cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int iIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"));
	setGameRuleIdex(iIdex);
}

void HNPrivateScenceHN::createRoom( int nKindID )
{
	CMD_GR_Create_Private m_kSendNet;
	zeromemory(&m_kSendNet,sizeof(m_kSendNet));
	m_kSendNet.bGameRuleIdex = m_dwPlayRule;
	m_kSendNet.cbGameType = Type_Private;
	m_kSendNet.bGameTypeIdex = m_iGameTypeIndex;
	m_kSendNet.bPlayCoutIdex = m_iPlayerCoutIdex;
	HNPrivateScenceBase::InstanceBase().ConnectAndCreatePrivateByKindID(nKindID,m_kSendNet); 
}

void HNPrivateScenceHN::setGameRuleIdex(int iIdex)
{
	if (iIdex == GAME_TYPE_ZZ_QIANGGANGHU 
		||iIdex == GAME_TYPE_ZZ_ZIMOHU
		/*||iIdex == GAME_TYPE_ZZ_258*/)
	{
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_QIANGGANGHU));
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZIMOHU));
		//FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_258));
	}
	if (iIdex == GAME_TYPE_ZZ_ZHANIAO2)
	{
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZHANIAO4));
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZHANIAO6));
	}
	if (iIdex == GAME_TYPE_ZZ_ZHANIAO4)
	{
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZHANIAO2));
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZHANIAO6));
	}
	if (iIdex == GAME_TYPE_ZZ_ZHANIAO6)
	{
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZHANIAO4));
		FvMask::Del(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZHANIAO2));
	}
	if (FvMask::HasAny(m_dwPlayRule,_MASK_((dword)iIdex)))
	{
		FvMask::Del(m_dwPlayRule,_MASK_((dword)iIdex));
	}
	else
	{
		FvMask::Add(m_dwPlayRule,_MASK_((dword)iIdex));
	}
	updateGameRule();
}
void HNPrivateScenceHN::updateGameRule()
{
	WidgetFun::setChecked(this,"Button_GameRule0",FvMask::HasAny(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_QIANGGANGHU)));
	WidgetFun::setChecked(this,"Button_GameRule1",FvMask::HasAny(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZIMOHU)));
	WidgetFun::setChecked(this,"Button_GameRule2",FvMask::HasAny(m_dwPlayRule,_MASK_((dword)GAME_TYPE_CS_ZHUANGXIANFEN)));
	WidgetFun::setChecked(this,"Button_GameRule3",FvMask::HasAny(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_QIDUI)));
	WidgetFun::setChecked(this,"Button_GameRule4",FvMask::HasAny(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_HONGZHONG)));
	WidgetFun::setChecked(this,"Button_GameRule5",FvMask::HasAny(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZHANIAO2)));
	WidgetFun::setChecked(this,"Button_GameRule6",FvMask::HasAny(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZHANIAO4)));
	WidgetFun::setChecked(this,"Button_GameRule7",FvMask::HasAny(m_dwPlayRule,_MASK_((dword)GAME_TYPE_ZZ_ZHANIAO6)));
}

void HNPrivateScenceHN::ButtonRule_ZhuanZhuanMj(cocos2d::Ref*,WidgetUserInfo*)
{
	m_iGameTypeIndex = GAME_TYPE_ZZ;
	m_dwPlayRule = 0;
	setGameRuleIdex(GAME_TYPE_ZZ_QIANGGANGHU);
}
void HNPrivateScenceHN::ButtonRule_ChangShaMj(cocos2d::Ref*,WidgetUserInfo*)
{
	m_iGameTypeIndex = GAME_TYPE_CS;
	m_dwPlayRule = 0;
	updateGameRule();
}