#include "RechargeLayer.h"
#include "RoomLayer.h"
#include "GroupSprite.h"
using namespace cocos2d;
RechargeLayer::RechargeLayer()
{

}
RechargeLayer::~RechargeLayer()
{

}
Scene* RechargeLayer::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		RechargeLayer *layer = RechargeLayer::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}


bool RechargeLayer::init()
{
	bool bRet = false;
	do 
	{

		cocos2d::Size winSize = Director::getInstance()->getWinSize();

		LayerColor* layer = LayerColor::create(ccc4(0, 0, 0, 255 * 0.5f), SCREEN_HEIGHT, SCREEN_WIDTH);
		layer->ignoreAnchorPointForPosition(false);
		layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
		this->addChild(layer);
		/**添加背景图片**/
		Sprite *pBGLeft = Sprite::createWithSpriteFrame(spriteFrame("dt_kefu_10.png")); 
		pBGLeft->setAnchorPoint(Vec2(1.0f,0.5f));
		pBGLeft->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
		this->addChild(pBGLeft);
		Sprite *pBGRight = Sprite::createWithSpriteFrame(spriteFrame("dt_kefu_10.png")); 
		pBGRight->setRotationY(180);
		pBGRight->setAnchorPoint(Vec2(0.0f,0.5f));
		pBGRight->setPosition(Vec2(winSize.width*0.5+pBGRight->getContentSize().width,winSize.height*0.5));
		this->addChild(pBGRight);

		/**添加标题LOG**/
		Sprite *pTitleLog = Sprite::createWithSpriteFrame(spriteFrame("dt_recharge_2.png")); 
		pTitleLog->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5+pBGRight->getContentSize().height/2-28));
		this->addChild(pTitleLog,2);

		/**添加关闭按钮**/
		Sprite *pCloseNormalImage = Sprite::createWithSpriteFrame(spriteFrame("dt_kefu_0.png")); 
		Sprite *pCloseSelectImage =Sprite::createWithSpriteFrame(spriteFrame("dt_kefu_0.png")); 
		MenuItemSprite *pCloseMenuItem= MenuItemSprite::create
			(pCloseNormalImage,pCloseSelectImage,CC_CALLBACK_1(RechargeLayer::dismiss,this)); 
		pCloseSelectImage->setScale(0.9f);
		pCloseSelectImage->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu *pCloseMenu = Menu::create(pCloseMenuItem,NULL);
		pCloseMenu->setPosition(winSize.width*0.5+pBGRight->getContentSize().width-25,winSize.height*0.5+pBGRight->getContentSize().height/2-25);
		this->addChild(pCloseMenu,2);

		/**添加确定按钮**/
		Sprite *pSureNormalImage =(Sprite *)GroupSprite::GroupSpriteWith("dt_warning_8.png","dt_prop_0.png",GorupSpriteTypePhotoAndPhoto);
		Sprite *pSureSelectImage =(Sprite *)GroupSprite::GroupSpriteWith("dt_warning_8.png","dt_prop_0.png",GorupSpriteTypePhotoAndPhoto);
		
		MenuItemSprite *pSureMenuItem= MenuItemSprite::create
			(pSureNormalImage,pSureSelectImage,CC_CALLBACK_1(RechargeLayer::makeSure,this)); 
		pSureSelectImage->setScale(0.9f);
		pSureSelectImage->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu *pSureMenu = Menu::create(pSureMenuItem,NULL);
		pSureMenu->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5-pBGRight->getContentSize().height/2+40));
		this->addChild(pSureMenu,2);


		/**添加文字提示**/
		//LabelTTF *pTextWaring0 =LabelTTF::create(ConfigMgr::instance()->text("t156"),"HelveticaNeue-Bold",18);
		//pTextWaring0->setPosition(Vec2(winSize.width*0.5,winSize.height*0.683));
		//this->addChild(pTextWaring0,3);

		//LabelTTF *pTextWaring1 =LabelTTF::create(ConfigMgr::instance()->text("t157"),"HelveticaNeue-Bold",18);
		//pTextWaring1->setPosition(Vec2(winSize.width*0.5,winSize.height*0.638));
		//this->addChild(pTextWaring1,4);

		///**添加图片提示**/
		//Sprite *pImageWarning0 = Sprite::createWithSpriteFrame(spriteFrame("dt_recharge_0.png")); 
		//pImageWarning0->setPosition(Vec2(winSize.width*0.5,winSize.height*0.43));
		//this->addChild(pImageWarning0,4);
		//Sprite *pImageWarning1 = Sprite::createWithSpriteFrame(spriteFrame("dt_recharge_1.png")); 
	 //   pImageWarning1->setPosition(Vec2(winSize.width*0.5,winSize.height*0.30));
		//this->addChild(pImageWarning1,4);

		Sprite *pImageWarning0 = Sprite::createWithSpriteFrame(spriteFrame("dt_recharge_0.jpg")); 
		pImageWarning0->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
		this->addChild(pImageWarning0,4);
		CC_BREAK_IF(! Layer::init());
		bRet = true;
	} while (0);

	return bRet;

}


void RechargeLayer::dismiss(Object *obj){ 
	RoomLayer *pRoomlayer = (RoomLayer *)this->getParent();
	pRoomlayer->permitButtonClick();
	this->removeFromParentAndCleanup(true);

}
void RechargeLayer::makeSure(Object *obj){
	//快捷充值 2快 = 60W + 话费点

}