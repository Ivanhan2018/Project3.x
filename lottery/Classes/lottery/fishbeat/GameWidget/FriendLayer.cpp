#include "FriendLayer.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace std;

FriendLayer::FriendLayer(void)
{
	server_close = nullptr;
}

FriendLayer::~FriendLayer(void)
{

}

FriendLayer * FriendLayer::create(float fScale)
{
	FriendLayer *ret = new FriendLayer();
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

bool FriendLayer::init(float fScale)
{
	LayerColor::initWithColor(Color4B(0,0,0,0));

	m_widgetUI = GUIReader::getInstance()->widgetFromJsonFile("PopWindow/zhongyou_friend.json");
	this->addChild(m_widgetUI);
// 	this->setContentSize(m_widgetUI->getContentSize());

	m_panel	= (Layout*)Helper::seekWidgetByName(m_widgetUI,"panel");

	m_btnMyFriend	= (Button*)Helper::seekWidgetByName(m_widgetUI,"btn_myF");
	m_btnAddFriend	= (Button*)Helper::seekWidgetByName(m_widgetUI,"btn_addF");
	m_btnAddGroup	= (Button*)Helper::seekWidgetByName(m_widgetUI,"btn_addG");
	m_btnCreateGroup	= (Button*)Helper::seekWidgetByName(m_widgetUI,"btn_createG");



	server_close	= (Button*)Helper::seekWidgetByName(m_widgetUI,"btn_quit");
	server_close->setTag(0);


	m_btnMyFriend->addTouchEventListener(this, toucheventselector(FriendLayer::onMenuSelect));
	m_btnAddFriend->addTouchEventListener(this, toucheventselector(FriendLayer::onMenuSelect));
	m_btnAddGroup->addTouchEventListener(this, toucheventselector(FriendLayer::onMenuSelect));
	m_btnCreateGroup->addTouchEventListener(this, toucheventselector(FriendLayer::onMenuSelect));


	//ÆÁ±Îµ×²ã´¥Ãş
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch*,Event*)->bool
	{
		return true;
	};
	////listener->onTouchEnded = CC_CALLBACK_2(SystemLayer::onTouchEnded, this);
	listener->setSwallowTouches(true);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	this->setScale(0);
	this->runAction(Sequence::create(ScaleTo::create(0.3f,fScale),CallFunc::create([&]{
		this->setOpacity(125);
	}),NULL));
	return true;
}

void FriendLayer::SetBtnCallBack(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector)
{
	server_close->addTouchEventListener(target,selector);
}



void FriendLayer::setMenuMode(Friend_Menu mode)
{

	switch (mode)
	{
	case FriendLayer::Menu_MF:
		{
			m_btnMyFriend->setBright(false);
			m_btnAddFriend->setBright(true);
			m_btnAddGroup->setBright(true);
			m_btnCreateGroup->setBright(true);

		}
		break;
	case FriendLayer::Menu_AF:
		{
			m_btnMyFriend->setBright(true);
			m_btnAddFriend->setBright(false);
			m_btnAddGroup->setBright(true);
			m_btnCreateGroup->setBright(true);

		}
		break;
	case FriendLayer::Menu_AG:
		{
			m_btnMyFriend->setBright(true);
			m_btnAddFriend->setBright(true);
			m_btnAddGroup->setBright(false);
			m_btnCreateGroup->setBright(true);

		}
		break;
	case FriendLayer::Menu_CG:
		{
			m_btnMyFriend->setBright(true);
			m_btnAddFriend->setBright(true);
			m_btnAddGroup->setBright(true);
			m_btnCreateGroup->setBright(false);

		}
		break;
	default:
		break;
	}


}

void FriendLayer::onMenuSelect(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
	{
		return;
	}

	Button* btn = (Button*)sender;

	switch (btn->getTag())
	{
	case 65:
		{
			setMenuMode(Menu_MF);
		}
		break;

	case 66:
		{
			setMenuMode(Menu_AF);

		}
		break;

	case 68:
		{
			setMenuMode(Menu_AG);

		}
		break;

	case 69:
		{
			setMenuMode(Menu_CG);

		}
		break;

	default:
		break;
	}

}
