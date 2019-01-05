#include "RoomListLayer.h"
#include "RoomListScrollView.h"
#include "Define.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;


RoomListLayer::RoomListLayer()
{
	m_btnQuit = nullptr;
}

RoomListLayer::~RoomListLayer()
{
}

bool RoomListLayer::init(int idx)
{
	if (!Layer::init())
	{
		return false;
	}

	LayerColor* color = LayerColor::create(ccc4(0, 0, 0, 120),SCREEN_WIDTH,1080);
	color->setContentSize(cocos2d::Size(SCREEN_WIDTH,SCREEN_HEIGHT));
	this->addChild(color);
	this->setContentSize(color->getContentSize());
	
	//auto bg = ui::ImageView::create("BackGround_Big.png");
	auto bg = ui::ImageView::create("RoomListBg.png");
	bg->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(bg);

	auto txt=LabelTTF::create("","Arial",20);
	string str=String::createWithFormat("%.2lf",g_GlobalUnits.getGolbalUserData()->lScore)->getCString();
	txt->setString(str.c_str());
	txt->setColor(ccc3(255,255,255));
	txt->setPosition(Vec2(210,1078));
	txt->setHorizontalAlignment(TextHAlignment::LEFT);
	bg->addChild(txt);

	//m_btnQuit = ui::Button::create("btn_quit.png","","");
	//m_btnQuit->setPosition(Vec2(bg->getContentSize().width - m_btnQuit->getContentSize().width / 2 - 20, bg->getContentSize().height - m_btnQuit->getContentSize().height / 2 - 20));
	m_btnQuit = ui::Button::create("regist_04.png","","");
	m_btnQuit->setPosition(Vec2(34, bg->getContentSize().height - m_btnQuit->getContentSize().height / 2 - 60));
	m_btnQuit->addTouchEventListener(this,SEL_TouchEvent(&RoomListLayer::buttonQuit));
	bg->addChild(m_btnQuit, 100);
	
	m_ListView = RoomListScrollView::create(idx);
	//m_ListView->setPosition(Vec2(bg->getContentSize().width / 2 - list->getContentSize().width / 2, bg->getContentSize().height / 2 - list->getContentSize().height / 2 - 10));
	m_ListView->setPosition(Vec2(200,330));
	bg->addChild(m_ListView);

	return true;
}

void RoomListLayer::buttonQuit(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	this->removeFromParentAndCleanup(true);
}
