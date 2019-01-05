#include "bjpk10Cart.h"
#include "RoomLayer.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "PromptBox.h"
#include "BetLimit.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* bjpk10Cart::scene()
{
	Scene *scene = Scene::create();
	bjpk10Cart *layer = bjpk10Cart::create();
	scene->addChild(layer);
	return scene;
}
bjpk10Cart::bjpk10Cart()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(bjpk10Cart::ZhuiHaoback), "ZhuiHaoback", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(bjpk10Cart::SetusedBonusRet), "SetusedBonusRet", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(bjpk10Cart::pressChaseNumShow), "ChaseNumBerItem", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(bjpk10Cart::RemoveCurrentLayer), "RemoveCurrentLayer", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(bjpk10Cart::RefreshData), "RefreshData", NULL);
}
bjpk10Cart::~bjpk10Cart()
{
	if(gameResultTable)
	{
		gameResultTable->release();
		//gameResultTable = nullptr;
	}

	NotificationCenter::getInstance()->removeObserver(this, "ZhuiHaoback");
	NotificationCenter::getInstance()->removeObserver(this, "SetusedBonusRet");
	NotificationCenter::getInstance()->removeObserver(this, "RemoveCurrentLayer");
	NotificationCenter::getInstance()->removeObserver(this, "RefreshData");
	NotificationCenter::getInstance()->removeObserver(this, "ChaseNumBerItem");
}

bool bjpk10Cart::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initData();
	this->initView();
	this->setKeypadEnabled(true);
	//this->setTouchEnabled(true);
	return true;
}

void bjpk10Cart::initData()
{
	winSize = Director::getInstance()->getWinSize();
	cellHeight = 60;

	gameResultTable = nullptr;
	KindTTfle = nullptr;	
	lotteryIdLabel = nullptr;
	MoenyIdLabel = nullptr; //合计	
	ZhuiHao11Xuan5Cart = nullptr;
	confirmItem = nullptr;
	pCloseButton = nullptr;
	bottomMenu = nullptr;
	transactionMenu = nullptr;
}

