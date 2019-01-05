#include "ChoosePlayerPhotoLayer.h"
#include "PlayerInfoLayer.h"
#include "ChoosePhotoScrollLayer.h"
#include "define.h"

using namespace cocos2d;
ChoosePlayerPhotoLayer::ChoosePlayerPhotoLayer()
{

}
ChoosePlayerPhotoLayer::~ChoosePlayerPhotoLayer()
{

}
Scene* ChoosePlayerPhotoLayer::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		ChoosePlayerPhotoLayer *layer = ChoosePlayerPhotoLayer::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}


bool ChoosePlayerPhotoLayer::init()
{
	bool bRet = false;
	do 
	{
		cocos2d::Size winSize = Director::getInstance()->getWinSize();
		
		LayerColor* layer = LayerColor::create(ccc4(0, 0, 0, 255 * 0.5f), 854.0f, SCREEN_WIDTH);
		layer->ignoreAnchorPointForPosition(false);
		layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
		this->addChild(layer);

		Sprite *pWarningBJLeft = Sprite::createWithSpriteFrame(spriteFrame("dt_warning_3.png"));
		Sprite *pWarningBJRight = Sprite::createWithSpriteFrame(spriteFrame("dt_warning_3.png"));
		pWarningBJRight->setRotationY(180);
		pWarningBJLeft->setPosition(Vec2(winSize.width*0.28,winSize.height*0.5));
		pWarningBJRight->setPosition(Vec2(winSize.width*0.28+380,winSize.height*0.5));
		this->addChild(pWarningBJLeft,0);
		this->addChild(pWarningBJRight,0);
		

		Sprite *pbuttonNormalClose = Sprite::createWithSpriteFrame(spriteFrame("dt_warning_11.png"));
		Sprite *pbuttonSelectClose = Sprite::createWithSpriteFrame(spriteFrame("dt_warning_11.png"));
		MenuItemSprite *pButtonClose = MenuItemSprite::create
			(pbuttonNormalClose,pbuttonSelectClose,CC_CALLBACK_1(ChoosePlayerPhotoLayer::dismiss,this));
		pbuttonSelectClose->setScale(0.9f);
		pbuttonSelectClose->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu *pButton0 = Menu::create(pButtonClose,NULL);
		pButton0->setPosition(Vec2(winSize.width*0.920,winSize.height*0.850));  
		this->addChild(pButton0,2);
		LabelTTF *pChoosePhotoLog = LabelTTF::create(ConfigMgr::instance()->text("t138"),"HelveticaNeue-Bold", 22);
		pChoosePhotoLog->setPosition(Vec2(winSize.width*0.5,winSize.height*0.845));
		pChoosePhotoLog->setColor(ccc3(166,117,42));
		this->addChild(pChoosePhotoLog,2);

		/**添加女生图像**/
		ChoosePhotoScrollLayer* m_pSystemScorllView = ChoosePhotoScrollLayer::PhotoScrollLayerWith(0,3);
		m_pSystemScorllView->setPosition(Vec2(73,72));
		this->addChild(m_pSystemScorllView,0,101);
		/**添加男生图像**/
		ChoosePhotoScrollLayer* m_pSystemScorllView1 = ChoosePhotoScrollLayer::PhotoScrollLayerWith(1,3);
		m_pSystemScorllView1->setPosition(Vec2(73,230));
		this->addChild(m_pSystemScorllView1,0,100);

		//Sprite *pPhotoBJ1 = Sprite::create("dt_register_6.png");
		//Sprite *pPhotoBJ2 = Sprite::create("dt_register_6.png");
		//Sprite *pPhotoBJ3 = Sprite::create("dt_register_6.png");
		//Sprite *pPhotoBJ4 = Sprite::create("dt_register_6.png");
		//Sprite *pPhotoBJ5 = Sprite::create("dt_register_6.png");
		//Sprite *pPhotoBJ6 = Sprite::create("dt_register_6.png");
		//Sprite *pPhotoBJ7 = Sprite::create("dt_register_6.png");
		//Sprite *pPhotoBJ8 = Sprite::create("dt_register_6.png");
		//pPhotoBJ1->setPosition(Vec2(winSize.width*0.210,winSize.height*0.604));
		//pPhotoBJ2->setPosition(Vec2(winSize.width*0.210+165,winSize.height*0.604));
		//pPhotoBJ3->setPosition(Vec2(winSize.width*0.210+2*165,winSize.height*0.604));
		//pPhotoBJ4->setPosition(Vec2(winSize.width*0.210+3*165,winSize.height*0.604));
		//pPhotoBJ5->setPosition(Vec2(winSize.width*0.210,winSize.height*0.323));
		//pPhotoBJ6->setPosition(Vec2(winSize.width*0.210+165,winSize.height*0.323));
		//pPhotoBJ7->setPosition(Vec2(winSize.width*0.210+2*165,winSize.height*0.323));
		//pPhotoBJ8->setPosition(Vec2(winSize.width*0.210+3*165,winSize.height*0.323));
		//this->addChild(pPhotoBJ1,2);
		//this->addChild(pPhotoBJ2,2);
		//this->addChild(pPhotoBJ3,2);
		//this->addChild(pPhotoBJ4,2);
		//this->addChild(pPhotoBJ5,2);
		//this->addChild(pPhotoBJ6,2);
		//this->addChild(pPhotoBJ7,2);
		//this->addChild(pPhotoBJ8,2);
		///*	Sprite * pPhotoNormalImage1 = Sprite::create("dt_register_8.png");
		//Sprite * pPhotoSelectImage1 = Sprite::create("dt_register_8.png");
		//MenuItemSprite *pPhoto1 = MenuItemSprite::create
		//(pPhotoNormalImage1,pPhotoSelectImage1,CC_CALLBACK_1(ChoosePlayerPhotoLayer::choose1));
		//pPhotoSelectImage1->setScale(0.9f);
		//pPhotoSelectImage1->setAnchorPoint(Vec2(-0.5/9,-0.5/9));*/

		//Sprite * pPhotoNormalImage1 = Sprite::create("dt_register_8.png");
		//Sprite * pPhotoSelectImage1 = Sprite::create("dt_register_8.png");
		//MenuItemSprite *pPhoto1 = MenuItemSprite::create
		//	(pPhotoNormalImage1,pPhotoSelectImage1,CC_CALLBACK_1(ChoosePlayerPhotoLayer::choose1));
		//pPhotoSelectImage1->setScale(0.9f);
		//pPhotoSelectImage1->setAnchorPoint(Vec2(-0.5/9,-0.5/9));

		//Sprite * pPhotoNormalImage2 = Sprite::create("dt_register_8.png");
		//Sprite * pPhotoSelectImage2 = Sprite::create("dt_register_8.png");
		//MenuItemSprite *pPhoto2 = MenuItemSprite::create
		//	(pPhotoNormalImage2,pPhotoSelectImage2,CC_CALLBACK_1(ChoosePlayerPhotoLayer::choose2));
		//pPhotoSelectImage2->setScale(0.9f);
		//pPhotoSelectImage2->setAnchorPoint(Vec2(-0.5/9,-0.5/9));

		//Sprite * pPhotoNormalImage3 = Sprite::create("dt_register_8.png");
		//Sprite * pPhotoSelectImage3 = Sprite::create("dt_register_8.png");
		//MenuItemSprite *pPhoto3 = MenuItemSprite::create
		//	(pPhotoNormalImage3,pPhotoSelectImage3,CC_CALLBACK_1(ChoosePlayerPhotoLayer::choose3));
		//pPhotoSelectImage3->setScale(0.9f);
		//pPhotoSelectImage3->setAnchorPoint(Vec2(-0.5/9,-0.5/9));

		//Sprite * pPhotoNormalImage4 = Sprite::create("dt_register_8.png");
		//Sprite * pPhotoSelectImage4 = Sprite::create("dt_register_8.png");
		//MenuItemSprite *pPhoto4 = MenuItemSprite::create
		//	(pPhotoNormalImage4,pPhotoSelectImage4,CC_CALLBACK_1(ChoosePlayerPhotoLayer::choose4));
		//pPhotoSelectImage4->setScale(0.9f);
		//pPhotoSelectImage4->setAnchorPoint(Vec2(-0.5/9,-0.5/9));

		//Sprite * pPhotoNormalImage5 = Sprite::create("dt_register_8.png");
		//Sprite * pPhotoSelectImage5 = Sprite::create("dt_register_8.png");
		//MenuItemSprite *pPhoto5 = MenuItemSprite::create
		//	(pPhotoNormalImage5,pPhotoSelectImage5,CC_CALLBACK_1(ChoosePlayerPhotoLayer::choose5));
		//pPhotoSelectImage5->setScale(0.9f);
		//pPhotoSelectImage5->setAnchorPoint(Vec2(-0.5/9,-0.5/9));

		//Sprite * pPhotoNormalImage6 = Sprite::create("dt_register_8.png");
		//Sprite * pPhotoSelectImage6 = Sprite::create("dt_register_8.png");
		//MenuItemSprite *pPhoto6 = MenuItemSprite::create
		//	(pPhotoNormalImage6,pPhotoSelectImage6,CC_CALLBACK_1(ChoosePlayerPhotoLayer::choose6));
		//pPhotoSelectImage6->setScale(0.9f);
		//pPhotoSelectImage6->setAnchorPoint(Vec2(-0.5/9,-0.5/9));

		//Sprite * pPhotoNormalImage7 = Sprite::create("dt_register_8.png");
		//Sprite * pPhotoSelectImage7 = Sprite::create("dt_register_8.png");
		//MenuItemSprite *pPhoto7 = MenuItemSprite::create
		//	(pPhotoNormalImage7,pPhotoSelectImage7,CC_CALLBACK_1(ChoosePlayerPhotoLayer::choose7));
		//pPhotoSelectImage7->setScale(0.9f);
		//pPhotoSelectImage7->setAnchorPoint(Vec2(-0.5/9,-0.5/9));

		//Sprite * pPhotoNormalImage8 = Sprite::create("dt_register_8.png");
		//Sprite * pPhotoSelectImage8 = Sprite::create("dt_register_8.png");
		//MenuItemSprite *pPhoto8 = MenuItemSprite::create
		//	(pPhotoNormalImage8,pPhotoSelectImage8,CC_CALLBACK_1(ChoosePlayerPhotoLayer::choose8));
		//pPhotoSelectImage8->setScale(0.9f);
		//pPhotoSelectImage8->setAnchorPoint(Vec2(-0.5/9,-0.5/9));

		//Menu *pMenuTop = Menu::create(pPhoto1,pPhoto2,pPhoto3,pPhoto4,NULL);
		//pMenuTop->alignItemsHorizontallyWithPadding(98.0f);
		//Menu *pMenuBottom = Menu::create(pPhoto5,pPhoto6,pPhoto7,pPhoto8,NULL);
		//pMenuBottom->alignItemsHorizontallyWithPadding(98.0f);
		//pMenuTop->setPosition(Vec2(winSize.width*0.5,winSize.height*0.604));
		//pMenuBottom->setPosition(Vec2(winSize.width*0.5,winSize.height*0.323));
		//this->addChild(pMenuTop,3);
		//this->addChild(pMenuBottom,3);
		//m_choose_log =Sprite::create("dt_warning_0.png");
		//m_choose_log->setPosition(Vec2(206.0f,322.0f));
		//this->addChild(m_choose_log,4);
		bRet = true;
	} while (0);

	return bRet;

}

