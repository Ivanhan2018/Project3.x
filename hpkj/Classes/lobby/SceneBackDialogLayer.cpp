#include "SceneBackDialogLayer.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace std;

SceneBackDialogLayer::SceneBackDialogLayer(void)
{
	m_widgetUI = nullptr;
	server_close = nullptr;
	btn_go = nullptr;
	btn_leave = nullptr;
}

SceneBackDialogLayer::~SceneBackDialogLayer(void)
{

}

SceneBackDialogLayer * SceneBackDialogLayer::create(float fScale)
{
	SceneBackDialogLayer *ret = new SceneBackDialogLayer();
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

bool SceneBackDialogLayer::init(float fScale)
{
	if (!Layer::init())
	{
		return false;
	}

	//m_widgetUI = GUIReader::shareReader()->widgetFromJsonFile("SceneBackDialog/SceneBackDialog.ExportJson");
	m_widgetUI = dynamic_cast<Widget*>(CSLoader::getInstance()->createNodeFromJson("SceneBackDialog.ExportJson"));
	this->addChild(m_widgetUI);
	this->setContentSize(m_widgetUI->getContentSize());

	btn_go=dynamic_cast<Button*>(Helper::seekWidgetByName(m_widgetUI,"Button_confirm"));
	btn_go->setTag(1);
	btn_go->addTouchEventListener(this,SEL_TouchEvent(&SceneBackDialogLayer::OnCallBack));

	btn_leave=dynamic_cast<Button*>(Helper::seekWidgetByName(m_widgetUI,"Button_cancel"));
	btn_leave->setTag(2);
	btn_leave->addTouchEventListener(this,SEL_TouchEvent(&SceneBackDialogLayer::OnCallBack));

	server_close=dynamic_cast<Button*>(Helper::seekWidgetByName(m_widgetUI,"close_Button"));
	server_close->setTag(2);
	server_close->addTouchEventListener(this,SEL_TouchEvent(&SceneBackDialogLayer::OnCallBack));

	////ÆÁ±Îµ×²ã´¥Ãþ
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch*,Event*)->bool
	{
		return true;
	};
	listener->setSwallowTouches(true);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	return true;
}

void SceneBackDialogLayer::onEnter()
{
	Layer::onEnter();
}

void SceneBackDialogLayer::onExit()
{
	Layer::onExit();
}

void SceneBackDialogLayer::OnCallBack(cocos2d::CCObject* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	int tag = dynamic_cast<Button*>(sender)->getTag();
	switch (tag)
	{
	case 1:
		{
			this->removeFromParentAndCleanup(true);
		}break;
	case 2:
		{
			this->removeFromParentAndCleanup(true);
		}break;
	default:
		break;
	}
}