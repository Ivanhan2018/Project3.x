#include "ChoosePhotoScrollLayer.h"
#include "CocosUnits.h"
#include "CusTomTableViewCell.h"
#include "ChoosePhotoCell.h"

ChoosePhotoScrollLayer::ChoosePhotoScrollLayer(void)

{
}


ChoosePhotoScrollLayer::~ChoosePhotoScrollLayer(void)
{

}

ChoosePhotoScrollLayer* ChoosePhotoScrollLayer::PhotoScrollLayerWith(int type,int num)
{
	ChoosePhotoScrollLayer* view = new ChoosePhotoScrollLayer();
	if(view && view->intPhotoScrollLayerWith(type,num))
	{
		view->autorelease();
		return view;
	}
	CC_SAFE_DELETE(view);
	return NULL;
}

bool ChoosePhotoScrollLayer::intPhotoScrollLayerWith(int type,int num)
{
	if(!Layer::init())
	{
		return false;
	}
	m_nType = type;
	m_nMessageNum = num;

	cocos2d::Size winSize = Director::getInstance()->getWinSize();
	/**设置滑动操作可视区域的大小**/
	TableView* tableView = TableView::create(this, cocos2d::Size(723.0f, 156.0f));
	tableView->setDirection(TableView::Direction::HORIZONTAL);
	tableView->setPosition(Vec2(0,0));
	tableView->setDelegate(this);
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	this->addChild(tableView);
	tableView->reloadData();
	return true;
}

void ChoosePhotoScrollLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
	
	//CCLOG("%d",cell->getIdx());
}

/**设置图片与图片之间的间距**/
cocos2d::Size ChoosePhotoScrollLayer::cellSizeForTable(TableView *table)
{
	return cocos2d::Size(185.0f,140.0f);
}

TableViewCell* ChoosePhotoScrollLayer::tableCellAtIndex(TableView *table, ssize_t idx)
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
			nScorllType = nScorllType+idx + 100;
		}
		ChoosePhotoCell *sprite = ChoosePhotoCell::PhotoCellWith(nScorllType);
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setPosition(Vec2(0, 0));
		sprite->setTag(1234);
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
			nScorllType = nScorllType+idx + 100;
		}
		ChoosePhotoCell *sprite = (ChoosePhotoCell*)cell->getChildByTag(1234);
		sprite->setCellIdx(nScorllType);

	}


	return cell;

}

ssize_t ChoosePhotoScrollLayer::numberOfCellsInTableView(TableView *table)
{
	return m_nMessageNum;
}