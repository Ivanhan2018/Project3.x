#include "CharityLeaveLayer.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "ByMenuLayer.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace std;

CharityLeaveLayer::CharityLeaveLayer(void)
{
	m_widgetUI = nullptr;
	server_close = nullptr;
	btn_go = nullptr;
	btn_leave = nullptr;
}

CharityLeaveLayer::~CharityLeaveLayer(void)
{

}

CharityLeaveLayer * CharityLeaveLayer::create(float fScale)
{
	CharityLeaveLayer *ret = new CharityLeaveLayer();
	if (ret && ret->init(fScale))
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

bool CharityLeaveLayer::init(float fScale)
{
	if (!Layer::init())
	{
		return false;
	}
	m_widgetUI = GUIReader::getInstance()->widgetFromJsonFile("709/CharityLeave/CharityLeave.ExportJson");
	this->addChild(m_widgetUI);
	this->setContentSize(m_widgetUI->getContentSize());

	btn_go	= (Button*)Helper::seekWidgetByName(m_widgetUI,"btn_go");
	btn_go->setTag(1);
	btn_go->addTouchEventListener(this,SEL_TouchEvent(&CharityLeaveLayer::OnCallBack));

	btn_leave	= (Button*)Helper::seekWidgetByName(m_widgetUI,"btn_leave");
	btn_leave->setTag(2);
	btn_leave->addTouchEventListener(this,SEL_TouchEvent(&CharityLeaveLayer::OnCallBack));

	server_close	= (Button*)Helper::seekWidgetByName(m_widgetUI,"btn_quit");
	server_close->setTag(2);
	server_close->addTouchEventListener(this,SEL_TouchEvent(&CharityLeaveLayer::OnCallBack));

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

void CharityLeaveLayer::OnCallBack(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	int tag = dynamic_cast<Button*>(sender)->getTag();
	switch (tag)
	{
	case 1:
		{
			this->removeFromParentAndCleanup(true);
			//³äÖµ
			ByMenuLayer::GetInstance()->onCharge(nullptr);
		}break;
	case 2:
		{
			this->removeFromParentAndCleanup(true);
			//ÍË³ö
			ByMenuLayer::GetInstance()->onQuitGame(nullptr);
		}break;
	default:
		break;
	}
}