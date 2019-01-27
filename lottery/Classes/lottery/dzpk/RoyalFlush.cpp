#include "RoyalFlush.h"
#include "BJLRes.h"//by hxh
using namespace cocos2d;

RoyalFlush::RoyalFlush(void)
{
	
}


RoyalFlush::~RoyalFlush(void)
{
}

RoyalFlush *RoyalFlush::create(){
	RoyalFlush * Royal= new RoyalFlush();
	if (Royal&&Royal->init())
	{
		Royal->autorelease();
		return Royal;
	}	
	CC_SAFE_DELETE(Royal);
	return NULL;
}

bool RoyalFlush::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(! Layer::init());

	    cocos2d::Size size = Director::getInstance()->getWinSize();

		LayerColor* layerColor1=LayerColor::create(ccc4(0, 0, 0, 100));
		this->addChild(layerColor1);

		m_pBackLightSpr = Sprite::createWithSpriteFrameName(texture_name::s_rf_back_light);
		this->addChild(m_pBackLightSpr);
		m_pBackLightSpr->setPosition(ccp(size.width/2, size.height/2));

		m_pBackLightSpr1 = Sprite::createWithSpriteFrameName(texture_name::s_rf_back_light);
		this->addChild(m_pBackLightSpr1);
		m_pBackLightSpr1->setPosition(ccp(size.width/2, size.height/2));
		m_pBackLightSpr1->setRotationX(180);
		m_pBackLightSpr1->setVisible(false);

		m_pCardSprA = Sprite::createWithSpriteFrameName(texture_name::s_rf_a);
		this->addChild(m_pCardSprA);
		m_pCardSprA->setPosition(ccp(size.width/2-60, size.height/2+45));
		m_pCardSprA->setVisible(false);

		m_pCardSprK = Sprite::createWithSpriteFrameName(texture_name::s_rf_k);
		this->addChild(m_pCardSprK);
		m_pCardSprK->setPosition(ccp(size.width/2-30, size.height/2+75));
		m_pCardSprK->setVisible(false);

		m_pCardSprQ = Sprite::createWithSpriteFrameName(texture_name::s_rf_q);
		this->addChild(m_pCardSprQ);
		m_pCardSprQ->setPosition(ccp(size.width/2+5, size.height/2+80));
		m_pCardSprQ->setVisible(false);

		m_pCardSprJ = Sprite::createWithSpriteFrameName(texture_name::s_rf_j);
		this->addChild(m_pCardSprJ);
		m_pCardSprJ->setPosition(ccp(size.width/2+45, size.height/2+70));
		m_pCardSprJ->setVisible(false);

		m_pCardSpr10 = Sprite::createWithSpriteFrameName(texture_name::s_rf_10);
		this->addChild(m_pCardSpr10);
		m_pCardSpr10->setPosition(ccp(size.width/2+70, size.height/2+40));
		m_pCardSpr10->setVisible(false);




		m_pBadgeSpr = Sprite::createWithSpriteFrameName(texture_name::s_rf_back_circle);
		this->addChild(m_pBadgeSpr);
		m_pBadgeSpr->setPosition(ccp(size.width/2, size.height/2));


		m_pZiSpr0 = Sprite::createWithSpriteFrameName(texture_name::s_rf_zi);
		this->addChild(m_pZiSpr0);
		m_pZiSpr0->setPosition(ccp(size.width/2, size.height/2-6));
		m_pZiSpr0->setScale(2.0f);
		m_pZiSpr0->setVisible(false);

		//zi1 = Sprite::createWithSpriteFrameName("zi1.png");
		//this->addChild(zi1);
		//zi1->setPosition(ccp(size.width/2, size.height/2));
		//zi1->setScale(2.0f);
		//zi1->runAction(ScaleTo::create(0.15,1));
		//zi1->setVisible(false);
		this->setVisible(false);
		bRet = true;
	}while(0);
	return bRet;
}


