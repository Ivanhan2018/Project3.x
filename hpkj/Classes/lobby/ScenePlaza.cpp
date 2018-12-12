#include "ScenePlaza.h"
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
CCSprite* ScenePlaza::FanliLayer;
int ScenePlaza::MyOutCardCount = 0;
int ScenePlaza::RightOutCardCount = 0;
int ScenePlaza::UpOutCardCount = 0;
int ScenePlaza::LeftOutCardCount = 0;
std::vector<int> 			    ScenePlaza::JoinRoomNumber;
std::vector<CCSprite*> 			ScenePlaza::JoinRoomNumberSprite;
ScenePlaza* ScenePlaza::s_pScenePlaza = NULL;

Scene* ScenePlaza::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ScenePlaza::create();
	s_pScenePlaza = layer;
	// add layer as a child to scene
	scene->addChild( layer, 0, "root");

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool ScenePlaza::init()
{

	if (!Layer::init())
	{
		return false;
	}
	

	Widget* Root = static_cast<Widget*>(CSLoader::createNode("ScenePlaza.csb"));
	addChild(Root, 0, "box1");

	ScenePlazaRoot = static_cast<Widget*>(Root->getChildByName("lobbyPanel"));

	Size size = Director::getInstance()->getWinSize();
	Root->setScale(size.width/ ScenePlazaRoot->getContentSize().width, size.height/ ScenePlazaRoot->getContentSize().height);

	Sprite* bgBackLobby = Sprite::create("Share/LoginBackView.jpg");
	Size size1 = Director::getInstance()->getWinSizeInPixels();
	float fScaleX = size1.width / bgBackLobby->getContentSize().width;
	float fScaleY = size1.height / bgBackLobby->getContentSize().height;
	bgBackLobby->setScaleX(fScaleX);
	bgBackLobby->setScaleY(fScaleY);
	bgBackLobby->setPosition(size1.width / 2, size1.height / 2);
	this->addChild(bgBackLobby, -10);

	Node* pNode1 = Helper::seekWidgetByName(ScenePlazaRoot, "btn_mahjong_invite");
	pNode1->setVisible(false);

	CCSprite* sy_jsk_20_0_0 = static_cast<CCSprite*>(ScenePlazaRoot->getChildByName("sy_jsk_20_0_0"));
	Text* roomCardsNum = static_cast<Text*>(Helper::seekWidgetByName(ScenePlazaRoot, "beanNum"));
	
	unsigned int CARD = 1;
	char CARDbuf[64] = "";
	sprintf(CARDbuf, "%d", CARD);

	roomCardsNum->setString(CARDbuf);

	const char *szBtnNameArr[] = { "btn_game_bjl" ,"btn_game_ddz","btn_game_by","btn_game_zzmj","btn_game_brnn","btn_game_ppc"};
	int nNum = sizeof(szBtnNameArr) / sizeof(szBtnNameArr[0]);
	for (int i = 0; i < nNum; i++)
	{
		Button* fksx = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, szBtnNameArr[i]));
		fksx->setTag(i);
		fksx->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuFKSXCallBack, this,i));
	}

	isMusicOpen = false;
	isVoiceOpen = false;

	m_isBulletinBoard = false;
	scheduleUpdate();

	return true;
}

void ScenePlaza::setCloseLayer()
{
	CCSprite* ExitLayer = static_cast<CCSprite*>(ScenePlazaRoot->getChildByName("tuichu"));
	ExitLayer->setVisible(false);

	Button* CloseBtn  = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_12"));
	CloseBtn->setTag(0);
	CloseBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuCloseCallBack, this, ExitLayer));
	
	
	Button* guanbiBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_38"));
	guanbiBtn->setTag(1);
	guanbiBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuCloseCallBack, this, ExitLayer));
	
	Button* quxiaoBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_40"));
	quxiaoBtn->setTag(2);
	quxiaoBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuCloseCallBack, this, ExitLayer));

	Button* tuichuOK  = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_39"));
	tuichuOK->setTag(3);
	tuichuOK->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuCloseCallBack, this, ExitLayer));
	
}

void ScenePlaza::update(float l)
{
	cocos2d::Layer::update(l);
}

void ScenePlaza::menuCloseCallBack(Ref* pSender, cocos2d::ui::Widget::TouchEventType type, CCSprite* Layer)
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

