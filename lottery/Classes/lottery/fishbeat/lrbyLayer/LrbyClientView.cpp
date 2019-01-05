#include "LrbyClientView.h"
#include "LrbyClientEngine.h"
#include <stdio.h>
#include <math.h>
#include "NetSprite.h"
#include "scene_fish_trace.h"
#include "ShakeScreen.h"
#include "AccountLayer.h"
#include "SystemLayer.h"
#include "LrbyGameConfig.h"
#include "math_aide.h"

//new add by zengxin
#include "ToolKit.h"//by hxh
#include "RoomLayer.h"
#include "LrbyClientEngine.h"
#include "FishSceneData.h"
#include "UINotificationCenter.h"
#include "Define.h"

#include "CharityLeaveLayer.h"
#include "IntroduceLayer.h"
#include "FishSettingLayer.h"
#include "DDZTimeCheckControl.h"
#include "LockLineLayer.h"
#include "LockFishManager.h"
#include "MyConfig.h"//by hxh

#ifdef WIN32
#include <time.h>
#include <sys/timeb.h>
#else
#include <arpa/inet.h>
#include <sys/time.h>
typedef unsigned short	WORD;
#endif

#define SETLAYER 1000
#define RECHARGELAYER 1001

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;

extern CLrbyGameConfig g_LrbyGameConfig;

//float ptPositionX[BY_GAME_PLAYER] = {
//	kResolutionWidth * 0.25,
//	kResolutionWidth * 0.5, 
//	kResolutionWidth * 0.75, 
//	kResolutionWidth - 40,
//	kResolutionWidth * 0.75,
//	kResolutionWidth * 0.5,
//	kResolutionWidth * 0.25,
//	40
//};

//float ptPositionY[BY_GAME_PLAYER] = {		
//	kResolutionHeight * 0.95,
//	kResolutionHeight * 0.95,
//	kResolutionHeight * 0.95,
//	kResolutionHeight * 0.5,
//	kResolutionHeight * 0.05 + 66,
//	kResolutionHeight * 0.05 + 66, 
//	kResolutionHeight * 0.05 + 66,
//	kResolutionHeight * 0.5 
//};

float ptPositionX[BY_GAME_PLAYER] = {
	214,
	kResolutionWidth * 0.5, 
	kResolutionWidth - 210, 
	kResolutionWidth - 44,
	kResolutionWidth - 214,
	kResolutionWidth * 0.5,
	214,
	44
};

float ptPositionY[BY_GAME_PLAYER] = {		
	kResolutionHeight * 0.94,
	kResolutionHeight * 0.94,
	kResolutionHeight * 0.94,
	kResolutionHeight * 0.5,
	kResolutionHeight * 0.05 + 66,
	kResolutionHeight * 0.05 + 66, 
	kResolutionHeight * 0.05 + 66,
	kResolutionHeight * 0.5 
};

//int ptPositionX2[BY_GAME_PLAYER] = {(int)(kResolutionWidth * 0.75), (int)(kResolutionWidth * 0.25), (int)(kResolutionWidth * 0.25), (int)(kResolutionWidth * 0.75)};
//int ptPositionY2[BY_GAME_PLAYER] = {(int)(kResolutionHeight * 0.05), (int)(kResolutionHeight * 0.05), (int)(kResolutionHeight * 0.95), (int)(kResolutionHeight * 0.95)};

/*
float ptPositionX2[BY_GAME_PLAYER] = {
	kResolutionWidth * 0.25,
	kResolutionWidth * 0.5, 
	kResolutionWidth * 0.75, 
	kResolutionWidth,
	kResolutionWidth * 0.75,
	kResolutionWidth * 0.5,
	kResolutionWidth * 0.25,
	0
};

float ptPositionY2[BY_GAME_PLAYER] = {		
	kResolutionHeight * 0.95,
	kResolutionHeight * 0.95,
	kResolutionHeight * 0.95,
	kResolutionHeight * 0.5,
	kResolutionHeight * 0.05,
	kResolutionHeight * 0.05, 
	kResolutionHeight * 0.05,
	kResolutionHeight * 0.5 
};
*/
//////////////////////////////////////////////////////////////////////////
class CCParticleExplosionEx : public CCParticleExplosion
{
public:
	CCParticleExplosionEx();
	~CCParticleExplosionEx();

	static CCParticleExplosionEx* create();

	//! Initializes a particle
	virtual void initParticle(ParticleSystem* particle);

public:

};

CCParticleExplosionEx::CCParticleExplosionEx()
{
}

CCParticleExplosionEx::~CCParticleExplosionEx()
{
}

void CCParticleExplosionEx::initParticle(ParticleSystem* particle)
{
	//particle->rotation = 180;// particle->
	//CCParticleExplosion::ParticleSystem(particle);
	//particle->rotation+=90;

}

CCParticleExplosionEx* CCParticleExplosionEx::create()
{
	CCParticleExplosionEx* ret = new CCParticleExplosionEx();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
LrbyClientView * LrbyClientView::m_pLrbyClientView = nullptr;					

LrbyClientView::LrbyClientView():
	isOk(false),
	m_bLockScreen(false),
	m_spGameBG(nullptr),
	m_bGameConfig(false),
	//m_pExchangeGold(nullptr),
	m_nCurChair(-1),
	m_nodegrid(nullptr),
	spScene(nullptr),
	m_bAutoShoot(false),
	m_bScheduleAutoShoot(false),
	m_accountLayer(nullptr),
	m_awardLayer(nullptr),
	m_isNext(true),
	m_isSuccess(false),
	m_isEnergy(false),
	m_userName(NULL),
	m_userGold(NULL),
	m_layerBottom(NULL),
	m_nMenuShowTime(0),
	m_current_bullet_mulriple(0),
	m_current_bullet_kind(0),
	menu_setting(NULL),
	menu_help(NULL),
	menu_exit(NULL),
	isShowTipArrow(false),
	m_isLockFish(false)
{
	m_nBgIndex = 0;
	m_nBgSoundIndex=0;
	for(int i = 0; i < BY_GAME_PLAYER; i ++)
	{
		m_spCannonION[i] = nullptr;
		m_fireBulletNum[i] = 0;
		m_lockFishIcon[i] = NULL;
	}

	m_pLrbyClientView = this;
	m_isFirst = true;
	m_isShowEntity = true;

	//add by zengxin
	CCLOG("xin::debug LrbyClientView::addObserver");
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LrbyClientView::OnMessage),MESSAGE_TOVIEW,NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LrbyClientView::onLoginFaild), MSG_UI_ANS_ROOMLOGINFAIL, NULL);
	//NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DDZSceneView::onSitFaild), MSG_UI_ANS_SITFAIL, NULL);

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LrbyClientView::netDissconnect), MSG_UI_ANS_NETWORKSHUT, NULL);
}

LrbyClientView::~LrbyClientView()
{
	
	CC_SAFE_RELEASE(m_netPool);
	//NotificationCenter::getInstance()->removeAllObservers(this);

	NotificationCenter::getInstance()->removeObserver(this, MESSAGE_TOVIEW);

	CUINotificationCenter::getInstance()->removeObserver(this, "update_userinfo");
	CUINotificationCenter::getInstance()->removeObserver(this, "game_fishMessage");

	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_ROOMLOGINFAIL);
	//NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_SITFAIL);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_NETWORKSHUT);

	m_LockFishManager = NULL;
}

void LrbyClientView::onExit()
{
	Layer::onExit();
	FishManager::GetInstance()->destroy();
}

void LrbyClientView::onEnter()
{
	Layer::onEnter();
}
 
// on "init" you need to initialize your instance
bool LrbyClientView::init()
{
	if (!Layer::init())
	{
		return false;
	}
	DDZTimeCheckControl::getInstance()->resetTimes();

	//初始化声音
	if ( SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		
	}

	{
		int nRand = rand()%4;
		if(nRand == 0)
		{
			Sound::getInstance()->playBgMusic(MUSIC_TYPE_BACKGROUND2);
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

	//监听返回键
	
	auto listener_back = EventListenerKeyboard::create();
	listener_back->onKeyReleased = [=](EventKeyboard::KeyCode keycode, Event *event){	

		if(keycode == EventKeyboard::KeyCode::KEY_BACK)
		{
			event->stopPropagation();
			if (this->getChildByTag(6626))
			{
				return;
			}
			auto setting = FishSettingLayer::create();
			setting->setLayerType(TAG_LEAVE);
			setting->setPosition(Vec2((kResolutionWidth - setting->getContentSize().width) / 2, (kResolutionHeight - setting->getContentSize().height) / 2));
			setting->setTag(6626);
			this->addChild(setting, 100);
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_back, this);

	m_dt = 1.0f;
	this->setContentSize(cocos2d::Size(NS_lrbyFish::kResolutionWidth,NS_lrbyFish::kResolutionHeight));

	//设置显示大小
	this->setScaleX(SCREEN_HEIGHT/(float)kResolutionWidth);
	this->setScaleY(SCREEN_WIDTH/(float)kResolutionHeight);	
	this->setAnchorPoint(Vec2::ZERO);
	this->setPosition(0,0);

	m_current_bullet_kind = BULLET_KIND_1_NORMAL;
	auto winsize = cocos2d::Size(kResolutionWidth,kResolutionHeight);
	m_ptLastPos = Vec2(winsize.width * 0.5, winsize.height * 0.5);
	m_bAutoShoot = false;
	m_isShowSetting = false;
	m_bScheduleAutoShoot = false;

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(LrbyClientView::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(LrbyClientView::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(LrbyClientView::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	if (UserDefault::getInstance()->getBoolForKey("Music"))
	{
		m_sound = Sound::getInstance();
		m_sound->playBgMusic(MUSIC_TYPE_BACKGROUND1);
	}
	
	initUi();
	m_spGameBG = Sprite::create("709/Scene/bg1.jpg");
	m_spGameBG->setName("ly_scene_bg");
	m_spGameBG->setScale((float)kResolutionHeight/640);
	m_spGameBG->setPosition(kResolutionWidth / 2, kResolutionHeight / 2);
	this->addChild(m_spGameBG, 0);
	initData();

	m_LockFishManager = LockFishManager::create();
	this->addChild(m_LockFishManager, 18);

	this->addChild(FishManager::GetInstance(), 5);//初始化鱼管理类
	CUINotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LrbyClientView::updateUserInfo), "update_userinfo", NULL);
	CUINotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LrbyClientView::onGameFishMessage), "game_fishMessage", NULL);

	MyConfig& myConfig=MyConfig::Instance();
	menu_AutoShoot = MenuItemImage::create("709/fish_cannon/btn_auto_shoot.png", "", CC_CALLBACK_1(LrbyClientView::onAutoShoot, this));
	//menu_AutoShoot->setPosition(Vec2(90, 120));
	menu_AutoShoot->setPosition(Vec2(myConfig.m_CannonNodePos[3][0],myConfig.m_CannonNodePos[3][1]));

	menu_LockFish = MenuItemImage::create("709/fish_cannon/btn_lock_fish.png", "709/fish_cannon/btn_lock_fish_click.png", CC_CALLBACK_1(LrbyClientView::onLockFish, this));
	menu_LockFish->setPosition(Vec2(1280 - 115, 120));

	menu_UnLockFish = MenuItemImage::create("709/fish_cannon/btn_un_lock_fish.png", "709/fish_cannon/btn_un_lock_fish.png", CC_CALLBACK_1(LrbyClientView::onUnLockFish, this));
	menu_UnLockFish->setPosition(Vec2(1280 - 55, 120));

	auto menu = Menu::create(menu_AutoShoot, menu_LockFish, menu_UnLockFish, nullptr);
	menu->setPosition(Vec2(0, 0));
	this->addChild(menu, 25);
	initMenuBtn();

	m_nodegrid = NodeGrid::create();
	Sprite* sp = Sprite::create("709/Scene/water1.png");
	sp->setScale(((float)kResolutionHeight/512)*1.2f);
	sp->setPosition(Vec2(winsize.width*0.5, winsize.height*0.5));
	m_nodegrid->addChild(sp);
	this->addChild(m_nodegrid,20);
	m_nodegrid->runAction(CCRepeatForever::create(Ripple3D::create(10.0f, CCSize(20, 10), Vec2(winsize.width*0.5, winsize.height*0.5), 800, 5, 200)));		//Waves3D::create(5.0f, CCSize(10, 10), 5, 10)));

	this->scheduleUpdate();
	return true;
}

//test code 
void LrbyClientView::onBackPress(Ref* sender)
{
	auto setting = FishSettingLayer::create();
	setting->setLayerType(TAG_LEAVE);
	setting->setTag(6626);
	setting->setPosition(Vec2((kResolutionWidth - setting->getContentSize().width) / 2, (kResolutionHeight - setting->getContentSize().height) / 2));
	this->addChild(setting, 100);
}

void LrbyClientView::initUi()
{
	m_strMessage = "";
	labelMessageTxt = LabelTTF::create(m_strMessage, "SimHei", 40);
	labelMessageTxt->setColor(cocos2d::Color3B::YELLOW);
	labelMessageTxt->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelMessageTxt->setPosition(Vec2(kResolutionWidth / 2, kResolutionHeight - 300));
	this->addChild(labelMessageTxt, 6);

	for(int i = 0; i < BY_GAME_PLAYER; i ++)
	{

		cannon[i] = CannonNode::create(i);		//炮塔初始化
		//设置炮塔位置
		(cannon[i])->setPosition(Vec2(ptPositionX[i], ptPositionY[i]));		
		this->addChild(cannon[i], 26);
		//初始化其它数据
		(cannon[i])->setCannonNumber(0);
		(cannon[i])->setScoreNumber(0L);
		(cannon[i])->setCannonStatue(BULLET_KIND_2_NORMAL);

		int nY = 130;
		int nX = -50;

		if (i >= 0 && i <= 2)
		{
			nY = -130;
			nX = -50;
		}
		if (i == 3)
		{
			nY = 65;
			nX = -130;
		}
		if (i >= 4 && i <= 6)
		{
			nY = 130;
			nX = -50;
		}
		if (i == 7)
		{
			nY = 65;
			nX = 130;
		}
		
		//炮塔角度
		(cannon[i])->setRotation(kChairDefaultAngleOther[i]);	
		
		m_spCannonION[i] = Sprite::create("709/card_ion.png");	
		Vec2 ionPos(cannon[i]->getPosition().x + nX, cannon[i]->getPosition().y + nY);
		m_spCannonION[i]->setPosition(ionPos);
		this->addChild(m_spCannonION[i], 20);
		m_spCannonION[i]->runAction(RepeatForever::create(Sequence::create(
			MoveTo::create(0.3f, Vec2(ionPos.x + 10, ionPos.y)),
			MoveTo::create(0.3f, Vec2(ionPos.x, ionPos.y - 10)),
			MoveTo::create(0.3f, Vec2(ionPos.x - 10, ionPos.y)),
			MoveTo::create(0.3f, Vec2(ionPos.x, ionPos.y+ 10)), nullptr)));
		m_spCannonION[i]->setVisible(false);

		int rotate = 270;
		if (i == 3 || i == 7)
		{
			nX = nX;
			nY = nY - 90;
			if (i == 7)
			{
				rotate = 90;
			}
		}
		else
		{
			rotate = 0;
			nX = nX + 110;
			nY = nY;
		}

		m_lockFishIcon[i] = Sprite::createWithSpriteFrameName("lock_flag_30.png");
		Vec2 ionPosLock(cannon[i]->getPosition().x + nX, cannon[i]->getPosition().y + nY);
		m_lockFishIcon[i]->setPosition(ionPosLock);
		this->addChild(m_lockFishIcon[i], 20);
		m_lockFishIcon[i]->runAction(RepeatForever::create(Sequence::create(
			MoveTo::create(0.3f, Vec2(ionPosLock.x + 10, ionPosLock.y)),
			MoveTo::create(0.3f, Vec2(ionPosLock.x, ionPosLock.y - 10)),
			MoveTo::create(0.3f, Vec2(ionPosLock.x - 10, ionPosLock.y)),
			MoveTo::create(0.3f, Vec2(ionPosLock.x, ionPosLock.y+ 10)), nullptr)));
		m_lockFishIcon[i]->setRotation(rotate);
		m_lockFishIcon[i]->setVisible(false);

	}

	isOk = true;
}
//初始化鱼网
void LrbyClientView::initData()
{
	m_netPool = NetPool::createNetPool(1);
	CC_SAFE_RETAIN(m_netPool);
}

std::string LrbyClientView::getSdCardLuJing(std::string szFileName)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	log("GuaranteeList::getSdCardLuJing");
	std::string fileName = "/sdcard/game_wuxinghw/";
	if (access(fileName.c_str(), 0) != 0) {
		if (mkdir(fileName.c_str(), 777) != 0) {
			return "";
		}
	}

	return fileName;
#else
	return "";
#endif
}

bool LrbyClientView::onTouchBegan(Touch *touch, Event *event)
{
	if(!m_bGameConfig)
		return true;
	if((m_nCurChair < 0) || (m_nCurChair > BY_GAME_PLAYER - 1))
		return true;

	//正在发射子弹
	static bool isShooting = false;
	if(isShooting) return true;
	isShooting = true;

	this->scheduleOnce([=](float dt){isShooting = false;},0.1f,"TouchShoot");

	//可以开炮
	if(!isCanFire())
	{		
		return true;
	}

	Vec2 location = touch->getLocation();
	Vec2 ptCannon = (cannon[m_nCurChair])->getPosition();	
	
	location.x = location.x *kResolutionWidth/SCREEN_HEIGHT;
	location.y = location.y *kResolutionHeight/SCREEN_WIDTH;
	
	m_ptLastPos = location;
	float angle = MathAide::CalcAngle(location.x, location.y, ptCannon.x, ptCannon.y);
	if (m_LockFishManager->GetLockFishID(m_nCurChair) == 0)
	{
		m_isLockFish = false;
		m_LockFishManager->ClearLockTrace(m_nCurChair);
		showLockFishIcon(m_nCurChair, 0, 0);
	}
	if (!m_isLockFish)
	{
		(cannon[m_nCurChair])->setCannonAngle(angle + kChairDefaultAngle[m_nCurChair]);	
	}
	else
	{
		angle = cannon[m_nCurChair]->getCannonAngle();
		angle = angle - kChairDefaultAngle[m_nCurChair];
		auto fish1 = FishManager::GetInstance()->getFishById(m_LockFishManager->GetLockFishID(m_nCurChair));
		if (fish1)
		{
			m_ptLastPos = fish1->getFishPosition();
		}
	}
	
	if (m_bScheduleAutoShoot)
	{
		return true;
	}

	if(m_bAutoShoot)
	{
		return true;
	}

	//是否在切换场景
	if(g_LrbyGameConfig.IsSwitchingScene())
	{
		return true;
	}

	//子弹的属性
	CMD_C_UserFire userFire;
	memset(&userFire, 0, sizeof(userFire));
	userFire.angle = angle_range(M_PI_2 - angle);

	
	userFire.bullet_kind = (BulletKind)m_current_bullet_kind;
	userFire.bullet_mulriple = m_current_bullet_mulriple;//
	userFire.lock_fishid = m_LockFishManager->GetLockFishID(m_nCurChair);
	//userFire.lock_fishid = 0;
	//发送子弹属性给服务器
	EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_USER_FIRE, &userFire, sizeof(CMD_C_UserFire));

	return true;
}

