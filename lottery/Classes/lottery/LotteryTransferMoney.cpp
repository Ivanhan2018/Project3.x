#include "LotteryTransferMoney.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "EntityMgr.h"
#include "MyNSString.h"
#include "LotterySceneControl.h"
#include "LotterySettingView.h"
#include "BetLimit.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryTransferMoney::scene()
{
	Scene *scene = Scene::create();
	LotteryTransferMoney *layer = LotteryTransferMoney::create();
	scene->addChild(layer);
	return scene;
}

LotteryTransferMoney::LotteryTransferMoney()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryTransferMoney::zhuanHuanRet), "ZhuanHuanRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryTransferMoney::getLastYueRet), "GetLastYueRet", NULL);  ///余额查询

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryTransferMoney::getKeFuUrl), "GetKeFuUrl", NULL);

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryTransferMoney::getNeedRefresh), "NeedRefreshData", NULL);	

	kefuStr = NULL;	
}
LotteryTransferMoney::~LotteryTransferMoney()
{
	NotificationCenter::getInstance()->removeObserver(this, "ZhuanHuanRet");
	NotificationCenter::getInstance()->removeObserver(this, "GetLastYueRet");
	NotificationCenter::getInstance()->removeObserver(this, "GetKeFuUrl");

	NotificationCenter::getInstance()->removeObserver(this, "NeedRefreshData");	
}

void LotteryTransferMoney::getLastYueRet(Object *obj)
{
	Dictionary *dic = (Dictionary *)obj;
	Double* f_t_yue = (Double *)dic->objectForKey("f_t_yue"); 
	Double* f_t_dongjie = (Double *)dic->objectForKey("f_t_dongjie");
	Double* f_t_score = (Double *)dic->objectForKey("f_t_score");

	money = f_t_yue->getValue();
	qiPaiMoney = f_t_score->getValue();

	String *moneyStr = String::createWithFormat("%s%.3lf%s",ConfigMgr::instance()->text("display_text.xml", "t161"),
		money, ConfigMgr::instance()->text("display_text.xml", "t162"));
	String *qiPaiMoneyStr = String::createWithFormat("%s%.3lf%s",ConfigMgr::instance()->text("display_text.xml", "t163"),
		qiPaiMoney, ConfigMgr::instance()->text("display_text.xml", "t162"));
	moneyLabel->setString(moneyStr->getCString());
	qiPaiMoneyLabel->setString(qiPaiMoneyStr->getCString());
}

void LotteryTransferMoney::zhuanHuanRet(Object *obj)
{
	Dictionary *dic = (Dictionary *)obj;

	Double* f_t_yue = (Double *)dic->objectForKey("f_t_yue"); 
	Double* f_t_score = (Double *)dic->objectForKey("f_t_score");
	Integer *nResult = (Integer *)dic->objectForKey("nResult");

	String *str = String::createWithFormat("t%d",948+nResult->getValue());
	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", str->getCString()), Vec2(SCREEN_WIDTH/2, 427));
	this->addChild(layer,20);

	if(nResult->getValue() != 0) return;

	money = f_t_yue->getValue();
	qiPaiMoney = f_t_score->getValue();

	String *moneyStr = String::createWithFormat("%s%.3lf%s",ConfigMgr::instance()->text("display_text.xml", "t161"),
		money, ConfigMgr::instance()->text("display_text.xml", "t162"));
	String *qiPaiMoneyStr = String::createWithFormat("%s%.3lf%s",ConfigMgr::instance()->text("display_text.xml", "t163"),
		qiPaiMoney, ConfigMgr::instance()->text("display_text.xml", "t162"));
	moneyLabel->setString(moneyStr->getCString());
	qiPaiMoneyLabel->setString(qiPaiMoneyStr->getCString());


	transferMoney->setText("0");
	updateUI();
}

void LotteryTransferMoney::setTransferType(int typeTrans)
{
	transferType = typeTrans;
	transferMoney->setText("0");
	updateUI();
}

