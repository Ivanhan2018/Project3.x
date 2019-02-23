#include "LiuHeGambleLayer.h"
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

#define CHOUMA_BOX 8321
#define CHOUMA_BUTTON  9321

using namespace CocosDenshion;
using namespace extension;

Scene* LiuHeGambleLayer::scene()
{
	Scene *scene = Scene::create();
	LiuHeGambleLayer *layer = LiuHeGambleLayer::create();
	scene->addChild(layer);
	return scene;
}

LiuHeGambleLayer::LiuHeGambleLayer()
{
	MyBetNumber::addRandSeed();
	wKindID = NULL;
	wPeriod = NULL;
	szLotNum = NULL;
	szShijian = NULL;
	gameRule = NULL;
	gameKind = CZ_HK6HeCai;
	betZhuShuTouZhuJinE =0;
	TouZhuJinE =0.00;
	beiShu = 0;
	betArea = NULL;
	ZhangHuYuE = EntityMgr::instance()->getDispatch()->f_yue;

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LiuHeGambleLayer::GetLastYueRet), "GetLastYueRet", NULL);  ///余额查询	
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LiuHeGambleLayer::touZhuRet), "CQSSCRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LiuHeGambleLayer::changeKind), "CQSSCChangeKind", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LiuHeGambleLayer::updatePeiLv), "UpdatePeiLv", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LiuHeGambleLayer::updateTouZhu), "UpdateTouZhu", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LiuHeGambleLayer::updateFenDan), "UpdateFenDan", NULL);

	//跨年期号处理
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LiuHeGambleLayer::kuaNianExpect), "KuaNian", NULL);

	//结果查询
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LiuHeGambleLayer::queryGameMoreRet), "QueryMoreGameRet", NULL);
}

void LiuHeGambleLayer::getSystemTime(float dt)
{
	
}

LiuHeGambleLayer::~LiuHeGambleLayer()
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
	NotificationCenter::getInstance()->removeObserver(this, "CQSSCRet");
	NotificationCenter::getInstance()->removeObserver(this, "CQSSCChangeKind");
	NotificationCenter::getInstance()->removeObserver(this, "UpdatePeiLv");
	NotificationCenter::getInstance()->removeObserver(this, "UpdateFenDan");
	NotificationCenter::getInstance()->removeObserver(this, "UpdateTouZhu");
	this->unschedule(schedule_selector(LiuHeGambleLayer::myUpdate));
	this->unschedule(schedule_selector(LiuHeGambleLayer::updatePeriod));
	//this->unschedule(schedule_selector(LiuHeGambleLayer::getSystemTime));

	NotificationCenter::getInstance()->removeObserver(this, "KuaNian");

	NotificationCenter::getInstance()->removeObserver(this, "QueryMoreGameRet");

	removeAllChildrenWithCleanup(true);
	TextureCache::getInstance()->removeAllTextures(); //释放到目前为止所有加载的图片
	TextureCache::getInstance()->removeUnusedTextures(); 
}

