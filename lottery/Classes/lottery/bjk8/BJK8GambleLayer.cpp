#include "BJK8GambleLayer.h"
#include "RoomLayer.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"
#include <sstream>
#include "LotteryHelpView.h"
#include "BetLimit.h"
#include "comm.h"
#include "LotterySceneControl.h"
#include "LotteryGameResultDetail.h"
#include "LotteryBetData.h"
#include "VersionControl.h"

using namespace CocosDenshion;
using namespace extension;


Scene* BJK8GambleLayer::scene()
{
	Scene *scene = Scene::create();
	BJK8GambleLayer *layer = BJK8GambleLayer::create();
	scene->addChild(layer);
	return scene;
}

BJK8GambleLayer::BJK8GambleLayer()
{
	MyBetNumber::addRandSeed();
	wKindID = NULL;
	wPeriod = NULL;
	szLotNum = NULL;
	szShijian = NULL;
	gameRule = NULL;
	gameKind = CZKUAILE8;
	nLMoshi =0;
	betZhuShuTouZhuJinE =0;
	TouZhuJinE =0.00;
	beiShu = 1;
	ZhangHuYuE = EntityMgr::instance()->getDispatch()->f_yue;

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BJK8GambleLayer::GetLastYueRet), "GetLastYueRet", NULL);  ///余额查询
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BJK8GambleLayer::ShiYiXuanWoNumber), "ShiYiXuanWoNumber", NULL);  ///追号所花费的金额
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BJK8GambleLayer::ZhuiHaoTouZhuJinENumber), "ZhuiHaoTouZhuJinENumber", NULL);  ///追号所花费的金额	
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BJK8GambleLayer::touZhuRet), "CQSSCRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BJK8GambleLayer::changeKind), "CQSSCChangeKind", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BJK8GambleLayer::deleteItem), "DeleteItem", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BJK8GambleLayer::addRandomItem), "AddRandomItem", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BJK8GambleLayer::cartConfirm), "CartConfirm", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BJK8GambleLayer::clearNumbers), "ClearNumbers", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BJK8GambleLayer::updateTouZhuJine), "UpdateTouZhuJine", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BJK8GambleLayer::KindMyFanDianRet), "QueryMyFanDianRet", NULL);

	//追号验证
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(BJK8GambleLayer::isFenDanHour), "IsFendanHour", NULL);

	//跨年期号处理
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(BJK8GambleLayer::kuaNianExpect), "KuaNian", NULL);

	//结果查询
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BJK8GambleLayer::queryGameMoreRet), "QueryMoreGameRet", NULL);
}

BJK8GambleLayer::~BJK8GambleLayer()
{
	if(wKindID != NULL)
	{
		wKindID->release();
		wKindID = NULL;
	}
	if(wPeriod != NULL)
	{
		wPeriod->release();
		wPeriod = NULL;
	}
	if(szLotNum != NULL)
	{
		szLotNum->release();
		szLotNum = NULL;
	}
	if(szShijian != NULL)
	{
		szShijian->release();
		szShijian = NULL;
	}
	if(m_data != nullptr)
	{
		m_data->removeAllObjects();
		m_data->release();
		m_data = nullptr;
	}

	NotificationCenter::getInstance()->removeObserver(this, "GetLastYueRet");
	NotificationCenter::getInstance()->removeObserver(this, "ShiYiXuanWoNumber");
	NotificationCenter::getInstance()->removeObserver(this, "ZhuiHaoTouZhuJinENumber");	
	NotificationCenter::getInstance()->removeObserver(this, "CQSSCRet");
	NotificationCenter::getInstance()->removeObserver(this, "CQSSCChangeKind");
	NotificationCenter::getInstance()->removeObserver(this, "DeleteItem");
	NotificationCenter::getInstance()->removeObserver(this, "AddRandomItem");
	NotificationCenter::getInstance()->removeObserver(this, "CartConfirm");
	NotificationCenter::getInstance()->removeObserver(this, "ClearNumbers");
	NotificationCenter::getInstance()->removeObserver(this, "UpdateTouZhuJine");
	this->unschedule(schedule_selector(BJK8GambleLayer::myUpdate));
	this->unschedule(schedule_selector(BJK8GambleLayer::getSystemTime));
	NotificationCenter::getInstance()->removeObserver(this, "QueryMyFanDianRet");

	NotificationCenter::getInstance()->removeObserver(this, "IsFendanHour");

	NotificationCenter::getInstance()->removeObserver(this, "KuaNian");

	NotificationCenter::getInstance()->removeObserver(this, "QueryMoreGameRet");

	removeAllChildrenWithCleanup(true);
	TextureCache::getInstance()->removeAllTextures(); //释放到目前为止所有加载的图片
	TextureCache::getInstance()->removeUnusedTextures(); 
}

void BJK8GambleLayer::setGameKind(CaiZhong kind)
{
	//初始化数据
	MyBetData::getInstance()->setCurCaiZhongId(kind);
	int last = MyBetData::getInstance()->getLastWanfaId();
	if(last == -1) last = tagKuai8[1];

	this->setWanFaKind((BeiJingKuai8Kind)last);

	selectedItemTag = 1;

	for(int i = 0;i < MyBetData::getInstance()->getWanfaSize();i ++)
	{
		if(tagKuai8[MyBetData::getInstance()->getWanfaAt(i)] == last)
		{
			selectedItemTag = MyBetData::getInstance()->getWanfaAt(i);
			break;
		}
	}

	//初始化
	int posSave = MyBetData::getInstance()->getLastMoshi();
	switch (posSave)
	{
	case 0:
		XuanZeMonenyLayer(YuanSelectedImageLayer);
		break;
	case 1:
		XuanZeMonenyLayer(JiaoSelectedImageLayer);
		break;
	case 2:
		XuanZeMonenyLayer(FenSelectedImageLayer);
		break;
	case 3:
		XuanZeMonenyLayer(LiSelectedImageLayer);
		break;
	default:
		break;
	}
	gameKind = kind;

	//开奖结果详细
	EntityMgr::instance()->getDispatch()->SendPacketQueryMoreGameResult(gameKind);	

	//跨年处理
#if 1
	EntityMgr::instance()->getDispatch()->SendPacketWithGetKuaNianQiHao(gameKind);
	this->schedule(schedule_selector(BJK8GambleLayer::getSystemTime), 3);	
#endif
	this->schedule(schedule_selector(BJK8GambleLayer::myUpdate), 1);

	gameTitle->setString(ConfigMgr::instance()->text("display_config.xml",String::createWithFormat("t%d", 20+gameKind)->getCString()));

	if(gameRule == NULL){
		if(gameKind == CZKUAILE8)
		{
			gameRule = new CKuaiLe8Rule();
		}
		this->addChild(gameRule);

		m_bIsFendan = gameRule->IsFenDanDuration();
	}

	EntityMgr::instance()->getDispatch()->SendPacketWithTouZhuCQSSC_FanDian(gameKind, BetLimit::GetWanfaIdFromId(wanFaKind,5));

	//获得表格
	auto layerTable = this->getChildByTag(9527);
	if(layerTable == nullptr) return;
	auto layerResult = layerTable->getChildByTag(1001);
	if(layerResult == nullptr) return;
	auto menuTable = (TableView *)layerResult->getChildByTag(1000);
	if(menuTable == nullptr) return;
	menuTable->reloadData();

	int iPos = 0;
	for(int i = 0;i < MyBetData::getInstance()->getWanfaSize();i ++)
	{
		if(MyBetData::getInstance()->getWanfaAt(i) == selectedItemTag)
		{			
			iPos = i;
			break;
		}
	}

	menuTable->setContentOffset(Vec2((-iPos / 3) * menuTable->getContentSize().width / 3,0));
}

void BJK8GambleLayer::refreshLayer()
{
	//更新余额
	EntityMgr::instance()->getDispatch()->SendPacketWithGetLastYue();	
	//更新奖金
	EntityMgr::instance()->getDispatch()->SendPacketWithTouZhuCQSSC_FanDian(gameKind, BetLimit::GetWanfaIdFromId(wanFaKind,5));
	isReceiveFanDian = false;

	this->schedule([=](float dt){
		if(!isReceiveFanDian)
		{
			EntityMgr::instance()->getDispatch()->SendPacketWithTouZhuCQSSC_FanDian(gameKind, BetLimit::GetWanfaIdFromId(wanFaKind,5));
		} else
		{
			this->unschedule("update_gold");
		}
	},2.0f,"update_gold");
}

void BJK8GambleLayer::setWanFaKind(BeiJingKuai8Kind kind)
{
	wanFaKind = kind;

	bool isNeedMove = betArea->getPositionY() == -300;

	if (betArea)
		betArea->removeFromParentAndCleanup(true);
	betArea = NULL;

	auto layer = (LayerColor *)this->getChildByTag(9527);
	if(layer == nullptr) return;

	betArea = BJK8WuFen::create();
	layer->addChild(betArea,0);
	betArea->setBetKind(wanFaKind);

	if(!isNeedMove) return;

	if(m_bIsOpenList)
	{
		betArea->setPositionY(-300);
		betArea->m_endPos.y += 300;	
	}else
	{
		betArea->setPositionY(0);
		betArea->m_endPos.y -= 300;	
	}

	MyBetData::getInstance()->setLastWanfaId(kind);
}

bool BJK8GambleLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initData();
	this->initView();
	this->setKeypadEnabled(true);
	
	EntityMgr::instance()->getDispatch()->SendPacketWithGetLastYue();
	return true;
}

void BJK8GambleLayer::getSystemTime(float dt)
{
	EntityMgr::instance()->getDispatch()->SendPacketWithGetKuaNianQiHao(gameKind);
}

void BJK8GambleLayer::myUpdate(float dt)
{
	string value6 = gameRule->GetNextExpect();
	string value3 = gameRule->GetKjShjDiffDesc();

	stringstream strValue;
	strValue << value6.c_str();
	strValue>>currentLotteryId;

	currentTime = value3;
	
	this->updateLotteryId();
	this->updateCurrentTime();
	EntityMgr::instance()->getDispatch()->TimeIncrease();
}

