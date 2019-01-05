#pragma once

#include "GlobalDef.h"
#include "CMD_Plaza.h"

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class FreeScrollLayer : public Layer, public TableViewDataSource, public TableViewDelegate
{
public:
	bool intFreeScrollLayerWith(int num,SDownloadGamePrize_Returnn* data);
	static FreeScrollLayer* FreeScrollLayerWith(int num,SDownloadGamePrize_Returnn* data);

	virtual void scrollViewDidScroll(ScrollView* view) {};
	virtual void scrollViewDidZoom(ScrollView* view) {}
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);

	FreeScrollLayer(void);
	~FreeScrollLayer(void);

	void onGetDownData(Object* obj);
private:
	int m_nType;
	int m_nMessageNum;

	SDownloadGamePrize_Returnn* m_Data;

};