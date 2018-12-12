#include "OverRegistLayer.h"
#include "RoomLayer.h"
#include "GroupSprite.h"
#include "MovingLabelLayer.h"
#include "LoginLayer.h"
#include "PromptBox.h"
#include "utf-8.h"
#include "RoomOption.h"


using namespace cocos2d;
OverRegistLayer::OverRegistLayer()
{
	NotificationCenter::getInstance()->addObserver(
		this, callfuncO_selector(OverRegistLayer::onOverRes), MSG_UI_ANS_OVERREGISTRES, NULL);
}
OverRegistLayer::~OverRegistLayer()
{
	NotificationCenter::getInstance()->removeObserver(this,MSG_UI_ANS_OVERREGISTRES);
}


bool OverRegistLayer::init()
{
	bool bRet = false;
	do 
	{
		winSize = Director::getInstance()->getWinSize();
		Sprite *pRegistBJLeft = Sprite::createWithSpriteFrame(spriteFrame("dt_register_12.jpg"));
		Sprite *pRegistBJRight = Sprite::createWithSpriteFrame(spriteFrame("dt_register_12.jpg"));
		pRegistBJRight->setRotationY(180);
		pRegistBJLeft->setPosition(Vec2(winSize.width*0.25,winSize.height*0.5));
		pRegistBJRight->setPosition(Vec2(winSize.width*0.75,winSize.height*0.5));
		this->addChild(pRegistBJLeft,0);
		this->addChild(pRegistBJRight,0);

		Sprite *pPlayerInfoBJLeft1 = Sprite::createWithSpriteFrame(spriteFrame("dt_email8.jpg"));
		Sprite *pPlayerInfoBJRight1 = Sprite::createWithSpriteFrame(spriteFrame("dt_email8.jpg"));
		pPlayerInfoBJRight1->setRotationY(180);
		pPlayerInfoBJLeft1->setPosition(Vec2(winSize.width*0.25+5,winSize.height*0.0-18));
		pPlayerInfoBJRight1->setPosition(Vec2(winSize.width*0.75-7,winSize.height*0.0-18));
		pPlayerInfoBJLeft1->setScaleY(1.2f);
		pPlayerInfoBJRight1->setScaleY(1.2f);
		pPlayerInfoBJLeft1->setAnchorPoint(Vec2(0.5,0));
		pPlayerInfoBJRight1->setAnchorPoint(Vec2(0.5,0));
		this->addChild(pPlayerInfoBJLeft1,1);
		this->addChild(pPlayerInfoBJRight1,1);


		Scale9Sprite *pDituLeft1 = Scale9Sprite::createWithSpriteFrame(spriteFrame("dt_playerinfo_5.png"));
		pDituLeft1->setContentSize(cocos2d::Size(198.0f,149.0f));
		pDituLeft1->setPosition(Vec2(340.0f,284.0f));
		this->addChild(pDituLeft1,1);
		Scale9Sprite *pDituRight1 = Scale9Sprite::createWithSpriteFrame(spriteFrame("dt_playerinfo_5.png"));
		pDituRight1->setRotationY(180);
		pDituRight1->setContentSize(cocos2d::Size(198.0f,149.0f));
		pDituRight1->setPosition(Vec2(340.0f+197.0f,284.0f));
		this->addChild(pDituRight1,1);


		Sprite *pRegistLogoBack = Sprite::createWithSpriteFrame(spriteFrame("dt_register_14.png"));
		pRegistLogoBack->setPosition(Vec2(winSize.width*0.5,winSize.height*0.923));
		this->addChild(pRegistLogoBack,1);
		Sprite *pRegistLogo = Sprite::createWithSpriteFrame(spriteFrame("dt_overregist2.png"));
		pRegistLogo->setPosition(Vec2(winSize.width*0.5,winSize.height*0.927));
		this->addChild(pRegistLogo,1);
	

		Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
		Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("dt_guanbibig.png"));
		MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(OverRegistLayer::dismiss,this));
		pCloseSelectButton->setScale(0.9f);
		pCloseSelectButton->setAnchorPoint(Vec2(-0.5/9,-0.5/9));
		Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
		pCloseButton->setPosition(Vec2(winSize.width*0.943,winSize.height*0.923));
		this->addChild(pCloseButton,1);


		const char * pTextTps = pConfigMgr->text("t463");

		LabelTTF* pTTFUserTps = LabelTTF::create(pTextTps, "HelveticaNeue-Bold", 17);
		pTTFUserTps->setColor(ccc3(115,78,17));
		pTTFUserTps->setPosition(Vec2(winSize.width*0.5, winSize.height * 0.8)); 
		this->addChild(pTTFUserTps,1);

		const char * pTextUserName = pConfigMgr->text("t94");
		LabelTTF* pTTFUserName = LabelTTF::create(pTextUserName, "HelveticaNeue-Bold", 22);
		pTTFUserName->setColor(ccc3(115,78,17));
		pTTFUserName->setPosition(Vec2(winSize.width*0.360+25, winSize.height * 0.702)); 
		this->addChild(pTTFUserName,1);

		m_pUserNameKuang= EditBox::create(cocos2d::Size(225, 32),Scale9Sprite::createWithSpriteFrame(spriteFrame("dt_register_19.png")));
		m_pUserNameKuang->setPosition(Vec2(winSize.width*0.600, winSize.height * 0.702));
		m_pUserNameKuang->setPlaceHolder(pConfigMgr->text("t163"));
		this->addChild(m_pUserNameKuang,1);
		std::string strUserName = UserDefault::getInstance()->getStringForKey("NEWZJD_ACCOUNT");
		m_pUserNameKuang->setText(strUserName.c_str());

		const char * pTextUserPassword = pConfigMgr->text("t461");

		LabelTTF* pTTFUserPassword = LabelTTF::create(pTextUserPassword, "HelveticaNeue-Bold", 22);
		pTTFUserPassword->setColor(ccc3(115,78,17));
		pTTFUserPassword->setPosition(Vec2(winSize.width*0.360+25, winSize.height * 0.598)); 
		this->addChild(pTTFUserPassword,1);

		m_pPassWordKuang= EditBox::create(cocos2d::Size(225, 32),Scale9Sprite::createWithSpriteFrame(spriteFrame("dt_register_19.png")));
		m_pPassWordKuang->setPosition(Vec2(winSize.width*0.600, winSize.height * 0.598));
		m_pPassWordKuang->setPlaceHolder(pConfigMgr->text("t97"));
		m_pPassWordKuang->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
		this->addChild(m_pPassWordKuang,1);

		const char * pTextUserPasswordOK = pConfigMgr->text("t462");

		LabelTTF* pTTFUserPasswordOK = LabelTTF::create(pTextUserPasswordOK, "HelveticaNeue-Bold", 22);
		pTTFUserPasswordOK->setColor(ccc3(115,78,17));
		pTTFUserPasswordOK->setPosition(Vec2(winSize.width*0.360+25, winSize.height * 0.492)); 
		this->addChild(pTTFUserPasswordOK,1);

		m_pPassWordKuangOK= EditBox::create(cocos2d::Size(225, 32),Scale9Sprite::createWithSpriteFrame(spriteFrame("dt_register_19.png")));
		m_pPassWordKuangOK->setPosition(Vec2(winSize.width*0.600, winSize.height * 0.492));
		m_pPassWordKuangOK->setPlaceHolder(pConfigMgr->text("t99"));
		m_pPassWordKuangOK->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
		this->addChild(m_pPassWordKuangOK,1);


		const char * pBangDingTXZ = pConfigMgr->text("t472");
		LabelTTF *pTTFBangDingTXZ=LabelTTF::create(pBangDingTXZ ,"HelveticaNeue-Bold",22);
		pTTFBangDingTXZ->setColor(ccc3(116,117,42));
		pTTFBangDingTXZ->setPosition(Vec2(winSize.width*0.159+30, winSize.height * 0.317));
		this->addChild(pTTFBangDingTXZ,1);
		/*Sprite *pBangDingTXZLine = Sprite::create("dt_register_15.png");
		pBangDingTXZLine->setScaleX(5.85f);
		pBangDingTXZLine->setPosition(Vec2(winSize.width*0.50, winSize.height * 0.303));
		this->addChild(pBangDingTXZLine);*/

	/*	Sprite *pDiKuang0 = Sprite::create("dt_register_16.png");	
		pDiKuang0->setPosition(Vec2(winSize.width*0.254, winSize.height * 0.225)) ;
		this->addChild(pDiKuang0,1);*/


		Scale9Sprite *pDituLeft2 = Scale9Sprite::createWithSpriteFrame(spriteFrame("dt_playerinfo_5.png"));
		pDituLeft2->setContentSize(cocos2d::Size(389.0f,125.0f));
		pDituLeft2->setPosition(Vec2(427.0f-194.5f,72.0f));
		this->addChild(pDituLeft2,2);
		Scale9Sprite *pDituRight2 = Scale9Sprite::createWithSpriteFrame(spriteFrame("dt_playerinfo_5.png"));
		pDituRight2->setRotationY(180);
		pDituRight2->setContentSize(cocos2d::Size(389.0f,125.0f));
		pDituRight2->setPosition(Vec2(427.0f+193.5f,72.0f));
		this->addChild(pDituRight2,1);


		Sprite *pLog0 = Sprite::createWithSpriteFrame(spriteFrame("dt_overregist1.png"));
		pLog0->setPosition(Vec2(winSize.width*0.25, winSize.height * 0.219));  
		this->addChild(pLog0,2);
		

		Sprite *okNormal =(Sprite *)GroupSprite::GroupSpriteWith("dt_warning_8.png","dt_prop_0.png",GorupSpriteTypePhotoAndPhoto);
		Sprite *okSelect =(Sprite *)GroupSprite::GroupSpriteWith("dt_warning_8.png","dt_prop_0.png",GorupSpriteTypePhotoAndPhoto);
		MenuItemSprite * okItemSprite = MenuItemSprite::create(okNormal, okSelect,  CC_CALLBACK_1(OverRegistLayer::showPlayerXieYi,this));
		Menu* m_pOkMenu = Menu::create(okItemSprite,NULL);
		//m_pOkMenu->setScale(0.8f);
		m_pOkMenu->setPosition(Vec2(winSize.width * 0.5 ,winSize.height * 0.36));
		okSelect->setScale(0.9f);
		okSelect->setAnchorPoint(Vec2(-0.5/9,-0.5/9));

		this->addChild(m_pOkMenu,5);

		/*Sprite *pDiKuang1 = Sprite::create("dt_register_16.png");	
		pDiKuang1->setPosition(Vec2(winSize.width*0.738, winSize.height * 0.225)) ; 
		this->addChild(pDiKuang1,1);*/
		Sprite *pLog1 = Sprite::createWithSpriteFrame(spriteFrame("dt_register_4.png"));
		pLog1->setPosition(Vec2(winSize.width*0.75, winSize.height * 0.219));  
		this->addChild(pLog1,2);
		
		


		/*Sprite *pDiKuang2 = Sprite::create("dt_register_16.png");	
		pDiKuang2->setPosition(Vec2(winSize.width*0.254, winSize.height * 0.079)) ; 
		this->addChild(pDiKuang2,1);*/
		Sprite *pLog2 = Sprite::createWithSpriteFrame(spriteFrame("dt_register_3.png"));
		pLog2->setPosition(Vec2(winSize.width*0.21, winSize.height * 0.085));  
		this->addChild(pLog2,2);

		/*Sprite *pDiKuang3 = Sprite::create("dt_register_16.png");	
		pDiKuang3->setPosition(Vec2(winSize.width*0.738, winSize.height * 0.079)) ;  
		this->addChild(pDiKuang3,1);*/
		Sprite *pLog3 = Sprite::createWithSpriteFrame(spriteFrame("dt_register_222.png"));
		pLog3->setPosition(Vec2(winSize.width*0.775, winSize.height * 0.085));  
		this->addChild(pLog3,2);

		/*LabelTTF *pTTF00=LabelTTF::create(pConfigMgr->text("t103") ,"HelveticaNeue-Bold",18);
		LabelTTF *pTTF01=LabelTTF::create(pConfigMgr->text("t104") ,"HelveticaNeue-Bold",12);
		LabelTTF *pTTF10=LabelTTF::create(pConfigMgr->text("t105") ,"HelveticaNeue-Bold",18);
		LabelTTF *pTTF11=LabelTTF::create(pConfigMgr->text("t106") ,"HelveticaNeue-Bold",12);
		LabelTTF *pTTF20=LabelTTF::create(pConfigMgr->text("t107") ,"HelveticaNeue-Bold",18);
		LabelTTF *pTTF21=LabelTTF::create(pConfigMgr->text("t108"), "HelveticaNeue-Bold",12);
		LabelTTF *pTTF30=LabelTTF::create(pConfigMgr->text("t109") ,"HelveticaNeue-Bold",18);
		LabelTTF *pTTF31=LabelTTF::create(pConfigMgr->text("t110") ,"HelveticaNeue-Bold",12);
		pTTF00->setAnchorPoint(Vec2(0,0));
		pTTF00->setPosition(Vec2(179.0f,106.0f));
		pTTF01->setAnchorPoint(Vec2(0,0));
		pTTF01->setPosition(Vec2(179.0f,85.0f));
		pTTF10->setAnchorPoint(Vec2(0,0));
		pTTF10->setPosition(Vec2(557.0f,106.0f));
		pTTF11->setAnchorPoint(Vec2(0,0));
		pTTF11->setPosition(Vec2(557.0f,85.0f));
		pTTF20->setAnchorPoint(Vec2(0,0));
		pTTF20->setPosition(Vec2(179.0f,40.0f));
		pTTF21->setAnchorPoint(Vec2(0,0));
		pTTF21->setPosition(Vec2(179.0f,19.0f));
		pTTF30->setAnchorPoint(Vec2(0,0));
		pTTF30->setPosition(Vec2(557.0f,40.0f));
		pTTF31->setAnchorPoint(Vec2(0,0));
		pTTF31->setPosition(Vec2(557.0f,19.0f));
		this->addChild(pTTF00,2);
		this->addChild(pTTF01,2);
		this->addChild(pTTF10,2);
		this->addChild(pTTF11,2);
		this->addChild(pTTF20,2);
		this->addChild(pTTF21,2);
		this->addChild(pTTF30,2);
		this->addChild(pTTF31,2);*/


		this->setKeypadEnabled(true);
		CC_BREAK_IF(! Layer::init());
		bRet = true;
	} while (0);

	return bRet;

}

