#include "MenuLayer.h"
#include "HelpLayer.h"
#include "PropLayer.h"
#include "RoomLayer.h"
#include "FreeLayer.h"
#include "LoginLayer.h"
#include "OverRegistLayer.h"


using namespace cocos2d;
MenuLayer::MenuLayer()
{

}
MenuLayer::~MenuLayer()
{

}
Scene* MenuLayer::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		MenuLayer *layer = MenuLayer::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}


bool MenuLayer::init()
{
	bool bRet = false;
	do 
	{
		float posY = 368;
		cocos2d::Size winSize = Director::getInstance()->getWinSize();
		Sprite *menuBk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_SettingBk.png"));
		menuBk->setAnchorPoint(Vec2(0,0.5));
		menuBk->setPosition(Vec2(355,posY+2));
		this->addChild(menuBk);
		Sprite *musicSprite =Sprite::createWithSpriteFrame(spriteFrame("Sprite_Setting5.png"));
		Sprite *effectSprite =Sprite::createWithSpriteFrame(spriteFrame("Sprite_Setting6.png"));
		musicSprite->setPosition(Vec2(426,posY));
		effectSprite->setPosition(Vec2(633,posY));
		this->addChild(musicSprite);
		this->addChild(effectSprite);

		hasEffect = UserDefault::getInstance()->getBoolForKey("hasEffect", true);
		hasMusic = UserDefault::getInstance()->getBoolForKey("hasMusic", true);
		Sprite *musicCheckbox;
		Sprite *effectCheckbox;
		if(hasMusic)
		{
			musicCheckbox =Sprite::createWithSpriteFrame(spriteFrame("Sprite_Setting12.png"));
			if(!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
			{
				playBGM("music/roomback.mp3");
			}
		}else
		{
			musicCheckbox =Sprite::createWithSpriteFrame(spriteFrame("Sprite_Setting10.png"));
		}
		if(hasEffect)
		{
			effectCheckbox =Sprite::createWithSpriteFrame(spriteFrame("Sprite_Setting12.png"));
		}else
		{
			effectCheckbox =Sprite::createWithSpriteFrame(spriteFrame("Sprite_Setting10.png"));
		}
		musicItem = MenuItemSprite::create(musicCheckbox,musicCheckbox,CC_CALLBACK_1(MenuLayer::pressMusicItem,this));
		effectItem = MenuItemSprite::create(effectCheckbox,effectCheckbox,CC_CALLBACK_1(MenuLayer::pressEffectItem,this));
		Menu *musicMenu = Menu::create(musicItem, NULL);
		Menu *effectMenu = Menu::create(effectItem, NULL);
		musicMenu->setPosition(Vec2(musicSprite->getPositionX()+70,posY));
		effectMenu->setPosition(Vec2(effectSprite->getPositionX()+70,posY));
		this->addChild(musicMenu);
		this->addChild(effectMenu);
		CC_BREAK_IF(! Layer::init());
		this->setKeypadEnabled(true);
		bRet = true;
	} while (0);

	return bRet;

}

void MenuLayer::permitButtonClick()
{
	RoomLayer *pRoomLayer =  (RoomLayer *)this->getParent();
	pRoomLayer->permitButtonClick();
}		
void MenuLayer::forbidButtonClick()
{
	//pButtonMenu->setTouchEnabled(false);

}
void MenuLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //·µ»Ø
	{
		RoomLayer * pRoomLayer = (RoomLayer * )this->getParent();
		pRoomLayer->permitButtonClick();
		playButtonSound();
		removeFromParentAndCleanup(true);
	}
}

//ÓÎÏ·±³¾°ÒôÀÖ
void MenuLayer::pressMusicItem(Object* obj)
{
	bool bRet = false;
	if(hasMusic)
	{	
		musicItem->setNormalImage(Sprite::createWithSpriteFrame(spriteFrame("Sprite_Setting10.png")));
		musicItem->setSelectedImage(Sprite::createWithSpriteFrame(spriteFrame("Sprite_Setting10.png")));
		bRet = false;
	}else
	{
		musicItem->setNormalImage(Sprite::createWithSpriteFrame(spriteFrame("Sprite_Setting12.png")));
		musicItem->setSelectedImage(Sprite::createWithSpriteFrame(spriteFrame("Sprite_Setting12.png")));
		bRet = true;
	}
	hasMusic = bRet;
	UserDefault::getInstance()->setBoolForKey("hasMusic", bRet);
	UserDefault::getInstance()->flush();
	if(hasMusic)
	{
		if(!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		{
			playBGM("music/roomback.mp3");
		}
	}else
	{
		if(CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		{
			stopBGM();
		}
	}
}

//ÓÎÏ·±³¾°ÒôÀÖ
void MenuLayer::pressEffectItem(Object* obj)
{
	bool bRet = false;
	if(hasEffect)
	{	
		effectItem->setNormalImage(Sprite::createWithSpriteFrame(spriteFrame("Sprite_Setting10.png")));
		effectItem->setSelectedImage(Sprite::createWithSpriteFrame(spriteFrame("Sprite_Setting10.png")));
		bRet = false;
	}else
	{
		effectItem->setNormalImage(Sprite::createWithSpriteFrame(spriteFrame("Sprite_Setting12.png")));
		effectItem->setSelectedImage(Sprite::createWithSpriteFrame(spriteFrame("Sprite_Setting12.png")));
		bRet = true;
	}
	hasEffect = bRet;
	UserDefault::getInstance()->setBoolForKey("hasEffect", bRet);
	UserDefault::getInstance()->flush();
}