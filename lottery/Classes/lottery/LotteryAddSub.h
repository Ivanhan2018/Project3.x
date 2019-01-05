#ifndef __LotteryAddSub__
#define __LotteryAddSub__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "BaseLayer.h"

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

#define EDITBOX_TAG  100

class LotteryAddSub: public Layer, public TableViewDelegate,
	public TableViewDataSource
{

public:
	LotteryAddSub();
	~LotteryAddSub();
	CREATE_FUNC(LotteryAddSub);
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

	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void onTouchCancelled(Touch *pTouch, Event *pEvent);

	//刷新数据
	void refreshLayer();
private:
	void initData();
	void initView();
	bool isLegal(string str);
	void registerAddr(Object *obj);
	//top menu
	void back(Object *obj);
	void pressDaili(Object *obj);
	void pressMembership(Object *obj);
	void pressSetFirst(Object *obj);
	void pressSetSecond(Object *obj);
	void pressConfirm(Object *obj);
	void pressSetFandian(Object *obj);

	void CopyURL(Object *obj);

	void hideTable();
	void showTable(int posIndex);               //1 pos1 2 pos2

	//添加下级返回
	void addHuiYuanRet(Object *obj);
	//配额返回
	void peiERet(Object *obj);
	void fanDianRet(Object *obj);
	void webFanDian(Object *obj);

	void setPeiEString();

	cocos2d::Size winSize;
	float posX1;
	float posX2;
	float gapY;
	float startY;
	MenuItemImage *daili;
	MenuItemImage *membership;
	int m_cbType;
	//	EditBox *bankNameInput;

	/**********/
	Node *m_TableNode;
	Sprite *fandianTableBk;
	TableView *fandianTable;
	cocos2d::Size tableViewSize;
	Vec2 tableViewPos;

	//增加配额
	LabelTTF *ltfDispach;
	LabelTTF *ltfDispachTitle;
	
	float tableCellWidth;
	float tableCellHeight;
	float tableCellNum;
	Vec2 tableViewPos1;
	Vec2 tableViewPos2;
	MenuItemSprite *timeButtonItem;
	MenuItemSprite *timeButtonItem1;
	int fandianStatus;              //0，1，2分别表示不可见，点击设置网页返点，设置返点，默认为0
	int isDaili;                    //代理or会员            0会员1代理
	//增加配额
	int pere30;
	int pere28;
	int pere27;
	//
	int pere29;
	int pere26;

	//添加返点最大值
	float fanDianMax;
	std::string		m_strRegURL;
};

#endif