#ifndef __LotteryRecordSubGameProfit__
#define __LotteryRecordSubGameProfit__
#include "LotteryRecord.h"

#define EDITBOX_TAG  100

class LotteryRecordSubGameProfit:public LotteryRecord
{
	
public:
	LotteryRecordSubGameProfit();
	~LotteryRecordSubGameProfit();
	CREATE_FUNC(LotteryRecordSubGameProfit);
	virtual bool init();
	static Scene* scene();
	
	void tableCellTouched(TableView* table, TableViewCell* cell);
	cocos2d::Size cellSizeForTable(TableView *table);
	TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	ssize_t numberOfCellsInTableView(TableView *table);
	void scrollViewDidScroll(ScrollView* view);
	void scrollViewDidZoom(ScrollView* view);	

	void refreshLayer(Object *obj);
private:
	void initView();
    void sendPacket();
    void pressConfirm(Object *obj);
    void getXJYXTjRet(Object *obj);
    void getXJYXTjCountRet(Object *obj);
};
#endif