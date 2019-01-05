//
//  RoomRule.cpp
//  gamePoker
//
//  Created by xavier on 13-11-28.
//
//

#include "RoomRule.h"
#include "Sound.h"

bool RoomRule::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite *info_bg = CCSprite::create("rooms/frame/card_rule_bk.png");
    info_bg->setAnchorPoint(ccp(1,1));
    info_bg->setPosition(ccp(0,winSize.height));
    this->addChild(info_bg);

    size = info_bg->getContentSize();
    info_bg->runAction(CCMoveBy::create(0.3f, ccp(size.width,0)));
    
    this->setTouchEnabled(true);
    return true;
}

//void RoomRule::registerWithTouchDispatcher()
//{
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
//}

bool RoomRule::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    Sound::getInstance()->playEffect("sound/sound_guanbi.ogg");
    
	CCMoveBy *by = CCMoveBy::create(0.3f,ccp(-size.width,0));
    CCCallFunc *cf = CCCallFunc::create(this, callfunc_selector(RoomRule::onTouchCallback));
	this->runAction(CCSequence::create(by, cf, NULL));
    return true;
}

void RoomRule::onTouchCallback()
{
	this->getParent()->removeChildByTag(200,true);
}