#pragma once
#include "cocos2d.h"
#include "TimeNode.h"
#include "FvSingleton.h"
#include "ScenceManagerBase.h"
#include "WidgetDefine.h"


class HNScenceManager
	:public cocos2d::Ref
	,public TimeNode
	,public ScenceManagerBase
	,public FvSingleton<HNScenceManager>
{
public:
	HNScenceManager(cocos2d::Node* pNode);
	HNScenceManager();
	~HNScenceManager();
public:
	virtual void init();
	void HideAll(cocos2d::Node* pActShowNode);
	cocos2d::Node* getRootNode();
	void update(float fTime);

public:
	void addGameNode(cocos2d::Node* pNode);
	void InLogonScence();
	void InHomeScence();
	void InPlayerInfoScence();
public:
	virtual void GameBackScence();
	void BackKey();
	void Button_CB_ExitGame(cocos2d::Ref*,WidgetUserInfo*);
protected:
	int					m_iScenceTimeID;
	bool				m_bInBackground;
	cocos2d::Node*		m_pRootNode;
};

