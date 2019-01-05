#include "ByMenuLayer.h"
#include "CMD_Fish.h"
#include "Sound.h"
#include "IntroduceLayer.h"
#include "EntityMgr.h"
#include "CharityLayer.h"
#include "CharityLeaveLayer.h"


//#define LRBY_VIEW (LrbyClientView::GetInstance())
#define CHARGELAYER				80
#define SETTINGRADIUS			100

ByMenuLayer* ByMenuLayer::m_pMenuLayer = nullptr;

ByMenuLayer::ByMenuLayer()
{
	m_awardLayer = nullptr;
	//m_pMenuLayer = this;
	//m_rechargelayer = nullptr;
	m_isExchange = false;
}


ByMenuLayer::~ByMenuLayer()
{
	m_pMenuLayer = nullptr;
}

bool ByMenuLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_dwTicketNumber = 0;

	this->setContentSize(cocos2d::Size(NS_lrbyFish::kResolutionWidth,NS_lrbyFish::kResolutionHeight));

	// 按照屏幕尺寸缩放
	this->setScaleX(SCREEN_HEIGHT/(float)kResolutionWidth);
	this->setScaleY(SCREEN_WIDTH/(float)kResolutionHeight);	
	this->setAnchorPoint(Vec2::ZERO);
	this->setPosition(0,0);

	//在线奖励按钮
	//auto btnAward = Sprite::create("gameMenu/AwardBTN.png");
	//btnAward->setPosition(Vec2(80, NS_lrbyFish::kResolutionHeight - 380));
	//this->addChild(btnAward);

	menu_Award = MenuItemImage::create("709/gameMenu/AwardBTN.png", "", CC_CALLBACK_1(ByMenuLayer::onAward, this));
	menu_Award->setPosition(Vec2(80, NS_lrbyFish::kResolutionHeight - 380));
	menu_Award->setVisible(false);


	auto spTicketFrame = Sprite::create("709/gameMenu/TicketFrame.png");
	spTicketFrame->setPosition(Vec2(80, NS_lrbyFish::kResolutionHeight - 230));
	this->addChild(spTicketFrame);
	
	m_labelTicketScore = LabelTTF::create("8888", "SimHei", 20);
	m_labelTicketScore->setPosition(Vec2(spTicketFrame->getContentSize().width / 2, 12));
	m_labelTicketScore->setColor(ccc3(255, 237, 85));
	m_labelTicketScore->setAnchorPoint(Vec2(0.5, 0.5));
	spTicketFrame->addChild(m_labelTicketScore);

	spTicketFrame->setVisible(false);

	//充值按钮
	menu_Charge = MenuItemImage::create("709/gameMenu/ChargeBTN.png", "", CC_CALLBACK_1(ByMenuLayer::onCharge, this));
	menu_Charge->setPosition(Vec2(80, NS_lrbyFish::kResolutionHeight - 80));
	menu_Charge->setScale(1.2f);
	menu_Charge->setVisible(false);

	auto spStar = Sprite::create("709/Ticket/star_big.png");
	spStar->setPosition(Vec2(menu_Charge->getContentSize().width /2, menu_Charge->getContentSize().height / 2));
	menu_Charge->addChild(spStar, -1);	
	spStar->runAction(RepeatForever::create(RotateBy::create(6.0f, 360)));

	auto menu = Menu::create(menu_Charge, menu_Award, nullptr);
	menu->setPosition(Vec2(0, 0));
	this->addChild(menu, 10);

	//设置按钮 扇形弹出框
	isShow = false;

	menu_Setting = MenuItemImage::create("709/gameMenu/SetBTN.png", "709/gameMenu/SetBTN.png", CC_CALLBACK_1(ByMenuLayer::btnCallback, this));
	//menu_Setting->setPosition(Vec2(NS_lrbyFish::kResolutionWidth - 50, 120));
	menu_Setting->setPosition(Vec2(80, 150));
	menu_Setting->setScale(1.2f);

	//音乐
	menu_Music = MenuItemImage::create("709/gameMenu/MusicBTN.png", "709/gameMenu/MusicBTN.png", CC_CALLBACK_1(ByMenuLayer::onControlMusic, this));
	menu_Music->setPosition(Vec2(80, 150));
	menu_Music->setScale(1.2f);

	//音效
	menu_Effect = MenuItemImage::create("709/gameMenu/EffectBTN.png","709/gameMenu/EffectBTN.png", CC_CALLBACK_1(ByMenuLayer::onControlEffect, this));
	menu_Effect->setPosition(Vec2(80, 150));
	menu_Effect->setScale(1.2f);

	//介绍
	menu_Instro = MenuItemImage::create("709/gameMenu/InstroBTN.png", "709/gameMenu/InstroBTN.png", CC_CALLBACK_1(ByMenuLayer::onInstroduce, this));
	menu_Instro->setPosition(Vec2(80, 150));
	menu_Instro->setScale(1.2f);

	//退出
	menu_Exit = MenuItemImage::create("709/gameMenu/QuitBTN.png", "709/gameMenu/QuitBTN.png", CC_CALLBACK_1(ByMenuLayer::onQuitGame, this));
	menu_Exit->setPosition(Vec2(80, 150));
	menu_Exit->setScale(1.2f);

	m_menu = Menu::create(menu_Music, menu_Effect, menu_Instro, menu_Exit, menu_Setting, nullptr);
	m_menu->setPosition(Vec2::ZERO);
	m_menu->setName("m_menu");
	this->addChild(m_menu,10);
	return true;
}

