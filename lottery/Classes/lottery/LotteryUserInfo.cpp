#include "LotteryUserInfo.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "EntityMgr.h"
#include "LotterySceneControl.h"
#include "RoomLayer.h"
#include "LoginLayer.h"
#include "MyNSString.h"

#include "LotteryRecharge.h"
#include "LotteryWithdraw.h"

#include "LotteryBasicUserInfo.h"
#include "LotteryModifyPassWord.h"
#include "LotteryCardBinding.h"
#include "LotteryMyLot.h"

#include "LotteryRecordRecharge.h"
#include "LotteryRecordWithdraw.h"
#include "LotteryRecordProfit.h"
#include "LotteryRecordProfitStatics.h"
#include "LotteryRecordGame.h"

#include "LotteryAddSub.h"
#include "LotteryRecordSubGambleStatics.h"
#include "LotteryRecordSubGamble.h"
#include "LotteryMemberShip.h"
#include "LotteryActiveCenter.h"
#include "LotteryMessageCenter.h"

#include "LotteryRecordSubProfitStatics.h"
#include "LotteryRecordSubProfit.h"

#include "LotteryRecordSubGameProfit.h"
#include "LotteryRecordSubGame.h"
#include "LotteryTransferMoney.h"

#include "BetLimit.h"
#include "VersionControl.h"

using namespace CocosDenshion;

//个人用户中心
Scene* LotteryUserInfo::scene()
{
	Scene *scene = Scene::create();
	LotteryUserInfo *layer = LotteryUserInfo::create();
	scene->addChild(layer);
	return scene;
}

LotteryUserInfo::LotteryUserInfo()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryUserInfo::getLastYueRet), "GetLastYueRet", NULL);  ///余额查询
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryUserInfo::zhuanHuanRet), "ZhuanHuanRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryUserInfo::exitReturn), "ExitReturn", NULL); //退出
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryUserInfo::getKeFuUrl), "GetKeFuUrl", NULL);

	kefuStr = NULL;	
}
LotteryUserInfo::~LotteryUserInfo()
{
	//userInfoTable->release();
	NotificationCenter::getInstance()->removeObserver(this, "GetLastYueRet");
	NotificationCenter::getInstance()->removeObserver(this, "ZhuanHuanRet");
	NotificationCenter::getInstance()->removeObserver(this,"ExitReturn");
	NotificationCenter::getInstance()->removeObserver(this, "GetKeFuUrl");
}

bool LotteryUserInfo::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initData();
	this->initView();
	EntityMgr::instance()->getDispatch()->SendPacketWithGetLastYue();
	EntityMgr::instance()->getDispatch()->sendPacketWithGetKeFuURL();
	this->setKeypadEnabled(true);
	return true;
}

void LotteryUserInfo::getLastYueRet(Object *obj)
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

	LabelTTF* moneyLabel = (LabelTTF *)this->getChildByTag(9527);
	if(moneyLabel) moneyLabel->setString(moneyStr->getCString());
	LabelTTF* qiPaiMoneyLabel = (LabelTTF *)this->getChildByTag(9528);
	if(qiPaiMoneyLabel) qiPaiMoneyLabel->setString(qiPaiMoneyStr->getCString());
}

void LotteryUserInfo::zhuanHuanRet(Object *obj)
{
	Dictionary *dic = (Dictionary *)obj;

	Double* f_t_yue = (Double *)dic->objectForKey("f_t_yue"); 
	Double* f_t_score = (Double *)dic->objectForKey("f_t_score");

	money = f_t_yue->getValue();
	qiPaiMoney = f_t_score->getValue();

	String *moneyStr = String::createWithFormat("%s%.3lf%s",ConfigMgr::instance()->text("display_text.xml", "t161"),
		money, ConfigMgr::instance()->text("display_text.xml", "t162"));
	String *qiPaiMoneyStr = String::createWithFormat("%s%.3lf%s",ConfigMgr::instance()->text("display_text.xml", "t163"),
		qiPaiMoney, ConfigMgr::instance()->text("display_text.xml", "t162"));

	LabelTTF* moneyLabel = (LabelTTF *)this->getChildByTag(9527);
	if(moneyLabel) moneyLabel->setString(moneyStr->getCString());
	LabelTTF* qiPaiMoneyLabel = (LabelTTF *)this->getChildByTag(9528);
	if(qiPaiMoneyLabel) qiPaiMoneyLabel->setString(qiPaiMoneyStr->getCString());
}

