//
//  GetCharge.cpp
//  Game
//
//  Created by 罗敏 on 13-8-6.
//
//

#include "ChongCoinsLayer.h"
#include <limits.h>
#include "EntityMgr.h"
#include "ConfigMgr.h"
#include "GroupSprite.h"
using namespace CocosDenshion;
using namespace extension;

ChongCoinsLayer * ChongCoinsLayer::ChongCoinsLayerWith(Vec2 pos)
{
    ChongCoinsLayer * charge = new ChongCoinsLayer();
    if(charge && charge->initChongCoinsLayer(pos))
    {
        charge->autorelease();
        return charge;
    }
    CC_SAFE_DELETE(charge);
    return charge;
}

bool ChongCoinsLayer::initChongCoinsLayer(Vec2 pos)
{
    if(!Layer::init())
    {
        return false;
    }

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(ChongCoinsLayer::onTouchBegan, this);
	listener->onTouchCancelled = CC_CALLBACK_2(ChongCoinsLayer::onTouchCancelled, this);
	listener->onTouchEnded = CC_CALLBACK_2(ChongCoinsLayer::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(ChongCoinsLayer::onTouchMoved, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    cocos2d::Size winSize = WINSIZE;
	LayerColor* layer = LayerColor::create(ccc4(0, 0, 0, 255 * 0.5f), 854.0f, SCREEN_WIDTH);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	Sprite *pBackLeft = Sprite::create("dt_warning_3.png");
	Sprite *pBackRight = Sprite::create("dt_warning_3.png");
	pBackRight->setRotationY(180);
	pBackLeft->setPosition(Vec2(winSize.width*0.5 - pBackLeft->getContentSize().width * 0.5,winSize.height*0.5));
	pBackRight->setPosition(Vec2(winSize.width*0.5 + pBackRight->getContentSize().width * 0.5,winSize.height*0.5));
	this->addChild(pBackLeft,0);
	this->addChild(pBackRight,0);




    Sprite * backNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_kefu_0.png"));
    Sprite * backSelect = Sprite::createWithSpriteFrame(spriteFrame("dt_kefu_0.png"));
    MenuItemSprite * backItemSprite = MenuItemSprite::create(backNormal, backSelect,  CC_CALLBACK_1(ChongCoinsLayer::closeChongCoinsLayer,this));
	backSelect->setScale(0.9f);
	backSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
    Menu* pBackMenu = Menu::create(backItemSprite,NULL);
	
    pBackMenu->setPosition(Vec2(winSize.width*0.928,winSize.height*0.848));
   this->addChild(pBackMenu);

	


    
	Sprite* zjdCardNormal = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange5.png","dt_rechange20.png",GorupSpriteTypePhotoAndPhoto);
	Sprite* zjdCardSelect = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange6.png","dt_rechange21.png",GorupSpriteTypePhotoAndPhoto);
	Sprite* zjdCardDis = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange6.png","dt_rechange21.png",GorupSpriteTypePhotoAndPhoto);

	Sprite* yidongSMSNormal = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange5.png","dt_rechange17.png",GorupSpriteTypePhotoAndPhoto);
	Sprite* yidongSMSSelect = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange6.png","dt_rechange18.png",GorupSpriteTypePhotoAndPhoto);
	Sprite* yidongSMSDis = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange6.png","dt_rechange18.png",GorupSpriteTypePhotoAndPhoto);

	Sprite* liantongSMSNormal = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange5.png","dt_rechange22.png",GorupSpriteTypePhotoAndPhoto);
	Sprite* liantongSMSSelect = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange6.png","dt_rechange23.png",GorupSpriteTypePhotoAndPhoto);
	Sprite* liantongSMSDis = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange6.png","dt_rechange23.png",GorupSpriteTypePhotoAndPhoto);

	Sprite* dianxingSMSNormal = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange5.png","dt_rechange12.png",GorupSpriteTypePhotoAndPhoto);
	Sprite* dianxingSMSSelect = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange6.png","dt_rechange13.png",GorupSpriteTypePhotoAndPhoto);
	Sprite* dianxingSMSDis = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange6.png","dt_rechange13.png",GorupSpriteTypePhotoAndPhoto);

	Sprite* phoneCardNormal = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange5.png","dt_rechange15.png",GorupSpriteTypePhotoAndPhoto);
	Sprite* phoneCardSelect = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange6.png","dt_rechange16.png",GorupSpriteTypePhotoAndPhoto);
	Sprite* phoneCardDis = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange6.png","dt_rechange16.png",GorupSpriteTypePhotoAndPhoto);

	Sprite* aliPayNormal = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange5.png","dt_rechange7.png",GorupSpriteTypePhotoAndPhoto);
	Sprite* aliPaySelect = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange6.png","dt_rechange8.png",GorupSpriteTypePhotoAndPhoto);
	Sprite* aliPayDis = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange6.png","dt_rechange8.png",GorupSpriteTypePhotoAndPhoto);
    
	m_pZJDCardItemSprite = MenuItemSprite::create(zjdCardNormal, zjdCardSelect, zjdCardDis,CC_CALLBACK_1(ChongCoinsLayer::showZJDCard,this));
	m_pYidongMSMItemSprite = MenuItemSprite::create(yidongSMSNormal, yidongSMSSelect, yidongSMSDis,CC_CALLBACK_1(ChongCoinsLayer::showYidongSMS,this));
	m_pLiantongMSMItemSprite = MenuItemSprite::create(liantongSMSNormal, liantongSMSSelect, liantongSMSDis,CC_CALLBACK_1(ChongCoinsLayer::showLiantongSMS,this));
	m_pDianxingMSMItemSprite = MenuItemSprite::create(dianxingSMSNormal, dianxingSMSSelect, dianxingSMSDis, CC_CALLBACK_1(ChongCoinsLayer::showDianxingSMS,this));
	m_pPhoneCardItemSprite = MenuItemSprite::create(phoneCardNormal, phoneCardSelect, phoneCardDis,CC_CALLBACK_1(ChongCoinsLayer::showPhoneCard,this));
	m_pAlipayMSMItemSprite = MenuItemSprite::create(aliPayNormal, aliPaySelect, aliPayDis,CC_CALLBACK_1(ChongCoinsLayer::showAliPay,this));
	m_pZJDCardItemSprite->setScale(0.9f);
	m_pYidongMSMItemSprite->setScale(0.9f);
	m_pLiantongMSMItemSprite->setScale(0.9f);
	m_pDianxingMSMItemSprite->setScale(0.9f);
	m_pPhoneCardItemSprite->setScale(0.9f);
	m_pAlipayMSMItemSprite->setScale(0.9f);


	Menu* pTipsMenu = Menu::create(m_pZJDCardItemSprite,m_pYidongMSMItemSprite,m_pLiantongMSMItemSprite,m_pDianxingMSMItemSprite,m_pPhoneCardItemSprite,m_pAlipayMSMItemSprite,NULL);
	pTipsMenu->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.728));
	pTipsMenu->alignItemsHorizontallyWithPadding(1);
	this->addChild(pTipsMenu);

//	/**添加内容背景**/
//	Sprite *pContentBGLeft = Sprite::create("dt_getchange2.png");
//	Sprite *pContentBGRight = Sprite::create("dt_getchange2.png");
//	pContentBGRight->setRotationY(180);
//	pContentBGLeft->setPosition(Vec2(winSize.width*0.5 - pContentBGLeft->getContentSize().width * 0.5,winSize.height*0.415));
//	pContentBGRight->setPosition(Vec2(winSize.width*0.5 + pContentBGRight->getContentSize().width * 0.5,winSize.height*0.415));
//	this->addChild(pContentBGLeft,0);
//	this->addChild(pContentBGRight,0);
//
//    
//    Sprite* dogetNormal = (Sprite*)GroupSprite::GroupSpriteWith("dt_warning_8.png","dt_getchange7.png",GorupSpriteTypePhotoAndPhoto);
//    Sprite* dogetSelect = (Sprite*)GroupSprite::GroupSpriteWith("dt_warning_8.png","dt_getchange7.png",GorupSpriteTypePhotoAndPhoto);
//    Sprite* dogetdis = (Sprite*)GroupSprite::GroupSpriteWith("dt_warning_8.png","dt_getchange7.png",GorupSpriteTypePhotoAndPhoto);
//    MenuItemSprite* dogetItemSprite = MenuItemSprite::create(dogetNormal, dogetSelect,dogetdis, this, CC_CALLBACK_1(GetCharge::doget));
//    m_pDogetMenu = Menu::create(dogetItemSprite,NULL);
//    m_pDogetMenu->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.25));
//   this->addChild(m_pDogetMenu);
//    m_pDogetMenu->setVisible(false);
//
//    currentSelct = 4;
//    
//    long lGold = g_GlobalUnits.getGolbalUserData()->lGoldEggs;
//    
//
//    long lTemp = 0;
//    if (lGold >= 20000)
//    {
//        m_bEnoughGet = true;
//    }else
//    {
//        m_bEnoughGet = false;
//        lTemp = 20000 - lGold;
//    }
//	/**领话费**/
//	const char* str1 = String::createWithFormat("%s %s %s",pConfigMgr->text("t67"),addCommaToNumber(lGold).c_str(),pConfigMgr->text("t74"))->getCString();
//    m_pTips11 = LabelTTF::create(str1, "Helvetica-Bold", 24);
//    m_pTips11->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.62));
//   this->addChild(m_pTips11);
//    m_pTips11->setVisible(false);
//
//	m_pTips14 =LabelTTF::create(pConfigMgr->text("t169"), "Helvetica-Bold",14);
//	m_pTips14->setColor(ccc3(215,119,19));
//	m_pTips14->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.57));
//	this->addChild(m_pTips14);
//	m_pTips14->setVisible(false);
//
//    m_pTips12 = Sprite::createWithSpriteFrame(spriteFrame("dt_getchange3.png"));
//	m_pTips13 = Sprite::createWithSpriteFrame(spriteFrame("dt_getchange4.png"));
//    m_pTips12->setAnchorPoint(Vec2(1.0, 0.5));
//    m_pTips13->setAnchorPoint(Vec2(1.0, 0.5));
//    m_pTips12->setPosition(Vec2(winSize.width * 0.45, winSize.height * 0.50));
//    m_pTips13->setPosition(Vec2(winSize.width * 0.45, winSize.height * 0.38));
//   this->addChild(m_pTips12);
//   this->addChild(m_pTips13);
//    m_pTips12->setVisible(false);
//    m_pTips13->setVisible(false);
//    
//    m_pInputBox1 = EditBox::create(cocos2d::Size(260,48), Scale9Sprite::createWithSpriteFrame(spriteFrame("dt_login5.png")));
//    m_pInputBox2 = EditBox::create(cocos2d::Size(260,48), Scale9Sprite::createWithSpriteFrame(spriteFrame("dt_login5.png")));
//    m_pInputBox1->setPosition(Vec2(winSize.width * 0.62, winSize.height * 0.50));
//    m_pInputBox2->setPosition(Vec2(winSize.width * 0.62, winSize.height * 0.38));
//    this->addChild(m_pInputBox1);
//    this->addChild(m_pInputBox2);
//    m_pInputBox1->setVisible(false);
//    m_pInputBox2->setVisible(false);
//    
//    //领QB
//	const char* str2 = String::createWithFormat("%s %s %s",pConfigMgr->text("t67"),addCommaToNumber(lGold).c_str(),pConfigMgr->text("t75"))->getCString();
//    m_pTips21 = LabelTTF::create(str2, "Helvetica-Bold", 24);
//    m_pTips21->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.62));
//   this->addChild(m_pTips21);
//    m_pTips21->setVisible(false);
//
//
//	m_pTips24 =LabelTTF::create(pConfigMgr->text("t169"), "Helvetica-Bold",14);
//	m_pTips24->setColor(ccc3(215,119,19));
//	m_pTips24->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.57));
//	this->addChild(m_pTips24);
//	m_pTips24->setVisible(false);
//    
//	m_pTips22 = Sprite::createWithSpriteFrame(spriteFrame("dt_getchange0.png"));
//	m_pTips23 = Sprite::createWithSpriteFrame(spriteFrame("dt_getchange1.png"));
//    m_pTips22->setAnchorPoint(Vec2(1.0, 0.5));
//    m_pTips23->setAnchorPoint(Vec2(1.0, 0.5));
//    m_pTips22->setPosition(Vec2(winSize.width * 0.45, winSize.height * 0.50));
//    m_pTips23->setPosition(Vec2(winSize.width * 0.45, winSize.height * 0.38));
//   this->addChild(m_pTips22);
//   this->addChild(m_pTips23);
//    m_pTips22->setVisible(false);
//    m_pTips23->setVisible(false);
//    
//    //领金币
////    std::string str311 = "您拥有话费点";
////    str311.insert(str311.find("点"), addCommaToNumber(lGold).c_str());
//    const char*  str333 = String::createWithFormat
//		("%s %s %s %s %s %s",pConfigMgr->text("t78"),addCommaToNumber(lGold).c_str(),pConfigMgr->text("t68"),pConfigMgr->text("t76"),addCommaToNumber(lGold * 12).c_str(),pConfigMgr->text("t77"))->getCString();
//	m_pTips31 = LabelTTF::create(str333, "Helvetica-Bold", 24);	
//	
//    m_pTips31->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.52));
//   this->addChild(m_pTips31);
//    m_pTips31->setVisible(false);
//	m_pTips34 =LabelTTF::create(pConfigMgr->text("t169"), "Helvetica-Bold",14);
//	m_pTips34->setColor(ccc3(215,119,19));
//	m_pTips34->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.45));
//	this->addChild(m_pTips34);
//	m_pTips34->setVisible(false);
//    
//    //白赚话费
//    setTouchEnabled(true);
//
//	showLingHuaFei();
    return true;
}

ChongCoinsLayer::ChongCoinsLayer()
{
   
}

ChongCoinsLayer::~ChongCoinsLayer()
{
    
}


bool ChongCoinsLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    return true;
}

void ChongCoinsLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    
}

void ChongCoinsLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    
}

void ChongCoinsLayer::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    
}

void ChongCoinsLayer::closeChongCoinsLayer(Object *obj)
{
    removeFromParentAndCleanup(true);
}

void ChongCoinsLayer::showZJDCard(Object* obj)
{

}

void ChongCoinsLayer::showYidongSMS(Object* obj)
{

}

void ChongCoinsLayer::showLiantongSMS(Object* obj)
{

}

void ChongCoinsLayer::showDianxingSMS(Object* obj)
{

}

void ChongCoinsLayer::showPhoneCard(Object* obj)
{

}

void ChongCoinsLayer::showAliPay(Object* obj)
{

}
