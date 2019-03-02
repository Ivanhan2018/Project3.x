#include "LotteryAddSub.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"
#include "comm.h"
#include "LotteryRechargeDetail.h"
#include "LotteryMyLot.h"
#include "EntityMgr.h"
#include "MyNSString.h"
#include "LotterySceneControl.h"
#include "BetLimit.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni/JniHelper.h"
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#include <android/log.h>
#include <jni.h>
#include <cstring>
#endif
#include "VersionControl.h"

USING_NS_CC;



using namespace CocosDenshion;

Scene* LotteryAddSub::scene()
{
	Scene *scene = Scene::create();
	LotteryAddSub *layer = LotteryAddSub::create();
	scene->addChild(layer);
	return scene;
}

LotteryAddSub::LotteryAddSub()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryAddSub::addHuiYuanRet), "AddHuiYuanRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryAddSub::peiERet), "PeiERet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryAddSub::fanDianRet), "SetWebFanDianRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryAddSub::webFanDian), "GetWebFanDianRet", NULL);
}

LotteryAddSub::~LotteryAddSub()
{
	if(fandianTable != nullptr)
	{
		fandianTable->release();
	}

	NotificationCenter::getInstance()->removeObserver(this, "AddHuiYuanRet");
	NotificationCenter::getInstance()->removeObserver(this, "PeiERet");
	NotificationCenter::getInstance()->removeObserver(this, "GetWebFanDianRet");
	NotificationCenter::getInstance()->removeObserver(this, "SetWebFanDianRet");
		
}

void LotteryAddSub::addHuiYuanRet(Object *obj)
{
	Dictionary *dic = (Dictionary *)obj;
	String *s_t_Desc = (String *)dic->objectForKey("s_t_Desc");

	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(s_t_Desc->getCString(), Vec2(SCREEN_WIDTH/2, 427));
	//    MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t808"),Vec2(SCREEN_WIDTH/2,427));
	this->addChild(layer,20);
}

void LotteryAddSub::peiERet(Object *obj)
{
	//配额
	if (obj == NULL || ltfDispach == NULL) return;
	//读数据
	Dictionary *dic = (Dictionary *)obj;
	Integer *itotal30 = (Integer *)dic->objectForKey("total30");
	Integer *itotal28 = (Integer *)dic->objectForKey("total28");
	Integer *itotal27 = (Integer *)dic->objectForKey("total27");
	Integer *ipere30 = (Integer *)dic->objectForKey("pere30");
	Integer *ipere28 = (Integer *)dic->objectForKey("pere28");
	Integer *ipere27 = (Integer *)dic->objectForKey("pere27");

	pere30 = itotal30->getValue() - ipere30->getValue();
	if (pere30 < 0) pere30 = 0;
	pere28 = itotal28->getValue() - ipere28->getValue();
	if (pere28 < 0) pere28 = 0;
	pere27 = itotal27->getValue() - ipere27->getValue();
	if (pere27 < 0) pere27 = 0;

#ifdef VER_QIANBO			
	Integer *itotal29 = (Integer *)dic->objectForKey("total29");
	Integer *itotal26 = (Integer *)dic->objectForKey("total26");
	Integer *ipere29 = (Integer *)dic->objectForKey("pere29");
	Integer *ipere26 = (Integer *)dic->objectForKey("pere26");
	pere29 = itotal29->getValue() - ipere29->getValue();
	if (pere29 < 0) pere29 = 0;
	pere26 = itotal26->getValue() - ipere26->getValue();
	if (pere26 < 0) pere26 = 0;
#endif

#ifdef VER_369			
	Integer *itotal29 = (Integer *)dic->objectForKey("total29");
	Integer *itotal26 = (Integer *)dic->objectForKey("total26");
	Integer *ipere29 = (Integer *)dic->objectForKey("pere29");
	Integer *ipere26 = (Integer *)dic->objectForKey("pere26");
	pere29 = itotal29->getValue() - ipere29->getValue();
	if (pere29 < 0) pere29 = 0;
	pere26 = itotal26->getValue() - ipere26->getValue();
	if (pere26 < 0) pere26 = 0;
#endif

	setPeiEString();

	//无法获取web返点
	//EditBox *webEditBox = (EditBox *)this->getChildByTag(EDITBOX_TAG);
	//webEditBox->getText();EntityMgr::instance()->getDispatch()->m_szAccount
}

void LotteryAddSub::fanDianRet(Object *obj)
{
	if (obj == NULL) return;

	Dictionary *dic = (Dictionary *)obj;
	Integer *res_id = (Integer *)dic->objectForKey("res");
	Integer *fandian = (Integer* )dic->objectForKey("fandian");

	int res = res_id->getValue();
	if(0 == res)
	{
		//成功
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t907"), Vec2(SCREEN_WIDTH / 2,427));
		this->addChild(layer,255);
	} else
	if(2 == res)
	{
		//超过限额
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(
			String::createWithFormat("%s%.02f%%",ConfigMgr::instance()->text("display_text.xml", "t908"),EntityMgr::instance()->getDispatch()->f_fandian * 100)->getCString(),
			Vec2(SCREEN_WIDTH / 2,427));
		this->addChild(layer,255);
	} else
	{
		//失败
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t909"), Vec2(SCREEN_WIDTH / 2,427));
		this->addChild(layer,255);
	}
}