bool LotteryTransferMoney::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initData();
	this->initView();
	this->setKeypadEnabled(true);
	EntityMgr::instance()->getDispatch()->sendPacketWithGetKeFuURL();
	return true;
}

void LotteryTransferMoney::initData()
{
	winSize = Director::getInstance()->getWinSize();
	//username = g_GlobalUnits.getGolbalUserData()->szNickName;
	username = g_GlobalUnits.getGolbalUserData()->szAccounts;
	money = EntityMgr::instance()->getDispatch()->f_yue;
	qiPaiMoney = EntityMgr::instance()->getDispatch()->f_qipai_yue;
	posX1 = 250;
	posX2 = 280;
	gapY = 60;
	transferType = 0;
	transferMoneyLast = 0;
}

void LotteryTransferMoney::getNeedRefresh(Object* obj)
{
	EntityMgr::instance()->getDispatch()->sendPacketWithGetKeFuURL();
	EntityMgr::instance()->getDispatch()->SendPacketWithGetLastYue();
}

void LotteryTransferMoney::getKeFuUrl(Object *obj)
{
	Dictionary *dic = (Dictionary *)obj;
	if(kefuStr) kefuStr->release();
	Integer* result = (Integer *)dic->objectForKey("result");
	kefuStr = (String* )dic->objectForKey("desc");	
	kefuStr->retain();
}

void LotteryTransferMoney::contactKefu(Object* obj)
{
	playButtonSound();

	if(kefuStr == nullptr)
	{
		kefuStr = String::createWithFormat("%s", ConfigMgr::instance()->text("display_config.xml", "t5000"));
	}

	Application::getInstance()->openURL(BetLimit::GetStrWithoutSpace(kefuStr->getCString()));
}

