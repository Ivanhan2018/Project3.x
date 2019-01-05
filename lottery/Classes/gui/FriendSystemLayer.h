#ifndef __Game__FriendSystemLayer__
#define __Game__FriendSystemLayer__

#include "TabLayer.h"
#include "cocos-ext.h"
#include "Define.h"
USING_NS_CC_EXT;

enum FriendSystemTableTag
{
	TableTag_FriendStatus = 1,
	TableTag_MyFriend
};

class FriendTableHelper:public TableViewDelegate,
	public TableViewDataSource
{ 
private:
	Array *friendStatusData;
public:
	void getFriendStatus();
	FriendTableHelper();
	~FriendTableHelper();
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);
};

class FriendSystemLayer : public TabLayer,public TableViewDelegate,
	public TableViewDataSource
{

private:
	cocos2d::Size winSize;
	//好友动态
	TableView *friendStatusTable;
	//我的好友
	int currentIndex;
	Array *friendData;
	TableView *myFriendTable;
	Sprite *myFriendBk;
	Menu *myFriendOperaMenu;
	Sprite *currentFriendBk;
	//添加好友
	LabelTTF *addFriendID;
	EditBox *friendAccount;
	Menu *addButton;

	//用户信息
	int friendIndex;
	int photoNum;
	cocos2d::Size tableViewCellSize;
	//Sprite *m_pPlayerPhoto;						//用户头像
	//Sprite *photoBk;								//头像背景
	LabelTTF *m_pUserName;						//用户名字
	LabelTTF *m_pUserSex;							//用户性别
	LabelTTF *pUserGold;							//用户金币
	LabelTTF *pUserRecord;						//用户战绩
	LabelTTF *pUserWinning;						//用户胜率
private:
	void initView();
	void getMyFriendData();
	void addMyFriendData(DWORD dwUserID);
public:
	FriendSystemLayer();
	~FriendSystemLayer();
	static FriendSystemLayer *create();
	static FriendSystemLayer *createWithTabItems(Array *itemNormalName, Array *itemSelectedName);
	virtual void doSomething();
	void sendButton(Object *obj);
	void deleteButton(Object *obj);
	void chatButton(Object *obj);
	void pressAddButton(Object *obj);
	void showFriendInfo(int index);

	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);


public:
};
#endif  //__Game__WarningLayler__