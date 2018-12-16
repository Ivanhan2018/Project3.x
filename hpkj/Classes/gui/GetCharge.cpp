//
//  GetCharge.cpp
//  Game
//
//  Created by 罗敏 on 13-8-6.
//
//

#include "GetCharge.h"
#include <limits.h>
#include "EntityMgr.h"
#include "ConfigMgr.h"
#include "GroupSprite.h"
#include "PromptBox.h"
#include "RoomLayer.h"
using namespace CocosDenshion;
using namespace extension;

GetCharge * GetCharge::GetChargeWith(Vec2 pos)
{
    GetCharge * charge = new GetCharge();
    if(charge && charge->initGetCharge(pos))
    {
        charge->autorelease();
        return charge;
    }
    CC_SAFE_DELETE(charge);
    return charge;
}

bool GetCharge::initGetCharge(Vec2 pos)
{
    if(!Layer::init())
    {
        return false;
    }

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GetCharge::onTouchBegan, this);
	listener->onTouchCancelled = CC_CALLBACK_2(GetCharge::onTouchCancelled, this);
	listener->onTouchEnded = CC_CALLBACK_2(GetCharge::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(GetCharge::onTouchMoved, this) ;

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    cocos2d::Size winSize = WINSIZE;
	LayerColor* layer = LayerColor::create(ccc4(0, 0, 0, 255 * 0.5f), 854.0f, SCREEN_WIDTH);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	Sprite *pBackLeft =  Sprite::createWithSpriteFrame(spriteFrame("dt_warning_3.png"));
	Sprite *pBackRight = Sprite::createWithSpriteFrame(spriteFrame("dt_warning_3.png"));
	pBackLeft->setScaleY(1.1f);
	pBackRight->setScaleY(1.1f);
	pBackRight->setRotationY(180);
	pBackLeft->setPosition(Vec2(winSize.width*0.5 - pBackLeft->getContentSize().width * 0.5,winSize.height*0.5));
	pBackRight->setPosition(Vec2(winSize.width*0.5 + pBackRight->getContentSize().width * 0.5 - 1,winSize.height*0.5));
	this->addChild(pBackLeft,0);
	this->addChild(pBackRight,0);




    Sprite * backNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_kefu_0.png"));
    Sprite * backSelect = Sprite::createWithSpriteFrame(spriteFrame("dt_kefu_0.png"));
    MenuItemSprite * backItemSprite = MenuItemSprite::create(backNormal, backSelect,  CC_CALLBACK_1(GetCharge::closeGetCharge,this));
	backSelect->setScale(0.9f);
	backSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
    m_pBackMenu = Menu::create(backItemSprite,NULL);
	
    m_pBackMenu->setPosition(Vec2(winSize.width*0.928,winSize.height*0.868));
   this->addChild(m_pBackMenu);

	
    
    Sprite* getchargeNormal = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange5.png","dt_getchange10.png",GorupSpriteTypePhotoAndPhoto);
    Sprite* getchargeSelect = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange6.png","dt_getchange11.png",GorupSpriteTypePhotoAndPhoto);
    Sprite* getchargeDis = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange6.png","dt_getchange11.png",GorupSpriteTypePhotoAndPhoto);
    
	Sprite* getcoinsNormal = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange5.png","dt_getchange12.png",GorupSpriteTypePhotoAndPhoto);
    Sprite* getcoinsSelect = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange6.png","dt_getchange13.png",GorupSpriteTypePhotoAndPhoto);
    Sprite* getcoinsDis = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange6.png","dt_getchange13.png",GorupSpriteTypePhotoAndPhoto);
   
	Sprite* getQBNormal = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange5.png","dt_getchange8.png",GorupSpriteTypePhotoAndPhoto);
    Sprite* getQBSelect =(Sprite*)GroupSprite::GroupSpriteWith("dt_getchange6.png","dt_getchange9.png",GorupSpriteTypePhotoAndPhoto);
    Sprite* getQBdis = (Sprite*)GroupSprite::GroupSpriteWith("dt_getchange6.png","dt_getchange9.png",GorupSpriteTypePhotoAndPhoto);
    
    getchargeItemSprite = MenuItemSprite::create(getchargeNormal, getchargeSelect, getchargeDis,CC_CALLBACK_1(GetCharge::getCharge,this));
    getcoinsItemSprite = MenuItemSprite::create(getcoinsNormal, getcoinsSelect, getcoinsDis,CC_CALLBACK_1(GetCharge::getCoins,this));
    getQBItemSprite = MenuItemSprite::create(getQBNormal, getQBSelect, getQBdis,CC_CALLBACK_1(GetCharge::getQB,this));
    m_pGetMenu = Menu::create(getchargeItemSprite,getQBItemSprite,getcoinsItemSprite,NULL);
    m_pGetMenu->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.748));
    m_pGetMenu->alignItemsHorizontallyWithPadding(20);
   this->addChild(m_pGetMenu);


	/**添加内容背景**/
	Sprite *pContentBGLeft =Sprite::createWithSpriteFrame(spriteFrame("dt_getchange2.png")); 
	pContentBGLeft->setScaleY(1.1f);
	Sprite *pContentBGRight =Sprite::createWithSpriteFrame(spriteFrame("dt_getchange2.png")); 
	pContentBGRight->setScaleY(1.1f);
	pContentBGRight->setRotationY(180);
	pContentBGLeft->setPosition(Vec2(winSize.width*0.5 - pContentBGLeft->getContentSize().width * 0.5,winSize.height*0.425));
	pContentBGRight->setPosition(Vec2(winSize.width*0.5 + pContentBGRight->getContentSize().width * 0.5,winSize.height*0.425));
	this->addChild(pContentBGLeft,0);
	this->addChild(pContentBGRight,0);

    
    Sprite* dogetNormal = (Sprite*)GroupSprite::GroupSpriteWith("dt_warning_8.png","dt_getchange7.png",GorupSpriteTypePhotoAndPhoto);
    Sprite* dogetSelect = (Sprite*)GroupSprite::GroupSpriteWith("dt_warning_8.png","dt_getchange7.png",GorupSpriteTypePhotoAndPhoto);
    Sprite* dogetdis = (Sprite*)GroupSprite::GroupSpriteWith("dt_warning_8.png","dt_getchange7.png",GorupSpriteTypePhotoAndPhoto);
    MenuItemSprite* dogetItemSprite = MenuItemSprite::create(dogetNormal, dogetSelect,dogetdis, CC_CALLBACK_1(GetCharge::doget,this));
    m_pDogetMenu = Menu::create(dogetItemSprite,NULL);
	dogetSelect->setScale(0.9f);
	dogetSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
    m_pDogetMenu->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.27));
   this->addChild(m_pDogetMenu);
    m_pDogetMenu->setVisible(false);

    currentSelct = 4;
    
    long lGold = g_GlobalUnits.getGolbalUserData()->lGoldEggs;
    

    long lTemp = 0;
    if (lGold >= 15000)
    {
        m_bEnoughGet = true;
    }else
    {
        m_bEnoughGet = false;
        lTemp = 15000 - lGold;
    }
	/**领话费**/
	const char* str1 = String::createWithFormat("%s%s%s",pConfigMgr->text("t67"),addCommaToNumber(lGold).c_str(),pConfigMgr->text("t74"))->getCString();
    m_pTips11 = LabelTTF::create(str1, "Helvetica-Bold", 24);
    m_pTips11->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.64));
   this->addChild(m_pTips11);
    m_pTips11->setVisible(false);

	m_pTips14 =LabelTTF::create(pConfigMgr->text("t169"), "Helvetica-Bold",14);
	m_pTips14->setColor(ccc3(215,119,19));
	m_pTips14->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.59));
	this->addChild(m_pTips14);
	m_pTips14->setVisible(false);

    m_pTips12 = Sprite::createWithSpriteFrame(spriteFrame("dt_getchange3.png"));
	m_pTips13 = Sprite::createWithSpriteFrame(spriteFrame("dt_getchange4.png"));
    m_pTips12->setAnchorPoint(Vec2(1.0, 0.5));
    m_pTips13->setAnchorPoint(Vec2(1.0, 0.5));
    m_pTips12->setPosition(Vec2(winSize.width * 0.45, winSize.height * 0.52));
    m_pTips13->setPosition(Vec2(winSize.width * 0.45, winSize.height * 0.4));
   this->addChild(m_pTips12);
   this->addChild(m_pTips13);
    m_pTips12->setVisible(false);
    m_pTips13->setVisible(false);
    
    m_pInputBox1 = EditBox::create(cocos2d::Size(230, 40), Scale9Sprite::createWithSpriteFrame(spriteFrame("dt_login5.png")));
    m_pInputBox2 = EditBox::create(cocos2d::Size(230, 40), Scale9Sprite::createWithSpriteFrame(spriteFrame("dt_login5.png")));
    m_pInputBox1->setPosition(Vec2(winSize.width * 0.62, winSize.height * 0.52));
    m_pInputBox2->setPosition(Vec2(winSize.width * 0.62, winSize.height * 0.4));
   this->addChild(m_pInputBox1);
   this->addChild(m_pInputBox2);
    m_pInputBox1->setVisible(false);
    m_pInputBox2->setVisible(false);
    
    //领QB
	const char* str2 = String::createWithFormat("%s%s%s",pConfigMgr->text("t67"),addCommaToNumber(lGold).c_str(),pConfigMgr->text("t75"))->getCString();
    m_pTips21 = LabelTTF::create(str2, "Helvetica-Bold", 24);
    m_pTips21->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.64));
   this->addChild(m_pTips21);
    m_pTips21->setVisible(false);


	m_pTips24 =LabelTTF::create(pConfigMgr->text("t169"), "Helvetica-Bold",14);
	m_pTips24->setColor(ccc3(215,119,19));
	m_pTips24->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.59));
	this->addChild(m_pTips24);
	m_pTips24->setVisible(false);
    
	m_pTips22 = Sprite::createWithSpriteFrame(spriteFrame("dt_getchange0.png"));
	m_pTips23 = Sprite::createWithSpriteFrame(spriteFrame("dt_getchange1.png"));
    m_pTips22->setAnchorPoint(Vec2(1.0, 0.5));
    m_pTips23->setAnchorPoint(Vec2(1.0, 0.5));
    m_pTips22->setPosition(Vec2(winSize.width * 0.45, winSize.height * 0.52));
    m_pTips23->setPosition(Vec2(winSize.width * 0.45, winSize.height * 0.4));
   this->addChild(m_pTips22);
   this->addChild(m_pTips23);
    m_pTips22->setVisible(false);
    m_pTips23->setVisible(false);
    
    //领金币
