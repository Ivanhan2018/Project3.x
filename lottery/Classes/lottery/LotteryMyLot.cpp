#include "LotteryMyLot.h"
#include "RoomLayer.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"
#include "MyNSString.h"

#include "LotterySettingView.h"
#include "DBHandler.h"
#include "LotteryMyLotRecordInfo.h"
#include "LotteryRecharge.h"
#include "LotteryWithdraw.h"
#include "LotterySceneControl.h"
#include "LotteryTransferMoney.h"
#include "BetLimit.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryMyLot::scene()
{
	Scene *scene = Scene::create();
	LotteryMyLot *layer = LotteryMyLot::create();
	scene->addChild(layer);
	return scene;
}

LotteryMyLot::LotteryMyLot()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryMyLot::getLastYueRet), "GetLastYueRet", NULL);  ///余额查询
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryMyLot::getTouZhuLogCount), "TouZhuLogCount", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryMyLot::getTouZhuLog), "TouZhuLog", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryMyLot::removeRecordInfoLayer), "RemoveRecordInfoLayer", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryMyLot::zhuanHuanRet), "ZhuanHuanRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryMyLot::getKeFuUrl), "GetKeFuUrl", NULL);
	
	touZhuData = Array::create();
	kefuStr = NULL;

	touZhuData->retain();
}

LotteryMyLot::~LotteryMyLot()
{
	clear();
}

void LotteryMyLot::getLastYueRet(Object *obj)
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

void LotteryMyLot::zhuanHuanRet(Object *obj)
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
	moneyLabel->setString(moneyStr->getCString());
	qiPaiMoneyLabel->setString(qiPaiMoneyStr->getCString());
}

void LotteryMyLot::getKeFuUrl(Object *obj)
{
	Dictionary *dic = (Dictionary *)obj;
	if(kefuStr) kefuStr->release();
	Integer* result = (Integer *)dic->objectForKey("result");
	kefuStr = (String* )dic->objectForKey("desc");	
	kefuStr->retain();
}

void LotteryMyLot::removeRecordInfoLayer(Object *obj)
{
	showRecordInfo = false;
}

void LotteryMyLot::getTouZhuLog(Object *obj)
{
	Array *data = (Array *)obj;
	touZhuData->removeAllObjects();
	if(data->count()>pageSize*20){
		for (int i = 0 ; i < pageSize*20 ; i++) {
			touZhuData->addObject(data->objectAtIndex(i));
		}
	}else{
		touZhuData->addObjectsFromArray(data);
	}

	auto touZhuTable = (TableView *)this->getChildByTag(2563);

	if (touZhuTable)
		touZhuTable->reloadData();
	this->refreshTouZhuLogLabel();
}


void LotteryMyLot::getTouZhuLogCount(Object *obj)
{
	touZhuLogCount = ((Integer *)obj)->getValue();
	totalPage = touZhuLogCount%pageSize!=0?(touZhuLogCount/pageSize+1):touZhuLogCount/pageSize;
	if (totalPage>20) {
		totalPage = 20;
	}
	if(touZhuLogCount != 0){
		time_t t = time(0);
		char timeStr[64] = {0};
		char startTimeStr[64] = {0};
		strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S.999",localtime(&t) );
		strftime(startTimeStr, sizeof(startTimeStr), "%Y-%m-%d 00:00:00.000",localtime(&t) );
		EntityMgr::instance()->getDispatch()->SendPacketWithGetTouZhuLog(1, pageSize, true, startTimeStr,timeStr);
	}
}

void LotteryMyLot::refreshTouZhuLogLabel()
{
	if(totalPage<1){
		return;
	}
	String *logCountStr = String::createWithFormat("%s%d%s  %s%d%s",ConfigMgr::instance()->text("display_text.xml", "t556"),
		currentPage,ConfigMgr::instance()->text("display_text.xml", "t557"),ConfigMgr::instance()->text("display_text.xml", "t558"),
		totalPage, ConfigMgr::instance()->text("display_text.xml", "t557"));
	logCountLabel->setString(logCountStr->getCString());
}

