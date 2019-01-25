#include "LotteryHappyHouse.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "EntityMgr.h"
#include "LotterySceneControl.h"
#include "LotteryActiveCenter.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryHappyHouse::scene()
{
	Scene *scene = Scene::create();
	LotteryHappyHouse *layer = LotteryHappyHouse::create();
	scene->addChild(layer);
	return scene;
}

LotteryHappyHouse::LotteryHappyHouse()
{
	
}
LotteryHappyHouse::~LotteryHappyHouse()
{
	
}

bool LotteryHappyHouse::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initData();
	this->initView();
	this->setKeypadEnabled(true);
	return true;
}

void LotteryHappyHouse::initData()
{
	winSize = Director::getInstance()->getWinSize();
}

void LotteryHappyHouse::initView()
{
	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	//
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame(HEAD_IMG));
	bk1->setAnchorPoint(Vec2(0,1));
	bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
	bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
	bk1->setScaleY(162 / bk1->getContentSize().height);
	this->addChild(bk1,1);

	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryHappyHouse::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	ccColor3B blackColor = ccc3(76,76,76);

	//欢乐颂
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t51"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	//背景图
	Sprite* sptBody = Sprite::createWithSpriteFrame(spriteFrame("hd_02.png"));
	sptBody->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	sptBody->setPosition(Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 162));
	this->addChild(sptBody);
}

void LotteryHappyHouse::onEnter()
{
	Layer::onEnter();
}

void LotteryHappyHouse::onExit()
{
	Layer::onExit();
}

void LotteryHappyHouse::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void LotteryHappyHouse::back(Object *obj)
{
	playButtonSound();
	Scene *scene = Scene::create();
	LotteryActiveCenter *layer = LotteryActiveCenter::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}