#include "RoomLayer.h"
#include "ConfigMgr.h"
#include "LotteryModifyPassWord.h"
#include "MovingLabelLayer.h"
#include "LotterySettingView.h"
#include "LotterySceneControl.h"
#include "VersionControl.h"
#include "ValidateLayer.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace extension;

Scene* LotteryModifyPassWord::scene()
{
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	LotteryModifyPassWord *layer = LotteryModifyPassWord::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

LotteryModifyPassWord::LotteryModifyPassWord()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryModifyPassWord::onChangePassWordResult), MSG_UI_ANS_PASSWORDUPDATE, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryModifyPassWord::bindPhoneNumberRet), "BindPhoneNumberRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryModifyPassWord::validateTypeRet), "ValidateTypeRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryModifyPassWord::unbindPhoneNumberRet), "UnBindPhoneNumberRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryModifyPassWord::onValidated), "Validated", NULL);
}

LotteryModifyPassWord::~LotteryModifyPassWord()
{
	NotificationCenter::getInstance()->removeObserver(this, MSG_UI_ANS_PASSWORDUPDATE);	
	NotificationCenter::getInstance()->removeObserver(this, "BindPhoneNumberRet");
	NotificationCenter::getInstance()->removeObserver(this, "UnBindPhoneNumberRet");
	NotificationCenter::getInstance()->removeObserver(this, "ValidateTypeRet");
	NotificationCenter::getInstance()->removeObserver(this, "Validated");
}

