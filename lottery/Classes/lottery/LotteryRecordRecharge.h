#ifndef __LotteryRecordRecharge__
#define __LotteryRecordRecharge__
#include "LotteryRecord.h"

class LotteryRecordRecharge:public LotteryRecord
{
	
public:
	LotteryRecordRecharge();
	~LotteryRecordRecharge();
	CREATE_FUNC(LotteryRecordRecharge);
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