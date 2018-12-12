#include "MovingLabelLayer.h"


MovingLabelLayer * MovingLabelLayer::MovingLabelLayerWith(const char* str,Vec2 pos)
{
    MovingLabelLayer * match = new MovingLabelLayer();
    if (match && match->initMovingLabelLayer(str,pos))
    {
        match->autorelease();
        return match;
    }
    CC_SAFE_DELETE(match);
    return match;
}

MovingLabelLayer::MovingLabelLayer()
{
    
}

MovingLabelLayer::~MovingLabelLayer()
{
    
}

bool MovingLabelLayer::initMovingLabelLayer(const char* str,Vec2 pos)
{
    if(!Node::init())
    {
        return false;
    }
    cocos2d::Size winSize = Director::getInstance()->getWinSize();

	Sprite* spriteBack0 = Sprite::createWithSpriteFrame(spriteFrame("moving_label_001.png"));
	Sprite* spriteBack1 = Sprite::createWithSpriteFrame(spriteFrame("moving_label_001.png"));
	spriteBack0->setPosition(Vec2(pos.x - spriteBack0->getContentSize().width * 0.5,pos.y));
	spriteBack1->setPosition(Vec2(pos.x + spriteBack1->getContentSize().width * 0.5,pos.y));
	spriteBack1->setFlipX(true);
	this->addChild(spriteBack0);
	this->addChild(spriteBack1);
	LabelTTF* label = LabelTTF::create(str,"",22);
	label->setColor(ccc3(255,255,0));
	label->setPosition(pos);
	this->addChild(label);

	MoveTo* moveto0 = MoveTo::create(0.2f,Vec2(spriteBack0->getPositionX(),spriteBack0->getPositionY() + 10));
	MoveTo* moveto1 = MoveTo::create(0.2f,Vec2(spriteBack1->getPositionX(),spriteBack1->getPositionY() + 10));
	MoveTo* moveto2 = MoveTo::create(0.2f,Vec2(label->getPositionX(),label->getPositionY() + 10));
	DelayTime* delay0 = DelayTime::create(2.0f);
	DelayTime* delay1 = DelayTime::create(2.0f);
	DelayTime* delay2 = DelayTime::create(2.0f);
	CallFuncN* call0 = CallFuncN::create(this,callfuncN_selector(MovingLabelLayer::onActionDown));
	spriteBack0->runAction(Sequence::create(moveto0,delay0,call0,NULL));
	spriteBack1->runAction(Sequence::create(moveto1,delay1,NULL));
	label->runAction(Sequence::create(moveto2,delay2,NULL));

    return true;
}

void MovingLabelLayer::onActionDown(Node* obj)
{
	this->removeFromParentAndCleanup(true);
}