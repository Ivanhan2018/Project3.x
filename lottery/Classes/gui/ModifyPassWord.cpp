#include "LoginLayer.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"
#include "Login.h"
#include "RoomLayer.h"
#include "AnsString.h"
#include "GBEvent.h"
#include "GBEventIDs.h"
#include "utf-8.h"
#include "ConfigMgr.h"
#include "GroupSprite.h"
#include "ModifyPassWord.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"
#include "FreeLayer.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace extension;

Scene* ModifyPassWord::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    ModifyPassWord *layer = ModifyPassWord::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

ModifyPassWord::ModifyPassWord()
{
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ModifyPassWord::onChangePassWordResult), MSG_UI_ANS_PASSWORDUPDATE, NULL);
}
ModifyPassWord::~ModifyPassWord()
{
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_PASSWORDUPDATE);

}



bool ModifyPassWord::init(bool bLogin)
{
   
    if ( !Layer::init() )
    {
        return false;
    }
    cocos2d::Size winSize = Director::getInstance()->getWinSize();

	//����
//	Sprite* spriteBack0 = Sprite::createWithSpriteFrame(spriteFrame("dt_register_12.jpg"));
//	Sprite* spriteBack1 = Sprite::createWithSpriteFrame(spriteFrame("dt_register_12.jpg"));
//	spriteBack0->setPosition(Vec2(spriteBack0->getContentSize().width * 0.5,winSize.height * 0.5));
//	spriteBack1->setPosition(Vec2(winSize.width - spriteBack1->getContentSize().width * 0.5-2,winSize.height * 0.5));
//	spriteBack1->setFlipX(true);
//	this->addChild(spriteBack0);
//	this->addChild(spriteBack1);

    
    Sprite *bk = Sprite::create("Bk_LoadView.png");
    bk->setPosition(Vec2(400, 240));
    this->addChild(bk,0);
	//����
	Sprite* backNormal= Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
	Sprite* backSelect= Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
	MenuItemSprite* backItemSprite = MenuItemSprite::create(backNormal,backSelect,CC_CALLBACK_1(ModifyPassWord::rBack,this));
	backSelect->setScale(0.9f);
	backSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
	Menu* backMenu= Menu::create(backItemSprite,NULL);
	backMenu->setPosition(Vec2(winSize.width * 0.93,winSize.height * 0.93));
	this->addChild(backMenu);

	//����
//	Sprite* spriteTitleBack = Sprite::createWithSpriteFrame(spriteFrame("dt_register_14.png"));
//	Sprite* spriteTitle = Sprite::createWithSpriteFrame(spriteFrame("dt_login6.png"));
//	spriteTitleBack->setPosition(Vec2(winSize.width * 0.5,winSize.height * 0.92));
//	spriteTitle->setPosition(Vec2(winSize.width * 0.5,winSize.height * 0.93));
//	this->addChild(spriteTitleBack);
//	this->addChild(spriteTitle);

	//�׿�
//	Sprite* spriteBackBox0 = Sprite::createWithSpriteFrame(spriteFrame("dt_email8.jpg")); 
//	Sprite* spriteBackBox1 = Sprite::createWithSpriteFrame(spriteFrame("dt_email8.jpg"));
//	spriteBackBox0->setPosition(Vec2(winSize.width * 0.5 - spriteBackBox0->getContentSize().width * 0.5,spriteBackBox0->getContentSize().height * 0.57));
//	spriteBackBox1->setPosition(Vec2(winSize.width * 0.5 + spriteBackBox1->getContentSize().width * 0.5-2,spriteBackBox1->getContentSize().height * 0.57));
//	spriteBackBox0->setScaleY(1.12f);
//	spriteBackBox1->setScaleY(1.12f);
//	spriteBackBox1->setFlipX(true);
//	this->addChild(spriteBackBox0);
//	this->addChild(spriteBackBox1);

	//���
//	Sprite *pMianFeiSpriteNormal = Sprite::createWithSpriteFrame(spriteFrame("dt_playerinfo_0.png"));
//	Sprite *pMianFeiSpriteSelect = Sprite::createWithSpriteFrame(spriteFrame("dt_playerinfo_0.png"));
//	MenuItemSprite* pMianfei = MenuItemSprite::create(pMianFeiSpriteNormal,pMianFeiSpriteSelect,CC_CALLBACK_1(ModifyPassWord::toFreeLayer));
//	pMianFeiSpriteSelect->setScale(0.9f);
//	pMianFeiSpriteSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
//	Menu* mianfeiMenu = Menu::create(pMianfei,NULL);
//	mianfeiMenu->setPosition(Vec2(winSize.width*0.054,winSize.height*0.885));
//	this->addChild(mianfeiMenu);
	
	//��½�׿�
	Sprite* loginBackBox = Sprite::create("Bk_Login.png");
	loginBackBox->setPosition(Vec2(winSize.width * 0.5,winSize.height * 0.45));
    loginBackBox->setScaleX(1.4f);
    loginBackBox->setScaleY(1.2f);
	this->addChild(loginBackBox);
    
    loginPass = MenuItemImage::create("register_bt_dotbg-hd.png", "register_bt_dot-hd.png", CC_CALLBACK_1(ModifyPassWord::pressLoginPass,this));
    bankPass = MenuItemImage::create("register_bt_dotbg-hd.png", "register_bt_dot-hd.png", CC_CALLBACK_1(ModifyPassWord::pressBankPass,this));
    Menu *selectPassMenu = Menu::create(loginPass, bankPass, NULL);
    selectPassMenu->alignItemsHorizontallyWithPadding(160);
    selectPassMenu->setPosition(Vec2(winSize.width * 0.54,winSize.height * 0.7));
    loginPass->selected();
    this->addChild(selectPassMenu);
    
    LabelTTF* loginPassLabel = LabelTTF::create(ConfigMgr::instance()->text("t1030"),"",24);
    LabelTTF* bankPassLabel = LabelTTF::create(ConfigMgr::instance()->text("t1031"),"",24);
    loginPassLabel->setPosition(Vec2(winSize.width * 0.3,winSize.height * 0.7));
    bankPassLabel->setPosition(Vec2(winSize.width * 0.55,winSize.height * 0.7));
    loginPassLabel->setColor(ccc3(0,0,0));
    bankPassLabel->setColor(ccc3(0,0,0));
   this->addChild(loginPassLabel);
   this->addChild(bankPassLabel);
	//tips
	LabelTTF* tipsLabel0 = LabelTTF::create(ConfigMgr::instance()->text("t158"),"",24);
	LabelTTF* tipsLabel1 = LabelTTF::create(ConfigMgr::instance()->text("t159"),"",24);
	LabelTTF* tipsLabel2 = LabelTTF::create(ConfigMgr::instance()->text("t160"),"",24);
	tipsLabel0->setPosition(Vec2(winSize.width * 0.3,winSize.height * 0.6));
	tipsLabel1->setPosition(Vec2(winSize.width * 0.3,winSize.height * 0.5));
	tipsLabel2->setPosition(Vec2(winSize.width * 0.3,winSize.height * 0.4));
	tipsLabel0->setColor(ccc3(0,0,0));
	tipsLabel1->setColor(ccc3(0,0,0));
	tipsLabel2->setColor(ccc3(0,0,0));
	this->addChild(tipsLabel0);
	this->addChild(tipsLabel1);
   this->addChild(tipsLabel2);
    
    LabelTTF* tipsLabel3 = LabelTTF::create(ConfigMgr::instance()->text("t1032"),"",20);
    tipsLabel3->setPosition(Vec2(winSize.width * 0.5,winSize.height * 0.32));
    tipsLabel3->setColor(ccc3(200,0,0));
   this->addChild(tipsLabel3);


    //�����
    orginPasswordInput = EditBox::create(cocos2d::Size(268,40), Scale9Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyInputBk.png")));
    modifyPasswordInput = EditBox::create(cocos2d::Size(268,40), Scale9Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyInputBk.png")));
	surePasswordInput = EditBox::create(cocos2d::Size(268,40), Scale9Sprite::createWithSpriteFrame(spriteFrame("Sprite_ShopBuyInputBk.png")));
    orginPasswordInput->setPosition(Vec2(winSize.width * 0.55, winSize.height * 0.6));
    modifyPasswordInput->setPosition(Vec2(winSize.width * 0.55, winSize.height * 0.5));
	surePasswordInput->setPosition(Vec2(winSize.width * 0.55, winSize.height * 0.4));
    orginPasswordInput->setPlaceHolder(ConfigMgr::instance()->text("t4"));
    modifyPasswordInput->setPlaceHolder(ConfigMgr::instance()->text("t4"));
	surePasswordInput->setPlaceHolder(ConfigMgr::instance()->text("t4"));
	orginPasswordInput->setPlaceholderFont("",25);
	modifyPasswordInput->setPlaceholderFont("",25);
	surePasswordInput->setPlaceholderFont("",25);
    orginPasswordInput->setFontColor(ccc3(0,0,0));
    modifyPasswordInput->setFontColor(ccc3(0,0,0));
    surePasswordInput->setFontColor(ccc3(0,0,0));
    orginPasswordInput->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    modifyPasswordInput->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    surePasswordInput->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
   this->addChild(orginPasswordInput);
   this->addChild(modifyPasswordInput);
	this->addChild(surePasswordInput);
	if(g_GlobalUnits.getGolbalUserData()->cbVisitor != 0)
	{
		std::string userPassword = UserDefault::getInstance()->getStringForKey("NEWZJD_PASSWORD");
		orginPasswordInput->setText(userPassword.c_str());
	}

    
    //��½//��ť
    Sprite * modifyNormalSprite = (Sprite*)GroupSprite::GroupSpriteWith("dt_login13.png","dt_login3.png",GorupSpriteTypePhotoAndPhoto);
    Sprite * modifySelectSprite = (Sprite*)GroupSprite::GroupSpriteWith("dt_login13.png","dt_login3.png",GorupSpriteTypePhotoAndPhoto);
    MenuItemSprite * modify = MenuItemSprite::create(modifyNormalSprite, modifySelectSprite,  CC_CALLBACK_1(ModifyPassWord::clickButton,this));
    Menu* m_pmodifyMenu = Menu::create(modify,NULL);
	modifySelectSprite->setScale(0.9f);
	modifySelectSprite->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
    m_pmodifyMenu->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.21));
   this->addChild(m_pmodifyMenu);
    
	this->setKeypadEnabled(true);

    return true;
}