void LrbyClientView::onTouchMoved(Touch *touch, Event *event)
{
	if (!m_isLockFish && m_LockFishManager->GetLockFishID(m_nCurChair) == 0)
	{
		m_ptLastPos = touch->getLocation();


		m_ptLastPos.x = m_ptLastPos.x *kResolutionWidth/SCREEN_HEIGHT;
		m_ptLastPos.y = m_ptLastPos.y *kResolutionHeight/SCREEN_WIDTH;
	}
	if (!m_bScheduleAutoShoot)
	{
		m_bScheduleAutoShoot = true;
		schedule(schedule_selector(LrbyClientView::autoShootBullet), 0.25f);
	}
}

void LrbyClientView::onTouchEnded(Touch *touch, Event *event)
{
	if (!m_bAutoShoot)
	{
		unschedule(schedule_selector(LrbyClientView::autoShootBullet));
		m_bScheduleAutoShoot = false;
	}	
}

void LrbyClientView::onTouchCancelled(Touch *touch, Event *event)
{
	//log("LrbyClientView::onTouchCancelled");
}

void LrbyClientView::updateUserInfo(Object* obj)
{	
	if (!isOk)
	{
		initUi();
	}

	for(int i = 0; i < BY_GAME_PLAYER; i++)
	{
		auto userItem = EntityMgr::instance()->roomFrame()->getUserItemByChairID(i);
		if (!userItem)
		{
			cannon[i]->setNickName("");
			cannon[i]->setScoreNumber(0);
			continue;
		}
		tagUserData* pUserDate = userItem->getUserData();
		if (!pUserDate)
		{
			continue;
		}
		if ( pUserDate == nullptr)
		{
			continue;
		}
		switch (pUserDate->cbUserStatus)
		{
		case US_NULL:
			break;
		case US_FREE:
			break;
		case US_SIT:
			break;
		case US_READY:
			break;
		case US_LOOKON:
			break;
		case US_PLAYING:
			{
				string strNickName=pUserDate->szNickName;
				if (i != m_nCurChair)
		        {
					strNickName = ToolKit::dealStringOfPrivacy(strNickName);	
				}
				cannon[i]->setNickName(strNickName);
			}
			break;
		case US_OFFLINE:
			break;
		default:
			{
				cannon[i]->setNickName("");
			}
			break;
		}
		if (i == m_nCurChair)
		{
			initUserInfo(pUserDate->szNickName, pUserDate->lScore);
			//addTipsView();

			if (isShowTipArrow)
			{
				continue;
			}
			isShowTipArrow = true;
			addUserPositionTipsView(m_nCurChair);
		}
	}
}
void LrbyClientView::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
	return;
#endif

	onQuitGame(nullptr);	//退出游戏调用

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void LrbyClientView::update(float dt)
{
	if (DDZTimeCheckControl::getInstance()->getPauseTime() > 20)
	{
		if (EntityMgr::instance()->getDispatch()->isGameLinkOK())
		{
			NS_lrbyFish::CMD_C_ExchangeFishScore exchangeFishScore;
			memset(&exchangeFishScore, 0, sizeof(NS_lrbyFish::CMD_C_ExchangeFishScore));
			exchangeFishScore.increase = 0;
			EntityMgr::instance()->getDispatch()->sendSocketDate(NS_lrbyFish::SUB_C_EXCHANGE_FISHSCORE, &exchangeFishScore, sizeof(NS_lrbyFish::CMD_C_ExchangeFishScore));
			EntityMgr::instance()->getDispatch()->SendPacketWithPerformStandup();

			this->scheduleOnce([](float dt){
				EntityMgr::instance()->getDispatch()->closeGameSocket();
			}, 0.1f, "FishLeaveGame");

			//EntityMgr::instance()->getDispatch()->closeGameSocket();
		}
		this->scheduleOnce(schedule_selector(LrbyClientView::checkUserEnterBackground), 1.0f);
	}
	m_dt += dt;
	if (m_dt<0.023)
	{
		return;
	}
	m_dt = 0;

	BoundingBox box;
	Rect rect(0,0,kResolutionWidth, kResolutionHeight);
	FishSprite* fish = nullptr;
	bool bFirst = true;
	for(int i = 0; i < FishManager::GetInstance()->m_fishList.size();)
	{
		fish = FishManager::GetInstance()->m_fishList.at(i);

		//设置捕鱼的锁定鱼界面
		for (int i = 0; i < BY_GAME_PLAYER; i++)
		{
			int fishId = m_LockFishManager->GetLockFishID(i);
			if (fishId == fish->getFishId())
			{
				createLockLineView(i, fish);
			}
			else if (fishId <= 0)
			{
				m_LockFishManager->ClearLockTrace(i);
				showLockFishIcon(i, 0, 0);
			}
		}
		
		if (!m_bLockScreen)
		{
			if (!fish->updateFish(dt))
				continue;
		}
		++i;  // 
		if (fish->getFishStatue() == FISH_DIE)
		{
			continue;
		}

		fish->getFishBoundingBox(box);

		// 
		for(int h = 0; h < BY_GAME_PLAYER; h ++)
		{
			for(int j = 0; j < (m_vecBullet[h]).size();)
			{
				BulletSprite* bullet = (m_vecBullet[h]).at(j);

				if (bFirst) bullet->updateBullet(dt);
				//
				const Vec2 ptBullet = bullet->getBulletPosition();
				// 
				
				if (bullet->getLockFishId() <= 0)
				{//普通子弹
					if(!box.ComputeCollision(ptBullet.x, ptBullet.y))
					{
						j++;
						continue;
					}
				}
				else
				{//锁定子弹
					if (bullet->getLockFishId() == fish->getFishId())
					{
						if(!box.ComputeCollision(ptBullet.x, ptBullet.y))
						{
							j++;
							continue;
						}
					}
					else
					{
						j++;
						continue;
					}
				}
				// 
				if(((h == m_nCurChair) || (bullet->getAndroidChairID() == m_nCurChair)))// &&  !//LKPY_ENGINE->IsLookonMode())
				{
					if(fish->getFishKind() == FISH_KIND_21)
					{
						CMD_C_HitFishLK hit_fish;
						hit_fish.fish_id = fish->getFishId();
						//verysharp //hit_fish.bulletmul= bullet->m_bulletMulriple;
						EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_HIT_FISH_I, &hit_fish, sizeof(hit_fish));
					}
					CMD_C_CatchFish catchFish;
					catchFish.chair_id = h;
					catchFish.fish_id = fish->getFishId();
					catchFish.bullet_kind = (BulletKind)bullet->m_bulletStatue;
					catchFish.bullet_id = bullet->getBulletId();
					catchFish.bullet_mulriple = bullet->m_bulletMulriple;
					//verysharp //catchFish.dwCatchFishTick = GetTickCount();
					EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_CATCH_FISH, &catchFish, sizeof(CMD_C_CatchFish));					
					fish->playHitedAnimation(h == m_nCurChair);
				}
				m_fireBulletNum[h]--;
				// 
				addNet(ptBullet, bullet->m_bulletStatue);
				bullet->removeBullet();
				(m_vecBullet[h]).eraseObject(bullet);
			}
		}
		if (bFirst)
		{
			bFirst = false;
		}
	}

	if (bFirst)
	{
		for(int h = 0; h < BY_GAME_PLAYER; h ++)
		{
			for(int j = 0; j < (m_vecBullet[h]).size(); ++j)
			{
				BulletSprite* bullet = (m_vecBullet[h]).at(j);
				bullet->updateBullet(dt);
			}
		}
	}

	//LvLog("qlog: sss dr tick %u", GetTickCount()-dwTick);
}

