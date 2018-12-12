#include "MyRoomLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ui/UIHelper.h"
#include "ui/UIButton.h"
#include "ui/UIWidget.h"
#include "cocostudio/CCArmature.h"
#include"SimpleAudioEngine.h"
#include "ui/UIImageView.h"

using namespace CocosDenshion;
using namespace cocos2d::ui;

USING_NS_CC;

using namespace cocostudio::timeline;
CCSprite* MyRoomLayer::FanliLayer;
int MyRoomLayer::MyOutCardCount = 0;
int MyRoomLayer::RightOutCardCount = 0;
int MyRoomLayer::UpOutCardCount = 0;
int MyRoomLayer::LeftOutCardCount = 0;
std::vector<int> 			    MyRoomLayer::JoinRoomNumber;
std::vector<CCSprite*> 			MyRoomLayer::JoinRoomNumberSprite;
MyRoomLayer* MyRoomLayer::s_pMyRoomLayer = NULL;

Scene* MyRoomLayer::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MyRoomLayer::create();
	s_pMyRoomLayer = layer;
	// add layer as a child to scene
	scene->addChild( layer, 0, "root");

	// return the scene
	return scene;
}
void string_replace(std::string &strBig, const std::string &strsrc, const std::string &strdst)
{
	std::string::size_type pos = 0;
	std::string::size_type srclen = strsrc.size();
	std::string::size_type dstlen = strdst.size();

	while ((pos = strBig.find(strsrc, pos)) != std::string::npos)
	{
		strBig.replace(pos, srclen, strdst);
		pos += dstlen;
	}
}
// on "init" you need to initialize your instance
bool MyRoomLayer::init()
{

	if (!Layer::init())
	{
		return false;
	}
	

	Widget* Root = static_cast<Widget*>(CSLoader::createNode("Hall.csb"));
	addChild(Root, 0, "box1");

	MyRoomLayerRoot = static_cast<Widget*>(Root->getChildByName("ditu"));
	OnGuiXiMaJiang();

	Size size = Director::getInstance()->getWinSize();
	Root->setScale(size.width/ MyRoomLayerRoot->getContentSize().width, size.height/ MyRoomLayerRoot->getContentSize().height);

	CCSprite* sy_jsk_20_0_0 = static_cast<CCSprite*>(MyRoomLayerRoot->getChildByName("sy_jsk_20_0_0"));
	Text* roomCardsNum = static_cast<Text*>(sy_jsk_20_0_0->getChildByName("roomCardsNum"));
	
	unsigned int CARD = 1;
	char CARDbuf[64] = "";
	sprintf(CARDbuf, "%d", CARD);

	roomCardsNum->setString(CARDbuf);
	Button* fksx = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_8"));
	fksx->setTag(0);
	fksx->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuFKSXCallBack, this));

	Button* pButton_11 = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_11"));
	pButton_11->setTag(0);
	pButton_11->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuToMilitary, this));

	Button* pButton_43 = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_43"));
	pButton_43->setTag(0);
	pButton_43->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuExplain, this));

	Button* pButton_44 = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_44"));
	pButton_44->setTag(1);
	pButton_44->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuExplain, this));

	isMusicOpen = false;
	isVoiceOpen = false;

	m_isBulletinBoard = false;
	setCloseLayer	();
	setFankuiLayer	();
	setShezhiLayer	();
	setKefuLayer	();
	setFangkaLayer	();
	setShengmingLayer();
	setHeadLayer	();
	setWanfaLayer   ();
	setCreateRoom	();
	setJoinRoom		();

	cocos2d::Node* pNodBulletine= MyRoomLayerRoot->getChildByName("Bulletin");
	cocos2d::ui::Text* pBulletineText = (cocos2d::ui::Text*)pNodBulletine->getChildByName("BulletinText");
	scheduleUpdate();
	DongDaiZhuang(0);
	return true;
}

void MyRoomLayer::setCloseLayer()
{
	CCSprite* ExitLayer = static_cast<CCSprite*>(MyRoomLayerRoot->getChildByName("tuichu"));
	ExitLayer->setVisible(false);

	Button* CloseBtn  = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_12"));//Button* CloseBtn = static_cast<Button*>(this->MyRoomLayerRoot->getChildByName("Button_12"));//(Button*)(Helper::seekWidgetByName
	CloseBtn->setTag(0);
	CloseBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuCloseCallBack, this, ExitLayer));//CloseBtn->addClickEventListener(CC_CALLBACK_1(MyRoomLayer::menuCloseCallBack, this));//CloseBtn->addTouchEventListener(this, toucheventselector(MyRoomLayer::menuCloseCallBack));
	
	
	Button* guanbiBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_38"));
	guanbiBtn->setTag(1);
	guanbiBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuCloseCallBack, this, ExitLayer));
	
	Button* quxiaoBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_40"));
	quxiaoBtn->setTag(2);
	quxiaoBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuCloseCallBack, this, ExitLayer));

	Button* tuichuOK  = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_39"));
	tuichuOK->setTag(3);
	tuichuOK->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuCloseCallBack, this, ExitLayer));
}