void LiuHeGambleLayer::setGameKind(CaiZhong kind)
{
	//初始化数据
	MyBetData::getInstance()->setCurCaiZhongId(kind);

	int last = MyBetData::getInstance()->getLastWanfaId();
	if(last == -1) last = tagLiuHe[1];

	this->setWanFaKind((LiuHeCaiKind)last);
	selectedItemTag = 1;

	for(int i = 0;i < MyBetData::getInstance()->getWanfaSize();i ++)
	{
		if(tagLiuHe[MyBetData::getInstance()->getWanfaAt(i)] == last)
		{
			selectedItemTag = MyBetData::getInstance()->getWanfaAt(i);
			break;
		}
	}

	gameKind = kind;

	//开奖结果详细
	EntityMgr::instance()->getDispatch()->SendPacketQueryMoreGameResult(gameKind);

	//余额
	EntityMgr::instance()->getDispatch()->SendPacketWithGetLastYue();
	//封单
	EntityMgr::instance()->getDispatch()->SendPacketWithGetLiuHeFenDan();

	//跨年
#if 1
	EntityMgr::instance()->getDispatch()->SendPacketWithGetKuaNianQiHao(gameKind);
	this->schedule(schedule_selector(LiuHeGambleLayer::updatePeriod), 3);		
#endif
	//更新
	this->schedule(schedule_selector(LiuHeGambleLayer::myUpdate), 1);

	gameTitle->setString(ConfigMgr::instance()->text("display_config.xml",String::createWithFormat("t%d", 20+gameKind)->getCString()));

	//初始化
	if(gameRule == NULL)
	{
		gameRule = new LiuHeCaiRule();
	}

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

void LiuHeGambleLayer::updatePeriod(float dt)
{	
	EntityMgr::instance()->getDispatch()->SendPacketWithGetLiuHeFenDan();

	EntityMgr::instance()->getDispatch()->SendPacketWithGetKuaNianQiHao(gameKind);
}

void LiuHeGambleLayer::refreshLayer()
{
	//更新余额
	EntityMgr::instance()->getDispatch()->SendPacketWithGetLastYue();		
}

void LiuHeGambleLayer::setWanFaKind(LiuHeCaiKind kind)
{
	wanFaKind = kind;	

	bool isNeedMove = betArea->getPositionY() == -300;

	if (betArea)
		betArea->removeFromParentAndCleanup(true);
	betArea = NULL;
	auto layer = (LayerColor *)this->getChildByTag(9527);
	if(layer == nullptr) return;

	betArea = LiuHeWuFen::create();
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

bool LiuHeGambleLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initData();
	this->initView();
	this->setKeypadEnabled(true);

	//监听
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LiuHeGambleLayer::onTouchBegan, this);
	listener->onTouchCancelled = CC_CALLBACK_2(LiuHeGambleLayer::onTouchCancelled, this);
	listener->onTouchEnded = CC_CALLBACK_2(LiuHeGambleLayer::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(LiuHeGambleLayer::onTouchMoved, this) ;

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void LiuHeGambleLayer::myUpdate(float dt)
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
void LiuHeGambleLayer::initData()
{

	n_t_kindid=NULL;			//玩法ID
	f_t_bonus = NULL;			//最小奖金数字
	f_t_bonusPercent = NULL;	//间距
	f_t_fandian = NULL;			//最大百分比

	selectedItemTag = 1;
	wanFaKind = LiuHeCaiKind::LiuHeCai_Tmdm;

	currentLotteryId = 0;
	currentTime = string("19:30");
	winSize = Director::getInstance()->getWinSize();

	kindLayerIsShow = false;
	kindLayer = nullptr;

	m_data = NULL;
	m_data = Array::create();
	m_data->retain();

	m_bIsOpenList = false;
}

//初始化界面
void LiuHeGambleLayer::initView()
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
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LiuHeGambleLayer::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setAnchorPoint(Vec2(0,1));
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	layer->addChild(pCloseButton,5);

	//顶部功能按钮
	Sprite *setupItemNormalImage = Sprite::createWithSpriteFrame(spriteFrame("room_03.png"));
	Sprite *setupItemSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("room_03.png"));
	MenuItemSprite *setupItem = MenuItemSprite::create(setupItemNormalImage,setupItemSelectedImage,CC_CALLBACK_1(LiuHeGambleLayer::setting,this));
	setupItem->setPosition(Vec2(SCREEN_WIDTH - 17 - setupItemNormalImage->getContentSize().width / 2, pCloseButton->getPositionY()));	
	//LabelTTF* moreItemLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t4700"), "", 30);
	//MenuItemLabel *lbItem = MenuItemLabel::create(moreItemLabel,CC_CALLBACK_1(LiuHeGambleLayer::jumpResult,this));
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
	moreItem = MenuItemSprite::create(moreNormalImage2,moreSelectedImage2,CC_CALLBACK_1(LiuHeGambleLayer::toMore,this));
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

	//底部背景不透明
	Sprite *bottomBk = Sprite::createWithSpriteFrame(spriteFrame("room_12_2.png"));
	bottomBk->setPosition(Vec2(SCREEN_WIDTH/2, 65));
	bottomBk->setScaleX(SCREEN_WIDTH / bottomBk->getContentSize().width);
	bottomBk->setScaleY(150/bottomBk->getContentSize().height);
	//bottomBk->setColor(ccc3(50,50,50));
	layer->addChild(bottomBk,2);

	int tempH = 30;

	auto wordColor = ccc3(0,0,0);
#ifdef VER_369
	wordColor = ccc3(255,255,255);
#endif

	//文字 投注多少，彩票余额多少
	TouZhuJinELabel = LabelTTF::create("", "", fontSize);
	TouZhuJinELabel->setPosition(Vec2(34,bottomBk->getPositionY() + tempH * 3 / 2));//winSize.width*0.03
	TouZhuJinELabel->setAnchorPoint(Vec2(0, 0.5f));
	TouZhuJinELabel->setFontFillColor(wordColor);
	layer->addChild(TouZhuJinELabel, 2);

	updateMoneyLabel();

	//新增 单注奖金 赔率
	beilvBkTTf  = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t40"), "", fontSize);
	beilvBkTTf->setAnchorPoint(Vec2(0,0.5f));
	beilvBkTTf->setPosition(Vec2(30, bottomBk->getPositionY() - tempH / 2));
	beilvBkTTf->setFontFillColor(wordColor);
	layer->addChild(beilvBkTTf, 3);

	//输入框
	fontSize = 30;

	//底部按钮 确定投注和选定
	Sprite *confirmNormalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_11.png"));//确认投注
	Sprite *confirmSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_12.png"));
	MenuItemSprite *confirmItem = MenuItemSprite::create(confirmNormalImage,confirmSelectedImage,CC_CALLBACK_1(LiuHeGambleLayer::toGamble,this));
	Sprite *deleteNormalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_111_2.png"));
	Sprite *deleteSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_112_2.png"));
	MenuItemSprite *deleteItem = MenuItemSprite::create(deleteNormalImage,deleteSelectedImage,CC_CALLBACK_1(LiuHeGambleLayer::toDelete,this));
	confirmItem->setPosition(Vec2(SCREEN_WIDTH - 32 - confirmSelectedImage->getContentSize().width / 2, bottomBk->getPositionY() - tempH / 2));
	confirmItem->setTag(9529);	
	Menu *bottomMenu = Menu::create(deleteItem,confirmItem,NULL);
	bottomMenu->setPosition(Vec2(SCREEN_WIDTH - 150,beilvBkTTf->getPositionY()));
	bottomMenu->alignItemsHorizontallyWithPadding(15);
	bottomMenu->setTag(9527);
	layer->addChild(bottomMenu,2);	

#ifdef VER_369
	bottomMenu->setPositionX(bottomMenu->getPositionX() - 50);	
#endif

	//分组选择 彩票选择模块
	betArea = LiuHeWuFen::create();
	layer->addChild(betArea,0);
	betArea->setBetKind(wanFaKind);

	//顶部掩体，向上滑动时到此为止就被遮住
	LayerColor* maskLayer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, 120);
	maskLayer->ignoreAnchorPointForPosition(false);
	maskLayer->setPosition(Vec2(winSize.width*0.5, SCREEN_HEIGHT - 162 - 74));
	layer->addChild(maskLayer, 1);

	//下划线
	Sprite *gap2 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap2->setAnchorPoint(Vec2(0,0.5));
	gap2->setPosition(Vec2(0, 1));
	maskLayer->addChild(gap2,2);

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
			if(m_bIsOpenList && listTable->getContentOffset().y < -460)
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

