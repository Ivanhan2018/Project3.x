#include "LotteryFeedback.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "EntityMgr.h"
#include "LotterySceneControl.h"
#include "LotterySettingView.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryFeedback::scene()
{
	Scene *scene = Scene::create();
	LotteryFeedback *layer = LotteryFeedback::create();
	scene->addChild(layer);
	return scene;
}

LotteryFeedback::LotteryFeedback()
{
	//NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryFeedback::setQukuanZhanghaoRet), "SetQukuanZhanghaoRet", NULL);	
}
LotteryFeedback::~LotteryFeedback()
{
	//NotificationCenter::getInstance()->removeObserver(this, "SetQukuanZhanghaoRet");
}

bool LotteryFeedback::init()
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

void LotteryFeedback::initData()
{
	winSize = Director::getInstance()->getWinSize();
}

void LotteryFeedback::initView()
{
	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	//±≥æ∞
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame(HEAD_IMG));
	bk1->setAnchorPoint(Vec2(0,1));
	bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
	bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
	bk1->setScaleY(162 / bk1->getContentSize().height);
	this->addChild(bk1,1);

	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryFeedback::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	ccColor3B blackColor = ccc3(76,76,76);

	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t145"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	Sprite *contentBk;
	contentBk = Sprite::createWithSpriteFrame(spriteFrame("beting03.png"));
	contentBk->setAnchorPoint(Vec2(0.5f,1));
	contentBk->setPosition(Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT - 110));
	//this->addChild(contentBk, 1);
	
	fontSize = 27;
	ccColor3B color = ccc3(0,0,0);
	LabelTTF *aboutLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t664"), "", fontSize);
	aboutLabel->setAnchorPoint(Vec2(0,1));
	aboutLabel->setPosition(Vec2(10, 650));
	aboutLabel->setColor(color);
	aboutLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
	this->addChild(aboutLabel, 1);

	LabelTTF *aboutLabel1 = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t666"), "", fontSize);
	aboutLabel1->setAnchorPoint(Vec2(0,1));
	aboutLabel1->setPosition(Vec2(10, 600));
	aboutLabel1->setColor(color);
	aboutLabel1->setHorizontalAlignment(kCCTextAlignmentLeft);
	this->addChild(aboutLabel1, 1);


}

void LotteryFeedback::onEnter()
{
	Layer::onEnter();
}

void LotteryFeedback::onExit()
{
	Layer::onExit();
}

void LotteryFeedback::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void LotteryFeedback::back(Object *obj)
{
	playButtonSound();
	Scene *scene = Scene::create();
	LotterySettingView *layer = LotterySettingView::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}