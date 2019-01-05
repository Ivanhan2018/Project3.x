#ifndef __LotteryMyLot__
#define __LotteryMyLot__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "ChongQingWuFen.h"
#include "Define.h"
#include "BaseLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class LotteryMyLot : public Layer, public TableViewDelegate,
	public TableViewDataSource
{
public:
	LotteryMyLot();
	~LotteryMyLot();
	CREATE_FUNC(LotteryMyLot);
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

	void dissButtonRecord();
	void ShowButtonRecord();
private:
	void initData();
	void initView();		

	void contactKefu(Object* obj);
	//top menu
	void back(Object *obj);
	void setting(Object* obj);
	//transaction menu
	void recharge(Object *obj);
	void withdraw(Object* obj);
	void transferToLottery(Object* obj);
	void transferToQiPai(Object *obj);
	//record menu
	void betRecord(Object *obj);
	void winningRecord(Object *obj);
	void contiRecord(Object *obj);


	//get touzhu data
	void getTouZhuDataFromDatabase();
	const char *getGameKindTitle(int gameType, int gameKind);

	void getLastYueRet(Object *obj);

	//get touzhulogcout
	void getTouZhuLogCount(Object *obj);
	//send packet 
	void getNextTouZhuLog(Object *obj);
	void getPreviousTouZhuLog(Object *obj);
	//get touzhulog
	void getTouZhuLog(Object *obj);
	void refreshTouZhuLogLabel();
	void removeRecordInfoLayer(Object *obj);

	void zhuanHuanRet(Object *obj);
	void getKeFuUrl(Object *obj);

	bool showRecordInfo;
	cocos2d::Size winSize;
	string username;
	DOUBLE money;
	DOUBLE qiPaiMoney;
	LabelTTF *moneyLabel;
	LabelTTF *qiPaiMoneyLabel;

	//TableView *touZhuTable;
	float cellHeight;
	Array *touZhuData;
	int touZhuLogCount;
	int currentPage;
	int totalPage;
	int pageSize;
	LabelTTF *logCountLabel;

	Menu *pCloseButton;
	Menu *m_pSettingMenu;
	Menu *contactMenu;
	Menu *transactionMenu;
	Menu *recordMenu;
	Menu *touZhuLogMenu;

	//urstring
	String* kefuStr;

	void clear();
};

#endif // __HELLOWORLD_SCENE_H__