void MyRoomLayer::update(float l)
{
	cocos2d::Layer::update(l);
}

void MyRoomLayer::menuCloseCallBack(Ref* pSender, cocos2d::ui::Widget::TouchEventType type, CCSprite* Layer)
{
	if (type == Widget::TouchEventType::BEGAN)//判断点击类型，按钮按下生效
	{
		
		
	
	}
	else if (type == Widget::TouchEventType::ENDED)//按钮松开时生效
	{
		int tag = ((Button*)pSender)->getTag();//通过点击事件来获取当前按钮的tag值，就是这一句话来区分当前哪个button响应来此次事件
		CCLOG("退出界面tag:%d",tag);
		if (tag == 1)
		{
			Layer->setVisible(false);
			return;
		}
		else if (tag == 2)
		{
			Layer->setVisible(false);
			return;
		}
		else if (tag == 0)
		{
			if (Layer->isVisible())
			{
				Layer->setVisible(false);
			}
			else
			{
				Layer->setVisible(true);
			}
		}
		else if (tag == 3)
		{
			// "close" menu item clicked
			CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
#endif
		}
	}

}

void MyRoomLayer::setFankuiLayer()
{
	FanliLayer = static_cast<CCSprite*>(MyRoomLayerRoot->getChildByName("fankui"));
	FanliLayer->setVisible(false);

	Button* fankuiBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_10"));
	fankuiBtn->setTag(0);
	fankuiBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuOnlyCloseCallBack, this, FanliLayer));

	Button* fkCloseBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_36"));//Button* CloseBtn = static_cast<Button*>(this->MyRoomLayerRoot->getChildByName("Button_12"));//(Button*)(Helper::seekWidgetByName
	fkCloseBtn->setTag(1);
	fkCloseBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuOnlyCloseCallBack, this, FanliLayer));//CloseBtn->addClickEventListener(CC_CALLBACK_1(MyRoomLayer::menuCloseCallBack, this));//CloseBtn->addTouchEventListener(this, toucheventselector(MyRoomLayer::menuCloseCallBack));


	Button* fkokBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_37"));
	fkokBtn->setTag(2);
	fkokBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuOnlyCloseCallBack, this, FanliLayer));

}

void MyRoomLayer::menuOnlyCloseCallBack(Ref* pSender, cocos2d::ui::Widget::TouchEventType type, CCSprite* Layer)
{
	CCLOG("ishere??????????????????");
	if (type == Widget::TouchEventType::BEGAN)//判断点击类型，按钮按下生效
	{



	}
	else if (type == Widget::TouchEventType::ENDED)//按钮松开时生效
	{
		if (Layer->isVisible())
		{
			Layer->setVisible(false);
		}
		else
		{
			Layer->setVisible(true);
		}
		
	}

}

void MyRoomLayer::setShezhiLayer()
{
	CCSprite* shezhiLayout = static_cast<CCSprite*>(MyRoomLayerRoot->getChildByName("shezhi"));
	shezhiLayout->setVisible(false);

	Button* shezhiBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_15"));
	shezhiBtn->setTag(0);
	shezhiBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuShezhiCallBack, this, shezhiLayout));

	Button* CloseBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_33"));
	CloseBtn->setTag(1);
	CloseBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuShezhiCallBack, this, shezhiLayout));

	Button* CloseBGMusicBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_34"));
	CloseBGMusicBtn->setTag(2);
	CloseBGMusicBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuShezhiCallBack, this, shezhiLayout));

	Button* CloseVoiceBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_35"));
	CloseVoiceBtn->setTag(3);
	CloseVoiceBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuShezhiCallBack, this, shezhiLayout));

	bool bFlag1 = true;
	if (bFlag1)
	{
		CCLOG("IsHightlight");
		CloseBGMusicBtn->setBrightStyle(BrightStyle::NORMAL);
	}
	else
	{
		CCLOG("NOTHightlight");
		CloseBGMusicBtn->setBrightStyle(BrightStyle::HIGHLIGHT);
	}

	bool bFlag2 = false;
	if (bFlag2)
	{
		CCLOG("IsHightlight");
		CloseVoiceBtn->setBrightStyle(BrightStyle::NORMAL);
	}
	else
	{
		CCLOG("NOTHightlight");
		CloseVoiceBtn->setBrightStyle(BrightStyle::HIGHLIGHT);
	}
}

