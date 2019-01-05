#ifndef __LotteryRecordSubGambleStatics__
#define __LotteryRecordSubGambleStatics__
#include "LotteryRecord.h"

#define EDITBOX_TAG  100

class LotteryRecordSubGambleStatics:public LotteryRecord
{
	
public:
	LotteryRecordSubGambleStatics();
	~LotteryRecordSubGambleStatics();
	CREATE_FUNC(LotteryRecordSubGambleStatics);
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
    void getXJTZHLogCount(Object *obj);
    void getXJTZHLog(Object *obj);
};
#endif