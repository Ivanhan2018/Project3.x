#pragma once
#include "cocos2d.h"
#include "FvSingleton.h"
#include "WidgetDefine.h"
#include "IServerPrivateSink.h"

class HNPrivateScenceBase
	:public cocos2d::Node
	,public IServerPrivateSink
	,public FvSingletonBase<HNPrivateScenceBase>
{
public:
	enum Type_LinkAction
	{
		Type_Link_NULL,
		Type_Link_Create,
		Type_Link_Join,
	};
public:
	HNPrivateScenceBase();
	~HNPrivateScenceBase();
public:
	void upJoinNumTxt();
public:
	virtual void OnSocketSubPrivateInfo(CMD_GR_Private_Info* pNetInfo);
	virtual void OnSocketSubPrivateCreateSuceess(CMD_GR_Create_Private_Sucess* pNetInfo);
	virtual void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	virtual void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo);
	virtual void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);
public:
	void Button_JoinNum(cocos2d::Ref*,WidgetUserInfo*);
	void Button_JoinNumDel(cocos2d::Ref*,WidgetUserInfo*);
	void Button_JoinNumReset(cocos2d::Ref*,WidgetUserInfo*);
	
	void Button_SureeDismissPrivate(cocos2d::Ref*,WidgetUserInfo*);
	void Button_DismissPrivate(cocos2d::Ref*,WidgetUserInfo*);
	void Button_DismissPrivateNot(cocos2d::Ref*,WidgetUserInfo*);
	
	void ConnectAndCreatePrivateByKindID(int iKindID,const CMD_GR_Create_Private& kNet);
	void ConnectAndCreatePrivateByServerID(int iServerID,const CMD_GR_Create_Private& kNet);
protected:
	int	 m_iJoinCout;
	char m_kJoinNumTxt[7];

	int		m_kActJoinNum;

	CMD_GR_Private_Info	m_kPrivateInfo;
	CMD_GF_Private_Room_Info m_kPrivateRoomInfo;

	CMD_GR_Create_Private	m_kCreatePrivateNet;
	Type_LinkAction			m_eLinkAction;
};