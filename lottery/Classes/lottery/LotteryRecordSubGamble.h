#ifndef __LotteryRecordSubGamble__
#define __LotteryRecordSubGamble__
#include "LotteryRecord.h"

#define EDITBOX_TAG  100
#define HIDE_POS_XX 2000
#define NOW_POS_XX 628
#define HEIGHT_LISTX 600
#define HEIGHT_LIST_H 400
#define WIDTH_LISTX 152

//状态
const int status[] = {-1,2,3,1,5,0,4};

class LotteryRecordSubGamble:public LotteryRecord
{
	
public:
	LotteryRecordSubGamble();
	~LotteryRecordSubGamble();
	CREATE_FUNC(LotteryRecordSubGamble);
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
    void cHKXJTZHLogCountRet(Object *obj);
    void cHKXJTZHLog(Object *obj);
    const char *getGameKindTitle(int gameType, int gameKind);

	void toSelect(Object *obj);
	void toSelectStatus(Object *obj);

	int nowPosIndex;
	int nowStatusIndex;

	//和xml中对应
	std::vector<int> caizhong;
};
#endif