//初始化默认数据
//确定进入以后默认玩法
void BJK8GambleLayer::initData()
{

	n_t_kindid=NULL;			//玩法ID
	f_t_bonus = NULL;			//最小奖金数字
	f_t_bonusPercent = NULL;	//间距
	f_t_fandian = NULL;			//最大百分比

	selectedItemTag = 1;
	wanFaKind = enBJK8_RX_1; //

	currentLotteryId = 0;
	currentTime = string("19:30");
	winSize = Director::getInstance()->getWinSize();

	BaiFenbiNumber = 0.00;
	FandianBaifenbipColse =0.0;
	FandianNumberpColse =0.0;
	PlusFanDianvalieMAX = 0.00;
	GlobalVariableFanDianvalie =0.00;
	fandianNumber =0.00;
	lenJindu = 0;
	JinduPosX = 0;
	FanDianbI =0.00;
	SlidePlusde = false; 
	m_bIsFendan = false;

	kindLayerIsShow = false;
	kindLayer = nullptr;

	m_data = NULL;
	m_data = Array::create();
	m_data->retain();
	m_bIsOpenList = false;
}

//初始化界面
void BJK8GambleLayer::initView()
{
	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	layer->setPosition(Vec2::ZERO);
	layer->setTag(9527);
	this->addChild(layer,1);

	//顶部背景
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame(HEAD_IMG));
	bk1->setAnchorPoint(Vec2(0,1));
	bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
	bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
	bk1->setScaleY(162 / bk1->getContentSize().height);
	layer->addChild(bk1,5);

	//顶部返回按钮
	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(BJK8GambleLayer::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setAnchorPoint(Vec2(0,1));
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	layer->addChild(pCloseButton,5);

	//顶部功能按钮
	Sprite *setupItemNormalImage = Sprite::createWithSpriteFrame(spriteFrame("room_03.png"));
	Sprite *setupItemSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("room_03.png"));
	MenuItemSprite *setupItem = MenuItemSprite::create(setupItemNormalImage,setupItemSelectedImage,CC_CALLBACK_1(BJK8GambleLayer::setting,this));
	setupItem->setPosition(Vec2(SCREEN_WIDTH - 17 - setupItemNormalImage->getContentSize().width / 2, pCloseButton->getPositionY()));	
	//LabelTTF* moreItemLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t4700"), "", 30);
	//MenuItemLabel *lbItem = MenuItemLabel::create(moreItemLabel,CC_CALLBACK_1(BJK8GambleLayer::jumpResult,this));
	//lbItem->setPosition(Vec2(setupItem->getPositionX() - setupItem->getContentSize().width - 15, setupItem->getPositionY() - 10));
	Menu *m_pSettingMenu = Menu::create(setupItem, NULL);
	m_pSettingMenu->setPosition(Vec2::ZERO);
	layer->addChild(m_pSettingMenu,5);	

	//彩种名称
	float fontSize = 38;
	gameTitle = LabelTTF::create(ConfigMgr::instance()->text("display_config.xml", "t20"), "", fontSize);
	gameTitle->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	gameTitle->setColor(ccc3(255,255,255));
	layer->addChild(gameTitle, 5);

	//第N期和倒计时，刷新层
	lyRefresh = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, 200);
	lyRefresh->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	lyRefresh->setPosition(Vec2(0,SCREEN_HEIGHT - 220));
	lyRefresh->setTag(1002);
	layer->addChild(lyRefresh, 4);
	//lyRefresh->setVisible(false);

	//动画 刷新
	SpriteFrameCache *frameCache=SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("refresh.plist");
	cocos2d::Vector<SpriteFrame *> frame;	
	for(int i = 1;i < 19;i ++)
	{
		SpriteFrame* temp = frameCache->getSpriteFrameByName(String::createWithFormat("refresh%02d.png",i)->getCString());
		frame.pushBack(temp);
	}
	Animation* ani = Animation::createWithSpriteFrames(frame, 0.1f,18);

	//动画sprite
	Sprite* sptRefresh = Sprite::create();
	sptRefresh->setDisplayFrame(frameCache->getSpriteFrameByName("refresh01.png"));
	sptRefresh->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sptRefresh->setPosition(Vec2(SCREEN_WIDTH / 3, 120));
	sptRefresh->runAction(RepeatForever::create(Sequence::create(Animate::create(ani),nullptr)));
	lyRefresh->addChild(sptRefresh);

	//下拉刷新字样
	LabelTTF *RefreshLabel = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t12"), "", 26);
	RefreshLabel->setColor(ccc3(0,0,0));
	RefreshLabel->setAnchorPoint(Vec2(0, 0.5f));
	RefreshLabel->setPosition(Vec2(sptRefresh->getPositionX() + 80, sptRefresh->getPositionY() + 25));
	lyRefresh->addChild(RefreshLabel);

	//最后更新 今天 XX:XX
	LabelTTF *ltfUpdate = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t1631"), "", 26);
	ltfUpdate->setColor(ccc3(0,0,0));
	ltfUpdate->setAnchorPoint(Vec2(0, 0.5f));
	ltfUpdate->setPosition(Vec2(sptRefresh->getPositionX() + 80, sptRefresh->getPositionY() - 25));
	lyRefresh->addChild(ltfUpdate);

	//第N期
	fontSize = 27;
	String *lotteryIdString;
	lotteryIdString = String::createWithFormat("%s%s",
		ConfigMgr::instance()->text("display_text.xml", "t70"),
		ConfigMgr::instance()->text("display_text.xml", "t71"));
	lotteryIdLabel = LabelTTF::create(lotteryIdString->getCString(), "", fontSize);
	lotteryIdLabel->setAnchorPoint(Vec2(0,0.5f));
	lotteryIdLabel->setPosition(Vec2(32, 30));
	lotteryIdLabel->setColor(ccc3(0,0,0));
	lyRefresh->addChild(lotteryIdLabel, 2);

	//当前时间倒计时
	String *currentTimeString = String::create(STR_XDSJ);
	currentTimeLabel = LabelTTF::create(currentTimeString->getCString(), "", fontSize);
	currentTimeLabel->setAnchorPoint(Vec2(0,0.5f));
	currentTimeLabel->setPosition(Vec2(SCREEN_WIDTH/2, lotteryIdLabel->getPositionY()));
	currentTimeLabel->setColor(ccc3(0,0,0));
	lyRefresh->addChild(currentTimeLabel, 2);

#ifdef VER_369
	RefreshLabel->setColor(ccc3(50, 62, 123));
	ltfUpdate->setColor(ccc3(50, 62, 123));
	lotteryIdLabel->setColor(ccc3(50, 62, 123));
	currentTimeLabel->setColor(ccc3(50, 62, 123));
