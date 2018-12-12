#include "User.h"
#include "MyTools.h"
User::User()
{

}

User::~User()
{

}

bool User::init()
{
	Layer::init();

	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(User::onTouchBegan,this);
	listener->onTouchEnded = CC_CALLBACK_2(User::onTouchEnded,this);
	listener->onTouchMoved = CC_CALLBACK_2(User::onTouchMoved,this);
	listener->onTouchCancelled = CC_CALLBACK_2(User::onTouchCancelled,this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	m_userIcon=Sprite::createWithSpriteFrameName(texture_name::s_usericon);
	m_userIcon->setAnchorPoint(ccp(0.5f,0.5f));
	m_userIcon->setPosition(ccp(-300,-300));
	this->addChild(m_userIcon);

	m_userInfor=Sprite::createWithSpriteFrameName(texture_name::s_userMessage);
	m_userInfor->setAnchorPoint(ccp(0.5f,0.5f));
	this->addChild(m_userInfor,1,100);
	m_userInfor->setVisible(false);
	m_pos =ccp(0,0);
	m_UserSptKey=0;
	m_userEye=NULL;

	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);

	m_isOut=true;
	m_isOnline=true;
	return true;
}

void User::setIsOnline(bool isOnline)
{
	m_isOnline=isOnline;
	if (isOnline)
	{
		m_userIcon->setColor(ccc3(255,255,255));
		if(m_userEye)m_userEye->setColor(ccc3(255,255,255));
	}
	else
	{
		m_userIcon->setColor(ccc3(100,100,100));
		if(m_userEye)m_userEye->setColor(ccc3(100,100,100));
	}
}

void User::setUserIcon( int gender )
{
	m_userInfor->setPosition(m_pos);
	m_gender=gender;
	m_userIcon->removeFromParent();
	m_userEye=NULL;

	if (m_gender==1)	//--ÄÐ
	{
		m_UserSptKey=1;
		m_userIcon = Sprite::createWithSpriteFrameName(texture_name::s_usericon);
	}
	else
	{
		m_UserSptKey=2;
		m_userIcon = Sprite::createWithSpriteFrameName(texture_name::s_womanIcon);
	}
	if (m_pos.x>Director::getInstance()->getWinSize().width*0.5)
		m_userIcon->setRotationY(180.0f);
	m_userIcon->setScale(0.8f);
	m_userIcon->setPosition(m_pos);
	this->addChild(m_userIcon);
	eyeCallUp(NULL);

	setIsOnline(m_isOnline);
	m_isOut=true;
	cocos2d::Size size=Director::getInstance()->getWinSize();
	Vec2 pt=m_pos;
	Vec2 posPt=m_pos;
	if (m_pos.x>size.width*0.5)
	{
		pt.x+=(m_userInfor->getContentSize().width+100);
		posPt.x-=60;
	}
	else
	{
		pt.x-=(m_userInfor->getContentSize().width+100);
		posPt.x+=60;
	}
	m_userInfor->stopAllActions();
	m_userInfor->setVisible(true);
	m_userInfor->setPosition(posPt);
	m_userInfor->runAction(Sequence::create(DelayTime::create(5.0f),MoveTo::create(0.2f,pt),NULL));
}

void User::setLordIcon( bool blord )
{
	m_userInfor->setPosition(m_pos);
	m_userIcon->stopAllActions();
	m_userIcon->removeFromParent();
	m_userEye=NULL;

	m_userIcon=Sprite::createWithSpriteFrameName(texture_name::s_usericon);
	SpriteFrame * frame=NULL;
	Vector<SpriteFrame *> animFrames;
	for(int i=0;i<5;i++)
	{
		String str;
		str.initWithFormat("yanwu%d.png",i);
		frame = SpriteFrameCache::getInstance()->spriteFrameByName(str.getCString());
		animFrames.pushBack(frame);
	}
	if (blord)	//--µØÖ÷
	{	
		if (m_gender==1)
		{
			m_UserSptKey=3;
			frame = SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_dizhu_man);
		}
		else
		{
			m_UserSptKey=4;
			frame = SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_dizhu_woman);
		}
	}
	else
	{
		if (m_gender==1)
		{
			m_UserSptKey=5;
			frame = SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_farmer_man);
		}
		else
		{
			m_UserSptKey=6;
			frame = SpriteFrameCache::getInstance()->spriteFrameByName(texture_name::s_farmer_woman);
		}
	}
	animFrames.pushBack(frame);
	Animation * animation = Animation::createWithSpriteFrames(animFrames,0.2f);
	m_userIcon->setScale(0.8f);
	m_userIcon->runAction(Sequence::create(Animate::create(animation),CCCallFuncO::create(this,callfuncO_selector(User::eyeCallUp),NULL),NULL));
	m_userIcon->setPosition(m_pos);
	this->addChild(m_userIcon);
	if (m_pos.x>Director::getInstance()->getWinSize().width*0.5)
		m_userIcon->setRotationY(180.0f);

	setIsOnline(m_isOnline);
	m_isOut=false;
	userIconCallBack(NULL);
}

