#include "LotteryRecord.h"
#include "ConfigMgr.h"
#include "MovingLabelLayer.h"
#include "LotterySceneControl.h"
#include "EntityMgr.h"
#include "VersionControl.h"

using namespace CocosDenshion;

Scene* LotteryRecord::scene()
{
	Scene *scene = Scene::create();
	LotteryRecord *layer = LotteryRecord::create();
	scene->addChild(layer);
	return scene;
}

LotteryRecord::LotteryRecord()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecord::getLogCount), "GetLogCount", NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LotteryRecord::getLog), "GetLog", NULL);
	recordData = Array::create();
	recordData->retain();
}
LotteryRecord::~LotteryRecord()
{
	NotificationCenter::getInstance()->removeObserver(this, "GetLogCount");
	NotificationCenter::getInstance()->removeObserver(this, "GetLog");
	timeTable->release();
	if(recordData)
	{	
		recordData->removeAllObjects();
		recordData->release();
		recordData = nullptr;
	}
	recordTable->release();
}

//下级投注
void LotteryRecord::getLog(Object *obj)
{
	Array *data = (Array *)obj;
    recordData->removeAllObjects();
    if(data->count()>pageSize*20){
        for (int i = 0 ; i < pageSize*20 ; i++) {
            recordData->addObject(data->objectAtIndex(i));
        }
    }else{
        recordData->addObjectsFromArray(data);
    }
	recordTable->reloadData();
	this->refreshLogLabel();
	
	isQuerying = false;
}

void LotteryRecord::getLogCount(Object *obj)
{
	recordDataCount = ((Integer *)obj)->getValue();
    totalPage = recordDataCount%pageSize!=0?(recordDataCount/pageSize+1):recordDataCount/pageSize;
    if (totalPage>20) {
        totalPage = 20;
    }
	if(recordDataCount != 0){
		sendPacket();
	}
}

void LotteryRecord::overTime(float delta)
{
	if(isQuerying)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml","t999"),Vec2(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2));
		this->addChild(layer,20);
		//超时还原
		isQuerying = false;
	}
}

bool LotteryRecord::init()
{
	if (!Layer::init())
	{
		return false;
	}
	CCLOG("LotteryRecord::init");
	this->initData();
	this->initView();
	this->setKeypadEnabled(true);
	return true;
}

void LotteryRecord::initData()
{
	winSize = Director::getInstance()->getWinSize();
	timeTableNum = 7;
	timeTableCellHeight = 25;
	timeTableCellWidth = 136;
	selectIndex = 1;
	timeButtonPressed = false;

	recordTableSize = cocos2d::Size(winSize.width, winSize.height - 250);

	totalPage = 0;
	pageSize = 7;
	currentPage = 1;
	recordDataCount = 0;
	tableCellItemNum = 1;
	
	tableCellPosX1 = 200;
	tableCellPosX2 = winSize.width/2+tableCellPosX1;
	tableCellGapX = 5;
	tableCellItemHeight = 60;
    
	n_t_type = 0;
	userID = EntityMgr::instance()->getDispatch()->m_dwUserID;
	s_t_account = "";
	s_t_start = "";
	s_t_end = "";

	isQuerying = false;
}