void LotteryAddSub::webFanDian(Object *obj)
{
	if (obj == NULL) return;

	Dictionary *dic = (Dictionary *)obj;	
	String* pRegURL = (String*)dic->objectForKey("regurl");
	Float *fandian = (Float* )dic->objectForKey("fandian");

	//设置网页返点
	EditBox *webEditBox = (EditBox *)this->getChildByTag(EDITBOX_TAG);
	webEditBox->setText(String::createWithFormat("%.1f",fandian->getValue() * 100)->getCString());
	//设置下级注册URL，2018.8
	m_strRegURL = pRegURL->getCString();
	EditBox *regEditBox = (EditBox *)this->getChildByTag(EDITBOX_TAG+50);

	//如果是代理，就有下级注册地址
	if(regEditBox)
	{
		String* strRegText = String::createWithFormat("%s%d", m_strRegURL.c_str(),  EntityMgr::instance()->getDispatch()->m_dwUserID );
		if(strRegText->length() > 20)
		{
			std::string strShortText = strRegText-> getCString();
			regEditBox->setText(strShortText.substr(0,20).append("...").c_str());
		}
		else
			regEditBox->setText(strRegText->getCString());
	}
}

void LotteryAddSub::refreshLayer()
{
	//刷新配额
	EntityMgr::instance()->getDispatch()->SendPacketWithPeiE();
	//刷新返点
	EntityMgr::instance()->getDispatch()->SendPacketWithQueryWebFanDian();
}

void LotteryAddSub::setPeiEString()
{
	//注释掉配额代码
	return;
	
	//个:xxx还剩X个的“个”
	String *perStr = String::create(ConfigMgr::instance()->text("display_text.xml","t777"));	
	//配额多少个
	String *dispatchStr = NULL;

#ifdef VER_QIANBO
	if (fanDianMax >= 31.0f)
	{
		dispatchStr = String::createWithFormat("[3.0]%d%s [2.9]%d%s [2.8]%d%s\n[2.7]%d%s [2.6]%d%s",				
			pere30,
			perStr->getCString(),
			pere29,
			perStr->getCString(),
			pere28,
			perStr->getCString(),				
			pere27,
			perStr->getCString(),
			pere26,
			perStr->getCString()
			);	
	} else
		if (fanDianMax >= 30.0f)
		{
			dispatchStr = String::createWithFormat("[2.9]%d%s [2.8]%d%s [2.7]%d%s\n[2.6]%d%s",				
				pere29,
				perStr->getCString(),
				pere28,
				perStr->getCString(),				
				pere27,
				perStr->getCString(),
				pere26,
				perStr->getCString()
				);	
		} else
			if (fanDianMax >= 28.0f)
			{
				dispatchStr = String::createWithFormat("[2.8]%d%s [2.7]%d%s [2.6]%d%s",					
					pere28,
					perStr->getCString(),				
					pere27,
					perStr->getCString(),
					pere26,
					perStr->getCString()
					);	
			} else			
					{
						ltfDispach->setVisible(false);
						ltfDispachTitle->setVisible(false);
					}
#endif

#ifdef VER_369
	if (fanDianMax >= 31.0f)
	{
		dispatchStr = String::createWithFormat("[3.0]%d%s [2.9]%d%s [2.8]%d%s\n[2.7]%d%s [2.6]%d%s",				
			pere30,
			perStr->getCString(),
			pere29,
			perStr->getCString(),
			pere28,
			perStr->getCString(),				
			pere27,
			perStr->getCString(),
			pere26,
			perStr->getCString()
			);	
	} else
		if (fanDianMax >= 30.0f)
		{
			dispatchStr = String::createWithFormat("[2.9]%d%s [2.8]%d%s [2.7]%d%s\n[2.6]%d%s",				
				pere29,
				perStr->getCString(),
				pere28,
				perStr->getCString(),				
				pere27,
				perStr->getCString(),
				pere26,
				perStr->getCString()
				);	
		} else
			if (fanDianMax >= 28.0f)
			{
				dispatchStr = String::createWithFormat("[2.8]%d%s [2.7]%d%s [2.6]%d%s",					
					pere28,
					perStr->getCString(),				
					pere27,
					perStr->getCString(),
					pere26,
					perStr->getCString()
					);	
			} else			
			{
				ltfDispach->setVisible(false);
				ltfDispachTitle->setVisible(false);
			}
#endif

#ifdef VER_9YI
	if (fanDianMax >= 30.0f)
	{
		dispatchStr = String::createWithFormat("[3.0]%d%s [2.8]%d%s [2.7]%d%s",				
			pere30,
			perStr->getCString(),				
			pere28,
			perStr->getCString(),				
			pere27,
			perStr->getCString()
			);	
	} else
		if (fanDianMax >= 28.0f)
		{
			dispatchStr = String::createWithFormat("[2.8]%d%s [2.7]%d%s",						
				pere28,
				perStr->getCString(),				
				pere27,
				perStr->getCString()
				);
		} else
			if (fanDianMax >= 27.0f)
			{
				dispatchStr = String::createWithFormat("[2.7]%d%s",						
					pere27,
					perStr->getCString()
					);
			} else
			{
				ltfDispach->setVisible(false);
				ltfDispachTitle->setVisible(false);
			}
#endif
			if(dispatchStr != NULL)
			{
				ltfDispach->setString(dispatchStr->getCString());
			}
}