void LotteryUserInfo::exitReturn(Object *obj)
{
	//有可能同时接收到几个消息
	static bool isRun = false;
	if (isRun) return;
	isRun = true;

	//开始退出
	Scene *scene = Scene::create();
	LoginLayer *layer = LoginLayer::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
	//还原
	isRun = false;
}

void LotteryUserInfo::initData()
{
	winSize = Director::getInstance()->getWinSize();
	//username = g_GlobalUnits.getGolbalUserData()->szNickName;
	username = g_GlobalUnits.getGolbalUserData()->szAccounts;
	money = EntityMgr::instance()->getDispatch()->f_yue;
	qiPaiMoney = EntityMgr::instance()->getDispatch()->f_qipai_yue;
	cellHeight = 80;

	cellNum = 10; 
}

void LotteryUserInfo::getKeFuUrl(Object *obj)
{
	Dictionary *dic = (Dictionary *)obj;
	if(kefuStr) kefuStr->release();
	Integer* result = (Integer *)dic->objectForKey("result");
	kefuStr = (String* )dic->objectForKey("desc");
	kefuStr->retain();
}

void LotteryUserInfo::contactKefu(Object* obj)
{	
	playButtonSound();

	if(kefuStr == nullptr)
	{
		kefuStr = String::createWithFormat("%s", ConfigMgr::instance()->text("display_config.xml", "t5000"));
	}
	
	Application::getInstance()->openURL(BetLimit::GetStrWithoutSpace(kefuStr->getCString()));
}

void LotteryUserInfo::initView()
{
	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	//±≥æ∞
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

	//顶部返回按钮
	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryUserInfo::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	ccColor3B blackColor = ccc3(76,76,76);

	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t670"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	fontSize = 27;
	LabelTTF *contactLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t73"), "", fontSize);
	contactLabel->setColor(ccc3(255, 238, 31));
	MenuItemLabel *contactItem = MenuItemLabel::create(contactLabel,CC_CALLBACK_1(LotteryUserInfo::contactKefu,this));
	contactItem->setPosition(Vec2(SCREEN_WIDTH - 80, SCREEN_HEIGHT - 200));
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
    userIdLabel->setPosition(Vec2(userNameLabel->getPositionX(), userNameLabel->getPositionY()-bk2->getContentSize().height/5));
    userIdLabel->setColor(ccc3(255,255,255));
    this->addChild(userIdLabel, 1);
    
    String *moneyStr = String::createWithFormat("%s%.3lf%s",ConfigMgr::instance()->text("display_text.xml", "t161"),
                                                    money, ConfigMgr::instance()->text("display_text.xml", "t162"));
    LabelTTF *moneyLabel = LabelTTF::create(moneyStr->getCString(), "", fontSize);
    moneyLabel->setAnchorPoint(Vec2(0,0.5f));
    moneyLabel->setPosition(Vec2(userNameLabel->getPositionX(), userNameLabel->getPositionY()-bk2->getContentSize().height*2/5));
    moneyLabel->setColor(ccc3(255,255,255));
	moneyLabel->setTag(9527);
    this->addChild(moneyLabel, 1);

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
	
	String *qiPaiMoneyStr = String::createWithFormat("%s%.3lf%s",ConfigMgr::instance()->text("display_text.xml", "t163"),
		qiPaiMoney, ConfigMgr::instance()->text("display_text.xml", "t162"));
	LabelTTF *qiPaiMoneyLabel = LabelTTF::create(qiPaiMoneyStr->getCString(), "", fontSize);
	qiPaiMoneyLabel->setAnchorPoint(Vec2(0,0.5f));
	qiPaiMoneyLabel->setPosition(Vec2(userNameLabel->getPositionX(), userNameLabel->getPositionY()-bk2->getContentSize().height*3/5));
	qiPaiMoneyLabel->setColor(ccc3(255,255,255));
	qiPaiMoneyLabel->setTag(9528);
	this->addChild(qiPaiMoneyLabel, 1);

	float menuGap = 10;
	
	Sprite *rechargeNormalImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_02.png"));
	Sprite *rechargeSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_03.png"));
	Sprite *withdrawNormalImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_04.png"));
	Sprite *withdrawSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_05.png"));
	Sprite *toLotteryNormalImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_18.png"));
	Sprite *toLotterySelectedImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_19.png"));
	Sprite *toQiPaiNormalImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_20.png"));
	Sprite *toQiPaiSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_21.png"));
	MenuItemSprite *rechargeItem = MenuItemSprite::create(rechargeNormalImage,rechargeSelectedImage,CC_CALLBACK_1(LotteryUserInfo::recharge,this));
	MenuItemSprite *withdrawItem = MenuItemSprite::create(withdrawNormalImage,withdrawSelectedImage,CC_CALLBACK_1(LotteryUserInfo::withdraw,this));
	MenuItemSprite *toLotteryItem = MenuItemSprite::create(toLotteryNormalImage,toLotterySelectedImage,CC_CALLBACK_1(LotteryUserInfo::transferToLottery,this));
	MenuItemSprite *toQiPaiItem = MenuItemSprite::create(toQiPaiNormalImage,toQiPaiSelectedImage,CC_CALLBACK_1(LotteryUserInfo::transferToQiPai,this));
	Menu *transactionMenu = Menu::create(toLotteryItem,toQiPaiItem,rechargeItem,withdrawItem,NULL);
	transactionMenu->setPosition(Vec2(SCREEN_WIDTH/2, bk2->getPositionY() - bk2->getContentSize().height - rechargeNormalImage->getContentSize().height / 2));
	transactionMenu->alignItemsHorizontallyWithPadding(0);
	this->addChild(transactionMenu,1);

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	int nYPos = transactionMenu->getPositionY() - rechargeNormalImage->getContentSize().height / 2 - menuGap;
	gap1->setPosition(Vec2(0, nYPos));
	gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
	this->addChild(gap1,1);

	cocos2d::Size tableViewSize;
	tableViewSize.width = winSize.width;
	tableViewSize.height = gap1->getPositionY();
	auto userInfoTable = TableView::create(this, tableViewSize);
	userInfoTable->setDelegate(this);
	userInfoTable->setAnchorPoint(Vec2(0.5, 0.5));
	userInfoTable->setPosition(Vec2(0, 0));
	userInfoTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	userInfoTable->setDirection(ScrollView::Direction::VERTICAL);	
	this->addChild(userInfoTable, 1);
}

