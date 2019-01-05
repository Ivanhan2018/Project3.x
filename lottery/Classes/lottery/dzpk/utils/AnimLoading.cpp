#include "AnimLoading.h"
#include "StartScene.h"

AnimLoading::AnimLoading():
pClip(NULL),
pLineFrame(NULL),
pScaleTTF(NULL),
currScale(0.0f),
moveStep(0.0f),
maxMoveCount(100),
pauseMoveCount(95),
isPause(true),
netConnTime(5.0f),
currNetConnTime(0.0f)
{
    
}

AnimLoading::~AnimLoading()
{
    CC_SAFE_RELEASE_NULL(pClip);
    CC_SAFE_RELEASE_NULL(pLineFrame);
    CC_SAFE_RELEASE_NULL(pScaleTTF);
}

bool AnimLoading::init()
{
    bool bRef = false;
    
    do{
        CC_BREAK_IF(!PopupNode::init());
        
        this->initUI();
        
        bRef = true;
    }while(0);
    
    return bRef;
}

void AnimLoading::initUI()
{
	this->setBgColorOpacity(50);
    
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pLineFrame = CCSprite::createWithSpriteFrame(frameCache->spriteFrameByName("line_frame.png"));
    CC_SAFE_RETAIN(pLineFrame);
    lineFrameSize = pLineFrame->getContentSize();

    pClip = ClipLayer::create(lineFrameSize);
    CC_SAFE_RETAIN(pClip);
    pClip->setAnchorPoint(Vec2::ZERO);
    pClip->setPosition(ccp(261-lineFrameSize.width,-1));
    this->addChild(pClip);
    
    pLineFrame->setAnchorPoint(Vec2::ZERO);
    pLineFrame->setPosition(ccp(lineFrameSize.width,0));
    pClip->addChild(pLineFrame);
    
    pScaleTTF = CCLabelTTF::create("0%", "Arial", 20);
    CC_SAFE_RETAIN(pScaleTTF);
    pScaleTTF->setAnchorPoint(Vec2::ZERO);
    pScaleTTF->setPosition(ccp(261+lineFrameSize.width/2-pScaleTTF->getContentSize().width/2,lineFrameSize.height/2-pScaleTTF->getContentSize().height/2));
    this->addChild(pScaleTTF);
    
    moveStep = lineFrameSize.width/maxMoveCount;
    
    //使下一层的触摸无效
    CCSprite* normalSprite = CCSprite::create("common/transparent_bg.png");
	CCSprite* selectedSprite = CCSprite::create("common/transparent_bg.png");
    normalSprite->setContentSize(CCSizeMake(this->visibleW, this->visibleH));
    selectedSprite->setContentSize(CCSizeMake(this->visibleW, this->visibleH));
	CCMenuItemSprite* menuItem = CCMenuItemSprite::create(normalSprite,selectedSprite);
	menuItem->setAnchorPoint(Vec2::ZERO);
	menuItem->setPosition(Vec2::ZERO);
    
    CCMenu* menu = CCMenu::create(menuItem,NULL);
	menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu,200);
}

void AnimLoading::onEnterTransitionDidFinish()
{
    float duration = 0.02f;
    this->schedule(schedule_selector(AnimLoading::moveCallback), duration);
    
    currNetConnTime = 0.0f;
    this->schedule(schedule_selector(AnimLoading::netConnTimeCallback), 1.0f);
}

void AnimLoading::move(float& differX)
{
    pClip->setAnchorPoint(Vec2::ZERO);
    pClip->setPositionX(pClip->getPositionX()+differX);
    
    pLineFrame->setAnchorPoint(Vec2::ZERO);
    pLineFrame->setPositionX(pLineFrame->getPositionX()-differX);
    
    pScaleTTF->setAnchorPoint(Vec2::ZERO);
    pScaleTTF->setString(CCString::createWithFormat("%.0f%s",currScale,"%")->getCString());
}

void AnimLoading::setResumeUpdate()
{
    this->isPause = false;
}

void AnimLoading::netConnTimeCallback(float dt)
{
    currNetConnTime += 1.0f;
    
    if (currNetConnTime>netConnTime)
    {
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        showToast("当前的网络状况较差哦，请切换到WIFI或更好的网络环境!");
        #endif
    }
}

void AnimLoading::stopUpdate()
{
    this->unschedule(schedule_selector(AnimLoading::moveCallback));
    this->unschedule(schedule_selector(AnimLoading::netConnTimeCallback));
}

void AnimLoading::moveCallback(float dt)
{
    if (this->currScale>=pauseMoveCount && isPause)
    {
        return;
    }
    
    this->currScale += 1.0f;
    
    if (this->currScale>=maxMoveCount)
    {
        this->currScale = 100.0f;
        this->unschedule(schedule_selector(AnimLoading::moveCallback));
        this->scheduleOnce(schedule_selector(AnimLoading::removeMyselfCallback), 0.0f);
    }
    this->move(moveStep);
}

void AnimLoading::removeMyselfCallback(float dt)
{
    ((StartScene*)this->getParent())->showUserInfo();
    this->removeFromParentAndCleanup(true);
}