void OverRegistLayer::setRegistLayerFrom(int num)
{
	m_nRegistFrom = num;
}


void OverRegistLayer::dismiss(Object *obj){
	playButtonSound();
	if(m_nRegistFrom == 1)
	{
		Scene* scene = Scene::create();
		RoomLayer * pRoomLayer = RoomLayer::create();
		pRoomLayer->closeLoginAward();
		pRoomLayer->automaticLogin(false);
		scene->addChild(pRoomLayer);
		Director::getInstance()->replaceScene(scene);
	}
	else
	{
		Scene* scene = Scene::create();
		RoomOption * pRoomLayer = RoomOption::create();
		scene->addChild(pRoomLayer);
		Director::getInstance()->replaceScene(scene);
	}
}

void OverRegistLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		dismiss(NULL);
	}
}

void OverRegistLayer::onOverRes(Object* obj)
{
	Integer* inter = (Integer*)obj;
	MovingLabelLayer* layer;
	switch(inter->getValue())
	{
		case 0:
			{
				string szAccount  = m_pUserNameKuang->getText();
				string szPassword = m_pPassWordKuang->getText();
				UserDefault::getInstance()->setStringForKey("NEWZJD_ACCOUNT", szAccount);
				UserDefault::getInstance()->setStringForKey("NEWZJD_PASSWORD", szPassword);

				layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t464"),Vec2(427,240));
				Scene* scene = Scene::create();
				RoomLayer* layer = RoomLayer::create();
				layer->automaticLogin(true,true);
				scene->addChild(layer);
				layer->closeLoginAward();
				TransitionFade *tScene = TransitionFade::create(2.0f, scene, Color3B::WHITE);
				Director::getInstance()->replaceScene(tScene); 
				break;
			}
		case 1:
			{
				layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t468"),Vec2(427,240));
				break;
			}
		case 2:
			{
				layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t465"),Vec2(427,240));
				break;
			}
		case 3:
			{
				layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t466"),Vec2(427,240));
				break;
			}
		case 4:
			{
				layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t467"),Vec2(427,240));
				break;
			}
		default:
			break;
	}
	this->addChild(layer,10);
}