bool LotteryMyLot::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initData();
	this->initView();
	EntityMgr::instance()->getDispatch()->sendPacketWithGetKeFuURL();

	time_t t = time(0);
	char timeStr[64] = {0};
	char startTimeStr[64] = {0};
	strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S.999",localtime(&t) );
	strftime(startTimeStr, sizeof(startTimeStr), "%Y-%m-%d 00:00:00.000",localtime(&t) );
	EntityMgr::instance()->getDispatch()->SendPacketWithGetTouZhuLogCount(true, startTimeStr,timeStr);
	EntityMgr::instance()->getDispatch()->SendPacketWithGetLastYue();
	this->setKeypadEnabled(true);
	return true;
}

void LotteryMyLot::getTouZhuDataFromDatabase()
{
	if (touZhuData != NULL) {
		touZhuData->release();
		touZhuData = NULL;
	}
	touZhuData = Array::create();
	touZhuData->retain();
	DBHandler *handler = new DBHandler();
	sqlite3 *DB = handler->getDB();
	sqlite3_stmt *stmt;
	char sql[125] = {0};
	sprintf(sql, "select * from TouZhuXXInfo where dwUserID = %d", EntityMgr::instance()->getDispatch()->m_dwUserID);
	int rv = sqlite3_prepare(DB, sql, 100, &stmt, NULL);
	rv = sqlite3_step(stmt);
	while(rv == SQLITE_ROW)
	{
		Dictionary *dictionary = Dictionary::create();
		dictionary->setObject(String::createWithFormat("%s", sqlite3_column_text(stmt, 1)), "time");
		dictionary->setObject(Integer::create(sqlite3_column_int(stmt, 3)), "gameType");
		dictionary->setObject(Integer::create(sqlite3_column_int(stmt, 4)), "gameKind");
		dictionary->setObject(String::createWithFormat("%s", sqlite3_column_text(stmt, 5)), "qiShu");
		dictionary->setObject(String::createWithFormat("%s", sqlite3_column_text(stmt, 6)), "betNumber");
		dictionary->setObject(Integer::create(sqlite3_column_int(stmt, 7)), "zhuShu");
		dictionary->setObject(Integer::create(sqlite3_column_int(stmt, 8)), "beiTou");
		dictionary->setObject(Integer::create(sqlite3_column_int(stmt, 9)), "moshi");
		dictionary->setObject(Integer::create(sqlite3_column_int(stmt, 10)), "state");
		dictionary->setObject(Double::create(sqlite3_column_int(stmt, 11)), "yingkui");
		touZhuData->addObject(dictionary);
		rv = sqlite3_step(stmt);
	}
	sqlite3_finalize(stmt);
	//if(friendData>0)
	//{
	//	friendIndex = 0;
	//}
}

void LotteryMyLot::initData()
{
	showRecordInfo = false;
	totalPage = 0;
	pageSize = 7;
	currentPage = 1;
	touZhuLogCount = 0;
	//username = g_GlobalUnits.getGolbalUserData()->szNickName;
	username = g_GlobalUnits.getGolbalUserData()->szAccounts;
	money = EntityMgr::instance()->getDispatch()->f_yue;
	qiPaiMoney = EntityMgr::instance()->getDispatch()->f_qipai_yue;
	cellHeight = 100;
	winSize = Director::getInstance()->getWinSize();
}

//客服
void LotteryMyLot::contactKefu(Object* obj)
{
	playButtonSound();

	if(kefuStr == nullptr)
	{
		kefuStr = String::createWithFormat("%s", ConfigMgr::instance()->text("display_config.xml", "t5000"));
	}

	Application::getInstance()->openURL(BetLimit::GetStrWithoutSpace(kefuStr->getCString()));
}

