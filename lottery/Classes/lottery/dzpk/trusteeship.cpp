#include "trusteeship.h"
#include "DDZRes.h"//by hxh
#include "DZPKTools.h"
#include "RoomLayer.h"
using namespace cocos2d;

Trusteeship::Trusteeship(void)
{
	m_wCoutDown=15;
}


Trusteeship::~Trusteeship(void)
{
}

Trusteeship *Trusteeship::create(){
	Trusteeship *trusteeship = new Trusteeship();
	if (trusteeship&&trusteeship->init())
	{
		trusteeship->autorelease();
		return trusteeship;
	}	
	CC_SAFE_DELETE(trusteeship);
	return NULL;
}

bool Trusteeship::init()
{
	auto listener = EventListenerTouchOneByOne::create();
	//listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(Trusteeship::onTouchBegan,this);
	listener->onTouchEnded = CC_CALLBACK_2(Trusteeship::onTouchEnded,this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	bool bRet = false;
	do 
	{

		CC_BREAK_IF(! Layer::init());
		m_isTrusteeship = false;

		m_pLockSpr=Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_ts_bg_lock));
		m_pLockSpr->setPosition(ccp(400+30, 210+40));
		this->addChild(m_pLockSpr);
		m_pLockSpr->setOpacity(100);
		m_pLockSpr->setScale(0.8f);
		m_pLockSpr->setVisible(false);


		m_pCircleSpr=Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_ts_bg_circle1));
		m_pCircleSpr->setPosition(ccp(400+30, 210+40));
		//v->setOpacity(150);
		this->addChild(m_pCircleSpr,20);
		m_pCircleSpr->setVisible(false);

		for (int i=0;i<=2;i++)
		{
			m_pArrowrSpr[i]=Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_ts_bg_arrow));
			m_pArrowrSpr[i]->setPosition(ccp(520+i*30+30, 210+40));
			this->addChild(m_pArrowrSpr[i],20);
			m_pArrowrSpr[i]->setOpacity(0);

			m_pArrowlSpr[i]=Sprite::createWithSpriteFrame(spriteFrame(texture_name::s_ts_bg_arrow));
			m_pArrowlSpr[i]->setPosition(ccp(270-i*30+40, 210+40));
			this->addChild(m_pArrowlSpr[i],20);
			m_pArrowlSpr[i]->setRotation(180);
			m_pArrowlSpr[i]->setOpacity(0);

		}


		LayerColor* layerColor1=LayerColor::create(ccc4(0, 0, 0, 100));
		this->addChild(layerColor1);
		this->setVisible(false);


		//_label = Label::create("","Arial", 24); //dzpk-jhy
		_label = Label::create("","", 24);
		_label->setString(DZPK_loadText("t7"));
		_label->setColor(ccc3(255,255,255));
		_label->setPosition(ccp(430,20));
		this->addChild(_label);


		//CountdownITF = Label::create("15","Arial", 30); //dzpk-jhy
		CountdownITF = Label::create("15","", 30);
		CountdownITF->setColor(ccc3(255,255,255));
		CountdownITF->setPosition(ccp(430,80));
		this->addChild(CountdownITF);

		//director=Director::getInstance();

		bRet = true;
	}while(0);
	return bRet;
}
void Trusteeship::trusteeStart()
{
	//Director::getInstance()->getTouchDispatcher()->removeDelegate(this);
	//Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

	m_isTrusteeship = true;

	this->schedule( schedule_selector(Trusteeship::trustee), 1.0f);
	this->schedule( schedule_selector(Trusteeship::Countdown), 1.0f);
	this->setVisible(true);

	//for (int i=0;i<=2;i++)
	//{
	//	
	//	m_pArrowrSpr[i]->setVisible(true);		
	//	m_pArrowlSpr[i]->setVisible(true);

	//}
	

}
void Trusteeship::Countdown(float f)
{
	m_wCoutDown--;
	char m_szName[10];
	sprintf(m_szName,"%d",m_wCoutDown);
	CountdownITF->setString(m_szName);
	if (m_wCoutDown==0)
	{
		EntityMgr::instance()->getDispatch()->SendPacketWithPerformStandup();
		EntityMgr::instance()->getDispatch()->closeGameSocket();
		EntityMgr::instance()->roomFrame()->reset();
		CocosDenshion::SimpleAudioEngine::getInstance()->end();
		EntityMgr::instance()->getDispatch()->setGameStatus(false);
		Scene * scene = RoomLayer::scene();
		Director::getInstance()->replaceScene(scene);
        unschedule(schedule_selector(Trusteeship::trustee));
		unschedule(schedule_selector(Trusteeship::Countdown));
	}
}
void Trusteeship::trusteeEnd()
{

	//----------------
	m_isTrusteeship = false;
	//Director::getInstance()->getTouchDispatcher()->removeDelegate(this);
	Layer::onExit();
	//director->getActionManager()->pauseAllRunningActions();
	//---------------
	unschedule(schedule_selector(Trusteeship::Countdown));
	m_wCoutDown=15;
	char m_szName[10];
	sprintf(m_szName,"%d",15);
	CountdownITF->setString(m_szName);
	this->setVisible(false);
	unschedule(schedule_selector(Trusteeship::trustee));
	m_pLockSpr->setVisible(false);
	m_pCircleSpr->setVisible(false);

	for (int i=0;i<=2;i++)
	{
	    m_pArrowrSpr[i]->setOpacity(0);
	//	m_pArrowrSpr[i]->setVisible(false);
		m_pArrowlSpr[i]->setOpacity(0);
	//	m_pArrowlSpr[i]->setVisible(false);

	}
	//m_pCircleSpr->setVisible(false);
}
void Trusteeship::trustee(float f)
{
	m_pLockSpr->setVisible(true);
	m_pLockSpr->setOpacity(100);
	m_pLockSpr->setScale(0.8f);
	CCActionInterval *ac=FadeTo::create(0.6f,255);
	CCActionInterval *as=ScaleTo::create(0.6f,1.0f);
	CCAction *h=Spawn::create(ac,as,NULL);
	m_pLockSpr->runAction(h);
	this->scheduleOnce(schedule_selector(Trusteeship::DrawCircle), 0.4f);
	 
    Sequence *actionr=(Sequence*)Sequence::create(DelayTime::create(0.4f),FadeIn::create(0.5f),FadeOut::create(0.5f),NULL);
    Sequence *actionr1=(Sequence*)Sequence::create(DelayTime::create(0.5f),FadeIn::create(0.5f),FadeOut::create(0.5f),NULL);
    Sequence *actionr2=(Sequence*)Sequence::create(DelayTime::create(0.6f),FadeIn::create(0.5f),FadeOut::create(0.5f),NULL);

    Sequence *actionl=(Sequence*)Sequence::create(DelayTime::create(0.4f),FadeIn::create(0.5f),FadeOut::create(0.5f),NULL);
    Sequence *actionl1=(Sequence*)Sequence::create(DelayTime::create(0.5f),FadeIn::create(0.5f),FadeOut::create(0.5f),NULL);
    Sequence *actionl2=(Sequence*)Sequence::create(DelayTime::create(0.6f),FadeIn::create(0.5f),FadeOut::create(0.5f),NULL);
	m_pArrowrSpr[0]->runAction(actionr);
	m_pArrowrSpr[1]->runAction(actionr1);
	m_pArrowrSpr[2]->runAction(actionr2);

	m_pArrowlSpr[0]->runAction(actionl);
	m_pArrowlSpr[1]->runAction(actionl1);
	m_pArrowlSpr[2]->runAction(actionl2);
											
}

void Trusteeship::DrawCircle(float f)
{
	m_pCircleSpr->setVisible(true);
	CCAnimation *animation=CCAnimation::create();
	for( int i=2;i<=5;i++)
	{
		char szName[100] = {0};
		sprintf(szName, "ts_bg_circle%d.png", i);

		SpriteFrame * frame = SpriteFrameCache::getInstance()->spriteFrameByName(szName);	
		animation->addSpriteFrame(frame);

	}	
	animation->setDelayPerUnit(0.2f);
	Animate * matchPeopleDH = Animate::create(animation);
	Sequence *seq=(Sequence*)Sequence::create(matchPeopleDH,CallFunc::create(this,callfunc_selector(Trusteeship::CircleEnd)),NULL);
	m_pCircleSpr->runAction(seq);
                                   
};

void Trusteeship::CircleEnd()
{
	m_pCircleSpr->setVisible(false);
}

bool Trusteeship::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}

void Trusteeship::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (m_isTrusteeship)
	{
		trusteeEnd();
	}
}