bool LotteryAddSub::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initData();
	this->initView();
	this->setKeypadEnabled(true);
	//if(m_cbType != 1){
	//	setTouchEnabled(true);
	//}
	return true;
}

void LotteryAddSub::initData()
{
	winSize = Director::getInstance()->getWinSize();
	posX1 = 250;
	posX2 = 260;
	gapY = 60;
	tableCellHeight = 30;
	tableCellNum = 0;
	m_cbType = EntityMgr::instance()->getDispatch()->m_cbType;
	fandianStatus = 0;
	isDaili = 1;
	pere27 = 0;
	pere28 = 0;
	pere30 = 0;
	ltfDispach = NULL;
	fanDianMax =  EntityMgr::instance()->getDispatch()->f_fandian * 1000;
}

void LotteryAddSub::registerAddr(Object *obj)
{
	String *url = String::createWithFormat("%s%d", ConfigMgr::instance()->text("display_config.xml", "t5001"), EntityMgr::instance()->getDispatch()->m_dwUserID);
//设置下级注册URL
	url = String::createWithFormat("%s%d", m_strRegURL.c_str(), EntityMgr::instance()->getDispatch()->m_dwUserID);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	TCHART w_url[100] = {0};
	MyNSString::mbs2wc(url->getCString(), strlen(url->getCString()), w_url);
	ShellExecute(NULL, L"open", w_url, NULL, NULL, SW_SHOWNORMAL);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	Application::getInstance()->openURL(url->getCString());
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	Application::getInstance()->openURL(url->getCString());
#endif
}
	/**
	@复制网址	 
	 */
