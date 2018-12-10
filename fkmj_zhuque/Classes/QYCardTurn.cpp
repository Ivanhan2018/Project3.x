
#include "QYCardTurn.h"
#include "QYPageTurn3D.h"
#include "utility.h"

USING_NS_CC;

QYCardTurn::QYCardTurn(float fWidth,float fHeight)
	:m_pGridNodeTarget(NULL)
	,m_pQYPageTurn3D(NULL)
	,m_pNodeFront(NULL)
	,m_pNodeBack(NULL)
	,m_bTurnEnd(false)
{
	init();
	initState(fWidth,fHeight);
	cocos2d::Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(QYCardTurn::updata),this,0.0f,false);
}

cocos2d::Node*	QYCardTurn::getNodeFront()
{
	return m_pNodeFront;
}
cocos2d::Node*	QYCardTurn::getNodeBack()
{
	return m_pNodeBack;
}
void QYCardTurn::setTurnPos(cocos2d::Vec2 kPos)
{
	float fWidth = m_pQYPageTurn3D->getWidth();
	float fHeight = m_pQYPageTurn3D->getHeight();
	m_pGridNodeTarget->setPosition(cocos2d::Point(kPos.x-fWidth/2,kPos.y-fHeight/2));
	m_pNodeFront->setPosition(fWidth/2-m_pGridNodeTarget->getPosition().x,fHeight/2-m_pGridNodeTarget->getPosition().y);
	m_pNodeBack->setPosition(fWidth/2-m_pGridNodeTarget->getPosition().x+fWidth,fHeight/2-m_pGridNodeTarget->getPosition().y);
	m_pQYPageTurn3D->OnActionPoint();
}
QYCardTurn::~QYCardTurn()
{
	cocos2d::Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(QYCardTurn::updata),this);
}
void QYCardTurn::initState(float fWidth,float fHeight)
{
	Director *director = Director::getInstance();
	Size s = director->getWinSizeInPixels();
	m_pGridNodeTarget = cocos2d::NodeGrid::create(Rect(0,0,fWidth,fHeight));
	addChild(m_pGridNodeTarget, 0);
	m_pGridNodeTarget->setPosition(cocos2d::Point(1280/2-fWidth/2,720/2-fHeight/2));
	m_pQYPageTurn3D = QYPageTurn3D::create(m_pGridNodeTarget,Size(80,50));
	m_pQYPageTurn3D->setSize(fWidth,fHeight);
	m_pQYPageTurn3D->setCallFun(CC_CALLBACK_3(QYCardTurn::CallPos,this));
	m_pQYPageTurn3D->setEndCallFun(CC_CALLBACK_0(QYCardTurn::turnEndCallback,this));

	m_pNodeFront = Node::create();
	m_pGridNodeTarget->addChild(m_pNodeFront, 0);
	m_pNodeFront->setTag(1);
	m_pNodeFront->setScale(0.5f);
	m_pNodeFront->setPosition(fWidth/2-m_pGridNodeTarget->getPosition().x-fWidth,fHeight/2-m_pGridNodeTarget->getPosition().y);

	m_pNodeBack = Node::create();
	m_pGridNodeTarget->addChild(m_pNodeBack, 0);
	m_pNodeBack->setScale(0.5f);
	m_pNodeBack->setTag(2);
	m_pNodeBack->setPosition(fWidth/2-m_pGridNodeTarget->getPosition().x+fWidth,fHeight/2-m_pGridNodeTarget->getPosition().y);


}
QYPageTurn3D* QYCardTurn::getPageTurn3D()
{
	return m_pQYPageTurn3D;
}
cocos2d::NodeGrid* QYCardTurn::getGridNodeTarget()
{
	return m_pGridNodeTarget;
}
bool GetCross(cocos2d::Vec2& CrossP,cocos2d::Vec2 p1,cocos2d::Vec2 p2,cocos2d::Vec2 p3,cocos2d::Vec2 p4)
{
	float fFloatMin = 0.00001f;
	float a1 = 0;
	float b1 = 0;
	if (utility::fAbs(p2.x - p1.x) < fFloatMin)
	{
		return false;
	}
	if (utility::fAbs(p3.x - p4.x) < fFloatMin)
	{
		return false;
	}
	a1 = (p2.y - p1.y)/(p2.x - p1.x);
	b1 = ((p1.x * p2.y) - p2.x*p1.y)/(p1.x - p2.x);

	float a2 = 0;
	float b2 = 0;
	a2 = (p4.y-p3.y)/(p4.x-p3.x+fFloatMin);
	b2 = ((p3.x*p4.y)-(p4.x*p3.y))/(p3.x-p4.x);

	if (utility::fAbs(a1 - a2) < fFloatMin)
	{
		return false;
	}
	CrossP.x = 0;
	CrossP.x = (b2-b1)/(a1-a2);
	CrossP.y =a1*CrossP.x +b1;
	return true;
}
void QYCardTurn::CallPos(cocos2d::Vec2 kPos,float fAngle,bool inAction)
{
	float fWidth = m_pQYPageTurn3D->getWidth();
	float fHeight = m_pQYPageTurn3D->getHeight();
	kPos -= Vec2(fWidth,fHeight)/2;
}
void QYCardTurn::updata(float fTime)
{

}

void QYCardTurn::turnEndCallback()
{
	setIsTurnEnd(true);
	if (m_pTurnEndCall)
	{
		m_pTurnEndCall();
	}
}
void QYCardTurn::setIsTurnEnd(bool bEnd)
{
	 m_bTurnEnd = bEnd;
}
bool QYCardTurn::getIsTurnEnd()
{
	return m_bTurnEnd;
}

void QYCardTurn::show()
{
	setVisible(true);
	setIsTurnEnd(false);
	m_pQYPageTurn3D->restCard();
}
void QYCardTurn::hide()
{
	setVisible(false);
	setIsTurnEnd(false);
}
void QYCardTurn::setEndCallFun(std::function<void(void)> pFun)
{
	m_pTurnEndCall = pFun;
}