#endif

	//选择下注模式  随机选择 机选和清除
	Sprite *deleteNormalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_111.png"));
	Sprite *deleteSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_112.png"));
	Sprite *randomNormalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_09.png"));
	Sprite *randomSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_10.png"));
	MenuItemSprite *deleteItem = MenuItemSprite::create(deleteNormalImage,deleteSelectedImage,CC_CALLBACK_1(BJK8GambleLayer::toDelete,this));
	MenuItemSprite *randomGambleItem = MenuItemSprite::create(randomNormalImage,randomSelectedImage,CC_CALLBACK_1(BJK8GambleLayer::toRandomGamble,this));
	Menu *middleMenu = Menu::create(deleteItem,randomGambleItem,NULL);
	//middleMenu->setPosition(Vec2(SCREEN_WIDTH - deleteNormalImage->getContentSize().width - 34, lotteryIdLabel->getPositionY() - 35 - 39));
	middleMenu->setPosition(Vec2(SCREEN_WIDTH - 100, 215-55));
	middleMenu->alignItemsHorizontallyWithPadding(5);
	layer->addChild(middleMenu,4);

	//开奖记录和菜单选择下拉框 层
	lyTables = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, 400);
	lyTables->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	lyTables->setPosition(Vec2(0,lyRefresh->getPositionY() - LAYER_HEIGHT));
	lyTables->setTag(1001);
	layer->addChild(lyTables,3);

	//前排标志 下拉框 背景
	gambleTypeBk = Sprite::createWithSpriteFrame(spriteFrame("bg_a1.png"));
	gambleTypeBk->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	gambleTypeBk->setPosition(Vec2(SCREEN_WIDTH / 2, 25));
	gambleTypeBk->setScaleX(SCREEN_WIDTH / gambleTypeBk->getContentSize().width);
	gambleTypeBk->setScaleY(60 / gambleTypeBk->getContentSize().height);
	gambleTypeBk->setColor(ccc3(200,200,200));
	lyTables->addChild(gambleTypeBk,2);

	//前排标志 下拉框
	Sprite *moreNormalImage2 = Sprite::createWithSpriteFrame(spriteFrame("regist_04_2.png"));
	Sprite *moreSelectedImage2 = Sprite::createWithSpriteFrame(spriteFrame("regist_04_2.png"));
	moreItem = MenuItemSprite::create(moreNormalImage2,moreSelectedImage2,CC_CALLBACK_1(BJK8GambleLayer::toMore,this));
	moreItem->setPosition(Vec2(SCREEN_WIDTH - 25, 25));;
	Menu *middleMenu2 = Menu::create(moreItem,NULL);
	middleMenu2->setPosition(Vec2::ZERO);
	lyTables->addChild(middleMenu2,3);

	//gamble type title; 默认玩法名字
	gambleTypeTitle = LabelTTF::create("    ", "", fontSize);
	gambleTypeTitle->setAnchorPoint(Vec2(0.5f,0.5f));
	gambleTypeTitle->setPosition(gambleTypeBk->getPosition());
	gambleTypeTitle->setColor(ccc3(255,255,255));
	lyTables->addChild(gambleTypeTitle, 2);
	gambleTypeTitle->setVisible(false);

	//菜单列表
	cocos2d::Size menuSize;
	menuSize.width = SCREEN_WIDTH - 50;
	menuSize.height = 60;
	auto menuTable = TableView::create(this, menuSize);
	menuTable->setDelegate(this);
	menuTable->setTag(1000);
	menuTable->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	menuTable->setPosition(Vec2::ZERO);
	menuTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	menuTable->setDirection(ScrollView::Direction::HORIZONTAL);
	lyTables->addChild(menuTable, 2);
	menuTable->retain();

	//下拉开奖结果
	cocos2d::Size listSize;
	listSize.width = SCREEN_WIDTH;
	listSize.height = 300;
	auto listTable = TableView::create(this, listSize);
	listTable->setDelegate(this);
	listTable->setTag(8888);
	listTable->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	listTable->setPosition(Vec2(0, 60));
	listTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	listTable->setDirection(ScrollView::Direction::VERTICAL);
	lyTables->addChild(listTable, 2);
	listTable->retain();

	listTable->setSwallowsTouches(false);

	//底部背景
	Sprite *gap2 = Sprite::createWithSpriteFrame(spriteFrame("beting51.png"));
	gap2->setAnchorPoint(Vec2(0,1));
	gap2->setPosition(Vec2(0, 202));
	gap2->setScaleX(SCREEN_WIDTH / gap2->getContentSize().width);
	gap2->setScaleY(202 / gap2->getContentSize().height);
	layer->addChild(gap2,2);

	//文字 投注多少，彩票余额多少
	TouZhuJinELabel = LabelTTF::create("", "", fontSize);
	TouZhuJinELabel->setPosition(Vec2(34,gap2->getPositionY() - 98));//winSize.width*0.03
	TouZhuJinELabel->setAnchorPoint(Vec2(0, 0.5f));
	TouZhuJinELabel->setColor(ccc3(255,255,255));
	layer->addChild(TouZhuJinELabel, 2);

	updateMoneyLabel();

	//新增
	LabelTTF *beilvBkTTf  = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t181"), "", fontSize);
	beilvBkTTf->setAnchorPoint(Vec2(0,0.5f));
	beilvBkTTf->setPosition(Vec2(34, gap2->getPositionY() - 42));
	beilvBkTTf->setColor(ccc3(255,255,255));
	layer->addChild(beilvBkTTf, 3);
	//后边框 多少倍
	Sprite *beilvBk = Sprite::createWithSpriteFrame(spriteFrame("beting111.png"));
	beilvBk->setAnchorPoint(Vec2(0,0.5f));
	beilvBk->setPosition(Vec2(150, beilvBkTTf->getPositionY()));
	layer->addChild(beilvBk,3);
	//增减菜单
	Sprite *beiAdd = Sprite::createWithSpriteFrame(spriteFrame("beting110.png"));
	Sprite *beiAddPress = Sprite::createWithSpriteFrame(spriteFrame("beting110_2.png"));
	Sprite *beiMinus = Sprite::createWithSpriteFrame(spriteFrame("beting112.png"));
	Sprite *beiMinusPress = Sprite::createWithSpriteFrame(spriteFrame("beting112_2.png"));	
	MenuItemSprite *beiAddItem = MenuItemSprite::create(beiAdd,beiAddPress,CC_CALLBACK_1(BJK8GambleLayer::beiAdd,this));
	MenuItemSprite *beiMinusItem = MenuItemSprite::create(beiMinus,beiMinusPress,CC_CALLBACK_1(BJK8GambleLayer::beiMinus,this));
	Menu* beiMenu = Menu::create(beiAddItem,beiMinusItem,NULL);
	beiMenu->setPosition(Vec2(beilvBk->getPositionX() + beiAdd->getContentSize().width, beilvBkTTf->getPositionY())); // + 1 means 图片横向差了一像素，用代码补上
	beiMenu->alignItemsHorizontallyWithPadding(beilvBk->getContentSize().width - 20);
	layer->addChild(beiMenu,3);
	//输入框
	fontSize = 30;
	beilvInput = EditBox::create(cocos2d::Size(80, 32), "blank.png");
	beilvInputMidX = beilvBk->getPositionX() + 5;
	beilvInput->setPosition(Vec2(beilvInputMidX, beilvBk->getPositionY()));
	beilvInput->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE);
	beilvInput->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	beilvInput->setInputMode(ui::EditBox::InputMode::NUMERIC);
	beilvInput->setAnchorPoint(Vec2(0,0.5f));
	beilvInput->setFontColor(ccc3(255,255,255));
	beilvInput->setFontName("");
	beilvInput->setFontSize(fontSize);
	beilvInput->setColor(ccc3(112,112,112));
	beilvInput->setText("1");
	beilvInput->setMaxLength(20);
	layer->addChild(beilvInput, 3);
	beilvInput->setDelegate(this);

	//单注奖金
	Sprite *FanDianSprite_1 = Sprite::createWithSpriteFrame(spriteFrame("beting11.png"));
	Sprite *FanDianSprite_2 = Sprite::createWithSpriteFrame(spriteFrame("beting12.png"));
	MenuItemSprite *FanDianSelectedImage = MenuItemSprite::create(FanDianSprite_1,FanDianSprite_2,CC_CALLBACK_1(BJK8GambleLayer::FanDianMinus,this));
	Menu* FanDianMenu = Menu::create(FanDianSelectedImage,NULL);
	FanDianMenu->setPosition(Vec2(SCREEN_WIDTH / 2 + 75, beilvBkTTf->getPositionY()));
	layer->addChild(FanDianMenu,3);

	//返点显示
	Cardnumttf = LabelTTF::create("", "", 27);
	Cardnumttf->setPosition(Vec2(FanDianMenu->getPositionX(), beilvBkTTf->getPositionY()));
	Cardnumttf->setAnchorPoint(Vec2(0.5f,0.5f));
	Cardnumttf->setColor(ccc3(0,0,0));
	layer->addChild(Cardnumttf, 3);

#ifdef VER_QIANBO
	Cardnumttf->setColor(cocos2d::ccColor3B::YELLOW);
#endif

#ifdef VER_369
	Cardnumttf->setColor(cocos2d::ccColor3B::WHITE);
	beilvInput->setFontColor(ccc3(50, 62, 123));
#endif

	//-----------------------------------------------------

	//底部背景不透明
	Sprite *bottomBk = Sprite::createWithSpriteFrame(spriteFrame("room_12_2.png"));
	bottomBk->setPosition(Vec2(SCREEN_WIDTH/2, 42));
	bottomBk->setScaleX(SCREEN_WIDTH / bottomBk->getContentSize().width);
	bottomBk->setScaleY(84/bottomBk->getContentSize().height);
	//bottomBk->setColor(ccc3(50,50,50));
	layer->addChild(bottomBk,2);

	//底部按钮 确定投注和选定
	Sprite *cartNormalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_11.png"));
	Sprite *cartSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_12.png"));
	Sprite *confirmNormalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_13.png"));
	Sprite *confirmSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_14.png"));
	cartItem = MenuItemSprite::create(cartNormalImage,cartSelectedImage,CC_CALLBACK_1(BJK8GambleLayer::toGamble,this));
	MenuItemSprite *confirmItem = MenuItemSprite::create(confirmNormalImage,confirmSelectedImage,CC_CALLBACK_1(BJK8GambleLayer::toCart,this));
	cartItem->setPosition(Vec2(SCREEN_WIDTH - 34 * 2 - cartNormalImage->getContentSize().width - confirmNormalImage->getContentSize().width / 2, bottomBk->getPositionY()));
	confirmItem->setPosition(Vec2(SCREEN_WIDTH - 34 - cartNormalImage->getContentSize().width / 2, bottomBk->getPositionY()));
	Menu *bottomMenu = Menu::create(cartItem,confirmItem,NULL);
	//Menu *bottomMenu = Menu::create(deleteItem,confirmItem,NULL);
	bottomMenu->setPosition(Vec2::ZERO);	
	layer->addChild(bottomMenu,2);

	//元角分按钮
	Sprite *YuanSprite_1 = Sprite::createWithSpriteFrame(spriteFrame("beting105.png"));
	Sprite *YuanSprite_2 = Sprite::createWithSpriteFrame(spriteFrame("beting06.png"));
	Sprite *YuanSprite_3 = Sprite::createWithSpriteFrame(spriteFrame("beting06.png"));
	Sprite *JiaoSprite_1 = Sprite::createWithSpriteFrame(spriteFrame("beting107.png"));
	Sprite *JiaoSprite_2 = Sprite::createWithSpriteFrame(spriteFrame("beting107.png"));
	Sprite *JiaoSprite_3 = Sprite::createWithSpriteFrame(spriteFrame("beting08.png"));
	Sprite *FenSprite_1 = Sprite::createWithSpriteFrame(spriteFrame("beting109.png"));
	Sprite *FenSprite_2 = Sprite::createWithSpriteFrame(spriteFrame("beting109.png"));
	Sprite *FenSprite_3 = Sprite::createWithSpriteFrame(spriteFrame("beting10.png"));
	Sprite *LiSprite_1 = Sprite::createWithSpriteFrame(spriteFrame("beting115.png"));
	Sprite *LiSprite_2 = Sprite::createWithSpriteFrame(spriteFrame("beting114.png"));
	Sprite *LiSprite_3 = Sprite::createWithSpriteFrame(spriteFrame("beting114.png"));
	YuanSelectedImageLayer = MenuItemSprite::create(YuanSprite_1,YuanSprite_2,YuanSprite_3,CC_CALLBACK_1(BJK8GambleLayer::XuanZeMonenyLayer,this));
	YuanSelectedImageLayer->setTag(YuanShiShiCAITag);
	JiaoSelectedImageLayer = MenuItemSprite::create(JiaoSprite_1,JiaoSprite_2,JiaoSprite_3,CC_CALLBACK_1(BJK8GambleLayer::XuanZeMonenyLayer,this));
	JiaoSelectedImageLayer->setTag(JiaoShiShiCAITag);
	FenSelectedImageLayer = MenuItemSprite::create(FenSprite_1,FenSprite_2,FenSprite_3,CC_CALLBACK_1(BJK8GambleLayer::XuanZeMonenyLayer,this));
	FenSelectedImageLayer->setTag(FenShiShiCAITag);
	//新增厘模式
	LiSelectedImageLayer = MenuItemSprite::create(LiSprite_1, LiSprite_2, LiSprite_3,CC_CALLBACK_1(BJK8GambleLayer::XuanZeMonenyLayer, this));
	LiSelectedImageLayer->setTag(LiShiShiTag);

	MonenyMenuLayer = Menu::create(YuanSelectedImageLayer,JiaoSelectedImageLayer,FenSelectedImageLayer,LiSelectedImageLayer,NULL);
	MonenyMenuLayer->setPosition(Vec2(34 + (YuanSprite_1->getContentSize().width* 3 + 20 + 70)/2, bottomBk->getPositionY()));
	MonenyMenuLayer->alignItemsHorizontallyWithPadding(10);
	layer->addChild(MonenyMenuLayer,2);	

	//分组选择 彩票选择模块
	betArea = BJK8WuFen::create();
	layer->addChild(betArea,0);
	betArea->setBetKind(wanFaKind);

	//顶部掩体，向上滑动时到此为止就被遮住
	LayerColor* maskLayer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, 120);
	maskLayer->ignoreAnchorPointForPosition(false);
	maskLayer->setPosition(Vec2(winSize.width*0.5, SCREEN_HEIGHT - 162 - 74));
	layer->addChild(maskLayer, 1);

	//下划线
	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, 1));
	gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
	maskLayer->addChild(gap1,2);

	cart = BJK8Cart::create();
	cart->setPosition(Vec2(0,2000));	
	layer->addChild(cart, 10);

