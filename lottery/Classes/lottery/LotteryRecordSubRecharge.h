#ifndef __LotteryRecordSubRecharge__
#define __LotteryRecordSubRecharge__
#include "LotteryRecord.h"

#define EDITBOX_TAG  100

class LotteryRecordSubRecharge:public LotteryRecord
{
	
public:
	LotteryRecordSubRecharge();
	~LotteryRecordSubRecharge();
	CREATE_FUNC(LotteryRecordSubRecharge);
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
    void getXJCZLogCountRet(Object *obj);
    void getXJCHZHLogRet(Object *obj);
    const char *getGameKindTitle(int gameType, int gameKind);
};
#endif