void bjpk10Cart::initView()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(bjpk10Cart::onTouchBegan,this);
	listener->onTouchEnded = CC_CALLBACK_2(bjpk10Cart::onTouchEnded,this);
	listener->onTouchMoved = CC_CALLBACK_2(bjpk10Cart::onTouchMoved,this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	LayerColor* layer = LayerColor::create(ccc4(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT);
	layer->ignoreAnchorPointForPosition(false);
	layer->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));
	this->addChild(layer);

	////±≥æ∞
	Sprite *bk1 = Sprite::createWithSpriteFrame(spriteFrame(HEAD_IMG));
	bk1->setAnchorPoint(Vec2(0,1));
	bk1->setPosition(Vec2(0, SCREEN_HEIGHT));
	bk1->setScaleX(SCREEN_WIDTH / bk1->getContentSize().width);
	bk1->setScaleY(162 / bk1->getContentSize().height);
	this->addChild(bk1,1);

	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(bjpk10Cart::back,this));
	pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT-105));
	this->addChild(pCloseButton,1);

		//当前位置名称
	float fontSize = 38;
	LabelTTF *title = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml", "t176"), "", fontSize);	
	title->setPosition(Vec2(SCREEN_WIDTH / 2, pCloseButton->getPositionY() +5 ));
	title->setColor(ccc3(255, 255, 255));
	this->addChild(title, 1);

	//底部背景
	Sprite *bottomBk = Sprite::createWithSpriteFrame(spriteFrame("room_12_2.png"));
	bottomBk->setPosition(Vec2(SCREEN_WIDTH/2, 60));
	bottomBk->setScaleX(SCREEN_WIDTH / bottomBk->getContentSize().width);
	bottomBk->setScaleY(120/bottomBk->getContentSize().height);
	//bottomBk->setColor(ccc3(50,50,50));
	this->addChild(bottomBk,1);

	fontSize = 20;

	Sprite *confirmNormalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_11.png"));
	Sprite *confirmSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_12.png"));
	Sprite *ZhuiHaoNormalImage = Sprite::createWithSpriteFrame(spriteFrame("beting13.png"));
	Sprite *ZhuiHaoSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("beting14.png"));
	confirmItem = MenuItemSprite::create(confirmNormalImage,confirmSelectedImage,CC_CALLBACK_1(bjpk10Cart::confirm,this));
	MenuItemSprite *ZhuiHaoItem = MenuItemSprite::create(ZhuiHaoNormalImage,ZhuiHaoSelectedImage,CC_CALLBACK_1(bjpk10Cart::ZhuiHaoItemMinusobj,this));
	bottomMenu = Menu::create(confirmItem,ZhuiHaoItem,NULL);
	bottomMenu->setPosition(Vec2(winSize.width*0.5,bottomBk->getPositionY()));
	bottomMenu->alignItemsHorizontallyWithPadding(150);
	this->addChild(bottomMenu,1);

	fontSize = 30;
	KindTTfle = LabelTTF::create(ConfigMgr::instance()->text("display_DuangDong.xml","t301"), "", fontSize);
	KindTTfle->setPosition(Vec2(20, SCREEN_HEIGHT - bk1->getContentSize().height - 50));
	KindTTfle->setAnchorPoint(Vec2(0, 0.5f));
	KindTTfle->setColor(ccc3(0,0,0));
	this->addChild(KindTTfle, 1);
	KindTTfle->setVisible(false);

	Sprite *addRandomNormalImage = Sprite::createWithSpriteFrame(spriteFrame("beting01.png"));
	Sprite *addRandomSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("beting02.png"));
	MenuItemSprite *addRandomItem = MenuItemSprite::create(addRandomNormalImage,addRandomSelectedImage,CC_CALLBACK_1(bjpk10Cart::deleRandom,this));
	transactionMenu = Menu::create(addRandomItem,NULL);
	transactionMenu->setPosition(Vec2(winSize.width - 90, SCREEN_HEIGHT - bk1->getContentSize().height - 50));
	transactionMenu->alignItemsHorizontallyWithPadding(0);
	this->addChild(transactionMenu,1);

	//空白 中间背景 160
	Sprite *FangjuangSprite = Sprite::createWithSpriteFrame(spriteFrame("beting03.png"));
	FangjuangSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	FangjuangSprite->setPosition(Vec2(winSize.width*0.5, transactionMenu->getPositionY() - 50));
	FangjuangSprite->setScaleY((transactionMenu->getPositionY() - 50 - 175) / FangjuangSprite->getContentSize().height);	
	this->addChild(FangjuangSprite,1);

	//投注号码列表
	cocos2d::Size tableViewSize;	
	tableViewSize.width = winSize.width;
	tableViewSize.height = 850;
	gameResultTable = TableView::create(this, tableViewSize);
	gameResultTable->setDelegate(this);
	gameResultTable->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	gameResultTable->setPosition(Vec2(0, 175));//FangjuangSprite->getPositionY() * FangjuangSprite->getScaleY() - 2));
	gameResultTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	gameResultTable->setDirection(ScrollView::Direction::VERTICAL);
	gameResultTable->retain();
	this->addChild(gameResultTable, 2);

	lotteryIdLabel = LabelTTF::create(" ", "", fontSize);
	lotteryIdLabel->setAnchorPoint(Vec2(0,0.5f));	
	lotteryIdLabel->setPosition(Vec2(34, 145));
	lotteryIdLabel->setColor(ccc3(0,0,0));
	this->addChild(lotteryIdLabel, 2);
	MoenyIdLabel = LabelTTF::create(" ", "", fontSize);
	MoenyIdLabel->setAnchorPoint(Vec2(0,0.5f));
	MoenyIdLabel->setPosition(Vec2(34, lotteryIdLabel->getPositionY()));
	MoenyIdLabel->setColor(ccc3(0,0,0));
	this->addChild(MoenyIdLabel, 2);

	ZhuiHao11Xuan5Cart = bjpk10ChaseNumBetLayer::create();
	ZhuiHao11Xuan5Cart->setPosition(Vec2(1000,0));
	this->addChild(ZhuiHao11Xuan5Cart, 3);

	dissisChaseNum();
}

