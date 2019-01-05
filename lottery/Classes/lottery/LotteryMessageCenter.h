#ifndef __LotteryMessageCenter__
#define __LotteryMessageCenter__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "Define.h"
#include "BaseLayer.h"

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotteryMessageCenter: public Layer, public TableViewDelegate,
	public TableViewDataSource
{

public:
	LotteryMessageCenter();
	~LotteryMessageCenter();
	CREATE_FUNC(LotteryMessageCenter);
	virtual bool init();
	static Scene* scene();
	virtual void onEnter();
	virtual void onExit();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);	

	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	virtual void scrollViewDidScroll(extension::ScrollView* view);
	virtual void scrollViewDidZoom(extension::ScrollView* view);	

	//刷新
	void refreshLayer();

	void displayIndex(const int index);
private:
	void initData();
	void initView();		
	//top menu
	void back(Object *obj);
	void toNotice(Object *obj);
	void toMail(Object *obj);
	//transaction menu
	void recharge(Object *obj);
	void withdraw(Object* obj);

	//公告返回
	void getNoticeRet(Object *obj);

	const char* getUtfString(const char* tempStr);

	cocos2d::Size winSize;
	string username;
	DOUBLE money;
	float cellHeight;
	float cellNum;
	//TableView *userInfoTable;

	//列表条数
	int row_count;
	cocos2d::Vector<String* > vec_content;
};

#endif