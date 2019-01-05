#ifndef __LotterySubExchange__
#define __LotterySubExchange__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "BaseLayer.h"

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

#define EDITBOX_TAG_EXCHANGE  100

class LotterySubExchange: public BaseLayer, public TableViewDelegate,
	public TableViewDataSource
{

public:
	LotterySubExchange();
	~LotterySubExchange();
	CREATE_FUNC(LotterySubExchange);
	virtual bool init();
	static Scene* scene(const char* username,const int value);
	virtual void onEnter();
	virtual void onExit();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);

	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);

	//刷新数据
	void refreshLayer();

	//设置帐号id
	void setTargetID(const char* username,const int value);
private:
	void initData();
	void initView();

	//top menu
	void back(Object *obj);
	void pressConfirm(Object *obj);

	int fandianStatus;

	//
	void zhuanZhangRet(Object *obj);
	void getLastYueRet(Object *obj);
	void pressSetFandian(Object *obj);

	void onValidated(Object *obj);
	void getTransPhoneRet(Object *obj);

	void hideTable();
	void showTable();  

	cocos2d::Size winSize;
	float posX1;
	float posX2;
	float gapY;
	float startY;

	/**********/
	Node *m_TableNode;
	Sprite *fandianTableBk;
	TableView *fandianTable;
	cocos2d::Size tableViewSize;
	Vec2 tableViewPos;
	
	float tableCellWidth;
	float tableCellHeight;
	float tableCellNum;

	MenuItemSprite *ButtonItem;
	MenuItemSprite *timeButtonItem1;
	//目标id
	int m_targetID;
	int m_index;

	//手机号码
	std::string m_phoneNum;
	bool m_isNeedValidate;
};

#endif