void LrbyClientView::onGameFishMessage(Object* obj)
{
	CUINotificationBufferPtr NotifRefPtr(dynamic_cast<CUINotificationBuffer *>(obj));
	if(NotifRefPtr == nullptr) return;

	switch (NotifRefPtr->GetID())
	{
	case em_GM_CONFIG:
		{
			setBulletMulriple(NotifRefPtr->GetCode());
		}break;
	case em_GM_FishTrace:
		{
			std::int64_t pointer = NotifRefPtr->GetCode();
			FishSprite *pFish = (FishSprite *)(pointer);
			if(pFish == nullptr) return;
			FishManager::GetInstance()->addFish(pFish);
			pFish->autorelease();
		}
		break;
	case em_GM_HARVEST:
		{
			void* data = NotifRefPtr->GetData();
			WORD data_size = NotifRefPtr->GetSize();
			CMD_S_FireHarvest* harvest = (CMD_S_FireHarvest*)(data);

			if (m_accountLayer == nullptr)
			{
				m_accountLayer = AccountLayer::create();
				m_accountLayer->setPosition((kResolutionWidth-m_accountLayer->getContentSize().width)/2,(kResolutionHeight-m_accountLayer->getContentSize().height)/2);
				this->addChild(m_accountLayer, 30);
				m_accountLayer->setAccountLayerInfo(harvest);
			}
		}
		break;
	case em_GM_ExchangeFishScore:
		{
			void* data = NotifRefPtr->GetData();
			WORD data_size = NotifRefPtr->GetSize();
			CMD_S_ExchangeFishScore* exchangeFishScore = static_cast<CMD_S_ExchangeFishScore*>(data);
			int  nChangeChair = exchangeFishScore->chair_id;

			(cannon[nChangeChair])->setScoreNumber(cannon[nChangeChair]->getScoreNumber() + exchangeFishScore->swap_fish_score);
			m_isSuccess = true;
		}
		break;
	case em_GM_AwardScore:
		{
			void* data = NotifRefPtr->GetData();
			WORD data_size = NotifRefPtr->GetSize();
			CMD_S_AwardScore* awardscore = static_cast<CMD_S_AwardScore*>(data);
			int nChair = awardscore->chair_id;
			cannon[nChair]->setScoreNumber(cannon[nChair]->getScoreNumber() + awardscore->award_score);

			if(m_awardLayer)
			{			
				m_awardLayer->setCurStatus(awardscore->curStep);				
			}
		}
		break;
	case em_GM_UserFire:
		{
			if(g_LrbyGameConfig.IsSwitchingScene())
			{
				return;
			}

			void* data = NotifRefPtr->GetData();
			WORD data_size = NotifRefPtr->GetSize();
			CMD_S_UserFire* user_fire = static_cast<CMD_S_UserFire*>(data);

			//锁定鱼判断
			int lockFishId = user_fire->lock_fishid;

			if (lockFishId > 0)
			{
				m_LockFishManager->SetLockFishID(user_fire->chair_id, lockFishId);
			}
			if (lockFishId > 0)
			{
				auto lockFish = FishManager::GetInstance()->getFishById(lockFishId);
				if (lockFish == NULL)
				{
					if (user_fire->chair_id == m_nCurChair)
					{
						FishKind new_fish_kind;
						int newFishId = FishManager::GetInstance()->lockFish(&new_fish_kind);
						if (newFishId > 0)
						{
							lockFishId = newFishId;
							m_LockFishManager->SetLockFishID(m_nCurChair, newFishId);
							m_LockFishManager->SetLockFishKind(m_nCurChair, new_fish_kind);
						}
					}
					else
					{
						m_LockFishManager->ClearLockTrace(user_fire->chair_id);
						showLockFishIcon(user_fire->chair_id, 0, 0);
					}
				}
				else
				{
					m_LockFishManager->SetLockFishKind(user_fire->chair_id, static_cast<FishKind>(lockFish->getFishKind()));
				}
			}
			showLockFishIcon(user_fire->chair_id, lockFishId, m_LockFishManager->GetLockFishKind(user_fire->chair_id));
			BulletSprite* pBullet = BulletSprite::create();
			pBullet->setLockFishId(m_LockFishManager->GetLockFishID(user_fire->chair_id));
			if (lockFishId > 0)
			{
				auto lockFish = FishManager::GetInstance()->getFishById(lockFishId);
				pBullet->setLockFish(lockFish);
			}
			int nShootChair = user_fire->chair_id;
			m_fireBulletNum[nShootChair]++;
			Vec2 ptCannon = Vec2(ptPositionX[nShootChair], ptPositionY[nShootChair]);
			
			float fAngle = angle_range(M_PI_2 - user_fire->angle);
			pBullet->setBulletStatue(user_fire->bullet_kind, user_fire->bullet_mulriple, ptCannon, fAngle, nShootChair);

			fAngle -= kChairDefaultAngle[nShootChair];

			if(nShootChair == 3 || nShootChair == 7) fAngle += M_PI;

			//不是自己在开炮
			if(nShootChair != m_nCurChair)
			{
				setOtherBulletMulriple(nShootChair, user_fire->bullet_mulriple);
				(cannon[nShootChair])->setCannonStatue(user_fire->bullet_kind);			

				//if (m_LockFishManager->GetLockFishID(nShootChair) <= 0)
				//{
				//	(cannon[nShootChair])->setCannonAngle(fAngle);
				//}
				(cannon[nShootChair])->setCannonAngle(fAngle);
			}
			LONGLONG tem11 = cannon[nShootChair]->getScoreNumber();
			(cannon[nShootChair])->setScoreNumber(cannon[nShootChair]->getScoreNumber() + user_fire->fish_score);
			(cannon[nShootChair])->cannonFire();

			//自已开炮
			if (nShootChair == m_nCurChair)
			{
				if (cannon[m_nCurChair]->getScoreNumber() < m_current_bullet_mulriple)
				{
					if(cannon[m_nCurChair]->getScoreNumber() > 10 && cannon[m_nCurChair]->getScoreNumber() <= 100)
					{
						m_current_bullet_mulriple = cannon[m_nCurChair]->getScoreNumber() / 10 * 10;
					}
					else if(cannon[m_nCurChair]->getScoreNumber() > 100 && cannon[m_nCurChair]->getScoreNumber() <= 1000)
					{
						m_current_bullet_mulriple = cannon[m_nCurChair]->getScoreNumber() / 100 * 100;
					}
					else if(cannon[m_nCurChair]->getScoreNumber() > 1000 && cannon[m_nCurChair]->getScoreNumber() <= 10000)
					{
						m_current_bullet_mulriple = cannon[m_nCurChair]->getScoreNumber() / 1000 * 1000;
					}
					else if(cannon[m_nCurChair]->getScoreNumber() > 10000 && cannon[m_nCurChair]->getScoreNumber() <= 100000)
					{
						m_current_bullet_mulriple = cannon[m_nCurChair]->getScoreNumber() / 10000 * 10000;
					}	

					if (cannon[m_nCurChair]->getScoreNumber() <  g_LrbyGameConfig.m_gameConfig.min_bullet_multiple)
					{
						m_current_bullet_mulriple = g_LrbyGameConfig.m_gameConfig.min_bullet_multiple;
					}
				}	
				cannon[m_nCurChair]->setCannonNumber(m_current_bullet_mulriple);
			}

			pBullet->setAndroidChairID(user_fire->android_chairid);
			pBullet->setBulletId(user_fire->bullet_id);	
			this->addChild(pBullet, 8);
			m_vecBullet[nShootChair].pushBack(pBullet);

			//正在播放
			static bool isBee = false;
			if(isBee) return;
			isBee = true;

			this->scheduleOnce([=](float dt){isBee = false;},0.1f,"PlayEffect");
			
			if (user_fire->bullet_kind<=BULLET_KIND_4_NORMAL)
			{
				m_sound->playEffect(EFFECT_TYPE_FIRE);
			}
			else
			{
				m_sound->playEffect(EFFECT_TYPE_ION_FIRE);
			}
		}
		break;
	case em_GM_CatchFish:
		{
			Vec2 ptCatchFishPos = Vec2(0.0f, 0.0f);
			FishSprite* fish = NULL;
			void* data = NotifRefPtr->GetData();
			WORD data_size = NotifRefPtr->GetSize();
			CMD_S_CatchFish* catch_fish = static_cast<CMD_S_CatchFish*>(data);

			int nChairID = catch_fish->chair_id;

			for(int i = 0; i < FishManager::GetInstance()->m_fishList.size(); i ++)
			{
				fish = FishManager::GetInstance()->m_fishList.at(i);
				if (fish->getFishId() != catch_fish->fish_id)
				{
					continue;
				}

				ptCatchFishPos = fish->getFishPosition();
				int nRand = rand()%2;
				static DWORD dwLastFishSound = 0;
						
				//播放声音
				playCatchEffect(nRand, fish->getFishKind());

				//播放金币动画及显示
				playCatchGold(ptCatchFishPos, catch_fish->fish_score, catch_fish->chair_id);

				cannon[catch_fish->chair_id]->setScoreNumber(cannon[catch_fish->chair_id]->getScoreNumber() + catch_fish->fish_score);
				
				if(fish->getFishKind() == FISH_KIND_22)		//定屏炸弹
				{
					setScreenLock(true);

					ParticleSystem* emitter = ParticleSystemQuad::create("709/dingParticle.plist");
					emitter->retain();
					emitter->setPosition(ptCatchFishPos);
					emitter->setEmissionRate(100);
					this->addChild(emitter, 20);
					emitter->release();
					emitter->runAction(Sequence::create(DelayTime::create(2.0f), CallFuncN::create(CC_CALLBACK_1(LrbyClientView::removeGold, this, (Node*)emitter)), nullptr));
				}

				int nY = 150;
				if(nChairID == 0)
				{
					nY = -150;
				}	

				if(fish->getFishKind() == FISH_KIND_22)
				{
					fish->removeFish();
				}
				else
				{
					fish->playDieAnimation();
				}
				break; // 
			}

			if ((catch_fish->fish_kind >= FISH_KIND_18 && catch_fish->fish_kind <= FISH_KIND_21) ||
				(catch_fish->fish_kind >= FISH_KIND_25 && catch_fish->fish_kind <= FISH_KIND_30))
			{
				const float bingoXPos[6] = {270.0, 756.0, 1210.0, 1180.0, 608.0, 132.0};
				const float bingoYPos[6] = {527.0, 527.0, 527.0, 256.0, 256.0, 256.0};
				//是否允许振动
				//if(UserDefault::getInstance()->getBoolForKey("Cartoon"))
				{	
					CCParticleExplosionEx  *pg = CCParticleExplosionEx::create();
					pg->setStartSize(25);
					pg->setStartSizeVar(10);
					pg->setEndSize(280);
					pg->setEndSizeVar(50);
					pg->setStartColor(Color4F(1,1,1,1));
					pg->setStartColorVar(Color4F(0,0,0,0.01));
					pg->setEndColor(Color4F(1,1,1,0.01));
					pg->setEndColorVar(Color4F(0,0,0,0.01));
					pg->setTotalParticles(120);
					pg->setSpeed(700);
					pg->setSpeedVar(260);
					pg->setRotationIsDir(true);
					pg->setLife(0.9);
					pg->setTexture(CCTextureCache::sharedTextureCache()->addImage("709/Flash_Ice_2.png"));
					pg->setAutoRemoveOnFinish(true);
					pg->setPosition(ptCatchFishPos);
					this->addChild(pg,20);						

					if (UserDefault::getInstance()->getBoolForKey("ShakeScreen"))
					{
						ShakeScreen* shake = ShakeScreen::create(1.4f, 40);
						m_spGameBG->runAction(shake);
					}						

					ParticleSystem* emitter = ParticleSystemQuad::create("709/goldParticle.plist");			
					emitter->retain();
					emitter->setPosition(ptCatchFishPos);
					addChild(emitter, 20);
					emitter->release();
					emitter->runAction(Sequence::create(DelayTime::create(2.0f), CallFuncN::create(CC_CALLBACK_1(LrbyClientView::removeGold, this, (Node*)emitter)), nullptr));
				}

				int nY = 150;
				if(nChairID == 0)
				{
					nY = -150;
				}
				showBingoGold(Vec2(ptPositionX[nChairID], ptPositionY[nChairID]+nY), catch_fish->fish_score);
			}

			if (catch_fish->bullet_ion)		//
			{
				float fAngle = (cannon[nChairID])->getCannonAngle();
				int nBulletKind = (cannon[nChairID])->getCannonStatue() + 4;
				if( nBulletKind > BULLET_KIND_4_ION)
				{
					nBulletKind = (cannon[nChairID])->getCannonStatue();
				}
				if (nChairID == m_nCurChair)
				{
					m_isEnergy = true;
					m_current_bullet_kind = nBulletKind;
				}
				(cannon[nChairID])->setCannonStatue(nBulletKind);
				(cannon[nChairID])->setCannonAngle(fAngle);
				(m_spCannonION[nChairID])->setVisible(true);
				m_sound->playEffect(EFFECT_TYPE_ION_CATCH);
			}
		}
		break;
	case em_GM_BulletIONTimeOut:
		{
			void* data = NotifRefPtr->GetData();
			WORD data_size = NotifRefPtr->GetSize();
			CMD_S_BulletIonTimeout* bullet_timeout = static_cast<CMD_S_BulletIonTimeout*>(data);

			int nChairID = bullet_timeout->chair_id;

			if (nChairID == m_nCurChair)
			{
				m_isEnergy = false;
				if (m_current_bullet_mulriple < 100)
				{
					m_current_bullet_kind = BULLET_KIND_1_NORMAL;
				} 
				else if (m_current_bullet_mulriple >= 100 && m_current_bullet_mulriple < 1000)
				{
					m_current_bullet_kind = BULLET_KIND_2_NORMAL;
				}
				else if (m_current_bullet_mulriple >= 1000 && m_current_bullet_mulriple < 5000) 
				{
					m_current_bullet_kind = BULLET_KIND_3_NORMAL;
				} 
				else
				{
					m_current_bullet_kind = BULLET_KIND_4_NORMAL;
				}
				(cannon[m_nCurChair])->setCannonStatue(m_current_bullet_kind);
			} 
			else
			{
				int bullet_kind = (cannon[nChairID])->getCannonStatue();
				(cannon[nChairID])->setCannonStatue(bullet_kind % 4);
			}
			(m_spCannonION[nChairID])->setVisible(false);
		}
		break;
	case em_GM_CatchSweepFish:
		{
			void* data = NotifRefPtr->GetData();
			WORD data_size = NotifRefPtr->GetSize();
			CMD_S_CatchSweepFish* catch_sweep_fish = static_cast<CMD_S_CatchSweepFish*>(data);
			
			FishSprite* fish = nullptr;
			for(int i = 0; i < FishManager::GetInstance()->m_fishList.size(); i ++)
			{
				fish = FishManager::GetInstance()->m_fishList.at(i);
				if (fish == NULL)
				{
					continue;
				}
				if (fish->getFishId() != catch_sweep_fish->fish_id)
				{
					continue;
				}

				CMD_C_CatchSweepFish catch_c_sweep_fish;
				memset(&catch_c_sweep_fish, 0, sizeof(catch_c_sweep_fish));
				catch_c_sweep_fish.fish_id = catch_sweep_fish->fish_id;
				catch_c_sweep_fish.chair_id = catch_sweep_fish->chair_id;
				//verysharp //catch_c_sweep_fish.bulletmul=catch_sweep_fish->bulletmul;						

				if (fish->getFishKind() == FISH_KIND_23)	//局部炸弹
				{
					int nPathCount = fish->m_nPathCount;
					FPointAngle& point_angle = fish->m_vecFishPath[nPathCount];
					BoundingBox box;
					box.CreateBoundingBox(g_LrbyGameConfig.m_gameConfig.bomb_range_width, g_LrbyGameConfig.m_gameConfig.bomb_range_height, 
						point_angle.x, point_angle.y, point_angle.angle);

					FishSprite* fish_temp = nullptr;
					for (int j = 0; j < FishManager::GetInstance()->m_fishList.size(); j ++)
					{
						fish_temp = FishManager::GetInstance()->m_fishList.at(j);
						if (fish_temp == fish) continue;
						if (fish_temp->getFishStatue() == FISH_DIE) continue;
						if (fish_temp->m_nPathCount >= fish_temp->m_vecFishPath.size())
						{
							continue;
						}
						FPointAngle& fish_pos = fish_temp->m_vecFishPath[fish_temp->m_nPathCount];
						if (InsideScreen(Vec2(fish_pos.x, fish_pos.y)) && box.ComputeCollision(fish_pos.x, fish_pos.y))
						{
							catch_c_sweep_fish.catch_fish_id[catch_c_sweep_fish.catch_fish_count++] = fish_temp->getFishId();
							if (catch_c_sweep_fish.catch_fish_count >= arraysize(catch_c_sweep_fish.catch_fish_id)) break;
						}
					}
					//verysharp //catch_c_sweep_fish.dwCatchFishTick = GetTickCount();
					EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_CATCH_SWEEP_FISH, &catch_c_sweep_fish, sizeof(catch_c_sweep_fish));
				} 
				else if (fish->getFishKind()  == FISH_KIND_24)	//超级炸弹
				{
					FishSprite* fish_temp = nullptr;
					for (int j = 0; j < FishManager::GetInstance()->m_fishList.size(); j ++)
					{
						fish_temp = FishManager::GetInstance()->m_fishList.at(j);
						if (fish_temp == fish) continue;
						if (fish_temp->getFishStatue() == FISH_DIE) continue;
						if (fish_temp->m_nPathCount >= fish_temp->m_vecFishPath.size())
						{
							continue;
						}
						FPointAngle& fish_pos = fish_temp->m_vecFishPath[fish_temp->m_nPathCount];
						if (InsideScreen(Vec2(fish_pos.x, fish_pos.y)))
						{
							catch_c_sweep_fish.catch_fish_id[catch_c_sweep_fish.catch_fish_count++] = fish_temp->getFishId();
							if(catch_c_sweep_fish.catch_fish_count >= arraysize(catch_c_sweep_fish.catch_fish_id)) break;
						}
					}
					//verysharp //catch_c_sweep_fish.dwCatchFishTick = GetTickCount();
					EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_CATCH_SWEEP_FISH, &catch_c_sweep_fish, sizeof(catch_c_sweep_fish));
				} 
				else if (fish->getFishKind() >= FISH_KIND_31 && fish->getFishKind() <= FISH_KIND_40)	//鱼王1-10
				{
					FishKind fish_kind = FishKind(fish->getFishId() - FISH_KIND_31);
					FishSprite* fish_temp = nullptr;
					for (int j = 0; j < FishManager::GetInstance()->m_fishList.size(); j ++)
					{
						fish_temp = FishManager::GetInstance()->m_fishList.at(j);
						if (fish_temp == fish) continue;
						if (fish_temp->getFishStatue() == FISH_DIE) continue;
						if (fish_temp->getFishKind() != fish_kind) continue;
						if (fish_temp->m_nPathCount >= fish_temp->m_vecFishPath.size())
						{
							continue;
						}
						FPointAngle& fish_pos = fish_temp->m_vecFishPath[fish_temp->m_nPathCount];
						if (InsideScreen(Vec2(fish_pos.x, fish_pos.y)))
						{
							catch_c_sweep_fish.catch_fish_id[catch_c_sweep_fish.catch_fish_count++] = fish_temp->getFishId();
							if(catch_c_sweep_fish.catch_fish_count >= arraysize(catch_c_sweep_fish.catch_fish_id)) break;
						}
					}
					//verysharp //catch_c_sweep_fish.dwCatchFishTick = GetTickCount();
					EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_CATCH_SWEEP_FISH, &catch_c_sweep_fish, sizeof(catch_c_sweep_fish));
				}
				break;
			}
		}
		break;
	case em_GM_CatchSweepFishResult:
		{
			void* data = NotifRefPtr->GetData();
			WORD data_size = NotifRefPtr->GetSize();
			CMD_S_CatchSweepFishResult* catch_sweep_result = static_cast<CMD_S_CatchSweepFishResult*>(data);

			int nChairID = catch_sweep_result->chair_id;
			//int nChairID = LKPY_ENGINE->SwitchViewChairID(catch_sweep_result->chair_id);

			//
			FishSprite* fish = nullptr;
			Vec2 ptCatchFishPos = Vec2(0.0f, 0.0f);

			for(int i = 0; i < FishManager::GetInstance()->m_fishList.size(); i ++)
			{
				fish = FishManager::GetInstance()->m_fishList.at(i);
				if(fish->getFishId() == catch_sweep_result->fish_id)
				{
					ptCatchFishPos = fish->getFishPosition();
					if(fish->getFishKind() == FISH_KIND_23 || fish->getFishKind() == FISH_KIND_24 || (fish->getFishKind() >= FISH_KIND_31 && fish->getFishKind() <= FISH_KIND_40))
					{
						if(UserDefault::getInstance()->getBoolForKey("Cartoon"))
						{
							if (UserDefault::getInstance()->getBoolForKey("ShakeScreen"))
							{
								ShakeScreen* shake = ShakeScreen::create(1.4f, 40);
								m_spGameBG->runAction(shake);
							}		

							ParticleSystem* emitter = ParticleSystemQuad::create("709/goldParticle.plist");
							emitter->retain();
							emitter->setPosition(ptCatchFishPos);
							addChild(emitter, 20);
							emitter->release();
							emitter->runAction(Sequence::create(DelayTime::create(2.0f), CallFuncN::create(CC_CALLBACK_1(LrbyClientView::removeGold, this, (Node*)emitter)), nullptr));
						}

						//fish->removeFish();
						fish->playDieAnimation();
					}
					break;
				}
			}

			for (int i=0; i< catch_sweep_result->catch_fish_count; ++i)
			{
				for(int j = 0; j < FishManager::GetInstance()->m_fishList.size(); j ++)
				{
					fish = FishManager::GetInstance()->m_fishList.at(j);
					if(fish->getFishId() == catch_sweep_result->catch_fish_id[i])
					{
						fish->playDieAnimation();
						break;
					}
				}
			}

			(cannon[nChairID])->setScoreNumber(cannon[nChairID]->getScoreNumber() + catch_sweep_result->user_fish_score);
	
			int nY = 150;
			if(nChairID == 0)
			{
				nY = -150;
			}
			showBingoGold(Vec2(ptPositionX[nChairID], ptPositionY[nChairID] + nY), catch_sweep_result->fish_score);
		}
		break;
	case em_GM_HitFishLK:
		{
			void* data = NotifRefPtr->GetData();
			WORD data_size = NotifRefPtr->GetSize();
			CMD_S_HitFishLK* hit_fish = static_cast<CMD_S_HitFishLK*>(data);

			FishSprite* fish = nullptr; 
			for(int i = 0; i < FishManager::GetInstance()->m_fishList.size(); i ++)
			{
				fish = FishManager::GetInstance()->m_fishList.at(i);
				if(fish->getFishId() == hit_fish->fish_id)
				{
					fish->setLKMultiple(hit_fish->fish_mulriple);
					break;
				}
			}
		}
		break;
	case em_GM_SwitchScene:
		{
			cocos2d::Size winsize = cocos2d::Size(kResolutionWidth,kResolutionHeight);//Director::getInstance()->getWinSize();
			g_LrbyGameConfig.setSwitchingScene(true);

			m_isLockFish = false;
			for (int i = 0; i < BY_GAME_PLAYER; i++)
			{
				m_LockFishManager->ClearLockTrace(i);
				showLockFishIcon(i, 0, 0);

			}

			void* data = NotifRefPtr->GetData();
			WORD data_size = NotifRefPtr->GetSize();
			BYTE *sceneData = new BYTE[data_size];
			memcpy(sceneData, data, data_size);

			switch_scene = (CMD_S_SwitchScene*)sceneData;
			WORD switch_scene_count = data_size / sizeof(CMD_S_SwitchScene);
			int nSceneKing = switch_scene->scene_kind;
			m_nBgIndex ++;
			if (m_nBgIndex>6 || m_nBgIndex<1)
			{
				m_nBgIndex = 1;
			}
			strBg = StringUtils::format("709/Scene/bg%d.jpg", m_nBgIndex);

			m_nBgSoundIndex++;
			if (m_nBgSoundIndex>4 || m_nBgSoundIndex<0)
			{
				m_nBgSoundIndex = 0;
			}
			m_sound->playEffect(EFFECT_TYPE_WAVE);
			m_sound->playBgMusic((MUSIC_TYPE )(MUSIC_TYPE_BACKGROUND1+m_nBgSoundIndex));

			//
			spScene = Sprite::create(strBg.getCString());
			spScene->setScale((float)kResolutionHeight/512);
			this->addChild(spScene, 7);

			spWave = Sprite::create("709/Scene/wave_01.png");	//水波
			spScene->addChild(spWave);
			spWave->setPosition(Vec2(winsize.width* 0.1, winsize.height * 0.5));
			spScene->setPosition(Vec2(winsize.width * 1.5, winsize.height * 0.5));
			spScene->runAction(Sequence::create(MoveTo::create(5.0f, Vec2(winsize.width * 0.5, winsize.height * 0.5)), CallFunc::create([&]{
				//
				FishManager::GetInstance()->clearAllFish();
				if(spWave != nullptr)
				{
					spWave->removeFromParentAndCleanup(true);
					spWave = nullptr;
				}
				if(m_spGameBG != nullptr)
				{
					m_spGameBG->removeFromParentAndCleanup(true);
					m_spGameBG = nullptr;
				}
				reorderChild(spScene, 0);
				m_spGameBG = spScene;
				spScene = nullptr;

				OnSwitchScene(switch_scene);
				g_LrbyGameConfig.setSwitchingScene(false);
			}), NULL));
		}
		break;
	case em_GM_SceneEnd:
		{
			g_LrbyGameConfig.setSwitchingScene(false);
		}
		break;
	case em_GM_FireScore:
		{
			void* data = NotifRefPtr->GetData();
			WORD data_size = NotifRefPtr->GetSize();
			CMD_S_FishScore* fishScore = static_cast<CMD_S_FishScore*>(data);

			for(int i = 0; i < BY_GAME_PLAYER; i ++)
			{
				(cannon[i])->setScoreNumber(fishScore->fish_score[i]);
				if (i == m_nCurChair)
				{//如果是自己， 在重新进入游戏时， 自己的鱼币 只能为0 ， 目前 服务器，在玩家异常退出时， 会发送上次退出前的鱼币值，该值为错误的，应该为0；
					(cannon[i])->setScoreNumber(0);
				}
			}
		}
		break;
	case em_GM_HELPMONEY:
		{
			showMassGold();
		}
		break;
	case em_GM_MESSAGE:
		{
			if (m_isNext)
			{
				m_isNext = false;
				std::string	strMessage = (char*)NotifRefPtr->GetData();
				//log("%s",strMessage);
				labelMessageTxt->setString(strMessage);				
				labelMessageTxt->runAction(Sequence::create(DelayTime::create(3.0f),CallFuncN::create(CC_CALLBACK_0(LrbyClientView::isNext, this)) ,nullptr));
			}
		}
		break;
	case em_GM_OnLineAward:
		{
			void* data = NotifRefPtr->GetData();
			WORD data_size = NotifRefPtr->GetSize();
			CMD_S_OnLineAward* onLineAward = static_cast<CMD_S_OnLineAward*>(data);

			if (onLineAward->nAllStep < onLineAward->nCurStep)
			{
				return;
			}
			if(m_awardLayer == nullptr && onLineAward->chair_id == m_nCurChair)
			{
				m_awardLayer = AwardLayer::create();
				m_awardLayer->setPosition(Vec2((kResolutionWidth - m_awardLayer->getContentSize().width) / 2, (kResolutionHeight - m_awardLayer->getContentSize().height) / 2));
				m_awardLayer->setStatus(onLineAward->nAllStep, onLineAward->nCurStep, onLineAward->nTotalTime);
				this->addChild(m_awardLayer, 30);							
			}
			//if (m_awardLayer != nullptr && (m_nCurChair == 0 || m_nCurChair == 1))
			//{
			//	m_awardLayer->setRotation(180);
			//}
		}
		break;
	default: break;
	}
}

 bool LrbyClientView::OnSwitchScene(CMD_S_SwitchScene* switscene)
 {
	 BYTE *pData = (BYTE *)switscene;
 	if (switscene->scene_kind == SCENE_KIND_1)
	{
 		if (switscene->fish_count != arraysize(scene_kind_1_trace_))
		{
			delete [] pData;
			return false;
		}
		FPoint fInit_pos[5] = {0.0f};
		FishSprite* fish = NULL;
 		for (int i = 0; i < switscene->fish_count; ++i)
		{
			fish = FishManager::GetInstance()->createFish(1, switscene->fish_kind[i], switscene->fish_id[i], TRACE_LINEAR, fInit_pos, 0);			
 			fish->setTraceType(TRACE_LINEAR);
			std::copy(scene_kind_1_trace_[i].begin(), scene_kind_1_trace_[i].end(), std::back_inserter(fish->m_vecFishPath));
			FishManager::GetInstance()->addFish(fish);
			fish->autorelease();
 		}
 	}
	else if (switscene->scene_kind == SCENE_KIND_2)
	{
		if (switscene->fish_count != arraysize(scene_kind_2_trace_))
		{
			delete [] pData;
			return false;
		}
		FPoint fInit_pos[5] = {0.0f};
		FishSprite* fish = NULL;
 		for (int i = 0; i < switscene->fish_count; ++i)
		{
			fish = FishManager::GetInstance()->createFish(1, switscene->fish_kind[i], switscene->fish_id[i], TRACE_LINEAR, fInit_pos, 0);

			fish->setTraceType(TRACE_LINEAR);
			std::copy(scene_kind_2_trace_[i].begin(), scene_kind_2_trace_[i].end(), std::back_inserter(fish->m_vecFishPath));
			FishManager::GetInstance()->addFish(fish);
			fish->autorelease();
 			if (i < 200) 
			{
 				fish->SetFishStop(scene_kind_2_small_fish_stop_index_[i], scene_kind_2_small_fish_stop_count_);
 			}
			else
			{
 				fish->SetFishStop(scene_kind_2_big_fish_stop_index_, scene_kind_2_big_fish_stop_count_);
 			}			
 		}
 	}
	else if (switscene->scene_kind == SCENE_KIND_3)
	{
		if (switscene->fish_count != arraysize(scene_kind_3_trace_))
		{
			delete [] pData;
			return false;
		}
		FPoint fInit_pos[5] = {0.0f};
		FishSprite* fish = NULL;
 		for (int i = 0; i < switscene->fish_count; ++i)
		{
			fish = FishManager::GetInstance()->createFish(1, switscene->fish_kind[i], switscene->fish_id[i], TRACE_LINEAR, fInit_pos, 0);

			fish->setTraceType(TRACE_LINEAR);
			std::copy(scene_kind_3_trace_[i].begin(), scene_kind_3_trace_[i].end(), std::back_inserter(fish->m_vecFishPath));
			FishManager::GetInstance()->addFish(fish);
			fish->autorelease();
 		}
 	}
	else if (switscene->scene_kind == SCENE_KIND_4)
	{
		if (switscene->fish_count != arraysize(scene_kind_4_trace_))
		{
			delete [] pData;
			return false;
		}
		FPoint fInit_pos[5] = {0.0f};
		FishSprite* fish = NULL;
 		for (int i = 0; i < switscene->fish_count; ++i)
		{
			fish = FishManager::GetInstance()->createFish(1, switscene->fish_kind[i], switscene->fish_id[i], TRACE_LINEAR, fInit_pos, 0);

			fish->setTraceType(TRACE_LINEAR);
			std::copy(scene_kind_4_trace_[i].begin(), scene_kind_4_trace_[i].end(), std::back_inserter(fish->m_vecFishPath));
			FishManager::GetInstance()->addFish(fish);
			fish->autorelease();
 		}
	}
	else if (switscene->scene_kind == SCENE_KIND_5)
	{
		if (switscene->fish_count != arraysize(scene_kind_5_trace_))
		{
			delete [] pData;
			return false;
		}
		FPoint fInit_pos[5] = {0.0f};
		FishSprite* fish = NULL;
 		for (int i = 0; i < switscene->fish_count; ++i)
		{
			fish = FishManager::GetInstance()->createFish(1, switscene->fish_kind[i], switscene->fish_id[i], TRACE_LINEAR, fInit_pos, 0);

			fish->setTraceType(TRACE_LINEAR);
			std::copy(scene_kind_5_trace_[i].begin(), scene_kind_5_trace_[i].end(), std::back_inserter(fish->m_vecFishPath));
			FishManager::GetInstance()->addFish(fish);
			fish->autorelease();
 		}
 	}

	delete [] pData;
 	return true;
 }
 
