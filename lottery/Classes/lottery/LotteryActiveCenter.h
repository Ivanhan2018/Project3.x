#ifndef __LotteryActiveCenter__
#define __LotteryActiveCenter__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "Define.h"
#include "BaseLayer.h"
#include "VersionControl.h"

#ifdef VER_QIANBO
	#define ROWCOUNT 3
#endif
#ifdef VER_369
	#define ROWCOUNT 2
#endif
#ifdef VER_9YI
	#define ROWCOUNT 4
#endif


USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

class LotteryActiveCenter: public BaseLayer, public TableViewDelegate,
	public TableViewDataSource
{

public:
	LotteryActiveCenter();
	~LotteryActiveCenter();
	CREATE_FUNC(LotteryActiveCenter);
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

	//Ë¢ÐÂ
	void refreshLayer();
private:
	void initData();
	void initView();		
	//top menu
	void back(Object *obj);

	cocos2d::Size winSize;

	void getActiveRet(Object* obj);
	void getActiveRetEx();
	Array* m_list;
	int getIdFromText(const char* name);
	void clear();
};

#endif