void LotteryTransferMoney::initView()
{
	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	//顶端背景
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame(HEAD_IMG));
	bk1->setAnchorPoint(Vec2(0,1));
	bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
	bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
	bk1->setScaleY(162 / bk1->getContentSize().height);
	this->addChild(bk1,1);

	Sprite *bk2 = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_00.png"));
	bk2->setAnchorPoint(Vec2(0,1));
	bk2->setPosition(Vec2(0, SCREEN_HEIGHT - bk1->getContentSize().height));
	this->addChild(bk2,1);

	//返回
	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryTransferMoney::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t160"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	fontSize = 30;
	LabelTTF *contactLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t73"), "", fontSize);
	contactLabel->setColor(ccc3(255, 238, 31));
	MenuItemLabel *contactItem = MenuItemLabel::create(contactLabel,CC_CALLBACK_1(LotteryTransferMoney::contactKefu,this));
	contactItem->setPosition(Vec2(SCREEN_WIDTH - 80, SCREEN_HEIGHT-200));
	Menu *contactMenu = Menu::createWithItem(contactItem);
	contactMenu->setPosition(Vec2::ZERO);
	this->addChild(contactMenu, 1);

	ScaleTo *scale1 = ScaleTo::create(1.0, 1.2);
	ScaleTo *scale2 = ScaleTo::create(0.3, 1);
	RepeatForever *repeat = RepeatForever::create(Sequence::create(scale1, scale2, DelayTime::create(2.0), NULL));
	contactItem->runAction(repeat);

	fontSize = 27;
	String *userNameStr = String::createWithFormat("%s%s", ConfigMgr::instance()->text("display_text.xml", "t691"), username.c_str());
	LabelTTF *userNameLabel = LabelTTF::create(userNameStr->getCString(), "", fontSize);
	userNameLabel->setAnchorPoint(Vec2(0,0.5f));
	userNameLabel->setPosition(Vec2(180, SCREEN_HEIGHT - 150 - bk2->getContentSize().height / 5));
	userNameLabel->setColor(ccc3(255,255,255));
	this->addChild(userNameLabel, 1);

	int userId = (int)EntityMgr::instance()->getDispatch()->m_dwUserID;
	String *userIdStr = String::createWithFormat("%s%d", ConfigMgr::instance()->text("display_text.xml", "t692"), userId);
	LabelTTF *userIdLabel = LabelTTF::create(userIdStr->getCString(), "", fontSize);
	userIdLabel->setAnchorPoint(Vec2(0,0.5f));
	userIdLabel->setPosition(Vec2(userNameLabel->getPositionX(), userNameLabel->getPositionY()-bk2->getContentSize().height*1/5));
	userIdLabel->setColor(ccc3(255,255,255));
	this->addChild(userIdLabel, 1);

	String *moneyStr = String::createWithFormat("%s%.3lf%s",ConfigMgr::instance()->text("display_text.xml", "t161"),
		money, ConfigMgr::instance()->text("display_text.xml", "t162"));
	moneyLabel = LabelTTF::create(moneyStr->getCString(), "", fontSize);
	moneyLabel->setAnchorPoint(Vec2(0,0.5f));
	moneyLabel->setPosition(Vec2(userNameLabel->getPositionX(), userNameLabel->getPositionY()-bk2->getContentSize().height*2/5));
	moneyLabel->setColor(ccc3(255,255,255));
	this->addChild(moneyLabel, 1);

	String *qiPaiMoneyStr = String::createWithFormat("%s%.3lf%s",ConfigMgr::instance()->text("display_text.xml", "t163"),
		qiPaiMoney, ConfigMgr::instance()->text("display_text.xml", "t162"));
	qiPaiMoneyLabel = LabelTTF::create(qiPaiMoneyStr->getCString(), "", fontSize);
	qiPaiMoneyLabel->setAnchorPoint(Vec2(0,0.5f));
	qiPaiMoneyLabel->setPosition(Vec2(userNameLabel->getPositionX(), userNameLabel->getPositionY()-bk2->getContentSize().height*3/5));
	qiPaiMoneyLabel->setColor(ccc3(255,255,255));
	this->addChild(qiPaiMoneyLabel, 1);

	//刷新按钮
	ui::Button* btRefresh = ui::Button::create("my_lottery_fresh.png","my_lottery_fresh.png","my_lottery_fresh.png",ui::Widget::TextureResType::LOCAL);
	btRefresh->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btRefresh->setPosition(Vec2(moneyLabel->getPositionX() + moneyLabel->getContentSize().width + 40, moneyLabel->getPositionY()));
	this->addChild(btRefresh, 1);
	btRefresh->addClickEventListener([=](Ref *){
		playButtonSound();
		static bool isRun = false;
		if(isRun) return;
		isRun = true;
		btRefresh->runAction(Sequence::create(RotateTo::create(2.0f,1440),
			CallFunc::create([=](){isRun = false;}),
			nullptr));
		EntityMgr::instance()->getDispatch()->SendPacketWithGetLastYue();
	});

	float menuGap = 10;

	//转彩票 转游戏按钮
	Sprite *toLotteryNormalImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_18_2.png"));
	Sprite *toLotterySelectedImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_19_2.png"));
	Sprite *toQiPaiNormalImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_20_2.png"));
	Sprite *toQiPaiSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_21_2.png"));
	MenuItemSprite *toLotteryItem = MenuItemSprite::create(toLotteryNormalImage,toLotterySelectedImage,CC_CALLBACK_1(LotteryTransferMoney::transferToLottery,this));
	MenuItemSprite *toQiPaiItem = MenuItemSprite::create(toQiPaiNormalImage,toQiPaiSelectedImage,CC_CALLBACK_1(LotteryTransferMoney::transferToQiPai,this));
	Menu *transactionMenu = Menu::create(toLotteryItem,toQiPaiItem,NULL);
	transactionMenu->setPosition(Vec2(SCREEN_WIDTH/2, bk2->getPositionY() - bk2->getContentSize().height - toLotteryNormalImage->getContentSize().height / 2));
	transactionMenu->alignItemsHorizontallyWithPadding(0);
	this->addChild(transactionMenu,1);

	Sprite *bottomBack = Sprite::createWithSpriteFrame(spriteFrame("moving_label_001.png"));
	bottomBack->setScaleX(SCREEN_WIDTH / bottomBack->getContentSize().width);
	bottomBack->setScaleY(toLotteryNormalImage->getContentSize().height / bottomBack->getContentSize().height);
	bottomBack->setPosition(Vec2(SCREEN_WIDTH/2, bk2->getPositionY() - bk2->getContentSize().height - toLotteryNormalImage->getContentSize().height / 2));
	this->addChild(bottomBack, 0);

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, transactionMenu->getPositionY()-80));
	this->addChild(gap1);

	startY = gap1->getPositionY()-30;

	fontSize = 30;
	ccColor3B blackColor = ccc3(76,76,76);
	for(int i = 0 ; i < 3 ; i++){
		String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 941+i)->getCString()));
		if(i == 0){
			transferMoneyTitlelabel = LabelTTF::create(labelStr->getCString(), "", fontSize);
			transferMoneyTitlelabel->setAnchorPoint(Vec2(1,0.5f));
			transferMoneyTitlelabel->setPosition(Vec2(posX1, startY-gapY*i));
			transferMoneyTitlelabel->setColor(blackColor);
			this->addChild(transferMoneyTitlelabel, 1);
			continue;
		}
		LabelTTF *label = LabelTTF::create(labelStr->getCString(), "", fontSize);
		label->setAnchorPoint(Vec2(1,0.5f));
		label->setPosition(Vec2(posX1, startY-gapY*i));
		label->setColor(blackColor);
		this->addChild(label, 1);
	}

	String *moneyStr1 = String::createWithFormat("%.3lf%s",
		qiPaiMoney, ConfigMgr::instance()->text("display_text.xml", "t162"));
	transferMoneylabel = LabelTTF::create(moneyStr1->getCString(), "", fontSize);
	transferMoneylabel->setAnchorPoint(Vec2(0,0.5f));
	transferMoneylabel->setPosition(Vec2(posX2, startY));
	transferMoneylabel->setColor(ccc3(0,0,0));
	this->addChild(transferMoneylabel, 1);

	transferTypelabel = LabelTTF::create("", "", fontSize);
	transferTypelabel->setAnchorPoint(Vec2(0,0.5f));
	transferTypelabel->setPosition(Vec2(posX2, startY-gapY));
	transferTypelabel->setColor(ccc3(0,0,0));
	this->addChild(transferTypelabel, 1);
	updataTransferTypeLabel();

	Sprite *transferMoneyBk = Sprite::createWithSpriteFrame(spriteFrame("recharge_16.png"));
	transferMoneyBk->setAnchorPoint(Vec2(0,0.5f));
	transferMoneyBk->setPosition(Vec2(posX2-5, startY-gapY*2));
	this->addChild(transferMoneyBk, 1);

	transferMoney = EditBox::create(cocos2d::Size(220, 30), "blank.png");
	transferMoney->setPosition(Vec2(posX2, startY-gapY*2));
	transferMoney->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
	transferMoney->setInputMode(EditBox::InputMode::DECIMAL);
	transferMoney->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	transferMoney->setAnchorPoint(Vec2(0,0.5f));
	transferMoney->setFontColor(ccc3(0,0,0));
	transferMoney->setFontName("");transferMoney->setFontSize(fontSize);
	transferMoney->setColor(ccc3(112,112,112));
	transferMoney->setPlaceHolder(ConfigMgr::instance()->text("display_text.xml", "t947"));
	transferMoney->setPlaceholderFontSize(25);
	transferMoney->setMaxLength(20);
	this->addChild(transferMoney, 1);
	transferMoney->setDelegate(this);
	transferMoney->setText("0");


	MenuItemImage *confirmItem = MenuItemImage::create("beting22.png", "beting23.png", CC_CALLBACK_1(LotteryTransferMoney::pressConfirm,this));
	confirmItem->setPosition(Vec2(SCREEN_WIDTH/2, startY - gapY * 4));
	//confirmItem->setScale(0.8f);
	Menu *confirmMenu = Menu::create(confirmItem, NULL);
	confirmMenu->setPosition(Vec2(0,0));
	this->addChild(confirmMenu);
}