/////////////////// begin of menu ////////////////////////////////////////
void LrbyClientView::autoShootBullet(float dt)
{

	if(!m_bGameConfig)
		return;
	if((m_nCurChair < 0) || (m_nCurChair > BY_GAME_PLAYER - 1))
		return;

	if(!isCanFire())
	{		
		return;
	}
	
	Vec2 ptCannon = (cannon[m_nCurChair])->getPosition();	

	float angle = MathAide::CalcAngle(m_ptLastPos.x, m_ptLastPos.y, ptCannon.x, ptCannon.y);
	(cannon[m_nCurChair])->setCannonAngle(angle + kChairDefaultAngle[m_nCurChair]);	
	//
	if(g_LrbyGameConfig.IsSwitchingScene())
	{
		return;
	}

	//子弹消息
	CMD_C_UserFire userFire;
	memset(&userFire, 0, sizeof(userFire));
	userFire.angle = angle_range(M_PI_2 - angle);

	userFire.bullet_kind = (BulletKind)m_current_bullet_kind;
	userFire.bullet_mulriple = m_current_bullet_mulriple;//锟斤拷前锟斤拷
	userFire.lock_fishid = m_LockFishManager->GetLockFishID(m_nCurChair);
	//
	EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_USER_FIRE, &userFire, sizeof(CMD_C_UserFire));

	return;
}