void LiuHeGambleLayer::updateLotteryId()
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
	wPeriod->retain();
	String *lotteryIdString;
	lotteryIdString = String::createWithFormat("%s%s%s",
		ConfigMgr::instance()->text("display_text.xml", "t70"), tempStr,
		ConfigMgr::instance()->text("display_text.xml", "t71"));
	lotteryIdLabel->setString(lotteryIdString->getCString());

	NotificationCenter::getInstance()->postNotification("GDselectedItemTagNumberCartFailed",String::create(tempStr));

	isRun = false;
}

void LiuHeGambleLayer::updateCurrentTime()
{
	String *headStr = NULL;
	if(currentLotteryId == 0)
	{
		headStr = String::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t1550"));
	} else
	{
		if(gameRule->IsFenDanDuration())//正在封单
		{
			headStr = String::create(STR_FDSJ);		
		} else
		{
			headStr = String::create(STR_XDSJ);
		}
	}

	currentTimeLabel->setString(String::createWithFormat("%s%s",headStr->getCString(), currentTime.c_str())->getCString());	
}

void LiuHeGambleLayer::touZhuRet(Object *obj)
{
	MyBetNumber::getInstance()->betOneEnd();
	
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
		beiShu = 0;
		//beilvInput->setText("0");	

		MovingLabelLayer* layer =MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t180"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}else if(returnType > 0 && returnType <= 18)
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
void LiuHeGambleLayer::setting(Object* obj)
{
	playButtonSound();

	return;

	LotteryHelpView *layer = LotteryHelpView::create();
	layer->setHelpId(gameKind);
	
	Scene *scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);
}