LotteryModifyPassWord* LotteryModifyPassWord::create(bool bLogin)
{
	LotteryModifyPassWord *pRet = new LotteryModifyPassWord(); 
	if (pRet && pRet->init(bLogin)) 
	{ 
		pRet->autorelease();
		return pRet; 
	} 
	else
	{ 
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}


bool LotteryModifyPassWord::init(bool bLogin)
{

	if ( !Layer::init() )
	{
		return false;
	}

	this->initData();
	this->initView();
	this->setKeypadEnabled(true);
	return true;
}

void LotteryModifyPassWord::initData()
{
	m_isQuerry = false;
}

void LotteryModifyPassWord::initView()
{
	cocos2d::Size winSize = Director::getInstance()->getWinSize();


	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame(HEAD_IMG));
	bk1->setAnchorPoint(Vec2(0,1));
	bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
	bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
	bk1->setScaleY(162 / bk1->getContentSize().height);
	this->addChild(bk1);

	int posX1 = 34 + 20;
	int posXDelta = 250;

	//返回按钮
	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryModifyPassWord::rBack,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton);

	float fontSize = 38;
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t1112"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title);

	fontSize = 27;

	//登录密码和取款密码选择
	int nYPos = SCREEN_HEIGHT - 210;

	auto loginPass = MenuItemImage::create("beting27.png", "beting28.png", CC_CALLBACK_1(LotteryModifyPassWord::pressLoginPass,this));
	auto bankPass = MenuItemImage::create("beting27.png", "beting28.png", CC_CALLBACK_1(LotteryModifyPassWord::pressBankPass,this));
	Menu *selectPassMenu = Menu::create(loginPass, bankPass, NULL);
	selectPassMenu->alignItemsHorizontallyWithPadding(200);
	selectPassMenu->setPosition(Vec2(posX1 + 260,nYPos));
	selectPassMenu->setTag(9527);
	loginPass->setTag(9528);
	bankPass->setTag(9529);
	loginPass->selected();
	this->addChild(selectPassMenu);

	//登录密码 取款密码 文字
	for(int i = 0;i < 2;i ++)
	{
		LabelTTF* labelWord = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", String::createWithFormat("t%d",652 + i)->getCString()), "", fontSize);
		labelWord->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		labelWord->setPosition(Vec2(posX1 + i * 250,nYPos));
		labelWord->setFontFillColor(ccColor3B::BLACK);
		this->addChild(labelWord);
	}

	nYPos -= 80;

	LabelTTF* tipsLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t657"), "", fontSize);
	tipsLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	tipsLabel->setPosition(Vec2(posX1, nYPos));
	tipsLabel->setColor(ccc3(200,0,0));
	tipsLabel->setVerticalAlignment(TextVAlignment::CENTER);
	tipsLabel->setHorizontalAlignment(TextHAlignment::LEFT);
	this->addChild(tipsLabel);

	nYPos -= 80; 
	posXDelta = 140;

	//tips 原始密码 修改密码 确认密码
	for(int j = 0;j < 3;j ++)
	{
		//
		LabelTTF* labelWord = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", String::createWithFormat("t%d",654 + j)->getCString()), "", fontSize);
		labelWord->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		labelWord->setPosition(Vec2(posX1,nYPos));
		labelWord->setFontFillColor(ccColor3B::BLACK);
		this->addChild(labelWord);
		//背景
		Sprite* sptBack = Sprite::createWithSpriteFrame(spriteFrame("recharge_16.png"));
		sptBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		sptBack->setPosition(Vec2(posX1 + posXDelta,nYPos));
		sptBack->setScaleX(280 / sptBack->getContentSize().width);
		sptBack->setScaleY(60 / sptBack->getContentSize().height);
		this->addChild(sptBack);

		//编辑框
		EditBox* editPass = EditBox::create(cocos2d::Size(280,60), "blank.png");	
		editPass->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		editPass->setFontName("");
		editPass->setFontSize(26);
		editPass->setMaxLength(32);
		editPass->setPosition(sptBack->getPosition());
		editPass->setFontColor(ccColor4B::BLACK);
		editPass->setPlaceholderFontColor(ccColor4B::GRAY);
		editPass->setPlaceHolder(ConfigMgr::instance()->text("display_text.xml", "t658"));
		editPass->setPlaceholderFontName("");
		editPass->setPlaceholderFontSize(22);
		editPass->setColor(ccc3(250,250,250));
		editPass->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
		editPass->setTag(8888 + j);
		this->addChild(editPass);

		nYPos -= 80;
	}	
	//修改密码	
	auto btnModify = ui::Button::create("modify_pass_01.png","modify_pass_02.png","modify_pass_01.png");
	btnModify->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	btnModify->addClickEventListener(CC_CALLBACK_1(LotteryModifyPassWord::clickButton,this));
	btnModify->setPosition(Vec2(posX1 + posXDelta, nYPos));
	this->addChild(btnModify);

	//return;

	nYPos -= 80;

	//绑定手机文字文本框
	LabelTTF* lbBind = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t1130"), "", fontSize);
	lbBind->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	lbBind->setPosition(Vec2(posX1,nYPos));
	lbBind->setFontFillColor(ccColor3B::BLACK);
	this->addChild(lbBind);

	//背景
	Sprite* sptBindBack = Sprite::createWithSpriteFrame(spriteFrame("recharge_16.png"));
	sptBindBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	sptBindBack->setPosition(Vec2(posX1 + posXDelta,nYPos));
	sptBindBack->setScaleX(280 / sptBindBack->getContentSize().width);
	sptBindBack->setScaleY(60 / sptBindBack->getContentSize().height);
	this->addChild(sptBindBack);

	//编辑框
	EditBox* editBindBox = EditBox::create(cocos2d::Size(280,60), "blank.png");	
	editBindBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	editBindBox->setPosition(sptBindBack->getPosition());
	editBindBox->setFontName("");
	editBindBox->setFontSize(26);
	editBindBox->setMaxLength(11);
	editBindBox->setFontColor(ccColor4B::BLACK);
	editBindBox->setPlaceholderFontColor(ccColor4B::GRAY);
	editBindBox->setPlaceHolder(ConfigMgr::instance()->text("display_text.xml", "t1135"));
	editBindBox->setPlaceholderFontName("");
	editBindBox->setPlaceholderFontSize(22);
	editBindBox->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
	editBindBox->setInputMode(EditBox::InputMode::NUMERIC);
	editBindBox->setTag(9999);
	this->addChild(editBindBox);

	nYPos -= 80;
	//绑定手机按钮
	auto btnBind = ui::Button::create("my_lottery_33.png","my_lottery_33_2.png","my_lottery_33.png");
	btnBind->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	btnBind->setPosition(Vec2(posX1 + posXDelta,nYPos));
	btnBind->addClickEventListener(CC_CALLBACK_1(LotteryModifyPassWord::toBind,this));
	btnBind->setTag(8383);
	this->addChild(btnBind);

	//解除绑定手机按钮
	auto btnUnBind = ui::Button::create("my_lottery_36.png","my_lottery_36_2.png","my_lottery_36.png");
	btnUnBind->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	btnUnBind->setPosition(Vec2(posX1 + posXDelta,nYPos));
	btnUnBind->addClickEventListener(CC_CALLBACK_1(LotteryModifyPassWord::toUnBind,this));
	btnUnBind->setTag(8484);
	this->addChild(btnUnBind);	
	btnUnBind->setVisible(false);

	nYPos -= 80;
	//绑定设置文字和选项
	LabelTTF* lbSet = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t1131"), "", fontSize);
	lbSet->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	lbSet->setPosition(Vec2(posX1,nYPos));
	lbSet->setFontFillColor(ccColor3B::BLACK);
	this->addChild(lbSet);

	for(int k = 0;k < 3;k ++)
	{
		auto btnCheck = ui::Button::create("beting27.png","beting28.png","beting27.png");
		btnCheck->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		btnCheck->setPosition(Vec2(posX1 + posXDelta + k * 160,nYPos));
		btnCheck->setTag(6666 + k);
		this->addChild(btnCheck);
		btnCheck->setBright(true);
		btnCheck->addClickEventListener([=](Ref *obj){
			auto btnCheck = static_cast<ui::Button *>(obj);
			if(btnCheck == nullptr) return;				
			//设置状态
			btnCheck->setBright(!btnCheck->isBright());
			setButtonChecked(btnCheck);
		});

		LabelTTF* lbSet = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", String::createWithFormat("t%d", 1132 + k)->getCString()), "", fontSize);
		lbSet->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		lbSet->setPosition(Vec2(posX1 + posXDelta + 50 + k * 160,nYPos));
		lbSet->setFontFillColor(ccColor3B::BLACK);
		this->addChild(lbSet);	

		//屏蔽
		if(k == 2)
		{
			btnCheck->setEnabled(false);			
		}
	}

	nYPos -= 80;

	//确定按钮
	auto btnSub = ui::Button::create("my_lottery_35.png","my_lottery_35_2.png","my_lottery_35_3.png");
	btnSub->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	btnSub->setPosition(Vec2(posX1 + posXDelta,nYPos));
	btnSub->addClickEventListener([=](Ref* ){			
		//
		ui::Button* btLogin = (ui::Button *)this->getChildByTag(6666);
		ui::Button* btTrans = (ui::Button *)this->getChildByTag(6666 + 1);
		ui::Button* btFen = (ui::Button *)this->getChildByTag(6666 + 2);
		if(btLogin == nullptr || btTrans == nullptr || btFen == nullptr) return;
		//发送验证
		EntityMgr::instance()->getDispatch()->SendPacketWithSetValidateType(btFen->isBright()?2:1,btLogin->isBright()?2:1,btTrans->isBright()?2:1);
	});

	btnSub->setTag(8585);
	this->addChild(btnSub);

	//开始查询
	m_isQuerry = true;
	EntityMgr::instance()->getDispatch()->SendPacketWithGetValidateType();
}