void LrbyClientView::onAutoShoot(Ref* sender)
{
	//log("LrbyClientView--onAutoShoot click");
	m_nMenuShowTime = 0;
	m_bAutoShoot = !m_bAutoShoot;
	if(m_bAutoShoot)
	{
		menu_AutoShoot->setNormalImage(Sprite::create("709/fish_cannon/btn_auto_shoot_click.png"));
		if (!m_bScheduleAutoShoot)
		{
			m_bScheduleAutoShoot = true;
			schedule(schedule_selector(LrbyClientView::autoShootBullet), 0.3f);
		}		
	}
	else
	{
		menu_AutoShoot->setNormalImage(Sprite::create("709/fish_cannon/btn_auto_shoot.png"));
		unschedule(schedule_selector(LrbyClientView::autoShootBullet));
		m_bScheduleAutoShoot = false;
	}
}

void LrbyClientView::onUnLockFish(Ref* sender)
{
	m_isLockFish = false;
	m_LockFishManager->ClearLockTrace(m_nCurChair);
	showLockFishIcon(m_nCurChair, 0, 0);
}

void LrbyClientView::onLockFish(Ref* sender)
{
	//log("LrbyClientView--onAutoShoot click");
	if (g_LrbyGameConfig.IsSwitchingScene())
	{
		return;
	}
	m_nMenuShowTime = 0;
	m_isLockFish = true;
	if(m_isLockFish)
	{//锁定
		FishKind lock_fish_kind;
		m_LockFishManager->SetLockFishID(m_nCurChair, FishManager::GetInstance()->lockFish(&lock_fish_kind, m_LockFishManager->GetLockFishID(m_nCurChair),m_LockFishManager->GetLockFishKind(m_nCurChair)));
		m_LockFishManager->SetLockFishKind(m_nCurChair, lock_fish_kind);

		int id = m_LockFishManager->GetLockFishID(m_nCurChair);
		int kind = m_LockFishManager->GetLockFishKind(m_nCurChair);
		log("xin::cccccccc================================================  id = %d, kind = %d", id, kind);
	}
}

void LrbyClientView::onAddFishScore(Ref* sender)
{
	m_nMenuShowTime = 0;

	//上分
	CMD_C_ExchangeFishScore exchangeFishScore;
	memset(&exchangeFishScore, 0, sizeof(CMD_C_ExchangeFishScore));
	exchangeFishScore.increase = 1;
	//
	if (m_nCurChair < 0 || m_nCurChair >= BY_GAME_PLAYER)
	{
		return;
	}
	auto userItem = EntityMgr::instance()->roomFrame()->getUserItemByChairID(m_nCurChair);
	if (!userItem)
	{
		return;
	}
	tagUserData* pUserDate = userItem->getUserData();
	if (!pUserDate)
	{
		return;
	}
	if (g_LrbyGameConfig.m_gameConfig.exchange_ratio_fishscore == 0)
	{
		return;
	}

	LONGLONG need_user_score = g_LrbyGameConfig.m_gameConfig.exchange_ratio_userscore * g_LrbyGameConfig.m_gameConfig.exchange_count / g_LrbyGameConfig.m_gameConfig.exchange_ratio_fishscore;		//兑换指定数量的鱼币所需要的金币
	LONGLONG user_leave_score = pUserDate->lScore;
	
	if (need_user_score > user_leave_score)
		need_user_score = user_leave_score;

	//每次减两块
	updateUserMoney(-need_user_score);

	//计算最后能转的鱼币
	need_user_score = need_user_score * g_LrbyGameConfig.m_gameConfig.exchange_ratio_fishscore / g_LrbyGameConfig.m_gameConfig.exchange_ratio_userscore;

	cannon[m_nCurChair]->setScoreNumber(cannon[m_nCurChair]->getScoreNumber() + need_user_score);

	m_sound->playEffect(EFFECT_TYPE_SILVER);

	//发送完这个消息后，后台数据已经更新了
	EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_EXCHANGE_FISHSCORE, &exchangeFishScore, sizeof(CMD_C_ExchangeFishScore));	
}

void LrbyClientView::onMinusFishScore(Ref* sender)
{
	m_nMenuShowTime = 0;

	CMD_C_ExchangeFishScore exchangeFishScore;
	memset(&exchangeFishScore, 0, sizeof(CMD_C_ExchangeFishScore));
	exchangeFishScore.increase = 0;
	if (m_nCurChair < 0 || m_nCurChair >= BY_GAME_PLAYER )
	{
		return;
	}
	auto userItem = EntityMgr::instance()->roomFrame()->getUserItemByChairID(m_nCurChair);
	if (!userItem)
	{
		return;
	}
	tagUserData* pUserDate = userItem->getUserData();
	if (!pUserDate)
	{
		return;
	}

	//	
	if (g_LrbyGameConfig.m_gameConfig.exchange_ratio_fishscore == 0)
	{
		return;
	}

	LONGLONG gain_money = cannon[m_nCurChair]->getScoreNumber() * g_LrbyGameConfig.m_gameConfig.exchange_ratio_userscore / g_LrbyGameConfig.m_gameConfig.exchange_ratio_fishscore;		//兑换指定数量的鱼币所需要的金币

	updateUserMoney(gain_money);

	//重置为零
	cannon[m_nCurChair]->setScoreNumber(0L);
	m_sound->playEffect(EFFECT_TYPE_SILVER);

	//减炮
	EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_EXCHANGE_FISHSCORE, &exchangeFishScore, sizeof(CMD_C_ExchangeFishScore));	
}

void LrbyClientView::onAddBulletMultiple(Ref* sender)
{
	if (m_nCurChair < 0 || m_nCurChair >= BY_GAME_PLAYER )
	{
		return;
	}
	m_nMenuShowTime = 0;

	if(m_current_bullet_mulriple < 10)
	{
		m_current_bullet_mulriple++;
	}
	else if((m_current_bullet_mulriple >= 10) && (m_current_bullet_mulriple < 100))
	{
		m_current_bullet_mulriple += 10;
	}
	else if((m_current_bullet_mulriple >= 100) && (m_current_bullet_mulriple < 1000))
	{
		m_current_bullet_mulriple += 100;	
	}
	else if((m_current_bullet_mulriple >= 1000) && (m_current_bullet_mulriple < 10000))
	{
		m_current_bullet_mulriple += 1000;
	}
	else if((m_current_bullet_mulriple >= 10000) && (m_current_bullet_mulriple < 100000))
	{
		m_current_bullet_mulriple += 10000;
	}

	//if(m_current_bullet_mulriple >= g_LrbyGameConfig.m_gameConfig.max_bullet_multiple)
	//{
	//	m_current_bullet_mulriple = g_LrbyGameConfig.m_gameConfig.max_bullet_multiple;
	//}
	if(m_current_bullet_mulriple > g_LrbyGameConfig.m_gameConfig.max_bullet_multiple)
	{
		m_current_bullet_mulriple = 1;
	}

	if(m_current_bullet_mulriple < 100)
	{
		m_current_bullet_kind = BULLET_KIND_1_NORMAL;
	}
	else if (m_current_bullet_mulriple >= 100 && m_current_bullet_mulriple < 1000)
	{
		m_current_bullet_kind = BULLET_KIND_2_NORMAL;
	}
	else if (m_current_bullet_mulriple >= 1000 && m_current_bullet_mulriple < 5000) 
	{
		m_current_bullet_kind = BULLET_KIND_3_NORMAL;
	} 
	else
	{
		m_current_bullet_kind = BULLET_KIND_4_NORMAL;
	}

	if (m_isEnergy)
	{
		m_current_bullet_kind += 4;
	}

	(cannon[m_nCurChair])->setCannonStatue(m_current_bullet_kind);
	(cannon[m_nCurChair])->setCannonNumber(m_current_bullet_mulriple);
	m_sound->playEffect(EFFECT_TYPE_CANNON_SWITCH);

	//EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_SWITCH_BULLENT, NULL,NULL);		//不需要向服务器发送切换子弹
}

