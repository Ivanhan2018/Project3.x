#pragma once

#include "cocos2d.h"
#include "FvSingleton.h"
#include "GameLib.h"
#include "QYPageTurn3D.h"

class QYCardTurn;

class QYCardTurnManager
	:public cocos2d::Layer
	,public FvSingleton<QYCardTurnManager>
{
public:
	enum TouchType
	{
		TYPE_BEGIN,
		TYPE_MOVE,
		TYPE_END,
		TYPE_Rotate,
	};
public:
	QYCardTurnManager();
	~QYCardTurnManager();

	void initCard();
	void HideCard();
	void showCard(bool bTouch,
		cocos2d::Node* pNode1Show,cocos2d::Node* pNode1Hide,cocos2d::Node* pNode2Show,cocos2d::Node* pNode2Hide);

	bool GetTouchCardTurn(QYCardTurn* pCardTurn,cocos2d::Vec2 kPos);

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void onTouchesBegan(cocos2d::Vec2 kPos);
	void onTouchesMoved(cocos2d::Vec2 kPos);
	void onTouchesEnded(cocos2d::Vec2 kPos);

	void onNetTouchAction(int iIdex,int iAction,cocos2d::Vec2 kPos);
	void setCallFun(std::function<void(int,int,cocos2d::Vec2)> pFun);

	void setEndCallBack(std::function<void(void)> pFun1,std::function<void(void)> pFun2);

	bool isCardTurndEnd();
	
	void showOpenBtn(bool bShow);
	void showShadowInfo(bool bShow);
	void setShadowInfoScale(float fScale);
	void setOpenPlayerName(const std::string& kName);
public:
	void AutoOpen();
	void setAngle(float fAngle,bool bDirect);
public:
	void Button_OpenCardDirect0(cocos2d::Ref*,WidgetUserInfo*);
	void Button_OpenCardDirect1(cocos2d::Ref*,WidgetUserInfo*);
	void Button_Rotate(cocos2d::Ref*,WidgetUserInfo*);
private:
	QYCardTurn*		m_pCardTurn1;
	QYCardTurn*		m_pCardTurn2;

	QYCardTurn*		m_pActCardTurn;

	bool			m_bMoved;
	bool			m_bCardTurned[2];
	std::function<void(int,int,cocos2d::Vec2)> m_pFun;

	cocos2d::Vec2            m_kBegainPos;
	bool                    m_bIsSelf;
};


