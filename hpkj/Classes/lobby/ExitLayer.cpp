#include "ExitLayer.h"
#include "EntityMgr.h"
#include "RoomLayer.h"
#include "GroupSprite.h"
#include "FreeLayer.h"
#include "OverRegistLayer.h"

bool ExitLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    cocos2d::Size winSize = Director::getInstance()->getWinSize();
    LayerColor* layer = LayerColor::create(ccc4(0, 0, 0, 255*0.5f), winSize.width, winSize.height);
    this->addChild(layer,-1);

	//背景
	Sprite* exitBack = Sprite::createWithSpriteFrame(spriteFrame("dt_exitlayer101.png"));
	exitBack->setPosition(Vec2(winSize.width * 0.5,winSize.height * 0.5));
	this->addChild(exitBack);

	//tips系统提示
	Sprite* spriteTips = Sprite::createWithSpriteFrame(spriteFrame("dt_exitlayer0.png"));
	spriteTips->setPosition(Vec2(winSize.width * 0.5,exitBack->getPositionY()+exitBack->getContentSize().height/2-spriteTips->getContentSize().height/2-5));
	this->addChild(spriteTips);

	//关闭对话框按钮
	Sprite* closeNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
	Sprite* closeSelect = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
	MenuItemSprite* closeI = MenuItemSprite::create(closeNormal,closeSelect,CC_CALLBACK_1(ExitLayer::close,this));
	closeSelect->setScale(0.9f);
	closeSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
	Menu* closeMenu = Menu::create(closeI,NULL);
	closeMenu->setPosition(Vec2(exitBack->getContentSize().width - closeNormal->getContentSize().width * 0.5+10,exitBack->getContentSize().height - closeNormal->getContentSize().height * 0.5+10));
	exitBack->addChild(closeMenu);

	//完善注册，继续游戏
	Sprite* overRegistNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_exitlayer100.png"));
	Sprite* overRegistSelect = Sprite::createWithSpriteFrame(spriteFrame("dt_exitlayer100.png"));
	
	Sprite* resumeNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_exitlayer1.png"));
	Sprite* resumeSelect = Sprite::createWithSpriteFrame(spriteFrame("dt_exitlayer1.png"));
	
	MenuItemSprite* overRegist = MenuItemSprite::create(overRegistNormal,overRegistSelect,CC_CALLBACK_1(ExitLayer::onOverRegist,this));
	MenuItemSprite* resumeItem = MenuItemSprite::create(resumeNormal,resumeSelect,CC_CALLBACK_1(ExitLayer::resumeGame,this));
	overRegistSelect->setScale(0.9f);
	overRegistSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
	resumeSelect->setScale(0.9f);
	resumeSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
	Menu* overRegistMenu = Menu::create(overRegist,NULL);
	Menu* resumeItemMenu = Menu::create(resumeItem,NULL);
	overRegistMenu->setPosition(Vec2(winSize.width * 0.5 - exitBack->getContentSize().width * 0.25,winSize.height * 0.47+30));
	resumeItemMenu->setPosition(Vec2(winSize.width * 0.5 - exitBack->getContentSize().width * 0.25,winSize.height * 0.47-40));
	this->addChild(overRegistMenu);
	this->addChild(resumeItemMenu);
	//免费获金币,离开游戏
	Sprite* freeGetCoinsNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_exitlayer2.png"));
	Sprite* freeGetCoinsSelect = Sprite::createWithSpriteFrame(spriteFrame("dt_exitlayer2.png"));
	
	Sprite* exitNormal =  Sprite::createWithSpriteFrame(spriteFrame("dt_exitlayer3.png"));
	Sprite* exitSelect =  Sprite::createWithSpriteFrame(spriteFrame("dt_exitlayer3.png"));
	MenuItemSprite* freeGetCoins = MenuItemSprite::create(freeGetCoinsNormal,freeGetCoinsSelect,CC_CALLBACK_1(ExitLayer::onFreeGetCoins,this));
	MenuItemSprite* exitItem = MenuItemSprite::create(exitNormal,exitSelect,CC_CALLBACK_1(ExitLayer::exitGame,this));
	freeGetCoinsSelect->setScale(0.9f);
	freeGetCoinsSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
	exitSelect->setScale(0.9f);
	exitSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
	Menu* freeGetCoinsMenu = Menu::create(freeGetCoins,NULL);
	Menu* exitItemMenu = Menu::create(exitItem,NULL);
	freeGetCoinsMenu->setPosition(Vec2(winSize.width * 0.5 + exitBack->getContentSize().width * 0.25,winSize.height * 0.47+30));
	exitItemMenu->setPosition(Vec2(winSize.width * 0.5 + exitBack->getContentSize().width * 0.25,winSize.height * 0.47-40));
	this->addChild(freeGetCoinsMenu);
	this->addChild(exitItemMenu);
	if(g_GlobalUnits.getGolbalUserData()->cbVisitor == 0)
	{
		freeGetCoinsMenu->setPosition(Vec2(winSize.width * 0.5,winSize.height * 0.47+30));
		overRegistMenu->setVisible(false);
	}
    
    return true;
}

ExitLayer::ExitLayer()
{
    
}

ExitLayer::~ExitLayer()
{
    
}

void ExitLayer::onFreeGetCoins(Object* obj)
{
	playButtonSound();
	Scene *scene = Scene::create();
	FreeLayer *pFreelayer = FreeLayer::create();
	scene->addChild(pFreelayer);
	Director::getInstance()->replaceScene(scene);   
}

void ExitLayer::onOverRegist(Object* obj)
{
	playButtonSound();
	Scene* scene = Scene::create();
	OverRegistLayer * pRegisterLayer = OverRegistLayer::create();
	pRegisterLayer->setRegistLayerFrom(1);
	scene->addChild(pRegisterLayer);
	Director::getInstance()->replaceScene(scene);
}

void ExitLayer::resumeGame(Object* obj)
{
	playButtonSound();
	RoomLayer* layer = (RoomLayer*)this->getParent();
	layer->permitButtonClick();
	this->removeFromParentAndCleanup(true);
}

void ExitLayer::exitGame(Object* obj)
{
	playButtonSound();
	Director::getInstance()->end();
}

void ExitLayer::close(Object* obj)
{
	playButtonSound();
	RoomLayer* layer = (RoomLayer*)this->getParent();
	layer->permitButtonClick();
	this->removeFromParentAndCleanup(true);
}