void LotteryAddSub::CopyURL(Object *obj)
{
	char temp[100] = {0};
	std::sprintf(temp, "%s%ld", m_strRegURL.c_str(), EntityMgr::instance()->getDispatch()->m_dwUserID);

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    //com/hapi/DDZClient/DDZClient
    if(JniHelper::getStaticMethodInfo(minfo,
                                      "com/hapigame/Lollery/Lollery",
                                      "CopyURL",
                                      "(Ljava/lang/String;)V"))
    {
        jstring StringArg1 = minfo.env->NewStringUTF(temp);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, StringArg1);
        minfo.env->DeleteLocalRef(StringArg1);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
#endif
}
void LotteryAddSub::initView()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LotteryAddSub::onTouchBegan, this);	
	listener->onTouchEnded = CC_CALLBACK_2(LotteryAddSub::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(LotteryAddSub::onTouchMoved, this);
	listener->onTouchCancelled = CC_CALLBACK_2(LotteryAddSub::onTouchCancelled, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	//头
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame(HEAD_IMG));
	bk1->setAnchorPoint(Vec2(0,1));
	bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
	bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
	bk1->setScaleY(162 / bk1->getContentSize().height);
	this->addChild(bk1,1);

	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryAddSub::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	//pCloseButton->setPosition(Vec2(32, SCREEN_HEIGHT - 120));
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	float deltaX = 70;
	ccColor3B blackColor = ccc3(76,76,76);

	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t889"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	////请认真填写一下信息
	fontSize = 30;
	//LabelTTF *title1 = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t760"), "", fontSize);
	//title1->setAnchorPoint(Vec2(0,0.5f));
	//title1->setPosition(Vec2(10, SCREEN_HEIGHT - 180));
	//title1->setColor(ccc3(171,71,71));
	//title1->setHorizontalAlignment(kCCTextAlignmentLeft);
	//this->addChild(title1);

	if(m_cbType!=1){
		
		LabelTTF *title2 = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t761"), "", fontSize);
		title2->setAnchorPoint(Vec2(0,0.5f));
		title2->setPosition(Vec2(20, SCREEN_HEIGHT - 220));
		title2->setColor(ccc3(171,71,71));
		title2->setHorizontalAlignment(kCCTextAlignmentLeft);
		this->addChild(title2);
	}else{
		
		//推荐网址文字	
		String *title2Str2 = String::createWithFormat("%s",
			ConfigMgr::instance()->text("display_DuangDong.xml", "t10"));
		LabelTTF *title3 = LabelTTF::create(title2Str2->getCString(), "",fontSize);
		title3->setAnchorPoint(Vec2(1,0.5f));
		title3->setColor(blackColor);
		title3->setHorizontalAlignment(kCCTextAlignmentLeft);
		title3->setPosition(Vec2(posX1-20, SCREEN_HEIGHT - 220));
		this->addChild(title3,1);

		//网址
		std::string title2Str = String::createWithFormat("%s%d",
			ConfigMgr::instance()->text("display_config.xml", "t5001"),
			EntityMgr::instance()->getDispatch()->m_dwUserID)->getCString();			

		Sprite *inputBk = Sprite::createWithSpriteFrame(spriteFrame("recharge_16.png"));
		inputBk->setAnchorPoint(Vec2(0,0.5f));
		inputBk->setPosition(Vec2(posX2 - 5,title3->getPositionY()));
		this->addChild(inputBk);

		EditBox *editBox = EditBox::create(cocos2d::Size(inputBk->getContentSize().width - 10, 40), "blank.png");
		editBox->setPosition(inputBk->getPosition());
		editBox->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
		editBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		editBox->setInputMode(EditBox::InputMode::URL);		
		editBox->setAnchorPoint(Vec2(0,0.5f));
		editBox->setFontColor(ccc3(0,0,0));
		editBox->setText(title2Str.substr(0,20).append("...").c_str());		
		editBox->setFontName("");		
		editBox->setFontSize(fontSize);
		editBox->setColor(ccc3(112,112,112));
		editBox->setEnabled(false);
		this->addChild(editBox, 1);

		//LabelTTF *ltUrl = LabelTTF::create();
		//ltUrl->setFontName("");
		//ltUrl->setFontSize(22);
		//ltUrl->setFontFillColor(blackColor);
		//ltUrl->setString(title2Str->getCString());
		////ltUrl->setDimensions(inputBk->getContentSize().width - cocos2d::Size(10,0));
		//ltUrl->setContentSize(inputBk->getContentSize());
		//ltUrl->setAnchorPoint(Vec2(0,0.5f));
		//ltUrl->setHorizontalAlignment(kCCTextAlignmentLeft);
		//ltUrl->setVerticalAlignment(TextVAlignment::CENTER);
		//ltUrl->setPosition(inputBk->getPosition() + Vec2(5,0));
		//this->addChild(ltUrl,1);	

		auto btReg = ui::Button::create("my_lottery_37.png","my_lottery_37_2.png","my_lottery_37.png");
		btReg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		btReg->setPosition(Vec2(SCREEN_WIDTH -deltaX, SCREEN_HEIGHT - 220));
		btReg->setScale(0.8);
		btReg->addClickEventListener(CC_CALLBACK_1(LotteryAddSub::registerAddr,this));
		this->addChild(btReg, 1);			
	}

	startY = SCREEN_HEIGHT - 180-60-60;

	fontSize = 30;
	int deltaUp = 5;

	int row_count = 9;

#ifdef VER_QIANBO
	row_count = 8;
#endif

//#ifdef VER_369
//	row_count = 8;
//#endif

	for(int i = 0 ; i < row_count ; i++){
		String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 762+i)->getCString()));
		LabelTTF *label = LabelTTF::create(labelStr->getCString(), "", fontSize);
		label->setAnchorPoint(Vec2(1,0.5f));
		label->setPosition(Vec2(posX1, startY-gapY*i + (i == 0 ? deltaUp : 0)));
		label->setColor(blackColor);
		this->addChild(label, 1);
		if(i == 1){
			continue;
		}
		Sprite *inputBk = Sprite::createWithSpriteFrame(spriteFrame("recharge_16.png"));
		inputBk->setAnchorPoint(Vec2(0,0.5f));
		inputBk->setPosition(Vec2(posX2-5, startY-gapY*i + (i == 0 ? deltaUp : 0)));
		this->addChild(inputBk);

		EditBox *editBox = EditBox::create(cocos2d::Size(220, 30), "blank.png");
		editBox->setPosition(Vec2(posX2, startY-gapY*i + (i == 0 ? deltaUp : 0)));
		editBox->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
		editBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		editBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
		editBox->setAnchorPoint(Vec2(0,0.5f));
		editBox->setFontColor(ccc3(0,0,0));
		editBox->setFontName("");editBox->setFontSize(fontSize);
		editBox->setColor(ccc3(112,112,112));
		editBox->setMaxLength(20);
		editBox->setTag(EDITBOX_TAG+i);
		if(i == 0){
			editBox->setText("1.0");
		}
		if(i == 2){
			editBox->setText("0.0");
		}
		if(i>=4&&i<=5){
			editBox->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
			//            editBox->setText("a123456");
		}
		if(i>=6&&i<=7){
			editBox->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
			//            editBox->setText("123456");
		}
		if(i>=8&&i<=9)
		{
			editBox->setInputMode(EditBox::InputMode::NUMERIC);
		}
		this->addChild(editBox, 1);
		if(m_cbType!=1){
			editBox->setEnabled(false);
		}
	}

	daili = MenuItemImage::create("add_sub_002.png", "add_sub_001.png", CC_CALLBACK_1(LotteryAddSub::pressDaili,this));
	membership = MenuItemImage::create("add_sub_002.png", "add_sub_001.png", CC_CALLBACK_1(LotteryAddSub::pressMembership,this));
	Menu *selectRegisterTypeMenu = Menu::create(daili, membership, NULL);
	selectRegisterTypeMenu->alignItemsHorizontallyWithPadding(120);
	selectRegisterTypeMenu->setPosition(Vec2(SCREEN_WIDTH / 2,startY-gapY*1));
	daili->selected();
	this->addChild(selectRegisterTypeMenu);

	LabelTTF* dailiLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml","t771"),"",fontSize);
	LabelTTF* membershipLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t772"),"",fontSize);
	dailiLabel->setPosition(Vec2(selectRegisterTypeMenu->getPositionX()-30,selectRegisterTypeMenu->getPositionY()));
	membershipLabel->setPosition(Vec2(selectRegisterTypeMenu->getPositionX()+130,selectRegisterTypeMenu->getPositionY()));
	dailiLabel->setColor(ccc3(0,0,0));
	membershipLabel->setColor(ccc3(0,0,0));
	this->addChild(dailiLabel);
	this->addChild(membershipLabel);

	//设置网上返点跟 下面开户功能的分割线
	//Sprite *gap2 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	//gap2->setAnchorPoint(Vec2(0, 0.5));
	//gap2->setPosition(Vec2(0, selectRegisterTypeMenu->getPositionY() + 30));
	//this->addChild(gap2);


	MenuItemImage *setItemFirst = MenuItemImage::create("add_sub_003.png", "add_sub_004.png",  CC_CALLBACK_1(LotteryAddSub::pressSetFirst,this));
	MenuItemImage *setItemSecond = MenuItemImage::create("add_sub_003.png", "add_sub_004.png", CC_CALLBACK_1(LotteryAddSub::pressSetSecond,this));
	setItemFirst->setPosition(Vec2(SCREEN_WIDTH - deltaX, startY - gapY * 0 + deltaUp));
	setItemSecond->setPosition(Vec2(SCREEN_WIDTH -deltaX, startY - gapY * 4));
	setItemFirst->setScale(0.8);
	setItemSecond->setScale(0.8);
	Menu *setMenu = Menu::create(setItemFirst, setItemSecond, NULL);
	setMenu->setPosition(Vec2(0,0));
	this->addChild(setMenu);

	//注释掉配额代码
	bool isShow =false;
	if(isShow)
	{
		String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml","t776"));
		ltfDispachTitle = LabelTTF::create(labelStr->getCString(), "", fontSize);
		ltfDispachTitle->setAnchorPoint(Vec2(1,0.5f));
		ltfDispachTitle->setPosition(Vec2(posX1, startY-gapY*row_count - 15));
		ltfDispachTitle->setColor(blackColor);
		this->addChild(ltfDispachTitle, 1);
	}

	ltfDispach = LabelTTF::create("", "", fontSize);
	ltfDispach->setAnchorPoint(Vec2(0,0.5f));
	ltfDispach->setPosition(Vec2(posX2, startY - gapY * row_count - 15));
	ltfDispach->setColor(ccc3(171,71,71));
	ltfDispach->setHorizontalAlignment(TextHAlignment::LEFT);
	ltfDispach->setVerticalAlignment(TextVAlignment::CENTER);
	this->addChild(ltfDispach);

	MenuItemImage *confirmItem = MenuItemImage::create("beting22.png", "beting23.png", CC_CALLBACK_1(LotteryAddSub::pressConfirm,this));
	confirmItem->setPosition(Vec2(SCREEN_WIDTH/2, startY - gapY * (row_count + 1) - 15));
	confirmItem->setScale(0.8f);
	Menu *confirmMenu = Menu::create(confirmItem, NULL);
	confirmMenu->setPosition(Vec2(0,0));
	this->addChild(confirmMenu);

	//Sprite *gap3 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	//gap3->setAnchorPoint(Vec2(0,0.5));
	//gap3->setPosition(Vec2(0, startY-gapY*(row_count + 2)));
	//this->addChild(gap3);


	/*********************************/
	m_TableNode = Node::create();
	m_TableNode->setPosition(Vec2::ZERO);
	this->addChild(m_TableNode, 1);

	tableCellWidth = 216;
	fandianTableBk = Sprite::createWithSpriteFrame(spriteFrame("lottery_record_01.png"));
	fandianTableBk->setAnchorPoint(Vec2(0,0));
	fandianTableBk->setScaleX(tableCellWidth/fandianTableBk->getContentSize().width);
	fandianTableBk->setPosition(Vec2::ZERO);
	m_TableNode->addChild(fandianTableBk,1);
	tableViewSize.width = tableCellWidth;
	tableViewSize.height = fandianTableBk->getContentSize().height;

	//下拉菜单的弹出位置
	tableViewPos1 = Vec2(posX2 - 5, winSize.height -430);
	tableViewPos2 = Vec2(tableViewPos1.x, tableViewPos1.y-gapY*2);

	fandianTable = TableView::create(this, tableViewSize);
	fandianTable->setDelegate(this);
	fandianTable->setAnchorPoint(Vec2(0.5, 0.5));
	fandianTable->setPosition(Vec2::ZERO);
	fandianTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	fandianTable->setDirection(ScrollView::Direction::VERTICAL);
	fandianTable->retain();
	m_TableNode->addChild(fandianTable, 2);
	hideTable();

	/********/

	Sprite *timeButtonNormal = Sprite::createWithSpriteFrame(spriteFrame("beting17.png"));
	Sprite *timeButtonSelect = Sprite::createWithSpriteFrame(spriteFrame("beting18.png"));
	timeButtonItem = MenuItemSprite::create(timeButtonNormal,timeButtonSelect,CC_CALLBACK_1(LotteryAddSub::pressSetFandian,this));
	timeButtonItem->setTag(1);
	timeButtonItem->setScale(0.86);
	timeButtonItem->setPosition(Vec2(posX2-5+320 -timeButtonNormal->getContentSize().width/2+4,startY + deltaUp));

	Sprite *timeButtonNormal1 = Sprite::createWithSpriteFrame(spriteFrame("beting17.png"));
	Sprite *timeButtonSelect1 = Sprite::createWithSpriteFrame(spriteFrame("beting18.png"));
	timeButtonItem1 = MenuItemSprite::create(timeButtonNormal1,timeButtonSelect1,CC_CALLBACK_1(LotteryAddSub::pressSetFandian,this));
	timeButtonItem1->setTag(2);
	timeButtonItem1->setScale(0.86);
	timeButtonItem1->setPosition(Vec2(timeButtonItem->getPositionX(),startY-2*gapY));

	Menu *timeButton = Menu::create(timeButtonItem, timeButtonItem1,NULL);
	timeButton->setPosition(Vec2::ZERO);
	this->addChild(timeButton);

	setPeiEString();
	//读取配额
	EntityMgr::instance()->getDispatch()->SendPacketWithPeiE();
	EntityMgr::instance()->getDispatch()->SendPacketWithQueryWebFanDian();
}