void MyRoomLayer::menuShezhiCallBack(Ref * pSender, cocos2d::ui::Widget::TouchEventType type, CCSprite* Layer)
{
	int tag = ((Button*)pSender)->getTag();
	if (type == Widget::TouchEventType::BEGAN)//判断点击类型，按钮按下生效
	{
		
	}
	else if (type == Widget::TouchEventType::ENDED)//按钮松开时生效
	{
		CCLOG("tagtag:%d",tag);
		if (tag == 0 || tag == 1)
		{
			if (Layer->isVisible())
			{
				Layer->setVisible(false);
			}
			else
			{
				Layer->setVisible(true);
			}
		}
		else if (tag == 2)
		{
			if (true)
			{
				CCLOG("IsHightlight");
				((Button*)pSender)->setBrightStyle(BrightStyle::HIGHLIGHT);
				isMusicOpen = false;
				SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			}
			else
			{
				CCLOG("NOTHightlight");
				((Button*)pSender)->setBrightStyle(BrightStyle::NORMAL);
				
				isMusicOpen = true;
			}
			CCLOG("Close BGM");
		}
		else if (tag == 3)
		{
			if (false)
			{
				CCLOG("IsHightlight");
				((Button*)pSender)->setBrightStyle(BrightStyle::HIGHLIGHT);
			}
			else
			{
				CCLOG("NOTHightlight");
				((Button*)pSender)->setBrightStyle(BrightStyle::NORMAL);
				isVoiceOpen = true;
			}
			CCLOG("Close Voice");
		}


	}
}

void MyRoomLayer::setKefuLayer()
{
	CCSprite* kefuLayout = static_cast<CCSprite*>(this->MyRoomLayerRoot->getChildByName("kefu"));
	kefuLayout->setVisible(false);

	Button* kefuBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_14"));
	kefuBtn->setTag(0);
	kefuBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuOnlyCloseCallBack, this, kefuLayout));

	Button* CloseBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_31"));
	CloseBtn->setTag(1);
	CloseBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuOnlyCloseCallBack, this, kefuLayout));

	Button* OkBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_32"));
	OkBtn->setTag(2);
	OkBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuOnlyCloseCallBack, this, kefuLayout));//CloseBtn->addClickEventListener(CC_CALLBACK_1(MyRoomLayer::menuCloseCallBack, this));//CloseBtn->addTouchEventListener(this, toucheventselector(MyRoomLayer::menuCloseCallBack));

}

void MyRoomLayer::setFangkaLayer()
{
	CCSprite* fangkaLayout = static_cast<CCSprite*>(this->MyRoomLayerRoot->getChildByName("fangka"));
	fangkaLayout->setVisible(false);

	Button* FangkaBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_9"));
	FangkaBtn->setTag(0);
	FangkaBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuOnlyCloseCallBack, this, fangkaLayout));

	Button* CloseBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_6"));
	CloseBtn->setTag(1);
	CloseBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuOnlyCloseCallBack, this, fangkaLayout));

	Button* OkBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_5"));
	OkBtn->setTag(2);
	OkBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuOnlyCloseCallBack, this, fangkaLayout));
}

void MyRoomLayer::setShengmingLayer()
{
	CCSprite* shengmingLayout = static_cast<CCSprite*>(this->MyRoomLayerRoot->getChildByName("shenming"));
	shengmingLayout->setVisible(false);

	auto* sehngmingBtn = static_cast<CCSprite*>(this->MyRoomLayerRoot->getChildByName("shenmingEvent"));
	sehngmingBtn->setTag(0);

	// Make sprite1 touchable
	auto listener1 = EventListenerTouchOneByOne::create();//创建一个触摸监听
	listener1->setSwallowTouches(false); //设置是否想下传递触摸

	//通过lambda表达式 直接实现触摸事件的回调方法
	listener1->onTouchBegan = [](Touch* touch, Event* event) {
		auto target = static_cast<CCSprite*>(event->getCurrentTarget());

		Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		if (rect.containsPoint(locationInNode))
		{
			log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
		
			return true;
		}
		return false;
	};

	listener1->onTouchMoved = [](Touch* touch, Event* event) {
		auto target = static_cast<CCSprite*>(event->getCurrentTarget());
		//target->setPosition(target->getPosition() + touch->getDelta());
	};

	listener1->onTouchEnded = [=](Touch* touch, Event* event) {
		auto target = static_cast<CCSprite*>(event->getCurrentTarget());
		log("sprite onTouchesEnded.. ");
		///g_ClientGameplayState->MyPlayEffects("Music/anniu.mp3", false);
		if (shengmingLayout->isVisible())
		{
			shengmingLayout->setVisible(false);
		}
		else
		{
			shengmingLayout->setVisible(true);
		}
	
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, sehngmingBtn);
	Button* CloseBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_3"));
	CloseBtn->setTag(1);
	CloseBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuOnlyCloseCallBack, this, shengmingLayout));

	Button* OkBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_4"));
	OkBtn->setTag(2);
	OkBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuOnlyCloseCallBack, this, shengmingLayout));
}

