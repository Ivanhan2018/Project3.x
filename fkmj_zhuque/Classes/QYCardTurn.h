#pragma once

#include "cocos2d.h"
#include "FvSingleton.h"

#include "QYPageTurn3D.h"

class QYCardTurn
	:public cocos2d::Node
{
public:
	QYCardTurn(float fWidth,float fHeight);
	~QYCardTurn();

	cocos2d::Node*	getNodeFront();
	cocos2d::Node*	getNodeBack();
	void setTurnPos(cocos2d::Vec2 kPos);

	void initState(float fWidth,float fHeight);
	cocos2d::QYPageTurn3D* getPageTurn3D();
	cocos2d::NodeGrid* getGridNodeTarget();
	void CallPos(cocos2d::Vec2 kPos1,float fAngle,bool inAction);
	void turnEndCallback();
	
	void setIsTurnEnd(bool bEnd);
	bool getIsTurnEnd();

	void show();
	void hide();
	void setEndCallFun(std::function<void(void)> pFun);
public:
	void updata(float fTime);
private:
	cocos2d::NodeGrid*		m_pGridNodeTarget;
	cocos2d::QYPageTurn3D*	m_pQYPageTurn3D;
	cocos2d::Node*			m_pNodeFront;
	cocos2d::Node*			m_pNodeBack;

	bool                     m_bTurnEnd;
	std::function<void()>      m_pTurnEndCall;
	

};