void LotteryAddSub::hideTable()
{
	m_TableNode->setPosition(Vec2(1000,1000));

	for(int i = 0 ; i < 9 ; i++){
		if(i == 1){
			continue;
		}
		EditBox *editBox = (EditBox *)this->getChildByTag(EDITBOX_TAG+i);
		if(editBox)	editBox->setEnabled(true);
	}
}

void LotteryAddSub::showTable(int posIndex)
{
	if(posIndex == 1){
		m_TableNode->setPosition(tableViewPos1);
	}else{
		m_TableNode->setPosition(tableViewPos2);
	}

	for(int i = 0 ; i < 9 ; i++){
		if(i == 1){
			continue;
		}
		EditBox *editBox = (EditBox *)this->getChildByTag(EDITBOX_TAG+i);
		if(editBox) editBox->setEnabled(false);
	}
}
void LotteryAddSub::pressSetFandian(Object *obj)
{
	playButtonSound();

	if(m_cbType!=1 || fanDianMax == 0){
		return;
	}
	MenuItemSprite *timeButtonItem = (MenuItemSprite *)obj;
	if (fandianStatus != 0) {
		hideTable();
		fandianStatus = 0;
		timeButtonItem->unselected();
		return;
	}
	timeButtonItem->selected();
	int objTag = timeButtonItem->getTag();
	if(objTag == 1){
		fandianStatus = 1;
		showTable(1);
	}else{
		fandianStatus = 2;
		showTable(2);
	}
}