void ModifyPassWord::pressLoginPass(Object *obj)
{
    loginPass->selected();
    bankPass->unselected();
}

void ModifyPassWord::pressBankPass(Object *obj)
{
    loginPass->unselected();
    bankPass->selected();
}

void ModifyPassWord::clickButton(Object* obj)
{
	playButtonSound();
	const char* szNewPassword = modifyPasswordInput->getText();
	const char* szSurePassword = surePasswordInput->getText();
	const char* szOldPassword = orginPasswordInput->getText();
	if (strcmp(szNewPassword,szSurePassword))
	{
		//�������벻һ��
		PromptBox * promptBox = PromptBox::PromptBoxWith(Vec2(427, 240),mPromptTypeSeverPasswordFaild);
		this->addChild(promptBox);
		return;
	}

	if (!isLegal(szNewPassword))
	{
		//���뺬�зǷ��ַ�
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("t457"),Vec2(427,240));
		this->addChild(layer,20);
		return;
	}

	if(strlen(szNewPassword) > 12 || strlen(szNewPassword) < 6)
	{
		//���볤�Ȳ���
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("t457"),Vec2(427,240));
		this->addChild(layer,20);
		return;
	}
    bool isLoginPass = false;
    if (loginPass->isSelected()) {
        isLoginPass = true;
    }
	bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
	if(bRet)
	{
		EntityMgr::instance()->getDispatch()->modifyPassWord(szOldPassword,szNewPassword, isLoginPass);
	}
}