//void ChoosePlayerPhotoLayer::choose1(Object *obj)
//{
//	
//	m_choose_log->setPosition(Vec2(206.0f,322.0f));
//	
//	
//}
//void ChoosePlayerPhotoLayer::choose2(Object *obj)
//{
//	m_choose_log->setPosition(Vec2(206.0f+165,322.0f));
//	
//	
//}
//void ChoosePlayerPhotoLayer::choose3(Object *obj)
//{
//	m_choose_log->setPosition(Vec2(206.0f+165*2,322.0f));
//	
//}
//void ChoosePlayerPhotoLayer::choose4(Object *obj)
//{
//	m_choose_log->setPosition(Vec2(206.0f+165*3,322.0f));
//	
//}
//void ChoosePlayerPhotoLayer::choose5(Object *obj)
//{
//	m_choose_log->setPosition(Vec2(206.0f,188.0f));
//	
//}
//void ChoosePlayerPhotoLayer::choose6(Object *obj)
//{
//	m_choose_log->setPosition(Vec2(206.0f+165*1,188.0f));
//	
//}
//void ChoosePlayerPhotoLayer::choose7(Object *obj)
//{
//	m_choose_log->setPosition(Vec2(206.0f+165*2,188.0f));
//	
//}
//void ChoosePlayerPhotoLayer::choose8(Object *obj)
//{
//	m_choose_log->setPosition(Vec2(206.0f+165*3,188.0f));
//	
//}
void ChoosePlayerPhotoLayer::dismiss(Object *obj){
  PlayerInfoLayer* playInfo = (PlayerInfoLayer*)this->getParent();
  playInfo->permitButtom();
  this->removeFromParentAndCleanup(true);

}