void LrbyClientView::onSubBulletMultiple(Ref* sender)
{
	if (m_nCurChair < 0 || m_nCurChair >= BY_GAME_PLAYER )
	{
		return;
	}
	m_nMenuShowTime = 0;
	if((m_current_bullet_mulriple == g_LrbyGameConfig.m_gameConfig.min_bullet_multiple))
	{
		m_current_bullet_mulriple = g_LrbyGameConfig.m_gameConfig.max_bullet_multiple;
	}
	else if((m_current_bullet_mulriple <= 10))
	{
		m_current_bullet_mulriple--;
	}
	else if((m_current_bullet_mulriple > 10) && (m_current_bullet_mulriple <= 100))
	{
		if (m_current_bullet_mulriple == g_LrbyGameConfig.m_gameConfig.max_bullet_multiple && g_LrbyGameConfig.m_gameConfig.max_bullet_multiple % 10 != 0)
		{
			m_current_bullet_mulriple = m_current_bullet_mulriple / 10 * 10;
		}
		else
		{
			m_current_bullet_mulriple -= 10;
		}	
	}
	else if((m_current_bullet_mulriple > 100) && (m_current_bullet_mulriple <= 1000))
	{
		if (m_current_bullet_mulriple == g_LrbyGameConfig.m_gameConfig.max_bullet_multiple  && g_LrbyGameConfig.m_gameConfig.max_bullet_multiple % 100 != 0)
		{
			m_current_bullet_mulriple = m_current_bullet_mulriple / 100 * 100;
		}
		else
		{
			m_current_bullet_mulriple -= 100;
		}		
	}
	else if((m_current_bullet_mulriple > 1000) && (m_current_bullet_mulriple <= 10000))
	{
		if (m_current_bullet_mulriple == g_LrbyGameConfig.m_gameConfig.max_bullet_multiple  && g_LrbyGameConfig.m_gameConfig.max_bullet_multiple % 1000 != 0)
		{
			m_current_bullet_mulriple = m_current_bullet_mulriple / 1000 * 1000;
		}
		else
		{
			m_current_bullet_mulriple -= 1000;
		}		
	}
	else if((m_current_bullet_mulriple > 10000) && (m_current_bullet_mulriple <= 100000))
	{
		if (m_current_bullet_mulriple == g_LrbyGameConfig.m_gameConfig.max_bullet_multiple  && g_LrbyGameConfig.m_gameConfig.max_bullet_multiple % 10000 != 0)
		{
			m_current_bullet_mulriple = m_current_bullet_mulriple / 10000 * 10000;
		}
		else
		{
			m_current_bullet_mulriple -= 10000;
		}		
	}

	if(m_current_bullet_mulriple < g_LrbyGameConfig.m_gameConfig.min_bullet_multiple)
	{
		m_current_bullet_mulriple = g_LrbyGameConfig.m_gameConfig.min_bullet_multiple;
	}

	if(m_current_bullet_mulriple < 100)
	{
		m_current_bullet_kind = BULLET_KIND_1_NORMAL;
	}
	else if (m_current_bullet_mulriple >= 100 && m_current_bullet_mulriple < 1000)
	{
		m_current_bullet_kind = BULLET_KIND_2_NORMAL;
	}
	else if (m_current_bullet_mulriple >= 1000 && m_current_bullet_mulriple < 5000) 
	{
		m_current_bullet_kind = BULLET_KIND_3_NORMAL;
	} 
	else
	{
		m_current_bullet_kind = BULLET_KIND_4_NORMAL;
	}

	if (m_isEnergy)
	{
		m_current_bullet_kind += 4;
	}

	(cannon[m_nCurChair])->setCannonStatue(m_current_bullet_kind);
	(cannon[m_nCurChair])->setCannonNumber(m_current_bullet_mulriple);
	m_sound->playEffect(EFFECT_TYPE_CANNON_SWITCH);
}

void LrbyClientView::onQuitGame(Ref* sender)
{
	m_nMenuShowTime = 0;
	log("LrbyClientView--onQuitGame click");
	m_bAutoShoot = false;
	m_bScheduleAutoShoot = false;
	unschedule(schedule_selector(LrbyClientView::autoShootBullet));		//
	menu_AutoShoot->setNormalImage(Sprite::create("709/fish_cannon/btn_auto_shoot.png"));

	EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_MOBILE_GET_Harvest, NULL, NULL);
}

/////////////////////// end of menu //////////////////////////////////

void LrbyClientView::setBulletMulriple(int nMulriple)
{
	if(m_nCurChair < 0) 
	{
		//提示

		return;
	}

	m_current_bullet_mulriple = nMulriple;

	if (m_current_bullet_mulriple < 100)
	{
		m_current_bullet_kind = BULLET_KIND_1_NORMAL;
	} 
	else if (m_current_bullet_mulriple >= 100 && m_current_bullet_mulriple < 1000)
	{
		m_current_bullet_kind = BULLET_KIND_2_NORMAL;
	}
	else if (m_current_bullet_mulriple >= 1000 && m_current_bullet_mulriple < 5000) 
	{
		m_current_bullet_kind = BULLET_KIND_3_NORMAL;
	} 
	else
	{
		m_current_bullet_kind = BULLET_KIND_4_NORMAL;
	}
	(cannon[m_nCurChair])->setCannonStatue(m_current_bullet_kind);		//
	(cannon[m_nCurChair])->setCannonNumber(m_current_bullet_mulriple);
}

void LrbyClientView::setOtherBulletMulriple(int nPlayer, int nMulriple)
{
	int nBulletKind = BULLET_KIND_2_NORMAL;

	if (nMulriple < 100)
	{
		nBulletKind = BULLET_KIND_1_NORMAL;
	} 
	else if (nMulriple >= 100 && nMulriple < 1000)
	{
		nBulletKind = BULLET_KIND_2_NORMAL;
	}
	else if (nMulriple >= 1000 && nMulriple < 5000) 
	{
		nBulletKind = BULLET_KIND_3_NORMAL;
	} 
	else
	{
		nBulletKind = BULLET_KIND_4_NORMAL;
	}
	(cannon[nPlayer])->setCannonStatue(nBulletKind);		//
	(cannon[nPlayer])->setCannonNumber(nMulriple);
}

void LrbyClientView::showBingoGold(Vec2 ptBingoPos, int nGold)
{
	Node* bingoNode = Node::create();
	bingoNode->setPosition(Vec2(10.0f, 10.f));	//ptBingoPos);
	this->addChild(bingoNode, 22);

	m_sound->playEffect(EFFECT_TYPE_BINGO);

	auto spBingo = Sprite::createWithSpriteFrameName("bingo/bingo_01.png");
	spBingo->setPosition(ptBingoPos);
	bingoNode->addChild(spBingo, 21);

	LabelAtlas* labelGold = LabelAtlas::create(String::createWithFormat("%d", nGold)->getCString(), "709/score_num.png", 33, 38, '0');
	labelGold->setPosition(Vec2(ptBingoPos.x, ptBingoPos.y));
	labelGold->setAnchorPoint(Vec2(0.5, 0.5));
	bingoNode->addChild(labelGold, 22);
	
	labelGold->setRotation(kChairDefaultAngleOther[m_nCurChair]);
	
	
	Vector<SpriteFrame*> normalFrames(10);
	for(int i = 1; i <= 10; i ++)
	{
		char str[64] = {0};
		sprintf(str, "bingo/bingo_%02d.png", i);
		SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
		normalFrames.pushBack(frame);
	}
	Animation* ani = Animation::createWithSpriteFrames(normalFrames, 0.1f);
	spBingo->runAction(RepeatForever::create(Animate::create(ani)));
	labelGold->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.25, 30.0), RotateTo::create(0.25, -30.0), nullptr)));
	spBingo->runAction(Sequence::create(DelayTime::create(3.0f), CallFuncN::create(CC_CALLBACK_1(LrbyClientView::removeBingo, this, bingoNode)),nullptr));

}

void LrbyClientView::showCatchFishGold(Vec2 ptGoldBeginPos, Vec2 ptGoldEndPos, int nGold)
{
	int nGoldCount = 2;
	if(nGold <= 0)
	{
		return;
	}
	if((nGold > 0) && (nGold < 500))
	{
		nGoldCount = 2;
	}
	else if((nGold >= 500) && (nGold < 5000))
	{
		nGoldCount = 3;
	}
	else if((nGold >= 5000) && (nGold < 50000))
	{
		nGoldCount = 4;
	}
	else if((nGold >= 50000) && (nGold < 500000))
	{
		nGoldCount = 5;
	}
	else if((nGold >= 500000) && (nGold < 5000000))
	{
		nGoldCount = 6;
	}
	else
	{
		nGoldCount = 8;
	}

	m_sound->playEffect(EFFECT_TYPE_GOLD);

	//金币
	LabelAtlas* labelGold = LabelAtlas::create(String::createWithFormat("%d", nGold)->getCString(), "709/score_num.png", 33, 38, '0');
	labelGold->setPosition(Vec2(ptGoldBeginPos.x, ptGoldBeginPos.y));
	labelGold->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(labelGold, 22);
	
	labelGold->setRotation(kChairDefaultAngleOther[m_nCurChair]);
	
	labelGold->runAction(Sequence::create(DelayTime::create(0.7f), CallFuncN::create(CC_CALLBACK_1(LrbyClientView::removeLabelGold, this, labelGold)),nullptr));
	float space = MathAide::CalcDistance(ptGoldBeginPos.x, ptGoldBeginPos.y, ptGoldEndPos.x, ptGoldEndPos.y);
	float fTime = 0.45f;
	if (space<300.0f)
	{
		fTime = 0.35;
	}
	else if (space<500.0f)
	{
		fTime = 0.38;
	}
	else if (space<800.0f)
	{
		fTime = 0.42;
	}
	 
	for(int i = 0; i < nGoldCount; i ++)
	{
		Node* goldNode = Node::create();
		goldNode->setPosition(Vec2(0.0f, 0.0f));
		this->addChild(goldNode, 22);

		auto spGold = Sprite::createWithSpriteFrameName("coin2/coin2_01.png");
		spGold->setPosition(Vec2(ptGoldBeginPos.x + 50 * i, ptGoldBeginPos.y));
		goldNode->addChild(spGold, 21);

		Vector<SpriteFrame*> normalFrames(12);
		for(int i = 1; i <= 12; i ++)
		{
			char str[64] = {0};
			sprintf(str, "coin2/coin2_%02d.png", i);
			SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
			normalFrames.pushBack(frame);
		}
		Animation* ani = Animation::createWithSpriteFrames(normalFrames, 0.1f);
		ActionInterval* ActGold = RepeatForever::create(Animate::create(ani));
		spGold->runAction(ActGold);
		FiniteTimeAction *ft = JumpTo::create(0.8f, Vec2(spGold->getPosition().x, spGold->getPosition().y), 120, 1);
		
		spGold->runAction(Sequence::create(ft, /*ft1, */MoveTo::create(fTime, ptGoldEndPos), CallFuncN::create(CC_CALLBACK_1(LrbyClientView::removeGold, this, goldNode)),nullptr));
	}
}

void LrbyClientView::removeBingo(Node* sender, Node* bingo)
{
	if((sender != nullptr) && (bingo != nullptr))
	{
		bingo->removeFromParentAndCleanup(true);
		bingo = nullptr;
	}
}

void LrbyClientView::removeGold(Node* sender, Node* gold)
{
	if((sender != nullptr) && (gold != nullptr))
	{
		gold->removeFromParentAndCleanup(true);
		gold = nullptr;
	}
}

void LrbyClientView::removeLabelGold(Node* sender, LabelAtlas* label)
{
	if((sender != nullptr) && (label != nullptr))
	{
		label->removeFromParentAndCleanup(true);
		label = nullptr;
	}
}

bool LrbyClientView::InsideScreen(Vec2 ptPos)
{
	Rect rt = CCRectMake(0.0f, 0.0f,NS_lrbyFish::kResolutionWidth,NS_lrbyFish::kResolutionHeight);
	if(rt.containsPoint(ptPos))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void LrbyClientView::setScreenLock(bool bLock)
{
	m_bLockScreen = bLock;
}

void LrbyClientView::onAccountGold()
{
	//if (//LKPY_ENGINE->IsLookonMode())
	{
		return ;
	}

	//
	CMD_C_ExchangeFishScore exchangeFishScore;
	memset(&exchangeFishScore, 0, sizeof(CMD_C_ExchangeFishScore));
	exchangeFishScore.increase = 0;
	//verysharp //exchangeFishScore.iExchangeCount = 0;
	//
	EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_EXCHANGE_FISHSCORE, &exchangeFishScore, sizeof(CMD_C_ExchangeFishScore));
}

void LrbyClientView::onAccountGoldAndQuit()
{
/*	//if (//LKPY_ENGINE->IsLookonMode())
	{
		//LKPY_ENGINE->m_userInfo[m_nCurChair] = nullptr;
		//
		//LKPY_ENGINE->CloseGameView();
		return ;
	}
*/
	//
	CMD_C_ExchangeFishScore exchangeFishScore;
	memset(&exchangeFishScore, 0, sizeof(CMD_C_ExchangeFishScore));
	exchangeFishScore.increase = 0;
	//verysharp //exchangeFishScore.iExchangeCount = 0;
	//
	EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_EXCHANGE_FISHSCORE, &exchangeFishScore, sizeof(CMD_C_ExchangeFishScore));
	this->scheduleOnce(SEL_SCHEDULE(&LrbyClientView::onAwardQuit), 1.0f);
}

