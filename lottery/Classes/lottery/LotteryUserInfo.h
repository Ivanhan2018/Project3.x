#ifndef __LotteryUserInfo__
#define __LotteryUserInfo__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "Define.h"
#include "BaseLayer.h"

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

//用户中心
class LotteryUserInfo: public BaseLayer, public TableViewDelegate,
	public TableViewDataSource
{

public:
	LotteryUserInfo();
	~LotteryUserInfo();
	CREATE_FUNC(LotteryUserInfo);
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
	void contactKefu(Object* obj);	
	//top menu
	void back(Object *obj);
	//transaction menu
	void recharge(Object *obj);
	void withdraw(Object* obj);
	void transferToLottery(Object* obj);
	void transferToQiPai(Object *obj);
    
    void loginOut(Object *obj);

	void getLastYueRet(Object *obj);
	void zhuanHuanRet(Object *obj);	
	void exitReturn(Object *obj);

	void getKeFuUrl(Object *obj);
	String* kefuStr;

	cocos2d::Size winSize;
	string username;
	DOUBLE money;
	DOUBLE qiPaiMoney;
	float cellHeight;
	float cellNum;
	//TableView *userInfoTable;
	void clear();
};

#endif