void LotteryModifyPassWord::pressLoginPass(Object *obj)
{
	playButtonSound();
	auto loginPass = (MenuItemSprite *)this->getChildByTag(9527)->getChildByTag(9528);
	if(loginPass == nullptr) return;
	auto bankPass = (MenuItemSprite *)this->getChildByTag(9527)->getChildByTag(9529);
	if(bankPass == nullptr) return;

	loginPass->selected();
	bankPass->unselected();
}

void LotteryModifyPassWord::pressBankPass(Object *obj)
{
	playButtonSound();
	auto loginPass = (MenuItemSprite *)this->getChildByTag(9527)->getChildByTag(9528);
	if(loginPass == nullptr) return;
	auto bankPass = (MenuItemSprite *)this->getChildByTag(9527)->getChildByTag(9529);
	if(bankPass == nullptr) return;

	loginPass->unselected();
	bankPass->selected();
}

void LotteryModifyPassWord::toBind(Object *obj)
{
	playButtonSound();

	auto editNumber = (EditBox *)this->getChildByTag(9999);
	if(editNumber == nullptr) return;

	std::string tempStr = editNumber->getText();
	if(tempStr.size() != 11)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t5040"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,255);
		return;
	}

	ValidateLayer::show(editNumber->getText(),1);	
}

