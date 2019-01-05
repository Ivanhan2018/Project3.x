//
//  SwitchSprite.cpp
//  TexasPoker
//
//  Created by peniel on 1/3/14.
//
//

#include "SwitchSprite.h"

SwitchSprite::SwitchSprite():
pMenu(NULL),
m_callbackListener(NULL),
m_callback(NULL),
pOnSprite(NULL),
pOffSprite(NULL),
pThumbSprite(NULL),
pOnTTF(NULL),
pOffTTF(NULL),
status(false),
isSwitchAnimFinish(true)
{

}

SwitchSprite::~SwitchSprite()
{
    CC_SAFE_RELEASE_NULL(pOnSprite);
    CC_SAFE_RELEASE_NULL(pOffSprite);
    CC_SAFE_RELEASE_NULL(pThumbSprite);
    CC_SAFE_RELEASE_NULL(pOnTTF);
    CC_SAFE_RELEASE_NULL(pOffTTF);
    CC_SAFE_RELEASE_NULL(pMenu);
}

SwitchSprite* SwitchSprite::createWith(const char* on,const char* off,const char* thumb)
{
    CCSprite* pOnSprite = CCSprite::create(on);
    CCSprite* pOffSprite = CCSprite::create(off);
    CCSprite* pThumbSprite = CCSprite::create(thumb);
    return createWith(pOnSprite,pOffSprite,pThumbSprite);
}

SwitchSprite* SwitchSprite::createWith(CCSprite* pOnSprite,CCSprite* pOffSprite,CCSprite* pThumbSprite)
{
    SwitchSprite *pRet = new SwitchSprite();
    if (pRet)
    {
        pRet->pOnSprite = pOnSprite;
        pRet->pOffSprite = pOffSprite;
        pRet->pThumbSprite = pThumbSprite;
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

bool SwitchSprite::init()
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

void SwitchSprite::setCallbackFunc(cocos2d::CCObject *target, SEL_CallFuncN callfun)
{
    m_callbackListener = target;
    m_callback = callfun;
}

void SwitchSprite::initUI()
{
    switchSize = pOnSprite->getContentSize();
    this->setContentSize(switchSize);
    
    CC_SAFE_RETAIN(pOnSprite);
    CC_SAFE_RETAIN(pOffSprite);
    CC_SAFE_RETAIN(pThumbSprite);
    
    pOnSprite->setAnchorPoint(Vec2::ZERO);
    pOnSprite->setPosition(Vec2::ZERO);
    this->addChild(pOnSprite,1);
    
    pOffSprite->setAnchorPoint(Vec2::ZERO);
    pOffSprite->setPosition(Vec2::ZERO);
    this->addChild(pOffSprite,2);
    
    //pOnTTF = CCLabelTTF::create("开", "Arial", 18);
    //CC_SAFE_RETAIN(pOnTTF);
    //pOnTTF->setColor(ccWHITE);
    //pOnTTF->setAnchorPoint(Vec2::ZERO);
    //pOnTTF->setPosition(ccp(pThumbSprite->getContentSize().width/2-pOnTTF->getContentSize().width/2,switchSize.height/2-pOnTTF->getContentSize().height/2));
    //this->addChild(pOnTTF,3);
    //
    //pOffTTF = CCLabelTTF::create("关", "Arial", 18);
    //CC_SAFE_RETAIN(pOffTTF);
    //pOffTTF->setColor(ccBLACK);
    //pOffTTF->setAnchorPoint(ccp(1.0f,0.0f));
    //pOffTTF->setPosition(ccp(switchSize.width-(pThumbSprite->getContentSize().width/2-pOnTTF->getContentSize().width/2),switchSize.height/2-pOnTTF->getContentSize().height/2));
    //this->addChild(pOffTTF,3);
    
    pThumbSprite->setAnchorPoint(Vec2::ZERO);
    pThumbSprite->setPosition(ccp(0,switchSize.height/2-pThumbSprite->getContentSize().height/2-1));
    this->addChild(pThumbSprite,4);
    
    // 初始化需要的 Menu
	pMenu = CCMenu::create();
    CC_SAFE_RETAIN(pMenu);
	pMenu->setPosition(Vec2::ZERO);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu,5);
}

const bool SwitchSprite::isOn() const
{
    return status;
}

void SwitchSprite::setSwitchStatus(bool onOrOff)
{
    status = onOrOff;
    
    if (status)
    {
        pThumbSprite->setPosition(CCPointMake(switchSize.width-pThumbSprite->getContentSize().width,switchSize.height/2-pThumbSprite->getContentSize().height/2-1));
        pOnSprite->setZOrder(2);
        pOffSprite->setZOrder(1);
    }
    else
    {
        pThumbSprite->setPosition(CCPointMake(0,switchSize.height/2-pThumbSprite->getContentSize().height/2-1));
        pOnSprite->setZOrder(1);
        pOffSprite->setZOrder(2);
    }
}

bool SwitchSprite::addMenu(const char *normalImage, const char *selectedImage,int tag)
{
	CCSprite* normalSprite = CCSprite::create(normalImage);
	CCSprite* selectedSprite = CCSprite::create(selectedImage);

    normalSprite->setContentSize(switchSize);
    selectedSprite->setContentSize(switchSize);
    
	CCMenuItemSprite* menuItem = CCMenuItemSprite::create(normalSprite,selectedSprite,CC_CALLBACK_1(SwitchSprite::menuCallback,this));
    
	menuItem->setTag(tag);
	menuItem->setAnchorPoint(Vec2::ZERO);
	menuItem->setPosition(Vec2::ZERO);
	this->pMenu->addChild(menuItem);
    
	return true;
}

void SwitchSprite::menuCallback(CCObject* pSender)
{
    if(!isSwitchAnimFinish)
    {
        return;
    }
    isSwitchAnimFinish = false;
    this->scheduleOnce(schedule_selector(SwitchSprite::switchAnimFinishCallback), 0.5f);
    
    if (status)
    {
        pThumbSprite->runAction(CCMoveTo::create(0.2f, CCPointMake(0,switchSize.height/2-pThumbSprite->getContentSize().height/2-1)));
        
        pOnSprite->setZOrder(1);
        pOffSprite->setZOrder(2);
        
        status = false;
    }
    else
    {
        pThumbSprite->runAction(CCMoveTo::create(0.2f, CCPointMake(switchSize.width-pThumbSprite->getContentSize().width,switchSize.height/2-pThumbSprite->getContentSize().height/2-1)));
        
        pOnSprite->setZOrder(2);
        pOffSprite->setZOrder(1);
        
        status = true;
    }
    
    CCNode* node = dynamic_cast<CCNode*>(pSender);
    //DLog::showLog("touch tag: %d", node->getTag());
    if (m_callback && m_callbackListener)
    {
        (m_callbackListener->*m_callback)(node);
    }
    
}

void SwitchSprite::switchAnimFinishCallback(float dt)
{
    isSwitchAnimFinish = true;
}