void LotteryMyLot::initView()
{
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

	Sprite *bk2 = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_00.png"));
	bk2->setAnchorPoint(Vec2(0,1));
	bk2->setPosition(Vec2(0, SCREEN_HEIGHT - bk1->getContentSize().height));
	this->addChild(bk2,1);

	//返回按钮
	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryMyLot::back,this));
	pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t160"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	fontSize = 27;
	Sprite *setupItemNormalImage = Sprite::createWithSpriteFrame(spriteFrame("room_03.png"));
	Sprite *setupItemSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("room_03.png"));
	MenuItemSprite *setupItem = MenuItemSprite::create(setupItemNormalImage,setupItemSelectedImage,CC_CALLBACK_1(LotteryMyLot::setting,this));
	//setupItem->setPosition(Vec2(SCREEN_WIDTH - pCloseButton->getPositionX(), pCloseButton->getPositionY()));
	setupItem->setPosition(Vec2(SCREEN_WIDTH - 17 - setupItemNormalImage->getContentSize().width / 2, pCloseButton->getPositionY()));
	m_pSettingMenu = Menu::createWithItem(setupItem);
	m_pSettingMenu->setPosition(Vec2::ZERO);
	//this->addChild(m_pSettingMenu,1);

	//联系客服
	LabelTTF *contactLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t73"), "", fontSize);
	contactLabel->setColor(ccc3(255, 238, 31));
	MenuItemLabel *contactItem = MenuItemLabel::create(contactLabel,CC_CALLBACK_1(LotteryMyLot::contactKefu,this));
	contactItem->setPosition(Vec2(SCREEN_WIDTH - 80, SCREEN_HEIGHT - 200));
	contactMenu = Menu::createWithItem(contactItem);
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
	qiPaiMoneyLabel = LabelTTF::create(qiPaiMoneyStr->getCString(), "", fontSize);
	qiPaiMoneyLabel->setAnchorPoint(Vec2(0,0.5f));
	qiPaiMoneyLabel->setPosition(Vec2(userNameLabel->getPositionX(), userNameLabel->getPositionY()-bk2->getContentSize().height*3/5));
	qiPaiMoneyLabel->setColor(ccc3(255,255,255));
	this->addChild(qiPaiMoneyLabel, 1);

	float menuGap = 20;

	Sprite *rechargeNormalImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_02.png"));
	Sprite *rechargeSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_03.png"));
	Sprite *withdrawNormalImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_04.png"));
	Sprite *withdrawSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_05.png"));
	Sprite *toLotteryNormalImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_18.png"));
	Sprite *toLotterySelectedImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_19.png"));
	Sprite *toQiPaiNormalImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_20.png"));
	Sprite *toQiPaiSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_21.png"));
	MenuItemSprite *rechargeItem = MenuItemSprite::create(rechargeNormalImage,rechargeSelectedImage,CC_CALLBACK_1(LotteryMyLot::recharge,this));
	MenuItemSprite *withdrawItem = MenuItemSprite::create(withdrawNormalImage,withdrawSelectedImage,CC_CALLBACK_1(LotteryMyLot::withdraw,this));
	MenuItemSprite *toLotteryItem = MenuItemSprite::create(toLotteryNormalImage,toLotterySelectedImage,CC_CALLBACK_1(LotteryMyLot::transferToLottery,this));
	MenuItemSprite *toQiPaiItem = MenuItemSprite::create(toQiPaiNormalImage,toQiPaiSelectedImage,CC_CALLBACK_1(LotteryMyLot::transferToQiPai,this));
	transactionMenu = Menu::create(toLotteryItem,toQiPaiItem,rechargeItem,withdrawItem,NULL);
	transactionMenu->setPosition(Vec2(SCREEN_WIDTH/2, bk2->getPositionY() - bk2->getContentSize().height - rechargeNormalImage->getContentSize().height / 2));
	transactionMenu->alignItemsHorizontallyWithPadding(0);
	this->addChild(transactionMenu,1);

	Sprite *betRecordNormalImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_06.png"));
	Sprite *betRecordSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_07.png"));
	Sprite *winningRecordNormalImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_08.png"));
	Sprite *winningRecordSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_09.png"));
	Sprite *contiRecordNormalImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_10.png"));
	Sprite *contiRecordSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_11.png"));
	MenuItemSprite *betRecordItem = MenuItemSprite::create(betRecordNormalImage,betRecordSelectedImage,CC_CALLBACK_1(LotteryMyLot::betRecord,this));
	MenuItemSprite *winningRecordItem = MenuItemSprite::create(winningRecordNormalImage,winningRecordSelectedImage,CC_CALLBACK_1(LotteryMyLot::winningRecord,this));
	MenuItemSprite *contiRecordItem = MenuItemSprite::create(contiRecordNormalImage,contiRecordSelectedImage,CC_CALLBACK_1(LotteryMyLot::contiRecord,this));
	recordMenu = Menu::create(betRecordItem,winningRecordItem,contiRecordItem,NULL);
	recordMenu->setPosition(Vec2(SCREEN_WIDTH/2, transactionMenu->getPositionY() - rechargeNormalImage->getContentSize().height / 2 - betRecordNormalImage->getContentSize().height / 2 - 4 * menuGap));
	recordMenu->alignItemsHorizontallyWithPadding(0);
	//this->addChild(recordMenu,1);

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, transactionMenu->getPositionY()-rechargeNormalImage->getContentSize().height/2-menuGap));
	gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
	this->addChild(gap1,1);

	Sprite *bottomBk = Sprite::createWithSpriteFrame(spriteFrame("room_12_2.png"));
	bottomBk->setPosition(Vec2(SCREEN_WIDTH/2, 50));
	bottomBk->setScaleX(SCREEN_WIDTH / bottomBk->getContentSize().width);
	bottomBk->setScaleY(100/bottomBk->getContentSize().height);
	//bottomBk->setColor(ccc3(50,50,50));
	this->addChild(bottomBk,1);

	//上一页 下一页
	Sprite *previousNormalImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_12.png"));
	Sprite *previousSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_13.png"));
	Sprite *nextNormalImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_14.png"));
	Sprite *nextSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_15.png"));
	MenuItemSprite *nextItem = MenuItemSprite::create(nextNormalImage,nextSelectedImage,CC_CALLBACK_1(LotteryMyLot::getNextTouZhuLog,this));
	MenuItemSprite *previousItem = MenuItemSprite::create(previousNormalImage,previousSelectedImage,CC_CALLBACK_1(LotteryMyLot::getPreviousTouZhuLog,this));
	touZhuLogMenu = Menu::create(previousItem, nextItem, NULL);
	touZhuLogMenu->setPosition(Vec2(500,50));	
	touZhuLogMenu->alignItemsHorizontallyWithPadding(60);
	this->addChild(touZhuLogMenu,1);

	String *logCountStr = String::createWithFormat("%s%d%s  %s%d%s",ConfigMgr::instance()->text("display_text.xml", "t556"),
		1,ConfigMgr::instance()->text("display_text.xml", "t557"),ConfigMgr::instance()->text("display_text.xml", "t558"),
		1, ConfigMgr::instance()->text("display_text.xml", "t557"));

	logCountLabel = LabelTTF::create(logCountStr->getCString(), "", fontSize);
	logCountLabel->setAnchorPoint(Vec2(0,0.5f));
	logCountLabel->setPosition(Vec2(10, 40));
	logCountLabel->setColor(ccc3(255,255,255));
	this->addChild(logCountLabel, 1);

	cocos2d::Size tableViewSize;
	tableViewSize.width = winSize.width;
	tableViewSize.height = gap1->getPositionY()-80;

