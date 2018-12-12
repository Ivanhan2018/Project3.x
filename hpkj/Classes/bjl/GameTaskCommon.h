#ifndef		__COMMON_TASK__
#define		__COMMON_TASK__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "define.h"
#include <limits>
#include <vector>
using namespace std;

USING_NS_CC;
USING_NS_CC_EXT;

#define USE_TEST_TABLEVIEW

class GameTaskCommon:public Layer,public TableViewDataSource, public TableViewDelegate
{
public:
	CREATE_FUNC(GameTaskCommon);
	virtual bool init();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void onTouchCancelled(Touch *pTouch, Event *pEvent);
	virtual void onEnter();
	virtual void onExit();

	//--TableViewDataSource
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	//--TableViewDelegate
	virtual void tableCellTouched(TableView* table, TableViewCell* cell) ;
	virtual void scrollViewDidScroll(ScrollView* view){};
	virtual void scrollViewDidZoom(ScrollView* view){};
	void runOutOrIn(bool isVisible=true);

public:
	void setData(const vector<DWORD>&PlayerList);
	bool IsInApplyBankerList(DWORD dwUserID);
private:
	CC_SYNTHESIZE(bool,m_isOut,IsOut);
	Sprite*         m_taskBk;
	TableView*  m_tableView;
	static char      m_taskBUff[1024];
	int totalCount;
public:
	vector<DWORD> m_ApplyBankerList;// иов╞ап╠М
};

#endif