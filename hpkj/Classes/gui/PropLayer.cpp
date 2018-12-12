#include "PropLayer.h"
#include "MenuLayer.h"
#include "cocos-ext.h"
#include "GroupSprite.h"
#include "EntityMgr.h"

using namespace extension;
using namespace cocos2d;
PropLayer::PropLayer()
{

}
PropLayer::~PropLayer()
{

}
Scene* PropLayer::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		PropLayer *layer = PropLayer::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}


bool PropLayer::init()
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
		Sprite *pBGLeft = Sprite::createWithSpriteFrame(spriteFrame("dt_kefu_12.png")); 
		pBGLeft->setAnchorPoint(Vec2(0.5f,0.5f));
		pBGLeft->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
		this->addChild(pBGLeft);

		/**添加标题LOG**/
		Sprite *pTitleLog =Sprite::createWithSpriteFrame(spriteFrame("dt_prop_1.png"));  
		pTitleLog->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5+pBGLeft->getTexture()->getPixelsHigh()/2-28));
		this->addChild(pTitleLog,2);

		/**添加关闭按钮**/
		Sprite *pCloseNormalImage = Sprite::createWithSpriteFrame(spriteFrame("dt_kefu_0.png")); 
		Sprite *pCloseSelectImage = Sprite::createWithSpriteFrame(spriteFrame("dt_kefu_0.png")); 
		MenuItemSprite *pCloseMenuItem= MenuItemSprite::create
			(pCloseNormalImage,pCloseSelectImage,CC_CALLBACK_1(PropLayer::dismiss,this)); 
		pCloseSelectImage->setScale(0.9f);
		pCloseSelectImage->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu *pCloseMenu = Menu::create(pCloseMenuItem,NULL);
		pCloseMenu->setPosition(winSize.width*0.5+pBGLeft->getTexture()->getPixelsWide()/2-25,winSize.height*0.5+pBGLeft->getTexture()->getPixelsHigh()/2-25);
		this->addChild(pCloseMenu,2);

		/**添加确定按钮**/
		Sprite *pSureNormalImage =(Sprite *)GroupSprite::GroupSpriteWith("dt_warning_8.png","dt_prop_0.png",GorupSpriteTypePhotoAndPhoto);
		Sprite *pSureSelectImage =(Sprite *)GroupSprite::GroupSpriteWith("dt_warning_8.png","dt_prop_0.png",GorupSpriteTypePhotoAndPhoto);
		MenuItemSprite *pSureMenuItem= MenuItemSprite::create
			(pSureNormalImage,pSureSelectImage,CC_CALLBACK_1(PropLayer::makeSure,this)); 
		pSureSelectImage->setScale(0.9f);
		pSureSelectImage->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu *pSureMenu = Menu::create(pSureMenuItem,NULL);
		pSureMenu->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5-pBGLeft->getTexture()->getPixelsHigh()/2+40));
		this->addChild(pSureMenu,2);

		/**添加文字提示**/
		LabelTTF *pTextWaring0 =LabelTTF::create(pConfigMgr->text("t152"),"HelveticaNeue-Bold",22);
		pTextWaring0->setPosition(Vec2(winSize.width*0.5,winSize.height*0.683));
		this->addChild(pTextWaring0,3);

		LabelTTF *pTextWaring1 =LabelTTF::create(pConfigMgr->text("t153"),"HelveticaNeue-Bold",22);
		pTextWaring1->setPosition(Vec2(winSize.width*0.5,winSize.height*0.608));
		this->addChild(pTextWaring1,4);

	

		/**添加输入框**/
		m_pUserInput= EditBox::create(cocos2d::Size(225, 32),Scale9Sprite::createWithSpriteFrame(spriteFrame("dt_register_1.png")));
		m_pUserInput->setPosition(Vec2(winSize.width*0.5, winSize.height * 0.49));
		m_pUserInput->setPlaceHolder(pConfigMgr->text("t154"));
	

		m_pUserVerifyInput= EditBox::create(cocos2d::Size(225, 32),Scale9Sprite::createWithSpriteFrame(spriteFrame("dt_register_1.png")));
		m_pUserVerifyInput->setPosition(Vec2(winSize.width*0.5, winSize.height * 0.39));
		m_pUserVerifyInput->setPlaceHolder(pConfigMgr->text("t155"));

		this->addChild(m_pUserInput,2);
		this->addChild(m_pUserVerifyInput,2);

		CC_BREAK_IF(! Layer::init());
		bRet = true;
	} while (0);

	return bRet;

}


void PropLayer::dismiss(Object *obj)
{ 
	playButtonSound();
	 MenuLayer *pMenulayer = (MenuLayer *)this->getParent();
     pMenulayer->permitButtonClick();
	 this->removeFromParentAndCleanup(true);
	
 }
 void PropLayer::makeSure(Object *obj){
	 playButtonSound();
	 const char* szAccount1 = m_pUserInput->getText();
	 if (strlen(szAccount1) < 1)
	 {
		 //用户名有误
		 cocos2d::Size winSize = Director::getInstance()->getWinSize();
		 /*		 PromptBox * promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5, winSize.height * 0.5),mPromptTypeLenFaild);
		this->addChild(promptBox,20)*/;
		 return;
	 }
	 const char* szAccount2 = m_pUserVerifyInput->getText();

	 if (strcmp(szAccount1, szAccount2))
	 {
		 ////两次输入玩家名不一致，赠送失败
		 //cocos2d::Size winSize = Director::getInstance()->getWinSize();
		 //PromptBox * promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5, winSize.height * 0.5),mPromptTypePesetFail);
		 //this->addChild(promptBox,20);
		 return;
	 }

	 //建立连接
	 bool bRet = pMsgDispatch->connectLoginServer();
	 if (!bRet)
	 {
		 //cocos2d::Size winSize = Director::getInstance()->getWinSize();
		 //PromptBox * promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5, winSize.height * 0.5),mPromptTypeLoginConnectFaild);
		 //this->addChild(promptBox);
		 return;
	 }

	 pMsgDispatch->setLoginState(enLogin_Server);
	 pMsgDispatch->sendScoreGiftPacket(szAccount1,2000000);
                                                               
 }