#ifndef _SCROLLADD_H_
#define _SCROLLADD_H_

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ScrollAdd: public Layer,TableViewDataSource,TableViewDelegate 
{
public:

	ScrollAdd();
	~ScrollAdd();
	virtual bool init();
	CREATE_FUNC(ScrollAdd);

	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);

	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);

	//触摸事件 计算的是点击的是那个子页
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	//每一项的高度和宽度
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	//生成列表的每一项内容
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	//一共多少项
	virtual ssize_t numberOfCellsInTableView(TableView *table);

	void onEnter();
	void onExit();
private:

	TableView* pView;
	cocos2d::Vector<String *> pData;
	Sprite* pSprite;

	Vec2 m_start_pos;
	int m_cur_index;

	void initData();
	void initView();

	void changeFrame(bool isLeft);
	void refresh(float delta);
};


#endif //_SCROLLADD_H_