void ScenePlaza::setFankuiLayer()
{
	FanliLayer = static_cast<CCSprite*>(ScenePlazaRoot->getChildByName("fankui"));
	FanliLayer->setVisible(false);

	Button* fankuiBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_10"));
	fankuiBtn->setTag(0);
	fankuiBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuOnlyCloseCallBack, this, FanliLayer));

	Button* fkCloseBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_36"));
	fkCloseBtn->setTag(1);
	fkCloseBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuOnlyCloseCallBack, this, FanliLayer));

	Button* fkokBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_37"));
	fkokBtn->setTag(2);
	fkokBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuOnlyCloseCallBack, this, FanliLayer));

}

void ScenePlaza::menuOnlyCloseCallBack(Ref* pSender, cocos2d::ui::Widget::TouchEventType type, CCSprite* Layer)
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

void ScenePlaza::setShezhiLayer()
{
	CCSprite* shezhiLayout = static_cast<CCSprite*>(ScenePlazaRoot->getChildByName("shezhi"));
	shezhiLayout->setVisible(false);

	Button* shezhiBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_15"));
	shezhiBtn->setTag(0);
	shezhiBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuShezhiCallBack, this, shezhiLayout));

	Button* CloseBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_33"));
	CloseBtn->setTag(1);
	CloseBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuShezhiCallBack, this, shezhiLayout));

	Button* CloseBGMusicBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_34"));//Button* CloseBtn = static_cast<Button*>(this->ScenePlazaRoot->getChildByName("Button_12"));//(Button*)(Helper::seekWidgetByName
	CloseBGMusicBtn->setTag(2);
	CloseBGMusicBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuShezhiCallBack, this, shezhiLayout));//CloseBtn->addClickEventListener(CC_CALLBACK_1(ScenePlaza::menuCloseCallBack, this));//CloseBtn->addTouchEventListener(this, toucheventselector(ScenePlaza::menuCloseCallBack));


	Button* CloseVoiceBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_35"));
	CloseVoiceBtn->setTag(3);
	CloseVoiceBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuShezhiCallBack, this, shezhiLayout));

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

void ScenePlaza::menuShezhiCallBack(Ref * pSender, cocos2d::ui::Widget::TouchEventType type, CCSprite* Layer)
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
			//if (0==1)
			//{
			//	CCLOG("IsHightlight");
			//	((Button*)pSender)->setBrightStyle(BrightStyle::HIGHLIGHT);
			//else
			{
				CCLOG("NOTHightlight");
				((Button*)pSender)->setBrightStyle(BrightStyle::NORMAL);
				isVoiceOpen = true;
			}
			CCLOG("Close Voice");
		}


	}
}

void ScenePlaza::setKefuLayer()
{
	CCSprite* kefuLayout = static_cast<CCSprite*>(this->ScenePlazaRoot->getChildByName("kefu"));
	kefuLayout->setVisible(false);

	Button* kefuBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_14"));
	kefuBtn->setTag(0);
	kefuBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuOnlyCloseCallBack, this, kefuLayout));

	Button* CloseBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_31"));
	CloseBtn->setTag(1);
	CloseBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuOnlyCloseCallBack, this, kefuLayout));

	Button* OkBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_32"));//Button* CloseBtn = static_cast<Button*>(this->ScenePlazaRoot->getChildByName("Button_12"));//(Button*)(Helper::seekWidgetByName
	OkBtn->setTag(2);
	OkBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuOnlyCloseCallBack, this, kefuLayout));//CloseBtn->addClickEventListener(CC_CALLBACK_1(ScenePlaza::menuCloseCallBack, this));//CloseBtn->addTouchEventListener(this, toucheventselector(ScenePlaza::menuCloseCallBack));

}

void ScenePlaza::setFangkaLayer()
{
	CCSprite* fangkaLayout = static_cast<CCSprite*>(this->ScenePlazaRoot->getChildByName("fangka"));
	fangkaLayout->setVisible(false);

	Button* FangkaBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_9"));
	FangkaBtn->setTag(0);
	FangkaBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuOnlyCloseCallBack, this, fangkaLayout));

	Button* CloseBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_6"));
	CloseBtn->setTag(1);
	CloseBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuOnlyCloseCallBack, this, fangkaLayout));

	Button* OkBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_5"));
	OkBtn->setTag(2);
	OkBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuOnlyCloseCallBack, this, fangkaLayout));
}