bool LrbyClientView::isCanLockFish(int chairId)
{
	if (chairId == m_nCurChair)
	{
		if (m_LockFishManager->GetLockFishID(chairId) <= 0)
		{
			m_isLockFish = false;
		}
	}
	else
	{

	}
	return true;
}

bool LrbyClientView::isCanFire()
{
	if((!m_bGameConfig) || (m_nCurChair < 0))
	{
		return false;
	}
	//最大子弹个数 暂时定为10发
	if (m_fireBulletNum[m_nCurChair] > 10)//g_LrbyGameConfig.m_gameConfig)
	{
		return false;
	}
	//不够钱
	if(cannon[m_nCurChair]->getScoreNumber() < m_current_bullet_mulriple)	//cannon[m_nCurChair]->getCannonNumber())
	{
		m_bAutoShoot = false;
		unschedule(schedule_selector(LrbyClientView::autoShootBullet));
		m_bScheduleAutoShoot = false;
		menu_AutoShoot->setNormalImage(Sprite::create("709/fish_cannon/btn_auto_shoot.png"));
		return false;
	}
	return true;
}

void LrbyClientView::menuCloseRechargeCallback(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
		
}

void LrbyClientView::menuCloseSetLayerCallback(cocos2d::Ref* sender,cocos2d::ui::TouchEventType type)
{
	
}

void LrbyClientView::onLayerExit()
{
	
	onQuitGame(nullptr);	//
}

void LrbyClientView::addNet(Vec2 pt, int nNetStatue)
{
	NetSprite *net = m_netPool->getNet(nNetStatue);
	if (net)
	{
		net->run(pt, nNetStatue);
		this->addChild(net, 8);
	}
}

void LrbyClientView::playCatchEffect(int nRand,int nFishKind)
{
	//
	if(nFishKind == FISH_KIND_9)
	{
		if(nRand == 0)
		{
			m_sound->playEffect(EFFECT_TYPE_FISH10_1);
		}
		else
		{
			m_sound->playEffect(EFFECT_TYPE_FISH10_2);
		}
	}
	else if(nFishKind == FISH_KIND_10)
	{
		if(nRand == 0)
		{
			m_sound->playEffect(EFFECT_TYPE_FISH11_1);
		}
		else
		{
			m_sound->playEffect(EFFECT_TYPE_FISH11_2);
		}
	}
	else if(nFishKind == FISH_KIND_11)
	{
		if(nRand == 0)
		{
			m_sound->playEffect(EFFECT_TYPE_FISH12_1);
		}
		else
		{
			m_sound->playEffect(EFFECT_TYPE_FISH12_2);
		}
	}
	else if(nFishKind == FISH_KIND_12)
	{
		if(nRand == 0)
		{
			m_sound->playEffect(EFFECT_TYPE_FISH13_1);
		}
		else
		{
			m_sound->playEffect(EFFECT_TYPE_FISH13_2);
		}
	}
	else if(nFishKind == FISH_KIND_13)
	{
		if(nRand == 0)
		{
			m_sound->playEffect(EFFECT_TYPE_FISH14_1);
		}
		else
		{
			m_sound->playEffect(EFFECT_TYPE_FISH14_2);
		}
	}
	else if(nFishKind == FISH_KIND_14)
	{
		if(nRand == 0)
		{
			m_sound->playEffect(EFFECT_TYPE_FISH15_1);
		}
		else
		{
			m_sound->playEffect(EFFECT_TYPE_FISH15_2);
		}
	}
	else if(nFishKind == FISH_KIND_15)
	{
		if(nRand == 0)
		{
			m_sound->playEffect(EFFECT_TYPE_FISH16_1);
		}
		else
		{
			m_sound->playEffect(EFFECT_TYPE_FISH16_2);
		}
	}
	else if(nFishKind == FISH_KIND_16)
	{
		if(nRand == 0)
		{
			m_sound->playEffect(EFFECT_TYPE_FISH17_1);
		}
		else
		{
			m_sound->playEffect(EFFECT_TYPE_FISH17_2);
		}
	}
	else
	{
		m_sound->playEffect(EFFECT_TYPE_CATCH);
	}
}

void LrbyClientView::playCatchGold(const Vec2 pos, const int gold, const int chairId)
{
	Vec2 ptGoldEndPos = Vec2(ptPositionX[chairId], ptPositionY[chairId]);

	showCatchFishGold(pos,ptGoldEndPos,gold);

	return;

	//显示鱼的得分
	auto m_labelLKNum = LabelAtlas::create(String::createWithFormat("%d", gold)->getCString(), "709/score_num.png", 33, 38, '0');
	m_labelLKNum->setPosition(pos);
	m_labelLKNum->setScale(0.5f);
	this->addChild(m_labelLKNum, 201);

	m_labelLKNum->runAction(Sequence::create(Spawn::createWithTwoActions(JumpBy::create(0.3,Vec2::ZERO,5,1),ScaleTo::create(0.1f,1.0f)),DelayTime::create(0.6f),FadeOut::create(0.2f),CallFunc::create([=](){
		m_labelLKNum->removeFromParent();
	}),nullptr));
	
	//金币动画
	Sound *m_sound = Sound::getInstance();
	m_sound->playEffect(EFFECT_TYPE_coinsky);	
	
	float fTime = 1.0f;
		
	ArmatureDataManager::getInstance()->addArmatureFileInfo("709/NewAnimation/NewAnimation.ExportJson");
	cocos2d::Size thieSize = Director::getInstance()->getWinSize();
	Armature* logoAni = Armature::create("NewAnimation");				
	logoAni->setPosition(pos);
	logoAni->getAnimation()->play("Animation2");
	this->addChild(logoAni,200);
	logoAni->setVisible(false);
	logoAni->setScale(2.0f);
	logoAni->runAction(Sequence::create(DelayTime::create(0.025f), CallFuncN::create([=](bool){
		logoAni->setVisible(true);
	}),
		Spawn::create(MoveTo::create(fTime, ptGoldEndPos), ScaleTo::create(fTime,0.5f), nullptr), CallFuncN::create(CC_CALLBACK_1(LrbyClientView::removeMassGold, this, logoAni)),nullptr));
}

void LrbyClientView::showTicketEffect(Vec2 ptStart, int nNum)
{
		
}

void LrbyClientView::removeTicket(Node* sender, Node* ticket)
{
	if((sender != nullptr) && (ticket != nullptr))
	{
		ticket->removeFromParentAndCleanup(true);
		ticket = nullptr;
	}
}

void LrbyClientView::onAccountCancle()
{
	if (m_accountLayer != nullptr)
	{
		m_accountLayer = nullptr;
	}
}

void LrbyClientView::showMassGold()
{
	Sound *m_sound = Sound::getInstance();
	m_sound->playEffect(EFFECT_TYPE_coinsky);
	Vec2 ptGoldEndPos = Vec2(200, 20);
	if (m_nCurChair == 0)
	{
		ptGoldEndPos = Vec2(kResolutionWidth - 200, kResolutionHeight- 20);
	}
	float fTime = 0.6f;

	for(int i = 0; i < 50; i ++)	
	{	
		ArmatureDataManager::getInstance()->addArmatureFileInfo("709/NewAnimation/NewAnimation.ExportJson");
		cocos2d::Size thieSize = Director::getInstance()->getWinSize();
		Armature* logoAni = Armature::create("NewAnimation");				
		logoAni->setPosition(Vec2(kResolutionWidth / 2, kResolutionHeight / 2));
		logoAni->getAnimation()->play("Animation2");
		this->addChild(logoAni,2);
		logoAni->setVisible(false);
		logoAni->setScale(0.1f);
		ptGoldEndPos = Vec2(501.30f * cosf(i  * M_PI/4.3333f+(i/25)*0.13f),501.3f * sinf(i * M_PI /4.3333f+(i/25)*0.13f)); 
		logoAni->runAction(Sequence::create(DelayTime::create(i * 0.025f), CallFuncN::create([=](bool){
			logoAni->setVisible(true);
		}),
			Spawn::create(MoveBy::create(fTime, ptGoldEndPos), ScaleTo::create(fTime,2.5f), nullptr), CallFuncN::create(CC_CALLBACK_1(LrbyClientView::removeMassGold, this, logoAni)),nullptr));
	}
}

void LrbyClientView::removeMassGold(Node* sender, Node* gold)
{
	if((sender != nullptr) && (gold != nullptr))
	{
		gold->removeFromParentAndCleanup(true);
		gold = nullptr;
	}
}

void LrbyClientView::isNext()
{
	m_isNext = true;
	labelMessageTxt->setString("");
}

void LrbyClientView::setSuccess(bool isSuccess)
{
	if (LrbyClientView::GetInstance())
	{
		m_isSuccess = isSuccess;
	}
	
}

void LrbyClientView::onOnLineAward(int nType)
{
	if (nType == 0)
	{
		EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_ONLINE_AWARD_GET, NULL, NULL);
	}
	else
	{
		EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_ONLINE_AWARD_COM, NULL, NULL);
	}	
}

void LrbyClientView::onAwardQuit()
{
	if (m_awardLayer != nullptr)
	{
		m_awardLayer = nullptr;
	}
}

void LrbyClientView::onAwardQuit(float dt)
{	
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

}


WORD LrbyClientView::switchChairID(WORD wCharID)
{
	return wCharID;
}

unsigned long long LrbyClientView::GetClientTickCount()
{
#ifdef WIN32
	struct __timeb64 tv;
	_ftime64(&tv);
	return tv.time * 1000 + tv.millitm;
#else
	struct timeval tv; 
	gettimeofday(&tv,0);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif   
}

void LrbyClientView::OnMessage(Ref* sender)
{
	Integer* _int=(Integer*)sender;
	switch(_int->getValue())
	{
	case FishmsgToView_UserInfor:	
		userInforDeal(_int);
		break;
	case FishmsgToView_SitDown:
		{
			// 如果捕鱼服务器没有处理时间戳请求,就不要发送时间戳的请求<by hxh 20161227>
#ifdef USE_TRACETIME
			m_TimeStamp.tClientSendTime=LrbyClientView::GetClientTickCount();
			EntityMgr::instance()->getDispatch()->sendSocketDate(SUB_C_TIME_STAMP, &m_TimeStamp, sizeof(CMD_C_TimeStamp));
#endif					
		}
		break;
	}
}

void LrbyClientView::userInforDeal(Integer* _data)
{
	FishCCuserInfor* _inforData=(FishCCuserInfor*)_data;
	
	if (_inforData->m_userInforID == EntityMgr::instance()->roomFrame()->getMeUserDate()->dwUserID)//
	{
		m_nCurChair = _inforData->m_ChairID;
	}
	else
	{
		
	}
	//m_nCurChair = EntityMgr::instance()->roomFrame()->getMeUserDate()->wChairID;
	m_nCurChair = EntityMgr::instance()->getDispatch()->m_wChairID;
	if (m_nCurChair == INVALID_CHAIR)
	{
		CCLOG("I am not in the table......");
		return;
	}
	updateUserInfo(NULL);
}

void LrbyClientView::initUserInfo(std::string nickName,SCORE lScore)
{	
	//底部状态栏
	if(m_layerBottom == NULL)
	{
		//m_layerBottom = LayerColor::create(ccc4(0,0,0,200));
		m_layerBottom = LayerColor::create();
		m_layerBottom->setContentSize(cocos2d::Size(SCREEN_HEIGHT, 66));
		m_layerBottom->setAnchorPoint(Vec2::ZERO);
		m_layerBottom->setPosition(0,0);
		this->addChild(m_layerBottom, 24);

		auto bg = Sprite::create("709/fish_cannon/bottom_bg.png");
		bg->setPosition(bg->getContentSize().width / 2, bg->getContentSize().height / 2);
		m_layerBottom->addChild(bg);
	}
	
	//个人信息
	//昵称
	if (m_userName == NULL)
	{
		//m_userName = LabelTTF::create("", "SimHei", 30);
		m_userName=LabelTTF::create("", "", 26);
		m_userName->setColor(ccc3(228, 255, 255));
		m_userName->setAnchorPoint(Vec2(0, 0.5));
		m_userName->setPosition(ccp(65,  30));
		m_userName->setString("xx");
		m_layerBottom->addChild(m_userName);
	}

	if (m_userGold == NULL)
	{//玩家金币
		//m_userGold = LabelAtlas::create("100", "709/fish_cannon/num_user_gold.png", 12, 17, '0');
		m_userGold = LabelTTF::create("", "", 26);
		m_userGold->setAnchorPoint(Vec2(0, 0.5));
		m_userGold->setPosition(Vec2(280, 24));
		m_userGold->setString("0");
		m_layerBottom->addChild(m_userGold);
	}
	
	m_userName->setString(nickName);
	string str=String::createWithFormat("%.2lf",lScore)->getCString();
	m_userGold->setString(str);
	//m_userGold->setString(StringUtils::toString(lScore));
}

void LrbyClientView::updateUserMoney(const int money)
{
	auto userItem = EntityMgr::instance()->roomFrame()->getUserItemByChairID(m_nCurChair);
	if (!userItem)
	{
		return;
	}
	tagUserData* pUserDate = userItem->getUserData();

	pUserDate->lScore += money;

	if(m_userGold)
	{
		//更新金币
		m_userGold->setString(String::createWithFormat("%.2lf",pUserDate->lScore)->getCString());
	}
}

void LrbyClientView::onLoginFaild(Object* obj)
{
	CCLOG("DDZSceneView::onLoginFaild");
	cocos2d::Size sVSize = Director::getInstance()->getVisibleSize();
	tagRoomLogonFailObj* pObj = (tagRoomLogonFailObj*)obj;
	int DataSize=128;
	char str[1000]={0};
	MyNSString::wc2mbs(pObj->szDescribeString,DataSize, str);
	PromptBox* promptBox = PromptBox::PromptBoxWith(Vec2(sVSize.width * 0.5, sVSize.height * 0.5), mPromptTypeGetError);
	promptBox->setPromptText(str);
	this->addChild(promptBox,1);
}