void LotteryTransferMoney::pressConfirm(Object *obj)
{
	playButtonSound();

	if (transferMoneyLast == 0) return;

	EntityMgr::instance()->getDispatch()->SendPacketWithZhuanHuanMoney(2-transferType, transferMoneyLast);
	//重置
	transferMoneyLast = 0;
}

void LotteryTransferMoney::updateUI()
{
	String *moneyStr = String::createWithFormat("%s%.3lf%s",ConfigMgr::instance()->text("display_text.xml", "t161"),
		money, ConfigMgr::instance()->text("display_text.xml", "t162"));
	String *qiPaiMoneyStr = String::createWithFormat("%s%.3lf%s",ConfigMgr::instance()->text("display_text.xml", "t163"),
		qiPaiMoney, ConfigMgr::instance()->text("display_text.xml", "t162"));
	moneyLabel->setString(moneyStr->getCString());
	qiPaiMoneyLabel->setString(qiPaiMoneyStr->getCString());
	updataTransferMoneyTitlelabel();
	updataTransferMoneylabel();
	updataTransferTypeLabel();
}

void LotteryTransferMoney::updataTransferMoneyTitlelabel()
{
	if(transferType == 0){
		String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml",
			"t941"));
		transferMoneyTitlelabel->setString(labelStr->getCString());
	}else{
		String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml",
			"t944"));
		transferMoneyTitlelabel->setString(labelStr->getCString());
	}
}

