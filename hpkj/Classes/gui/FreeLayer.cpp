#include "FreeLayer.h"
#include "RoomLayer.h"
#include "FreeScrollLayer.h"
#include "MovingLabelLayer.h"
#include "RoomLayer.h"
#include "FreeCell.h"
using namespace cocos2d;
FreeLayer::FreeLayer()
{
	NotificationCenter::getInstance()->addObserver(
		this, callfuncO_selector(FreeLayer::onGetDownData), MSG_UI_ANS_DOWNLOADDATA, NULL);
	NotificationCenter::getInstance()->addObserver(
		this, callfuncO_selector(FreeLayer::getDownAwardOver), MSG_UI_ANS_DOWNLOADAWARDOVER, NULL);
	
}
FreeLayer::~FreeLayer()
{
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_DOWNLOADDATA);
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_DOWNLOADAWARDOVER);
}
Scene* FreeLayer::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		FreeLayer *layer = FreeLayer::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

void FreeLayer::onGetDownData(Object* obj)
{
	SDownloadGamePrize_Returnn* data = (SDownloadGamePrize_Returnn*)obj;
	memcpy(&returnData, data, sizeof(SDownloadGamePrize_Returnn));
	LabelTTF* label = (LabelTTF*)this->getChildByTag(123);
	label->removeFromParentAndCleanup(true);
	//FreeScrollLayer *pFreeScroll = FreeScrollLayer::FreeScrollLayerWith(4,&returnData);
	//pFreeScroll->setPosition(Vec2(20.0f,3.0f));
	//this->addChild(pFreeScroll,2);
	FreeCell *sprite0 = FreeCell::FreeCellWith(0,data->dwDownloadGameInfo >> 0 & 1);
	sprite0->setAnchorPoint(Vec2::ZERO);
	sprite0->setCellIdx(0,data->dwDownloadGameInfo >> 0 & 1);
	sprite0->setPosition(Vec2(10, 310));
	this->addChild(sprite0,5);
	FreeCell *sprite1 = FreeCell::FreeCellWith(1,data->dwDownloadGameInfo >> 1 & 1);
	sprite1->setAnchorPoint(Vec2::ZERO);
	sprite1->setCellIdx(1,data->dwDownloadGameInfo >> 1 & 1);
	sprite1->setPosition(Vec2(10, 220));
	this->addChild(sprite1,5);
	FreeCell *sprite2 = FreeCell::FreeCellWith(2,data->dwDownloadGameInfo >> 2 & 1);
	sprite2->setAnchorPoint(Vec2::ZERO);
	sprite2->setCellIdx(2,data->dwDownloadGameInfo >> 2 & 1);
	sprite2->setPosition(Vec2(10, 130));
	this->addChild(sprite2,5);
	FreeCell *sprite3 = FreeCell::FreeCellWith(3,data->dwDownloadGameInfo >> 3 & 1);
	sprite3->setAnchorPoint(Vec2::ZERO);
	sprite3->setCellIdx(3,data->dwDownloadGameInfo >> 3 & 1);
	sprite3->setPosition(Vec2(10, 40));
	this->addChild(sprite3,5);




	Sprite *m_bantouming_bj_left = Sprite::createWithSpriteFrame(spriteFrame("dt_help_2.png"));

	m_bantouming_bj_left->setPosition(Vec2(427.0f-164.0f,20.0f));
	this->addChild(m_bantouming_bj_left,4);

	Sprite *m_bantouming_bj_right = Sprite::createWithSpriteFrame(spriteFrame("dt_help_2.png"));
	m_bantouming_bj_right->setRotationY(180.0f);
	m_bantouming_bj_right->setPosition(Vec2(427.0f+164.0f,20.0f));
	this->addChild(m_bantouming_bj_right,4);

	LabelTTF* labelTips = LabelTTF::create(pConfigMgr->text("t474"),"",22);
	labelTips->setPosition(Vec2(427,20));
	labelTips->setColor(ccc3(0,0,0));
	this->addChild(labelTips,5);
}