void LotteryAddSub::pressSetFirst(Object *obj)
{
	playButtonSound();

	EditBox *webEditBox = (EditBox *)this->getChildByTag(EDITBOX_TAG);
	const char* szUrl = webEditBox->getText();

	float fqq = atof(szUrl);

	bool isShow = (fqq * 10 == 28.0f && pere28 == 0) || (fqq * 10 == 27.0f && pere27 == 0) || (fqq * 10 == 30.0f && pere30 == 0);

#ifdef VER_QIANBO
	isShow = (fqq * 10 == 28.0f && pere28 == 0) || (fqq * 10 == 27.0f && pere27 == 0) || (fqq * 10 == 30.0f && pere30 == 0)|| (fqq * 10 == 29.0f && pere29 == 0)|| (fqq * 10 == 26.0f && pere26 == 0);
#endif // VER_QIANBO
#ifdef VER_369
	isShow = (fqq * 10 == 28.0f && pere28 == 0) || (fqq * 10 == 27.0f && pere27 == 0) || (fqq * 10 == 30.0f && pere30 == 0)|| (fqq * 10 == 29.0f && pere29 == 0)|| (fqq * 10 == 26.0f && pere26 == 0);
#endif // VER_369

	//注释掉配额代码
	isShow =false;

	if(isShow)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(String::createWithFormat("%s0",ConfigMgr::instance()->text("display_text.xml", "t776"))->getCString(),
			Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,20);
		return;
	}
	if(fqq * 10 < 25 && fqq * 10 < fanDianMax)
	{
		EntityMgr::instance()->getDispatch()->SendPacketWithWebFanDian(fqq / 100.0f);
	} else
	{
		//超过限额
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(
			String::createWithFormat("%s%.02f%%",ConfigMgr::instance()->text("display_text.xml", "t908"),2.50f)->getCString(),
			Vec2(SCREEN_WIDTH / 2,427));
		this->addChild(layer,255);
	}
	return;
}