#ifdef VER_369
	MonenyMenuLayer->alignItemsHorizontallyWithPadding(3);
	MonenyMenuLayer->setPositionX(MonenyMenuLayer->getPositionX() - 20);
	cartItem->setPositionX(cartItem->getPositionX() + 40);	
	confirmItem->setPositionX(confirmItem->getPositionX() + 20);	
#endif

	//添加监听 刷新
	//正在动画
	isRun = false;

	auto refreshListener = EventListenerTouchOneByOne::create();
	refreshListener->setSwallowTouches(false);
	refreshListener->onTouchBegan = [=](Touch *pTouch, Event *pEvent){			

		if(isRun) return false;

		if(lyRefresh->convertTouchToNodeSpace(pTouch).y < 0 && lyTables->convertTouchToNodeSpace(pTouch).y < 60)
		{				
			return false;				
		}		

		startPoint = pTouch->getLocation();
		return true;
	};
	refreshListener->onTouchEnded = [=](Touch *pTouch, Event *pEvent){
		CCLOG("================================  %f",  listTable->getContentOffset().y);
		//开奖记录下拉
		if(pTouch->getLocation().y - startPoint.y < -60)
		{		
			//下拉刷新图标
			if(m_bIsOpenList && listTable->getContentOffset().y < -760)
			{
				isRun = true;

				lyRefresh->runAction(Sequence::create(
					Spawn::createWithTwoActions(
					TargetedAction::create(lyRefresh,MoveTo::create(0.5f,Vec2(lyRefresh->getPositionX(),SCREEN_HEIGHT - 220 - lyRefresh->getContentSize().height + 50))),
					TargetedAction::create(lyTables, MoveTo::create(0.5f,Vec2(lyTables->getPositionX(),SCREEN_HEIGHT - 220 - LAYER_HEIGHT - 300 - lyRefresh->getContentSize().height + 50)))),
					DelayTime::create(2.0f),
					CallFunc::create([=](){EntityMgr::instance()->getDispatch()->SendPacketQueryMoreGameResult(gameKind);}),
					Spawn::createWithTwoActions(
					TargetedAction::create(lyRefresh,MoveTo::create(0.5f,Vec2(lyRefresh->getPositionX(),SCREEN_HEIGHT - 220))),
					TargetedAction::create(lyTables, MoveTo::create(0.5f,Vec2(lyTables->getPositionX(),SCREEN_HEIGHT - 220 - LAYER_HEIGHT - 300)))),
					CallFunc::create([=](){isRun = false;}),
					nullptr));

				return;
			}

			m_bIsOpenList = true;

			maskLayer->setContentSize(cocos2d::Size(SCREEN_WIDTH, 700));
			betArea->setPositionY(-300);
			betArea->m_endPos.y += 300;	
			lyTables->runAction(MoveTo::create(0.1f,Vec2(lyTables->getPositionX(),SCREEN_HEIGHT - 220 -LAYER_HEIGHT - 300)));

			return;
		}	

		//收回来
		if(pTouch->getLocation().y - startPoint.y > 60  && (listTable->getContentOffset().y > 60 || (lyRefresh->convertToNodeSpace(startPoint).y > 0 && lyRefresh->convertTouchToNodeSpace(pTouch).y > 0)))
		{
			if(!m_bIsOpenList) return;
			m_bIsOpenList = false;

			maskLayer->setContentSize(cocos2d::Size(SCREEN_WIDTH, 120));
			betArea->setPositionY(0);
			betArea->m_endPos.y -= 300;	
			lyTables->runAction(MoveTo::create(0.1f,Vec2(lyTables->getPositionX(),SCREEN_HEIGHT - 220 - LAYER_HEIGHT)));

			return;
		}			

	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(refreshListener, lyRefresh);
}

void BJK8GambleLayer::updateLotteryId()
{
	static bool isRun = false;
	if(isRun) return;
	isRun = true;

	char tempStr[128] = {0};
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	sprintf(tempStr, "%I64d", currentLotteryId);
#else
	sprintf(tempStr, "%lld", currentLotteryId);
#endif
	if(wPeriod != NULL)
	{
		wPeriod->release();
		wPeriod = NULL;
	}
	wPeriod = String::create(tempStr);
	//CCLOG("nnnnnnnnnnnnnnnnn now period: %s", wPeriod->getCString());
	wPeriod->retain();
	String *lotteryIdString;
	lotteryIdString = String::createWithFormat("%s%s%s",
		ConfigMgr::instance()->text("display_text.xml", "t70"), tempStr,
		ConfigMgr::instance()->text("display_text.xml", "t71"));
	lotteryIdLabel->setString(lotteryIdString->getCString());

	NotificationCenter::getInstance()->postNotification("GDselectedItemTagNumberCartFailed",String::create(tempStr));

	isRun = false;
}

void BJK8GambleLayer::updateCurrentTime()
{
	String *headStr = NULL;
	m_bIsFendan = gameRule->IsFenDanDuration();
	if(currentLotteryId == 0)
	{
		headStr = String::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t1550"));
	} else
	{
		if(m_bIsFendan)//正在封单
		{
			headStr = String::create(STR_FDSJ);		
		} else
		{
			headStr = String::create(STR_XDSJ);
		}
	}

	currentTimeLabel->setString(String::createWithFormat("%s%s",headStr->getCString(), currentTime.c_str())->getCString());	
}

