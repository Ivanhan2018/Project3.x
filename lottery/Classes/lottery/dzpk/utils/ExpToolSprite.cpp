#include "ExpToolSprite.h"


ExpToolSprite::ExpToolSprite():expSprite(NULL),expBgSprite(NULL),percent(1.0f)
{

}

ExpToolSprite::~ExpToolSprite()
{
	CC_SAFE_RELEASE_NULL(expBgSprite);
	CC_SAFE_RELEASE_NULL(expSprite);
}

ExpToolSprite* ExpToolSprite::createWith(const char* expToolBg,const char* expTool)
{
	ExpToolSprite *pRet = new ExpToolSprite();
	if (pRet)
	{
		pRet->expBgSprite = CCSprite::create(expToolBg);
		pRet->expSprite = CCSprite::create(expTool);
	}
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

ExpToolSprite* ExpToolSprite::createWithFrame(CCSpriteFrame* expBg,CCSpriteFrame* exp)
{
    ExpToolSprite *pRet = new ExpToolSprite();
	if (pRet)
	{
		pRet->expBgSprite = CCSprite::createWithSpriteFrame(expBg);
		pRet->expSprite = CCSprite::createWithSpriteFrame(exp);
	}
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

bool ExpToolSprite::init()
{
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!Layer::init());

		this->initUI();

		bRet = true;

	} while (0);

	return bRet;
}

void ExpToolSprite::initUI()
{
	CC_SAFE_RETAIN(this->expBgSprite);
	CC_SAFE_RETAIN(this->expSprite);
	
	CCSize contentSize = this->expBgSprite->getContentSize();
	this->setContentSize(contentSize);

	expBgSprite->setAnchorPoint(Vec2::ZERO);
	expBgSprite->setPosition(Vec2::ZERO);
	this->addChild(expBgSprite);

	this->expSprite->setAnchorPoint(Vec2::ZERO);
	this->expSprite->setPosition(ccp(contentSize.width/2-this->expSprite->getContentSize().width/2,contentSize.height/2-this->expSprite->getContentSize().height/2));
	expBgSprite->addChild(this->expSprite,1);
}

void ExpToolSprite::updatePercent(float percent)
{
	this->expSprite->setScaleX(percent);
}

void ExpToolSprite::updatePercent(float percent,UNS_INT total,UNS_INT sub)
{
    this->expSprite->setScaleX(percent);
    
    CCString* str = CCString::createWithFormat("%d/%d",sub,total);
    CCLabelTTF* pTTF = CCLabelTTF::create(str->getCString(), "Arial", 15);
    pTTF->setAnchorPoint(ccp(0.5f,0.5f));
    pTTF->setPosition(ccp(this->getContentSize().width/2,this->getContentSize().height/2));
    expBgSprite->addChild(pTTF,2);
}


















