//
//  MatchError.cpp
//  Game
//
//  Created by 罗敏 on 13-7-10.
//
//

#include "MatchError.h"
#include <limits.h>
#include "EntityMgr.h"
#include "RoomFrame.h"
#include "ConfigMgr.h"
MatchError * MatchError::MatchErrorWith()
{
    MatchError * match = new MatchError();
    if (match && match->initMatchError())
    {
        match->autorelease();
        return match;
    }
    CC_SAFE_DELETE(match);
    return match;
}

MatchError::MatchError()
{
    
}

MatchError::~MatchError()
{
    
}

bool MatchError::initMatchError()
{
    if(!Layer::init())
    {
        return false;
    }
    
    cocos2d::Size winSize = Director::getInstance()->getWinSize();
    Sprite* back = Sprite::createWithSpriteFrame(spriteFrame("ui_tishiback.png"));
    back->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.5));
    addChild(back);
    
    Sprite* spriteTips = Sprite::createWithSpriteFrame(spriteFrame("match_tipsicon.png"));
    spriteTips->setPosition(Vec2(winSize.width * 0.5 - back->getContentSize().width * 0.5 + spriteTips->getContentSize().width * 0.48,back->getContentSize().height * 0.5 + winSize.height * 0.5));
    addChild(spriteTips);
    
    Sprite* spriteMtachTips = Sprite::createWithSpriteFrame(spriteFrame("match_waittips.png"));
    spriteMtachTips->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.45));
    addChild(spriteMtachTips);
    LabelTTF* matchLabelTips = LabelTTF::create(ConfigMgr::instance()->text("t76"), "HelveticaNeue-Bold", 24, cocos2d::Size(380, 70), kCCTextAlignmentCenter);
    matchLabelTips->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.65));
    addChild(matchLabelTips);

    Sprite * backNormal = Sprite::createWithSpriteFrame(spriteFrame("ui_guanb0.png"));
    Sprite * backSelect = Sprite::createWithSpriteFrame(spriteFrame("ui_guanb1.png"));
    MenuItemSprite * backItemSprite = MenuItemSprite::create(backNormal, backSelect, CC_CALLBACK_1(MatchError::closeMatchErrorLayer, this));
    
    m_pBackMenu = Menu::create(backItemSprite,NULL);
    m_pBackMenu->setPosition(Vec2(back->getContentSize().width * 0.5 +  winSize.width * 0.5,back->getContentSize().height * 0.5 + winSize.height * 0.5));
    addChild(m_pBackMenu);
    
    Sprite* yesNormal = Sprite::createWithSpriteFrame(spriteFrame("match_yes0.png"));
    Sprite* yesSelect = Sprite::createWithSpriteFrame(spriteFrame("match_yes1.png"));
    MenuItemSprite* yesItemSprite = MenuItemSprite::create(yesNormal, yesSelect, CC_CALLBACK_1(MatchError::agreeMatch, this));
    
    Sprite* noNormal = Sprite::createWithSpriteFrame(spriteFrame("match_no0.png"));
    Sprite* noSelect = Sprite::createWithSpriteFrame(spriteFrame("match_no1.png"));
    MenuItemSprite* noItemSprite = MenuItemSprite::create(noNormal, noSelect,CC_CALLBACK_1(MatchError::refuseMatch,this));
    
    m_pSelectMenu = Menu::create(yesItemSprite,noItemSprite,NULL);
    m_pSelectMenu->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.35));
    m_pSelectMenu->alignItemsHorizontallyWithPadding(170);
    addChild(m_pSelectMenu);
    
	// 创建一个事件监听器类型为OneByOne的单点触摸
	auto listener1 = EventListenerTouchOneByOne::create();

	// 设置是否吞没事件，在onTouchBegan方法返回true时吞没
	//listener1->setSwallowTouches(true);
	listener1->onTouchBegan = CC_CALLBACK_2(MatchError::onTouchBegan, this);
	listener1->onTouchMoved = CC_CALLBACK_2(MatchError::onTouchMoved, this);
	listener1->onTouchEnded = CC_CALLBACK_2(MatchError::onTouchEnded, this);
	listener1->onTouchCancelled = CC_CALLBACK_2(MatchError::onTouchCancelled,this);

	// 添加监听器
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
	

    return true;
}

void MatchError::onEnter()
{
    //Layer::onEnter();
    //registerWithTouchDispatcher();
}
void MatchError::onExit()
{
    //Layer::onExit();
//    Director::getInstance()->getEventDispatcher()->removeEventListener(this);
}

//void MatchError::registerWithTouchDispatcher()
//{
//    //Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,(std::numeric_limits<int>::min)(),true);
//   // Layer::registerWithTouchDispatcher();
//
//	// Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,1000,true);
//}

bool MatchError::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    bBackMenu = m_pBackMenu->onTouchBegan(pTouch, pEvent);
    bSelectMenu = m_pSelectMenu->onTouchBegan(pTouch, pEvent);
    return true;
}

void MatchError::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    if(bBackMenu)
    {
        m_pBackMenu->onTouchMoved(pTouch, pEvent);
    }
    if(bSelectMenu)
    {
        m_pSelectMenu->onTouchMoved(pTouch, pEvent);
    }
}

void MatchError::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    if(bBackMenu)
    {
        m_pBackMenu->onTouchEnded(pTouch, pEvent);
    }
    if(bSelectMenu)
    {
        m_pSelectMenu->onTouchEnded(pTouch, pEvent);
    }
}

void MatchError::onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    if(bBackMenu)
    {
        m_pBackMenu->onTouchCancelled(pTouch, pEvent);
    }
    if(bSelectMenu)
    {
        m_pSelectMenu->onTouchCancelled(pTouch, pEvent);
    }
}

void MatchError::closeMatchErrorLayer(Object* obj)
{
	 EntityMgr::instance()->getDispatch()->sendQueuePacket(2);
    removeFromParentAndCleanup(true);;
}

void MatchError::agreeMatch(Object* obj)
{
    playButtonSound();
    EntityMgr::instance()->getDispatch()->sendQueuePacket(2);
    removeFromParentAndCleanup(true);
}

void MatchError::refuseMatch(Object* obj)
{
    playButtonSound();
    EntityMgr::instance()->getDispatch()->sendQueuePacket(1);
    removeFromParentAndCleanup(true);
}