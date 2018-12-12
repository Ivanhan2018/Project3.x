//
//  SelectButton.cpp
//  Game
//
//  Created by �� ���� on 13-6-7.
//
//

#include "ActivitySprite.h"
#include "ActivityLayer.h"

ActivitySprite * ActivitySprite::ActivitySpriteWith(int type)
{
    ActivitySprite * button = new ActivitySprite();
	int actuvityNum = 100+type;
	if(actuvityNum > 101)
	{
		actuvityNum = 99;
	}
	String* str = String::createWithFormat("dt_activity_%d.jpg",actuvityNum);
    if(button && button->initWithSpriteFrame(spriteFrame(str->getCString())))
    {
		button->initActivitySprite(type);
        button->autorelease();
        return button;
    }
    CC_SAFE_DELETE(button);
    return NULL;
}
bool ActivitySprite::initActivitySprite(int type)
{
	m_nType = type;

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(ActivitySprite::onTouchBegan, this);
	listener->onTouchCancelled = CC_CALLBACK_2(ActivitySprite::onTouchCancelled, this);
	listener->onTouchEnded = CC_CALLBACK_2(ActivitySprite::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(ActivitySprite::onTouchMoved, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
ActivitySprite::ActivitySprite()
{

}
ActivitySprite::~ActivitySprite()
{
    
}

void ActivitySprite::setSelect(bool select)
{
	if(select)
	{
		this->setScale(1.0);
	}
	else
	{
		this->setScale(0.8);
	}
}

void ActivitySprite::onEnter()
{
    Sprite::onEnter();
    //Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);

}
void ActivitySprite::onExit()
{
    Sprite::onExit();
    
}
bool ActivitySprite::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	Vec2 touchPoint = pTouch->getLocationInView();
	touchPoint = Director::getInstance()->convertToGL(touchPoint);
	if(this->boundingBox().containsPoint(touchPoint))
	{
		ActivityLayer* layer = (ActivityLayer*)this->getParent();
		layer->changeActivity(m_nType);
		return false;
	}
	return false;
}
void ActivitySprite::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}
void ActivitySprite::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    
}
void ActivitySprite::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    
}
