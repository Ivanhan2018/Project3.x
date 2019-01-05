#ifndef __LotteryRecord__
#define __LotteryRecord__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Define.h"
#include <string>
#include "BaseLayer.h"

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class TimeTableHelper:public Node, public TableViewDelegate,
	public TableViewDataSource
{
public:
	TimeTableHelper(Node *fath);
	~TimeTableHelper();
	void tableCellTouched(TableView* table, TableViewCell* cell);
	cocos2d::Size cellSizeForTable(TableView *table);
	TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	ssize_t numberOfCellsInTableView(TableView *table);
	void scrollViewDidScroll(ScrollView* view);
	void scrollViewDidZoom(ScrollView* view);	
	void setTableCellSize(float width, float height){timeTableCellWidth = width;timeTableCellHeight=height;};
	void setTableNum(int num){timeTableNum = num;};
private :
	Node *father;
	int selectIndex;
	float timeTableCellWidth;
	float timeTableCellHeight;
	Vec2 tableViewPos;
	int timeTableNum;
};


//查询列表的基类
//注意有些方法和属性，是大家公用的。
class LotteryRecord: public Layer, public TableViewDelegate,
	public TableViewDataSource
{

public:
	LotteryRecord();
	virtual ~LotteryRecord();
	CREATE_FUNC(LotteryRecord);
	bool init();
	static Scene* scene();
	virtual void onEnter();
	virtual void onExit();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);	
	void setSelectedItem(int index);

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
	void pressTimeButton(Object *obj);

	void getNext(Object *obj);
	void getPrevious(Object *obj);
	void getLog(Object *obj);				//下级投注
	void getLogCount(Object *obj);		//下级投注数量

	//超时
	void overTime(float delta);

	int timeTableNum;
	float timeTableCellWidth;
	float timeTableCellHeight;
	bool timeButtonPressed;
	Sprite *timeTableBk;
	MenuItemSprite *timeButtonItem;
	TableView *timeTable;
	cocos2d::Size tableViewSize;
	Vec2 tableViewPos;
protected:
	void hideTimeTable();
	void showTimeTable();
	void setTitle(int xmlIndex);
	virtual void sendPacket();
	void refreshLogLabel();
    void resetInfo();                           //重新设置n_t_type = 0,查询所有人
	bool checkIsQuerying(const bool querry = false);

	cocos2d::Size winSize;
	LabelTTF *title;
	LabelTTF *timeLabel;
	cocos2d::Size recordTableSize;
	int selectIndex;

	int recordDataCount;
	int currentPage;
	int totalPage;
	int pageSize;

	LabelTTF *logCountLabel;
	Array *recordData;
	TableView *recordTable;
	int tableCellItemNum;
	float tableCellPosX1;
	float tableCellPosX2;
	float tableCellGapX;
	float tableCellItemHeight;
    
	int n_t_type;
	DWORD userID;
	string s_t_account;
	string s_t_start;
	string s_t_end;
	//是不是正在查询
	bool isQuerying;
};

#endif