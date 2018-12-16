//
//  LuckyDraw.cpp
//  Game
//
// Created by wangjun on 15-1-7.
//
//

#include "LuckyDraw.h"
#include "CocosUnits.h"
#include "AnsString.h"
#include "utf-8.h"
#include "ConfigMgr.h"
#include "EntityMgr.h"
#include "RoomLayer.h"
#include "JsonHelper.h"
#include "PromptBox.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../JniHelper.h"
#endif

USING_NS_CC;

LuckyDraw::LuckyDraw()
{
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LuckyDraw::turnTableRet), "TurnTableRet", NULL);
}

LuckyDraw::~LuckyDraw()
{
    NotificationCenter::getInstance()->removeObserver(this, "TurnTableRet");
}

bool LuckyDraw::init()
{
	canDraw = true;
	cocos2d::Size winSize = WINSIZE;
	LayerColor *base = LayerColor::create(ccc4(0, 0, 0, 255 * 0.5f), SCREEN_HEIGHT, SCREEN_WIDTH);
	this->addChild(base);
	//����
	Sprite *background = Sprite::createWithSpriteFrame(spriteFrame("Bk_LuckyDraw1.png"));
	background->setPosition(Vec2(winSize.width/2-98, winSize.height/2));
	this->addChild(background);	

	Vector<SpriteFrame*> animFrames;
	animFrames.pushBack(spriteFrame("Bk_LuckyDraw1.png"));
	animFrames.pushBack(spriteFrame("Bk_LuckyDraw2.png"));
	animFrames.pushBack(spriteFrame("Bk_LuckyDraw3.png"));
	animFrames.pushBack(spriteFrame("Bk_LuckyDraw4.png"));
	Animation *animation = Animation::createWithSpriteFrames(animFrames, 0.2);
	FiniteTimeAction *forever = RepeatForever::create(Animate::create(animation));
	background->runAction(forever);

	//ָ��
	pinSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_LuckDrawPin.png"));
	pinSprite->setAnchorPoint(Vec2(0.5f, 0));
	pinSprite->setPosition(Vec2(winSize.width/2, winSize.height/2+5));
	this->addChild(pinSprite);	
	//�齱��ť
	Sprite *itemNormalImage = Sprite::createWithSpriteFrame(spriteFrame("Sprite_LuckDrawButton1.png"));
	Sprite *itemSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("Sprite_LuckDrawButton2.png"));
	MenuItemSprite *menuItem = MenuItemSprite::create(itemNormalImage,itemSelectedImage,CC_CALLBACK_1(LuckyDraw::pressDraw,this));
	drawMenu = Menu::create(menuItem, NULL);
	drawMenu->setPosition(Vec2(winSize.width/2, winSize.height/2+5));
	this->addChild(drawMenu,1);
	//����
	Sprite* backNormal= Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
	Sprite* backSelect= Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
	MenuItemSprite* backItemSprite = MenuItemSprite::create(backNormal,backSelect,CC_CALLBACK_1(LuckyDraw::back,this));
	backSelect->setScale(0.9f);
	backSelect->setAnchorPoint(Vec2(594,337));
	Menu* backMenu= Menu::create(backItemSprite,NULL);
	backMenu->setPosition(Vec2(winSize.width * 0.93,winSize.height * 0.93));
	this->addChild(backMenu);
	return true;
}

void LuckyDraw::onEnter()
{
	Layer::onEnter();
	//Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,-1000,true);
}

void LuckyDraw::onExit()
{
	//
	Layer::onExit();
}

void LuckyDraw::back(Object* obj)
{
	//playButtonSound();
	//Scene *pScene = Scene::create();
	//RoomLayer *pRoomLayer = RoomLayer::create();
	//pRoomLayer->automaticLogin(false);
	//pRoomLayer->closeLoginAward();
	//pScene->addChild(pRoomLayer);
	//Director::getInstance()->replaceScene(pScene);

	playButtonSound();
	RoomLayer* layer = (RoomLayer*)this->getParent();
	layer->permitButtonClick();
	this->removeFromParentAndCleanup(true);
}

void LuckyDraw::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
	}
}

void LuckyDraw::pressDraw( Object* obj)
{
	if(canDraw == false)
	{
		return;
	}
	pinSprite->setRotation(0);
	canDraw = false;
    pMsgDispatch->SendPacketWithTurnTable();
//	this->addRandSeed();
//	int num = rand()%8;
//    lastLuckyNum = num;
//	this->pinMove(num);
}

void LuckyDraw::pinMove(int tag)
{
	float anglePerUnit = 360/8;
	float finalAngle = anglePerUnit*tag;
	float circle = 4;
	this->addRandSeed();
	finalAngle+=rand()%43+1;
	finalAngle+=360*circle;
	CCRotateBy *rotateBy = CCRotateBy::create(10.0, finalAngle);
	CCEaseExponentialOut *easeAction = CCEaseExponentialOut::create(dynamic_cast<CCActionInterval *>(rotateBy));
	Sequence *sequence = Sequence::createWithTwoActions(easeAction, CallFuncN::create(this, callfuncN_selector(LuckyDraw::pinMoveEnd)));
	pinSprite->runAction(sequence);
}

void LuckyDraw::pinMoveEnd(Node *obj)
{
    cocos2d::Size winSize = WINSIZE;
    PromptBox *promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeGetError);
    promptBox->setPromptText(pConfigMgr->text(String::createWithFormat("t%d",1011+lastLuckyNum)->getCString()));
    this->addChild(promptBox,1);
	canDraw = true;
}

void LuckyDraw::addRandSeed()
{
	timeval psv;	
	gettimeofday(&psv, NULL);
	unsigned long int rand_seed = psv.tv_sec*1000+psv.tv_usec/1000;
	srand(rand_seed);
}

void LuckyDraw::turnTableRet(Object *pObject)
{
    Array *data = (Array *)pObject;
    lastLuckyNum = ((Integer *)data->objectAtIndex(0))->getValue();
    moneyEarned = ((Integer *)data->objectAtIndex(1))->getValue();
    this->pinMove(lastLuckyNum);

}