void ByMenuLayer::onControlMusic(Ref* sender)
{
	bool music = UserDefault::getInstance()->getBoolForKey("Music");
	UserDefault::getInstance()->setBoolForKey("Music",!music);

	if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		menu_Music->setNormalImage(Sprite::create("709/gameMenu/MusicBTN_1.png"));
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}else
	{
		menu_Music->setNormalImage(Sprite::create("709/gameMenu/MusicBTN.png"));
		int nRand = rand()%4;
		if(nRand == 0)
		{
			Sound::getInstance()->playBgMusic(MUSIC_TYPE_BACKGROUND1);
		}
		else if(nRand == 1)
		{
			Sound::getInstance()->playBgMusic(MUSIC_TYPE_BACKGROUND2);
		}
		else if(nRand == 2)
		{
			Sound::getInstance()->playBgMusic(MUSIC_TYPE_BACKGROUND3);
		}
		else if(nRand == 3)
		{
			Sound::getInstance()->playBgMusic(MUSIC_TYPE_BACKGROUND4);
		}
	}
}

void ByMenuLayer::onControlEffect(Ref* sender)
{
	//默认为开 
	bool musicEffect = UserDefault::getInstance()->getBoolForKey("MusicEffect");

	if (musicEffect)
	{
		menu_Effect->setNormalImage(Sprite::create("709/gameMenu/EffectBTN_1.png"));
	}
	else
	{
		menu_Effect->setNormalImage(Sprite::create("709/gameMenu/EffectBTN.png"));
	}

	UserDefault::getInstance()->setBoolForKey("MusicEffect",!musicEffect);
}

void ByMenuLayer::onInstroduce(Ref* sender)
{
	IntroduceLayer *layer = IntroduceLayer::create();
	this->addChild(layer);
	layer->setPosition(Vec2((kResolutionWidth - layer->getContentSize().width) / 2 + 150, (kResolutionHeight - layer->getContentSize().height) / 2 + 50));	
}

void ByMenuLayer::onQuitGame(Ref* sender)
{
	menu_Exit->setEnabled(false);
	//LRBY_VIEW->onQuitGame(nullptr);
}