//    std::string str311 = "您拥有话费点";
//    str311.insert(str311.find("点"), addCommaToNumber(lGold).c_str());
    const char*  str333 = String::createWithFormat
		("%s%s%s%s%s%s",pConfigMgr->text("t78"),addCommaToNumber(lGold).c_str(),pConfigMgr->text("t68"),pConfigMgr->text("t76"),addCommaToNumber(((lGold - 200)>0?(lGold-200):0) * 10).c_str(),pConfigMgr->text("t77"))->getCString();
	m_pTips31 = LabelTTF::create(str333, "Helvetica-Bold", 24);	
	
    m_pTips31->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.54));
   this->addChild(m_pTips31);
    m_pTips31->setVisible(false);
	m_pTips34 =LabelTTF::create(pConfigMgr->text("t169"), "Helvetica-Bold",14);
	m_pTips34->setColor(ccc3(215,119,19));
	m_pTips34->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.47));
	this->addChild(m_pTips34);
	m_pTips34->setVisible(false);

	LabelTTF* m_ElseLabel = LabelTTF::create(pConfigMgr->text("t480"),"",15);
	m_ElseLabel->setPosition(Vec2(winSize.width * 0.5,winSize.height * 0.18));
	m_ElseLabel->setColor(ccc3(215,119,19));
	this->addChild(m_ElseLabel);
    
    //白赚话费
    //setTouchEnabled(true);
	this->setKeypadEnabled(true);
	showLingHuaFei();
    return true;
}

