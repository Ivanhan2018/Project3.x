#include "CustomerServiceLayer.h"
#include "RoomLayer.h"
#include "GroupSprite.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../JniHelper.h"
#endif
using namespace cocos2d;
CustomerServiceLayer::CustomerServiceLayer()
{

}
CustomerServiceLayer::~CustomerServiceLayer()
{

}
Scene* CustomerServiceLayer::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		CustomerServiceLayer *layer = CustomerServiceLayer::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}


bool CustomerServiceLayer::init()
{
	bool bRet = false;
	do 
	{
		cocos2d::Size winSize = Director::getInstance()->getWinSize();
		LayerColor* layer = LayerColor::create(ccc4(0, 0, 0, 255 * 0.5f), 854.0f, SCREEN_WIDTH);
		layer->ignoreAnchorPointForPosition(false);
		layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
		this->addChild(layer);
		/**添加背景图片**/
		Sprite *pBGLeft = Sprite::createWithSpriteFrame(spriteFrame("dt_kefu_10.png"));
		pBGLeft->setAnchorPoint(Vec2(1.0f,0.5f));
		pBGLeft->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
		this->addChild(pBGLeft);
		Sprite *pBGRight = Sprite::createWithSpriteFrame(spriteFrame("dt_kefu_10.png"));
		pBGRight->setFlipX(true);
		pBGRight->setAnchorPoint(Vec2(0.0f,0.5f));
		pBGRight->setPosition(Vec2(winSize.width*0.5 - 1,winSize.height*0.5));
		this->addChild(pBGRight);

		/**添加标题LOG**/
		Sprite *pTitleLog =Sprite::createWithSpriteFrame(spriteFrame("dt_kefu_3.png"));
		pTitleLog->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5+pBGRight->getContentSize().height/2-28));
		this->addChild(pTitleLog,2);

		/**添加关闭按钮**/
		Sprite *pCloseNormalImage =Sprite::createWithSpriteFrame(spriteFrame("dt_kefu_0.png"));
		Sprite *pCloseSelectImage =Sprite::createWithSpriteFrame(spriteFrame("dt_kefu_0.png"));
		MenuItemSprite *pCloseMenuItem= MenuItemSprite::create
			(pCloseNormalImage,pCloseSelectImage,CC_CALLBACK_1(CustomerServiceLayer::dismiss,this)); 
		pCloseSelectImage->setScale(0.9f);
		pCloseSelectImage->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu *pCloseMenu = Menu::create(pCloseMenuItem,NULL);
		pCloseMenu->setPosition(winSize.width*0.5+pBGRight->getContentSize().width-25,winSize.height*0.5+pBGRight->getContentSize().height/2-25);
		this->addChild(pCloseMenu,2);

		Sprite *pCustomerPhoto = Sprite::createWithSpriteFrame(spriteFrame("dt_kefu_6.png"));
		pCustomerPhoto->setPosition(Vec2(winSize.width*0.30f+40,winSize.height*0.45f));
		this->addChild(pCustomerPhoto,2);

		/**添加在线客服的按钮**/
		Sprite *pNormalCSOnlineImage = (Sprite *)GroupSprite::GroupSpriteWith
			("dt_warning_8.png","dt_kefu_1.png",GorupSpriteTypePhotoAndPhoto);
		Sprite *pSelectCSOnlineImage = (Sprite *)GroupSprite::GroupSpriteWith
			("dt_warning_8.png","dt_kefu_1.png",GorupSpriteTypePhotoAndPhoto);
		MenuItemSprite *pCSOnline = MenuItemSprite::create
			(pNormalCSOnlineImage,pSelectCSOnlineImage,CC_CALLBACK_1(CustomerServiceLayer::customerOnLine,this));
        pSelectCSOnlineImage->setScale(0.9f);
		pSelectCSOnlineImage->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		/**添加客服电话的按钮**/
		Sprite *pNormalCSPhoneImage = (Sprite *)GroupSprite::GroupSpriteWith
			("dt_warning_8.png","dt_kefu_4.png",GorupSpriteTypePhotoAndPhoto);
		Sprite *pSelectCSPhoneImage = (Sprite *)GroupSprite::GroupSpriteWith
			("dt_warning_8.png","dt_kefu_4.png",GorupSpriteTypePhotoAndPhoto);
		
		MenuItemSprite *pCSPhone = MenuItemSprite::create
			(pNormalCSPhoneImage,pSelectCSPhoneImage,CC_CALLBACK_1(CustomerServiceLayer::customerPhone,this));
        pSelectCSPhoneImage->setScale(0.9f);
		pSelectCSPhoneImage->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu *pMenu = Menu::create(pCSOnline,pCSPhone,NULL);
		pMenu->setPosition(Vec2(winSize.width*0.65,winSize.height*0.45));
		pMenu->alignItemsVerticallyWithPadding(50.0f);
		this->addChild(pMenu,2);
		CC_BREAK_IF(! Layer::init());
		this->setKeypadEnabled(true);
		bRet = true;
	} while (0);

	return bRet;

}

void CustomerServiceLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		if(pConfigMgr->m_Config.m_bToAndroid)
		{
			pConfigMgr->m_Config.m_bToAndroid = false;
			return;
		}
		else
		{
			dismiss(NULL);
		}
	}
}



void CustomerServiceLayer::dismiss(Object *obj)
{ 
	playButtonSound();
	//isGoingRoomLayer = false;
	RoomLayer *pRoomlayer = (RoomLayer *)this->getParent();
	pRoomlayer->permitButtonClick();
	this->removeFromParentAndCleanup(true);

}
void CustomerServiceLayer::customerOnLine(Object *obj){
	playButtonSound();
	RoomLayer* layer = (RoomLayer*)this->getParent();
	layer->setGoingRoomLayer();
	pConfigMgr->m_Config.m_bToAndroid = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	showAndroidFeedBackLayer();
#endif
}
void CustomerServiceLayer::customerPhone(Object *obj){
	playButtonSound();
	RoomLayer* layer = (RoomLayer*)this->getParent();
	layer->setGoingRoomLayer();
	pConfigMgr->m_Config.m_bToAndroid = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	toPhoneSever();
#endif
}