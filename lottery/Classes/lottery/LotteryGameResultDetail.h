#ifndef __LotteryGameResultDetail__
#define __LotteryGameResultDetail__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

#define SETTING_ITEM_TAG   100

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class LotteryGameResultDetail : public Layer, public TableViewDelegate,
	public TableViewDataSource
{
public:
    LotteryGameResultDetail();
    ~LotteryGameResultDetail();
    CREATE_FUNC(LotteryGameResultDetail);
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
private:
	cocos2d::Size winSize;
	Array *data;
	void initView();	
	float cellHeight;
	TableView *gameResultTable;
	//top menu
	void back(Object *obj);
    void queryGameRet(Object *obj);
    void transferString(string &str, int nStep);
};

#endif // __HELLOWORLD_SCENE_H__