GetCharge::GetCharge()
{
   
}

GetCharge::~GetCharge()
{
    
}

void GetCharge::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		closeGetCharge(NULL);
	}
}

void GetCharge::showLingHuaFei(){
	playButtonSound();
	getchargeItemSprite->setEnabled(false);
	getcoinsItemSprite->setEnabled(true);
	getQBItemSprite->setEnabled(true);
	currentSelct = 0;


	m_pTips11->setVisible(true);

	m_pTips12->setVisible(true);
	m_pTips13->setVisible(true);

	m_pInputBox1->setVisible(true);
	m_pInputBox2->setVisible(true);
	m_pInputBox1->setPlaceHolder(pConfigMgr->text("t304"));
	m_pInputBox2->setPlaceHolder(pConfigMgr->text("t305"));


	m_pTips21->setVisible(false);

	m_pTips22->setVisible(false);
	m_pTips23->setVisible(false);


	m_pTips31->setVisible(false);
	// m_pTips32->setVisible(false);

	m_pDogetMenu->setVisible(true);

	m_pTips14->setVisible(true);
	m_pTips24->setVisible(false);
	m_pTips34->setVisible(false);
}


void GetCharge::getCharge(Object *obj)
{
	playButtonSound();
    playButtonSound();
    getchargeItemSprite->setEnabled(false);
    getcoinsItemSprite->setEnabled(true);
    getQBItemSprite->setEnabled(true);
    currentSelct = 0;
    
    
    m_pTips11->setVisible(true);

    m_pTips12->setVisible(true);
    m_pTips13->setVisible(true);

    m_pInputBox1->setVisible(true);
    m_pInputBox2->setVisible(true);
	m_pInputBox1->setText("");
	m_pInputBox2->setText("");
    m_pInputBox1->setPlaceHolder(pConfigMgr->text("t304"));
    m_pInputBox2->setPlaceHolder(pConfigMgr->text("t305"));
    
    
    m_pTips21->setVisible(false);

    m_pTips22->setVisible(false);
    m_pTips23->setVisible(false);

    
    m_pTips31->setVisible(false);
   // m_pTips32->setVisible(false);

    m_pDogetMenu->setVisible(true);

	m_pTips14->setVisible(true);
	m_pTips24->setVisible(false);
	m_pTips34->setVisible(false);
}