void FreeLayer::getDownAwardOver(Object* obj)
{
	MovingLabelLayer* moveingLabel = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t473"),Vec2(427,240));
	this->addChild(moveingLabel,99);
	Scene *pScene = Scene::create();
	RoomLayer *pRoomLayer = RoomLayer::create();
	pRoomLayer->automaticLogin(false);
	pRoomLayer->closeLoginAward();
	pScene->addChild(pRoomLayer);
	TransitionFade *tScene = TransitionFade::create(2.0f, pScene, Color3B::WHITE); 
	Director::getInstance()->replaceScene(tScene); 
}


bool FreeLayer::init()
{
	bool bRet = false;
	do 
	{

		pMsgDispatch->connectLoginServer();
		pMsgDispatch->getDownLoadAward(0,0);
		cocos2d::Size winSize = Director::getInstance()->getWinSize();
		Sprite *pPlayerInfoBJLeft = Sprite::createWithSpriteFrame(spriteFrame("dt_register_12.jpg"));
		Sprite *pPlayerInfoBJRight = Sprite::createWithSpriteFrame(spriteFrame("dt_register_12.jpg"));
		pPlayerInfoBJRight->setRotationY(180);
		pPlayerInfoBJLeft->setPosition(Vec2(winSize.width*0.25,winSize.height*0.5));
		pPlayerInfoBJRight->setPosition(Vec2(winSize.width*0.75 - 1,winSize.height*0.5));
		this->addChild(pPlayerInfoBJLeft,0);
		this->addChild(pPlayerInfoBJRight,0);


		Sprite *pPlayerInfoBJLeft1 = Sprite::createWithSpriteFrame(spriteFrame("dt_email8.jpg"));
		Sprite *pPlayerInfoBJRight1 = Sprite::createWithSpriteFrame(spriteFrame("dt_email8.jpg"));
		pPlayerInfoBJRight1->setRotationY(180);
		pPlayerInfoBJLeft1->setPosition(Vec2(winSize.width*0.25+5,winSize.height*0.0-18));
		pPlayerInfoBJRight1->setPosition(Vec2(winSize.width*0.75-5,winSize.height*0.0-18));
		pPlayerInfoBJLeft1->setScaleY(1.2f);
		pPlayerInfoBJRight1->setScaleY(1.2f);
		pPlayerInfoBJLeft1->setAnchorPoint(Vec2(0.5,0));
		pPlayerInfoBJRight1->setAnchorPoint(Vec2(0.5,0));
		this->addChild(pPlayerInfoBJLeft1,1);
		this->addChild(pPlayerInfoBJRight1,1);

		/*Sprite *pMianFeiSprite = Sprite::create("dt_playerinfo_0.png");
		pMianFeiSprite->setPosition(Vec2(winSize.width*0.054,winSize.height*0.885));
		this->addChild(pMianFeiSprite,2);*/

		LabelTTF* label = LabelTTF::create(pConfigMgr->text("t20"),"",30);
		label->setPosition(Vec2(427,240));
		label->setColor(ccc3(0,0,0));
		this->addChild(label,5,123);


		Sprite *pPlayerInfoLogo =  Sprite::createWithSpriteFrame(spriteFrame("dt_register_14.png"));
		pPlayerInfoLogo->setPosition(Vec2(winSize.width*0.5,winSize.height*0.923));
		this->addChild(pPlayerInfoLogo,2);

		Sprite *pPlayerInfoLogoText = Sprite::createWithSpriteFrame(spriteFrame("dt_free_3.png"));
		pPlayerInfoLogoText->setPosition(Vec2(winSize.width*0.5,winSize.height*0.933));
		this->addChild(pPlayerInfoLogoText,3);



		Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
		Sprite *pCloseSelectButton =  Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
		MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
			(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(FreeLayer::dismiss,this));
		pCloseSelectButton->setScale(0.9f);
		pCloseSelectButton->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
		pCloseButton->setPosition(Vec2(winSize.width*0.943,winSize.height*0.923));
		this->addChild(pCloseButton,1);
		
		this->setKeypadEnabled(true);

		bRet = true;
	} while (0);

	return bRet;

}

void FreeLayer::dismiss(Object *obj){
	playButtonSound();
	Scene *pScene = Scene::create();
	RoomLayer *pRoomLayer = RoomLayer::create();
	pRoomLayer->automaticLogin(false);
	pRoomLayer->closeLoginAward();
	pScene->addChild(pRoomLayer);
	Director::getInstance()->replaceScene(pScene);
}

void FreeLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //·µ»Ø
	{
		dismiss(NULL);
	}
}