void ModifyPassWord::rBack(Object* obj)
{
	playButtonSound();
	Director::getInstance()->replaceScene(LoginLayer::scene());
}

void ModifyPassWord::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		rBack(NULL);
	}
}

void ModifyPassWord::toFreeLayer(Object *obj){
	playButtonSound();
	Director::getInstance()->replaceScene(FreeLayer::scene());
}

void ModifyPassWord::onChangePassWordResult(Object* obj)
{
	Integer* pNickNameResult = (Integer*)obj;
	int i = pNickNameResult->getValue();
	if(i == 0)
	{
		CCLOG("成功修改密码");
		std::string szNewPassword = modifyPasswordInput->getText();
		UserDefault::getInstance()->setStringForKey("NEWZJD_PASSWORD", szNewPassword);
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("t167"),Vec2(427,240));
		this->addChild(layer);
		Scene *pScene = Scene::create();
		RoomLayer *pRoomLayer = RoomLayer::create();
		pRoomLayer->automaticLogin(false);
		pRoomLayer->closeLoginAward();
		pScene->addChild(pRoomLayer);
		TransitionFade *tScene = TransitionFade::create(2.0f, pScene, Color3B::WHITE);
		Director::getInstance()->replaceScene(tScene); 
		//�ɹ�
	}
	else
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("t168"),Vec2(427,240));
		this->addChild(layer);
		//ʧ��
	}
}

bool ModifyPassWord::isLegal(string str)
{
	const char* s = str.c_str();
	int i = 0;
	while (i++ < str.length() - 1)
	{
		int a = s[i];
		if (s[i]>255)
		{
			//("中文");
		}
		if (s[i] < 0)
		{
			//("中文");
		}
		else if (s[i]>='0' && s[i]<='9')
		{
			//("数字");
		}
		else if (s[i]>='A' && s[i]<='Z' || s[i]>='a' && s[i]<='z')
		{
			//Memo1->Lines->Add("字母");
		}
		else
		{
			//("其它");
			return  false;
		}
	}
	return  true;
}