void ByMenuLayer::btnCallback(Ref* pSender)
{
	Sound *m_sound = Sound::getInstance();
	m_sound->playEffect(EFFECT_TYPE_CATCH);
	FiniteTimeAction* acf = (FiniteTimeAction*)CallFuncN::create(CC_CALLBACK_1(ByMenuLayer::enabledCallback, this));  
	if (isShow)  
	{  

		menu_Setting->runAction(RotateTo::create(0.3f, -720));  
		menu_Music->runAction(ByMenuLayer::spawnIn(0.6f));  
		menu_Effect->runAction(ByMenuLayer::spawnIn(0.65f));  
		menu_Instro->runAction(ByMenuLayer::spawnIn(0.7f));  
		menu_Exit->runAction(Sequence::create(ByMenuLayer::spawnIn(0.75f), acf, nullptr));  
	}  
	else  
	{   
		menu_Setting->runAction(RotateTo::create(0.3f, 720));  		
		menu_Music->runAction(ByMenuLayer::spawnOut(Vec2(menu_Setting->getPositionX() - SETTINGRADIUS  * cosf(M_PI / 12), menu_Setting->getPositionY() - SETTINGRADIUS * sinf(M_PI / 12)), 0.6f));  
		menu_Effect->runAction(ByMenuLayer::spawnOut(Vec2(menu_Setting->getPositionX() - SETTINGRADIUS * cosf(5 * M_PI / 36), menu_Setting->getPositionY() + SETTINGRADIUS * sinf(5 * M_PI / 36)), 0.65f));  
		menu_Instro->runAction(ByMenuLayer::spawnOut(Vec2(menu_Setting->getPositionX() - SETTINGRADIUS * cosf(13 * M_PI / 36), menu_Setting->getPositionY() + SETTINGRADIUS * sinf(13 * M_PI / 36)), 0.7f));   
		menu_Exit->runAction(Sequence::create(ByMenuLayer::spawnOut(Vec2(menu_Setting->getPositionX() - SETTINGRADIUS  * cosf(21*  M_PI / 36), menu_Setting->getPositionY() + SETTINGRADIUS  * sinf(21 * M_PI /36)), 0.75f), acf, nullptr));  

		//如果是开启的不管
 		if (UserDefault::getInstance()->getBoolForKey("Music"))
 		{
			menu_Music->setNormalImage(Sprite::create("709/gameMenu/MusicBTN.png"));			
 		}
 		else
		{
			menu_Music->setNormalImage(Sprite::create("709/gameMenu/MusicBTN_1.png"));
		}

		if (UserDefault::getInstance()->getBoolForKey("MusicEffect"))
		{
			menu_Effect->setNormalImage(Sprite::create("709/gameMenu/EffectBTN.png"));			
		}
		else
		{
			menu_Effect->setNormalImage(Sprite::create("709/gameMenu/EffectBTN_1.png"));
		}
	}  

	isShow = !isShow;  
	m_menu->setEnabled(false);  
}

void ByMenuLayer::enabledCallback(Ref* pSender)
{
	m_menu->setEnabled(true);
}

ActionInterval* ByMenuLayer::spawnOut(Vec2 point, float duration)
{
	ActionInterval* rotateTo = RotateTo::create(duration, 1080);  
	auto move_ease_out = EaseBackOut::create(MoveTo::create(duration, point));  
	return Spawn::create(move_ease_out, rotateTo, nullptr);  
}

ActionInterval* ByMenuLayer::spawnIn(float duration)
{
	ActionInterval* rotateTo = RotateTo::create(duration, -1080);  
	auto move_ease_in = EaseBackIn::create(MoveTo::create(duration, Vec2(80, 150)));  
	return Spawn::create(move_ease_in, rotateTo, nullptr);  
}

void ByMenuLayer::onAutoShoot(Ref* sender)
{
	//LRBY_VIEW->setAutoShoot(!LRBY_VIEW->getAutoShoot());

	//if(LRBY_VIEW->getAutoShoot())
	//{
	//	menu_AutoShoot->setNormalImage(Sprite::create("gameMenu/AutoShootBTN_Click.png"));
	//	if (!LRBY_VIEW->getScheduleAutoShoot())
	//	{
	//		LRBY_VIEW->setScheduleAutoShoot(true);
	//		//schedule(schedule_selector(LrbyClientView::GetInstance()->autoShootBullet), 0.25f);

	//		this->schedule(SEL_SCHEDULE(&LrbyClientView::autoShootBullet),0.25f);


	//		//this->schedule(SEL_SCHEDULE(&LRBY_VIEW->autoShootBullet),0.25f);

	//	}

	//}
	//else
	//{
	//	menu_AutoShoot->setNormalImage(Sprite::create("gameMenu/AutoShootBTN.png"));
	//	//unschedule(schedule_selector(LrbyClientView::GetInstance()->autoShootBullet));
	//	LRBY_VIEW->setScheduleAutoShoot(false);
	//}
}

