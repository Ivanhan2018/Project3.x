#ifndef __LotteryMyLotRecordInfo__
#define __LotteryMyLotRecordInfo__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "BaseLayer.h"

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotteryMyLotRecordInfo : public BaseLayer, public TableViewDelegate,
	public TableViewDataSource
{
public:
	LotteryMyLotRecordInfo();
	~LotteryMyLotRecordInfo();
	CREATE_FUNC(LotteryMyLotRecordInfo);
	virtual bool init();
	static Scene* scene();
	virtual void onEnter();
	virtual void onExit();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);	
	void setTID(int id){n_t_id = id;};

	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);	
private:
	void initData();
	void initView();		
	//top menu
	void back(Object *obj);
	//
	void getTouZhuXXRet(Object *obj);
	void cancelTouZhuRet(Object *obj);
	void cheDan(Object *obj);
	void handleTouZhuHaoMa(string &originStr);

	int n_t_id;
	int n_gamekindid;
	string	str_qihao;
	float labelStartY;
	float gapY;
	float labelStartX1;
	float labelStartX2;
	float gapX;
	Sprite *contentBk;
	cocos2d::Size winSize;
	TableView *touZhuNumberTable;
	string touZhuHaoMaStr;
	int huanHandNum;
	Menu *cheDanButton;

	LabelTTF *zhuangTailabel[7];
};
#endif
