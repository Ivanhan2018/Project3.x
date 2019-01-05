#ifndef __LotteryRecordSubProfit__
#define __LotteryRecordSubProfit__
#include "LotteryRecord.h"

#define EDITBOX_TAG  100
#define HIDE_POS_X 2000
#define NOW_POS_X 628
#define HEIGHT_LIST 600

class LotteryRecordSubProfit:public LotteryRecord
{
public:
	LotteryRecordSubProfit();
	~LotteryRecordSubProfit();
	CREATE_FUNC(LotteryRecordSubProfit);
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
	void toSelect(Object *obj);
	void getXJYKLogCountRet(Object *obj);
	void getXJYKLogRet(Object *obj);

	const char *getGameKindTitle(int gameType, int gameKind);

	int nowPosIndex;
};
#endif