void BJK8GambleLayer::touZhuRet(Object *obj)
{
	MyBetNumber::getInstance()->betOneEnd();

	cart->setPositionX(2000);
	this->setKeypadEnabled(true);
	Array *data = (Array *)obj;
	Integer *lResult = (Integer *)data->objectAtIndex(0);
	String *resultDesc = (String *)data->objectAtIndex(1);
	Integer *nSign = (Integer *)data->objectAtIndex(2);

	//计数
	MyBetNumber::getInstance()->setRetType(lResult->getValue());
	if(!MyBetNumber::getInstance()->isEndBet()) return;
	int returnType = MyBetNumber::getInstance()->getRetType();

	if(returnType == 0){		
		EntityMgr::instance()->getDispatch()->f_yue-= TouZhuJinE;  

		TouZhuJinE =0;
		betZhuShuTouZhuJinE =0;
		beiShu = 1;
		beilvInput->setText("1");	

		MovingLabelLayer* layer =MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t180"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}
	else if(returnType > 0 && returnType <= 18)
	{
		int nTempType=returnType;
		//from T200 to T208
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", String::createWithFormat("t%ld", 200+ nTempType)->getCString()),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}
	else if(lResult->getValue() == 112)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t4600"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}else if(lResult->getValue() == 111)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t717"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}else if(lResult->getValue() > 18)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t186"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}else if(lResult->getValue() > 18){
		int temp = 1900 + lResult->getValue() * 2;
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(String::createWithFormat("%s%d%s",
			ConfigMgr::instance()->text("display_text.xml", "t718"),
			temp,
			ConfigMgr::instance()->text("display_text.xml", "t719"))->getCString(),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}else{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t182"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}

	EntityMgr::instance()->getDispatch()->SendPacketWithGetLastYue();
}

//top menu
void BJK8GambleLayer::setting(Object* obj)
{
	playButtonSound();

	return; //暂时没有

	LotteryHelpView *layer = LotteryHelpView::create();
	layer->setHelpId(gameKind);
	
	Scene *scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);
}

void BJK8GambleLayer::jumpResult(Object* obj)
{	
	playButtonSound();	

	//跳转到开奖
	LotteryGameResultDetail *layer = LotteryGameResultDetail::create();
	Scene *scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->pushScene(scene);	

	EntityMgr::instance()->getDispatch()->SendPacketQueryMoreGameResult(gameKind);
}

void BJK8GambleLayer::XuanZeMonenyLayer(Object *obj)
{
	//playButtonSound();

	int tag = dynamic_cast<Node*>(obj)->getTag();
	switch (tag)
	{
	case YuanShiShiCAITag:
		{
			YuanSelectedImageLayer->setEnabled(false);
			JiaoSelectedImageLayer->setEnabled(true);
			FenSelectedImageLayer->setEnabled(true);
			LiSelectedImageLayer->setEnabled(true);
			nLMoshi =0;
			break;
		}
	case JiaoShiShiCAITag:	
		{
			YuanSelectedImageLayer->setEnabled(true);
			JiaoSelectedImageLayer->setEnabled(false);
			FenSelectedImageLayer->setEnabled(true);	
			LiSelectedImageLayer->setEnabled(true);
			nLMoshi = 1;
			break;
		}
	case FenShiShiCAITag:
		{
			YuanSelectedImageLayer->setEnabled(true);
			JiaoSelectedImageLayer->setEnabled(true);
			FenSelectedImageLayer->setEnabled(false);	
			LiSelectedImageLayer->setEnabled(true);
			nLMoshi = 2;
			break;
		}
	case LiShiShiTag:
		{
			YuanSelectedImageLayer->setEnabled(true);
			JiaoSelectedImageLayer->setEnabled(true);
			FenSelectedImageLayer->setEnabled(true);	
			LiSelectedImageLayer->setEnabled(false);
			nLMoshi = 3;
		}
	default:
		{
			break;
		}
	}
	MyBetData::getInstance()->setLastMoShi(nLMoshi);
	updateMoneyLabel();
}

//middle menu
void BJK8GambleLayer::toMore(Object* obj)
{
	playButtonSound();

	MenuItemSprite *moreItem = (MenuItemSprite *)obj;
	if(kindLayerIsShow == false){
		this->showKindView();
		
	}else{
		this->hideKindView();		
	}
}

void BJK8GambleLayer::toRandomGamble(Object* obj)
{
	playButtonSound();

	if(betArea != NULL)
	{
		betArea->createRandomNumbers();
		betArea->getBetNumber();
		betZhuShuTouZhuJinE = betArea->getBetZhuShu();

		updateMoneyLabel();
	}
}
void BJK8GambleLayer::ZhuiHaoTouZhuJinENumber(Object *obj)
{
	float LinShiTouZhuJinE = ((Float *)obj)->getValue(); 
	TouZhuJinE = LinShiTouZhuJinE;
	MyBetNumber::getInstance()->clear();
	betArea->clearNumbers();
	betArea->cancelAllMenuSelect();
}

void BJK8GambleLayer::ShiYiXuanWoNumber(Object *obj)
{
	float ZhangHuYuENumber = ((Float *)obj)->getValue(); 
	ZhangHuYuE = ZhangHuYuENumber; 
	char szBuf2[100] = {0};  
	sprintf(szBuf2, "%.3f ",ZhangHuYuE);
	String *TouZhuYuEString = String::createWithFormat("%s %s%s",
		ConfigMgr::instance()->text("display_DuangDong.xml", "t3"),szBuf2,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t2"));
	//ZhangHuYuELabel->setString(TouZhuYuEString->getCString());
}
//bottom menu
void BJK8GambleLayer::toDelete(Object* obj)
{
	playButtonSound();

	if(betArea != NULL)
	{
		betArea->clearNumbers();
		betArea->cancelAllMenuSelect();
		betZhuShuTouZhuJinE = 0;		

		updateMoneyLabel();
	}

}

void BJK8GambleLayer::toCart(Object* obj)
{
	playButtonSound();

	bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
	if(!bRet)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t167"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}else{
		if(betArea == NULL){
			return;
		}
		String *betNumber = betArea->getBetNumber();
		CCLOG("%s", betNumber->getCString());
		if(strcmp(betNumber->getCString(), "") == 0){
			return;
		}		

		//判断注数限制
		//int maxCount = BetLimit::GetMaxBetFromId(gameKind,wanFaKind);
		//int nowTouzhu = betZhuShuTouZhuJinE;
		//if(maxCount < nowTouzhu)
		//{
		//	std::string strDisplay =
		//		BetLimit::GetWanFaNameFromId(gameKind,wanFaKind) +
		//		ConfigMgr::instance()->text("display_text.xml", "t954") +
		//		String::createWithFormat("%d",maxCount)->getCString() +
		//		ConfigMgr::instance()->text("display_text.xml", "t0");

		//	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(strDisplay.c_str(),
		//		Vec2(winSize.width * 0.5,winSize.height * 0.5));
		//	this->addChild(layer, 255);
		//	return;
		//}

		//if(!BetLimit::CheckDingWeiDanStr(wanFaKind,betNumber->getCString()))
		//{
		//	MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t184"),
		//		Vec2(winSize.width * 0.5,winSize.height * 0.5));
		//	this->addChild(layer, 255);
		//	return;
		//}

		Integer* b_gameKind = Integer::create(gameKind);
		Integer* b_wanFaKind = Integer::create(BetLimit::GetWanfaIdFromId(wanFaKind, 5));
		String* b_betNumber = String::create(betNumber->getCString());		
		Integer* b_betZhuShuTouZhuJinE = Integer::create(betArea->getBetZhuShu());
		Integer* b_nLMoshi = Integer::create(nLMoshi);
		Integer* b_beiShu = Integer::create(beiShu);

		b_gameKind->retain();
		b_wanFaKind->retain();
		b_betNumber->retain();
		b_betZhuShuTouZhuJinE->retain();
		b_nLMoshi->retain();
		b_beiShu->retain();

		Bet_Info temp = {
			b_gameKind,b_wanFaKind,b_betNumber,b_betZhuShuTouZhuJinE,b_nLMoshi,b_beiShu			
		};

		MyBetNumber::getInstance()->addItem(temp);	
		
		NotificationCenter::getInstance()->postNotification("RefreshData");

		betArea->clearNumbers();	
		//设置显示
		cart->setPosition(Vec2::ZERO);	
		cart->bottomMenuEnabled(true);		
		this->setKeypadEnabled(true);
	}
}

void BJK8GambleLayer::toGamble(Object* obj)
{
	playButtonSound();

	bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
	if(!bRet)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t167"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}else{
		if(betArea == NULL){
			return;
		}
		//封单		
		if(m_bIsFendan)
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t8"),
				Vec2(winSize.width * 0.5,winSize.height * 0.5));
			this->addChild(layer, 255);
			MyBetNumber::getInstance()->clear();
			betArea->clearNumbers();
			betArea->cancelAllMenuSelect();
			return;
		}
		String *betNumber = betArea->getBetNumber();
		int betZhuShu = betArea->getBetZhuShu();
		CCLOG("%s", betNumber->getCString());
		
		if(strcmp(betNumber->getCString(), "") == 0){
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t198"),
				Vec2(winSize.width * 0.5, winSize.height * 0.5));
			this->addChild(layer, 255);
			return;
		}
		if(wPeriod == NULL)
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t439"),
				Vec2(winSize.width * 0.5,winSize.height * 0.5));
			this->addChild(layer, 255);
			return;
		}
		
		//判断是否还在投注
		if(!MyBetNumber::getInstance()->isEndBet())
		{
			return;
		}

		MyBetNumber::getInstance()->betReady();

		char tempStr[20] = {0};
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		sprintf(tempStr, "%I64d", currentLotteryId);
#else
		sprintf(tempStr, "%lld", currentLotteryId);
#endif
		string qishuStr = string(tempStr);
		EntityMgr::instance()->getDispatch()->SendPacketWithTouZhuCQSSC(qishuStr, gameKind, BetLimit::GetWanfaIdFromId(wanFaKind, 5),
			betNumber->getCString(), betZhuShu, beiShu, nLMoshi, 1, 0, 0);
	}
	//清空数据
	MyBetNumber::getInstance()->clear();
	betArea->clearNumbers();
	betArea->cancelAllMenuSelect();
}

//更改玩法ID
void BJK8GambleLayer::changeKind(Object *obj)
{
	selectedItemTag = ((Integer *)obj)->getValue(); 
	//保存
	MyBetData::getInstance()->setLastWanfaId(tagKuai8[selectedItemTag]);
	//
	this->toMore(moreItem);
	//这里显示的是从T4001开始的名字
	//gambleTypeTitle->setString(ConfigMgr::instance()->text("display_text.xml", String::createWithFormat("t%d", 4000 +selectedItemTag)->getCString()));

	this->setWanFaKind(tagKuai8[selectedItemTag]);
	betArea->clearNumbers();
	betArea->cancelAllMenuSelect();
	betZhuShuTouZhuJinE = 0;

	EntityMgr::instance()->getDispatch()->SendPacketWithTouZhuCQSSC_FanDian(gameKind, BetLimit::GetWanfaIdFromId(wanFaKind,5));	

	updateMoneyLabel();

	//获得表格
	auto layerTable = this->getChildByTag(9527);
	if(layerTable == nullptr) return;
	auto layerResult = layerTable->getChildByTag(1001);
	if(layerResult == nullptr) return;
	auto menuTable = (TableView *)layerResult->getChildByTag(1000);
	if(menuTable == nullptr) return;
	menuTable->reloadData();

	int iPos = 0;
	for(int i = 0;i < MyBetData::getInstance()->getWanfaSize();i ++)
	{
		if(MyBetData::getInstance()->getWanfaAt(i) == selectedItemTag)
		{			
			iPos = i;
			break;
		}
	}

	menuTable->setContentOffset(Vec2((-iPos / 3) * menuTable->getContentSize().width / 3,0));
}

void BJK8GambleLayer::deleteItem(Object *obj)
{
	int deleteItem = ((Integer *)obj)->getValue(); 
	//vecotr删除
	MyBetNumber::getInstance()->removeById(deleteItem);	
	
	NotificationCenter::getInstance()->postNotification("RefreshData",nullptr);
}

void BJK8GambleLayer::addRandomItem(Object *obj)
{
	this->setKeypadEnabled(true);
	betZhuShuTouZhuJinE = 0;
	beilvInput->setText("1");
	beiShu = 1;

	updateMoneyLabel();
}

void BJK8GambleLayer::cartConfirm(Object *obj)
{	
	if(MyBetNumber::getInstance()->size() < 1) return; 

	//判断是否还在投注
	if(!MyBetNumber::getInstance()->isEndBet())
	{
		return;
	}

	//封单		
	if(m_bIsFendan)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t8"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);		
		return;
	}	

	MyBetNumber::getInstance()->betReady();

	for(int i = 0 ; i < MyBetNumber::getInstance()->size();i ++){

		Bet_Info tempInfo = MyBetNumber::getInstance()->getItem(i);

		Integer *gameKind = tempInfo.m_gameKind;
		Integer *wanFaKind = tempInfo.m_wanFaKind;
		String *numbers = tempInfo.m_betNumber;   //投注号码
		Integer *singleZhuShu = tempInfo.m_betCount;	//注数
		Integer *moshi = tempInfo.m_mode;	//模式
		Integer *beishu = tempInfo.m_betPlus;	//倍数

		if(strcmp(numbers->getCString(), "") == 0){
			return;
		}

		if(wPeriod == NULL)
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t439"),
				Vec2(winSize.width * 0.5,winSize.height * 0.5));
			this->addChild(layer, 255);
			return;
		}

		char tempStr[20] = {0};
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		sprintf(tempStr, "%I64d", currentLotteryId);
	#else
		sprintf(tempStr, "%lld", currentLotteryId);
	#endif
		string qishuStr = string(tempStr);			
			
		EntityMgr::instance()->getDispatch()->SendPacketWithTouZhuCQSSC(qishuStr, gameKind->getValue(), wanFaKind->getValue(),
			numbers->getCString(), singleZhuShu->getValue(), beishu->getValue(), moshi->getValue(), 1, 0, 0);	


	}
	//投注完毕，清空
	MyBetNumber::getInstance()->clear();
	betArea->clearNumbers();
	betArea->cancelAllMenuSelect();
}

void BJK8GambleLayer::clearNumbers(Object *obj)
{
	//其它情况，清空
	MyBetNumber::getInstance()->clear();

	NotificationCenter::getInstance()->postNotification("RefreshData", nullptr);
}

