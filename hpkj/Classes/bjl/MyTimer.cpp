#include "MyTimer.h"
#include "MyTools.h"
#include "MyConfig.h"
#include "CocosUnits.h"

using namespace std;
using namespace texture_name;

MyTimer::MyTimer()
{

}

MyTimer::~MyTimer()
{
	RemoveObject();
}

bool MyTimer::init()
{
	if (!Node::init())
	{
		return false;
	}

	MyConfig& myConfig=MyConfig::Instance();

	g_timerNode = Node::create();
	//时钟
	Sprite * timer = Sprite::createWithSpriteFrameName(s_timer);
	timer->setPosition(ccp(myConfig.m_TimerPos[0],myConfig.m_TimerPos[1]));
	g_timerNode->addChild(timer);
	//时间数字
	m_number = CCLabelAtlas::create("0",texture_name::s_timerNumber,20,32,'0');
	m_number->setAnchorPoint(ccp(0.5f,0.5f));
	m_number->setPosition(ccp(myConfig.m_TimerPos[0],myConfig.m_TimerPos[1]));
	g_timerNode->addChild(m_number);

	m_timeKey= 0;
	m_gameState =Game_null;
	this->addChild(g_timerNode);

	m_nikeSprite =Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_CCteefTag));
	m_nikeSprite->setPosition(ccp(myConfig.m_NikePos[0],myConfig.m_NikePos[1]));
	this->addChild(m_nikeSprite);
	AddObject();
	return true;
}
void MyTimer::AddObject()
{
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(MyTimer::setCCteefTag),"MI_setCCteefTag",NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(MyTimer::setPlaceScore),"MI_setPlaceScore",NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(MyTimer::setCardTalayer),"MI_setCardTalayer",NULL);

}
void MyTimer::RemoveObject()
{
	NotificationCenter::getInstance()->removeObserver(this, "MI_setCCteefTag");
	NotificationCenter::getInstance()->removeObserver(this, "MI_setPlaceScore");
	NotificationCenter::getInstance()->removeObserver(this, "MI_setCardTalayer");
}
void MyTimer::start(int userId, int t,Game_State gameState,int key)
{
	this->unscheduleAllSelectors();
	g_timerNode->stopAllActions();
	m_time = t;
	m_gameState = gameState;
	m_UserID =userId;
	m_timeKey= key;
	String str;
	str.initWithFormat("%d",m_time--);
	m_number->setString(str.getCString());
	this->schedule(schedule_selector(MyTimer::time_schedule),1.0f);
}

void MyTimer::time_schedule(float t)
{
	if (m_time>0)
	{
		String str;
		str.initWithFormat("%d",m_time--);
		m_number->setString(str.getCString());
		if (m_time<5)
		{
			m_number->runAction(Sequence::create(ScaleTo::create(0.2f,1.2f),ScaleTo::create(0.2f,1.0f),NULL));
			if (g_timerNode->numberOfRunningActions()<=0)
			{
				g_timerNode->runAction(CCShake::create(10.0f,1));
			}
		}
	}else{
		this->unschedule(schedule_selector(MyTimer::time_schedule));
		g_timerNode->stopAllActions();
		this->setVisible(false);
		m_nikeSprite->setVisible(false);
		if (m_UserID ==1)
		{
			CCtimeCallBack* _callBackData=CCtimeCallBack::create(msgToView_TimeCallBack);
			_callBackData->m_gameState =m_gameState;
			_callBackData->m_timeKey =m_timeKey;
			NotificationCenter::getInstance()->postNotification(MESSAGE_TOVIEW,_callBackData);
		}
	}
}
void MyTimer::setCCteefTag(Object *obj)
{
	m_nikeSprite->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_CCteefTag));
	m_nikeSprite->setVisible(true);

}
void MyTimer::setCardTalayer( Object *obj )
{
	m_nikeSprite->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_CardTalayer));
	m_nikeSprite->setVisible(true);

}

void MyTimer::setPlaceScore( Object *obj )
{
	m_nikeSprite->setSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_PlaceScore));
	m_nikeSprite->setVisible(true);
}