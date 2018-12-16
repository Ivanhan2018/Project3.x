#include "LoadingLayer.h"
#include "RoomLayer.h"
#include "PromptBox.h"
#include "EntityMgr.h"
#include "LoginLayer.h"
#include "MsgDispatch.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../JniHelper.h"
#endif

using namespace cocos2d;
LoadingLayer::LoadingLayer()
{

}
LoadingLayer::~LoadingLayer()
{

}
Scene* LoadingLayer::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		LoadingLayer *layer = LoadingLayer::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}


bool LoadingLayer::init()
{
	bool bRet = false;
	do 
	{
		cocos2d::Size winSize = WINSIZE;


		UserDefault::getInstance()->getBoolForKey("ZJD_SOUNDSTATE",true);
		if(!UserDefault::getInstance()->getFloatForKey("ZJD_SOUNDNUM"))
		{
			UserDefault::getInstance()->setFloatForKey("ZJD_SOUNDNUM",0.5);
			UserDefault::getInstance()->flush();
			CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
		}
		else
		{
			float dt = UserDefault::getInstance()->getFloatForKey("ZJD_SOUNDNUM");
			CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(dt);
		}
	
	    Sprite *pLoadingBG = Sprite::createWithSpriteFrame(spriteFrame("login_01.png"));
		if (pLoadingBG)
		{			
			pLoadingBG->setPosition(Vec2(winSize.width*0.5, winSize.height*0.5));
			this->addChild(pLoadingBG);
		}	   	

		this->scheduleUpdate();

		bRet = true;
	} while (0);

	return bRet;

}

void LoadingLayer::onEnter()
{
	Layer::onEnter();	
	
}

void LoadingLayer::update(float dt)
{
		this->unscheduleUpdate();

		CCLOG("update must run once!");
		
		//出错
		bool bRet = pMsgDispatch->connectLoginServer();		

		if(bRet)
		{
			int version = ((pConfigMgr->m_VersionInfo.m_nHighVersion)<<24) + ((pConfigMgr->m_VersionInfo.m_nMiddleVersion)<<16) + (pConfigMgr->m_VersionInfo.m_nLowVersion);
		}
		else
		{
			cocos2d::Size winSize = WINSIZE;
			PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeServerShut);
			this->addChild(box,100);
		}
      
        bool auto_login = UserDefault::getInstance()->getBoolForKey("AUTO_LOGIN", false);
        if (auto_login == true) {
            UserDefault::getInstance()->setBoolForKey("AUTO_LOGIN", false);
            pMsgDispatch->getVersion();
            RoomLayer* layer = RoomLayer::create();
            Scene* pScene = Scene::create();
            layer->automaticLogin(true, true);
            pScene->addChild(layer);
            TransitionFade *tScene = TransitionFade::create(0.5f, pScene, Color3B::WHITE);
            Director::getInstance()->replaceScene(tScene);
            return;
        }
    
		LoginLayer *layer = LoginLayer::create();
		Scene* pScene = Scene::create();
		pScene->addChild(layer);
		TransitionFade *tScene = TransitionFade::create(0.5f, pScene, Color3B::WHITE);
		Director::getInstance()->replaceScene(tScene);
		///layer->checkIfAutoLogin(0);	
}