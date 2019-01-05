#include "LotterySettingView.h"
#include "RoomLayer.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"

#include "ChongQingWuFen.h"
#include "LotteryHelpView.h"
#include "LotteryModifyPassWord.h"
#include "LotterySceneControl.h"
#include "LotteryCardBinding.h"
#include "LoginLayer.h"
#include "LotteryAbout.h"
#include "LotteryFeedback.h"
#include "VersionControl.h"

using namespace CocosDenshion;
using namespace extension;

Scene* LotterySettingView::scene()
{
	Scene *scene = Scene::create();
	LotterySettingView *layer = LotterySettingView::create();
	scene->addChild(layer);
	return scene;
}

LotterySettingView::LotterySettingView()
{
	//NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotterySettingView::onAnsLoginFinish), MSG_UI_ANS_LOGINFINISH, NULL);
}
LotterySettingView::~LotterySettingView()
{
	//NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_LOGINFINISH);
}

bool LotterySettingView::init()
{
	if (!Layer::init())
	{
		return false;
	}
	winSize = Director::getInstance()->getWinSize();

	startX = 10.0f;
	startY1 = 680.0f;
	lineGap = 66.0f;
	startY2 = startY1-lineGap*2;

	this->initView();
	this->initLabel();
	this->initButton();
	//this->schedule(schedule_selector(LotterySettingView::checkIfAutoLogin), 1, true, 0.5);
	this->setKeypadEnabled(true);
	return true;
}

void LotterySettingView::initView()
{
	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame(HEAD_IMG));
	bk1->setAnchorPoint(Vec2(0,1));
	bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
	bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
	bk1->setScaleY(162 / bk1->getContentSize().height);
	this->addChild(bk1,1);

	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotterySettingView::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t141"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	//
	Sprite * loginOutNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("setting_02.png"));
	Sprite * loginOutSelectSprite = Sprite::createWithSpriteFrame(spriteFrame("setting_03.png"));
	MenuItemSprite *loginOutItemSprite = MenuItemSprite::create(loginOutNormalSprite, loginOutSelectSprite, CC_CALLBACK_1(LotterySettingView::loginOut,this));
	Menu *loginOutMenu = Menu::create(loginOutItemSprite,NULL);
	loginOutMenu->setPosition(Vec2(SCREEN_WIDTH/2, 200));
	this->addChild(loginOutMenu, 1);
}

void LotterySettingView::loginOut(Object *obj)
{
	playButtonSound();

	Scene *scene = Scene::create();
	LoginLayer *layer = LoginLayer::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

void LotterySettingView::initLabel()
{
	float fontSize = 27;
	for(int i = 0 ; i <= 1 ; i++)
	{
		LabelTTF *label1 = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", 
			String::createWithFormat("t%d", 141+i)->getCString()), "", fontSize);
		label1->setPosition(Vec2(startX, startY1-i*lineGap-lineGap*0.5));
		label1->setColor(ccc3(0,0,0));
		label1->setAnchorPoint(Vec2(0,0.5f));
		this->addChild(label1, 1);

		Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
		gap1->setAnchorPoint(Vec2(0,0.5));
		gap1->setPosition(Vec2(0, label1->getPositionY()-lineGap/2));
		gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
		this->addChild(gap1,1);

		LabelTTF *label2 = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", 
			String::createWithFormat("t%d", 145+i)->getCString()), "", fontSize);
		label2->setPosition(Vec2(startX, startY2-i*lineGap-lineGap*0.5));
		label2->setColor(ccc3(0,0,0));
		label2->setAnchorPoint(Vec2(0,0.5f));
		this->addChild(label2, 1);

		Sprite *gap2 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
		gap2->setAnchorPoint(Vec2(0,0.5));
		gap2->setPosition(Vec2(0, label2->getPositionY()-lineGap/2));
		gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
		this->addChild(gap2,1);
	}

	Sprite *gap3 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap3->setAnchorPoint(Vec2(0,0.5));
	gap3->setPosition(Vec2(0, startY1));
	gap3->setScaleX(SCREEN_WIDTH / gap3->getContentSize().width);
	this->addChild(gap3,1);

	//Sprite *gap4 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	//gap4->setAnchorPoint(Vec2(0,1));
	//gap4->setPosition(Vec2(0, startY2));
	//gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
	//this->addChild(gap4,1);
}

void LotterySettingView::initButton()
{
	for(int i = 0 ; i <= 1 ; i++)
	{
		Sprite *normalImage1 = Sprite::createWithSpriteFrame(spriteFrame("setting_06.png"));
		Sprite *selectedImage1 = Sprite::createWithSpriteFrame(spriteFrame("setting_06.png"));
		MenuItemSprite *menuItem1 = MenuItemSprite::create(normalImage1,selectedImage1,CC_CALLBACK_1(LotterySettingView::settingAction,this));
		menuItem1->setTag(i+SETTING_ITEM_TAG);
		Menu *menu1 = Menu::createWithItem(menuItem1);
		menu1->setPosition(Vec2(SCREEN_WIDTH/2, startY1 - i*lineGap - lineGap*0.5));
		//menu1->setPosition(Vec2(SCREEN_WIDTH-startX*2, startY1-i*lineGap-lineGap*0.5));
		this->addChild(menu1);

		Sprite *normalImage2 = Sprite::createWithSpriteFrame(spriteFrame("setting_06.png"));
		Sprite *selectedImage2 = Sprite::createWithSpriteFrame(spriteFrame("setting_06.png"));
		MenuItemSprite *menuItem2 = MenuItemSprite::create(normalImage2,selectedImage2,CC_CALLBACK_1(LotterySettingView::settingAction,this));
		menuItem2->setTag(i+SETTING_ITEM_TAG+3);
		Menu *menu2 = Menu::createWithItem(menuItem2);
		menu2->setPosition(Vec2(SCREEN_WIDTH/2, startY2 - i*lineGap - lineGap*0.5));
		//menu2->setPosition(Vec2(SCREEN_WIDTH-startX*2, startY2-i*lineGap-lineGap*0.5));	
		this->addChild(menu2);
	}
}


void LotterySettingView::settingAction(Object *obj)
{
	int objTag = ((Node *)obj)->getTag();
	if(objTag == SETTING_ITEM_TAG){
		Scene *scene = Scene::create();
		LotteryModifyPassWord *layer = LotteryModifyPassWord::create();
		scene->addChild(layer);
		Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}else if(objTag == SETTING_ITEM_TAG+1){
		Scene *scene = Scene::create();
		LotteryCardBinding *layer = LotteryCardBinding::create();
		scene->addChild(layer);
		Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}else if(objTag == SETTING_ITEM_TAG+4){
		Scene *scene = Scene::create();
		LotteryAbout *layer = LotteryAbout::create();
		scene->addChild(layer);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}else if(objTag == SETTING_ITEM_TAG+3){
		Scene *scene = Scene::create();
		LotteryFeedback *layer = LotteryFeedback::create();
		scene->addChild(layer);
		Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	}
}

void LotterySettingView::onEnter()
{
	BaseLayer::onEnter();
}

void LotterySettingView::onExit()
{
	BaseLayer::onExit();
}

void LotterySettingView::back(Object *obj)
{
	playButtonSound();
	RoomLayer *pRoomLayer = RoomLayer::create();
	pRoomLayer->automaticLogin(false);
	Scene* pScene = Scene::create();
	pScene->addChild(pRoomLayer);
	Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(pScene));
}

void LotterySettingView::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}