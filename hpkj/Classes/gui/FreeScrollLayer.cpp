#include "FreeScrollLayer.h"
#include "CocosUnits.h"
#include "CusTomTableViewCell.h"
#include "FreeCell.h"
#include "EntityMgr.h"

FreeScrollLayer::FreeScrollLayer(void)
{
	
}


FreeScrollLayer::~FreeScrollLayer(void)
{
	
}

FreeScrollLayer* FreeScrollLayer::FreeScrollLayerWith(int num,SDownloadGamePrize_Returnn* data)
{
	FreeScrollLayer* view = new FreeScrollLayer();
	if(view && view->intFreeScrollLayerWith(num,data))
	{
		view->autorelease();
		return view;
	}
	CC_SAFE_DELETE(view);
	return NULL;
}

bool FreeScrollLayer::intFreeScrollLayerWith(int num,SDownloadGamePrize_Returnn* data)
{
	if(!Layer::init())
	{
		return false;
	}
	m_nMessageNum = num;
	m_Data = data;

	cocos2d::Size winSize = WINSIZE;
	/**设置滑动操作可视区域的大小**/
	TableView* tableView = TableView::create(this, cocos2d::Size(808.0f, 384.0f));
	/**列表设置是横屏TableView::Direction::HORIZONTAL滑动还是竖屏ScrollView::Direction::VERTICAL滑动**/
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setPosition(Vec2(0,0));
	tableView->setDelegate(this);
	/**列表设置从大到小显示**/
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	this->addChild(tableView);
	tableView->reloadData();
	return true;
}

void FreeScrollLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{

	CCLOG("%d",cell->getIdx());
}

/**设置图片与图片之间的间距**/
cocos2d::Size FreeScrollLayer::cellSizeForTable(TableView *table)
{
	return cocos2d::Size(780.0f,93.0f);
}

TableViewCell* FreeScrollLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
	String *string = String::createWithFormat("%d", idx);
	TableViewCell *cell = table->dequeueCell();
	if (!cell)
	{
		cell = new CusTomTableViewCell();
		cell->autorelease();
		cell->setTag(idx);
	}
	else
	{
		cell->setTag(idx);
		cell->removeAllChildren();

		FreeCell *sprite = FreeCell::FreeCellWith(idx,m_Data->dwDownloadGameInfo >> idx & 1);
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setPosition(Vec2(0, 0));
		cell->addChild(sprite,1,1234);
		sprite->setCellIdx(idx,m_Data->dwDownloadGameInfo >> idx & 1);
	}


	return cell;

}

ssize_t FreeScrollLayer::numberOfCellsInTableView(TableView *table)
{
	return m_nMessageNum;
}

void FreeScrollLayer::onGetDownData(Object* obj)
{
	
}