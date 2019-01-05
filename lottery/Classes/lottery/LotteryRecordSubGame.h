#ifndef __LotteryRecordSubGame__
#define __LotteryRecordSubGame__
#include "LotteryRecord.h"
#include "Define.h"

#define EDITBOX_TAG  100
#define HIDE_POS_XX 2000
#define NOW_POS_XX 628
#define HEIGHT_LISTX 400
#define HEIGHT_LIST_H 400
#define WIDTH_LISTX 152

class LotteryRecordSubGame:public LotteryRecord
{
	
public:
	LotteryRecordSubGame();
	~LotteryRecordSubGame();
	CREATE_FUNC(LotteryRecordSubGame);
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
	void getGameKindRet(Object *obj);

    bool isValidUserID(const char *text);

	void toSelect(Object *obj);    
  
	int m_sort_index;
	Array* m_arr_game;
};
#endif