void ScenePlaza::setShengmingLayer()
{
	CCSprite* shengmingLayout = static_cast<CCSprite*>(this->ScenePlazaRoot->getChildByName("shenming"));
	shengmingLayout->setVisible(false);

	auto* sehngmingBtn = static_cast<CCSprite*>(this->ScenePlazaRoot->getChildByName("shenmingEvent"));//(Armature*)(Helper::seekWidgetByName(ScenePlazaRoot, "ArmatureNode_2"));
	sehngmingBtn->setTag(0);

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
	Button* CloseBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_3"));
	CloseBtn->setTag(1);
	CloseBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuOnlyCloseCallBack, this, shengmingLayout));

	Button* OkBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_4"));
	OkBtn->setTag(2);
	OkBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuOnlyCloseCallBack, this, shengmingLayout));
}

void ScenePlaza::setHeadLayer()
{
	CCSprite* touxiangLayout = static_cast<CCSprite*>(this->ScenePlazaRoot->getChildByName("touxiang"));
	touxiangLayout->setVisible(false);

	CCSprite* touxiangBtn = static_cast<CCSprite*>(this->ScenePlazaRoot->getChildByName("head_50_25"));//(Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "head_50_25"));
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

	Button* CloseBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_2"));
	CloseBtn->setTag(1);
	CloseBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuOnlyCloseCallBack, this, touxiangLayout));


	ImageView * Image_8    = static_cast<ImageView *>(touxiangLayout->getChildByName("Image_8"));
	ImageView * head_50_25 = static_cast<ImageView *>(ScenePlazaRoot->getChildByName("head_50_25"));
	Text* Text_Name_0      = static_cast<Text *>(touxiangLayout->getChildByName("Text_Name_0"));
	Text* Text_ID_0        = static_cast<Text *>(touxiangLayout->getChildByName("Text_ID_0")); 
	Text* Text_IP_0        = static_cast<Text *>(touxiangLayout->getChildByName("Text_IP_0"));
	Text* Text_CARD_0      = static_cast<Text *>(touxiangLayout->getChildByName("Text_CARD_0"));
	
	
		
	CCSprite* sy_jsk_20 = static_cast<CCSprite*>(this->ScenePlazaRoot->getChildByName("sy_jsk_20"));
	CCSprite* sy_jsk_20_0 = static_cast<CCSprite*>(this->ScenePlazaRoot->getChildByName("sy_jsk_20_0"));
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

void ScenePlaza::setWanfaLayer()
{
	CCSprite* wanfa = static_cast<CCSprite*>(this->ScenePlazaRoot->getChildByName("wanfa"));
	wanfa->setVisible(false);

	Button* kefuBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_7"));
	kefuBtn->setTag(0);
	kefuBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuOnlyCloseCallBack, this, wanfa));

	Button* CloseBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_46"));
	CloseBtn->setTag(1);
	CloseBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuOnlyCloseCallBack, this, wanfa));
	
}