void bjpk10Cart::updateBetInfo()
{
	int totalZhuShu = 0;
	float totalMoney = 0;

	for(int i = 0 ; i < MyBetNumber::getInstance()->size(); i++)
	{
		Bet_Info tempInfo = MyBetNumber::getInstance()->getItem(i);

		Integer *zhushu = tempInfo.m_betCount;
		Integer *moshi = tempInfo.m_mode;
		Integer *beishu = tempInfo.m_betPlus;

		float temoMoshi = 0;
		if(moshi->getValue() ==0){temoMoshi =1;}
		else if(moshi->getValue() ==1){temoMoshi = 0.1f;}
		else if(moshi->getValue() ==2){temoMoshi = 0.01f;}
		else if(moshi->getValue() == 3){temoMoshi = 0.001f;}

		totalZhuShu += zhushu->getValue();
		totalMoney += tempInfo.getBetMoney();
	}	

	//投注金
	char szBuf[100] = {0}; 
	sprintf(szBuf, "%.3f ", totalMoney);
	//注数
	String* lotteryIdString = String::createWithFormat("%d %s",	totalZhuShu,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t183"));
	lotteryIdLabel->setString(lotteryIdString->getCString());

	String* MoenyIdString = String::createWithFormat("%s %s %s",
		ConfigMgr::instance()->text("display_DuangDong.xml", "t177"), szBuf/*totalZhuShu*atoi(beilvInput->getText())*2*/,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t178"));
	MoenyIdLabel->setString(MoenyIdString->getCString());

	MoenyIdLabel->setPositionX(lotteryIdLabel->getPositionX() + lotteryIdLabel->getContentSize().width + 30);

}
void bjpk10Cart::ZhuiHaoItemMinusobj(Object *obj)
{
	playButtonSound();

	bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
	if(!bRet)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t167"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}else{

		if(MyBetNumber::getInstance()->size() == 0) return;

		ZhuiHao11Xuan5Cart->setPosition(Vec2::ZERO);
		dissisChaseNum();
		this->setKeypadEnabled(true);				

		NotificationCenter::getInstance()->postNotification("TouZhuChaseNumBer");
	}
}
void bjpk10Cart::pressChaseNumShow(Object *obj)
{
	ShowIsChaseNum();
	this->setKeypadEnabled(true);
}


void bjpk10Cart::SetusedBonusRet(Object *obj)
{
	int lResult = ((Integer *)obj)->getValue();
	if(lResult ==0)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t163"),
		Vec2(winSize.width * 0.5,winSize.height * 0.5));
	   this->addChild(layer, 2);
	}else
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t164"),
		Vec2(winSize.width * 0.5,winSize.height * 0.5));
	   this->addChild(layer, 2);
	}

}

void bjpk10Cart::ZhuiHaoback(Object *obj)
{
	back(NULL);
}

void bjpk10Cart::back(Object *obj)
{
	playButtonSound();
	this->setPositionY(2000);
	NotificationCenter::getInstance()->postNotification("AddRandomItem");

}
void bjpk10Cart::ShowIsChaseNum()
{
	bottomMenu->setEnabled(true);
	transactionMenu->setEnabled(true);
	pCloseButton->setEnabled(true);
	if(gameResultTable)
		gameResultTable->setTouchEnabled(true);
}
void bjpk10Cart::dissisChaseNum()
{
	bottomMenu->setEnabled(false);
	transactionMenu->setEnabled(false);
	pCloseButton->setEnabled(false);
	if(gameResultTable) gameResultTable->setTouchEnabled(false);
}
//transaction menu
void bjpk10Cart::confirm(Object* obj)
{
	playButtonSound();

	bool bRet = EntityMgr::instance()->getDispatch()->connectLoginServer();
	if(!bRet)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_DuangDong.xml", "t167"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 255);
	}else{
		//dissisChaseNum();

		NotificationCenter::getInstance()->postNotification("CartConfirm", nullptr);
	}
}

void bjpk10Cart::bottomMenuEnabled(bool GuangDong11Xuan5BoolSuccen)
{
	if(GuangDong11Xuan5BoolSuccen == false)
	{
		Sprite *cartNormalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_12.png"));
		Sprite *cartSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_12.png"));
		confirmItem->setNormalImage(cartNormalImage);
		confirmItem->setSelectedImage(cartSelectedImage);

		dissisChaseNum();

	}else{
		Sprite *cartNormalImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_11.png"));
		Sprite *cartSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("gamble_11.png"));
		confirmItem->setNormalImage(cartNormalImage);
		confirmItem->setSelectedImage(cartSelectedImage);

		ShowIsChaseNum();
	}
}

void bjpk10Cart::deleRandom(Object* obj)
{
	playButtonSound();

	NotificationCenter::getInstance()->postNotification("ClearNumbers");
}

void bjpk10Cart::onEnter()
{
	Layer::onEnter();

}

void bjpk10Cart::onExit()
{
	Layer::onExit();
}

void bjpk10Cart::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	//if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	//{
	//	back(nullptr);
	//	event->stopPropagation();
	//}
}

