//  [4/14/2014 wucan]
#ifndef		DDZ___COMMON_TASK__
#define		DDZ___COMMON_TASK__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <limits>
USING_NS_CC;
USING_NS_CC_EXT;

class DDZGameTaskCommon:public Layer,public TableViewDataSource, public TableViewDelegate
{
public:
	CREATE_FUNC(DDZGameTaskCommon);
	virtual bool init();
	virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void onTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	virtual void onEnter();
	virtual void onExit();

	//--CCTableViewDataSource
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	//--CCTableViewDelegate
	virtual void tableCellTouched(TableView* table,TableViewCell* cell) ;
	virtual void scrollViewDidScroll(ScrollView* view){};
	virtual void scrollViewDidZoom(ScrollView* view){};
	void runOutOrIn(bool isVisible=true);
private:
	CC_SYNTHESIZE(bool,m_isOut,IsOut);
	Sprite*         m_taskBk;
	TableView*  m_tableView;
	static char      m_taskBUff[1024];
	int totalCount;
};

#endif