void LiuHeGambleLayer::jumpResult(Object* obj)
{	
	playButtonSound();	

	//跳转到开奖
	LotteryGameResultDetail *layer = LotteryGameResultDetail::create();
	Scene *scene = Scene::create();
	scene->addChild(layer);
	Director::getInstance()->pushScene(scene);	

	EntityMgr::instance()->getDispatch()->SendPacketQueryMoreGameResult(gameKind);
}

//bottom menu
void LiuHeGambleLayer::toDelete(Object* obj)
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

//middle menu
void LiuHeGambleLayer::toMore(Object* obj)
{
	playButtonSound();

	MenuItemSprite *moreItem = (MenuItemSprite *)obj;
	if(kindLayerIsShow == false){
		this->showKindView();
		
	}else{
		this->hideKindView();
	
	}
}

void LiuHeGambleLayer::showKindView()
{
	if(kindLayerIsShow) return;

	if(kindLayer)
	{
		kindLayer->removeFromParentAndCleanup(true);
		kindLayer = nullptr;
	}

	//彩种下拉框选择
	kindLayer = LiuHeKind::create();
	kindLayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	kindLayer->setPosition(Vec2(0,0));
	this->addChild(kindLayer, 2);

	LayerColor* layer = (LayerColor *)this->getChildByTag(9527);
	if(layer == nullptr) return;
	layer->setPosition(SCREEN_WIDTH - 100, 0);

	kindLayerIsShow = true;
}

void LiuHeGambleLayer::hideKindView()
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

void LiuHeGambleLayer::toGamble(Object* obj)
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
		if(gameRule->IsFenDanDuration())
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t8"),
				Vec2(winSize.width * 0.5,winSize.height * 0.5));
			this->addChild(layer, 255);
			MyBetNumber::getInstance()->clear();
			betArea->clearNumbers();
			betArea->cancelAllMenuSelect();
			return;
		}
		betArea->getBetNumber();
		int betZhuShu = betArea->getBetZhuShu(); 		
		beiShu = betArea->getBetBeiShu();		
		
		if(betZhuShu == 0){
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t198"),
				Vec2(winSize.width * 0.5, winSize.height * 0.5));
			this->addChild(layer, 255);
			return;
		}
		if(beiShu < 1)
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t1005"),
				Vec2(winSize.width * 0.5, winSize.height * 0.5));
			this->addChild(layer, 255);
			return;
		}

		if(beiShu > ZhangHuYuE)
		{
			MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t1006"),
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

		for(int i = 0;i < 50;i ++)
		{
			if(betArea->getBetBeiShuAt(i) == 0) continue;

			String* betNumber = betArea->getBetNumberAt(i);	
			int betBeiShu = betArea->getBetBeiShuAt(i);

			EntityMgr::instance()->getDispatch()->SendPacketWithTouZhuCQSSC(qishuStr, gameKind, wanFaKind,
				betNumber->getCString(), 1, betBeiShu, 0, 1, 0, 0);
		}
	}
	//清空数据
	MyBetNumber::getInstance()->clear();
	betArea->clearNumbers();
	betArea->cancelAllMenuSelect();
}

void LiuHeGambleLayer::getSystemTimeObject(Object *obj)
{
	BaseLayer::getSystemTimeObject(nullptr);

	//Dictionary *data = (Dictionary *)obj;
	//wYear = ((Integer *)data->objectForKey("wYear"))->getValue();
	//wMonth = ((Integer *)data->objectForKey("wMonth"))->getValue();
	//wDay = ((Integer *)data->objectForKey("wDay"))->getValue();
	//wHour = ((Integer *)data->objectForKey("wHour"))->getValue();
	//wMinute = ((Integer *)data->objectForKey("wMinute"))->getValue();
	//wSecond = ((Integer *)data->objectForKey("wSecond"))->getValue();
}

