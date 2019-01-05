#include "EmailScorllLayer.h"
#include "CocosUnits.h"
#include "CusTomTableViewCell.h"
#include "EmailCell.h"

EmailScorllLayer::EmailScorllLayer(void)
{
}


EmailScorllLayer::~EmailScorllLayer(void)
{

}

EmailScorllLayer* EmailScorllLayer::EmailScorllLayerWith(int type,int num)
{
	EmailScorllLayer* view = new EmailScorllLayer();
	if(view && view->intEmailScorllLayerWith(type,num))
	{
		view->autorelease();
		return view;
	}
	CC_SAFE_DELETE(view);
	return NULL;
}

bool EmailScorllLayer::intEmailScorllLayerWith(int type,int num)
{
	if(!Layer::init())
	{
		return false;
	}
	m_nType = type;
	m_nMessageNum = num;

	cocos2d::Size winSize = Director::getInstance()->getWinSize();

	TableView* tableView = TableView::create(this, cocos2d::Size(774, 355));
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setPosition(Vec2(0,0));
	tableView->setDelegate(this);
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	this->addChild(tableView);
	tableView->reloadData();

	return true;
}

void EmailScorllLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
	CCLOG("%d",cell->getIdx());
}

cocos2d::Size EmailScorllLayer::cellSizeForTable(TableView *table)
{
	return cocos2d::Size(774,116);
}

TableViewCell* EmailScorllLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
	String *string = String::createWithFormat("%d", idx);
	TableViewCell *cell = table->dequeueCell();
	if (!cell)
	{
		cell = new CusTomTableViewCell();
		int nScorllType = 0;
		if(m_nType == 0)
		{
			nScorllType = nScorllType+idx;
		}
		else if(m_nType == 1)
		{
			nScorllType = nScorllType+idx + 10;
		}
		EmailCell *sprite = EmailCell::EmailCellWith(nScorllType);
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setPosition(Vec2(0, 0));
		sprite->setTag(1234+idx);
		cell->addChild(sprite);
	}
	else
	{
		int nScorllType = 0;
		if(m_nType == 0)
		{
			nScorllType = nScorllType+idx;
		}
		else if(m_nType == 1)
		{
			nScorllType = nScorllType+idx + 10;
		}
		EmailCell *sprite = (EmailCell*)cell->getChildByTag(1234+idx);
		sprite->setCellIdx(nScorllType);
	}


	return cell;

}

ssize_t EmailScorllLayer::numberOfCellsInTableView(TableView *table)
{
	return m_nMessageNum;
}