void LotteryRecord::initView()
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

	//顶部返回按钮
	Sprite *pCloseNormalButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	Sprite *pCloseSelectButton = Sprite::createWithSpriteFrame(spriteFrame("regist_04.png"));
	MenuItemSprite *pCloseItemSprite = MenuItemSprite::create
		(pCloseNormalButton,pCloseSelectButton,CC_CALLBACK_1(LotteryRecord::back,this));
	Menu *pCloseButton = Menu::create(pCloseItemSprite,NULL);
	pCloseButton->setPosition(Vec2(34, SCREEN_HEIGHT - 105));
	this->addChild(pCloseButton,1);

	float fontSize = 38;
	ccColor3B blackColor = ccc3(76,76,76);

	title = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t686"), "", fontSize);
	title->setPosition(Vec2(SCREEN_WIDTH/2, pCloseButton->getPositionY()));
	title->setColor(ccc3(255,255,255));
	this->addChild(title, 1);

	Sprite *bottomBk = Sprite::createWithSpriteFrame(spriteFrame("room_12_2.png"));
	bottomBk->setPosition(Vec2(SCREEN_WIDTH/2, 40));
	bottomBk->setScaleX(SCREEN_WIDTH / bottomBk->getContentSize().width);
	bottomBk->setScaleY(80/bottomBk->getContentSize().height);
	//bottomBk->setColor(ccc3(50,50,50));
	this->addChild(bottomBk,1);

	Sprite *previousNormalImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_12.png"));
	Sprite *previousSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_13.png"));
	Sprite *nextNormalImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_14.png"));
	Sprite *nextSelectedImage = Sprite::createWithSpriteFrame(spriteFrame("my_lottery_15.png"));
	MenuItemSprite *nextItem = MenuItemSprite::create(nextNormalImage,nextSelectedImage,CC_CALLBACK_1(LotteryRecord::getNext,this));
	MenuItemSprite *previousItem = MenuItemSprite::create(previousNormalImage,previousSelectedImage,CC_CALLBACK_1(LotteryRecord::getPrevious,this));
	Menu *touZhuLogMenu = Menu::create(previousItem, nextItem, NULL);
	touZhuLogMenu->setPosition(Vec2(500,40));	
	touZhuLogMenu->alignItemsHorizontallyWithPadding(60);
	this->addChild(touZhuLogMenu,1);

	fontSize = 27;
	String *logCountStr = String::createWithFormat("%s%d%s  %s%d%s",ConfigMgr::instance()->text("display_text.xml", "t556"),
		1,ConfigMgr::instance()->text("display_text.xml", "t557"),ConfigMgr::instance()->text("display_text.xml", "t558"),
		1, ConfigMgr::instance()->text("display_text.xml", "t557"));

	logCountLabel = LabelTTF::create(logCountStr->getCString(), "", fontSize);
	logCountLabel->setAnchorPoint(Vec2(0,0.5f));
	logCountLabel->setPosition(Vec2(10, 40));
	logCountLabel->setColor(ccc3(255,255,255));
	this->addChild(logCountLabel, 1);


	Sprite *timeLabelBk = Sprite::createWithSpriteFrame(spriteFrame("lottery_record_03.png"));
	timeLabelBk->setAnchorPoint(Vec2(0.5f,0.5f));
	timeLabelBk->setPosition(Vec2(200, SCREEN_HEIGHT - 130));
	//this->addChild(timeLabelBk,1);

	Sprite *timeButtonNormal = Sprite::createWithSpriteFrame(spriteFrame("beting17.png"));
	Sprite *timeButtonSelect = Sprite::createWithSpriteFrame(spriteFrame("beting18.png"));
	timeButtonItem = MenuItemSprite::create(timeButtonNormal,timeButtonSelect,CC_CALLBACK_1(LotteryRecord::pressTimeButton,this));
	Menu *timeButton = Menu::create(timeButtonItem,NULL);
	timeButton->setPosition(Vec2(timeLabelBk->getPositionX()+timeLabelBk->getContentSize().width/2-timeButtonNormal->getContentSize().width/2,timeLabelBk->getPositionY()));	
	//timeButton->setPosition(Vec2(timeLabelBk->getPositionX()+timeLabelBk->getContentSize().width/2,timeLabelBk->getPositionY()));	
	//this->addChild(timeButton,2);

	fontSize = 27;
	LabelTTF *timeHintLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t701")
		,"",fontSize);
	timeHintLabel->setColor(blackColor);
	timeHintLabel->setAnchorPoint(Vec2(0,0.5f));
	timeHintLabel->setPosition(Vec2(20, timeLabelBk->getPositionY()));
	//this->addChild(timeHintLabel, 1);

	float timeLabelPosX = timeLabelBk->getPositionX()+timeLabelBk->getContentSize().width/2-
		timeButtonNormal->getContentSize().width-(timeLabelBk->getContentSize().width-timeButtonNormal->getContentSize().width)/2;
	timeLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", "t702")
		,"",fontSize);
	timeLabel->setColor(blackColor);
	timeLabel->setAnchorPoint(Vec2(0.5f,0.5f));
	timeLabel->setPosition(Vec2(timeLabelPosX, timeLabelBk->getPositionY()));
	//this->addChild(timeLabel, 1);

	tableViewSize.width = timeTableCellWidth;
	tableViewSize.height = timeTableCellHeight*timeTableNum;
	tableViewPos = Vec2(timeLabelBk->getPositionX()-timeLabelBk->getContentSize().width/2, 
		timeLabelBk->getPositionY()-timeLabelBk->getContentSize().height/2-tableViewSize.height);

	timeTableBk = Sprite::createWithSpriteFrame(spriteFrame("lottery_record_01.png"));
	timeTableBk->setAnchorPoint(Vec2(0,0));
	timeTableBk->setPosition(tableViewPos);
	this->addChild(timeTableBk,1);

	TimeTableHelper *helper = new TimeTableHelper(this);
	helper->setTableCellSize(timeTableCellWidth,timeTableCellHeight);
	helper->setTableNum(timeTableNum);

	timeTable = TableView::create(helper, tableViewSize);
	timeTable->setDelegate(helper);
	timeTable->setAnchorPoint(Vec2(0.5, 0.5));
	timeTable->setPosition(tableViewPos);
	timeTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	timeTable->setDirection(ScrollView::Direction::VERTICAL);
	timeTable->retain();
	this->addChild(timeTable, 2);
	hideTimeTable();
	
	Sprite *gap1 = Sprite::createWithSpriteFrame(spriteFrame("room_12.png"));
	gap1->setAnchorPoint(Vec2(0,0.5));
	gap1->setPosition(Vec2(0, SCREEN_HEIGHT - 160));
	gap1->setColor(ccc3(100,100,100));
	this->addChild(gap1,1);

	recordTable = TableView::create(this, recordTableSize);
	recordTable->setDelegate(this);
	recordTable->setAnchorPoint(Vec2(0.5, 0.5));
	recordTable->setPosition(Vec2(0,80));
	recordTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	recordTable->setDirection(ScrollView::Direction::VERTICAL);
	recordTable->retain();
	this->addChild(recordTable, 1);
}

