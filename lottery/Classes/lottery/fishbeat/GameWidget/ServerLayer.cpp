#include "ServerLayer.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"




USING_NS_CC;
using namespace cocostudio;
using namespace ui;
using namespace std;

ServerLayer::ServerLayer(void)
{
	 server_close = nullptr;
	 m_txtIns = nullptr;
}

ServerLayer::~ServerLayer(void)
{
	
}

ServerLayer * ServerLayer::create(float fScale)
{
	ServerLayer *ret = new ServerLayer();
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

bool ServerLayer::init(float fScale)
{
	LayerColor::initWithColor(Color4B(0,0,0,0));

	m_widgetUI = GUIReader::getInstance()->widgetFromJsonFile("PopWindow/zhongyou_help.json");
	this->addChild(m_widgetUI);
	this->setContentSize(m_widgetUI->getContentSize());

	m_panel	= (Layout*)Helper::seekWidgetByName(m_widgetUI,"panel");

	server_close	= (Button*)Helper::seekWidgetByName(m_widgetUI,"btn_quit");
	server_close->setTag(0);

	/*LabelTTF *label = LabelTTF::create("", "Arial", 25, cocos2d::Size(450, 200), TextHAlignment::LEFT, TextVAlignment::CENTER);
	this->addChild(label);
	label->setColor(Color3B::RED);
	label->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
	label->setString(CGameLobby::GetInstance()->readLanguageString("SeverSay")->getCString());*/

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//char szURL[MAX_PATH]={0};
	//TChar2Utf8(CGlobalUserInfo::GetInstance()->GetGlobalUserData()->szAgencyWebURL, szURL, MAX_PATH);
	//string url = StringUtils::format("%s/Navigation12.aspx",szURL);
	//m_WebNode = experimental::ui::WebView::create();
	//m_WebNode->loadURL(url);
	////m_WebNode->loadURL("http://www.baidu.com");
	//m_WebNode->setContentSize(cocos2d::Size(740,440));
	//m_WebNode->setPosition(Vec2(200,60));
	//m_WebNode->setAnchorPoint(Vec2::ZERO);
	//m_WebNode->setScalesPageToFit(true);
	//addChild(m_WebNode);

#endif

	//ÆÁ±Îµ×²ã´¥Ãþ
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch*,Event*)->bool
	{
		return true;
	};
	//listener->onTouchEnded = CC_CALLBACK_2(SystemLayer::onTouchEnded, this);
	listener->setSwallowTouches(true);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	this->setScale(0);
	this->runAction(Sequence::create(ScaleTo::create(0.3f,1.0f),CallFunc::create([&]{
		this->setOpacity(125);
	}),NULL));
	return true;
}

void ServerLayer::SetBtnCallBack(cocos2d::Ref* target,cocos2d::ui::SEL_TouchEvent selector)
{
	server_close->addTouchEventListener(target,selector);
}

void ServerLayer::btnCallback(cocos2d::Ref* pSender)
{
	MenuItemImage* pp = (MenuItemImage*)pSender;
	std::int32_t tag = pp->getTag();
	
}
void ServerLayer::DestroyWebView()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	this->removeChild(m_WebNode);
#endif
}
