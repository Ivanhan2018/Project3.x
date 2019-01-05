#include "AgreementLayer.h"
#include "RoomLayer.h"
#include "AgreementScrollView.h"
#include "RegistLayer.h"
using namespace cocos2d;
AgreementLayer::AgreementLayer()
{

}
AgreementLayer::~AgreementLayer()
{

}
Scene* AgreementLayer::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		AgreementLayer *layer = AgreementLayer::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}


bool AgreementLayer::init()
{
	bool bRet = false;
	do 
	{
		
		cocos2d::Size winSize = Director::getInstance()->getWinSize();

		Sprite *pPlayerInfoBJLeft = Sprite::createWithSpriteFrame(spriteFrame("dt_register_12.jpg"));
		Sprite *pPlayerInfoBJRight =Sprite::createWithSpriteFrame(spriteFrame("dt_register_12.jpg"));
		pPlayerInfoBJRight->setRotationY(180);
		pPlayerInfoBJLeft->setPosition(Vec2(winSize.width*0.25,winSize.height*0.5));
		pPlayerInfoBJRight->setPosition(Vec2(winSize.width*0.75,winSize.height*0.5));
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


		Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
		Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
		MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
			(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(AgreementLayer::dismiss,this));	
		pCloseSelectButton->setScale(0.9f);
		pCloseSelectButton->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
		pCloseButton->setPosition(Vec2(winSize.width*0.943,winSize.height*0.923));
		this->addChild(pCloseButton,1);

		AgreementScrollView * pAgreementView = AgreementScrollView::create();
		this->addChild(pAgreementView,2);

		bRet = true;
		this->setKeypadEnabled(true);
	} while (0);

	return bRet;

}

void AgreementLayer::dismiss(Object *obj){
	Scene *pScene = Scene::create();
	RegistLayer *pRegistLayer = RegistLayer::create();
	//pRegistLayer->automaticLogin(false);
	//pRegistLayer->closeLoginAward();
	pScene->addChild(pRegistLayer);
	Director::getInstance()->replaceScene(pScene);
}

void AgreementLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //·µ»Ø
	{
		dismiss(NULL);
	}
}
