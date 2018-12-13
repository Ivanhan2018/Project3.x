#include "cocos2d.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "RoomLayer.h"
#include "ConfigMgr.h"
#include "EntityMgr.h"
#include "TableViewLayer.h"
#include "LoadingLayer.h"
#include "LoginLayer.h"
#include "MyLoginLayer.h"
#include "ScenePlaza.h"
#include "SceneControl.h"
#include "DBHandler.h"
#include "SoundControl.h"
#include "MovingLabelLayer.h"
using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
	SimpleAudioEngine::end();
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{

	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("QiXing", Rect(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH));
		//需要显示的大小必须放到designresolution之前
		glview->setFrameSize(WINDOW_HEIGHT, WINDOW_WIDTH);
#else
		glview = GLViewImpl::create("QiXing");
#endif
		director->setOpenGLView(glview);
	}

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);
	
	// Set the design resolution	
	glview->setDesignResolutionSize(SCREEN_HEIGHT, SCREEN_WIDTH, ResolutionPolicy::EXACT_FIT);
 
	// turn on display FPS
#if defined(WIN32)						// windows 平台
	director->setDisplayStats(true);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	[[UIApplication sharedApplication] setStatusBarHidden:NO];
	[UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleLightContent;
#endif

	std::string wFile = CCFileUtils::sharedFileUtils()->getWritablePath();
	FileUtils::getInstance()->addSearchPath("SceneBackDialog/");
	FileUtils::getInstance()->addSearchPath("logon/");
	FileUtils::getInstance()->addSearchPath("PlazzScene_Src/");
	FileUtils::getInstance()->addSearchPath("plaza/");

	FileUtils::getInstance()->addSearchPath("res/");
	FileUtils::getInstance()->addSearchPath("res1/");
	FileUtils::getInstance()->addSearchPath(wFile);
	
    FileUtils::getInstance()->addSearchPath("LotteryImage/");
    FileUtils::getInstance()->addSearchPath("background/");
    FileUtils::getInstance()->addSearchPath("hlddz_particle/");
    FileUtils::getInstance()->addSearchPath("music/");
    FileUtils::getInstance()->addSearchPath("soundeffect/");
    FileUtils::getInstance()->addSearchPath("xml/");
    FileUtils::getInstance()->addSearchPath("xyx_music/");
    FileUtils::getInstance()->addSearchPath("ShopImage/");
	FileUtils::getInstance()->addSearchPath("bjl/");
	FileUtils::getInstance()->addSearchPath("bjl/background/");
	FileUtils::getInstance()->addSearchPath("bjl/BJL/");
	FileUtils::getInstance()->addSearchPath("bjl/hlddz_particle/");
	FileUtils::getInstance()->addSearchPath("bjl/ui_card/");
	FileUtils::getInstance()->addSearchPath("bjl/xml/");
	FileUtils::getInstance()->addSearchPath("bjl/xyx_music/");
	FileUtils::getInstance()->addSearchPath("bjl/soundeffect/");
	FileUtils::getInstance()->addSearchPath("dzpk/");
	FileUtils::getInstance()->addSearchPath("dzpk/plist/");
	FileUtils::getInstance()->addSearchPath("dzpk/xml/");

    bool hasMusic = UserDefault::getInstance()->getBoolForKey("hasMusic", true);
    if(hasMusic == true){
       SoundControl::sharedSoundControl()->playBackGroundMusic("GAME_PLACKGROUND.mp3", true);
    }

	//开始等待消息
	EntityMgr::instance()->startSchedule();

	//Scene *pScene = RoomLayer::scene();
	//auto pScene = ScenePlaza::createScene();//MyRoomLayer
	//auto pScene = LoginLayer::scene();
	CCScene *pScene = LoadingLayer::scene();
	director->runWithScene(pScene);

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
	Director::getInstance()->stopAnimation();

	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();	
}

void AppDelegate::initPlatformConfig()
{
}