void ByMenuLayer::autoHideMenu(float dt)
{
	FiniteTimeAction* acf = (FiniteTimeAction*)CallFuncN::create(CC_CALLBACK_1(ByMenuLayer::enabledCallback, this));  

	menu_Setting->runAction(RotateTo::create(0.3f, -720));  
	menu_Music->runAction(ByMenuLayer::spawnIn(0.6f));  
	menu_Effect->runAction(ByMenuLayer::spawnIn(0.65f));  
	menu_Instro->runAction(ByMenuLayer::spawnIn(0.7f));  
	menu_Exit->runAction(Sequence::create(ByMenuLayer::spawnIn(0.75f), acf, nullptr));  

	isShow = !isShow;  
	m_menu->setEnabled(false);  

	unschedule(schedule_selector(ByMenuLayer::autoHideMenu));
}

void ByMenuLayer::onCharge(Ref* pSender)
{
	//m_rechargelayer = ByRechargeLayer::create(1.2f);
	//this->addChild(m_rechargelayer,25, CHARGELAYER);
	//m_rechargelayer->setPosition(Vec2((kResolutionWidth - m_rechargelayer->getContentSize().width) / 2 + 150,
									//(kResolutionHeight - m_rechargelayer->getContentSize().height) / 2 + 50));
	//m_rechargelayer->SetBtnCallBack(this, SEL_TouchEvent(&ByMenuLayer::menuCloseCallback));
}

void ByMenuLayer::onAlms(bool bAlms)
{
	//救济金
	if (bAlms)// && m_isExchange)
	{
		struct tm *t;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		time_t timep;
		time(&timep);
		t = localtime(&timep);
#else
		timeval now;
		gettimeofday(&now,NULL);
		t = localtime(&now.tv_sec);
#endif
		int64_t newTime = (((100*(t->tm_year%100))+t->tm_mon)*100)+t->tm_mday;
		string str = UserDefault::getInstance()->getStringForKey(StringUtils::format("%d",EntityMgr::instance()->getDispatch()->m_dwUserID).c_str(),"0");
		int64_t oldTime =0; //stol(str);
		if(newTime>oldTime)
		{
			
		}
		else
		{
			auto charityLayer = CharityLeaveLayer::create(1.0f);
			charityLayer->setPosition((kResolutionWidth-charityLayer->getContentSize().width)/2, (kResolutionHeight-charityLayer->getContentSize().height)/2);
			this->addChild(charityLayer);
		}
	}
}

void ByMenuLayer::menuCloseCallback(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	//if(m_rechargelayer != nullptr)
	//{
	//	m_rechargelayer->removeFromParentAndCleanup(true);
	//	m_rechargelayer = nullptr;
	//	LrbyClientView::GetInstance()->setSuccess(true);
	//}
}

void ByMenuLayer::setTicketNumber(std::int32_t dwTicket)
{
	String str = StringUtils::toString(dwTicket);
	if(m_labelTicketScore)
	{
		m_labelTicketScore->setString(StringUtils::toString(dwTicket));
		m_dwTicketNumber = dwTicket;
	}
}

ByMenuLayer* ByMenuLayer::GetInstance()
{
	if (m_pMenuLayer == nullptr)
	{
		m_pMenuLayer = ByMenuLayer::create();
	}
	return m_pMenuLayer;
}

void ByMenuLayer::setExitMenuStatue(bool isEnabled)
{
	menu_Exit->setEnabled(isEnabled);
}

void ByMenuLayer::onAward(Ref* pSender)
{
	//m_awardLayer = AwardLayer::create();
	//this->addChild(m_awardLayer);
	//m_awardLayer->setPosition(Vec2((kResolutionWidth - m_awardLayer->getContentSize().width) / 2, (kResolutionHeight - m_awardLayer->getContentSize().height) / 2));

	////向服务器请求数据
	//CMissionPlaza::GetInstance()->QueryOnlineTime();

	//LRBY_VIEW->onOnLineAward(0);		
}

void ByMenuLayer::showAward(CMD_S_OnLineAward* onLineAward)
{
	m_awardLayer = AwardLayer::create();
	this->addChild(m_awardLayer);
	m_awardLayer->setPosition(Vec2((kResolutionWidth - m_awardLayer->getContentSize().width) / 2, (kResolutionHeight - m_awardLayer->getContentSize().height) / 2));

	////向服务器请求数据
	//CMissionPlaza::GetInstance()->QueryOnlineTime();
}