void MyRoomLayer::setHeadLayer()
{
	CCSprite* touxiangLayout = static_cast<CCSprite*>(this->MyRoomLayerRoot->getChildByName("touxiang"));
	touxiangLayout->setVisible(false);

	CCSprite* touxiangBtn = static_cast<CCSprite*>(this->MyRoomLayerRoot->getChildByName("head_50_25"));
	touxiangBtn->setTag(0);
	// Make sprite1 touchable
	auto listener1 = EventListenerTouchOneByOne::create();//创建一个触摸监听
	listener1->setSwallowTouches(false); //设置是否想下传递触摸

	//通过 lambda 表达式 直接实现触摸事件的回掉方法
	listener1->onTouchBegan = [](Touch* touch, Event* event) {
		auto target = static_cast<CCSprite*>(event->getCurrentTarget());

		Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		if (rect.containsPoint(locationInNode))
		{
			log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
			//target->setOpacity(180);
			return true;
		}
		return false;
	};

	listener1->onTouchMoved = [](Touch* touch, Event* event) {
		auto target = static_cast<CCSprite*>(event->getCurrentTarget());
		//target->setPosition(target->getPosition() + touch->getDelta());
	};

	listener1->onTouchEnded = [=](Touch* touch, Event* event) {
		auto target = static_cast<CCSprite*>(event->getCurrentTarget());
		log("sprite onTouchesEnded.. ");
		//g_ClientGameplayState->MyPlayEffects("Music/anniu.mp3", false);
		if (touxiangLayout->isVisible())
		{
			touxiangLayout->setVisible(false);
		}
		else
		{
			touxiangLayout->setVisible(true);
		}
	
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, touxiangBtn);

	Button* CloseBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_2"));
	CloseBtn->setTag(1);
	CloseBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuOnlyCloseCallBack, this, touxiangLayout));


	ImageView * Image_8    = static_cast<ImageView *>(touxiangLayout->getChildByName("Image_8"));
	ImageView * head_50_25 = static_cast<ImageView *>(MyRoomLayerRoot->getChildByName("head_50_25"));
	Text* Text_Name_0      = static_cast<Text *>(touxiangLayout->getChildByName("Text_Name_0"));
	Text* Text_ID_0        = static_cast<Text *>(touxiangLayout->getChildByName("Text_ID_0")); 
	Text* Text_IP_0        = static_cast<Text *>(touxiangLayout->getChildByName("Text_IP_0"));
	Text* Text_CARD_0      = static_cast<Text *>(touxiangLayout->getChildByName("Text_CARD_0"));
	
	
		
	CCSprite* sy_jsk_20 = static_cast<CCSprite*>(this->MyRoomLayerRoot->getChildByName("sy_jsk_20"));
	CCSprite* sy_jsk_20_0 = static_cast<CCSprite*>(this->MyRoomLayerRoot->getChildByName("sy_jsk_20_0"));
	Text* playernameText   = static_cast<Text *>(sy_jsk_20->getChildByName("playernameText"));
	Text* playerIdtext     = static_cast<Text *>(sy_jsk_20_0->getChildByName("playerIdtext"));

	char gbuffer[64] = "";

	char infobuffer[512] = "";

	unsigned int ID = 1;
	char pid[64] = "";
	char pids[64] = "";
	sprintf(pid, "%d", ID);
	sprintf(pids, "ID:%d", ID);
	Text_ID_0->setString(pid);
	playerIdtext->setString(pids);
	Text_IP_0->setString("");
	unsigned int CARD = 2;
	char CARDbuf[64] = "";
	sprintf(CARDbuf, "%d", CARD);
	Text_CARD_0->setString(CARDbuf);
}