cocos2d::Size bjpk10Cart::cellSizeForTable(TableView *table)
{
	cocos2d::Size size;
	size.width = winSize.width;
	size.height = 80;//cellHeight;
	return size;
}

ssize_t bjpk10Cart::numberOfCellsInTableView(TableView *table)
{
	return MyBetNumber::getInstance()->size() + 1;

}
TableViewCell *bjpk10Cart::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	cell->setTag(index);

	ccColor3B redColor = ccc3(61,8,40);
	ccColor3B blackColor = ccc3(0,0,0);
	float textWidth = 600;
	ccColor3B clBackground = ccc3(200, 200, 200);
	std::string backName = "room_12_2.png";


#ifdef VER_QIANBO
	clBackground = ccc3(68, 185, 110);
#endif

#ifdef VER_369
	clBackground = ccc3(50, 62, 123);

	backName.clear();
	backName.append("yx_03_02.png");

	if (index % 2 == 0)
	{
		backName.clear();
		backName.append("yx_03_01.png");
	}
#endif

	if(MyBetNumber::getInstance()->size() == 0 || MyBetNumber::getInstance()->size() == index) return cell;

	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame(backName.c_str()));
	gap1->setAnchorPoint(Vec2(0.5f, 0));
	//gap1->setOpacity(100);
	gap1->setPosition(Vec2(SCREEN_WIDTH / 2, 0));
	gap1->setScaleX((SCREEN_WIDTH - 40) / gap1->getContentSize().width);
	gap1->setScaleY(70 / gap1->getContentSize().height);
	cell->addChild(gap1, 0);

	if (index % 2 == 0)
	{
#ifndef VER_369
		gap1->setColor(clBackground);
#endif // !VER_369		
		redColor = ccc3(255,255,255);
	}else
	{
		redColor = ccc3(61,8,40);
	}

	string result = "";

	Bet_Info tempInfo = MyBetNumber::getInstance()->getItem(index); 

	Integer *wanFanKind = tempInfo.m_wanFaKind;
	Integer *wanGameKind = tempInfo.m_gameKind;
	String *betNumber = tempInfo.m_betNumber;
	Integer *betZhuShu = tempInfo.m_betCount;
	Integer *betMoShi = tempInfo.m_mode;
	Integer *betBeiShu = tempInfo.m_betPlus;

	result.append(betNumber->getCString());

	if(wanFanKind->getValue() == SSC_LongHu)
	{
		result = BetLimit::GetLongHuHeStr(result.c_str());		
	}
	if(wanFanKind->getValue() == DaXiaoDanShuang_DW  || wanFanKind->getValue() == Zonghe_DXDSLh || wanFanKind->getValue() == DaXiaoDanShuang) 
	{
		result = BetLimit::GetDXDSHStr(result.c_str());
	}
	//游戏名称
	LabelTTF *title  = LabelTTF::create(String::createWithFormat("[%s]",BetLimit::GetWanFaNameFromId(wanGameKind->getValue(), 
		wanFanKind->getValue()).c_str())->getCString(),"",27,cocos2d::Size(0,0),kCCTextAlignmentLeft);
	title->setPosition(Vec2(34,50));
	title->setAnchorPoint(Vec2(0,0.5f));
	title->setColor(redColor);
	cell->addChild(title);
	//投注号码
	LabelTTF *descibeLabel = LabelTTF::create(result.c_str(), "", 27, cocos2d::Size(textWidth - title->getContentSize().width,cellHeight / 2), kCCTextAlignmentLeft);
	descibeLabel->setColor(redColor);
	descibeLabel->setAnchorPoint(Vec2(0,0.5f));
	descibeLabel->setPosition(Vec2(title->getContentSize().width + 44, 50));	
	cell->addChild(descibeLabel, 1);

	//倍数
	String* beiShuString = String::createWithFormat("%d %s",betBeiShu->getValue(),ConfigMgr::instance()->text("display_DuangDong.xml", "t199"));

	//模式
	std::string moshiString = ConfigMgr::instance()->text("display_text.xml", String::createWithFormat("t%d", 4300 + betMoShi->getValue())->getCString());

	//统计金
	char szBuf[100] = {0}; 
	sprintf(szBuf, "%0.3f ", tempInfo.getBetMoney());
	//注数
	String* lotteryIdString = String::createWithFormat("%s %d %s",ConfigMgr::instance()->text("display_text.xml", "t166"), betZhuShu->getValue(),
		ConfigMgr::instance()->text("display_text.xml", "t165"));

	LabelTTF *ltfBeishu = LabelTTF::create(beiShuString->getCString(), "", 27, cocos2d::Size(0, 0), kCCTextAlignmentLeft);
	ltfBeishu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	ltfBeishu->setPosition(Vec2(34, 20));
	ltfBeishu->setColor(redColor);
	cell->addChild(ltfBeishu);

	LabelTTF *ltfMoshi = LabelTTF::create(moshiString.c_str(), "", 27, cocos2d::Size(0, 0), kCCTextAlignmentLeft);
	ltfMoshi->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	ltfMoshi->setPosition(Vec2(ltfBeishu->getPositionX() + ltfBeishu->getContentSize().width + 10, 20));
	ltfMoshi->setColor(redColor);
	cell->addChild(ltfMoshi);

	LabelTTF *ltfZhuShu = LabelTTF::create(lotteryIdString->getCString(), "", 27, cocos2d::Size(0, 0), kCCTextAlignmentLeft);
	ltfZhuShu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	ltfZhuShu->setPosition(Vec2(ltfMoshi->getPositionX() + ltfMoshi->getContentSize().width + 20, 20));
	ltfZhuShu->setColor(redColor);
	cell->addChild(ltfZhuShu);

	String* MoenyIdString = String::createWithFormat("%s %s",
		szBuf,
		ConfigMgr::instance()->text("display_DuangDong.xml", "t178"));
	LabelTTF *lftMoney = LabelTTF::create(MoenyIdString->getCString(), "", 27, cocos2d::Size(0, 0), kCCTextAlignmentLeft);
	lftMoney->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	lftMoney->setPosition(Vec2(ltfZhuShu->getPositionX() + ltfZhuShu->getContentSize().width + 10, 20));
	lftMoney->setColor(redColor);
	cell->addChild(lftMoney);

	//删除按钮
	Sprite *deleteItem = Sprite::createWithSpriteFrame(spriteFrame("gamble_15.png"));
	deleteItem->setPosition(Vec2(winSize.width - 50, 40));
	cell->addChild(deleteItem);
	deleteItem->setTag(DELETE_SPRITE_TAG);
	return cell;  
}