void BJK8GambleLayer::getSystemTimeObject(Object *obj)
{
	BaseLayer::getSystemTimeObject(nullptr);

	//Dictionary *data = (Dictionary *)obj;
	//wYear = ((Integer *)data->objectForKey("wYear"))->getValue();
	//wMonth = ((Integer *)data->objectForKey("wMonth"))->getValue();
	//wDay = ((Integer *)data->objectForKey("wDay"))->getValue();
	//wHour = ((Integer *)data->objectForKey("wHour"))->getValue();
	//wMinute = ((Integer *)data->objectForKey("wMinute"))->getValue();
	//wSecond = ((Integer *)data->objectForKey("wSecond"))->getValue();
	//if (gameKind == CZChongQingSSC)
	//{
	//	this->calculateCQSSCQiShu();
	//}
}

void BJK8GambleLayer::calculateCQSSCQiShu()
{

}

void BJK8GambleLayer::onEnter()
{
	BaseLayer::onEnter();
}

void BJK8GambleLayer::onExit()
{
	BaseLayer::onExit();
}

void BJK8GambleLayer::back(Object *obj)
{
	playButtonSound();

	//菜单
	if(kindLayerIsShow)
	{
		hideKindView();
		return;
	}

	//列表
	if(cart->getPosition() == Vec2::ZERO)
	{
		cart->setPosition(Vec2(0,2000));
		return;
	}

	MyBetNumber::getInstance()->clear();

	//将table隐藏起来
	if(lyRefresh)
	{
		lyRefresh->setPosition(Vec2(0,SCREEN_HEIGHT - 220));
		if(lyTables && lyTables->getPositionY() == lyRefresh->getPositionY() - LAYER_HEIGHT)
		{			
			auto tbList = (TableView *)lyTables->getChildByTag(8888);
			if(tbList != nullptr)
			{
				tbList->setVisible(false);
			}			
		}		
	}

	RoomLayer* layer = RoomLayer::create();
	layer->automaticLogin(false);
	Scene* scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(LotterySceneControl::sharedSceneControl()->getScene(scene));
}

void BJK8GambleLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void BJK8GambleLayer::showKindView()
{
	if(kindLayerIsShow) return;

	if(kindLayer)
	{
		kindLayer->removeFromParentAndCleanup(true);
		kindLayer = nullptr;
	}

	//彩种下拉框选择
	kindLayer = BJK8Kind::create();
	kindLayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	kindLayer->setPosition(Vec2(0,0));
	this->addChild(kindLayer, 2);

	LayerColor* layer = (LayerColor *)this->getChildByTag(9527);
	if(layer == nullptr) return;
	layer->setPosition(SCREEN_WIDTH - 100, 0);

	kindLayerIsShow = true;
}

void BJK8GambleLayer::hideKindView()
{
	LayerColor* layer = (LayerColor *)this->getChildByTag(9527);
	if(layer == nullptr) return;
	layer->setPosition(0, 0);

	if(kindLayer)
	{	
		kindLayer->removeFromParentAndCleanup(true);
		kindLayer = nullptr;
	}
	kindLayerIsShow = false;

	//获得表格
	auto layerTable = this->getChildByTag(9527);
	if(layerTable == nullptr) return;
	auto layerResult = layerTable->getChildByTag(1001);
	if(layerResult == nullptr) return;
	auto menuTable = (TableView *)layerResult->getChildByTag(1000);
	if(menuTable == nullptr) return;
	menuTable->reloadData();

	int iPos = 0;
	for(int i = 0;i < MyBetData::getInstance()->getWanfaSize();i ++)
	{
		if(MyBetData::getInstance()->getWanfaAt(i) == selectedItemTag)
		{			
			iPos = i;
			break;
		}
	}

	menuTable->setContentOffset(Vec2((-iPos / 3) * menuTable->getContentSize().width / 3,0));	
}

void BJK8GambleLayer::updateTouZhuJine(Object *obj)
{
	betArea->getBetNumber();
	betZhuShuTouZhuJinE = betArea->getBetZhuShu();

	updateMoneyLabel();
}
void BJK8GambleLayer::GetLastYueRet(Object *obj)
{
	Dictionary *dic = (Dictionary *)obj;
	Double *f_t_yue = (Double *)dic->objectForKey("f_t_yue");
	Double *f_t_dongjie = (Double *)dic->objectForKey("f_t_dongjie");
	Double *f_t_score = (Double *)dic->objectForKey("f_t_score");
	ZhangHuYuE =f_t_yue->getValue();

	updateMoneyLabel();
}

//------------------------------------------------新增----------------------------------------------------------------
void BJK8GambleLayer::editBoxEditingDidEnd(EditBox* editBox)
{
	string str = editBox->getText();
	beiShu = atoi(str.c_str());

	if(beiShu <=0)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t169"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
		beilvInput->setText("1");
		beiShu = 1;
	}else if(beiShu > MAX_BEISHU)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t170"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
		beilvInput->setText("1");
		beiShu = 1;
	}

	updateMoneyLabel();
}

void BJK8GambleLayer::FanDianMinus(Object *obj)
{
	playButtonSound();

	//创建半透明层
	layerColor = LayerColor::create(ccc4(0,0,0,100));//ccc3+透明度
	this->addChild(layerColor,10);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(BJK8GambleLayer::onTouchBegan,this);
	listener->onTouchEnded = CC_CALLBACK_2(BJK8GambleLayer::onTouchEnded,this);
	listener->onTouchMoved = CC_CALLBACK_2(BJK8GambleLayer::onTouchMoved,this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, layerColor);

	static int tempInt = 25;

	//进度条背景
	Sprite *VipLagerBack  = Sprite::createWithSpriteFrame(spriteFrame("beting39.png"));
	VipLagerBack->setAnchorPoint(Vec2(0.5,0.5));
	VipLagerBack->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	layerColor->addChild(VipLagerBack,10);
	fandian = (EntityMgr::instance()->getDispatch()->f_fandian)*100;
	fandianubt =fandian*10;

	//显示百分比
	numttf=LabelTTF::create("","",28);
	numttf->setAnchorPoint(Vec2(0.5,0.5));
	numttf->setPosition(Vec2(winSize.width*0.8,winSize.height*0.48 + 5));
	numttf->setColor(ccc3(0,0,0));
	numttf->setString(String::createWithFormat("%.3f",PlusFanDianvalieMAX)->getCString());
	layerColor->addChild(numttf,10);
	numLingttf=LabelTTF::create("","",28);
	numLingttf->setPosition(Vec2(winSize.width*0.2 - 20,winSize.height*0.48 + 5));
	numLingttf->setColor(ccc3(0,0,0));
	numLingttf->setAnchorPoint(Vec2(0.5,0.5));
	char str[33];
	sprintf(str, "%.1f", BaiFenbiNumber);
	string userId=string(str);
	string urlExtra = "%";
	string url;
	url = userId+urlExtra;
	numLingttf->setString(url.c_str());
	layerColor->addChild(numLingttf,10,522);
	//游标
	henggangSprite = Sprite::createWithSpriteFrame(spriteFrame("beting42.png"));
	henggangSprite->setAnchorPoint(Vec2(0.5,0.5));
	henggangSprite->setPosition(Vec2(winSize.width*0.2,winSize.height*0.5 + tempInt));
	layerColor->addChild(henggangSprite,16);
	henggangSprite->setPositionX(winSize.width*0.2+((winSize.width*0.58)/fandianubt*BaiFenbiNumber*10));
	//进度条背景
	Sprite *pJinDuTiaoBack = Sprite::createWithSpriteFrame(spriteFrame("beting44.png"));
	layerColor->addChild(pJinDuTiaoBack,15);
	//红色进度条
	Sprite *pJinDuTiao1 = Sprite::createWithSpriteFrame(spriteFrame("beting43.png"));
	pJinDuTiao = ProgressTimer::create(pJinDuTiao1);
	/**先设置进度条从左到右走**/
	pJinDuTiao->setAnchorPoint(Vec2(0.5,0.5));
	pJinDuTiao->setMidpoint(Vec2(0,0));  
	pJinDuTiao->setBarChangeRate(Vec2(1, 0));
	pJinDuTiao->setType(kCCProgressTimerTypeBar);
	pJinDuTiao->setPercentage(percent);
	if (BaiFenbiNumber)
	{
		pJinDuTiaoBack->setPosition(Vec2(winSize.width / 2, winSize.height*0.5 + tempInt + 5));
		pJinDuTiao->setPosition(Vec2(winSize.width / 2, winSize.height*0.5 + tempInt + 5));
	}
	else
	{
		pJinDuTiaoBack->setPosition(Vec2(winSize.width / 2 - 5, winSize.height*0.5 + tempInt));
		pJinDuTiao->setPosition(Vec2(winSize.width / 2 - 5, winSize.height*0.5 + tempInt));
	}

	lenJindu = pJinDuTiaoBack->getContentSize().width / 100;
	JinduPosX = winSize.width / 2 - pJinDuTiaoBack->getContentSize().width / 2;
	henggangSprite->setPositionX( JinduPosX + (BaiFenbiNumber * 100 / fandian) * lenJindu);

	layerColor->addChild(pJinDuTiao,15);
	percent =(henggangSprite->getPositionX()-winSize.width*0.2)/(winSize.width*0.58)*100;
	if(percent<100.f){pJinDuTiao->setPercentage(percent);
	}else{
		pJinDuTiao->setPercentage(100);
	}
	//确定  取消
	Sprite *MakeSureSprite_1  = Sprite::createWithSpriteFrame(spriteFrame("beting22.png"));
	Sprite *MakeSureSprite_2  = Sprite::createWithSpriteFrame(spriteFrame("beting23.png"));
	Sprite *PCloseItem_1  = Sprite::createWithSpriteFrame(spriteFrame("beting24.png"));
	Sprite *PCloseItem_2  = Sprite::createWithSpriteFrame(spriteFrame("beting25.png"));
	MenuItemSprite* MakeSureMenuItem = MenuItemSprite::create(MakeSureSprite_1,MakeSureSprite_2,CC_CALLBACK_1(BJK8GambleLayer::SuccenGameResume,this));
	MenuItemSprite* PCloseItemMenuItem = MenuItemSprite::create(PCloseItem_1,PCloseItem_2,CC_CALLBACK_1(BJK8GambleLayer::pCoseGameResume,this));
	Menu* FanDianmenu = Menu::create(PCloseItemMenuItem,MakeSureMenuItem,NULL);
	FanDianmenu->setAnchorPoint(Vec2(0.5,0.5));
	FanDianmenu->setPosition(Vec2(winSize.width*0.5,winSize.height*0.44));
	FanDianmenu->alignItemsHorizontallyWithPadding(30);
	layerColor->addChild(FanDianmenu,10);
	//加减
	Sprite *MinusValueSprite_1  = Sprite::createWithSpriteFrame(spriteFrame("beting41.png"));
	Sprite *MinusValueSprite_2  = Sprite::createWithSpriteFrame(spriteFrame("beting41.png"));
	Sprite *PlusValueSprite_1  = Sprite::createWithSpriteFrame(spriteFrame("beting40.png"));
	Sprite *PlusValueSprite_2  = Sprite::createWithSpriteFrame(spriteFrame("beting40.png"));
	MenuItemSprite* MinusMenuItem = MenuItemSprite::create(MinusValueSprite_1,MinusValueSprite_2,CC_CALLBACK_1(BJK8GambleLayer::PlusFianDian,this));
	MinusMenuItem->setAnchorPoint(Vec2(0.5,0.5));
	MinusMenuItem->setPosition(Vec2(winSize.width*0.84 + 10,winSize.height*0.5 + tempInt));
	MenuItemSprite* PlusMenuItem = MenuItemSprite::create(PlusValueSprite_1,PlusValueSprite_2,CC_CALLBACK_1(BJK8GambleLayer::MinusFanDian,this));
	PlusMenuItem->setAnchorPoint(Vec2(0.5,0.5));
	PlusMenuItem->setPosition(Vec2(winSize.width*0.13 + 10,winSize.height*0.5 + tempInt));
	Menu* TianJiaMenu = Menu::create(MinusMenuItem,PlusMenuItem,NULL);
	TianJiaMenu->setAnchorPoint(Vec2(0.5,0.5));
	TianJiaMenu->setPosition(Vec2::ZERO);
	layerColor->addChild(TianJiaMenu,10);

#ifdef VER_QIANBO
	numttf->setColor(ccc3(255,255,255));
	numLingttf->setColor(ccc3(255,255,255));
#endif

#ifdef VER_369
	numttf->setColor(ccc3(255,255,255));
	numLingttf->setColor(ccc3(255,255,255));
	FanDianmenu->setPositionY(FanDianmenu->getPositionY() - 10);
	FanDianmenu->alignItemsHorizontallyWithPadding(50);
#endif
}