void MyRoomLayer::setWanfaLayer()
{
	CCSprite* wanfa = static_cast<CCSprite*>(this->MyRoomLayerRoot->getChildByName("wanfa"));
	wanfa->setVisible(false);

	Button* kefuBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_7"));
	kefuBtn->setTag(0);
	kefuBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuOnlyCloseCallBack, this, wanfa));

	Button* CloseBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_46"));
	CloseBtn->setTag(1);
	CloseBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuOnlyCloseCallBack, this, wanfa));
	
}

void MyRoomLayer::setCreateRoom()
{
	CCSprite* jianfangLayout = static_cast<CCSprite*>(this->MyRoomLayerRoot->getChildByName("jianfang"));
	jianfangLayout->setVisible(false);

	CCSprite* cjfjpanel1 = static_cast<CCSprite*>(jianfangLayout->getChildByName("cjfjpanel1"));
	cjfjpanel1->setVisible(false);
	CCSprite* cjfjpanel2 = static_cast<CCSprite*>(jianfangLayout->getChildByName("cjfjpanel2"));
	cjfjpanel2->setVisible(true);
	
	CCSprite* CreateRoomBtn1 = static_cast<CCSprite*>(this->MyRoomLayerRoot->getChildByName("jianfangEvent"));//(Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "head_50_25"));
	CreateRoomBtn1->setTag(0);
	// Make sprite1 touchable
	auto listener1 = EventListenerTouchOneByOne::create();//创建一个触摸监听
	listener1->setSwallowTouches(false); //设置是否想下传递触摸

	//通过 lambda 表达式 直接实现触摸事件的回调方法
	listener1->onTouchBegan = [](Touch* touch, Event* event) {
		auto target = static_cast<CCSprite*>(event->getCurrentTarget());

		Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		if (rect.containsPoint(locationInNode))
		{
			log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
			//target->setOpacity(180);
			return true;
		}
		return false;
	};

	listener1->onTouchMoved = [](Touch* touch, Event* event) {
		auto target = static_cast<CCSprite*>(event->getCurrentTarget());
		//target->setPosition(target->getPosition() + touch->getDelta());
	};

	listener1->onTouchEnded = [=](Touch* touch, Event* event) {
		auto target = static_cast<CCSprite*>(event->getCurrentTarget());
		log("sprite onTouchesEnded.. ");
		///g_ClientGameplayState->MyPlayEffects("Music/anniu.mp3", false);
		if (jianfangLayout->isVisible())
		{
			jianfangLayout->setVisible(false);
		}
		else
		{
			jianfangLayout->setVisible(true);
		}

	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, CreateRoomBtn1);

	Button* CreateRoomBtn2 = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_17"));
	CreateRoomBtn2->setTag(0);
	CreateRoomBtn2->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::CreateRoom, this));

	Button* CloseBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_16"));
	CloseBtn->setTag(1);
	CloseBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuOnlyCloseCallBack, this, jianfangLayout));

	CheckBox* wanfa1 = (CheckBox*)(Helper::seekWidgetByName(MyRoomLayerRoot, "CheckBox_2"));//东带庄
	wanfa1->setTag(1);
	CheckBox* wanfa2 = (CheckBox*)(Helper::seekWidgetByName(MyRoomLayerRoot, "CheckBox_4"));//东不带庄
	wanfa2->setTag(0);

	wanfa1->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::SelectWanfa, this, wanfa1, wanfa2));
	wanfa2->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::SelectWanfa, this, wanfa1, wanfa2));
	//东带庄
	CheckBox* moshi1 = (CheckBox*)(Helper::seekWidgetByName(MyRoomLayerRoot, "CheckBox_3"));
	moshi1->setTag(1);
	//东不带庄
	CheckBox* moshi2 = (CheckBox*)(Helper::seekWidgetByName(MyRoomLayerRoot, "CheckBox_5"));
	moshi2->setTag(0);

	moshi1->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::SelectMoshi, this, moshi1, moshi2));
	moshi2->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::SelectMoshi, this, moshi1, moshi2));

	moshi1->setSelected(false);
	moshi2->setSelected(true);

	CheckBox* fen1 = (CheckBox*)(Helper::seekWidgetByName(MyRoomLayerRoot, "CheckBox_1"));
	fen1->setTag(0);
	CheckBox* fen2 = (CheckBox*)(Helper::seekWidgetByName(MyRoomLayerRoot, "CheckBox_8"));
	fen2->setTag(1);

	fen1->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::Selectfenorju, this, fen1, fen2));
	fen2->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::Selectfenorju, this, fen1, fen2));

	CheckBox* ju1 = (CheckBox*)(Helper::seekWidgetByName(MyRoomLayerRoot, "CheckBox_7"));
	ju1->setTag(2);
	CheckBox* ju2 = (CheckBox*)(Helper::seekWidgetByName(MyRoomLayerRoot, "CheckBox_6"));
	ju2->setTag(3);

	ju1->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::Selectfenorju, this, ju1, ju2));
	ju2->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::Selectfenorju, this, ju1, ju2));
}