void LotteryAddSub::pressSetSecond(Object *obj)
{
	playButtonSound();

	EditBox *passEditBox = (EditBox *)this->getChildByTag(EDITBOX_TAG+4);
	EditBox *surePassEditBox = (EditBox *)this->getChildByTag(EDITBOX_TAG+5);
	EditBox *securePassEditBox = (EditBox *)this->getChildByTag(EDITBOX_TAG+6);
	EditBox *sureSecurePassEditBox = (EditBox *)this->getChildByTag(EDITBOX_TAG+7);
	passEditBox->setText("a123456");
	surePassEditBox->setText("a123456");
	securePassEditBox->setText("123456");
	sureSecurePassEditBox->setText("123456");
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t774"), Vec2(SCREEN_WIDTH/2, 427));
	this->addChild(layer,20);
	return;
}

void LotteryAddSub::pressConfirm(Object *obj)
{
	playButtonSound();

	//是不是代理	
	if(m_cbType!=1){		
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t775"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,20);
		return;
	}
	EditBox *accountEditBox = (EditBox *)this->getChildByTag(EDITBOX_TAG+3);
	EditBox *passEditBox = (EditBox *)this->getChildByTag(EDITBOX_TAG+4);
	EditBox *surePassEditBox = (EditBox *)this->getChildByTag(EDITBOX_TAG+5);
	EditBox *securePassEditBox = (EditBox *)this->getChildByTag(EDITBOX_TAG+6);
	EditBox *sureSecurePassEditBox = (EditBox *)this->getChildByTag(EDITBOX_TAG+7);	
	EditBox *qqEditBox = (EditBox *)this->getChildByTag(EDITBOX_TAG+8);	

	const char* szAccount = accountEditBox->getText();
	const char* szLoginPass = passEditBox->getText();
	const char* szSureLoginPass = surePassEditBox->getText();
	const char* szSecurePass = securePassEditBox->getText();
	const char* szSureSecurePass = sureSecurePassEditBox->getText();
	const char* szQQ = qqEditBox->getText();

	//用户名不能为空！
	if (strcmp(szAccount, "") == 0) {
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t808"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,20);
		return;
	}
	//用户名包含非法字符！
	if (BetLimit::ValidateStr(szAccount)) {
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t816"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,20);
		return;
	}
	//账号必须位6-12位
	if(strlen(szAccount) > 12 || strlen(szAccount) < 6)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t809"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,20);
		return;
	}

	//账号密码不能为空！
	if (strcmp(szLoginPass, "") == 0) {
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t810"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,20);
		return;
	}

	//账号密码必须位6-12位
	if(strlen(szLoginPass) > 12 || strlen(szLoginPass) < 6)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t814"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,20);
		return;
	}

	//两次账号密码不一致！
	if (strcmp(szLoginPass, szSureLoginPass) != 0) {
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t810"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,20);
		return;
	}

	//取款密码不能为空！
	if (strcmp(szSecurePass, "") == 0) {
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t812"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,20);
		return;
	}

	//取款密码必须位6-12位
	if(strlen(szSecurePass) > 12 || strlen(szSecurePass) < 6)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t815"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,20);
		return;
	}

	//两次取款密码不一致！
	if (strcmp(szSecurePass, szSureSecurePass) != 0) {
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t813"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,20);
		return;
	}

	//联系QQ必须>=5位
	if(strlen(szQQ) < 5)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("t37"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,20);
		return;
	}

	EditBox *editBox = (EditBox *)this->getChildByTag(EDITBOX_TAG+2);
	const char* szFandian = editBox->getText();	

	float ffan = atof(szFandian); 

	bool isShow = (ffan * 10 == 28.0f && pere28 == 0) || (ffan * 10 == 27.0f && pere27 == 0) || (ffan * 10 == 30.0f && pere30 == 0);
	//注释掉配额代码

#ifdef VER_QIANBO
	isShow = (ffan * 10 == 28.0f && pere28 == 0) || (ffan * 10 == 27.0f && pere27 == 0) || (ffan * 10 == 30.0f && pere30 == 0)|| (ffan * 10 == 29.0f && pere29 == 0)|| (ffan * 10 == 26.0f && pere26 == 0);
#endif // VER_QIANBO

