#include "AppDelegate.h"
#include "HNGame.h"
#include "GameManagerBase.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

	GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
	return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("GameBase", Rect(0, 0, 960,540));
#else
		glview = GLViewImpl::create("GameBase");
#endif
		director->setOpenGLView(glview);
	}
	// turn on display FPS
	director->setDisplayStats(false);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);


	register_all_packages();

    // add by hxh 20161215
	FileUtils::getInstance()->addSearchPath("ClientHN_SC_HJ/");
	FileUtils::getInstance()->addSearchPath("ClientHN_SC_HJ/Script/");
	FileUtils::getInstance()->addSearchPath("ClientHN_SC_HJ/Script/HNMJ/");
	FileUtils::getInstance()->addSearchPath("ClientHN_SC_HJ/Script/GameXZDD/");
	

	// create a scene. it's an autorelease object
	Scene *pScene = HNGame::scene();

	srand(time(0));
	// run
	director->runWithScene(pScene);

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

	if (GameManagerBase::pInstanceBase())
	{
		GameManagerBase::InstanceBase().applicationDidEnterBackground();
	}
	// if you use SimpleAudioEngine, it must be pause
	// SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

	if (GameManagerBase::pInstanceBase())
	{
		GameManagerBase::InstanceBase().applicationWillEnterForeground();
	}
	// if you use SimpleAudioEngine, it must resume here
	// SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