#ifdef VER_369
	tableViewSize.height -= 20;
#endif

	auto touZhuTable = TableView::create(this, tableViewSize);
	touZhuTable->setDelegate(this);
	touZhuTable->setAnchorPoint(Vec2(0.5, 0.5));
	touZhuTable->setPosition(Vec2(0, 80));
	touZhuTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	touZhuTable->setDirection(ScrollView::Direction::VERTICAL);
	touZhuTable->setTag(2563);
	this->addChild(touZhuTable, 1);

#ifdef VER_369
	touZhuTable->setPositionY(touZhuTable->getPositionY() + 20);
#endif
}

void LotteryMyLot::getNextTouZhuLog(Object *obj)
{
	playButtonSound();

	if(showRecordInfo == true){
		return;
	}
	if(currentPage<totalPage){
		currentPage++;
		time_t t = time(0);
		char timeStr[64] = {0};
		char startTimeStr[64] = {0};
		strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S.999",localtime(&t) );
		strftime(startTimeStr, sizeof(startTimeStr), "%Y-%m-%d 00:00:00.000",localtime(&t) );
		EntityMgr::instance()->getDispatch()->SendPacketWithGetTouZhuLog(currentPage, pageSize, true, startTimeStr,timeStr);
	}
}

void LotteryMyLot::getPreviousTouZhuLog(Object *obj)
{
	playButtonSound();

	if(showRecordInfo == true){
		return;
	}
	if(currentPage>1){
		currentPage--;
		time_t t = time(0);
		char timeStr[64] = {0};
		char startTimeStr[64] = {0};
		strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S.999",localtime(&t) );
		strftime(startTimeStr, sizeof(startTimeStr), "%Y-%m-%d 00:00:00.000",localtime(&t) );
		EntityMgr::instance()->getDispatch()->SendPacketWithGetTouZhuLog(currentPage, pageSize, true, startTimeStr,timeStr);
	}
}