void  RoyalFlush::Card10(float f)
{
	m_pCardSpr10->setVisible(true);
	cocos2d::Size size = Director::getInstance()->getWinSize();
	m_pCardSpr10->setPosition(ccp(size.width/2+70-30, size.height/2+40-30));
	m_pCardSpr10->setVisible(true);
	m_pCardSpr10->setScale(1.02f);
	CCAction *action=Spawn::create(CCMoveBy::create(0.05,Vec2(-1,-1)),ScaleTo::create(0.05f,1.0f),NULL);
	m_pCardSpr10->runAction((Sequence*)Sequence::create(CCMoveBy::create(0.05,Vec2(30,30)),DelayTime::create(0.05f),action,NULL));

}
void  RoyalFlush::CardJ(float f)
{
	m_pCardSprJ->setVisible(true);
	cocos2d::Size size = Director::getInstance()->getWinSize();
	m_pCardSprJ->setPosition(ccp(size.width/2+45-20, size.height/2+70-30));
	m_pCardSprJ->setVisible(true);
	m_pCardSprJ->setScale(1.02f);
	CCAction *action=Spawn::create(CCMoveBy::create(0.05,Vec2(-1,-1)),ScaleTo::create(0.05f,1.0f),NULL);
	m_pCardSprJ->runAction((Sequence*)Sequence::create(CCMoveBy::create(0.05,Vec2(20,30)),DelayTime::create(0.05f),action,NULL));
}
void  RoyalFlush::CardQ(float f)
{
	m_pCardSprQ->setVisible(true);
	cocos2d::Size size = Director::getInstance()->getWinSize();
	m_pCardSprQ->setPosition(ccp(size.width/2+5+5, size.height/2+80-30));


	m_pCardSprQ->setScale(1.02f);
	CCAction *action=Spawn::create(CCMoveBy::create(0.05,Vec2(-1,-1)),ScaleTo::create(0.05f,1.0f),NULL);
	m_pCardSprQ->runAction((Sequence*)Sequence::create(CCMoveBy::create(0.05,Vec2(-5,30)),DelayTime::create(0.05f),action,NULL));

}
void  RoyalFlush::CardK(float f)
{
	m_pCardSprK->setVisible(true);
	cocos2d::Size size = Director::getInstance()->getWinSize();
	m_pCardSprK->setPosition(ccp(size.width/2-30+30, size.height/2+75-20));
	CCAction *action=Spawn::create(CCMoveBy::create(0.05,Vec2(-1,-1)),ScaleTo::create(0.05f,1.0f),NULL);
	m_pCardSprK->runAction((Sequence*)Sequence::create(CCMoveBy::create(0.05,Vec2(-30,20)),DelayTime::create(0.05f),action,NULL));
}
void  RoyalFlush::CardA(float f)
{
	m_pCardSprA->setVisible(true);
	cocos2d::Size size = Director::getInstance()->getWinSize();
	m_pCardSprA->setPosition(ccp(size.width/2-60+30, size.height/2+45));
	CCAction *action=Spawn::create(CCMoveBy::create(0.05,Vec2(-1,-1)),ScaleTo::create(0.05f,1.0f),NULL);
	m_pCardSprA->runAction((Sequence*)Sequence::create(CCMoveBy::create(0.05,Vec2(-30,0)),DelayTime::create(0.05f),action,NULL));

}

void RoyalFlush::CardAni(float f)
{
	m_pZiSpr0->setVisible(true);
	m_pZiSpr0->runAction(ScaleTo::create(0.1,1));
	m_pCardSpr10->runAction((Sequence*)Sequence::create(CCMoveBy::create(0.05,Vec2(2,-3)),CCMoveBy::create(0.1,Vec2(-2,3)),NULL));
	m_pCardSprA->runAction((Sequence*)Sequence::create(CCMoveBy::create(0.05,Vec2(2,-3)),CCMoveBy::create(0.1,Vec2(-2,3)),NULL));
	m_pCardSprQ->runAction((Sequence*)Sequence::create(CCMoveBy::create(0.05,Vec2(2,-3)),CCMoveBy::create(0.1,Vec2(-2,3)),NULL));
	m_pCardSprJ->runAction((Sequence*)Sequence::create(CCMoveBy::create(0.05,Vec2(2,-3)),CCMoveBy::create(0.1,Vec2(-2,3)),NULL));
	m_pCardSprK->runAction((Sequence*)Sequence::create(CCMoveBy::create(0.05,Vec2(2,-3)),CCMoveBy::create(0.1,Vec2(-2,3)),NULL));
	m_pBadgeSpr->runAction((Sequence*)Sequence::create(CCMoveBy::create(0.05,Vec2(2,-3)),CCMoveBy::create(0.1,Vec2(-2,3)),NULL));

}

void RoyalFlush::BackLight(float f)
{

}

void RoyalFlush::RoyalStart()
{
	this->setVisible(true);
	m_pZiSpr0->setScale(3.0f);
	m_pZiSpr0->setVisible(false);

	m_pCardSprK->setVisible(false);
	m_pCardSprQ->setVisible(false);
	m_pCardSprJ->setVisible(false);
	m_pCardSprA->setVisible(false);
	m_pCardSpr10->setVisible(false);
	//schedule(schedule_selector(RoyalFlush::Card10), 0.4f);
	scheduleOnce(schedule_selector(RoyalFlush::Card10), 0.4f);
	scheduleOnce(schedule_selector(RoyalFlush::CardJ), 0.6f);
	scheduleOnce(schedule_selector(RoyalFlush::CardQ), 0.8f);
	scheduleOnce(schedule_selector(RoyalFlush::CardK), 1.0f);
	scheduleOnce(schedule_selector(RoyalFlush::CardA), 1.2f);
	scheduleOnce(schedule_selector(RoyalFlush::CardAni), 2.0f);
	m_pBackLightSpr->runAction((Sequence*)Sequence::create(DelayTime::create(0.4),RotateBy::create(3.0f,-200),CallFunc::create(this,callfunc_selector(RoyalFlush::End)),NULL));
	//m_pBackLightSpr1->runAction((Sequence*)Sequence::create(DelayTime::create(0.4),RotateBy::create(4.0f,-200),NULL));
	m_pBadgeSpr->setScale(0.2f);
	m_pBadgeSpr->runAction((Sequence*)Sequence::create(ScaleTo::create(0.25f,1.0f),ScaleTo::create(0.1f,0.9f),ScaleTo::create(0.1f,1.0f),NULL));

}
void RoyalFlush::End()
{
	this->setVisible(false);

}
