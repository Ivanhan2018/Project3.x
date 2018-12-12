#ifndef __Game__MissionLayer__
#define __Game__MissionLayer__

#include "TabLayer.h"
#include "cocos-ext.h"
#include "cocos2d.h"
//#include "C2DXShareSDK.h"
#include "Define.h"
USING_NS_CC;
USING_NS_CC_EXT;
//using namespace cn::sharesdk;

enum MissionLayerTab
{
	MissionLayerTabEveryday,
	MissionLayerTabBeginner,
	MissionLayerTabScore,
	MissionLayerTabReference
};

class MissionLayer : public TabLayer, public TableViewDelegate,
	public TableViewDataSource
{
private:
	cocos2d::Size winSize;
	cocos2d::Size tableItemSize;
	float tableGapX;
	float tableGapY;
	MenuItemSprite* everydayItem;
	MenuItemSprite* beginnerItem;
	MenuItemSprite* awardItem;
	//ÿ������
	TableView *everydayTable;
	//��������
	TableView *beMissionTable;
	int beMissionNum;
public:
	MissionLayer();
	~MissionLayer();
	static Scene *scene();
	static MissionLayer *create();
	static MissionLayer *createWithTabItems(Array *itemNormalName, Array *itemSelectedName);
	void initView();
	virtual void doSomething();
	void reloadData(MissionLayerTab tag);
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);					//�ֻ����ذ�ť
	void dismiss(Object *obj);
	
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);

	//����ص�
	//void authResultHandler(C2DXResponseState state, C2DXPlatType platType, Dictionary *error);
	//void getUserResultHandler(C2DXResponseState state, C2DXPlatType platType, Dictionary *userInfo, Dictionary *error);
	//void shareResultHandler(C2DXResponseState state, C2DXPlatType platType, Dictionary *shareInfo, Dictionary *error);
};

class MissionTableHelper: public Object, public TableViewDelegate,
	public TableViewDataSource
{ 
private:
	MissionLayer *father;
	cocos2d::Size winSize;
	cocos2d::Size tableItemSize;
	int itemsNum;
	MissionLayerTab tableTag;
	string url;
	Array *field4ValueArray;
	bool cancelOrNot;
	DWORD totalCount;
	BYTE cbMemberOrder;
	LONG lScore;
public:
	MissionTableHelper(MissionLayer *fath, int num, MissionLayerTab tag);
	~MissionTableHelper();
	void cancelHttpRequest(Object *obj);
	void onHttpRequestCompleted(Node *sender, void *data);
	void addTableData(int field4);
	void pressGet(Object *obj);
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);
};
#endif  //__Game__WarningLayler__