void MyRoomLayer::setJoinRoom()
{
	CCSprite* joinroomLayout = static_cast<CCSprite*>(MyRoomLayerRoot->getChildByName("jiafang"));
	joinroomLayout->setVisible(false);

	CCSprite* joinRoomBtn1 = static_cast<CCSprite*>(MyRoomLayerRoot->getChildByName("jiafangEvent"));//(Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "head_50_25"));
	joinRoomBtn1->setTag(0);
	// Make sprite1 touchable
	auto listener1 = EventListenerTouchOneByOne::create();//创建一个触摸监听
	listener1->setSwallowTouches(false); //设置是否想下传递触摸

	//通过 lambda 表达式 直接实现触摸事件的回调方法
	listener1->onTouchBegan = [](Touch* touch, Event* event) {
		auto target = static_cast<CCSprite*>(event->getCurrentTarget());

		Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		if (rect.containsPoint(locationInNode))
		{
			log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
			//target->setOpacity(180);
			return true;
		}
		return false;
	};

	listener1->onTouchMoved = [](Touch* touch, Event* event) {
		auto target = static_cast<CCSprite*>(event->getCurrentTarget());
		//target->setPosition(target->getPosition() + touch->getDelta());
	};

	listener1->onTouchEnded = [=](Touch* touch, Event* event) {
		auto target = static_cast<CCSprite*>(event->getCurrentTarget());
		log("sprite onTouchesEnded.. ");
		///g_ClientGameplayState->MyPlayEffects("Music/anniu.mp3", false);
		if (joinroomLayout->isVisible())
		{
			joinroomLayout->setVisible(false);
		}
		else
		{
			joinroomLayout->setVisible(true);
		}

	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, joinRoomBtn1);


	Button* CloseBtn = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_30"));
	CloseBtn->setTag(12);
	CloseBtn->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuNumberCallBack, this, joinroomLayout));

	Button* num0 = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_18"));
	num0->setTag(0);
	num0->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuNumberCallBack, this, joinroomLayout));

	Button* num1 = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_19"));
	num1->setTag(1);
	num1->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuNumberCallBack, this, joinroomLayout));

	Button* num2 = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_20"));
	num2->setTag(2);
	num2->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuNumberCallBack, this, joinroomLayout));

	Button* num3 = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_21"));
	num3->setTag(3);
	num3->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuNumberCallBack, this, joinroomLayout));

	Button* num4 = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_22"));
	num4->setTag(4);
	num4->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuNumberCallBack, this, joinroomLayout));

	Button* num5 = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_23"));
	num5->setTag(5);
	num5->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuNumberCallBack, this, joinroomLayout));

	Button* num6 = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_24"));
	num6->setTag(6);
	num6->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuNumberCallBack, this, joinroomLayout));

	Button* num7 = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_25"));
	num7->setTag(7);
	num7->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuNumberCallBack, this, joinroomLayout));

	Button* num8 = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_26"));
	num8->setTag(8);
	num8->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuNumberCallBack, this, joinroomLayout));

	Button* num9 = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_27"));
	num9->setTag(9);
	num9->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuNumberCallBack, this, joinroomLayout));

	Button* chongxinshuru = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_29"));
	chongxinshuru->setTag(10);
	chongxinshuru->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuNumberCallBack, this, joinroomLayout));

	Button* shanchu = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_28"));
	shanchu->setTag(11);
	shanchu->addTouchEventListener(CC_CALLBACK_2(MyRoomLayer::menuNumberCallBack, this, joinroomLayout));
}
void MyRoomLayer::AfterCapture(CCRenderTexture* pScreen, const std::string& fileName)
{
	Text* pT1 = Text::create();
	pT1->setColor(Color3B(0, 255, 100));
	pT1->setPosition(Vec2(200, 500));
	pT1->setString(fileName);
	MyRoomLayerRoot->addChild(pT1);
}