void ScenePlaza::setCreateRoom()
{
	CCSprite* jianfangLayout = static_cast<CCSprite*>(this->ScenePlazaRoot->getChildByName("jianfang"));
	jianfangLayout->setVisible(false);

	CCSprite* cjfjpanel1 = static_cast<CCSprite*>(jianfangLayout->getChildByName("cjfjpanel1"));
	cjfjpanel1->setVisible(false);
	CCSprite* cjfjpanel2 = static_cast<CCSprite*>(jianfangLayout->getChildByName("cjfjpanel2"));
	cjfjpanel2->setVisible(true);
	
	CCSprite* CreateRoomBtn1 = static_cast<CCSprite*>(this->ScenePlazaRoot->getChildByName("jianfangEvent"));//(Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "head_50_25"));
	CreateRoomBtn1->setTag(0);
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

	Button* CreateRoomBtn2 = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_17"));
	CreateRoomBtn2->setTag(0);
	CreateRoomBtn2->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::CreateRoom, this));

	Button* CloseBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_16"));
	CloseBtn->setTag(1);
	CloseBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuOnlyCloseCallBack, this, jianfangLayout));

	CheckBox* wanfa1 = (CheckBox*)(Helper::seekWidgetByName(ScenePlazaRoot, "CheckBox_2"));//东带庄
	wanfa1->setTag(1);
	CheckBox* wanfa2 = (CheckBox*)(Helper::seekWidgetByName(ScenePlazaRoot, "CheckBox_4"));//东不带庄
	wanfa2->setTag(0);

	wanfa1->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::SelectWanfa, this, wanfa1, wanfa2));
	wanfa2->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::SelectWanfa, this, wanfa1, wanfa2));
	//东带庄
	CheckBox* moshi1 = (CheckBox*)(Helper::seekWidgetByName(ScenePlazaRoot, "CheckBox_3"));
	moshi1->setTag(1);
	//东不带庄
	CheckBox* moshi2 = (CheckBox*)(Helper::seekWidgetByName(ScenePlazaRoot, "CheckBox_5"));
	moshi2->setTag(0);

	moshi1->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::SelectMoshi, this, moshi1, moshi2));
	moshi2->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::SelectMoshi, this, moshi1, moshi2));

	moshi1->setSelected(false);
	moshi2->setSelected(true);

	CheckBox* fen1 = (CheckBox*)(Helper::seekWidgetByName(ScenePlazaRoot, "CheckBox_1"));
	fen1->setTag(0);
	CheckBox* fen2 = (CheckBox*)(Helper::seekWidgetByName(ScenePlazaRoot, "CheckBox_8"));
	fen2->setTag(1);

	fen1->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::Selectfenorju, this, fen1, fen2));
	fen2->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::Selectfenorju, this, fen1, fen2));

	CheckBox* ju1 = (CheckBox*)(Helper::seekWidgetByName(ScenePlazaRoot, "CheckBox_7"));
	ju1->setTag(2);
	CheckBox* ju2 = (CheckBox*)(Helper::seekWidgetByName(ScenePlazaRoot, "CheckBox_6"));
	ju2->setTag(3);

	ju1->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::Selectfenorju, this, ju1, ju2));
	ju2->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::Selectfenorju, this, ju1, ju2));
}

void ScenePlaza::setJoinRoom()
{
	CCSprite* joinroomLayout = static_cast<CCSprite*>(ScenePlazaRoot->getChildByName("jiafang"));
	joinroomLayout->setVisible(false);

	CCSprite* joinRoomBtn1 = static_cast<CCSprite*>(ScenePlazaRoot->getChildByName("jiafangEvent"));//(Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "head_50_25"));
	joinRoomBtn1->setTag(0);
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


	Button* CloseBtn = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_30"));
	CloseBtn->setTag(12);
	CloseBtn->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuNumberCallBack, this, joinroomLayout));

	Button* num0 = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_18"));
	num0->setTag(0);
	num0->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuNumberCallBack, this, joinroomLayout));

	Button* num1 = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_19"));
	num1->setTag(1);
	num1->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuNumberCallBack, this, joinroomLayout));

	Button* num2 = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_20"));
	num2->setTag(2);
	num2->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuNumberCallBack, this, joinroomLayout));

	Button* num3 = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_21"));
	num3->setTag(3);
	num3->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuNumberCallBack, this, joinroomLayout));

	Button* num4 = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_22"));
	num4->setTag(4);
	num4->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuNumberCallBack, this, joinroomLayout));

	Button* num5 = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_23"));
	num5->setTag(5);
	num5->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuNumberCallBack, this, joinroomLayout));

	Button* num6 = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_24"));
	num6->setTag(6);
	num6->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuNumberCallBack, this, joinroomLayout));

	Button* num7 = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_25"));
	num7->setTag(7);
	num7->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuNumberCallBack, this, joinroomLayout));

	Button* num8 = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_26"));
	num8->setTag(8);
	num8->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuNumberCallBack, this, joinroomLayout));

	Button* num9 = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_27"));
	num9->setTag(9);
	num9->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuNumberCallBack, this, joinroomLayout));

	Button* chongxinshuru = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_29"));
	chongxinshuru->setTag(10);
	chongxinshuru->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuNumberCallBack, this, joinroomLayout));

	Button* shanchu = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_28"));
	shanchu->setTag(11);
	shanchu->addTouchEventListener(CC_CALLBACK_2(ScenePlaza::menuNumberCallBack, this, joinroomLayout));
}
void ScenePlaza::AfterCapture(CCRenderTexture* pScreen, const std::string& fileName)
{
	Text* pT1 = Text::create();
	pT1->setColor(Color3B(0, 255, 100));
	pT1->setPosition(Vec2(200, 500));
	pT1->setString(fileName);
	ScenePlazaRoot->addChild(pT1);
}

