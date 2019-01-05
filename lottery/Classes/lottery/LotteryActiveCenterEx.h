#ifndef __LotteryActiveCenterEx__
#define __LotteryActiveCenterEx__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "Define.h"
#include "BaseLayer.h"

#define ROWCOUNT 2

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

//用户中心
class LotteryActiveCenterEx: public BaseLayer, public TableViewDelegate,
	public TableViewDataSource
{

public:
	LotteryActiveCenterEx();
	~LotteryActiveCenterEx();
	CREATE_FUNC(LotteryActiveCenterEx);
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
	//void recharge(Object *obj);
	//void withdraw(Object* obj);
	//void transferToLottery(Object* obj);
	//void transferToQiPai(Object *obj);
    
    void loginOut(Object *obj);

	//void getLastYueRet(Object *obj);
	//void zhuanHuanRet(Object *obj);	
	//id exitReturn(Object *obj);

	//void getKeFuUrl(Object *obj);
	//String* kefuStr;

	cocos2d::Size winSize;
	//string username;
	//DOUBLE money;
	//DOUBLE qiPaiMoney;
	//float cellHeight;
	//float cellNum;
	//TableView *userInfoTable;
	void getActiveRet(Object* obj);
	void getActiveRetEx();
	Array* m_list;
	int getIdFromText(const char* name);

	void clear();
};

#endif