//top menu
void LotteryMyLot::setting(Object* obj)
{
	if(showRecordInfo == true){
		return;
	}
	playButtonSound();

	LotterySettingView *layer = LotterySettingView::create();

	Scene *scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);
}

void LotteryMyLot::back(Object *obj)
{
	if(showRecordInfo == true){
		return;
	}
	playButtonSound();

	Director::getInstance()->popScene();
}

//transaction menu
void LotteryMyLot::transferToLottery(Object* obj)
{
	playButtonSound();
	Scene *scene = Scene::create();
	LotteryTransferMoney *layer = LotteryTransferMoney::create();
	layer->setTransferType(0);
	scene->addChild(layer);
	Director::getInstance()->pushScene(scene);
}
void LotteryMyLot::dissButtonRecord()
{
	pCloseButton->setEnabled(false);
//	m_pSettingMenu->setEnabled(false);
	contactMenu->setEnabled(false);
	transactionMenu->setEnabled(false);
	//recordMenu->setEnabled(false);
	touZhuLogMenu->setEnabled(false);
}
void LotteryMyLot::ShowButtonRecord()
{
	pCloseButton->setEnabled(true);
	//m_pSettingMenu->setEnabled(true);
	contactMenu->setEnabled(true);
	transactionMenu->setEnabled(true);
	//recordMenu->setEnabled(true);
	touZhuLogMenu->setEnabled(true);
}
void LotteryMyLot::transferToQiPai(Object *obj)
{
	playButtonSound();
	Scene *scene = Scene::create();
	LotteryTransferMoney *layer = LotteryTransferMoney::create();
	layer->setTransferType(1);
	scene->addChild(layer);
	Director::getInstance()->pushScene(scene);
}