void MyRoomLayer::DongDaiZhuang(signed int tag)
{
	CheckBox* moshi1 = (CheckBox*)(Helper::seekWidgetByName(MyRoomLayerRoot, "CheckBox_3"));
	CheckBox* moshi2 = (CheckBox*)(Helper::seekWidgetByName(MyRoomLayerRoot, "CheckBox_5"));
	switch (tag)
	{
	case 0: // 东不带庄
		{
			moshi1->setSelected(false);
			moshi2->setSelected(true);
		}break;
	case 1:	// 东带庄
		{
			moshi1->setSelected(true);
			moshi2->setSelected(false);
		}break;
	default:
		break;
	}
}

void MyRoomLayer::OnGetBullet(const std::string& strBullet)
{
	cocos2d::Node* pNodBulletine = MyRoomLayerRoot->getChildByName("Bulletin");
	cocos2d::ui::Text* pBulletineText = (cocos2d::ui::Text*)pNodBulletine->getChildByName("BulletinText");
	size_t len = strBullet.length();
	for (size_t i = 0; i < len; ++i)
	{
		if (strBullet[i] == '\n')
		{
			const_cast<char&>(strBullet[i]) = ' ';
		}
		if (strBullet[i] == '\r')
		{
			const_cast<char&>(strBullet[i]) = ' ';
		}
	}
	pBulletineText->setString(strBullet);

	m_isBulletinBoard = true;
}

void MyRoomLayer::CreateRoom(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::BEGAN)//判断点击类型，按钮按下生效
	{
		


	}
	else if (type == Widget::TouchEventType::ENDED)//按钮松开时生效
	{
		if (m_isAfterCaptureing)
		{
			return;
		}
	}

}

void MyRoomLayer::menuNumberCallBack(Ref * pSender, cocos2d::ui::Widget::TouchEventType type, CCSprite* Layer)
{
	if (type == Widget::TouchEventType::BEGAN)//判断点击类型，按钮按下生效
	{

	}
	else if (type == Widget::TouchEventType::ENDED)//按钮松开时生效
	{
		int tag = ((Button*)pSender)->getTag();
		if (tag == 12)
		{
			std::vector<CCSprite*>::iterator itr;
			for (itr = JoinRoomNumberSprite.begin(); itr != JoinRoomNumberSprite.end(); itr++)
			{
				(*itr)->removeFromParent();
			}
			//JoinRoomNumberSprite.pop_back();
			JoinRoomNumberSprite.clear();
			JoinRoomNumber.clear();

			Layer->setVisible(false);

		}
		else if (tag == 10)
		{
			//重输
			std::vector<CCSprite*>::iterator itr;
			for (itr = JoinRoomNumberSprite.begin(); itr != JoinRoomNumberSprite.end(); itr++)
			{
				(*itr)->removeFromParent();
			}
			//JoinRoomNumberSprite.pop_back();
			JoinRoomNumberSprite.clear();
			JoinRoomNumber.clear();
		}
		else if (tag == 11)
		{
			//删除
			if (JoinRoomNumberSprite.empty())
			{
				return;
			}
			JoinRoomNumber.pop_back();
			std::vector<CCSprite*>::iterator itr = JoinRoomNumberSprite.end() - 1;
			(*itr)->removeFromParent();
			JoinRoomNumberSprite.pop_back();
		}
		else
		{
			if (JoinRoomNumber.size() >= 6)
				return;
			
			int posx = Layer->getContentSize().width*0.22;
			posx += JoinRoomNumber.size() * 140;
			JoinRoomNumber.push_back(tag);

			CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage("fonts/score_num.png");
			CCSpriteFrame *frame = CCSpriteFrame::createWithTexture(texture, CCRectMake(tag * 60, 0, 60, 80));
			CCSprite* sprite = Sprite::createWithSpriteFrame(frame);
			CCSprite* numbg = static_cast<CCSprite*>(MyRoomLayerRoot->getChildByName("jiafang"));
			numbg->addChild(sprite);
			sprite->setPosition(posx, Layer->getContentSize().height * 0.72);
			JoinRoomNumberSprite.push_back(sprite);
			if (JoinRoomNumber.size() >= 6)
			{
				char buffer[128] = "";
				char buffer2[128] = "";
				for (int i = 0; i < JoinRoomNumber.size(); i++)
				{
					sprintf(buffer, "%d", JoinRoomNumber[i]);
					strcat(buffer2, buffer);
				}
			}
		}
	}
}