void GetCharge::getCoins(Object *obj)
{
    playButtonSound();
    getchargeItemSprite->setEnabled(true);
    getcoinsItemSprite->setEnabled(false);
    getQBItemSprite->setEnabled(true);
    currentSelct = 1;
    
    
    m_pTips11->setVisible(false);

    m_pTips12->setVisible(false);
    m_pTips13->setVisible(false);
    
    
    
    m_pTips21->setVisible(false);

    m_pTips22->setVisible(false);
    m_pTips23->setVisible(false);

    m_pInputBox1->setVisible(false);
    m_pInputBox2->setVisible(false);
    
    m_pTips31->setVisible(true);
   // m_pTips32->setVisible(true);

    m_pDogetMenu->setVisible(true);

	m_pTips14->setVisible(false);
	m_pTips24->setVisible(false);
	m_pTips34->setVisible(true);
}

void GetCharge::getQB(Object *obj)
{
    playButtonSound();
    getchargeItemSprite->setEnabled(true);
    getcoinsItemSprite->setEnabled(true);
    getQBItemSprite->setEnabled(false);
    currentSelct = 2;
    
    
    m_pTips11->setVisible(false);

    m_pTips12->setVisible(false);
    m_pTips13->setVisible(false);

    
    m_pTips21->setVisible(true);

    m_pTips22->setVisible(true);
    m_pTips23->setVisible(true);

    m_pInputBox1->setVisible(true);
    m_pInputBox2->setVisible(true);
	m_pInputBox1->setText("");
	m_pInputBox2->setText("");
    m_pInputBox1->setPlaceHolder(pConfigMgr->text("t302"));
    m_pInputBox2->setPlaceHolder(pConfigMgr->text("t303"));
    
    m_pTips31->setVisible(false);
   // m_pTips32->setVisible(false);

    m_pDogetMenu->setVisible(true);

	m_pTips14->setVisible(false);
	m_pTips24->setVisible(true);
	m_pTips34->setVisible(false);
}

bool GetCharge::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    m_bBackMenu = m_pBackMenu->onTouchBegan(pTouch, pEvent);
    m_bGetMenu = m_pGetMenu->onTouchBegan(pTouch, pEvent);
    if(currentSelct == 0 || currentSelct == 2)
    {
        m_bInputBox1 = m_pInputBox1->onTouchBegan(pTouch, pEvent);
        m_bInputBox2 = m_pInputBox2->onTouchBegan(pTouch, pEvent);
    }
    m_bDogetMenu = m_pDogetMenu->onTouchBegan(pTouch, pEvent);
    return true;
}

void GetCharge::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    if(m_bBackMenu)
    {
        m_pBackMenu->onTouchMoved(pTouch, pEvent);
    }
    if(m_bGetMenu)
    {
        m_pGetMenu->onTouchMoved(pTouch, pEvent);
    }
    if(m_bInputBox1)
    {
        m_pInputBox1->onTouchMoved(pTouch, pEvent);
    }
    if(m_bInputBox2)
    {
        m_pInputBox2->onTouchMoved(pTouch, pEvent);
    }
    if(m_bDogetMenu)
    {
        m_pDogetMenu->onTouchMoved(pTouch, pEvent);
    }
}

void GetCharge::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if(m_bBackMenu)
    {
        m_pBackMenu->onTouchEnded(pTouch, pEvent);
    }
    if(m_bGetMenu)
    {
        m_pGetMenu->onTouchEnded(pTouch, pEvent);
    }
    if(m_bInputBox1)
    {
        m_pInputBox1->onTouchEnded(pTouch, pEvent);
    }
    if(m_bInputBox2)
    {
        m_pInputBox2->onTouchEnded(pTouch, pEvent);
    }
    if(m_bDogetMenu)
    {
        m_pDogetMenu->onTouchEnded(pTouch, pEvent);
    }
}