void LotteryMyLot::recharge(Object* obj)
{
	playButtonSound();

	if(showRecordInfo == true){
		return;
	}
	Scene *scene = LotteryRecharge::scene();
	Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

void LotteryMyLot::withdraw(Object *obj)
{
	playButtonSound();

	if(showRecordInfo == true){
		return;
	}
	Scene *scene = LotteryWithdraw::scene();
	Director::getInstance()->pushScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

//record menu
void LotteryMyLot::betRecord(Object *obj)
{
	playButtonSound();

	if(showRecordInfo == true){
		return;
	}
}

void LotteryMyLot::winningRecord(Object *obj)
{
	playButtonSound();

	if(showRecordInfo == true){
		return;
	}
}

void LotteryMyLot::contiRecord(Object *obj)
{
	playButtonSound();

	if(showRecordInfo == true){
		return;
	}
}

void LotteryMyLot::onEnter()
{
	Layer::onEnter();
}

void LotteryMyLot::onExit()
{
	Layer::onExit();
}

void LotteryMyLot::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

cocos2d::Size LotteryMyLot::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = winSize.width;
	size.height = cellHeight;
	return size;
}

ssize_t LotteryMyLot::numberOfCellsInTableView(TableView *table)
{
	if(touZhuData == nullptr) return 0;

	int num = touZhuData->count();
	if(num == 0){
		num = 1;
	}
	return num;
}

TableViewCell *LotteryMyLot::tableCellAtIndex(TableView *table, ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();
	cell->autorelease();


	ccColor3B redColor = ccc3(61, 8, 40);
	ccColor3B blackColor = ccc3(150, 150, 150);
	float fontSize = 27;
	float gap = 30;
	float textWidth = 600;
	if (touZhuData->count() == 0){
		fontSize = 30;
		LabelTTF *gameTypeLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t559"), "", fontSize);
		gameTypeLabel->setColor(blackColor);
		gameTypeLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
		gameTypeLabel->setPosition(Vec2(SCREEN_WIDTH/2, cellHeight / 2.0f));
		cell->addChild(gameTypeLabel, 1);
		return cell;
	}

	Dictionary *items = (Dictionary *)touZhuData->objectAtIndex(index);
	String *time = (String *)items->objectForKey("time");
	Integer *gameType = (Integer *)items->objectForKey("gameType");
	Integer *gameKind = (Integer *)items->objectForKey("gameKind");
	String *qiShu = (String *)items->objectForKey("qiShu");
	String *betNumber = (String *)items->objectForKey("betNumber");
	Integer *zhuShu = (Integer *)items->objectForKey("zhuShu");
	Integer *beiTou = (Integer *)items->objectForKey("beiTou");
	Integer *moshi = (Integer *)items->objectForKey("moshi");
	Integer *state = (Integer *)items->objectForKey("state");
	Double *yingkui = (Double *)items->objectForKey("yingkui");
	Integer *WinzhuShu = (Integer *)items->objectForKey("winZhuShu");	//赢了多少注
	
	//显示彩种的代码，只预留了20个
	LabelTTF *gameTypeLabel = LabelTTF::create(ConfigMgr::instance()->text("display_config.xml",String::createWithFormat("t%d", 20 + gameType->getValue())->getCString()),
		"", fontSize, cocos2d::Size(textWidth, 0), kCCTextAlignmentLeft);
	gameTypeLabel->setColor(blackColor);
	gameTypeLabel->setAnchorPoint(Vec2(0, 0.5f));
	gameTypeLabel->setPosition(Vec2(gap, cellHeight*2.0f / 3.0f + 5));
	cell->addChild(gameTypeLabel, 1);

	LabelTTF *stateLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml",
		String::createWithFormat("t%d", 579 + state->getValue())->getCString()),
		"", fontSize, cocos2d::Size(textWidth, 0), kCCTextAlignmentLeft);
	stateLabel->setColor(blackColor);
	stateLabel->setAnchorPoint(Vec2(0, 0.5f));
	stateLabel->setPosition(Vec2(gap, cellHeight / 3.0f - 5));
	cell->addChild(stateLabel, 1);
	if (state->getValue() == 1)
	{
		//输赢是根据 赢了多少注来判断的，只以成败论英雄
		if (WinzhuShu->getValue() > 0)
		{
			//中奖了N注
			String *labelStr = String::createWithFormat("%s%ld%s", ConfigMgr::instance()->text("display_DuangDong.xml",
				String::createWithFormat("t9")->getCString()), WinzhuShu->getValue(), ConfigMgr::instance()->text("display_DuangDong.xml", String::createWithFormat("t13")->getCString()));

			stateLabel->setString(labelStr->getCString());
		}
	}
	//游戏类型
	LabelTTF *gameKindLabel = LabelTTF::create(BetLimit::GetWanFaNameFromId(gameType->getValue(), gameKind->getValue()).c_str(),
		"", fontSize, cocos2d::Size(textWidth, 0), kCCTextAlignmentLeft);
	gameKindLabel->setColor(blackColor);
	gameKindLabel->setAnchorPoint(Vec2(0, 0.5f));
	gameKindLabel->setPosition(Vec2(250, gameTypeLabel->getPositionY()));
	cell->addChild(gameKindLabel, 1);

	//期数
	LabelTTF *qiShuLabel = LabelTTF::create(String::createWithFormat("%s%s%s",
		ConfigMgr::instance()->text("display_text.xml", "t70"), qiShu->getCString(),
		ConfigMgr::instance()->text("display_text.xml", "t71"))->getCString(),
		"", fontSize, cocos2d::Size(textWidth, 0), kCCTextAlignmentLeft);
	qiShuLabel->setColor(redColor);
	qiShuLabel->setAnchorPoint(Vec2(0, 0.5f));
	qiShuLabel->setPosition(Vec2(450, gameTypeLabel->getPositionY()));
	cell->addChild(qiShuLabel, 1);

	//金额
	LabelTTF *haoMaLabel;

	float tempFloat = 2.0f;
	if(moshi->getValue() == 0)
	{
		tempFloat = 2.0f;
	}else
		if(moshi->getValue() == 1)
		{
			tempFloat = 0.2f;
		}else
			if(moshi->getValue() == 2)
			{
				tempFloat = 0.02f;
			}else
				if(moshi->getValue() == 3)
				{
					tempFloat = 0.002f;
				}
	if(gameType->getValue() == CZ_LUCKY_28 || gameType->getValue() == CZ_HK6HeCai) tempFloat = 1.0f;

	
	haoMaLabel = LabelTTF::create(String::createWithFormat("%s%.3lf%s", ConfigMgr::instance()->text("display_text.xml", "t79"), zhuShu->getValue()*beiTou->getValue()*tempFloat, ConfigMgr::instance()->text("display_text.xml", "t78"))->getCString(), "", fontSize, cocos2d::Size(textWidth, 0), kCCTextAlignmentLeft);
	
	haoMaLabel->setColor(redColor);
	haoMaLabel->setAnchorPoint(Vec2(0,0.5f));
	haoMaLabel->setPosition(Vec2(gameKindLabel->getPositionX(), cellHeight/3.0f-5));
	cell->addChild(haoMaLabel, 1);

	LabelTTF *timeLabel = LabelTTF::create(time->getCString()
		,"",fontSize-2,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	timeLabel->setColor(blackColor);
	timeLabel->setAnchorPoint(Vec2(0,0.5f));
	timeLabel->setPosition(Vec2(qiShuLabel->getPositionX(), haoMaLabel->getPositionY()));
	cell->addChild(timeLabel, 1);

	Sprite *more = Sprite::createWithSpriteFrame(spriteFrame("setting_01.png"));
	more->setAnchorPoint(Vec2(1,0.5f));
	more->setPosition(Vec2(SCREEN_WIDTH, cellHeight / 2));
	cell->addChild(more);

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, 0));
	cell->addChild(gap1,1);

	cell->setTag(index);
	return cell;  
}

