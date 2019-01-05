//
//  PopFrame.cpp
//  Game
//
//  Created by 罗敏 on 13-8-26.
//
//
#include "EmailLayer.h"
#include "RoomLayer.h"
#include "JsonHelper.h"
#include "DBHandler.h"

EmailLayer::EmailLayer()
{
	myMsgTable = NULL;
	sysMsgTable = NULL;
	friendMsgTable = NULL;
}

EmailLayer::~EmailLayer()
{
	if(myMsgTable!=NULL){
		myMsgTable->release();
		myMsgTable = NULL;
	}
	if(sysMsgTable!=NULL){
		sysMsgTable->release();
		sysMsgTable = NULL;
	}
	if(friendMsgTable!=NULL){
		friendMsgTable->release();
		friendMsgTable = NULL;
	}
}

EmailLayer *EmailLayer::create()
{
	Array *normalName = Array::create();
	Array *selectedName = Array::create();
	normalName->addObject(String::create("Tab_MyMsg1.png"));
	selectedName->addObject(String::create("Tab_MyMsg2.png"));
	normalName->addObject(String::create("Tab_SysMsg1.png"));
	selectedName->addObject(String::create("Tab_SysMsg2.png"));
	//normalName->addObject(String::create("Tab_FriendMsg1.png"));
	//selectedName->addObject(String::create("Tab_FriendMsg2.png"));
	return EmailLayer::createWithTabItems(normalName, selectedName);
}

EmailLayer *EmailLayer::createWithTabItems(Array *itemNormalName, Array *itemSelectedName)
{
	EmailLayer *tabLayer = new EmailLayer();
	if (tabLayer && tabLayer->initWithTabItems(itemNormalName, itemSelectedName))
	{
		tabLayer->initView();
		tabLayer->autorelease();
		return tabLayer;
	}
	else
	{
		CC_SAFE_DELETE(tabLayer);
		return NULL;
	}
}

void EmailLayer::initView()
{
	float gapX = 11;
	float gapY = 10;
	winSize = Director::getInstance()->getWinSize();
	winSize.width -= gapX*2;

	cocos2d::Size tableViewSize;
	tableViewSize.width = winSize.width;
	tableViewSize.height = 388-gapY;

	//我的消息
	EmailTableHelper *myMsgHelper = new EmailTableHelper(this, 8, EmailTableTag_Personal);
	myMsgTable = TableView::create(myMsgHelper, tableViewSize);
	myMsgTable->setDelegate(myMsgHelper);
	myMsgTable->setAnchorPoint(Vec2(0.5, 0.5));
	myMsgTable->setPosition(Vec2(gapX, gapY));
	myMsgTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	myMsgTable->setDirection(ScrollView::Direction::VERTICAL);
	myMsgTable->retain();
	this->addChild(myMsgTable, 1);
	//系统消息
	EmailTableHelper *sysMsgHelper = new EmailTableHelper(this, 8, EmailTableTag_System);
	sysMsgTable = TableView::create(sysMsgHelper, tableViewSize);
	sysMsgTable->setDelegate(sysMsgHelper);
	sysMsgTable->setAnchorPoint(Vec2(0.5, 0.5));
	sysMsgTable->setPosition(Vec2(gapX, gapY));
	sysMsgTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	sysMsgTable->setDirection(ScrollView::Direction::VERTICAL);
	sysMsgTable->retain();
	this->addChild(sysMsgTable, 1);
	sysMsgTable->setVisible(false);
	//好友消息
	EmailTableHelper *friendMsgHelper = new EmailTableHelper(this, 8, EmailTableTag_Friend);
	friendMsgTable = TableView::create(new EmailTableHelper(this, 10, EmailTableTag_Friend), tableViewSize);
	friendMsgTable->setDelegate(friendMsgHelper);
	friendMsgTable->setAnchorPoint(Vec2(0.5, 0.5));
	friendMsgTable->setPosition(Vec2(gapX, gapY));
	friendMsgTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	friendMsgTable->setDirection(ScrollView::Direction::VERTICAL);
	friendMsgTable->retain();
	this->addChild(friendMsgTable, 1);
	friendMsgTable->setVisible(false);

	this->setKeypadEnabled(true);
	//this->setTouchEnabled(true);
	//setTouchEnabled(true);
}

void EmailLayer::doSomething()
{
	CCLOG("EmailLayer::doSomething");
	if(currentItem == TabLayerBaseTag){
		myMsgTable->setVisible(true);
		sysMsgTable->setVisible(false);
		friendMsgTable->setVisible(false);
	}else if(currentItem == TabLayerBaseTag+1){
		myMsgTable->setVisible(false);
		sysMsgTable->setVisible(true);
		friendMsgTable->setVisible(false);
	}else if(currentItem == TabLayerBaseTag+2){
		myMsgTable->setVisible(false);
		sysMsgTable->setVisible(false);
		friendMsgTable->setVisible(true);
	}
}

