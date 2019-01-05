#ifndef __LotteryRecordSubMember__
#define __LotteryRecordSubMember__
#include "LotteryRecord.h"
#include "Define.h"
#define EDITBOX_TAG  100

class LotteryRecordSubMember:public LotteryRecord
{
	
public:
	LotteryRecordSubMember();
	~LotteryRecordSubMember();
	CREATE_FUNC(LotteryRecordSubMember);
	virtual bool init();
	static Scene* scene();
	
	void tableCellTouched(TableView* table, TableViewCell* cell);
	cocos2d::Size cellSizeForTable(TableView *table);
	TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	ssize_t numberOfCellsInTableView(TableView *table);
	void scrollViewDidScroll(ScrollView* view);
	void scrollViewDidZoom(ScrollView* view);	

	//Ë¢ÐÂÊý¾Ý
	void refreshLayer(Object *obj);
private:
	void initView();
    void sendPacket();
    void pressConfirm(Object *obj);
    bool isValidUserID(const char *text);

	void updateView(Object *obj);
    
    DWORD userId;

	void clear();
};
#endif