void LotteryUserInfo::onEnter()
{
	BaseLayer::onEnter();
}

void LotteryUserInfo::onExit()
{
	BaseLayer::onExit();
}

void LotteryUserInfo::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void LotteryUserInfo::back(Object *obj)
{
	playButtonSound();
	Scene *scene = Scene::create();
	RoomLayer *layer = RoomLayer::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

//transaction menu
void LotteryUserInfo::transferToLottery(Object* obj)
{
	playButtonSound();
	Scene *scene = Scene::create();
	LotteryTransferMoney *layer = LotteryTransferMoney::create();
	layer->setTransferType(0);
	scene->addChild(layer);
	Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));

	clear();
}

void LotteryUserInfo::transferToQiPai(Object *obj)
{
	playButtonSound();
	Scene *scene = Scene::create();
	LotteryTransferMoney *layer = LotteryTransferMoney::create();
	layer->setTransferType(1);
	scene->addChild(layer);
	Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));

	clear();
}

void LotteryUserInfo::recharge(Object* obj)
{
	playButtonSound();

	//if(showRecordInfo == true){
	//	return;
	//}
	Scene *scene = LotteryRecharge::scene();
	Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

void LotteryUserInfo::withdraw(Object *obj)
{
	playButtonSound();

	Scene *scene = LotteryWithdraw::scene();
	Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));

	clear();
}

cocos2d::Size LotteryUserInfo::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = winSize.width;
	size.height = 100;//cellHeight;
	return size;
}

ssize_t LotteryUserInfo::numberOfCellsInTableView(TableView *table)
{
	return cellNum+2;
}
TableViewCell *LotteryUserInfo::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
    cell->autorelease();
    cell->setTag(index);
	if (index > 65535)
		return cell;


	ccColor3B redColor = ccc3(61,8,40);
	ccColor3B blackColor = ccc3(150,150,150);
	ccColor3B blackColor1 = ccc3(0,0,0);
	float fontSize = 30;
	float gap = 120;
	float textWidth = 200;

#ifdef VER_369
	redColor = ccc3(50, 62, 123);
