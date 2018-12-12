#ifndef __TABLE_VIEW_LAYER__
#define __TABLE_VIEW_LAYER__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class TableViewLayer : public Layer,TableViewDataSource,TableViewDelegate 
{
public:
	virtual bool init();

	static Scene* scene();
	static TableViewLayer* tableViewLayerWith1();

	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);

	//触摸事件 计算的是点击的是那个子页
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	//每一项的高度和宽度
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	//生成列表的每一项内容
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	//一共多少项
	virtual ssize_t numberOfCellsInTableView(TableView *table);

	virtual cocos2d::Size tableCellSizeForIndex(TableView *table, ssize_t idx) {
		return cellSizeForTable(table);
	} 

	CREATE_FUNC(TableViewLayer);
protected:
private:
	void spriteClickCallBack(Node* pNode);
};

#endif //__TABLE_VIEW_LAYER__