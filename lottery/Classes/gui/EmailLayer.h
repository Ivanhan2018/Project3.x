//
//  PopFrame.h
//  Game
//
//  Created by 罗敏 on 13-8-26.
//
//

#ifndef __Game__PopFrame__
#define __Game__PopFrame__

#include "TabLayer.h"
#include "cocos-ext.h"
#include "cocos2d.h"
USING_NS_CC;
USING_NS_CC_EXT;


enum EmailTableTag
{
	EmailTableTag_Personal,
	EmailTableTag_System,
	EmailTableTag_Friend
};

class EmailLayer : public TabLayer 
{
private:
	cocos2d::Size winSize;
	cocos2d::Size tableItemSize;
	//我的消息
	TableView *myMsgTable;

	//系统消息
	TableView *sysMsgTable;  

	//好友消息
	TableView *friendMsgTable;

public:
    EmailLayer();
    ~EmailLayer();
	static EmailLayer *create();
	static EmailLayer *createWithTabItems(Array *itemNormalName, Array *itemSelectedName);
    void initView();
	virtual void doSomething();
	void back(Object* obj);
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//手机返回按钮
	void reloadData(EmailTableTag tag);
};

class EmailTableHelper: public Object, public TableViewDelegate,
	public TableViewDataSource
{ 
private:
	EmailLayer *father;
	cocos2d::Size winSize;
	cocos2d::Size tableItemSize;
	int itemsNum;
	EmailTableTag tableTag;
	Array *msgArray;
public:
	EmailTableHelper(EmailLayer *fath, int num, EmailTableTag tag);
	~EmailTableHelper();
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);
};

#endif /* defined(__Game__PopFrame__) */