void User::setUserInfor( char* name,int gold,char* huafeidian )
{
		m_userInfor->removeAllChildren();
		cocos2d::Size size=m_userInfor->getContentSize();

		LabelTTF* laberName=LabelTTF::create(name,"",16);
		laberName->setColor(ccc3(173,118,0));
		laberName->setPosition(ccp(size.width*0.5,size.height*0.8));
		m_userInfor->addChild(laberName);

		LabelTTF* goldname=LabelTTF::create(String::createWithFormat("%d",gold)->getCString(),"",16);
		goldname->setAnchorPoint(ccp(0.5f,0.5f));
		goldname->setColor(ccc3(173,118,0));
		goldname->setPosition(ccp(size.width*0.5,size.height*0.5));
		m_userInfor->addChild(goldname);

		LabelTTF* huafeidianname=LabelTTF::create(huafeidian,"",16);
		huafeidianname->setAnchorPoint(ccp(0.5f,0.5f));
		huafeidianname->setColor(ccc3(173,118,0));
		huafeidianname->setPosition(ccp(size.width*0.5,size.height*0.2));
		m_userInfor->addChild(huafeidianname);
}

void User::userIconCallBack( Object* obj )
{
	m_isOut=!m_isOut;

	cocos2d::Size size=Director::getInstance()->getWinSize();
	Vec2 pt=m_pos;
	Vec2 posPt=m_pos;
	if (m_pos.x>size.width*0.5)
	{
		pt.x+=(m_userInfor->getContentSize().width+100);
		posPt.x-=60;
	}
	else
	{
		pt.x-=(m_userInfor->getContentSize().width+100);
		posPt.x+=60;
	}
	m_userInfor->stopAllActions();
	m_userInfor->setVisible(true);
	if (m_isOut)
	{
		m_userInfor->runAction(MoveTo::create(0.2f,pt));
	}
	else
	{
		m_userInfor->setPosition(pt);
		m_userInfor->runAction(Sequence::create(MoveTo::create(0.2f,posPt),DelayTime::create(3.0f),MoveTo::create(0.2f,pt),NULL));
	}
}

bool User::onTouchBegan(Touch *pTouch,Event *pEvent)
{
	Vec2 pt=pTouch->getLocation();
	if (m_userIcon->boundingBox().containsPoint(pt))
	{
		userIconCallBack(NULL);
	}
	return false;
}

void User::eyeCallUp( Object* _obj )
{
	if (m_userIcon==NULL)return;
	if (m_userEye)
	{
		m_userEye->stopAllActions();
		m_userEye->removeFromParent();
	}

	SpriteFrame * frame=NULL;
	Vector<SpriteFrame *> animFrames;
	Vec2 pt;
	for(int i=0;i<3;i++)
	{
		String str;
		switch (m_UserSptKey)
		{
		case 1:
			str.initWithFormat("m_eye%d.png",i);
			pt=ccp(55,126);
			break;
		case 2:
			str.initWithFormat("wm_eye%d.png",i);
			pt=ccp(55+12,125);
			break;
		case 3:
			str.initWithFormat("dz_m_eye%d.png",i);
			pt=ccp(54,126+38+12+3);
			break;
		case 4:
			str.initWithFormat("dz_wm_eye%d.png",i);
			pt=ccp(54.5,126+21);
			break;
		case 5:
			str.initWithFormat("fm_m_eye%d.png",i);
			pt=ccp(55+1,126+38);
			break;
		case 6:
			str.initWithFormat("fm_wm_eye%d.png",i);
			pt=ccp(55+12,126+36);
			break;
		}
		frame = SpriteFrameCache::getInstance()->spriteFrameByName(str.getCString());
		animFrames.pushBack(frame);
	}
	Animation * animation = Animation::createWithSpriteFrames(animFrames,0.2f);
	m_userEye=Sprite::createWithSpriteFrame(frame);
	m_userEye->setAnchorPoint(ccp(0,0));
	m_userEye->setPosition(pt);
	m_userEye->runAction(Sequence::create(Animate::create(animation),DelayTime::create(rand()%10),CCCallFuncO::create(this,callfuncO_selector(User::eyeCallUp),NULL),NULL));
	m_userIcon->addChild(m_userEye);
	setIsOnline(m_isOnline);
}