void LotteryRecord::getNext(Object *obj)
{
	playButtonSound();

	if(recordData->count() <= 0){
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t720"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 2);
		return;
	}
	if(currentPage<totalPage){
		currentPage++;

		recordData->removeAllObjects();
		recordTable->reloadData();

		sendPacket();
	}else{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t721"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 2);
	}
}

void LotteryRecord::getPrevious(Object *obj)
{
	playButtonSound();

	if(recordData->count() <= 0){
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t720"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 2);
		return;
	}
	if(currentPage>1){
		currentPage--;

		recordData->removeAllObjects();
		recordTable->reloadData();

		sendPacket();
	}else{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml", "t722"),
			Vec2(winSize.width * 0.5,winSize.height * 0.5));
		this->addChild(layer, 2);
	}
}

void LotteryRecord::resetInfo()
{
    n_t_type = 0;
    userID = EntityMgr::instance()->getDispatch()->m_dwUserID;
    s_t_account = "";
}

bool LotteryRecord::checkIsQuerying(const bool querry)
{
	if (isQuerying || querry)
	{
		MovingLabelLayer* layer = MovingLabelLayer::MovingLabelLayerWith(ConfigMgr::instance()->text("display_text.xml","t998"),Vec2(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2));
		this->addChild(layer,20);
	}
	return isQuerying;
}

void LotteryRecord::refreshLogLabel()
{
	if(totalPage<1){
		return;
	}
	String *logCountStr = String::createWithFormat("%s%d%s  %s%d%s",ConfigMgr::instance()->text("display_text.xml", "t556"),
		currentPage,ConfigMgr::instance()->text("display_text.xml", "t557"),ConfigMgr::instance()->text("display_text.xml", "t558"),
		totalPage, ConfigMgr::instance()->text("display_text.xml", "t557"));
	logCountLabel->setString(logCountStr->getCString());
}

void LotteryRecord::sendPacket()
{
	isQuerying = true;
	//调用子类的方法
	this->scheduleOnce(schedule_selector(LotteryRecord::overTime), 10.0f);
}

void LotteryRecord::pressTimeButton(Object *obj)
{
	playButtonSound();

	MenuItemSprite *item = (MenuItemSprite *)obj;
	if(timeButtonPressed == true){
		item->unselected();
		timeButtonPressed = false;
		hideTimeTable();
	}else{
		item->selected();
		timeButtonPressed = true;
		showTimeTable();
	}
}

