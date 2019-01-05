#include "HelpLayer.h"
#include "RoomLayer.h"
#include "HelpScrollView.h"
#include "FreeLayer.h"
using namespace cocos2d;
HelpLayer::HelpLayer()
{

}
HelpLayer::~HelpLayer()
{

}
Scene* HelpLayer::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		HelpLayer *layer = HelpLayer::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}


bool HelpLayer::init()
{
	bool bRet = false;
	do 
	{	
		
		cocos2d::Size winSize = Director::getInstance()->getWinSize();
		srand(unsigned(time(NULL)));
		
		LayerColor *m_layer = LayerColor::create(ccc4(0, 0, 0, 255*0.5f), 813.0f, 400.0f);
		m_layer->ignoreAnchorPointForPosition(false);
		m_layer->setPosition(Vec2(winSize.width*0.5f,winSize.height*0.5f-50)); 
		this->addChild(m_layer,0);

		Sprite *helpLayerBk = Sprite::create("background/bg2.png");
		helpLayerBk->setPosition(Vec2(winSize.width/2,winSize.height/2));
		this->addChild(helpLayerBk,0);

		HelpScrollView *pHelpView = HelpScrollView::create();
		this->addChild(pHelpView,0);
	
		Sprite *maskSprite = Sprite::createWithSpriteFrame(spriteFrame("bg2_2.png"));
		maskSprite->setPosition(Vec2(winSize.width/2,winSize.height-maskSprite->getContentSize().height/2));
		this->addChild(maskSprite,1);

		Sprite *titleSprite = Sprite::createWithSpriteFrame(spriteFrame("Tab_GameRule2.png"));
		titleSprite->setPosition(Vec2(winSize.width/2,winSize.height*0.9f-2));
		this->addChild(titleSprite,1);

		//Sprite *pPlayerInfoBJLeft = Sprite::createWithSpriteFrame(spriteFrame("dt_email11.jpg"));
		//Sprite *pPlayerInfoBJRight = Sprite::createWithSpriteFrame(spriteFrame("dt_email11.jpg"));
		//pPlayerInfoBJRight->setRotationY(180);
		//pPlayerInfoBJLeft->setPosition(Vec2(winSize.width*0.25,winSize.height-52));
		//pPlayerInfoBJRight->setPosition(Vec2(winSize.width*0.75 - 1,winSize.height-52));
		//this->addChild(pPlayerInfoBJLeft,0);
		//this->addChild(pPlayerInfoBJRight,0);

	


		//Sprite *pPlayerInfoBJLeft1 = Sprite::createWithSpriteFrame(spriteFrame("dt_email10.png"));
		//Sprite *pPlayerInfoBJRight1 = Sprite::createWithSpriteFrame(spriteFrame("dt_email10.png"));
		//pPlayerInfoBJRight1->setRotationY(180);
		//pPlayerInfoBJLeft1->setPosition(Vec2(winSize.width*0.25+5,winSize.height*0.0-18));
		//pPlayerInfoBJRight1->setPosition(Vec2(winSize.width*0.75-5,winSize.height*0.0-18));
		//pPlayerInfoBJLeft1->setScaleY(1.2f);
		//pPlayerInfoBJRight1->setScaleY(1.2f);
		//pPlayerInfoBJLeft1->setAnchorPoint(Vec2(0.5,0));
		//pPlayerInfoBJRight1->setAnchorPoint(Vec2(0.5,0));
		//this->addChild(pPlayerInfoBJLeft1,1);
		//this->addChild(pPlayerInfoBJRight1,1);

		//Ãâ·Ñ
		//Sprite *pMianFeiSpriteNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_playerinfo_0.png"));
		//Sprite *pMianFeiSpriteSelect = Sprite::createWithSpriteFrame(spriteFrame("dt_playerinfo_0.png"));
		//MenuItemSprite* pMianfei = MenuItemSprite::create(pMianFeiSpriteNormal,pMianFeiSpriteSelect,CC_CALLBACK_1(HelpLayer::toFreeLayer));
		//pMianFeiSpriteSelect->setScale(0.9f);
		//pMianFeiSpriteSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		//Menu* mianfeiMenu = Menu::create(pMianfei,NULL);
		//mianfeiMenu->setPosition(Vec2(winSize.width*0.054,winSize.height*0.885));
		//this->addChild(mianfeiMenu,1);


		//Sprite *pPlayerInfoLogo = Sprite::createWithSpriteFrame(spriteFrame("dt_register_14.png"));
		//pPlayerInfoLogo->setPosition(Vec2(winSize.width*0.5,winSize.height*0.923));
		//this->addChild(pPlayerInfoLogo,2);

		//Sprite *pPlayerInfoLogoText = Sprite::createWithSpriteFrame(spriteFrame("dt_help_1.png"));
		//pPlayerInfoLogoText->setPosition(Vec2(winSize.width*0.5,winSize.height*0.933));
		//this->addChild(pPlayerInfoLogoText,3);



		Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
		Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
		MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
			(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(HelpLayer::dismiss,this));
		pCloseSelectButton->setScale(0.9f);
		pCloseSelectButton->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
		pCloseButton->setPosition(Vec2(winSize.width*0.943,winSize.height*0.923));
		this->addChild(pCloseButton,1);

		//Sprite *m_bantouming_bj_left = Sprite::createWithSpriteFrame(spriteFrame("dt_help_2.png"));
		//
		//m_bantouming_bj_left->setPosition(Vec2(427.0f-164.0f,26.0f));
		//this->addChild(m_bantouming_bj_left,4);

		//Sprite *m_bantouming_bj_right = Sprite::createWithSpriteFrame(spriteFrame("dt_help_2.png"));
		//m_bantouming_bj_right->setRotationY(180.0f);
		//m_bantouming_bj_right->setPosition(Vec2(427.0f+164.0f,26.0f));
		//this->addChild(m_bantouming_bj_right,4);

		pNode = Node::create();
		
		String* strTips = String::createWithFormat("t%d",rand()%9+420);

		m_pXiaoTiShi = LabelTTF::create(ConfigMgr::instance()->text(strTips->getCString()),"HelveticaNeue-Bold", 22);
		m_pXiaoTiShi->setPosition(Vec2(winSize.width*0.5,winSize.height*0.054));
		m_pXiaoTiShi->setColor(ccc3(255,255,0));
		this->addChild(m_pXiaoTiShi,5);

		LabelTTF *pWanFaShuoMing = LabelTTF::create(ConfigMgr::instance()->text("t140"),"",21);
		pWanFaShuoMing->setColor(ccc3(189,154,97));
		pWanFaShuoMing->setAnchorPoint(Vec2(0,0.5));
		pWanFaShuoMing->setPosition(Vec2(41.0f,346.0f));
		//this->addChild(pWanFaShuoMing,5);
	   /* HelpScrollLayer *pHelpScroll = HelpScrollLayer::HelpScrollLayerWith(0,34);
		pHelpScroll->setPosition(Vec2(21.0f,45.0f));
		this->addChild(pHelpScroll,5);
*/
		/*HelpScrollView *pHelpView = HelpScrollView::create();
		this->addChild(pHelpView,5);*/
		schedule(schedule_selector(HelpLayer::changeTipsContent),5.0f);
		this->setKeypadEnabled(true);
		bRet = true;
	} while (0);

	return bRet;

}

void HelpLayer::changeTipsContent(float dt)
{
	String* strTips = String::createWithFormat("t%d",rand()%9+420);

	m_pXiaoTiShi->setString(ConfigMgr::instance()->text(strTips->getCString()));
}

void HelpLayer::dismiss(Object *obj){
	playButtonSound();
	Scene *pScene = Scene::create();
	RoomLayer *pRoomLayer = RoomLayer::create();
	pRoomLayer->automaticLogin(false);
	pRoomLayer->closeLoginAward();
	pScene->addChild(pRoomLayer);
	Director::getInstance()->replaceScene(pScene);
}

void HelpLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //·µ»Ø
	{
		dismiss(NULL);
	}
}

void HelpLayer::toFreeLayer(Object *obj){
	playButtonSound();
	Director::getInstance()->replaceScene(FreeLayer::scene());

}