void LotteryMyLot::tableCellTouched(TableView *table, TableViewCell *cell)
{
	if(showRecordInfo == true){
		return;
	}
	if(touZhuData == NULL ||touZhuData->count()==0)
	{
		return;
	};

	int tag = cell->getTag();
	Dictionary *dic = (Dictionary *)touZhuData->objectAtIndex(tag);
	int n_t_id = ((Integer *)dic->objectForKey("tid"))->getValue();

	LotteryMyLotRecordInfo *layer = LotteryMyLotRecordInfo::create();
	Scene *scene = Scene::create();
	scene->addChild(layer,3);
	Director::getInstance()->pushScene(scene);

	layer->setTID(n_t_id);
	EntityMgr::instance()->getDispatch()->SendPacketWithGetTouZhuXX(n_t_id);
	showRecordInfo = true;
}

void LotteryMyLot::scrollViewDidScroll(ScrollView* view)
{

}
void LotteryMyLot::scrollViewDidZoom(ScrollView* view)
{

}

//根据彩种ID和玩法ID，返回玩法名称
const char *LotteryMyLot::getGameKindTitle(int gameType, int gameKind)
{
	return BetLimit::GetWanFaNameFromId(gameType, gameKind).c_str();
}

void LotteryMyLot::clear()
{
	NotificationCenter::getInstance()->removeObserver(this, "GetLastYueRet");
	NotificationCenter::getInstance()->removeObserver(this, "TouZhuLogCount");
	NotificationCenter::getInstance()->removeObserver(this, "TouZhuLog");
	NotificationCenter::getInstance()->removeObserver(this, "RemoveRecordInfoLayer");
	NotificationCenter::getInstance()->removeObserver(this, "ZhuanHuanRet");
	NotificationCenter::getInstance()->removeObserver(this, "GetKeFuUrl");
	if(touZhuData)
	{
		touZhuData->removeAllObjects();
		touZhuData->release();
		touZhuData = nullptr;
	}
}