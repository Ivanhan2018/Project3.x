#ifndef __LotteryRecordProfitStatics__
#define __LotteryRecordProfitStatics__
#include "LotteryRecord.h"

class LotteryRecordProfitStatics:public LotteryRecord
{
	
public:
	LotteryRecordProfitStatics();
	~LotteryRecordProfitStatics();
	CREATE_FUNC(LotteryRecordProfitStatics);
	virtual bool init();
	static Scene* scene();
	
	void tableCellTouched(TableView* table, TableViewCell* cell);
	cocos2d::Size cellSizeForTable(TableView *table);
	TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	ssize_t numberOfCellsInTableView(TableView *table);
	void scrollViewDidScroll(ScrollView* view);
	void scrollViewDidZoom(ScrollView* view);	
private:
	void initView();
	void sendPacket();
};
#endif