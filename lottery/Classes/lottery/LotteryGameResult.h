#ifndef __LotteryGameResult__
#define __LotteryGameResult__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "BaseLayer.h"

#define SETTING_ITEM_TAG   100

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class LotteryGameResult : public BaseLayer, public TableViewDelegate,
	public TableViewDataSource
{
public:
    LotteryGameResult();
    ~LotteryGameResult();
    CREATE_FUNC(LotteryGameResult);
    virtual bool init();
    static Scene* scene();
    virtual void onEnter();
    virtual void onExit();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);		
	
	//点击
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual cocos2d::Size cellSizeForTable(TableView *table);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);
	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);
	virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
	    /*该方法中设置抬起手后取消cell的高亮图片*/
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);

	//bool onTouchBegan(Touch *pTouch, Event *pEvent);	
	//virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
	void refreshLayer();

private:
	bool	m_bRefreshEnable;
	Vec2	m_pt_offset;
	cocos2d::Size winSize;
	Array *m_data;
	void initView();	
	float cellHeight;
	TableView *gameResultTable;
	Sprite *RefreshSprite; 

	float scrollLen;
	//Vec2 startPoint;
	//top menu
	void back(Object *obj);
    void queryGameRet(Object *obj);
    void transferString(string &str, int nStep);
    void Refresh(Object *obj);
	void ZhongJiangUpdate();
	void ZhongJiangTime();

    //menu
    void pressBuy(Object *obj);
    void pressMore(Object* obj);
	bool ScrollTest;
	bool SpeefTable;

	bool m_bLoadData;
	int m_needLoad;
	int m_iCount;

	void clear();
};

#endif // __HELLOWORLD_SCENE_H__