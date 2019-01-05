#ifndef __LotteryRecordProfit__
#define __LotteryRecordProfit__
#include "LotteryRecord.h"

class LotteryRecordProfit:public LotteryRecord
{
	
public:
	LotteryRecordProfit();
	~LotteryRecordProfit();
	CREATE_FUNC(LotteryRecordProfit);
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
	string username;
};
#endif