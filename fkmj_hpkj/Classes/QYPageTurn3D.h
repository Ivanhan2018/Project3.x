#pragma once

#include "2d/CCActionGrid3D.h"


NS_CC_BEGIN

class Grid3D;

class QYPageTurn3D
	:public Ref
{
public:
	enum PointType
	{
		Type_Button_Left,
		Type_Button_Right,
		Type_Top_Left,
		Type_Top_Right,
	};
	enum TouchType
	{
		Type_Button,
		Type_Top,
		Type_Left,
		Type_Right,
	};
public:
	QYPageTurn3D();
	~QYPageTurn3D();
public:
	static QYPageTurn3D* create(NodeGrid* _gridNodeTarget, const Size& gridSize);

	void restCard();
	float getWidth();
	float getHeight();
	void setSize(float fWidth,float fHeight);
	Vec2 changePosToNode(Vec2 kPos);
	void setTouchPos(Vec2 kPos);
	void setMoveToPos(Vec2 kPos,float fOffect = 0);
	void setEndPos(Vec2 kPos);
	Vec2 getPosByPoint(PointType kType);
	void OnStartAction(TouchType eTouchType);
	void setEndCallFun(std::function<void(void)> pFun);
	void setCallFun(std::function<void(cocos2d::Vec2,float,bool)> pFun);
	void OnActionPoint();
	void OpenDirect();
	void setAngle(float fAngle,bool bDirect);
	float getAngle();
	bool IsInMoveAction(bool bEnd = false);
private:
	void init(NodeGrid* _gridNodeTarget, const Size& gridSize);
	float distance(Vec2 a,Vec2 b,Vec2 p);
	void updata(float fTime);
private:
	Size m_kGridSize;
	NodeGrid* m_pNodeGrid;
	Grid3D*	m_pGrid3D;
	float m_fWidth;
	float m_fHeight;
	Vec2 m_kTouchPos;
	PointType m_ePoingType;

	float m_fActAngle;
	float m_fDestAngle;
	float m_fMoveActionTime;
	float m_fDurTime;
	Vec2 m_kMoveEndPos;
	Vec2 m_kMoveEndDestPos;
	Vec2 m_kMoveTouchPos;
	Vec2 m_kMoveTouchDestPos;

	std::function<void()>   m_pEndCall;
	std::function<void(cocos2d::Vec2,float,bool)> m_pCallPos;
};


NS_CC_END