void GetCharge::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    if(m_bBackMenu)
    {
        m_pBackMenu->onTouchCancelled(pTouch, pEvent);
    }
    if(m_bGetMenu)
    {
        m_pGetMenu->onTouchCancelled(pTouch, pEvent);
    }
    if(m_bInputBox1)
    {
        m_pInputBox1->onTouchCancelled(pTouch, pEvent);
    }
    if(m_bInputBox2)
    {
        m_pInputBox2->onTouchCancelled(pTouch, pEvent);
    }
    if(m_bDogetMenu)
    {
        m_pDogetMenu->onTouchCancelled(pTouch, pEvent);
    }
}

void GetCharge::closeGetCharge(Object *obj)
{
	playButtonSound();
	RoomLayer * pRoomLayer = (RoomLayer * )this->getParent();
    pRoomLayer->permitButtonClick();
    playButtonSound();
    removeFromParentAndCleanup(true);
	
}

void GetCharge::doget(Object* obj)
{
    playButtonSound();
    //建立连接
    bool bRet = pMsgDispatch->connectLoginServer();
    cocos2d::Size winSize = WINSIZE;
    if (!bRet)
    {
		PromptBox * promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5, winSize.height * 0.5),mPromptTypeServerShut);
		this->addChild(promptBox);
		CCLOG("login connect faild!");
        return;
    }


    if(currentSelct != 1)
	{
		const char* str = m_pInputBox1->getText();
		const char* str1 = m_pInputBox2->getText();
		int resultCmp = strcmp(str, str1);
		if(currentSelct == 0)
		{
			if(strlen(str) != 11 || !isLegal(str))
			{
				PromptBox * promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5, winSize.height * 0.5),mPromptTypeErrorPhone);
				this->addChild(promptBox);
				CCLOG("erroe phone");
				return;
			}

			if(resultCmp != 0)
			{
				PromptBox * promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5, winSize.height * 0.5),mPromptTypeTwoInputError);
				this->addChild(promptBox);
				CCLOG("input error");
				return;
			}
		}
		if(currentSelct == 2)
		{
			if(strlen(str) <= 3 || strlen(str) > 11 || !isLegal(str))
			{
				PromptBox * promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5, winSize.height * 0.5),mPromptTypeErrorQQNumber);
				this->addChild(promptBox);
				CCLOG("error qq number");
				return;
			}
			if(resultCmp != 0)
			{
				PromptBox * promptBox = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5, winSize.height * 0.5),mPromptTypeTwoInputError);
				this->addChild(promptBox);
				return;
			}
			}
		if(!m_bEnoughGet)
		{
			PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5, winSize.height * 0.5),mPromptTypeGetChargeFaild);
			this->addChild(box);
			CCLOG("get charge faild!");
			long lGold = g_GlobalUnits.getGolbalUserData()->lGoldEggs;

			const char* str22 = String::createWithFormat("%s %s %s",pConfigMgr->text("t79"),addCommaToNumber(15000-lGold).c_str(),pConfigMgr->text("t80"))->getCString();
			const char* str33 = String::createWithFormat("%s %s %s",pConfigMgr->text("t79"),addCommaToNumber(15000-lGold).c_str(),pConfigMgr->text("t81"))->getCString();
			if(currentSelct == 0)
			{
				box->setPromptText(str22);
			}
			else if(currentSelct == 2)
			{
				box->setPromptText(str33);
			}
			return;
		}
		pMsgDispatch->sendChargePacket(currentSelct,str);
		RoomLayer * pRoomLayer = (RoomLayer * )this->getParent();
		pRoomLayer->permitButtonClick();
		removeFromParentAndCleanup(true);
	}
	else
	{
		pMsgDispatch->sendChargePacket(currentSelct,"000");
		RoomLayer * pRoomLayer = (RoomLayer * )this->getParent();
		pRoomLayer->permitButtonClick();
		removeFromParentAndCleanup(true);
	}
}

bool GetCharge::isLegal(string str)
{
    const char* s = str.c_str();
    int i = 0;
    while (i++ < str.length() - 1)
    {
        int a = s[i];
        if (s[i]>255)
        {
            //("中文");
            return false;
        }
        if (s[i] < 0)
        {
            //("中文");
            return false;
        }
        else if (s[i]>='0' && s[i]<='9')
        {
            //("数字");
        }
        else if (s[i]>='A' && s[i]<='Z' || s[i]>='a' && s[i]<='z')
        {
            //Memo1->Lines->Add("字母");
            return false;
        }
        else
        {
            //("其它");
            return  false;
        }
    }
    return  true;
}