//void OverRegistLayer::onRegistSucc(Object* obj)
//{
//	RoomLayer* layer = RoomLayer::create();
//	layer->automaticLogin(false);
//	layer->showLoginAward();
//	layer->showMoveingLabelTips(pConfigMgr->text("t407"));
//	Scene* scene = Scene::create();
//	scene->addChild(layer);
//	Director::getInstance()->replaceScene(scene);
//}

//void OverRegistLayer::onRegisterFaild(Object* obj)
//{
//	String* str = (String*)obj;
//	PromptBox* box = PromptBox::PromptBoxWith(Vec2(427,240),mPromptTypePasswordFind);
//	box->setPromptText(str->getCString());
//	this->addChild(box,100);
//}

void OverRegistLayer::showPlayerXieYi(Object *obj){
	playButtonSound();
	winSize = Director::getInstance()->getWinSize();
	string szAccount  = m_pUserNameKuang->getText();
	string szPassword = m_pPassWordKuang->getText();
	string szSPassword = m_pPassWordKuangOK->getText();

	if (szPassword != szSPassword)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t28"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer,10);
		return;
	}


	if(szAccount.length() < 6 || szAccount.length() > 12)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t29"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer,10);
		return;
	}

	if (szPassword.length() < 6 || szPassword.length() > 12)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t457"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer,10);
		return;
	}

	if ( !isLegal(szAccount) || !isLegal(szPassword))
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(pConfigMgr->text("t30"),Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer,10);
		return;
	}
	bool bRet = pMsgDispatch->connectLoginServer();
	if (!bRet)
	{
		PromptBox* box = PromptBox::PromptBoxWith(Vec2(winSize.width * 0.5,winSize.height * 0.5),mPromptTypeServerShut);
		this->addChild(box,100);
	}

	//char buf[50];
	//memset(buf, 0, 50);
	//utf::utf_gb_convert("utf-8", "gb2312", buf,50, const_cast<char*>(szAccount.c_str()), szAccount.length());

	//EntityMgr::instance()->getActor()->login()->setUserRegister(szAccount.c_str(),szPassword.c_str(),nFaceId,nGender);
	pMsgDispatch->overRegist(szAccount.c_str(),szSPassword);
}

bool OverRegistLayer::isLegal(string str)
{
	//const char* s = str.c_str();
	char s[128] = {0};
	memcpy(s, str.c_str(), min(sizeof(s)-1, str.length()));
	CCLOG("get user accout str = %s, strlen(str)=%d",str.c_str(), strlen(str.c_str()));
	CCLOG("get user accout char = %s strlen(s)=%d", s, strlen(s));
	int i = 0;
	while (i++ < str.length() - 1)
	{
		if (s[i] < 0 || s[i] > 127)
		{
			//("中文");
			CCLOG("registLayer chinese");
		}
		else if (s[i]>='0' && s[i]<='9')
		{
			//("数字");
			CCLOG("registLayer number");
		}
		else if (s[i]>='A' && s[i]<='Z' || s[i]>='a' && s[i]<='z')
		{
			CCLOG("registLayer a b c");
		}
		else
		{
			//("其它");
			CCLOG("RegistLayer::isLegal false s[%d]:%d %s", i, s[i], &s[i]);
			return  false;
		}
	}
	return  true;
}

 