void LotteryRecord::onEnter()
{
	Layer::onEnter();
}

void LotteryRecord::onExit()
{
	Layer::onExit();
}

void LotteryRecord::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
		event->stopPropagation();
	}
}

void LotteryRecord::back(Object *obj)
{
	playButtonSound();
	Director::getInstance()->popScene();
}

void LotteryRecord::setTitle(int xmlIndex)
{
	title->setString(ConfigMgr::instance()->text("display_text.xml", 
		String::createWithFormat("t%d", xmlIndex)->getCString()));
}

void LotteryRecord::hideTimeTable()
{
	timeTableBk->setPositionX(2000);
	timeTable->setPositionX(2000);
}

void LotteryRecord::showTimeTable()
{
	timeTableBk->setPosition(tableViewPos);
	timeTable->setPosition(tableViewPos);
}

void LotteryRecord::setSelectedItem(int index)
{
	if(selectIndex != index){
		selectIndex = index;
		timeTable->reloadData();

		timeLabel->setString(ConfigMgr::instance()->text("display_text.xml",
			String::createWithFormat("t%d", 701+selectIndex)->getCString()));
	}

	timeButtonItem->unselected();
	timeButtonPressed = false;
	hideTimeTable();
}
void LotteryRecord::tableCellTouched(TableView* table, TableViewCell* cell)
{
	int cellTag = cell->getTag();
}

cocos2d::Size LotteryRecord::cellSizeForTable(TableView *table)
{
	return recordTableSize;
}

TableViewCell* LotteryRecord::tableCellAtIndex(TableView *table, ssize_t idx)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	cell->setTag(idx);

	return cell;
}

ssize_t LotteryRecord::numberOfCellsInTableView(TableView *table)
{
	return 0;
}

void LotteryRecord::scrollViewDidScroll(ScrollView* view)
{

}

void LotteryRecord::scrollViewDidZoom(ScrollView* view)
{

}

TimeTableHelper::TimeTableHelper(Node *fath)
{
	father = fath;
	selectIndex = 1;
}

TimeTableHelper::~TimeTableHelper()
{

}

void TimeTableHelper::tableCellTouched(TableView* table, TableViewCell* cell)
{
	int cellTag = cell->getTag();
	if(cellTag != 0){
		selectIndex = cellTag;
		LotteryRecord *lotteryRecord = static_cast<LotteryRecord *>(father);
		lotteryRecord->setSelectedItem(selectIndex);
	}
}

cocos2d::Size TimeTableHelper::cellSizeForTable(TableView *table)
{
	return cocos2d::Size(timeTableCellWidth, timeTableCellHeight);
}

TableViewCell* TimeTableHelper::tableCellAtIndex(TableView *table, ssize_t idx)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	cell->setTag(idx);

	ccColor3B redColor = ccc3(61,8,40);
	ccColor3B blackColor = ccc3(150,150,150);
	ccColor3B blackColor1 = ccc3(0,0,0);
	float fontSize = 27;
	float gap = 0;
	float textWidth = 600;
	LabelTTF *timeLabel = LabelTTF::create(ConfigMgr::instance()->text("display_text.xml", 
		String::createWithFormat("t%d", 701+idx)->getCString())
		,"",fontSize,cocos2d::Size(textWidth,0),kCCTextAlignmentLeft);
	timeLabel->setColor(blackColor1);
	timeLabel->setAnchorPoint(Vec2(0,0.5f));
	timeLabel->setPosition(Vec2(gap, timeTableCellHeight/2));
	cell->addChild(timeLabel, 1);

	if(selectIndex == idx){
		Sprite *more = Sprite::createWithSpriteFrame(spriteFrame("lottery_record_02.png"));
		more->setAnchorPoint(Vec2(0.5f,0.5f));
		more->setPosition(Vec2(timeTableCellWidth/2, timeTableCellHeight/2));
		cell->addChild(more);
	}

	return cell;
}

ssize_t TimeTableHelper::numberOfCellsInTableView(TableView *table)
{
	return timeTableNum;
}

void TimeTableHelper::scrollViewDidScroll(ScrollView* view)
{

}

void TimeTableHelper::scrollViewDidZoom(ScrollView* view)
{

}