void LrbyClientView::initMenuBtn()
{
	//底部状态栏
	if(m_layerBottom == NULL)
	{
		//m_layerBottom = LayerColor::create(ccc4(0,0,0,200));
		m_layerBottom = LayerColor::create();
		m_layerBottom->setContentSize(cocos2d::Size(SCREEN_HEIGHT, 66));
		m_layerBottom->setAnchorPoint(Vec2::ZERO);
		m_layerBottom->setPosition(0,0);
		this->addChild(m_layerBottom, 24);

		auto bg = Sprite::create("709/fish_cannon/bottom_bg.png");
		bg->setPosition(bg->getContentSize().width / 2, bg->getContentSize().height / 2);
		m_layerBottom->addChild(bg);
	}

	//个人信息
	//昵称
	if (m_userName == NULL)
	{
		//m_userName = LabelTTF::create("", "SimHei", 30);
		m_userName=LabelTTF::create("", "", 26);
		m_userName->setColor(ccc3(228, 255, 255));
		m_userName->setAnchorPoint(Vec2(0, 0.5));
		m_userName->setPosition(ccp(65,  30));
		m_userName->setString("xx");
		m_layerBottom->addChild(m_userName);
	}

	if (m_userGold == NULL)
	{//玩家金币
		//m_userGold = LabelAtlas::create("100", "709/fish_cannon/num_user_gold.png", 12, 17, '0');
		m_userGold = LabelTTF::create("", "", 26);
		m_userGold->setAnchorPoint(Vec2(0, 0.5));
		m_userGold->setPosition(Vec2(280, 26));
		m_userGold->setString("0");
		m_layerBottom->addChild(m_userGold);
	}

	//减炮
	menu_SubGun = MenuItemImage::create("709/fish_cannon/gun_minus_nomal.png", "709/fish_cannon/gun_minus_press.png", CC_CALLBACK_1(LrbyClientView::onSubBulletMultiple, this));
	menu_SubGun->setPosition(Vec2(510, m_layerBottom->getContentSize().height / 2));

	//加炮
	menu_AddGun = MenuItemImage::create("709/fish_cannon/gun_add_nomal.png", "709/fish_cannon/gun_add_press.png", CC_CALLBACK_1(LrbyClientView::onAddBulletMultiple, this));
	menu_AddGun->setPosition(Vec2(660, m_layerBottom->getContentSize().height / 2));//kResolutionHeight * 0.05

	//上分
	menu_AddBringGold = MenuItemImage::create("709/fish_cannon/score_add_nomal.png","709/fish_cannon/score_add_press.png", CC_CALLBACK_1(LrbyClientView::onAddFishScore, this));
	menu_AddBringGold->setPosition(Vec2(810, m_layerBottom->getContentSize().height / 2));//kResolutionHeight * 0.05

	//下分
	menu_SubBringGold = MenuItemImage::create("709/fish_cannon/score_minus_nomal.png", "709/fish_cannon/score_minus_press.png", CC_CALLBACK_1(LrbyClientView::onMinusFishScore, this));
	menu_SubBringGold->setPosition(Vec2(960, m_layerBottom->getContentSize().height / 2));

	//设置
	menu_setting = MenuItemImage::create("709/fish_cannon/fish_set_nomal.png", "709/fish_cannon/fish_set_press.png", CC_CALLBACK_1(LrbyClientView::onPressSetting, this));
	menu_setting->setPosition(Vec2(1080, m_layerBottom->getContentSize().height / 2 - 5));

	//帮助
	menu_help = MenuItemImage::create("709/fish_cannon/fish_help_nomal.png", "709/fish_cannon/fish_help_press.png", CC_CALLBACK_1(LrbyClientView::onPressHelp, this));
	menu_help->setPosition(Vec2(1150, m_layerBottom->getContentSize().height / 2 - 5));

	// back btn
	auto size = Director::getInstance()->getWinSize();
	menu_exit = MenuItemImage::create("709/fish_cannon/fish_exit_nomal.png", "709/fish_cannon/fish_exit_press.png", CC_CALLBACK_1(LrbyClientView::onBackPress, this));
	menu_exit->setPosition(Vec2(1240 , m_layerBottom->getContentSize().height / 2 - 5));

	auto menu = Menu::create(menu_AddGun, menu_SubGun, menu_AddBringGold, menu_SubBringGold, menu_setting, menu_help, menu_exit, nullptr);
	menu->setPosition(Vec2(0, 0));
	m_layerBottom->addChild(menu, 25);
}

void LrbyClientView::onPressSetting(Ref* sender)
{
	auto setting = FishSettingLayer::create();
	setting->setLayerType(TAG_SETTING);
	setting->setPosition(Vec2((kResolutionWidth - setting->getContentSize().width) / 2, (kResolutionHeight - setting->getContentSize().height) / 2));
	this->addChild(setting, 100);
}
void LrbyClientView::onPressHelp(Ref* sender)
{
	IntroduceLayer *layer = IntroduceLayer::create();
	this->addChild(layer, 100);
	layer->setPosition(Vec2((kResolutionWidth - layer->getContentSize().width) / 2, (kResolutionHeight - layer->getContentSize().height) / 2));	
}

void LrbyClientView::addTipsView()
{
	//add tip view
	cocos2d::Size size = Director::getInstance()->getWinSize();

	auto arrow = Sprite::create("709/fish_cannon/fish_arrow.png");
	arrow->setPosition(Vec2(size.width * 3 / 4 - 120, 110));
	arrow->setTag(TAG_ARROW_ADD_SCORE);
	this->addChild(arrow, 100);
	//auto fadeIn = FadeIn::create(1);
	auto blink = Blink ::create(5, 8);
	auto out = FadeOut::create(1);

	auto *action = Sequence::create(blink, out, CallFunc::create(this,callfunc_selector(LrbyClientView::removeTipScoreView)), NULL);

	arrow->runAction(action);

	auto tip1 = Sprite::create("709/fish_cannon/fish_text_score.png");
	tip1->setTag(TAG_TEXT_SCORE);
	tip1->setPosition(arrow->getPositionX() - tip1->getContentSize().width / 2 - 40, arrow->getPositionY() + 15);
	this->addChild(tip1, 101);
}

void LrbyClientView::removeTipScoreView()
{
	auto arrow = this->getChildByTag(TAG_ARROW_ADD_SCORE);
	auto tip = this->getChildByTag(TAG_TEXT_SCORE);

	if (arrow)
	{
		this->removeChild(arrow);
	}

	if (tip)
	{
		this->removeChild(tip);
	}
}

void LrbyClientView::addUserPositionTipsView(int posIndex)
{
	//add tip view
	cocos2d::Size size = Director::getInstance()->getWinSize();

	auto arrow = Sprite::create("709/fish_cannon/fish_arrow.png");
	
	
	arrow->setTag(TAG_ARROW_MY_POSITION);
	this->addChild(arrow, 100);
	auto blink = Blink ::create(5, 8);
	auto out = FadeOut::create(1);

	auto *action = Sequence::create(blink, out, CallFunc::create(this,callfunc_selector(LrbyClientView::removePositionTipsView)), NULL);

	

	auto tip1 = Sprite::create("709/fish_cannon/fish_text_here.png");
	tip1->setTag(TAG_TEXT_POSITION);
	

	if (posIndex >= 0 && posIndex <= 2)
	{
		arrow->setRotation(180);
		arrow->setPosition(Vec2(ptPositionX[posIndex] + 140, ptPositionY[posIndex] - 55));

		tip1->setPosition(arrow->getPositionX() - tip1->getContentSize().width / 2  - 40, arrow->getPositionY() - 20);
	}
	if (posIndex == 3)
	{
		arrow->setRotation(270);
		arrow->setPosition(Vec2(ptPositionX[posIndex] - 60, ptPositionY[posIndex] - 100));
		tip1->setRotation(270);
		tip1->setPosition(arrow->getPositionX() - 20, arrow->getPositionY() + tip1->getContentSize().height / 2 + 130);
	}
	if (posIndex == 7)
	{
		arrow->setRotation(90);
		arrow->setPosition(Vec2(ptPositionX[posIndex] + 60, ptPositionY[posIndex] - 100));
		tip1->setRotation(90);
		tip1->setPosition(arrow->getPositionX() + 20, arrow->getPositionY() + tip1->getContentSize().height / 2 + 130);
	}
	if (posIndex >= 4 && posIndex <= 6)
	{
		arrow->setPosition(Vec2(ptPositionX[posIndex] + 140, ptPositionY[posIndex] + 55));
		tip1->setPosition(arrow->getPositionX() - tip1->getContentSize().width / 2 - 40, arrow->getPositionY() + 15);
	}

	arrow->runAction(action);

	
	this->addChild(tip1, 101);
}
void LrbyClientView::removePositionTipsView()
{
	auto arrow = this->getChildByTag(TAG_ARROW_MY_POSITION);
	auto tip = this->getChildByTag(TAG_TEXT_POSITION);

	if (arrow)
	{
		this->removeChild(arrow);
	}

	if (tip)
	{
		this->removeChild(tip);
	}
	addTipsView();
}

void LrbyClientView::netDissconnect(Ref* pSender)
{
	/*
	PromptBox * promptBox = PromptBox::PromptBoxWith(Vec2(kResolutionWidth * 0.5, kResolutionHeight * 0.5),mPromptTypeGameServerShut);
	addChild(promptBox,1000);
	*/

	FishSettingLayer* layer = FishSettingLayer::create();
	layer->setPosition(Vec2((kResolutionWidth - layer->getContentSize().width) / 2, (kResolutionHeight - layer->getContentSize().height) / 2));
	layer->setLayerType(TAG_DISCONNECT);
	this->addChild(layer, 100);
}

void LrbyClientView::checkUserEnterBackground(float dt)
{
	if (this->getChildByTag(6626))
	{
		this->removeChildByTag(6626);
	}
	
	FishSettingLayer* layer = FishSettingLayer::create();
	layer->setPosition(Vec2((kResolutionWidth - layer->getContentSize().width) / 2, (kResolutionHeight - layer->getContentSize().height) / 2));
	//layer->setPosition(Vec2::ZERO);
	layer->setLayerType(TAG_IN_BACKGROUND);
	layer->setTag(6626);
	this->addChild(layer, 1001);
}

void LrbyClientView::createLockLineView(int chairId, FishSprite* fish)
{
	int fishId = m_LockFishManager->GetLockFishID(chairId);
	int fishKind = m_LockFishManager->GetLockFishKind(chairId);
	int currFishId = fish->getFishId();
	bool switchLock = false;//更换锁定的鱼
	if (fishKind == FISH_KIND_COUNT || fishId == 0)
	{
		//m_LockFishManager->setVisible(false);
		m_LockFishManager->ClearLockTrace(chairId);
		showLockFishIcon(chairId, 0, 0);
		if (chairId == m_nCurChair)
		{
			//没有能锁定的鱼，解除锁定状态
			m_isLockFish = false;
		}
		return;
	}
	if (FishManager::insideScreen(fish->getFishPosition()))
	{
		m_LockFishManager->SetEnterFishStatue(chairId, true);
	}
	if (!FishManager::insideScreen(fish->getFishPosition()) || fish->getFishStatue() == FISH_DIE)
	{
		if (m_LockFishManager->GetEnterFishStatue(chairId))
		{
			switchLock = true;
		}
	}
	if (m_isLockFish && switchLock && chairId == m_nCurChair)
	{
		 m_LockFishManager->ClearLockTrace(chairId);
		 showLockFishIcon(chairId, 0, 0);
		 FishKind lock_fish_kind;
		 m_LockFishManager->SetLockFishID(m_nCurChair, FishManager::GetInstance()->lockFish(&lock_fish_kind));
		 m_LockFishManager->SetLockFishKind(m_nCurChair, lock_fish_kind);
		 if (m_LockFishManager->GetLockFishID(m_nCurChair) == 0)
		 {
			 //没有能锁定的鱼，解除锁定状态
			 m_isLockFish = false;
		 }
		 return;
	}
	if (chairId != m_nCurChair && switchLock)
	{
		//隐藏当前玩家锁定鱼动画
		 m_LockFishManager->ClearLockTrace(chairId);
		 showLockFishIcon(chairId, 0, 0);
		 return;
	}
	
	m_LockFishManager->createLockLine(chairId, Vec2(ptPositionX[chairId], ptPositionY[chairId]), fish->getFishPosition());
	auto cannonPos = cannon[chairId]->getPosition();
	float angle = MathAide::CalcAngle(fish->getFishPosition().x, fish->getFishPosition().y, cannonPos.x, cannonPos.y);

	(cannon[chairId])->setCannonAngle(angle + kChairDefaultAngle[chairId]);

	if (chairId == m_nCurChair)
	{
		auto fish1 = FishManager::GetInstance()->getFishById(m_LockFishManager->GetLockFishID(m_nCurChair));
		if (fish1)
		{
			m_ptLastPos = fish1->getFishPosition();
		}
	}
	showLockFishIcon(chairId, m_LockFishManager->GetLockFishID(chairId), m_LockFishManager->GetLockFishKind(chairId));
}

void LrbyClientView::showLockFishIcon(int chairId, int fishId, int fishKind)
{
	if (fishId <= 0 || fishKind < FISH_KIND_16 || fishKind > FISH_KIND_40)
	{
		m_lockFishIcon[chairId]->setVisible(false);
		return;
	}
	if (m_lockFishIcon[chairId]->getTag() == fishId)
	{
		m_lockFishIcon[chairId]->setVisible(true);
		return;
	}
	std::string str = String::createWithFormat("lock_flag_%d.png", fishKind)->getCString();
	m_lockFishIcon[chairId]->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(str));

	m_lockFishIcon[chairId]->setTag(fishId);
	m_lockFishIcon[chairId]->setVisible(true);
}