void LotteryModifyPassWord::toUnBind(Object *obj)
{
	playButtonSound();

	auto editNumber = (EditBox *)this->getChildByTag(9999);
	if(editNumber == nullptr) return;

	std::string tempStr = editNumber->getText();
	if(tempStr.size() != 11)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t5040"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,255);
		return;
	}

	ValidateLayer::show(tempStr.c_str(),2);	
}

void LotteryModifyPassWord::clickButton(Object* obj)
{
	playButtonSound();

	auto orginPasswordInput = (ui::EditBox *)this->getChildByTag(8888);
	auto modifyPasswordInput = (ui::EditBox *)this->getChildByTag(8888 + 1);
	auto surePasswordInput = (ui::EditBox *)this->getChildByTag(8888 + 2);

	const char* szNewPassword = modifyPasswordInput->getText();
	const char* szSurePassword = surePasswordInput->getText();
	const char* szOldPassword = orginPasswordInput->getText();

	if (strcmp(szNewPassword,szSurePassword))
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t662"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,20);
		return;
	}

	//if (!isLegal(szNewPassword))
	//{		
	//	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t659"), Vec2(SCREEN_WIDTH/2, 427));
	//	this->addChild(layer,20);
	//	return;
	//}

	if(strlen(szNewPassword) > 32 || strlen(szNewPassword) < 6)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t659"), Vec2(SCREEN_WIDTH/2, 427));
		this->addChild(layer,20);
		return;
	}
	bool isLoginPass = false;

	auto loginPass = (MenuItemSprite *)this->getChildByTag(9527)->getChildByTag(9528);
	if(loginPass == nullptr) return;

	if (loginPass->isSelected()) {
		isLoginPass = true;
	}
	//判断是否还连网
	bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
	if(bRet)
	{
		EntityMgr::instance()->getDispatch()->modifyPassWord(szOldPassword, szNewPassword, isLoginPass);
	}
}

void LotteryModifyPassWord::rBack(Object* obj)
{
	playButtonSound();

	Director::getInstance()->popScene();
}

void LotteryModifyPassWord::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		rBack(NULL);
		event->stopPropagation();
	}
}

void LotteryModifyPassWord::onChangePassWordResult(Object* obj)
{
	Integer* pNickNameResult = (Integer*)obj;
	int i = pNickNameResult->getValue();
	if(i < 0 || i > 2){
		i = 2;
	}
	if(i == 0)
	{
		auto modifyPasswordInput = (ui::EditBox *)this->getChildByTag(8888 + 1);
		if(modifyPasswordInput != nullptr)
		{
			const char* szNewPassword = modifyPasswordInput->getText();
			UserDefault::getInstance()->setStringForKey("NEWZJD_PASSWORD",szNewPassword);
		}
	}
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml",
		String::createWithFormat("t%d", 667 + i)->getCString()), Vec2(SCREEN_WIDTH/2, 427));
	this->addChild(layer);
}

void LotteryModifyPassWord::bindPhoneNumberRet(Object *obj)
{
	Integer* result = (Integer *)obj;
	if(result == nullptr) return;

	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml",
		String::createWithFormat("t%d", 5019 + result->getValue())->getCString()), Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT / 2));
	this->addChild(layer);
	//8383 9999
	if(result->getValue() == 1)
	{
		auto btBind = (ui::Button*)this->getChildByTag(8383);
		auto btUnBind = (ui::Button*)this->getChildByTag(8484);
		auto dtPhone = (ui::EditBox*)this->getChildByTag(9999);
		if(btBind == nullptr || btUnBind == nullptr || dtPhone == nullptr) return;
		btBind->setVisible(false);
		btUnBind->setVisible(true);
		dtPhone->setEnabled(false);		
	}
}

void LotteryModifyPassWord::unbindPhoneNumberRet(Object *obj)
{
	Dictionary* dic = (Dictionary *)obj;
	if(dic == nullptr) return;

	Integer* result = (Integer *)dic->objectForKey("nResult");
	String* desc = (String *)dic->objectForKey("sDesc");
	
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(desc->getCString(), Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT / 2));
	this->addChild(layer);
	//8484 9999
	if(result->getValue() == 0)
	{
		auto btBind = (ui::Button*)this->getChildByTag(8383);
		auto btUnBind = (ui::Button*)this->getChildByTag(8484);
		auto dtPhone = (ui::EditBox*)this->getChildByTag(9999);
		if(btBind == nullptr || btUnBind == nullptr || dtPhone == nullptr) return;
		btBind->setVisible(true);
		btUnBind->setVisible(false);
		dtPhone->setEnabled(true);	
		dtPhone->setText("");
	}
}

