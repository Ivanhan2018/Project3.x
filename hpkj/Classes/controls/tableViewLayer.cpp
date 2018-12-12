#include "TableViewLayer.h"
#include "CCTouchSprite.h"
#include "CocosUnits.h"
#include "GBEventIDs.h"
USING_NS_CC;
USING_NS_CC_EXT;

Scene* TableViewLayer::scene()
{
	Scene* se = Scene::create();

	TableViewLayer* layer = TableViewLayer::create();

	se->addChild(layer);
	return se;
}

TableViewLayer* TableViewLayer::tableViewLayerWith1()
{
	TableViewLayer* view = new TableViewLayer();
	if (view && view->init())
	{
		view->autorelease();
		return view;
	}
	return NULL;
}

bool TableViewLayer::init()
{
	bool isRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		cocos2d::Size winSize = Director::getInstance()->getWinSize();
		TableView* pTableView = TableView::create(this, cocos2d::Size(123 * 4 + 175, 114*2 + 20));
		pTableView->setDirection(TableView::Direction::HORIZONTAL);
		//pTableView->setViewLine();
		//pTableView->setViewOffset(Vec2(30,20));
		//pTableView->set
		pTableView->setPosition(Vec2(95,winSize.height / 5));
		pTableView->setDelegate(this);
		this->addChild(pTableView);
		pTableView->reloadData();

		isRet = true;
	} while (0);
	return isRet;
}

void TableViewLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
	//table->insertCellAtIndex(cell->getIdx());

	CCTouchSprite *label = (CCTouchSprite*)cell->getChildByTag(123);
	//CCLOG("cell m_uID at index: %i", label->m_uID);
}

cocos2d::Size TableViewLayer::cellSizeForTable(TableView *table)
{
	//return cocos2d::Size(table->getContentSize().width,114);
	return cocos2d::Size(123,114);
}

TableViewCell* TableViewLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
	String *string = String::createWithFormat("%d", idx);
	TableViewCell *cell = table->dequeueCell();
	if (!cell) 
	{
		cell = new TableViewCell();
		cell->autorelease();
		const char * str = String::createWithFormat("tubiao%d.png",idx)->getCString();

		//传入哪行哪列 回调函数
		CCTouchSprite* sprite=CCTouchSprite::createWithSpriteFrame(spriteFrame(str), idx, 0, this, callfuncN_selector(TableViewLayer::spriteClickCallBack));
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setPosition(Vec2(0, 0));
		//sprite->m_uID = idx;
		cell->addChild(sprite);
		sprite->setTag(123);
		//LabelTTF *label = LabelTTF::create(string->getCString(), "Helvetica", 20.0);
		//label->setPosition(Vec2::ZERO);
		//label->setAnchorPoint(Vec2::ZERO);
		//label->setTag(1234);
		//cell->addChild(label);
	}
	else
	{
		CCTouchSprite *sprite = (CCTouchSprite*)cell->getChildByTag(123);
		//sprite->m_uID = idx;

		//LabelTTF *label1 = (LabelTTF*)cell->getChildByTag(1234);
		//label1->setString(string->getCString());

		//CCLOG("m_uID = %s",string->getCString());
	}
	return cell;
}

void TableViewLayer::spriteClickCallBack(Node* pNode)
{
	CCTouchSprite* pTouchSprite = (CCTouchSprite*)pNode;
	bool ret = isVisible();
	if (!isVisible())
	{
		return;
	}
	//CCLOG("this is my click row is %d cell is %d, isdouble %d", pTouchSprite->getRowIndex(), pTouchSprite->getCellIndex(), pTouchSprite->isDoubleClick);
	//Object obj;
	//obj.m_uID = pTouchSprite->m_uID;
	NotificationCenter::getInstance()->postNotification(MSG_UI_ANS_CLICKTIPS,pTouchSprite);

	//CCLOG("pTouchSprite m_uID = %d",pTouchSprite->m_uID);
}

ssize_t TableViewLayer::numberOfCellsInTableView(TableView *table)
{
	return 8;
}

void TableViewLayer::scrollViewDidScroll(ScrollView* view)
{

}

void TableViewLayer::scrollViewDidZoom(ScrollView *view)
{

}