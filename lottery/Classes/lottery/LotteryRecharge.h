#ifndef __LotteryRecharge__
#define __LotteryRecharge__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotteryRecharge: public Layer, public TableViewDelegate,
	public TableViewDataSource
{

public:
	LotteryRecharge();
	~LotteryRecharge();
	CREATE_FUNC(LotteryRecharge);
	virtual bool init();
	static Scene* scene();
	virtual void onEnter();
	virtual void onExit();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);	

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

	void recharge(Object *obj);
	void back(Object *obj);

	//打开充值链接
	void openUrl(Dictionary		*obj);

	//充值类型数据
	void getChongzhiTypeRet(Object *obj);
	void getNeedRefresh(Object *obj);

	cocos2d::Size winSize;
	TableView *bankTable;
	EditBox *rechargeInput;
	int bankNum;
	float tableGap;
	float tableHeight;
	Array *bankInfo;

	int currentBank;
	Sprite *selectBankSprite;

	void clear();
};


#endif