void BJK8GambleLayer::PlusFianDian(Object *obj)
{
	playButtonSound();

	float PlusFanDianvalie =PlusFanDianvalieMAX;
	if(BaiFenbiNumber>(fandian-0.01))
	{
		return;
	}
	if(PlusFanDianvalie<0 ||BaiFenbiNumber<0)
	{
		return;
	}
	SlidePlusde = true;
	BaiFenbiNumber+=0.1;
	if (BaiFenbiNumber > fandian)
	{
		BaiFenbiNumber = fandian;		
	} 	
	PlusFanDianvalie-=fandianNumber;
	float temp = EntityMgr::instance()->getDispatch()->f_fandian * 100;
	if (PlusFanDianvalie < temp)
	{
		PlusFanDianvalie = temp;
	}	
	char str[33];
	char str2[33];
	sprintf(str, "%.1f", BaiFenbiNumber);
	sprintf(str2, "%.3f", PlusFanDianvalie);
	string userId=string(str);
	string userId2=string(str2);
	string url = "%";
	string ur2 = userId+url;
	numLingttf->setString(ur2.c_str());
	numttf->setString(userId2.c_str());
	pJinDuTiao->setVisible(true);
	henggangSprite->setVisible(true);
	PlusFanDianvalieMAX =PlusFanDianvalie;
	//henggangPointX =henggangSprite->getPositionX();
	percent = BaiFenbiNumber * 100 / fandian;
	CCLOG("before %.3f",percent);
	if (percent > 100) percent = 100;
	CCLOG("after %.3f",percent);
	henggangPointX = JinduPosX + percent * lenJindu;//henggangPointX+14.2+(20-fandianubt)* 0.2;		//步长	
	henggangSprite->setPositionX(henggangPointX);	

	pJinDuTiao->setPercentage(percent);	   
}

void BJK8GambleLayer::MinusFanDian(Object *obj)
{
	playButtonSound();

	float PlusFanDianvalie =PlusFanDianvalieMAX;
	CCLOG("%.3f",BaiFenbiNumber);
	if(BaiFenbiNumber<0.01)
	{
		return;
	}
	if(PlusFanDianvalie<0 ||BaiFenbiNumber<0)
	{
		return;
	}
	BaiFenbiNumber-=0.1;
	if(BaiFenbiNumber<0.0)
	{
		BaiFenbiNumber =0.0;
	}
	PlusFanDianvalie+=fandianNumber;
	char str[33];
	char str2[33];
	sprintf(str, "%.1f", BaiFenbiNumber);
	sprintf(str2, "%.3f", PlusFanDianvalie);
	string userId=string(str);
	string userId2=string(str2);
	string url = "%";
	string ur2 = userId+url;
	numLingttf->setString(ur2.c_str());
	numttf->setString(userId2.c_str());
	pJinDuTiao->setVisible(true);
	henggangSprite->setVisible(true);
	PlusFanDianvalieMAX =PlusFanDianvalie;
	//henggangPointX =henggangSprite->getPositionX();
	percent = BaiFenbiNumber * 100 / fandian;
	if (percent < 0) percent = 0;
	henggangPointX = JinduPosX + percent * lenJindu;//henggangPointX+14.2+(20-fandianubt)* 0.2;		//步长	
	henggangSprite->setPositionX(henggangPointX);	

	pJinDuTiao->setPercentage(percent);	
}

void BJK8GambleLayer::pCoseGameResume(Object *obj)
{
	PlusFanDianvalieMAX =GlobalVariableFanDianvalie;
	BaiFenbiNumber = FandianNumberpColse;
	this->removeChild(layerColor, true);
	char str[33];
	char str2[33];
	sprintf(str, "%.1f", FandianNumberpColse);
	sprintf(str2, "%.3f", PlusFanDianvalieMAX);
	string userId=string(str);
	string userId2=string(str2);
	string url = "/";
	string ur2 = "%";
	string ur3 =userId2+url+userId+ur2;
	Cardnumttf->setString(ur3.c_str());
}

void BJK8GambleLayer::SuccenGameResume(Object *obj)
{
	this->removeChild(layerColor, true);
	char str[33];
	char str2[33];
	sprintf(str, "%.1f", BaiFenbiNumber);
	sprintf(str2, "%.3f", PlusFanDianvalieMAX);
	string userId=string(str);
	string userId2=string(str2);
	string url = "/";
	string ur2 = "%";
	string ur3 =userId2+url+userId+ur2;
	Cardnumttf->setString(ur3.c_str());

	bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
	if(!bRet)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t167"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}else{
		EntityMgr::instance()->getDispatch()->SendPacketWithGetTouZhuSetUserBonus(gameKind, BetLimit::GetWanfaIdFromId(wanFaKind,5), PlusFanDianvalieMAX, BaiFenbiNumber);
	}
}

/**触摸开始*/
bool BJK8GambleLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{	
	Vec2 pos = Director::getInstance()->convertToGL(pTouch->getLocationInView());

	startPoint = pos;

	if (pos.x < JinduPosX || pos.x > JinduPosX + 100 * lenJindu) return true;
	if (pos.y < winSize.height / 2 - 10 || pos.y > winSize.height / 2 + 60) return true;	 //增加范围

	char str[33];
	char str2[33];

	//BaiFenbiNumber = ((pos.x-winSize.width*0.2)/(winSize.width*0.58/fandianubt))/10; ceil(1.5f);
	BaiFenbiNumber = (pos.x - JinduPosX) / lenJindu * fandian / 100;

	//去掉1位小数后面的
	BaiFenbiNumber = std::ceil(BaiFenbiNumber * 10) / 10;

	float PlusFanDianvalie =0;
	PlusFanDianvalie  =(GlobalVariableFanDianvalie+(fandianNumber*(FanDianbI)*10))-(fandianNumber*(BaiFenbiNumber)*10);
	//去掉2位小数后面
	PlusFanDianvalie = int(PlusFanDianvalie * 100) / 100.0f;

	sprintf(str, "%.1f", BaiFenbiNumber);
	sprintf(str2, "%.3f", PlusFanDianvalie);
	PlusFanDianvalieMAX =PlusFanDianvalie;
	string userId=string(str);
	string userId2=string(str2);
	string url = "%";
	string ur2 = userId+url;
	numLingttf->setString(ur2.c_str());
	numttf->setString(userId2.c_str());
	henggangSprite->setPositionX(winSize.width*0.2+((winSize.width*0.58)/fandianubt*BaiFenbiNumber*10));
	percent =(henggangSprite->getPositionX()-winSize.width*0.2)/(winSize.width*0.58)*100;
	if(percent<100.f){
		pJinDuTiao->setPercentage(percent);
	}else{
		pJinDuTiao->setPercentage(100);
	}
	return true;
}

void BJK8GambleLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void BJK8GambleLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{	

}

void BJK8GambleLayer::KindMyFanDianRet(Object *obj)
{
	isReceiveFanDian = true;

	Array *data = (Array *)obj;
	int count = data->count();
	if (count == 0) return;
	int myWanFa = BetLimit::GetWanfaIdFromId(wanFaKind,5);
	for (int i = 0;i < count; i += 4)
	{
		Integer *id = (Integer*) data->objectAtIndex(i);
		if (id->getValue() == myWanFa)
		{
			if (n_t_kindid != NULL)
			{
				n_t_kindid->release();
				f_t_bonus->release();
				f_t_bonusPercent->release();
				f_t_fandian->release();
			}
			n_t_kindid = (Integer *)data->objectAtIndex(0 + i);
			f_t_bonus = (String *)data->objectAtIndex(1 + i);
			f_t_bonusPercent = (String *)data->objectAtIndex(2 + i);
			f_t_fandian = (String *)data->objectAtIndex(3 + i);
			n_t_kindid->retain();
			f_t_bonus->retain();
			f_t_bonusPercent->retain();
			f_t_fandian->retain();
			break;
		}
	}

	GlobalVariableFanDianvalie =atof(f_t_bonus->getCString());
	BaiFenbiNumber =atof(f_t_bonusPercent->getCString());
	fandianNumber = (atof(f_t_fandian->getCString()))/10;
	char str[33];
	char str2[33];
	sprintf(str, "%.3f", GlobalVariableFanDianvalie);
	sprintf(str2, "%.1f", BaiFenbiNumber);
	string userId1=string(str);
	string userId2=string(str2);
	string urlExtra1 = "/";
	string urlExtra2 = "%";
	string url;
	url = userId1+urlExtra1+userId2+urlExtra2;
	Cardnumttf->setString(url.c_str());
	FandianNumberpColse =atof(f_t_bonusPercent->getCString());
	PlusFanDianvalieMAX=GlobalVariableFanDianvalie;
	FanDianbI = atof(f_t_bonusPercent->getCString());
}

