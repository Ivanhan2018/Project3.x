//
//  BaseLayer.cpp
//  Game
//
//  Created by 罗敏 on 13-6-19.
//  Modify by hd
//

#include "BaseLayer.h"
#include "GBEventIDs.h"
#include "MovingLabelLayer.h"
#include "ConfigMgr.h"
#include "Define.h"
#include "LoginLayer.h"

BaseLayer::BaseLayer()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseLayer::CloseNetWorkShut), MSG_UI_ANS_LS_NETWORKSHUT, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseLayer::ReConNetWork), MSG_UI_ANS_NETWORK_RECON, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseLayer::ReadyToConNet), MSG_UI_ANS_NETWORK_RECON_ING, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseLayer::getSystemTimeObject), "GetSystemTimeRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseLayer::backToLogin), "BackToLogin",NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseLayer::changeNet), "NetWorkChange",NULL);
}
BaseLayer::~BaseLayer()
{
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_LS_NETWORKSHUT);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_NETWORK_RECON);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_NETWORK_RECON_ING);
	NotificationCenter::getInstance()->removeObserver(this, "GetSystemTimeRet");
	NotificationCenter::getInstance()->removeObserver(this, "BackToLogin");
	NotificationCenter::getInstance()->removeObserver(this, "NetWorkChange");
}

bool BaseLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	return true;
}

void  BaseLayer::CloseNetWorkShut(Object *obj)
{
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml","t180"),
		Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
	Director::getInstance()->getRunningScene()->addChild(layer, 255);
}

void  BaseLayer::ReConNetWork(Object *obj)
{
	//刷新界面
	refreshLayer();
	//重置
	EntityMgr::instance()->getDispatch()->resetConTimes();
	//为0则不需要弹提示了
	Integer* tempInt = (Integer *)obj;
	if (tempInt->getValue() == 0|| tempInt->getValue() == 1) return;
	//弹出提示
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml","t1000"),
		Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
	Director::getInstance()->getRunningScene()->addChild(layer, 255);
}

void BaseLayer::getSystemTimeObject(Object *obj)
{
	EntityMgr::instance()->getDispatch()->resetConTimes();
}

void BaseLayer::ReadyToConNet(Object *obj)
{
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml","t1001"),
		Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
	Director::getInstance()->getRunningScene()->addChild(layer, 255);
}

void BaseLayer::backToLogin(Object *obj)
{
	LoginLayer *loginLayer = LoginLayer::create();
	Scene* pScene = Scene::create();
	pScene->addChild(loginLayer);
	TransitionFade *tScene = TransitionFade::create(0.5f, pScene, Color3B::WHITE);
	Director::getInstance()->replaceScene(tScene);	

	//1为网络不稳定
	Integer* tempInt = (Integer *)obj;
	if(tempInt->getValue())
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml","t1003"),
			Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
		loginLayer->addChild(layer, 255);	
	} else
	{	
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml","t1002"),
			Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
		loginLayer->addChild(layer, 255);
	}
}

void BaseLayer::changeNet(Object *obj)
{
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml","t1004"),
		Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
	Director::getInstance()->getRunningScene()->addChild(layer, 255);
}