void LotteryTransferMoney::updataTransferMoneylabel()
{
	if(transferType == 0){
		String *labelStr = String::createWithFormat("%.3lf%s",
			qiPaiMoney, ConfigMgr::instance()->text("display_text.xml", "t162"));
		transferMoneylabel->setString(labelStr->getCString());
	}else{
		String *labelStr = String::createWithFormat("%.3lf%s",
			money, ConfigMgr::instance()->text("display_text.xml", "t162"));
		transferMoneylabel->setString(labelStr->getCString());
	}
}

void LotteryTransferMoney::updataTransferTypeLabel()
{
	String *labelStr = String::create(ConfigMgr::instance()->text("display_text.xml",
		String::createWithFormat("t%d", 945+transferType)->getCString()));
	transferTypelabel->setString(labelStr->getCString());
}

void LotteryTransferMoney::onEnter()
{
	Layer::onEnter();
}

void LotteryTransferMoney::onExit()
{
	Layer::onExit();
}

void LotteryTransferMoney::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void LotteryTransferMoney::back(Object *obj)
{
	playButtonSound();
	Director::getInstance()->popScene();
}

void LotteryTransferMoney::transferToLottery(Object* obj)
{
	playButtonSound();

	transferType = 0;
	transferMoney->setText("0");
	updateUI();
}

void LotteryTransferMoney::transferToQiPai(Object *obj)
{
	playButtonSound();

	transferType = 1;
	transferMoney->setText("0");
	updateUI();
}

void LotteryTransferMoney::editBoxEditingDidEnd(EditBox* editBox)
{
	string moneyStr = transferMoney->getText();

	for(int i = 0;i < moneyStr.size();i ++)
	{
		if(moneyStr.at(i) < '0' || moneyStr.at(i) > '9') return;
	}

	DOUBLE moneyInput = atof(moneyStr.c_str());
	if(transferType == 0){
		if(moneyInput > qiPaiMoney){
			moneyInput = qiPaiMoney;
			char temp[100] = {0};
			sprintf(temp, "%.3lf", moneyInput);
			transferMoney->setText(temp);
		}
	}else{
		if(moneyInput > money){
			moneyInput = money;
			char temp[100] = {0};
			sprintf(temp, "%.3lf", moneyInput);
			transferMoney->setText(temp);
		}
	}
	transferMoneyLast = moneyInput;
}
void LotteryTransferMoney::editBoxEditingDidBegin(EditBox* editBox)
{

}