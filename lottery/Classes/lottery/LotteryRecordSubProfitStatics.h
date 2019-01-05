#ifndef __LotteryRecordSubProfitStatics__
#define __LotteryRecordSubProfitStatics__
#include "LotteryRecord.h"

#define EDITBOX_TAG  100
#define HIDE_POS_XX 2000
#define NOW_POS_XX 600
#define HEIGHT_LISTX 500
#define HEIGHT_LIST_H 400
#define WIDTH_LISTX 200

class LotteryRecordSubProfitStatics:public LotteryRecord
{
	
public:
	LotteryRecordSubProfitStatics();
	~LotteryRecordSubProfitStatics();
	CREATE_FUNC(LotteryRecordSubProfitStatics);
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
	int m_sort_index;
	void initView();
    void sendPacket();
    void pressConfirm(Object *obj);
    void getXJYKTjRet(Object *obj);
    void getXJYKTjCountRet(Object *obj);
	void getParentRelativeRet(Object *obj);

	void toSelect(Object *obj);

	void queryDataByUserName(const char* userName);
	void sortArray();
};
#endif