#ifdef VER_369
	isShow = (ffan * 10 == 28.0f && pere28 == 0) || (ffan * 10 == 27.0f && pere27 == 0) || (ffan * 10 == 30.0f && pere30 == 0)|| (ffan * 10 == 29.0f && pere29 == 0)|| (ffan * 10 == 26.0f && pere26 == 0);
#endif // VER_369

	isShow =false;

	if(isShow){
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(String::createWithFormat("%s0",ConfigMgr::instance()->text("display_text.xml", "t776"))->getCString(),
			Vec2(SCREEN_WIDTH/2, 427));		
		this->addChild(layer,20);
		return;
	}

	std::string tempQQ = szQQ;

#ifdef VER_9YI
	EditBox *qqEditBox = (EditBox *)this->getChildByTag(EDITBOX_TAG+8);
	if(qqEditBox) tempQQ = qqEditBox->getText();	
	int tempLength = tempQQ.size();
	if(tempLength < 5 || tempLength >= 15)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(String::createWithFormat("%s",ConfigMgr::instance()->text("display_text.xml", "t4561"))->getCString(),
			Vec2(SCREEN_WIDTH/2, 427));		
		this->addChild(layer,20);
		return;
	}
#endif // 

	if((ffan * 10 < fanDianMax)|| (fanDianMax == 28.0f && ffan * 10 == 28.0f))
	{
		EntityMgr::instance()->getDispatch()->SendPacketWithAddHuiYuan(isDaili, ffan / 100.0f, szAccount, szLoginPass, szSecurePass, tempQQ);
		EntityMgr::instance()->getDispatch()->SendPacketWithPeiE();
	} else
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t778"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,20);
		return;
	}
}

void LotteryAddSub::pressDaili(Object *obj)
{
	playButtonSound();

	daili->selected();
	membership->unselected();
	isDaili = 1;
}

void LotteryAddSub::pressMembership(Object *obj)
{
	playButtonSound();

	daili->unselected();
	membership->selected();
	isDaili = 0;
}

void LotteryAddSub::back(Object *obj)
{
	playButtonSound();
	Scene *scene = LotteryMyLot::scene();
	Director::getInstance()->popScene();
}

void LotteryAddSub::onEnter()
{
	Layer::onEnter();
}

void LotteryAddSub::onExit()
{
	Layer::onExit();
}

void LotteryAddSub::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}


bool LotteryAddSub::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	//Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	return true;
}
void LotteryAddSub::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	//Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());

}

void LotteryAddSub::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	//Vec2 touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
}

void LotteryAddSub::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
	//
}

void LotteryAddSub::tableCellTouched(TableView* table, TableViewCell* cell)
{
	int cellTag = cell->getTag();
	float value = 0.1f*cellTag;
	char valueStr[10] = {0};
	sprintf(valueStr, "%.1f", value);
	if (fandianStatus == 1) {
		EditBox *editBox = (EditBox *)this->getChildByTag(EDITBOX_TAG+0);
		editBox->setText(valueStr);
		timeButtonItem->unselected();
	}else{
		EditBox *editBox = (EditBox *)this->getChildByTag(EDITBOX_TAG+2);
		editBox->setText(valueStr);
		timeButtonItem1->unselected();
	}
	hideTable();
}

cocos2d::Size LotteryAddSub::cellSizeForTable(TableView *table)
{
	return cocos2d::Size(tableCellWidth, tableCellHeight);
}

TableViewCell* LotteryAddSub::tableCellAtIndex(TableView *table, ssize_t idx)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();
	cell->autorelease();
	cell->setTag(idx);

	float value = 0.0;
	value+=0.1f*idx;

	char str[4] = {0};
	sprintf(str, "%.1f", value);

	float fontSize = 27;
	ccColor3B blackColor = ccc3(76,76,76);
	LabelTTF *label = LabelTTF::create(str, "", fontSize);
	label->setAnchorPoint(Vec2(0,0.5f));
	label->setPosition(Vec2(10, tableCellHeight/2));
	//    label->setPosition(Vec2(tableCellWidth/2, tableCellHeight/2));
	label->setColor(blackColor);
	cell->addChild(label, 1);

	return cell;
}

ssize_t LotteryAddSub::numberOfCellsInTableView(TableView *table)
{
	int result = int(fanDianMax);
	if (result == 28) return 29;
	
	return fanDianMax;
}

void LotteryAddSub::scrollViewDidScroll(ScrollView* view)
{

}

void LotteryAddSub::scrollViewDidZoom(ScrollView* view)
{

}

bool LotteryAddSub::isLegal(string str)
{
	const char* s = str.c_str();
	int i = 0;
	while (i++ < str.length() - 1)
	{
		int a = s[i];
		if (s[i]>255)
		{

		}
		if (s[i] < 0)
		{

		}
		else if (s[i]>='0' && s[i]<='9')
		{

		}
		else if (s[i]>='A' && s[i]<='Z' || s[i]>='a' && s[i]<='z')
		{

		}
		else
		{

			return  false;
		}
	}
	return  true;
}