void MyRoomLayer::SelectWanfa(Ref* pSender, cocos2d::ui::Widget::TouchEventType type, CheckBox* btn1, CheckBox* btn2)
{
	if (type == Widget::TouchEventType::BEGAN)//判断点击类型，按钮按下生效
	{

	}
	else if (type == Widget::TouchEventType::ENDED)//按钮松开时生效
	{
		int tag = ((Button*)pSender)->getTag();
		if (tag == 0)
		{
			CheckBox* moshi1 = (CheckBox*)(Helper::seekWidgetByName(MyRoomLayerRoot, "CheckBox_3"));
			moshi1->setEnabled(true);
			DongDaiZhuang(1);
			btn1->setSelected(false);
			btn2->setSelected(true);
			
		}
		else if (tag == 1)
		{
			DongDaiZhuang(0);
			CheckBox* moshi1 = (CheckBox*)(Helper::seekWidgetByName(MyRoomLayerRoot, "CheckBox_3"));
			moshi1->setEnabled(false);
			btn1->setSelected(true);
			btn2->setSelected(false);
		}

	}
}

void MyRoomLayer::SelectMoshi(Ref* pSender, cocos2d::ui::Widget::TouchEventType type, CheckBox* btn1, CheckBox* btn2)
{
	if (type == Widget::TouchEventType::BEGAN)//判断点击类型，按钮按下生效
	{

	}
	else if (type == Widget::TouchEventType::ENDED)//按钮松开时生效
	{
		int tag = ((Button*)pSender)->getTag();
		DongDaiZhuang(tag);
		if (tag == 0)//东不带庄
		{

		}
		else if (tag == 1)//东带庄
		{

		}
	}
}

void MyRoomLayer::Selectfenorju(Ref* pSender, cocos2d::ui::Widget::TouchEventType type, CheckBox* btn1, CheckBox* btn2)
{
	if (type == Widget::TouchEventType::BEGAN)//判断点击类型，按钮按下生效
	{

	}
	else if (type == Widget::TouchEventType::ENDED)//按钮松开时生效
	{
		int tag = ((Button*)pSender)->getTag();

		if (tag == 0)
		{
			btn1->setSelected(true);
			btn2->setSelected(false);
		}
		else if (tag == 1)
		{
			btn1->setSelected(false);
			btn2->setSelected(true);
		}
		else if (tag == 2)
		{
			btn1->setSelected(true);
			btn2->setSelected(false);
		}
		else if (tag == 3)
		{
			btn1->setSelected(false);
			btn2->setSelected(true);
		}
	}
}

void MyRoomLayer::menuFKSXCallBack(Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::BEGAN)//判断点击类型，按钮按下生效
	{

	}
	else if (type == Widget::TouchEventType::ENDED)//按钮松开时生效
	{

	}
}

void MyRoomLayer::menuToMilitary(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)//按钮松开时生效
	{

	}
}

void  MyRoomLayer::menuExplain(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::BEGAN)
	{

	}
	else if (type == Widget::TouchEventType::ENDED)
	{
		int tag = ((Button*)pSender)->getTag();

		if (tag == 0)
		{
			OnGuiXiMaJiang();
		}
		else
		{
			OnHowOpenRoom();
		}
	}
}

void MyRoomLayer::OnGuiXiMaJiang()
{
	Button* pButton_43 = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_43"));
	Button* pButton_44 = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_44"));
	pButton_43->setBrightStyle(BrightStyle::HIGHLIGHT);
	pButton_44->setBrightStyle(BrightStyle::NORMAL);
	Node* pNode1 = Helper::seekWidgetByName(MyRoomLayerRoot, "ScrollView_1");
	Node* pNode2 = Helper::seekWidgetByName(MyRoomLayerRoot, "ScrollView_2");
	pNode1->setVisible(true);
	pNode2->setVisible(false);
}
void MyRoomLayer::OnHowOpenRoom()
{
	Button* pButton_43 = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_43"));
	Button* pButton_44 = (Button*)(Helper::seekWidgetByName(MyRoomLayerRoot, "Button_44"));
	pButton_43->setBrightStyle(BrightStyle::NORMAL);
	pButton_44->setBrightStyle(BrightStyle::HIGHLIGHT);
	Node* pNode1 = Helper::seekWidgetByName(MyRoomLayerRoot, "ScrollView_1");
	Node* pNode2 = Helper::seekWidgetByName(MyRoomLayerRoot, "ScrollView_2");
	pNode1->setVisible(false);
	pNode2->setVisible(true);
}

void MyRoomLayer::ReflashRoomCard()
{
	CCSprite* sy_jsk_20_0_0 = static_cast<CCSprite*>(MyRoomLayerRoot->getChildByName("sy_jsk_20_0_0"));
	Text* roomCardsNum = static_cast<Text*>(sy_jsk_20_0_0->getChildByName("roomCardsNum"));
	std::stringstream ss;
	roomCardsNum->setText(ss.str());
}