void bjpk10Cart::tableCellTouched(TableView *table, TableViewCell *cell)
{
	//if(cell->getTag() == MyBetNumber::getInstance()->size())
	//{
	//	NotificationCenter::getInstance()->postNotification("ClearNumbers");
	//	MyBetNumber::getInstance()->clear();
	//	gameResultTable->reloadData();
	//	this->updateBetInfo();
	//}
}
void bjpk10Cart::scrollViewDidScroll(ScrollView* view)
{

}
void bjpk10Cart::scrollViewDidZoom(ScrollView* view)
{

}

bool bjpk10Cart::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	if (this->getPositionX() == 2000 || this->getPositionY() == 2000) return false;

	Vec2 pos = Director::getInstance()->convertToGL(pTouch->getLocationInView());

	startPoint = pos;		

	return true;
}

void bjpk10Cart::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void bjpk10Cart::onTouchEnded(Touch *pTouch, Event *pEvent)
{	
	Vec2 pos = Director::getInstance()->convertToGL(pTouch->getLocationInView());
	endPoint = pos;
	if (ChongQingcartAbs(startPoint.x - endPoint.x) > 10 ||   
		ChongQingcartAbs(startPoint.y - endPoint.y) > 10)   
	{ 
		return;  
	}

	int index;
	for(index = 0 ; index <= MyBetNumber::getInstance()->size(); index++)
	{
		TableViewCell *selectedCell = gameResultTable->cellAtIndex(index);
		if(!selectedCell)
		{
			continue;
		}
		Sprite *sprite = (Sprite *)(selectedCell->getChildByTag(DELETE_SPRITE_TAG));
		if(sprite)
		{
			Rect rect = sprite->boundingBox();			
			if(rect.containsPoint(selectedCell->convertToNodeSpace(endPoint)))
			{
				this->pressDeleteButton(index);
			}
		}
	}
}

void bjpk10Cart::pressDeleteButton(int index)
{
	NotificationCenter::getInstance()->postNotification("DeleteItem", Integer::create(index));
}

void bjpk10Cart::RemoveCurrentLayer(Object *obj)
{
	this->setPositionY(2000);
	this->ShowIsChaseNum();	
}

void bjpk10Cart::RefreshData(Object *obj)
{
	if(!gameResultTable || gameResultTable->getPositionY() == 2000) return;

	gameResultTable->reloadData();
	this->updateBetInfo();
}