void BJK8GambleLayer::pressMinus(Object *obj)
{
	int beilvNum = atoi(beilvInput->getText());
	if(beilvNum>1){
		beilvNum--;
		beilvInput->setText(String::createWithFormat("%d", beilvNum)->getCString());
		string str = beilvInput->getText();
		beilvInput->setPositionX(beilvInputMidX-(str.length()-1)*5);
		
	}
}

void BJK8GambleLayer::isFenDanHour(Object *obj)
{
	NotificationCenter::getInstance()->postNotification("IsFendanHourRet", Bool::create(m_bIsFendan));
}

void BJK8GambleLayer::kuaNianExpect(Object *obj)
{
	Dictionary* data = (Dictionary *)obj;
	Integer* I_KindId = (Integer *)data->objectForKey("n_t_kindid");
	Integer* I_Ifts = (Integer *)data->objectForKey("c_t_ifts");
	String* S_Expect = (String *)data->objectForKey("s_t_expect");
	String* S_Time = (String *)data->objectForKey("s_t_shijian");

	//有可能是上一个返回的
	if(I_KindId->getValue() != gameKind) return;
	gameRule->setIsStopSell(I_Ifts->getValue());	
	gameRule->SetStartQiHao(atoi(S_Expect->getCString()));
	gameRule->SetStartTime(S_Time->getCString());
}

void BJK8GambleLayer::beiAdd(Object *obj)
{
	playButtonSound();

	beiShu --;
	beilvInput->setText(String::createWithFormat("%d", beiShu)->getCString());
	//直接调
	editBoxEditingDidEnd(beilvInput);
}

void BJK8GambleLayer::beiMinus(Object *obj)
{
	playButtonSound();

	beiShu ++;
	beilvInput->setText(String::createWithFormat("%d", beiShu)->getCString());
	//省代码
	editBoxEditingDidEnd(beilvInput);
}

void BJK8GambleLayer::updateMoneyLabel()
{
	float temoMoshi =0.00;
	if(nLMoshi ==0){temoMoshi =1 ;}
	else if(nLMoshi ==1){temoMoshi = 0.1;}
	else if(nLMoshi ==2){temoMoshi = 0.01;}
	else if(nLMoshi == 3){temoMoshi = 0.001;}
	TouZhuJinE =temoMoshi*betZhuShuTouZhuJinE*2*beiShu;
	//显示投注金额，账户余额
	char szBuf[100] = { 0 };
	char szBuf1[100] = { 0 };
	char szBuf2[100] = { 0 };
	sprintf(szBuf, "%d ", betZhuShuTouZhuJinE);
	sprintf(szBuf1, "%0.3f ", TouZhuJinE);
	sprintf(szBuf2, "%0.3f ", ZhangHuYuE);

	String *TouZhuJinEString = String::createWithFormat("%s%s%s%s  %s %s%s", szBuf,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t4"), szBuf1,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t2"),
		ConfigMgr::instance()->text("display_DuangDong.xml", "t3"), szBuf2,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t2"));

	TouZhuJinELabel->setString(TouZhuJinEString->getCString());	
}

void BJK8GambleLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
	if(table->getTag() == 1000)
	{
		int id = cell->getTag() - 100;

		selectedItemTag = MyBetData::getInstance()->getWanfaAt(id);

		this->setWanFaKind(tagKuai8[selectedItemTag]);
		betArea->clearNumbers();
		betArea->cancelAllMenuSelect();
		betZhuShuTouZhuJinE = 0;
		EntityMgr::instance()->getDispatch()->SendPacketWithTouZhuCQSSC_FanDian(gameKind, BetLimit::GetWanfaIdFromId(wanFaKind,0));	
		updateMoneyLabel();

		//更新
		for(int i = 0;i < MyBetData::getInstance()->getWanfaSize();i ++)
		{
			table->updateCellAtIndex(i);
		}		
	}
}

cocos2d::Size BJK8GambleLayer::cellSizeForTable(TableView *table)
{
	cocos2d::Size result;

	if(table->getTag() == 8888)
	{
		result.width = SCREEN_WIDTH;
		result.height = 70;
	}else
		if(table->getTag() == 1000) //菜单
		{
			result.width = SCREEN_WIDTH / 4;
			result.height = 60;
		}

		return result;
}

TableViewCell* BJK8GambleLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell* cell = TableViewCell::create();
	cell->setTag(idx + 100);
	cell->autorelease();

	if(table->getTag() == 1000) //菜单
	{
		if(MyBetData::getInstance()->getWanfaSize() <= idx) return cell;

		int nowPos =MyBetData::getInstance()->getWanfaAt(idx);
		const char* tempStr = ConfigMgr::instance()->text("display_kuaile8.xml",String::createWithFormat("t%d",20 +  tagKuai8[nowPos])->getCString());
		LabelTTF* ltfCaption = LabelTTF::create(tempStr,"",30);
		ltfCaption->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		ltfCaption->setTag(7777);
		ltfCaption->setPosition(Vec2(100,25));		
		cell->addChild(ltfCaption,1);

		//颜色
		if(nowPos == selectedItemTag)
		{
			ltfCaption->setFontFillColor(ccc3(255,255,255));
		}else
		{
			ltfCaption->setFontFillColor(ccc3(150,150,150));
		}

	}else
		if(table->getTag() == 8888)
		{
			//没有值
			if(m_data == nullptr ||m_data->count() == 0 || idx * 4 + 2 > m_data->count()) return cell;

			String *wPeriod = (String *)m_data->objectAtIndex(idx*4+1);
			String *szLotNum = (String *)m_data->objectAtIndex(idx*4+2);

			std::string result=ConfigMgr::instance()->text("display_text.xml", "t454");
			result.append(wPeriod->getCString());
			result.append(ConfigMgr::instance()->text("display_text.xml", "t455"));

			//第N期
			auto redColor = ccc3(61,8,40);
			LabelTTF *descibeLabel1 = LabelTTF::create(result.c_str(), "", 28);
			descibeLabel1->setColor(redColor);
			descibeLabel1->setAnchorPoint(Vec2(0,0.5f));
			descibeLabel1->setPosition(Vec2(32, 70/2 - 4));
			cell->addChild(descibeLabel1, 1);

			redColor = ccc3(100,0,0);
			result=szLotNum->getCString();
			int step = 1;
			if (result.length() >= 10)
				step = 2;

			transferString(result, step);

			LabelTTF *descibeLabel2 = LabelTTF::create(result.c_str(), "", 28);
			descibeLabel2->setColor(redColor);
			descibeLabel2->setAnchorPoint(Vec2(0,0.5f));
			descibeLabel2->setHorizontalAlignment(TextHAlignment::LEFT);
			descibeLabel2->setPosition(Vec2(descibeLabel1->getContentSize().width + 100, 70/2 - 4));
			cell->addChild(descibeLabel2, 1);

			ccColor3B clBackground = ccc3(200, 200, 200);

#ifdef VER_QIANBO
			clBackground = ccc3(68, 185, 110);
#endif	

#ifdef VER_369
			clBackground = ccc3(245, 245, 245);
			descibeLabel1->setColor(ccc3(76,76,76));
			descibeLabel2->setColor(ccc3(50, 62, 123));
#endif

			if (idx % 2 == 0)
			{
				Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12_2.png"));
				gap1->setAnchorPoint(Vec2(0.5f, 0));
				gap1->setPosition(Vec2(SCREEN_WIDTH / 2, 0));
				gap1->setScaleX(SCREEN_WIDTH / gap1->getContentSize().width);
				gap1->setScaleY(70 / gap1->getContentSize().height);
				cell->addChild(gap1, 0);
				gap1->setColor(clBackground);

#ifdef VER_369
				gap1->setOpacity(50);
#endif
			}
		}

		return cell;
}

ssize_t BJK8GambleLayer::numberOfCellsInTableView(TableView *table)
{
	if(table->getTag() == 1000) //菜单
	{
		return MyBetData::getInstance()->getWanfaSize();
	}
	if(table->getTag() == 8888)
	{
		int result = m_data->count() / 4;
		if(result > 20) result = 20;
		return result;
	}

	return 0;
}

void BJK8GambleLayer::scrollViewDidScroll(ScrollView* view)
{

}

void BJK8GambleLayer::scrollViewDidZoom(ScrollView* view)
{

}

void BJK8GambleLayer::transferString(string &str, int nStep)
{
	bool bTeMa = false;
	int nPos = str.find("|");
	if (nPos >= 0)
	{
		str.erase(nPos, 1);
		bTeMa = true;
	}
	int strLen = (int)str.length();
	for (int i = strLen; i >= 0; i -= nStep)
	{
		str.insert(i, "  ");

		if(strLen > 20 && i == strLen / 2)
			str.insert(i,"\n");			
	}
	if (bTeMa)
		str.insert(str.length() - 4, "| ");
}

void BJK8GambleLayer::queryGameMoreRet(Object *obj)
{
	if(m_data == nullptr) return;

	m_data->removeAllObjects();
	Array *receivedData = (Array *)obj;
	m_data->addObjectsFromArray(receivedData);

	//获得表格
	auto layer = this->getChildByTag(9527);
	if(layer == nullptr) return;
	auto layerResult = layer->getChildByTag(1001);
	if(layerResult == nullptr) return;
	auto gameResultTable = (TableView *)layerResult->getChildByTag(8888);
	if(gameResultTable == nullptr) return;
	gameResultTable->reloadData();
}