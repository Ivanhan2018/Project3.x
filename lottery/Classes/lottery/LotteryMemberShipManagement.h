#ifndef __LotteryMemberShipManagement__
#define __LotteryMemberShipManagement__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "Define.h"
#include "BaseLayer.h"

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotteryMemberShipManagement: public BaseLayer, public TableViewDelegate,
	public TableViewDataSource
{

public:
	LotteryMemberShipManagement();
	~LotteryMemberShipManagement();
	CREATE_FUNC(LotteryMemberShipManagement);
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

	//Ë¢ÐÂ
	virtual void refreshLayer();
private:
	void initData();
	void initView();		
	//top menu
	void back(Object *obj);
	//transaction menu
	void recharge(Object *obj);
	void withdraw(Object* obj);

	cocos2d::Size winSize;
	string username;
	DOUBLE money;
	float cellHeight;
	float cellNum;
};

#endif