void EmailLayer::back(Object* obj)
{
	playButtonSound();
	Scene *pScene = Scene::create();
	RoomLayer *pRoomLayer = RoomLayer::create();
	pRoomLayer->automaticLogin(false);
	pRoomLayer->closeLoginAward();
	pScene->addChild(pRoomLayer);
	Director::getInstance()->replaceScene(pScene);
}

void EmailLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
	{
		back(NULL);
	}
}

void EmailLayer::reloadData(EmailTableTag tag)
{
	if(tag == EmailTableTag_Personal)
	{
		myMsgTable->reloadData();
	}else if(tag == EmailTableTag_System)
	{
		sysMsgTable->reloadData();
	}else if(tag == EmailTableTag_Friend)
	{
		friendMsgTable->reloadData();
	}
}

EmailTableHelper::EmailTableHelper(EmailLayer *fath, int num, EmailTableTag tag)
{
	float gapX = 11;
	float gapY = 10;
	father = fath;
	winSize = Director::getInstance()->getWinSize();
	winSize.width -= gapX*2;
	Sprite *tempSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board6.png"));
	tableItemSize.height = tempSprite->getContentSize().height;
	tableItemSize.width = winSize.width;
	itemsNum = num;
	tableTag = tag;
	msgArray = Array::create();
	msgArray->retain();
	char sql[400];
	string tableName;
	if(tableTag == EmailTableTag_Personal)
	{
		tableName = "PersonalMsg";
	}else if(tableTag == EmailTableTag_System)
	{
		tableName = "SystemMsg";
	}else if(tableTag == EmailTableTag_Friend)
	{
		tableName = "FriendMsg";
	}
	sprintf(sql, "select * from %s", tableName.c_str());
	DBHandler *handler = DBHandler::sharedDBHandler();
	sqlite3 *DB = handler->getDB();
	sqlite3_stmt *stmt;
	int rv = sqlite3_prepare(DB, sql, 100, &stmt, NULL);
	rv = sqlite3_step(stmt);
	char msg[1024] = {0};
	while(rv == SQLITE_ROW)
	{
		memset(msg, 0, sizeof(msg));
		memcpy(msg, sqlite3_column_text(stmt, 0), 1024);
		msgArray->addObject(String::create(msg));
		rv = sqlite3_step(stmt);
	}
	sqlite3_finalize(stmt);
}

EmailTableHelper::~EmailTableHelper()
{
	msgArray->release();
}

cocos2d::Size EmailTableHelper::cellSizeForTable(TableView *table)
{
	return tableItemSize;
}

/**表格中单元格的个数
参数：table单元格所属的表格
*/
ssize_t EmailTableHelper::numberOfCellsInTableView(TableView *table)
{
	return msgArray->count();
}

/**单元格初始化
参数：table单元格所属的表格
*/
TableViewCell *EmailTableHelper::tableCellAtIndex(TableView *table,ssize_t index)
{
	static const char * cellIdentifier = "cell-identifier";
	TableViewCell *cell = new TableViewCell();  
	cell->autorelease(); 
	//背景图
	Sprite *bk;
	bk = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Msg3.png"));
	bk->setAnchorPoint(Vec2(0,0));
	bk->setPosition(Vec2(8,0));
	cell->addChild(bk);
	//奖杯
	Sprite *awardSprite;
	if(index == 0){
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board1.png"));
	}else if(index == 1){
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board2.png"));
	}else if(index == 2){
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board3.png"));
	}else{
		awardSprite = Sprite::createWithSpriteFrame(spriteFrame("Sprite_Board4.png"));
	}
	awardSprite->setAnchorPoint(Vec2(0,0.5f));
	awardSprite->setPosition(Vec2(17,tableItemSize.height/2));
	//cell->addChild(awardSprite);

	LabelTTF *nameLabel = LabelTTF::create(((String *)msgArray->objectAtIndex(index))->getCString(), "", 30);
	nameLabel->setColor(ccc3(0,0,0));
	nameLabel->setAnchorPoint(Vec2(0,0.5));
	nameLabel->setPosition(Vec2(20, tableItemSize.height/2));
	cell->addChild(nameLabel, 1);
	return cell;  
}

/**点击单元格
参数：table，单元格所属的表格
cell，单元格
*/
void EmailTableHelper::tableCellTouched(TableView *table, TableViewCell *cell)
{
	//LabelTTF *nameLabel = (LabelTTF *)cell->getChildByTag(nameLabelTag);
	//const char *name = nameLabel->getString();
	//textField->setString(name);
	//this->removeChild(_tableView, true);
	//flag = 0;
}

void EmailTableHelper::scrollViewDidScroll(ScrollView* view)
{

}
void EmailTableHelper::scrollViewDidZoom(ScrollView* view)
{

}