void LiuHeGambleLayer::toCustom(Object* obj)
{
	Menu* tempMenu = (Menu *)this->getChildByTag(9527);
	if(tempMenu == nullptr) return;
	MenuItemSprite *tempItem1 = (MenuItemSprite *)tempMenu->getChildByTag(9528);
	if(tempItem1) tempItem1->setVisible(false);
	tempItem1 = (MenuItemSprite *)tempMenu->getChildByTag(9529);
	if(tempItem1) tempItem1->setVisible(false);
	tempItem1 = (MenuItemSprite *)tempMenu->getChildByTag(9530);
	if(tempItem1) tempItem1->setVisible(true);
	tempItem1 = (MenuItemSprite *)tempMenu->getChildByTag(9531);
	if(tempItem1) tempItem1->setVisible(true);
	//放开
	for(int k = 0;k < 5;k ++)
	{		
		//编辑
		EditBox* dtChouMa = (EditBox* )this->getChildByTag(CHOUMA_BOX + k);
		if (dtChouMa)
		{
			dtChouMa->setEnabled(true);			
		}	
	}	
}

void LiuHeGambleLayer::toConfirm(Object* obj)
{
	toCancel(nullptr);

	//编辑框
	for(int k = 0;k < 5;k ++)
	{		
		//编辑
		EditBox* dtChouMa = (EditBox* )this->getChildByTag(CHOUMA_BOX + k);
		if (dtChouMa)
		{
			std::string tempStr = dtChouMa->getText();
			UserDefault::getInstance()->setStringForKey(String::createWithFormat("CHOUMA_%d", k)->getCString(),tempStr);
		}	
	}	
}

void LiuHeGambleLayer::toCancel(Object* obj)
{
	Menu* tempMenu = (Menu *)this->getChildByTag(9527);
	if(tempMenu == nullptr) return;
	MenuItemSprite *tempItem1 = (MenuItemSprite *)tempMenu->getChildByTag(9528);
	if(tempItem1) tempItem1->setVisible(true);
	tempItem1 = (MenuItemSprite *)tempMenu->getChildByTag(9529);
	if(tempItem1) tempItem1->setVisible(true);
	tempItem1 = (MenuItemSprite *)tempMenu->getChildByTag(9530);
	if(tempItem1) tempItem1->setVisible(false);
	tempItem1 = (MenuItemSprite *)tempMenu->getChildByTag(9531);
	if(tempItem1) tempItem1->setVisible(false);

	//放开
	for(int k = 0;k < 5;k ++)
	{		
		//编辑
		EditBox* dtChouMa = (EditBox* )this->getChildByTag(CHOUMA_BOX + k);
		if (dtChouMa)
		{
			dtChouMa->setEnabled(false);			
		}	
	}
}

void LiuHeGambleLayer::calculateCQSSCQiShu()
{

}

void LiuHeGambleLayer::onEnter()
{
	BaseLayer::onEnter();
}

void LiuHeGambleLayer::onExit()
{
	BaseLayer::onExit();
}