void LotteryModifyPassWord::validateTypeRet(Object *obj)
{
	Dictionary* data = (Dictionary *)obj;
	if(data == nullptr) return;
	//
	Integer* nResult = (Integer *)data->objectForKey("nResult");
	Integer* cfenhong = (Integer *)data->objectForKey("cfenhong");
	Integer* cLoginverify = (Integer *)data->objectForKey("cLoginverify");
	Integer* cTranferAccount = (Integer *)data->objectForKey("cTranferAccount");
	String* phone = (String *)data->objectForKey("phone");
	//检查返回值
	if(!m_isQuerry)
	{
		if(nResult->getValue() != 0)
		{
			//提示设置失败
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t5027"), Vec2(SCREEN_WIDTH/2, 427));
			this->addChild(layer,255);			
			return;
		}else
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t5028"), Vec2(SCREEN_WIDTH/2, 427));
			this->addChild(layer,255);	
		}		
	}else
	{
		ui::EditBox* dtPhone = (ui::EditBox *)this->getChildByTag(9999);
		ui::Button* btnBind = (ui::Button *)this->getChildByTag(8383);
		ui::Button* btnUnBind = (ui::Button *)this->getChildByTag(8484);

		if(dtPhone == nullptr || btnBind == nullptr || btnUnBind == nullptr) return;

		//手机号码单独处理
		if(phone == nullptr || phone->length() < 1)
		{
			dtPhone->setText("");
			btnBind->setVisible(true);
			btnUnBind->setVisible(false);
		}else
		{
			dtPhone->setText(phone->getCString());
			btnBind->setVisible(false);
			btnUnBind->setVisible(true);
		}

		dtPhone->setEnabled(phone == nullptr || phone->length() < 1);

		//修改初始值

		ui::Button* btLogin = (ui::Button *)this->getChildByTag(6666);
		ui::Button* btTrans = (ui::Button *)this->getChildByTag(6666 + 1);
		ui::Button* btFen = (ui::Button *)this->getChildByTag(6666 + 2);

		if(btLogin == nullptr || btTrans == nullptr || btFen == nullptr) return;
		btLogin->setBright(cLoginverify->getValue() == 0 || cLoginverify->getValue() == 2);
		btTrans->setBright(cTranferAccount->getValue() == 0 || cTranferAccount->getValue() == 2);
		btFen->setBright(cfenhong->getValue() == 0 || cfenhong->getValue() == 2);

		//重新设置状态
		setButtonChecked(btLogin);
		setButtonChecked(btTrans);
		setButtonChecked(btFen);
	}

	m_isQuerry = false;			
}

void LotteryModifyPassWord::onValidated(Object* obj)
{
	Integer* tempObj = (Integer *)obj;
	if(tempObj->getValue() == 1)
	{
		auto editNumber = (EditBox *)this->getChildByTag(9999);
		if(editNumber == nullptr) return;
		EntityMgr::instance()->getDispatch()->SendPacketWithBindMobileNum(editNumber->getText());
	}else
	if(tempObj->getValue() == 2)
	{
		auto editNumber = (EditBox *)this->getChildByTag(9999);
		if(editNumber == nullptr) return;
		EntityMgr::instance()->getDispatch()->SendPacketWithUnBindMobileNum();
	}

}

void LotteryModifyPassWord::sendValidateCode(Object *obj)
{
	Integer* tempInt = (Integer *)obj;
	if(tempInt == nullptr) return;

	if(tempInt->getValue() == 1)
	{
		toBind(nullptr);
	}else
	if(tempInt->getValue() == 2)
	{
		toUnBind(nullptr);
	}
};

bool LotteryModifyPassWord::isLegal(string str)
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

void LotteryModifyPassWord::setButtonChecked(Object *obj)
{
	ui::Button* btnCheck = static_cast<ui::Button* >(obj);
	if(btnCheck == nullptr) return;
	bool isBright = btnCheck->isBright();
	if(isBright)
	{
		btnCheck->loadTextures("beting27.png","beting28.png","beting27.png");
	}else
	{
		btnCheck->loadTextures("beting28.png","beting27.png","beting28.png");
	}
}