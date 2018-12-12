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


BaseLayer::BaseLayer()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseLayer::CloseNetWorkShut), MSG_UI_ANS_NETWORKSHUT, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseLayer::ReConNetWork), MSG_UI_ANS_NETWORK_RECON, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BaseLayer::ReadyToConNet), MSG_UI_ANS_NETWORK_RECON_ING, NULL);
}
BaseLayer::~BaseLayer()
{
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_NETWORKSHUT);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_NETWORK_RECON);
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_NETWORK_RECON_ING);
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
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("display_DuangDong.xml","t180"),
		Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
	this->addChild(layer, 255);
}

void  BaseLayer::ReConNetWork(Object *obj)
{
	//刷新界面
	refreshLayer();
	//弹出提示
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("display_DuangDong.xml","t1000"),
		Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
	this->addChild(layer, 255);
}

void BaseLayer::ReadyToConNet(Object *obj)
{
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("display_DuangDong.xml","t1001"),
		Vec2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f));
	this->addChild(layer, 255);
}