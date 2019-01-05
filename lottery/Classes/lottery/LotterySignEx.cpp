#include "LotterySignEx.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "EntityMgr.h"
#include "LotterySceneControl.h"

using namespace CocosDenshion;

LotterySignEx::LotterySignEx()
{

}

LotterySignEx::~LotterySignEx()
{

}

Scene* LotterySignEx::scene()
{
	Scene *scene = Scene::create();
	LotterySignEx *layer = LotterySignEx::create();
	scene->addChild(layer);
	return scene;
}

bool LotterySignEx::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initData();
	this->initView();
	this->setKeypadEnabled(true);
	EntityMgr::instance()->getDispatch()->SendPacketWithGetUserInfo();
	return true;
}

void LotterySignEx::onEnter()
{
	Layer::onEnter();
}

void LotterySignEx::onExit()
{
	Layer::onExit();
}

void LotterySignEx::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //·µ»Ø
	{
		back(NULL);
		event->stopPropagation();
	}
}

void LotterySignEx::back(Object *obj)
{
	playButtonSound();
	//Scene *scene = Scene::create();
	//LotteryUserInfo *layer = LotteryUserInfo::create();
	//scene->addChild(layer);
	//Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	Director::getInstance()->popScene();
}

void LotterySignEx::initData()
{
	winSize = Director::getInstance()->getWinSize();
}

void LotterySignEx::initView()
{

}