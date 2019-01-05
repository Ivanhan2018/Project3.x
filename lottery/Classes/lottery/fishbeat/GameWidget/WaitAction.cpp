#include "WaitAction.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"


#include "VisibleRect.h"


USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace std;


WaitAction* WaitAction::m_waitAction=nullptr;
WaitAction::WaitAction(void)
{
	m_waitAction = this;
}

WaitAction::~WaitAction(void)
{
	m_waitAction->stopAllActions();
	m_waitAction = nullptr;
}
WaitAction* WaitAction::create()
{
	WaitAction *ret = new WaitAction();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}
bool WaitAction::init()
{
	if(!Layer::init())
		return false;
	//LayerColor::initWithColor(Color4B(125, 136, 155, 125));
	this->setContentSize(cocos2d::Size(1136,640));
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	Sprite* bg = Sprite::create("loadingbg.png");
	this->addChild(bg,1);
	bg->setPosition(VisibleRect::center());
	bg->setPositionY(bg->getPositionY()-10);
	Sprite* action = Sprite::create("chendashen.png");
	this->addChild(action,1);
	action->setPosition(VisibleRect::center());
	action->runAction(RepeatForever::create(RotateBy::create(0.6f, 360)));

	auto waitActionTip = Text::create("mybaba","SimHei",26);
	waitActionTip->setPositionX(action->getPositionX());
	waitActionTip->setPositionY(action->getPositionY()-60);
	waitActionTip->setColor(Color3B(255,255,255));
	this->addChild(waitActionTip,2);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch*,Event*)->bool
	{
		return true;
	};
	listener->setSwallowTouches(true);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	return true;
}