void LiuHeGambleLayer::back(Object *obj)
{
	playButtonSound();

	//菜单
	if(kindLayerIsShow)
	{
		hideKindView();
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

void LiuHeGambleLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void LiuHeGambleLayer::GetLastYueRet(Object *obj)
{
	Dictionary *dic = (Dictionary *)obj;
	Double *f_t_yue = (Double *)dic->objectForKey("f_t_yue");
	Double *f_t_dongjie = (Double *)dic->objectForKey("f_t_dongjie");
	Double *f_t_score = (Double *)dic->objectForKey("f_t_score");
	ZhangHuYuE =f_t_yue->getValue();

	updateMoneyLabel();
}

void LiuHeGambleLayer::updatePeiLv(Object *obj)
{
	String* strPeiLv = (String* )obj;
	//这里是赔率

	if(strPeiLv == nullptr) 
	{
		beilvBkTTf->setVisible(false);
		return;
	}
	//
	beilvBkTTf->setVisible(true);

	strPeiLv = String::createWithFormat("%s %s",ConfigMgr::instance()->text("display_DuangDong.xml", "t40"), strPeiLv->getCString());	
	beilvBkTTf->setString(strPeiLv->getCString());	
}

void LiuHeGambleLayer::updateTouZhu(Object *obj)
{
	//if(betArea == nullptr) return;
	//这个函数里有注数的计算
	//betArea->getBetNumber();

	//更新
	updateMoneyLabel();
}

void LiuHeGambleLayer::kuaNianExpect(Object *obj)
{
	Dictionary* data = (Dictionary *)obj;
	Integer* I_KindId = (Integer *)data->objectForKey("n_t_kindid");
	Integer* I_Ifts = (Integer *)data->objectForKey("c_t_ifts");
	String* S_Expect = (String *)data->objectForKey("s_t_expect");
	String* S_Time = (String *)data->objectForKey("s_t_shijian");

	//有可能是上一个返回的
	if(I_KindId->getValue() != gameKind) return;
	gameRule->setIsStopSell(I_Ifts->getValue());

}

void LiuHeGambleLayer::updateFenDan(Object *obj)
{
	Dictionary *data = (Dictionary *)obj;	

	if (szLotNum != NULL)
	{
		szLotNum->release();
		szLotNum = NULL;
	}
	if (szShijian != NULL)
	{
		szShijian->release();
		szShijian = NULL;
	}	

	Integer *mPeriod = (Integer *)data->objectForKey("n_t_qihao");
	Integer *wYear = (Integer *)data->objectForKey("wYear");
	Integer *wMonth = (Integer *)data->objectForKey("wMonth");
	Integer *wDay = (Integer *)data->objectForKey("wDay");
	Integer *wHour = (Integer *)data->objectForKey("wHour");
	Integer *wMinute = (Integer *)data->objectForKey("wMinute");
	Integer *wSecond = (Integer *)data->objectForKey("wSecond");
	Integer *wMilliseconds = (Integer *)data->objectForKey("wMilliseconds");
			
	if(mPeriod->getValue() == 0)
		return;

	LiuHeCaiRule::m_nextExpect = mPeriod->getValue();	
	LiuHeCaiRule::SetNextKjShj(wYear->getValue(),wMonth->getValue(),wDay->getValue(),wHour->getValue(),wMinute->getValue(),wSecond->getValue(),wMilliseconds->getValue());
}

void LiuHeGambleLayer::changeKind(Object *obj)
{
	selectedItemTag = ((Integer *)obj)->getValue(); 
	//保存
	MyBetData::getInstance()->setLastWanfaId(tagLiuHe[selectedItemTag]);
	//
	this->toMore(moreItem);
	//这里显示的是从T4001开始的名字
	//gambleTypeTitle->setString(ConfigMgr::instance()->text("display_text.xml", String::createWithFormat("t%d", 4000 +selectedItemTag)->getCString()));

	this->setWanFaKind(tagLiuHe[selectedItemTag]);
	betArea->clearNumbers();
	betArea->cancelAllMenuSelect();
	betZhuShuTouZhuJinE = 0;

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

//------------------------------------------------新增----------------------------------------------------------------
void LiuHeGambleLayer::editBoxEditingDidEnd(EditBox* editBox)
{
	string str = editBox->getText();
	beiShu = atoi(str.c_str());

	if(beiShu < 0)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t169"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
		//beilvInput->setText("0");
		beiShu = 0;
	}

	updateMoneyLabel();
}

/**触摸开始*/
bool LiuHeGambleLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{	
	Vec2 pos = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	startPoint = pos;

	for(int k = 0;k < 5;k ++)
	{
		Sprite* sptChouMa = (Sprite *)this->getChildByTag(CHOUMA_BUTTON + k);
		if(sptChouMa)
		{
			if(sptChouMa->getBoundingBox().containsPoint(pos))
			{
				EditBox* tempBox = (EditBox *)this->getChildByTag(CHOUMA_BOX + k);
				if(tempBox)
				{
					if(tempBox->isEnabled()) break;//正在编辑则退出

					int tempInt = atoi(tempBox->getText());	
				}
			}
		}
	}	
	
	return true;
}

void LiuHeGambleLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void LiuHeGambleLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{	

}

void LiuHeGambleLayer::updateMoneyLabel()
{	
	if(betArea == nullptr) return;
	//得到注数和倍数还有投注号码
	betArea->getBetNumber();

	betZhuShuTouZhuJinE = betArea->getBetZhuShu();	
	TouZhuJinE = betArea->getBetBeiShu();
	//显示投注金额，账户余额
	char szBuf[100] = { 0 };
	char szBuf1[100] = { 0 };
	char szBuf2[100] = { 0 };
	sprintf(szBuf, "%d ", betZhuShuTouZhuJinE);
	sprintf(szBuf1, "%.0f ", TouZhuJinE);
	sprintf(szBuf2, "%0.3f ", ZhangHuYuE);

	String *TouZhuJinEString = String::createWithFormat("%s%s%s%s  %s %s%s", szBuf,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t4"), szBuf1,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t2"),
		ConfigMgr::instance()->text("display_DuangDong.xml", "t3"), szBuf2,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t2"));

	TouZhuJinELabel->setString(TouZhuJinEString->getCString());	
}

void LiuHeGambleLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
	if(table->getTag() == 1000)
	{
		int id = cell->getTag() - 100;

		selectedItemTag = MyBetData::getInstance()->getWanfaAt(id);

		this->setWanFaKind(tagLiuHe[selectedItemTag]);
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

cocos2d::Size LiuHeGambleLayer::cellSizeForTable(TableView *table)
{
	cocos2d::Size result;

	if(table->getTag() == 8888)
	{
		result.width = SCREEN_WIDTH;
		result.height = 50;
	}else
		if(table->getTag() == 1000) //菜单
		{
			result.width = SCREEN_WIDTH / 4;
			result.height = 60;
		}

		return result;
}

TableViewCell* LiuHeGambleLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell* cell = TableViewCell::create();
	cell->setTag(idx + 100);
	cell->autorelease();

	if(table->getTag() == 1000) //菜单
	{
		if(MyBetData::getInstance()->getWanfaSize() <= idx) return cell;

		int nowPos =MyBetData::getInstance()->getWanfaAt(idx);
		const char* tempStr = ConfigMgr::instance()->text("display_6he.xml",String::createWithFormat("t%d",20 +  tagLiuHe[nowPos])->getCString());
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
			String *szTime  = (String*) m_data->objectAtIndex(idx*4+3);
			if(idx == 0)
			{
				if(LiuHeCaiRule::m_nextExpect == 0)
				{
					LiuHeCaiRule::m_nextExpect  = atoi(wPeriod->getCString()) +1;
					time_t today;
					today = time(NULL);
//					today += 24 * 3600 * 1; //后1天
					string strTime = szTime->getCString();
					struct tm *p = localtime(&today);
					p->tm_year = atoi(strTime.substr(0, 4).c_str()) - 1900;
					p->tm_mon = atoi(strTime.substr(5, 2).c_str()) -1;
					p->tm_mday = atoi(strTime.substr(8, 2).c_str());
					p->tm_hour = 21;
					p->tm_min  = 25;
					p->tm_min   = 0;
					LiuHeCaiRule::m_nextKjShj	= mktime(p) + 2*24*3600; //默认为 +2 day
				}
			}
			std::string result=ConfigMgr::instance()->text("display_text.xml", "t454");
			result.append(wPeriod->getCString());
			result.append(ConfigMgr::instance()->text("display_text.xml", "t455"));

			//第N期
			auto redColor = ccc3(61,8,40);
			LabelTTF *descibeLabel1 = LabelTTF::create(result.c_str(), "", 28);
			descibeLabel1->setColor(redColor);
			descibeLabel1->setAnchorPoint(Vec2(0,0.5f));
			descibeLabel1->setPosition(Vec2(32, 50/2 - 4));
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
			descibeLabel2->setPosition(Vec2(descibeLabel1->getContentSize().width + 100, 50/2 - 4));
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
				gap1->setScaleY(50 / gap1->getContentSize().height);
				cell->addChild(gap1, 0);
				gap1->setColor(clBackground);

#ifdef VER_369
				gap1->setOpacity(50);
#endif
			}
		}

		return cell;
}

ssize_t LiuHeGambleLayer::numberOfCellsInTableView(TableView *table)
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

void LiuHeGambleLayer::scrollViewDidScroll(ScrollView* view)
{

}

void LiuHeGambleLayer::scrollViewDidZoom(ScrollView* view)
{

}

void LiuHeGambleLayer::transferString(string &str, int nStep)
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

void LiuHeGambleLayer::queryGameMoreRet(Object *obj)
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