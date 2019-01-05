#ifndef __LotteryRecordWithdraw__
#define __LotteryRecordWithdraw__
#include "LotteryRecord.h"

class LotteryRecordWithdraw:public LotteryRecord
{

public:
	LotteryRecordWithdraw();
	~LotteryRecordWithdraw();
	CREATE_FUNC(LotteryRecordWithdraw);
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