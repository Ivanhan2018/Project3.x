#ifndef __LotteryCardBinding__
#define __LotteryCardBinding__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotteryCardBinding: public Layer,  public TableViewDelegate,
	public TableViewDataSource
{

public:
	LotteryCardBinding();
	~LotteryCardBinding();
	CREATE_FUNC(LotteryCardBinding);
	virtual bool init();
	static Scene* scene();
	virtual void onEnter();
	virtual void onExit();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);	

	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t index);
	virtual ssize_t numberOfCellsInTableView(TableView *table);

	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);

private:
	void initData();
	void initView();		
	//top menu
	void back(Object *obj);

	void bangDing(Object *obj);
	void setQukuanZhanghaoRet(Object *obj);

	void selectBank(Object *obj);

	void queryMyYinHangRet(Object *obj);

	void queryBankTypeRet(Object *obj);

	cocos2d::Size winSize;
	Node *m_node;
	float posX1;
	float posX2;
	float gapY;
	float startY;
	string username;
	//EditBox *bankNameInput;
	LabelTTF *bankNameInput;
	EditBox *bankUserNameInput;
	EditBox *bankAccountInput;
	EditBox *passwordInput;
	Menu *bangDingbutton;
	LabelTTF *nameLabel;
	Sprite *nameInputBk;

	//添加下拉框
	Node *m_TableNode;
	Sprite *m_TableBk;
	TableView *m_Table;

	//下拉按钮
	Menu *btnMenu;

	//银行信息
	vector<std::string> bankNameVec;
};

#endif