#endif

	int cellHeight = 100;

    if(index >= cellNum)
	{
        if(index == cellNum + 1){
            Sprite * loginOutNormalSprite = Sprite::createWithSpriteFrame(spriteFrame("setting_02.png"));
            Sprite * loginOutSelectSprite = Sprite::createWithSpriteFrame(spriteFrame("setting_03.png"));
            MenuItemSprite *loginOutItemSprite = MenuItemSprite::create(loginOutNormalSprite, loginOutSelectSprite, CC_CALLBACK_1(LotteryUserInfo::loginOut,this));
            Menu *loginOutMenu = Menu::create(loginOutItemSprite,NULL);
			loginOutMenu->setPosition(Vec2(SCREEN_WIDTH/2, cellHeight));
            cell->addChild(loginOutMenu, 1);
        }
        return cell;
    }

	//处理列表的图标
	Texture2D *pTexture = TextureCache::getInstance()->addImage("logo_user.png");
	cocos2d::Rect logoRect = cocos2d::Rect((index)* 59, 0, 59, 59);

#ifdef VER_369
	logoRect = cocos2d::Rect((index)* 64, 0, 64, 63);
#endif

	if (index < 10 && pTexture)
	{
		Sprite *ballSprite = Sprite::createWithSpriteFrame(
			SpriteFrame::createWithTexture(pTexture, logoRect));
		//ballSprite->setAnchorPoint(Vec2(0, 0.5f));
		ballSprite->setPosition(Vec2(gap - 40, cellHeight / 2));
		cell->addChild(ballSprite, 1);
	}
	LabelTTF *timeLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", 
		String::createWithFormat("t%d", 1111 +index)->getCString())
		,"",fontSize,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	timeLabel->setColor(blackColor1);
	timeLabel->setAnchorPoint(Vec2(0,0.5f));
	timeLabel->setPosition(Vec2(gap, cellHeight/2));
	cell->addChild(timeLabel, 1);	

	Sprite *more = Sprite::createWithSpriteFrame(spriteFrame("setting_01.png"));
	more->setAnchorPoint(Vec2(0,0.5f));
	more->setPosition(Vec2(SCREEN_WIDTH-20, cellHeight / 2));
	cell->addChild(more,3);

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5f));
	gap1->setPosition(Vec2(0, 0));
	gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);	
	cell->addChild(gap1, 3);

	return cell;  
}

void LotteryUserInfo::tableCellTouched(TableView *table, TableViewCell *cell)
{
	int cellTag = cell->getTag();
	switch(cellTag)
	{
	case 0:
		{
			playButtonSound();
			Scene *scene = LotteryBasicUserInfo::scene();
			Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;
		}
	case 1:
		{
			playButtonSound();
			Scene *scene = LotteryModifyPassWord::scene();
			Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;
		}
	case 2:
		{
			playButtonSound();
			Scene *scene = LotteryCardBinding::scene();
			Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;
		}
	case 3:
		{
			playButtonSound();
			Scene *scene = LotteryMyLot::scene();
			Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;
		}
	case 4:
		{
			playButtonSound();
			Scene *scene = LotteryRecordWithdraw::scene();
			Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;
		}
	case 5:
		{
			playButtonSound();
			Scene *scene = LotteryRecordRecharge::scene();
			Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;
		}
	case 6:
		{
			playButtonSound();
			Scene *scene = LotteryRecordProfit::scene(); //盈亏统计
			Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;
		}
	case 7:
		{
			playButtonSound();
			Scene *scene = LotteryRecordGame::scene();//棋牌记录
			Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));

			clear();
			break;
		}
	case 8:
        {
			playButtonSound();
            Scene *scene = LotteryActiveCenter::scene();
            Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));

			clear();
			break;
		}
	case 9:
        {		
			playButtonSound();
            Scene *scene = LotteryMessageCenter::scene();
            Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
			break;
		}
	case 10:
        {

			break;
		}
	case 11:
        {

			break;
		}	
	default:
		break;
	}
}

void LotteryUserInfo::scrollViewDidScroll(ScrollView* view)
{

}
void LotteryUserInfo::scrollViewDidZoom(ScrollView* view)
{

}

void LotteryUserInfo::loginOut(Object *obj)
{
	playButtonSound();

	EntityMgr::instance()->getDispatch()->SendExitGame();

	this->scheduleOnce([](float dt){
		NotificationCenter::getInstance()->postNotification("ExitReturn");
	}, 1.5f, "ExitAccount");
}

void LotteryUserInfo::clear()
{
	NotificationCenter::getInstance()->removeObserver(this, "GetLastYueRet");
	NotificationCenter::getInstance()->removeObserver(this, "ZhuanHuanRet");
	NotificationCenter::getInstance()->removeObserver(this,"ExitReturn");
	NotificationCenter::getInstance()->removeObserver(this, "GetKeFuUrl");
}