#include "VIPSystemLayer.h"
#include "RoomLayer.h"
#include "CocosUnits.h"
#include "ShopSystemLayer.h"

VIPSystemLayer::VIPSystemLayer()
{

}
VIPSystemLayer::~VIPSystemLayer()
{

}
Scene* VIPSystemLayer::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		VIPSystemLayer *layer = VIPSystemLayer::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

bool VIPSystemLayer::init()
{
	bool bRet = false;
	do 
	{
		cocos2d::Size winSize = WINSIZE;
		/**���ư�͸���ɰ�**/
		LayerColor* layer = LayerColor::create(ccc4(0, 0, 0, 255 * 0.5f), 854.0f, SCREEN_WIDTH);
		layer->ignoreAnchorPointForPosition(false);
		layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
		this->addChild(layer);


		/**���Ʊ���ͼƬ**/
		Sprite * pBJ = Sprite::create("Bk_VIP.png");
		pBJ->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
		this->addChild(pBJ,0);

		/**�����ť**/
		Sprite *normalSprite1 = Sprite::createWithSpriteFrame(spriteFrame("Sprite_VIP1.png"));
		Sprite *selectSprite1 = Sprite::createWithSpriteFrame(spriteFrame("Sprite_VIP2.png"));
		selectSprite1->setScale(0.9f);
		MenuItemSprite *menuSprite1 = MenuItemSprite::create
			(normalSprite1,selectSprite1,CC_CALLBACK_1(VIPSystemLayer::onDiscount,this));

		Menu *menu1= Menu::create(menuSprite1,NULL);
		menu1->setPosition(Vec2(242, 76));  
		this->addChild(menu1);

		//��ΪVIP��ť
		Sprite *normalSprite = Sprite::createWithSpriteFrame(spriteFrame("Button_VIP1.png"));
		Sprite *selectSprite = Sprite::createWithSpriteFrame(spriteFrame("Button_VIP2.png"));
		MenuItemSprite *menuSprite = MenuItemSprite::create
			(normalSprite,selectSprite,CC_CALLBACK_1(VIPSystemLayer::onBecomeVIP,this));

		Menu *menu= Menu::create(menuSprite,NULL);
		menu->setPosition(Vec2(579, menu1->getPositionY())); 
		this->addChild(menu);
		/**�رհ�ť**/
		Sprite *pbuttonNormalClose = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
		Sprite *pbuttonSelectClose = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
		MenuItemSprite *pButtonClose = MenuItemSprite::create
			(pbuttonNormalClose,pbuttonSelectClose,CC_CALLBACK_1(VIPSystemLayer::dismiss,this));
		pbuttonSelectClose->setScale(0.9f);
		pbuttonSelectClose->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu *pButton0 = Menu::create(pButtonClose,NULL);
		pButton0->setPosition(Vec2(pBJ->getPositionX()+pBJ->getContentSize().width/2-pbuttonNormalClose->getContentSize().width/2+10,
			pBJ->getPositionY()+pBJ->getContentSize().height/2-pbuttonNormalClose->getContentSize().height/2+10));  
		this->addChild(pButton0,2);
		bRet = true;
		this->setKeypadEnabled(true);
	} while (0);

	return bRet;

}

void VIPSystemLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		RoomLayer* layer = (RoomLayer*)this->getParent();
		layer->permitButtonClick();
		this->removeFromParentAndCleanup(true);
	}
}

void VIPSystemLayer::dismiss(Object *obj){
	playButtonSound();
	RoomLayer* layer = (RoomLayer*)this->getParent();
	layer->permitButtonClick();
	this->removeFromParentAndCleanup(true);

}

void VIPSystemLayer::onDiscount(Object* obj)
{
	playButtonSound();
	ShopSystemLayer *tabLayer = ShopSystemLayer::create();
	Scene *scene = Scene::create();
	scene->addChild(tabLayer);
	Director::getInstance()->replaceScene(scene);
}

void VIPSystemLayer::onBecomeVIP(Object* obj)
{
	playButtonSound();
	ShopSystemLayer *tabLayer = ShopSystemLayer::create();
	Scene *scene = Scene::create();
	scene->addChild(tabLayer);
	Director::getInstance()->replaceScene(scene);
}