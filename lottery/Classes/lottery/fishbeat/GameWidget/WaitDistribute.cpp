#include "WaitDistribute.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "Define.h"


USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace std;

WaitDistribute::WaitDistribute(void)
{
	
}

WaitDistribute::~WaitDistribute(void)
{
}
bool WaitDistribute::init()
{
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setContentSize(cocos2d::Size(SCREEN_HEIGHT,SCREEN_WIDTH));
	LayerColor::initWithColor(Color4B(125,125,125,125));


	Sprite* waitDistribute = Sprite::create("WaitDistribute/waitdistribute_ing.png");
	waitDistribute->setPosition(Vec2((SCREEN_HEIGHT - waitDistribute->getContentSize().width) / 2, (SCREEN_WIDTH - waitDistribute->getContentSize().height) / 2));
	waitDistribute->setAnchorPoint(Vec2::ZERO);
	this->addChild(waitDistribute,2);

	auto close = Button::create("WaitDistribute/waitdistribute_close.png");
	close->setPosition(Vec2((SCREEN_HEIGHT - close->getContentSize().width) / 2, waitDistribute->getPositionY()-100));
	close->setAnchorPoint(Vec2::ZERO);
	close->addTouchEventListener(this,SEL_TouchEvent(&WaitDistribute::OnBtnCallBack));
	this->addChild(close,1);
	//ÆÁ±Îµ×²ã´¥Ãþ
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch*,Event*)->bool
	{
		return true;
	};
	listener->setSwallowTouches(true);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	return true;
}

void WaitDistribute::OnBtnCallBack(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	//GameRoom::GetInstance()->GetIGameServerItem()->CloseGameRoom();
	deleteView();
}

void WaitDistribute::deleteView()
{
	this->removeFromParentAndCleanup(true);
}