void ScenePlaza::DongDaiZhuang(signed int tag)
{
	CheckBox* moshi1 = (CheckBox*)(Helper::seekWidgetByName(ScenePlazaRoot, "CheckBox_3"));
	CheckBox* moshi2 = (CheckBox*)(Helper::seekWidgetByName(ScenePlazaRoot, "CheckBox_5"));
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

void ScenePlaza::OnGetBullet(const std::string& strBullet)
{
	cocos2d::Node* pNodBulletine = ScenePlazaRoot->getChildByName("Bulletin");
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

void ScenePlaza::CreateRoom(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
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

void ScenePlaza::menuNumberCallBack(Ref * pSender, cocos2d::ui::Widget::TouchEventType type, CCSprite* Layer)
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
			CCSprite* numbg = static_cast<CCSprite*>(ScenePlazaRoot->getChildByName("jiafang"));
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

void ScenePlaza::SelectWanfa(Ref* pSender, cocos2d::ui::Widget::TouchEventType type, CheckBox* btn1, CheckBox* btn2)
{
	if (type == Widget::TouchEventType::BEGAN)//判断点击类型，按钮按下生效
	{

	}
	else if (type == Widget::TouchEventType::ENDED)//按钮松开时生效
	{
		int tag = ((Button*)pSender)->getTag();
		if (tag == 0)
		{
			CheckBox* moshi1 = (CheckBox*)(Helper::seekWidgetByName(ScenePlazaRoot, "CheckBox_3"));
			moshi1->setEnabled(true);
			DongDaiZhuang(1);
			btn1->setSelected(false);
			btn2->setSelected(true);
			
		}
		else if (tag == 1)
		{
			DongDaiZhuang(0);
			CheckBox* moshi1 = (CheckBox*)(Helper::seekWidgetByName(ScenePlazaRoot, "CheckBox_3"));
			moshi1->setEnabled(false);
			btn1->setSelected(true);
			btn2->setSelected(false);
		}

	}
}

void ScenePlaza::SelectMoshi(Ref* pSender, cocos2d::ui::Widget::TouchEventType type, CheckBox* btn1, CheckBox* btn2)
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

void ScenePlaza::Selectfenorju(Ref* pSender, cocos2d::ui::Widget::TouchEventType type, CheckBox* btn1, CheckBox* btn2)
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
			btn2->setSelected(false);
		}
		else if (tag == 3)
		{
			btn1->setSelected(false);
			btn2->setSelected(true);
		}
	}
}

void ScenePlaza::menuFKSXCallBack(Ref * pSender, cocos2d::ui::Widget::TouchEventType type, int tag)
{
	CCLOG("tag=%d\n",tag);
	if (type == Widget::TouchEventType::BEGAN)//判断点击类型，按钮按下生效
	{

	}
	else if (type == Widget::TouchEventType::ENDED)//按钮松开时生效
	{

	}
}

void ScenePlaza::menuToMilitary(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)//按钮松开时生效
	{

	}
}

void  ScenePlaza::menuExplain(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
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

void ScenePlaza::OnGuiXiMaJiang()
{

}
void ScenePlaza::OnHowOpenRoom()
{
	Button* pButton_43 = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_43"));
	Button* pButton_44 = (Button*)(Helper::seekWidgetByName(ScenePlazaRoot, "Button_44"));
	pButton_43->setBrightStyle(BrightStyle::NORMAL);
	pButton_44->setBrightStyle(BrightStyle::HIGHLIGHT);
	Node* pNode1 = Helper::seekWidgetByName(ScenePlazaRoot, "ScrollView_1");
	Node* pNode2 = Helper::seekWidgetByName(ScenePlazaRoot, "ScrollView_2");
	pNode1->setVisible(false);
	pNode2->setVisible(true);
}

void ScenePlaza::ReflashRoomCard()
{
	CCSprite* sy_jsk_20_0_0 = static_cast<CCSprite*>(ScenePlazaRoot->getChildByName("sy_jsk_20_0_0"));
	Text* roomCardsNum = static_cast<Text*>(sy_jsk_20_0_0->getChildByName("roomCardsNum"));
	std::stringstream ss;
	roomCardsNum->setText(ss.str());
}
